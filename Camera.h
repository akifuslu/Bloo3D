#pragma once

#include "glm/glm.hpp"
#include "glm/vec2.hpp"

class Ray;

class Camera
{
    public:
        Camera(glm::vec3 pos, 
               glm::vec3 dir, 
               glm::vec3 up,
               float fov,
               glm::ivec2 res);

        Ray GetRay(int x, int y);
    private:
        glm::vec3 _pos;
        glm::vec3 _dir;
        glm::vec3 _up;
        float _fov;
        glm::ivec2 _res;

        float _scale;
        float _aspect;
};
