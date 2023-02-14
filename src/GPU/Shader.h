#pragma once

#include "glm/glm.hpp"
#include "pch.h"

class Shader
{
    public:
        static Shader* Create(const std::string& filepath);
        virtual ~Shader() = default;
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void SetMatrix4(const std::string& name, glm::mat4 matrix) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetVec3(const std::string& name, glm::vec3 value) = 0;
        virtual void SetVec2(const std::string& name, glm::vec2 value) = 0;
        virtual void SetUniformBlockBinding(const std::string& name, int index) const = 0;
};
