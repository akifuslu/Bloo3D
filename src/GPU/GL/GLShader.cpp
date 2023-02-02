#include "GLShader.h"
#include "GL/glew.h"

#include "Logger.h"
#include "pch.h"
#include <filesystem>

unsigned int CreateShader(const std::string& name, const std::string& source, int type)
{
    int length = source.length();
    const char *shader = (const char *)source.c_str();

    unsigned int vs = glCreateShader(type);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    int outLen = 0;
    glGetShaderInfoLog(vs, 1024, &outLen, output);
    if(outLen == 0)
    {
        return vs; // no error
    }
    // report error
    std::string stype = (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
    LOGERROR("Shader compile error in {} stage {} \n {}", name, stype, output);

    return vs; // TODO: In case of error default fallback shader must be used!!
}

struct ShaderSources{
    std::string vsSource;
    std::string fsSource;
};

ShaderSources ParseShaderSource(const std::string& path)
{
    enum class ShaderMode
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::fstream fs(path);
    std::string line;
    ShaderMode mode;
    std::stringstream sources[2];
    while(std::getline(fs, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("VERTEX") != std::string::npos)
            {
                mode = ShaderMode::VERTEX;
            }
            else if(line.find("FRAGMENT") != std::string::npos)
            {
                mode = ShaderMode::FRAGMENT;
            }
            else
            {
                mode = ShaderMode::NONE;
            }
        }
        else
        {
            sources[(int)mode] << line << std::endl;
        }
    }

    return {sources[0].str(), sources[1].str()};
}

GLShader::GLShader(const std::string& filepath) 
{
    _name = std::filesystem::path(filepath).filename();
    ShaderSources sources = ParseShaderSource(filepath);
    unsigned int vs = CreateShader(_name, sources.vsSource, GL_VERTEX_SHADER);
    unsigned int fs = CreateShader(_name, sources.fsSource, GL_FRAGMENT_SHADER);

    _rendererId = glCreateProgram();

    glAttachShader(_rendererId, vs);
    glAttachShader(_rendererId, fs);

    glLinkProgram(_rendererId);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

GLShader::~GLShader() 
{
    glDeleteProgram(_rendererId);    
}

void GLShader::Bind() const
{
    glUseProgram(_rendererId);
}

void GLShader::UnBind() const
{
    glUseProgram(0);
}

void GLShader::SetMatrix4(const std::string& name, glm::mat4 matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void GLShader::SetFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void GLShader::SetVec3(const std::string& name, glm::vec3 value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void GLShader::SetUniformBlockBinding(const std::string& name, int index) const
{
    auto i = glGetUniformBlockIndex(_rendererId, name.c_str());
    glUniformBlockBinding(_rendererId, i, index);
}


int GLShader::GetUniformLocation(const std::string& name) 
{
    auto cached = _uniforms.find(name);
    if(cached != _uniforms.end())
    {
        return cached->second;
    }

    int loc = glGetUniformLocation(_rendererId, name.c_str());    
    if(loc == -1)
    {
        LOGERROR("Uniform variable {} not found in {}", name, _name);
        return -1;
    }

    _uniforms[name] = loc;

    return loc;
}
