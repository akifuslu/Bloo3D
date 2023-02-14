#pragma once

#include "glm/glm.hpp"
#include "GPU/Shader.h"

#include "pch.h"

class GLShader : public Shader
{
    public:
        GLShader(const std::string& filepath);
        virtual ~GLShader() override;
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void SetMatrix4(const std::string& name, glm::mat4 matrix) override;
        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetVec3(const std::string& name, glm::vec3 value) override;
        virtual void SetVec2(const std::string& name, glm::vec2 value) override;
        virtual void SetUniformBlockBinding(const std::string& name, int index) const override;
    private:
        std::string _name;
        unsigned int _rendererId;
        std::unordered_map<std::string, int> _uniforms;
        int GetUniformLocation(const std::string& name);
};
