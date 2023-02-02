#include "Buffer.h"

#include "GPU/GL/GLVertexBuffer.h"
#include "GPU/GL/GLIndexBuffer.h"
#include "GPU/GL/GLUniformBuffer.h"
#include "Logger.h"

VertexBuffer* VertexBuffer::Create(const void* data, unsigned int size) 
{
    switch (s_RenderAPI)
    {
        case RenderAPI::OPENGL:
            return new GLVertexBuffer(data, size);
        default:
            LOGERROR("Requested RenderAPI is not available");
            return nullptr;
    }
}

IndexBuffer* IndexBuffer::Create(const void* data, unsigned int count) 
{
    switch (s_RenderAPI)
    {
        case RenderAPI::OPENGL:
            return new GLIndexBuffer(data, count);
        default:
            LOGERROR("Requested RenderAPI is not available");
            return nullptr;
    }
}

UniformBuffer* UniformBuffer::Create(const void* data, unsigned int size)
{
    switch (s_RenderAPI)
    {
        case RenderAPI::OPENGL:
            return new GLUniformBuffer(data, size);
        default:
            LOGERROR("Requested RenderAPI is not available");
            return nullptr;
    }
}
