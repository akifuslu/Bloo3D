#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <string>

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
        void SetScale(glm::vec3 sca);
        glm::vec3 GetLocation();
        glm::quat GetRotation();
        glm::vec3 GetScale();
    protected:
        void BuildMats();
        glm::mat4 _localToWorld;
        glm::mat4 _worldToLocal;
        glm::vec3 _location;
        glm::quat _rotation;
        glm::vec3 _scale;
};
