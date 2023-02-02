#include "Input.h"

#include <GLFW/glfw3.h>
#include "Logger.h"

std::unordered_map<KeyCode, ButtonState> Input::_keyStates = std::unordered_map<KeyCode, ButtonState>();
std::unordered_map<MouseButton, ButtonState> Input::_mouseButtonStates = std::unordered_map<MouseButton, ButtonState>();
glm::vec2 Input::_scroll = glm::vec2(0);
glm::ivec2 Input::_mousePos = glm::ivec2(0);

std::unordered_map<int, KeyCode> KeyLookup =
{
    {GLFW_KEY_Q,             KeyCode::Q},
    {GLFW_KEY_W,             KeyCode::W},
    {GLFW_KEY_E,             KeyCode::E},
    {GLFW_KEY_A,             KeyCode::A},
    {GLFW_KEY_S,             KeyCode::S},
    {GLFW_KEY_D,             KeyCode::D},
    {GLFW_KEY_LEFT_SHIFT,    KeyCode::SHIFT},
    {GLFW_KEY_RIGHT_SHIFT,   KeyCode::SHIFT},
    {GLFW_KEY_LEFT_CONTROL,  KeyCode::CONTROL},
    {GLFW_KEY_RIGHT_CONTROL, KeyCode::CONTROL},
};

std::unordered_map<int, MouseButton> MouseButtonLookup =
{
    {GLFW_MOUSE_BUTTON_LEFT,   MouseButton::LEFT},
    {GLFW_MOUSE_BUTTON_RIGHT,  MouseButton::RIGHT},
    {GLFW_MOUSE_BUTTON_MIDDLE, MouseButton::MIDDLE},
};

void Input::Initialize()
{
    for(auto& kl: KeyLookup)
    {
        _keyStates[kl.second] = ButtonState::NONE;
    }
    for(auto& mb: MouseButtonLookup)
    {
        _mouseButtonStates[mb.second] = ButtonState::NONE;
    }
}

void Input::NewFrame()
{
    // reset scroll
    _scroll = glm::vec2(0);
    // refresh keystates
    for (auto& ks: _keyStates)
    {
        if(ks.second == ButtonState::DOWN)
        {
            ks.second = ButtonState::HOLD;
        }
        if(ks.second == ButtonState::UP)
        {
            ks.second = ButtonState::NONE;
        }
    }
    // set up mouse buttons to none
    for(auto& mb: _mouseButtonStates)
    {
        if(mb.second == ButtonState::DOWN)
        {
            mb.second = ButtonState::HOLD;
        }
        if(mb.second == ButtonState::UP)
        {
            mb.second = ButtonState::NONE;
        }
    }
}

void Input::SetKeyState(int key, int action)
{
    auto kc = KeyLookup[key];
    if(action == GLFW_PRESS)
    {
        _keyStates[kc] = ButtonState::DOWN;
    }
    else if(action == GLFW_RELEASE)
    {
        _keyStates[kc] = ButtonState::UP;
    }
}

void Input::SetMouseButtonState(int button, int action)
{
    auto mb = MouseButtonLookup[button];
    if(action == GLFW_PRESS)
    {
        _mouseButtonStates[mb] = ButtonState::DOWN;
    }
    else if(action == GLFW_RELEASE)
    {
        _mouseButtonStates[mb] = ButtonState::UP;
    }
}

void Input::SetMousePosition(int x, int y)
{   
    _mousePos = {x, y};
}

void Input::SetMouseScroll(float x, float y)
{
    _scroll = {x, y};
}

bool Input::GetKey(KeyCode key)
{
    return _keyStates[key] == ButtonState::DOWN || _keyStates[key] == ButtonState::HOLD;
}

bool Input::GetKeyDown(KeyCode key)
{
    return _keyStates[key] == ButtonState::DOWN;
}

bool Input::GetKeyUp(KeyCode key)
{
    return _keyStates[key] == ButtonState::UP;
}

bool Input::GetMouseButton(MouseButton button)
{
    return _mouseButtonStates[button] == ButtonState::DOWN || _mouseButtonStates[button] == ButtonState::HOLD;
}

bool Input::GetMouseButtonDown(MouseButton button)
{
    return _mouseButtonStates[button] == ButtonState::DOWN;
}

bool Input::GetMouseButtonUp(MouseButton button)
{
    return _mouseButtonStates[button] == ButtonState::UP;
}

glm::ivec2 Input::GetMousePosition()
{
    return _mousePos;
}

glm::vec2 Input::GetMouseScroll()
{
    return _scroll;
}
