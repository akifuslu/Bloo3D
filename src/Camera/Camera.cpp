#include "Camera.h"

#include "Geometry/Ray.h"
#include "glm/matrix.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"



Camera::Camera(glm::vec3 pos, 
                       glm::vec3 rot, 
                       float fov,
                       u_int32_t width,
                       u_int32_t height) 
{
    _pos = pos;
    _rot = rot;
    _fov = fov;
    _width = width;
    _height = height;

    _scale = tan(_fov * 0.5 * M_PI / 180.0);
    _aspect = _width / (float)_height;        

    RebuildMatrix();
}

Ray Camera::GetRay(int x, int y) 
{
    float a = (2 * (x + 0.5) / (float)_width - 1) * _aspect * _scale;
    float b = (1 - 2 * (y + 0.5) / (float)_height) * _scale;

    auto wdir = normalize(_tInvMat * glm::vec4(a, b, 1, 1));
    return Ray(_pos, wdir);
}

void Camera::SetPosition(glm::vec3 pos)
{
    _pos = pos;
    RebuildMatrix();
}

void Camera::SetRotation(glm::vec3 rot)
{
    _rot = rot;
    RebuildMatrix();
}

void Camera::OnResize(int width, int height)
{
    if(_width == width && _height == height) 
        return;
    _width = width;
    _height = height;
    _aspect = _width / (float)_height;        
    RebuildMatrix();
}

void Camera::RebuildMatrix()
{
    _mat = glm::mat4(1.0f);
    _mat = glm::translate(_mat, _pos);
    _mat = glm::rotate(glm::radians(_rot.x), glm::vec3(1, 0, 0)) * _mat;
    _mat = glm::rotate(glm::radians(_rot.y), glm::vec3(0, 1, 0)) * _mat;
    _mat = glm::rotate(glm::radians(_rot.z), glm::vec3(0, 0, 1)) * _mat;
    _tInvMat = transpose((inverse(_mat)));
    
    _projection = glm::perspective(glm::radians(_fov), _aspect, 0.01f, 100.0f);
    _vp = _projection * _mat;
}
