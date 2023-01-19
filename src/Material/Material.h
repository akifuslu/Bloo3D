#pragma once

#include "glm/glm.hpp"

struct ShadeProperties
{
    glm::vec3 Normal;
    glm::vec3 LightDir;
    glm::vec3 HalfDir;
    glm::vec3 ViewDir;
    glm::vec3 Radiance;
};

class MaterialBase
{
    public:
        virtual glm::vec3 Shade(const ShadeProperties& props)
        {
            return glm::vec3(0);
        }
        glm::vec3 Ambient;
        glm::vec3 Albedo;
};

class MaterialDiffuse : public MaterialBase
{
    public:
        virtual glm::vec3 Shade(const ShadeProperties& props) override;
};


class MaterialPBR : public MaterialBase
{
    public:
        virtual glm::vec3 Shade(const ShadeProperties& props) override;
        float Metallic;
        float Roughness;
};
