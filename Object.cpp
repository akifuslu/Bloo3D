#include "Object.h"

#include "glm/gtx/transform.hpp"

Object::Object(const std::string& name) 
    : Name(name)
{
}

void Object::SetLocation(glm::vec3 loc) 
{
    _location = loc;
}

void Object::SetRotation(glm::quat rot) 
{
    _rotation = rot;
}

void Object::SetScale(glm::vec3 sca) 
{
    _scale = sca;
}

glm::vec3 Object::GetLocation() 
{
    return _location;
}

glm::quat Object::GetRotation() 
{
    return _rotation;
}

glm::vec3 Object::GetScale() 
{
    return _scale;
}

void Object::BuildMats()
{
//    _localToWorld = glm::mat4(1.0f);
//    _localToWorld = glm::scale(_scale) * _localToWorld;
//    _localToWorld = glm::gt * _localToWorld;
}
