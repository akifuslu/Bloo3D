#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "pch.h"

enum class ObjectType
{
    EMPTY,
    MESH,
    LIGHT,
    CAMERA
};

class Object
{
    public:
        Object();
        Object(const std::string& name);
        std::string Name;
        ObjectType Type;
        void SetLocation(glm::vec3 loc);
        void SetRotation(glm::quat rot);
        void SetRotationEuler(glm::vec3 rot);
        void SetScale(glm::vec3 sca);
        glm::vec3 GetLocation() const;
        glm::quat GetRotation() const;
        glm::vec3 GetRotationEuler() const;
        glm::vec3 GetScale() const;
        glm::mat4 WorldToLocal() const;
        glm::mat4 LocalToWorld() const;
    protected:
        void BuildMats();
        glm::vec3 QuatToEuler(glm::quat);
        glm::quat EulerToQuat(glm::vec3);
        glm::mat4 _localToWorld;
        glm::mat4 _worldToLocal;
        glm::vec3 _location;
        glm::quat _rotation;
        glm::vec3 _rotationEuler;
        glm::vec3 _scale;
};
