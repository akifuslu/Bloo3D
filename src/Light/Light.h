#pragma once

#include "Scene/Object.h"
#include "glm/glm.hpp"
#include "pch.h"

class Light : public Object
{
    public:
        Light() : Object(), _color(1), _power(1)
        {
            type = ObjectType::LIGHT;
        }
        virtual ~Light() = default;
        virtual glm::vec3 GetAttenuation(glm::vec3 point) = 0;
        inline void SetColor(glm::vec3 color)
        {
            _color = color;
        }
        inline void SetPower(float power)
        {
            _power = power;
        }
        inline glm::vec3 GetColor() const
        {
            return _color;
        }
        inline float GetPower() const
        {
            return _power;
        }
    protected:
        glm::vec3 _color;
        float _power;
};
