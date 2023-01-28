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
        Object(const std::string& name) : Name(name) {}
        std::string Name;
        ObjectType Type;
        inline Transform* GetTransform() const
        {
            return _transform.get();
        }
    private:
        std::unique_ptr<Transform> _transform;
};
