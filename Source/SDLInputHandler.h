#pragma once
#include <SDL3/SDL.h>
#include <Tbx/Events/AppEvents.h>
#include <Tbx/PluginAPI/RegisterPlugin.h>
#include <array>
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

        bool OnSDLEvent(SDL_Event* event);
    
    private:
        std::unordered_map<int, SDL_Gamepad*> _gamepads = {};

        std::unordered_map<int, std::array<bool, SDL_GAMEPAD_BUTTON_COUNT>> _currGamepadBtnState;
        std::unordered_map<int, std::array<bool, SDL_GAMEPAD_BUTTON_COUNT>> _prevGamepadBtnState;

        std::array<Uint8, SDL_SCANCODE_COUNT> _currKeyState{};
        std::array<Uint8, SDL_SCANCODE_COUNT> _prevKeyState{};

        Uint32 _currMouseState = 0;
        Uint32 _prevMouseState = 0;
    };

    TBX_REGISTER_PLUGIN(SDLInputHandler);
}
