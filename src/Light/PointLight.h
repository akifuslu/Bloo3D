#pragma once

#include "Light.h"
#include "glm/glm.hpp"

class PointLight : public Light
{
    public:
        PointLight() : Light() {}
        virtual glm::vec3 GetAttenuation(glm::vec3 point) override;
};
