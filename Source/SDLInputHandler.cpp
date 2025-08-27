#include "SDLInputHandler.h"
#include "SDLTbxInputCodeConverters.h"
#include <Tbx/Events/EventCoordinator.h>
#include <Tbx/Debug/Debugging.h>
#include <cstring>

namespace SDLInput
{
    /* ==== Lifetime ==== */

    static bool PumpSDLEventToHandler(void* inputHandler, SDL_Event* event)
    {
        ((SDLInputHandler*)inputHandler)->OnSDLEvent(event);
        return false;
    }

    SDLInputHandler::SDLInputHandler()
    {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);

        // Open all connected gamepads
        int numGamepads = 0;
        SDL_GetGamepads(&numGamepads);
        for (int i = 0; i < numGamepads; ++i)
        {
            if (SDL_IsGamepad(i))
            {
                _gamepads[i]= SDL_OpenGamepad(i);
            }
        }

        SDL_AddEventWatch(PumpSDLEventToHandler, nullptr);
    }

    SDLInputHandler::~SDLInputHandler()
    {
        for (auto& [id, gamepad] : _gamepads)
        {
            if (gamepad)
            {
                SDL_CloseGamepad(gamepad);
            }
        }
    }

    void SDLInputHandler::OnLoad()
    {
        TBX_ASSERT(SDL_Init(SDL_INIT_GAMEPAD) != 0, "Failed to initialize SDL");
        TBX_ASSERT(SDL_Init(SDL_INIT_HAPTIC) != 0, "Failed to initialize SDL");
        TBX_ASSERT(SDL_Init(SDL_INIT_SENSOR) != 0, "Failed to initialize SDL");
    }

    void SDLInputHandler::OnUnload()
    {
        SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
        SDL_QuitSubSystem(SDL_INIT_HAPTIC);
        SDL_QuitSubSystem(SDL_INIT_SENSOR);
    }

    void SDLInputHandler::Update()
    {
        // Store previous states
        _prevKeyState = _currKeyState;
        _prevMouseState = _currMouseState;
        _prevGamepadBtnState = _currGamepadBtnState;

        // Get latest states
        for (auto& [id, gamepad] : _gamepads)
        {
            if (!gamepad) continue;
            std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> buttons{};
            for (int b = 0; b < SDL_GAMEPAD_BUTTON_COUNT; ++b)
            {
                buttons[b] = SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(b));
            }
            _currGamepadBtnState[id] = buttons;
        }
        const bool* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState)
        {
            std::memcpy(_currKeyState.data(), keyboardState, sizeof(bool) * SDL_SCANCODE_COUNT);
        }
        _currMouseState = SDL_GetMouseState(nullptr, nullptr);
    }

    /* ==== Keyboard ==== */

    bool SDLInputHandler::IsKeyDown(const int keyCode) const
    {
        SDL_Scancode sc = ConvertKey(keyCode);
        return _currKeyState[sc] && !_prevKeyState[sc];
    }

    bool SDLInputHandler::IsKeyUp(const int keyCode) const
    {
        SDL_Scancode sc = ConvertKey(keyCode);
        return !_currKeyState[sc] && _prevKeyState[sc];
    }

    bool SDLInputHandler::IsKeyHeld(const int keyCode) const
    {
        SDL_Scancode sc = ConvertKey(keyCode);
        return _currKeyState[sc] && _prevKeyState[sc];
    }

    /* ==== Mouse ==== */

    bool SDLInputHandler::IsMouseButtonDown(const int button) const
    {
        int sdlBtn = ConvertMouseButton(button);
        bool curr = _currMouseState & SDL_BUTTON_MASK(sdlBtn);
        bool prev = _prevMouseState & SDL_BUTTON_MASK(sdlBtn);
        return curr && !prev;
    }

    bool SDLInputHandler::IsMouseButtonUp(const int button) const
    {
        int sdlBtn = ConvertMouseButton(button);
        bool curr = _currMouseState & SDL_BUTTON_MASK(sdlBtn);
        bool prev = _prevMouseState & SDL_BUTTON_MASK(sdlBtn);
        return !curr && prev;
    }

    bool SDLInputHandler::IsMouseButtonHeld(const int button) const
    {
        int sdlBtn = ConvertMouseButton(button);
        bool curr = _currMouseState & SDL_BUTTON_MASK(sdlBtn);
        bool prev = _prevMouseState & SDL_BUTTON_MASK(sdlBtn);
        return curr && prev;
    }

    Tbx::Vector2 SDLInputHandler::GetMousePosition() const
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return Tbx::Vector2(x, y);
    }

    /* ==== Gamepads ==== */

    bool SDLInputHandler::IsGamepadButtonDown(const int gamepadId, const int button) const
    {
        auto currIt = _currGamepadBtnState.find(gamepadId);
        auto prevIt = _prevGamepadBtnState.find(gamepadId);
        if (currIt == _currGamepadBtnState.end()) return false;
        int sdlBtn = ConvertGamepadButton(button);
        bool curr = currIt->second[sdlBtn];
        bool prev = (prevIt != _prevGamepadBtnState.end()) ? prevIt->second[sdlBtn] : false;
        return curr && !prev;
    }

    bool SDLInputHandler::IsGamepadButtonUp(const int gamepadId, const int button) const
    {
        auto currIt = _currGamepadBtnState.find(gamepadId);
        auto prevIt = _prevGamepadBtnState.find(gamepadId);
        if (currIt == _currGamepadBtnState.end()) return false;
        int sdlBtn = ConvertGamepadButton(button);
        bool curr = currIt->second[sdlBtn];
        bool prev = (prevIt != _prevGamepadBtnState.end()) ? prevIt->second[sdlBtn] : false;
        return !curr && prev;
    }

    bool SDLInputHandler::IsGamepadButtonHeld(const int gamepadId, const int button) const
    {
        auto currIt = _currGamepadBtnState.find(gamepadId);
        auto prevIt = _prevGamepadBtnState.find(gamepadId);
        if (currIt == _currGamepadBtnState.end()) return false;
        int sdlBtn = ConvertGamepadButton(button);
        bool curr = currIt->second[sdlBtn];
        bool prev = (prevIt != _prevGamepadBtnState.end()) ? prevIt->second[sdlBtn] : false;
        return curr && prev;
    }

    bool SDLInputHandler::OnSDLEvent(SDL_Event* event)
    {
        switch (event->type)
        {
            case SDL_EVENT_JOYSTICK_ADDED:
            {
                int numGamepads = 0;
                SDL_GetGamepads(&numGamepads);
                for (int i = 0; i < numGamepads; ++i)
                {
                    if (SDL_IsGamepad(i))
                    {
                        _gamepads[i] = SDL_OpenGamepad(i);
                    }
                }
                break;
            }
            case SDL_EVENT_JOYSTICK_REMOVED:
            {
                int id = event->jdevice.which;
                auto it = _gamepads.find(id);
                if (it != _gamepads.end() && it->second)
                {
                    SDL_CloseGamepad(it->second);
                    _gamepads.erase(it);
                    _currGamepadBtnState.erase(id);
                    _prevGamepadBtnState.erase(id);
                }
                break;
            }
            default:
                break;
        }

        return false;
    }
}
