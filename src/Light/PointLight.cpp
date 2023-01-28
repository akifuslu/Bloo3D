#include "PointLight.h"
#include "Logger.h"


glm::vec3 PointLight::GetAttenuation(glm::vec3 point) 
{
    float r =  length(transform.GetLocation() - point);
    return _color * _power / (r * r);
}

