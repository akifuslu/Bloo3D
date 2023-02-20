#include "Camera.h"

#include "Geometry/Ray.h"
#include "Logger.h"
#include "Input.h"
#include "glm/matrix.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <glm/gtx/string_cast.hpp>


Camera::Camera( float fov,
                float near,
                float far,
                u_int32_t width,
                u_int32_t height) 
    : Object(), _fov(fov), _near(near), _far(far), _width(width), _height(height)
{
    projection = CameraProjection::PERSPECTIVE; // default
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

Ray Camera::ScreenPointToRay(glm::ivec2 point)
{
    int x = point.x;
    int y = _height - point.y;
    return GetRay(x, y);
}


void Camera::OnUpdate()
{
    if(Input::MouseOnUI() || Input::KeyboardOnUI()) // do not transform camera if user interacting with ui
        return;

    auto delta = Input::GetMouseScroll();
    if(length(delta) > 0)
    {
        if(Input::GetKey(KeyCode::SHIFT)) // pan
        {
            auto mov = transform.Right() * -delta.x + transform.Up() * delta.y;
            transform.SetLocation(transform.GetLocation() + mov * 0.1f);
        }
        else if(Input::GetKey(KeyCode::CONTROL)) // zoom
        {
            float f = length(delta);
            if(abs(delta.x) > abs(delta.y))
            {
                f *= delta.x < 0 ? -1 : 1;
            }
            else
            {
                f *= delta.y < 0 ? -1 : 1;
            }
            transform.SetLocation(transform.GetLocation() + transform.Forward() * f);
            _fdist = length(transform.GetLocation());
        }
        else // orbit
        {
            auto rot = glm::vec3(delta.y, delta.x, 0);
            auto center = transform.GetLocation() + transform.Forward() * 5.0f;
            transform.SetLocation(center);
            transform.SetRotation(transform.GetRotation() + rot);
            auto loc = center - transform.Forward() * 5.0f;
            transform.SetLocation(loc);
        }
    }

    if(Input::GetKeyDown(KeyCode::N5))
    {
        // toggle pers/ortho
        projection = (projection == CameraProjection::PERSPECTIVE) ? 
                     CameraProjection::ORTHOGRAPHIC : CameraProjection::PERSPECTIVE;
        _fdist = length(transform.GetLocation());
        RebuildMatrix();
    }
    else if(Input::GetKeyDown(KeyCode::N1)) // front view
    {
        _fdist = length(transform.GetLocation());
        projection = CameraProjection::ORTHOGRAPHIC;
        transform.SetRotation({0, 0, 0});
    }
    else if(Input::GetKeyDown(KeyCode::N3)) // right view
    {
        _fdist = length(transform.GetLocation());
        projection = CameraProjection::ORTHOGRAPHIC;
        transform.SetRotation({0, -90, 0});
    }
    else if(Input::GetKeyDown(KeyCode::N7)) // top view
    {
        _fdist = length(transform.GetLocation());
        projection = CameraProjection::ORTHOGRAPHIC;
        transform.SetRotation({90, 0, 0});
    }

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
    _view = glm::lookAtLH(transform.GetLocation(), transform.GetLocation() + transform.Forward(), transform.Up());    
    if(projection == CameraProjection::PERSPECTIVE)
    {
        _proj = glm::perspectiveLH(glm::radians(_fov), _aspect, _near, _far);
    }
    else
    {
        float ratioSize = atan(glm::radians(_fov / 2.0f)) * 2.0f;
        float distance = _fdist;
        float y = ratioSize * distance;
        float x = ratioSize * distance * _aspect;
        _proj = glm::orthoLH(-x, x, -y, y, -_far, _far);
    }
    _viewProj = _proj * _view;
    _invView = inverse(_view);
    _invProj = inverse(_proj);
}
