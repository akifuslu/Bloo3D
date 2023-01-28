#pragma once

#include "glm/glm.hpp"
#include "glm/vec2.hpp"

struct Ray;

class Camera
{
    public:
        Camera(glm::vec3 pos, 
               glm::vec3 rot, 
               float fov,
               u_int32_t width,
               u_int32_t height);

        Ray GetRay(int x, int y);
        void SetPosition(glm::vec3 pos);
        void SetRotation(glm::vec3 rot);
        void OnResize(int width, int height);
        inline u_int32_t GetWidth() const
        {
            return _width;
        }
        inline u_int32_t GetHeight() const
        {
            return _height;
        }
        inline glm::mat4 GetVP() const
        {
            return _vp;
        }
    private:
        void RebuildMatrix();
        glm::mat4 _projection;
        glm::mat4 _vp;
        glm::mat4 _mat;
        glm::mat4 _tInvMat;
        glm::vec3 _pos;
        glm::vec3 _rot;
        float _fov;
        float _scale;
        float _aspect;
        u_int32_t _width;
        u_int32_t _height;
};
