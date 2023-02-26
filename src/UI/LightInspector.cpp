#include "LightInspector.h"
#include "Light/Light.h"

#include "imgui.h"

bool LightInspector::OnGUI() 
{
    if(_light == nullptr)
        return false;

    if(ImGui::CollapsingHeader("Light Properties"))
    {
        ImGui::ColorPicker3("Color", &_light->color[0]);
        ImGui::InputFloat("Power", &_light->power);
    }
    return false;
}

void LightInspector::Bind(Light* light) 
{
    _light = light;
}

void LightInspector::UnBind() 
{
    _light = nullptr;
}
