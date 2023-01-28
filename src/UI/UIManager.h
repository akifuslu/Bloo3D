#pragma once

#include "Window.h"

class UIManager
{
    public:
        UIManager(Window* window);
        void NewFrame();
        void Render();
    private:
        void Init(Window* window);
};
