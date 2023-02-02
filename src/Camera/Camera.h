#pragma once

#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "Scene/Transform.h"

struct Ray;

class Camera
{
    public:
        Camera(float fov,
               float near,
               float far,
               u_int32_t width,
               u_int32_t height);

        Ray GetRay(int x, int y);
        void OnUpdate();
        void OnResize(int width, int height);
        inline u_int32_t GetWidth() const
        {
            return _width;
        }
        inline u_int32_t GetHeight() const
        {
            return _height;
        }
        inline glm::mat4 GetViewProj() const
        {
            return _viewProj;
        }
        inline glm::mat4 GetInvView() const
        {
            return _invView;
        }
        inline glm::mat4 GetInvProj() const
        {
            return _invProj;
        }
        inline float GetNear() const
        {
            return _near;
        }
        inline float GetFar() const
        {
            return _far;
        }
        Transform transform;
    private:
        void RebuildMatrix();
        glm::mat4 _proj;
        glm::mat4 _invProj;
        glm::mat4 _viewProj;
        glm::mat4 _view;
        glm::mat4 _invView;
        float _fov;
        float _aspect;
        float _near;
        float _far;
        u_int32_t _width;
        u_int32_t _height;
};
