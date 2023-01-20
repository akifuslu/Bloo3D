#pragma once

#include "glm/glm.hpp"
#include "GPU/Shader.h"

#include<string>
#include<unordered_map>

class GLShader : public Shader
{
    public:
        GLShader(const std::string& filepath);
        virtual ~GLShader() override;
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void SetMatrix4(const std::string& name, glm::mat4 matrix) override;
    private:
        unsigned int _rendererId;
        std::unordered_map<std::string, int> _uniforms;
        int GetUniformLocation(const std::string& name);
};
