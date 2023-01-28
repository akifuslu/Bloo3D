#include "GLTexture.h"

#include <GL/glew.h>

GLTexture::GLTexture(const TextureProperties& props)
    : _unit(props.unit), _width(props.width), _height(props.height)
{
    glGenTextures(1, &_rendererId);
    glActiveTexture(GL_TEXTURE0 + _unit);    
    glBindTexture(GL_TEXTURE_2D, _rendererId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &_rendererId);
}

void GLTexture::Bind(int unit)
{
    glBindTexture(GL_TEXTURE_2D, _rendererId);
}

void GLTexture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::Write(unsigned char* pixels)
{
    Bind(0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void GLTexture::Resize(int width, int height)
{
    _width = width;
    _height = height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}
