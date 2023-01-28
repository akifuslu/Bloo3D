#pragma once

#include "Transform.h"
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
        Object() = default;
        Object(const std::string& name) : name(name) {}
        std::string name;
        ObjectType type;
        Transform transform;
};
