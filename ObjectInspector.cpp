#include "ObjectInspector.h"

#include "Object.h"
#include "imgui.h"

bool ObjectInspector::ObjectInspector::OnGUI() 
{
    if(_obj == nullptr)
    {
        return false;
    }    
    bool refresh = false;
    glm::vec3 loc = _obj->GetLocation();
    glm::vec3 rot = _obj->GetRotationEuler();
    glm::vec3 sca = _obj->GetScale();    
    ImGui::Begin(_obj->Name.c_str(), &_open);
    if(ImGui::DragFloat3("Location", &loc[0]))
    {
        _obj->SetLocation(loc);
        refresh = true;
    }
    if(ImGui::DragFloat3("Rotation", &rot[0]))
    {
        _obj->SetRotationEuler(rot);
        refresh = true;
    }
    if(ImGui::DragFloat3("Scale", &sca[0]))
    {
        _obj->SetScale(sca);
        refresh = true;
    }
    ImGui::End();
    return refresh;
}

void ObjectInspector::ObjectInspector::Bind(Object* obj) 
{
    _obj = obj;
}

void ObjectInspector::ObjectInspector::UnBind() 
{
    _obj = nullptr;
}
