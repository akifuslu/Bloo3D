#include "Camera.h"

#include "Geometry/Ray.h"
#include "glm/matrix.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"


Camera::Camera::Camera(glm::vec3 pos, 
                       glm::vec3 rot, 
                       float fov,
                       glm::ivec2 res) 
{
    _pos = pos;
    _rot = rot;
    _fov = fov;
    _res = res;

    _scale = tan(_fov * 0.5 * M_PI / 180.0);
    _aspect = _res.x / (float)_res.y;    

    RebuildMatrix();
}

Ray Camera::Camera::GetRay(int x, int y) 
{
    float a = (2 * (x + 0.5) / (float)_res.x - 1) * _aspect * _scale;
    float b = (1 - 2 * (y + 0.5) / (float)_res.y) * _scale;

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

void Camera::RebuildMatrix()
{
    _mat = glm::mat4(1.0f);
    _mat = glm::translate(_mat, _pos);
    _mat = glm::rotate(glm::radians(_rot.x), glm::vec3(1, 0, 0)) * _mat;
    _mat = glm::rotate(glm::radians(_rot.y), glm::vec3(0, 1, 0)) * _mat;
    _mat = glm::rotate(glm::radians(_rot.z), glm::vec3(0, 0, 1)) * _mat;
    _tInvMat = transpose((inverse(_mat)));
}

