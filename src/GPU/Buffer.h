#pragma once


enum class RenderAPI
{
    NONE,
    OPENGL,
    VULKAN
};

static RenderAPI s_RenderAPI = RenderAPI::OPENGL;

class VertexBuffer
{
    public:
        static VertexBuffer* Create(const void* data, unsigned int size);
        virtual ~VertexBuffer();        
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
};

class IndexBuffer
{
    public:
        static IndexBuffer* Create(const void* data, unsigned int count);
        virtual ~IndexBuffer();        
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual int GetCount() const = 0;
};

