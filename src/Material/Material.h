#pragma once

#include "glm/glm.hpp"

struct ShadeProperties
{
    glm::vec3 normal;
    glm::vec3 lightDir;
    glm::vec3 halfDir;
    glm::vec3 viewDir;
    glm::vec3 radiance;
};

class MaterialBase
{
    public:
        virtual ~MaterialBase() = default;
        virtual glm::vec3 Shade(const ShadeProperties& props)
        {
            return glm::vec3(0);
        }
        glm::vec3 ambient;
        glm::vec3 albedo;
};

class MaterialDiffuse : public MaterialBase
{
    public:
        virtual ~MaterialDiffuse() override{}
        virtual glm::vec3 Shade(const ShadeProperties& props) override;
};


class MaterialPBR : public MaterialBase
{
    public:
        virtual ~MaterialPBR() override{}
        virtual glm::vec3 Shade(const ShadeProperties& props) override;
        float metallic;
        float roughness;
};
