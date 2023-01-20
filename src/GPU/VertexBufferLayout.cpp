#include "VertexBufferLayout.h"
#include <GL/glew.h>

VertexBufferLayout::VertexBufferLayout()
{
    _stride = 0;
}

template<>
void VertexBufferLayout::Push<float>(int count)
{
    _elements.push_back(VertexBufferLayoutElement{GL_FLOAT, count, GL_FALSE, sizeof(float)});
    _stride += count * sizeof(float);
}

const std::vector<VertexBufferLayoutElement>& VertexBufferLayout::GetElements() const
{
    return _elements;
}

unsigned int VertexBufferLayout::GetStride() const
{
    return _stride;
}

