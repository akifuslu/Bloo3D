#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "pch.h"

enum class KeyCode
{
    Q, W, E, A, S, D, CONTROL, SHIFT, G, R, X, Y, Z, 
    N0, N1, N2, N3, N4, N5, N6, N7, N8, N9     
};

enum class MouseButton
{
    RIGHT, LEFT, MIDDLE
};

enum class ButtonState
{
    NONE,
    DOWN,
    HOLD,
    UP
};

class Input
{
    public:
        static void Initialize();
        static void NewFrame();
        static void SetScreenSize(glm::ivec2 size);
        static void SetKeyState(int key, int action);
        static void SetMouseButtonState(int button, int action);
        static void SetMousePosition(int x, int y);    
        static void SetMouseScroll(float x, float y);    
        static bool GetKey(KeyCode key);        
        static bool GetKeyDown(KeyCode key);
        static bool GetKeyUp(KeyCode key);
        static bool GetMouseButton(MouseButton button);
        static bool GetMouseButtonDown(MouseButton button);
        static bool GetMouseButtonUp(MouseButton button);
        static bool MouseOnUI();
        static bool KeyboardOnUI();
        static glm::ivec2 GetMousePosition();
        static glm::vec2 GetMousePositionNDC();
        static glm::vec2 GetNormalizedMouseDelta();
        static glm::vec2 GetMouseScroll();        
    private:
        static std::unordered_map<KeyCode, ButtonState> _keyStates;
        static std::unordered_map<MouseButton, ButtonState> _mouseButtonStates;
        static glm::vec2 _scroll;
        static glm::ivec2 _mousePos;
        static glm::ivec2 _lastMousePos;
        static glm::ivec2 _screenSize;
};
