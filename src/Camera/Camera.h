#pragma once

#include "glm/glm.hpp"
#include "glm/vec2.hpp"

class Ray;

class Camera
{
    public:
        Camera(glm::vec3 pos, 
               glm::vec3 rot, 
               float fov,
               glm::ivec2 res);

        Ray GetRay(int x, int y);
        void SetPosition(glm::vec3 pos);
        void SetRotation(glm::vec3 rot);
    private:
        void RebuildMatrix();
        glm::mat4 _mat;
        glm::mat4 _tInvMat;
        glm::vec3 _pos;
        glm::vec3 _rot;
        float _fov;
        glm::ivec2 _res;
        float _scale;
        float _aspect;
};
