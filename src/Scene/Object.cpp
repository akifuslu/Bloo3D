#include "Object.h"

#include "glm/gtx/transform.hpp"

Object::Object() 
    : _location(0), _rotation(glm::identity<glm::quat>()), _scale(1.0), _rotationEuler(0)
{
    BuildMats();
}

Object::Object(const std::string& name) 
    : Name(name), _location(0), _rotation(glm::identity<glm::quat>()), _scale(1.0), _rotationEuler(0)
{
    BuildMats();
}

void Object::SetLocation(glm::vec3 loc) 
{
    _location = loc;
    BuildMats();
}

void Object::SetRotation(glm::quat rot) 
{
    _rotation = rot;
    _rotationEuler = QuatToEuler(rot);
    BuildMats();
}

void Object::SetRotationEuler(glm::vec3 rot) 
{
    _rotationEuler = rot;
    _rotation = EulerToQuat(rot);
    BuildMats();
}

void Object::SetScale(glm::vec3 sca) 
{
    _scale = sca;
    BuildMats();
}

glm::vec3 Object::GetLocation() const
{
    return _location;
}

glm::quat Object::GetRotation() const
{
    return _rotation;
}

glm::vec3 Object::GetRotationEuler() const
{
    return _rotationEuler;
}

glm::vec3 Object::GetScale() const
{
    return _scale;
}

glm::mat4 Object::WorldToLocal() const
{
    return _worldToLocal;
}

glm::mat4 Object::LocalToWorld() const
{
    return _localToWorld;
}

void Object::BuildMats()
{
    _localToWorld = glm::translate(_location) * glm::toMat4(_rotation) * glm::scale(_scale);
    _worldToLocal = inverse(_localToWorld);
}

glm::vec3 Object::QuatToEuler(glm::quat q)
{
    glm::vec3 angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    float cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    angles.x = glm::degrees(angles.x);
    angles.y = glm::degrees(angles.y);
    angles.z = glm::degrees(angles.z);    
    return angles;
}

// roll (x), pitch (Y), yaw (z)
glm::quat Object::EulerToQuat(glm::vec3 euler)
{
    float roll = glm::radians(euler.x);
    float pitch = glm::radians(euler.y);
    float yaw = glm::radians(euler.z);

    float cr = std::cosf(roll * 0.5);
    float sr = std::sinf(roll * 0.5);
    float cp = std::cosf(pitch * 0.5);
    float sp = std::sinf(pitch * 0.5);
    float cy = std::cosf(yaw * 0.5);
    float sy = std::sinf(yaw * 0.5);

    glm::quat q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;    
}

