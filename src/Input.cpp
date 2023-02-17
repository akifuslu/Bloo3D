#include "Input.h"

#include "imgui.h"
#include <GLFW/glfw3.h>
#include "Logger.h"

std::unordered_map<KeyCode, ButtonState> Input::_keyStates = std::unordered_map<KeyCode, ButtonState>();
std::unordered_map<MouseButton, ButtonState> Input::_mouseButtonStates = std::unordered_map<MouseButton, ButtonState>();
glm::vec2 Input::_scroll = glm::vec2(0);
glm::ivec2 Input::_mousePos = glm::ivec2(0);
glm::ivec2 Input::_lastMousePos = glm::ivec2(0);
glm::ivec2 Input::_screenSize = glm::ivec2(0);

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
    {GLFW_KEY_G,             KeyCode::G},
    {GLFW_KEY_R,             KeyCode::R},
    {GLFW_KEY_X,             KeyCode::X},
    {GLFW_KEY_Y,             KeyCode::Y},
    {GLFW_KEY_Z,             KeyCode::Z},
    {GLFW_KEY_0,             KeyCode::N0},
    {GLFW_KEY_1,             KeyCode::N1},
    {GLFW_KEY_2,             KeyCode::N2},
    {GLFW_KEY_3,             KeyCode::N3},
    {GLFW_KEY_4,             KeyCode::N4},
    {GLFW_KEY_5,             KeyCode::N5},
    {GLFW_KEY_6,             KeyCode::N6},
    {GLFW_KEY_7,             KeyCode::N7},
    {GLFW_KEY_8,             KeyCode::N8},
    {GLFW_KEY_9,             KeyCode::N9},
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
    _lastMousePos = _mousePos;
}

void Input::SetScreenSize(glm::ivec2 size)
{
    _screenSize = size;
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

glm::vec2 Input::GetMousePositionNDC()
{
    float x = ((float)_mousePos.x / _screenSize.x) * 2 - 1;
    float y = ((float)_mousePos.y / _screenSize.y) * 2 - 1;
    return {x, y};
}

glm::vec2 Input::GetNormalizedMouseDelta()
{
    float x = (_mousePos.x - _lastMousePos.x) / (float)_screenSize.x;
    float y = (_mousePos.y - _lastMousePos.y) / (float)_screenSize.y;
    return {x, y};
}

glm::vec2 Input::GetMouseScroll()
{
    return _scroll;
}

bool Input::MouseOnUI()
{
    return ImGui::GetIO().WantCaptureMouse;
}

bool Input::KeyboardOnUI()
{
    return ImGui::GetIO().WantCaptureKeyboard;
}
