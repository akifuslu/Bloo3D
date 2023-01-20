#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

struct WindowProps
{
    std::string Title;
    int Width = 960;
    int Height = 540;
};

class Window
{
    public:
        Window(const WindowProps& props);
        ~Window();
        void Init(const WindowProps& props);
        void Shutdown();
        bool ShouldClose() const;
        void SwapBuffers() const;
        void Poll() const;
        void SetResizeCallback(const std::function<void(int, int)> f);
        inline GLFWwindow* Get() const
        {
            return _window;
        }
        inline int GetWidth() const
        {
            return _width;
        }
        inline int GetHeight() const
        {
            return _height;
        }
    private:
        void OnResize(int width, int height);
        void OnKeyInput(int key, int scancode, int action, int mods);
        void OnMouseInput(int button, int action, int mods);
        GLFWwindow* _window;
        int _width;
        int _height;
        std::function<void(int, int)> _resizeCallback; 
};
