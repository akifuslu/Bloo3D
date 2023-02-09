#include "ObjectInspector.h"

#include "imgui.h"

bool ObjectInspector::OnGUI() 
{
    if(_obj == nullptr)
        return false;

    char buf[60];
    strlcpy(buf, _obj->name.c_str(), 60);
    //ImGui::BeginChild("Object Properties")
    //ImGui::C("Object Properties");
    if(ImGui::CollapsingHeader("Object Properties"))
    {
        if(ImGui::InputText("Name", buf, 60))
        {
            _obj->name.assign(buf);
        }
    }
    //ImGui::EndChild();   
    return false;
}

void ObjectInspector::Bind(Object* obj) 
{
    _obj = obj;
}

void ObjectInspector::UnBind() 
{
    _obj = nullptr;
}
