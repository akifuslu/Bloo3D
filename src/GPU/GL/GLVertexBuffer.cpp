#include "GLVertexBuffer.h"
#include <GL/glew.h>

GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &_rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &_rendererId);
}

void GLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
}

void GLVertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
