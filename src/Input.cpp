#include "Input.h"

#include <GLFW/glfw3.h>

std::unordered_map<KeyCode, bool> Input::_keyStates = std::unordered_map<KeyCode, bool>();
glm::vec2 Input::_scroll = glm::vec2(0);

void Input::NewFrame()
{
    _scroll = glm::vec2(0);
}

void Input::SetKeyState(int key, int action)
{
    switch (key)
    {
    case GLFW_KEY_Q:
        _keyStates[KeyCode::Q] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;
    case GLFW_KEY_W:
        _keyStates[KeyCode::W] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;
    case GLFW_KEY_E:
        _keyStates[KeyCode::E] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;
    case GLFW_KEY_A:
        _keyStates[KeyCode::A] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;
    case GLFW_KEY_S:
        _keyStates[KeyCode::S] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;
    case GLFW_KEY_D:
        _keyStates[KeyCode::D] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;            
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        _keyStates[KeyCode::SHIFT] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;            
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        _keyStates[KeyCode::CONTROL] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        break;            
    default:
        break;
    }
}

void Input::SetMouseScroll(float x, float y)
{
    _scroll = {x, y};
}

bool Input::GetKey(KeyCode key)
{
    return _keyStates[key];
}

glm::vec2 Input::GetMouseScroll()
{
    return _scroll;
}
