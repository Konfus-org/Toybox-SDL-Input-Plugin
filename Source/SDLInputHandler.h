#pragma once
#include <SDL3/SDL.h>
#include <Tbx/Events/AppEvents.h>
#include <Tbx/PluginAPI/RegisterPlugin.h>
#include <unordered_map>

namespace SDLInput
{
    class SDLInputHandler : public Tbx::IInputHandlerPlugin
    {
    public:
        SDLInputHandler();
        ~SDLInputHandler();

        void OnLoad() override;
        void OnUnload() override;

        void Update() override;

        bool IsGamepadButtonDown(const int gamepadId, const int button) const override;
        bool IsGamepadButtonUp(const int gamepadId, const int button) const override;
        bool IsGamepadButtonHeld(const int gamepadId, const int button) const override;

        bool IsKeyDown(const int keyCode) const override;
        bool IsKeyUp(const int keyCode) const override;
        bool IsKeyHeld(const int keyCode) const override;

        bool IsMouseButtonDown(const int button) const override;
        bool IsMouseButtonUp(const int button) const override;
        bool IsMouseButtonHeld(const int button) const override;
        Tbx::Vector2 GetMousePosition() const override; 
    
    private:
        std::unordered_map<int, SDL_Gamepad*> _gamepads = {};
    };

    TBX_REGISTER_PLUGIN(SDLInputHandler);
}
