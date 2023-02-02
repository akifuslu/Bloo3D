#include "GLUniformBuffer.h"
#include <GL/glew.h>

GLUniformBuffer::GLUniformBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &_rendererId);
    glBindBuffer(GL_UNIFORM_BUFFER, _rendererId);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
}

GLUniformBuffer::~GLUniformBuffer()
{
    glDeleteBuffers(1, &_rendererId);
}

void GLUniformBuffer::Bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, _rendererId);
}

void GLUniformBuffer::UnBind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformBuffer::BindIndex(int index) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, _rendererId);
}

void GLUniformBuffer::Update(const void* data, unsigned int size) const
{
    Bind();
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
}

