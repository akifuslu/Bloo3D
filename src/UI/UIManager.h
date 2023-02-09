#pragma once

#include "HierarchyView.h"
#include "InspectorView.h"
#include "Window.h"
#include "Scene/Scene.h"

class UIManager
{
    public:
        UIManager(Window* window);
        void NewFrame();
        void Render();
        void SetScene(Scene* scene);
    private:
        void Init(Window* window);
        Scene* _scene;
        // views
        HierarchyView _hierView;
        InspectorView _inspView;
};
