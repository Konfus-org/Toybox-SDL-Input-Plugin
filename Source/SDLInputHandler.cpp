#include "SDLInputHandler.h"
#include <Tbx/Debug/Debugging.h>

namespace SDLInput
{
    void SDLInputHandler::OnLoad()
    {
        TBX_ASSERT(SDL_Init(SDL_INIT_EVENTS) != 0, "Failed to initialize SDL");
        TBX_ASSERT(SDL_Init(SDL_INIT_GAMEPAD) != 0, "Failed to initialize SDL");
        TBX_ASSERT(SDL_Init(SDL_INIT_HAPTIC) != 0, "Failed to initialize SDL");
        TBX_ASSERT(SDL_Init(SDL_INIT_SENSOR) != 0, "Failed to initialize SDL");
    }

    void SDLInputHandler::OnUnload()
    {
    }

    void SDLInputHandler::SetContext(const std::shared_ptr<Tbx::IWindow>& windowToListenTo)
    {
    }

    bool SDLInputHandler::IsGamepadButtonDown(const int gamepadId, const int button) const
    {
        return false;
    }

    bool SDLInputHandler::IsGamepadButtonUp(const int gamepadId, const int button) const
    {
        return false;
    }

    bool SDLInputHandler::IsGamepadButtonHeld(const int gamepadId, const int button) const
    {
        return false;
    }

    bool SDLInputHandler::IsKeyDown(const int keyCode) const
    {
        return false;
    }

    bool SDLInputHandler::IsKeyUp(const int keyCode) const
    {
        return false;
    }

    bool SDLInputHandler::IsKeyHeld(const int keyCode) const
    {
        return false;
    }

    bool SDLInputHandler::IsMouseButtonDown(const int button) const
    {
        return false;
    }

    bool SDLInputHandler::IsMouseButtonUp(const int button) const
    {
        return false;
    }

    bool SDLInputHandler::IsMouseButtonHeld(const int button) const
    {
        return false;
    }

    Tbx::Vector2 SDLInputHandler::GetMousePosition() const
    {
        return Tbx::Vector2();
    }
}
