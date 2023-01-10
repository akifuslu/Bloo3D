#include "Camera.h"

#include "Ray.h"


Camera::Camera::Camera(glm::vec3 pos, 
                       glm::vec3 dir, 
                       glm::vec3 up,
                       float fov,
                       glm::ivec2 res) 
{
    _pos = pos;
    _dir = normalize(dir);
    _up = normalize(up);
    _fov = fov;
    _res = res;

    _scale = tan(_fov * 0.5 * M_PI / 180.0);
    _aspect = _res.x / (float)_res.y;    
}

Ray Camera::Camera::GetRay(int x, int y) 
{
    float a = (2 * (x + 0.5) / (float)_res.x - 1) * _aspect * _scale;
    float b = (1 - 2 * (y + 0.5) / (float)_res.y) * _scale;

    return Ray(_pos, normalize(glm::vec3(a, b, 1)));
}
