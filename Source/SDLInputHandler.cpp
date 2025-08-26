#include "SDLInputHandler.h"
#include "SDLTbxInputCodeConverters.h"
#include <Tbx/Events/EventCoordinator.h>
#include <Tbx/Debug/Debugging.h>

namespace SDLInput
{
    /* ==== Initialization ==== */

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

    void SDLInputHandler::SetContext(const std::shared_ptr<Tbx::IWindow>& windowToListenTo)
    {
        // Do nothing...
    }

    /* ==== Keyboard ==== */

    bool SDLInputHandler::IsKeyDown(const int keyCode) const
    {
        const auto* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState == nullptr) return false;
        SDL_Scancode sc = ConvertKey(keyCode);
        return keyboardState[sc];
    }

    bool SDLInputHandler::IsKeyUp(const int keyCode) const
    {
        const auto* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState == nullptr) return false;
        SDL_Scancode sc = ConvertKey(keyCode);
        return !keyboardState[sc];
    }

    bool SDLInputHandler::IsKeyHeld(const int keyCode) const
    {
        const auto* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState == nullptr) return false;
        SDL_Scancode sc = ConvertKey(keyCode);
        return keyboardState[sc];
    }

    /* ==== Mouse ==== */

    bool SDLInputHandler::IsMouseButtonDown(const int button) const
    {
        Uint32 mask = SDL_GetMouseState(nullptr, nullptr);
        int sdlBtn = ConvertMouseButton(button);
        return (mask & SDL_BUTTON_MASK(sdlBtn));
    }

    bool SDLInputHandler::IsMouseButtonUp(const int button) const
    {
        Uint32 mask = SDL_GetMouseState(nullptr, nullptr);
        int sdlBtn = ConvertMouseButton(button);
        return !(mask & SDL_BUTTON_MASK(sdlBtn));
    }

    bool SDLInputHandler::IsMouseButtonHeld(const int button) const
    {
        Uint32 mask = SDL_GetMouseState(nullptr, nullptr);
        int sdlBtn = ConvertMouseButton(button);
        return (mask & SDL_BUTTON_MASK(sdlBtn));
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
        auto it = _gamepads.find(gamepadId);
        if (it == _gamepads.end() || !it->second) return false;

        int sdlBtn = ConvertGamepadButton(button);
        bool curr = SDL_GetGamepadButton(it->second, (SDL_GamepadButton)sdlBtn);
        return curr;
    }

    bool SDLInputHandler::IsGamepadButtonUp(const int gamepadId, const int button) const
    {
        auto it = _gamepads.find(gamepadId);
        if (it == _gamepads.end() || !it->second) return false;

        int sdlBtn = ConvertGamepadButton(button);
        bool curr = SDL_GetGamepadButton(it->second, (SDL_GamepadButton)sdlBtn);
        return !curr;
    }

    bool SDLInputHandler::IsGamepadButtonHeld(const int gamepadId, const int button) const
    {
        auto it = _gamepads.find(gamepadId);
        if (it == _gamepads.end() || !it->second) return false;

        int sdlBtn = ConvertGamepadButton(button);
        return SDL_GetGamepadButton(it->second, (SDL_GamepadButton)sdlBtn);
    }
}
