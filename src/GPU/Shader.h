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
};
