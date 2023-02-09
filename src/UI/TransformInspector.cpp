#include "TransformInspector.h"

#include "imgui.h"

bool TransformInspector::OnGUI() 
{
    if(_transform == nullptr)
    {
        return false;
    }    
    if(ImGui::CollapsingHeader("Transform Properties"))
    {
        bool refresh = false;
        glm::vec3 loc = _transform->GetLocation();
        glm::vec3 rot = _transform->GetRotation();
        glm::vec3 sca = _transform->GetScale();    
        if(ImGui::DragFloat3("Location", &loc[0], 0.1f))
        {
            _transform->SetLocation(loc);
            refresh = true;
        }
        if(ImGui::DragFloat3("Rotation", &rot[0]))
        {
            _transform->SetRotation(rot);
            refresh = true;
        }
        if(ImGui::DragFloat3("Scale", &sca[0], 0.1f))
        {
            _transform->SetScale(sca);
            refresh = true;
        }
        return refresh;
    }
    return false;
}

void TransformInspector::Bind(Transform* transform) 
{
    _transform = transform;
}

void TransformInspector::UnBind() 
{
    _transform = nullptr;
}
