#include "Buffer.h"

#include "GPU/GL/GLVertexBuffer.h"
#include "GPU/GL/GLIndexBuffer.h"

VertexBuffer* VertexBuffer::Create(const void* data, unsigned int size) 
{
    switch (s_RenderAPI)
    {
        case RenderAPI::OPENGL:
            return new GLVertexBuffer(data, size);
        default:
            return nullptr;
    }
    // TODO: log message about not supported APIs
    return nullptr;
}

VertexBuffer::~VertexBuffer() 
{
    
}

IndexBuffer* IndexBuffer::Create(const void* data, unsigned int count) 
{
    switch (s_RenderAPI)
    {
        case RenderAPI::OPENGL:
            return new GLIndexBuffer(data, count);
        default:
            return nullptr;
    }
    // TODO: log message about not supported APIs
    return nullptr;    
}

IndexBuffer::~IndexBuffer() 
{
    
}
