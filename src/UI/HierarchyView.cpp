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
    for(auto& obj: _scene->GetObjects())
    {
        if(obj->transform.GetParent() != nullptr) // skip objects with parent, root objects will display its hierarchy recursively
            continue;
        
        DrawHierRecur(obj);
    }
    ImGui::End();
    return false;
}

void HierarchyView::DrawHierRecur(Object* obj)
{
    ImGuiTreeNodeFlags flags = _baseFlags | (obj->selected ? ImGuiTreeNodeFlags_Selected : 0);
    bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)obj->id, flags, "%s", obj->name.c_str());
    if (ImGui::IsItemClicked())
    {
        for(auto& obj2 : _scene->GetObjects()) // TODO: iterate only on currently selected objects?
        {
            obj2->selected = false;
        }
        obj->selected = true;
        _scene->selectedObject.Set(obj);
    }
    if (open)
    {
        for(auto& ch: obj->transform.GetChilds())
        {
            DrawHierRecur(ch->GetObject());
        }
        ImGui::TreePop();
    }
}

void HierarchyView::Bind(Scene* scene)
{
    _scene = scene;
}

void HierarchyView::UnBind()
{
    _scene = nullptr;
}



