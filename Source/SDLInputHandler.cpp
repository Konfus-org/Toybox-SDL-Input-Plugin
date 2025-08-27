#include "SDLInputHandler.h"
#include "SDLTbxInputCodeConverters.h"
#include <Tbx/Events/EventCoordinator.h>
#include <Tbx/Debug/Debugging.h>

namespace SDLInput
{
    /* ==== Lifetime ==== */

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

    void SDLInputHandler::Update()
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_KEY_DOWN:
                {
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION:
                {
                    break;
                }
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
                    int id = event.jdevice.which;
                    auto it = _gamepads.find(id);
                    if (it != _gamepads.end() && it->second)
                    {
                        SDL_CloseGamepad(it->second);
                        it->second = nullptr;
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_AXIS_MOTION:
                {
                    int id = event.jaxis.which;
                    auto it = _gamepads.find(id);
                    if (it != _gamepads.end() && it->second)
                    {
                        int axis = event.jaxis.axis;
                        float value = event.jaxis.value;
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_BALL_MOTION:
                {
                    int id = event.jball.which;
                    auto it = _gamepads.find(id);
                    if (it != _gamepads.end() && it->second)
                    {
                        int ball = event.jball.ball;
                        int xrel = event.jball.xrel;
                        int yrel = event.jball.yrel;
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_HAT_MOTION:
                {
                    int id = event.jhat.which;
                    auto it = _gamepads.find(id);
                    if (it != _gamepads.end() && it->second)
                    {
                        int hat = event.jhat.hat;
                        int value = event.jhat.value;
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
                {
                    int id = event.jbutton.which;
                    auto it = _gamepads.find(id);
                    if (it != _gamepads.end() && it->second)
                    {
                        int button = event.jbutton.button;
                        bool curr = SDL_GetGamepadButton(it->second, (SDL_GamepadButton)button);
                        if (curr)
                        {
                        }
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_BUTTON_UP:
                {
                    int id = event.jbutton.which;
                    auto it = _gamepads.find(id);
                    if (it != _gamepads.end() && it->second)
                    {
                        int button = event.jbutton.button;
                        bool curr = SDL_GetGamepadButton(it->second, (SDL_GamepadButton)button);
                        if (!curr)
                        {
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
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
