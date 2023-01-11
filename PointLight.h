#pragma once

#include "glm/glm.hpp"

class PointLight
{
    public:
        PointLight();
        PointLight(glm::vec3 pos, glm::vec3 color, float power);

        glm::vec3 GetLuminance(glm::vec3 point);
        void SetPos(glm::vec3 pos);
        void SetColor(glm::vec3 color);
        void SetPower(float power);

        glm::vec3 GetPos() const;
        glm::vec3 GetColor() const;
        float GetPower() const;

    private:
        glm::vec3 _pos;
        glm::vec3 _color;
        float _power;

};
