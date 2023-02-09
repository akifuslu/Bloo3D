#pragma once

#include "imgui.h"
class Scene;

class HierarchyView
{
    public:
        HierarchyView();
        bool OnGUI();
        void Bind(Scene* scene);
        void UnBind();
    private:
        Scene* _scene = nullptr;
        ImGuiTreeNodeFlags _baseFlags;
};
