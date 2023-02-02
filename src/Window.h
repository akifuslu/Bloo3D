#pragma once

#include <GLFW/glfw3.h>
#include "pch.h"

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
        void SwapBuffers();
        void Poll() const;
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
        inline bool IsResized() const
        {
            return _resized;
        }
    private:
        void OnResize(int width, int height);
        void OnKeyInput(int key, int scancode, int action, int mods);
        void OnMouseInput(int button, int action, int mods);
        void OnMousePositionInput(double x, double y);
        void OnScrollInput(double x, double y);
        GLFWwindow* _window;
        int _width;
        int _height;
        bool _resized;
};
