#pragma once

#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Logger.h"
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
        Object()
        {
            id = s_idCounter++;
            selected = false;
        }
        virtual ~Object() = default;
        std::string name;
        ObjectType type;
        Transform transform;
        bool selected;
        unsigned int id;
    private:
        static unsigned int s_idCounter;
};
