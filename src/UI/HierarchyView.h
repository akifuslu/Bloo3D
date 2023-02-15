#pragma once

#include "imgui.h"
class Scene;
class Object;

class HierarchyView
{
    public:
        HierarchyView();
        bool OnGUI();
        void Bind(Scene* scene);
        void UnBind();
    private:
        void DrawHierRecur(Object* obj);
        Scene* _scene = nullptr;
        ImGuiTreeNodeFlags _baseFlags;
};
