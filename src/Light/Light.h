#pragma once

#include "Scene/Transform.h"
#include "glm/glm.hpp"
#include "pch.h"

class Light
{
    public:
        Light() : _color(0), _power(0) 
        {}
        Light(glm::vec3 color, float power) : _color(color), _power(power)
        {}
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
        Transform transform;
    protected:
        glm::vec3 _color;
        float _power;
};
