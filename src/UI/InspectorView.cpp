#include "InspectorView.h"

#include "imgui.h"

bool InspectorView::OnGUI() 
{
    ImGui::Begin("Inspector");
    if(_obj == nullptr)
    {
        ImGui::Text("No object has been selected.");
        ImGui::End();
        return false;
    }
    _objInsp.OnGUI();
    _transformInsp.OnGUI();
    ImGui::End();
    return false;
}

void InspectorView::Bind(Object* obj) 
{
    _obj = obj;    
    _objInsp.Bind(obj);
    _transformInsp.Bind(&obj->transform);
}

void InspectorView::UnBind() 
{
    _obj = nullptr;
    _objInsp.UnBind();
    _transformInsp.UnBind();
}
