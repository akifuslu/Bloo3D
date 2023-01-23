#pragma once

#include "GL/glew.h"

#include "pch.h"

namespace ShaderLoader
{
    unsigned int CreateShader(const std::string& source, int type)
    {
        int length = source.length();
        const char *shader = (const char *)source.c_str();

        unsigned int vs = glCreateShader(type);
        glShaderSource(vs, 1, &shader, &length);
        glCompileShader(vs);

        char output[1024] = {0};
        glGetShaderInfoLog(vs, 1024, &length, output);
        std::string stype = (type == GL_VERTEX_SHADER ? "VS" : "FS");
        std::cout << stype << " compile log:" << output << std::endl;

        return vs;
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

    unsigned int Load(const std::string& filepath) 
    {
        ShaderSources sources = ParseShaderSource(filepath);
        unsigned int vs = CreateShader(sources.vsSource, GL_VERTEX_SHADER);
        unsigned int fs = CreateShader(sources.fsSource, GL_FRAGMENT_SHADER);

        unsigned int id;
        id = glCreateProgram();

        glAttachShader(id, vs);
        glAttachShader(id, fs);

        glLinkProgram(id);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return id;
    }
}
