#include "Light.h"

glm::vec3 PointLight::GetAttenuation(glm::vec3 point) 
{
    float r =  length(context->transform.GetLocation() - point);
    return context->color * context->power / (r * r);
}

glm::vec3 DirectionalLight::GetAttenuation(glm::vec3 point)
{
    return context->color * context->power;
}

glm::vec3 SpotLight::GetAttenuation(glm::vec3 point)
{
    return context->color * context->power;
}

glm::vec3 AreaLight::GetAttenuation(glm::vec3 point)
{
    return context->color * context->power;
}
