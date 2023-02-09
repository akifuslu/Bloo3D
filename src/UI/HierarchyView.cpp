#include "HierarchyView.h"

#include "Scene/Scene.h"

HierarchyView::HierarchyView()
{
    // set up flags
    _baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
}


bool HierarchyView::OnGUI()
{
    if(_scene == nullptr)
        return false;
    
    ImGui::Begin("Hierarchy");

    for(auto& obj: _scene->objects)
    {
        ImGuiTreeNodeFlags flags = _baseFlags | (obj->selected ? ImGuiTreeNodeFlags_Selected : 0);
        bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)obj->id, flags, "%s", obj->name.c_str());
        if (ImGui::IsItemClicked())
        {
            for(auto& obj2 : _scene->objects) // TODO: iterate only on currently selected objects?
            {
                obj2->selected = false;
            }
            obj->selected = true;
            _scene->selectedObject.Set(obj);
        }
        if (open)
        {
            ImGui::TreePop();
        }
    }
    ImGui::End();
    return false;
}

void HierarchyView::Bind(Scene* scene)
{
    _scene = scene;
}

void HierarchyView::UnBind()
{
    _scene = nullptr;
}



