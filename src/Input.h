#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "glm/glm.hpp"

enum class KeyCode
{
    Q, W, E, A, S, D, CONTROL, SHIFT    
};

class Input
{
    public:
        static void NewFrame();
        static void SetKeyState(int key, int action);
        static void SetMouseScroll(float x, float y);
        static bool GetKey(KeyCode key);        
        static glm::vec2 GetMouseScroll();
    private:
        static std::unordered_map<KeyCode, bool> _keyStates;
        static glm::vec2 _scroll;
};
