#include "GLVertexArray.h"
#include "GPU/Buffer.h"
#include "GPU/VertexBufferLayout.h"
#include <GL/glew.h>

GLVertexArray::GLVertexArray()
{
    glGenVertexArrays(1, &_rendererId);
    glBindVertexArray(_rendererId);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &_rendererId);
}

void GLVertexArray::Bind()
{
    glBindVertexArray(_rendererId);
}

void GLVertexArray::UnBind()
{
    glBindVertexArray(0);
}

void GLVertexArray::AddBuffer(VertexBuffer* buffer, const VertexBufferLayout& layout)
{
    buffer->Bind();
    const auto& elements = layout.GetElements();
    
    size_t offset = 0;
    for (size_t i = 0; i < elements.size(); i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, elements[i].Count, elements[i].Type, elements[i].Normalized, layout.GetStride(), (const void*)offset);
        offset += elements[i].Count * elements[i].Size;
    }
}

