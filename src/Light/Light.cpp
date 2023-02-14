#include "Light.h"

glm::vec3 PointLight::GetAttenuation(glm::vec3 point) 
{
    float r =  length(transform.GetLocation() - point);
    return _color * _power / (r * r);
}


glm::vec3 DirectionalLight::GetAttenuation(glm::vec3 point)
{
    return _color * _power;
}
