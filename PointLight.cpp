#include "PointLight.h"


PointLight::PointLight()
    : _pos(glm::vec3(0)), _color(glm::vec3(1)), _power(1)
{

}

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float power) 
    : _pos(pos), _color(color), _power(power)
{
    
}

glm::vec3 PointLight::GetLuminance(glm::vec3 point) 
{
    float r =  length(_pos - point);
    return _color * _power / (r * r);
}

void PointLight::SetPos(glm::vec3 pos) 
{
    _pos = pos;    
}

void PointLight::SetColor(glm::vec3 color) 
{
    _color = color;    
}

void PointLight::SetPower(float power) 
{
    _power = power;    
}

glm::vec3 PointLight::GetPos() const
{
    return _pos;
}

glm::vec3 PointLight::GetColor() const
{
    return _color;
}

float PointLight::GetPower() const
{
    return _power;
}

