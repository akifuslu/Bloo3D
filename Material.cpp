#include "Material.h"

#include <algorithm>

glm::vec3 MaterialDiffuse::Shade(const ShadeProperties& props)
{
    float d = dot(props.Normal, props.LightDir);
    return Ambient + Albedo * props.Luminance * std::max(0.0f, d) / (float)M_PI;    
}