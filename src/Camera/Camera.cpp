#include "Camera.h"

#include "Geometry/Ray.h"
#include "Logger.h"
#include "glm/matrix.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <glm/gtx/string_cast.hpp>


Camera::Camera( float fov,
                float near,
                float far,
                u_int32_t width,
                u_int32_t height) 
    : _fov(fov), _near(near), _far(far), _width(width), _height(height)
{
    _aspect = _width / (float)_height;        
    RebuildMatrix();
    transform.onUpdate = std::bind(&Camera::RebuildMatrix, this);
}

Ray Camera::GetRay(int x, int y) 
{
    float a = (x + 0.5) / (float)_width;
    float b = (y + 0.5) / (float)_height;
    a = a * 2 - 1;
    b = b * 2 - 1;

    glm::vec4 target = _invProj * glm::vec4(a, b, 1, 1);
    glm::vec3 dir = glm::vec3(_invView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space

    return Ray(transform.GetLocation(), dir);
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
    _view = glm::lookAt(transform.GetLocation(), transform.GetLocation() + transform.Forward(), transform.Up());    
    _proj = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
    _viewProj = _proj * _view;

    _invView = inverse(_view);
    _invProj = inverse(_proj);
}
