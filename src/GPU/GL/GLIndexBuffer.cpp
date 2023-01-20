#include "GLIndexBuffer.h"
#include <GL/glew.h>

GLIndexBuffer::GLIndexBuffer(const void* data, unsigned int count)
    : _count(count)
{
    auto size = _count * sizeof(unsigned int);
    glGenBuffers(1, &_rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &_rendererId);
}

void GLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
}

void GLIndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int GLIndexBuffer::GetCount() const
{
    return _count;
}
