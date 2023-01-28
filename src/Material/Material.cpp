#include "Material.h"

#include "pch.h"

float NDFBlinn(const ShadeProperties& props, float roughness)
{
    auto nDotH = std::max(0.0f, dot(props.normal, props.halfDir));
    float a = roughness * roughness + 0.0001f; // avoid divide by zero
    float pow = 2 / (a * a) - 2;     
    return std::powf(nDotH, pow) / (M_PI * a * a);
}

float GeomCookTorrance(const ShadeProperties& props)
{
    auto nDotH = std::max(0.0f, dot(props.normal, props.halfDir));
    auto nDotV = std::max(0.0f, dot(props.normal, props.viewDir));
    auto nDotL = std::max(0.0f, dot(props.normal, props.lightDir));
    auto vDotH = std::max(0.0f, dot(props.viewDir, props.halfDir));

    float val = std::min(2 * nDotH * nDotV / vDotH, 2 * nDotH * nDotL / vDotH);
    return std::min(1.0f, val);
}

glm::vec3 FresnelSchlick(const ShadeProperties& props, glm::vec3 f0)
{
    auto vDotH = std::max(0.0f, dot(props.viewDir, props.halfDir));
    return f0 + (glm::vec3(1.0f) - f0) * std::powf(1 - vDotH, 5);
}

glm::vec3 BRDFCookTorrance(const ShadeProperties& props, glm::vec3 albedo, float metallic, float roughness)
{
    //auto nDotH = std::max(0.0f, dot(props.normal, props.halfDir));
    auto nDotV = std::max(0.0f, dot(props.normal, props.viewDir));
    auto nDotL = std::max(0.0f, dot(props.normal, props.lightDir));
    //auto vDotH = std::max(0.0f, dot(props.viewDir, props.halfDir));

    glm::vec3 f0(0.04);
    glm::mix(f0, albedo, metallic);
    float D = NDFBlinn(props, roughness);
    float G = GeomCookTorrance(props);
    glm::vec3 F = FresnelSchlick(props, f0);

    glm::vec3 rs = (D * G * F) / (4.0f * nDotL * nDotV + 0.0001f);
    auto ks = f0;
    auto kd = glm::vec3(1.0f) - ks;
    kd *= 1 - metallic;
    return (kd * albedo * (float)M_1_PI + rs) * props.radiance * nDotL;
}

glm::vec3 MaterialDiffuse::Shade(const ShadeProperties& props)
{
    float d = dot(props.normal, props.lightDir);
    return ambient + albedo * props.radiance * std::max(0.0f, d) / (float)M_PI;    
}


glm::vec3 MaterialPBR::Shade(const ShadeProperties& props)
{
    return ambient + BRDFCookTorrance(props, albedo, metallic, roughness);
}
