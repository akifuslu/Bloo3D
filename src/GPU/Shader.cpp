#include "Shader.h"

#include "GPU/GL/GLShader.h"

Shader* Shader::Create(const std::string& filepath) 
{
    return new GLShader(filepath);
}
