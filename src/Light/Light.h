#pragma once

#include "Scene/Object.h"
#include "glm/glm.hpp"
#include "pch.h"


enum class LightType
{
    POINT,
    DIRECTIONAL,
    SPOT,
    AREA
};

class Light;

class LightState
{
    public:
        LightState(Light* context) : context(context){}
        virtual ~LightState() = default;
        virtual glm::vec3 GetAttenuation(glm::vec3 point) = 0;
        Light* context;
};

class PointLight : public LightState
{
    public:
        PointLight(Light* c) : LightState(c) {}
        virtual glm::vec3 GetAttenuation(glm::vec3 point) override;
};

class DirectionalLight : public LightState
{
    public:
        DirectionalLight(Light* c) : LightState(c) {}
        virtual glm::vec3 GetAttenuation(glm::vec3 point) override;
};

class SpotLight : public LightState
{
    public:
        SpotLight(Light* c) : LightState(c) {}
        virtual glm::vec3 GetAttenuation(glm::vec3 point) override;
};

class AreaLight : public LightState
{
    public:
        AreaLight(Light* c) : LightState(c) {}
        virtual glm::vec3 GetAttenuation(glm::vec3 point) override;
};


class Light : public Object
{
    public:
        Light() : Object(), color(1), power(1)
        {
            type = ObjectType::LIGHT;
            lightType = LightType::POINT; // default type
            _states[LightType::POINT]       = new PointLight(this);
            _states[LightType::DIRECTIONAL] = new DirectionalLight(this);
            _states[LightType::SPOT]        = new SpotLight(this);
            _states[LightType::AREA]        = new AreaLight(this);
        }
        ~Light()
        {
            for(auto& s: _states)
            {
                delete s.second;
            }
        }
        inline glm::vec3 GetAttenuation(glm::vec3 point)
        {
            return _states[lightType]->GetAttenuation(point);
        }
        LightType lightType;
        glm::vec3 color;
        float power;
    private:
        std::unordered_map<LightType, LightState*> _states;
};


