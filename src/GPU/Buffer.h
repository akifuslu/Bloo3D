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
        virtual ~VertexBuffer() = default;        
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void Update(const void* data, unsigned int size) const = 0;
};

class IndexBuffer
{
    public:
        static IndexBuffer* Create(const void* data, unsigned int count);
        virtual ~IndexBuffer() = default;        
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual int GetCount() const = 0;
        virtual void Update(const void* data, unsigned int count) = 0;
};

class UniformBuffer
{
    public:
        static UniformBuffer* Create(const void* data, unsigned int size);
        virtual ~UniformBuffer() = default;
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void BindIndex(int index) const = 0;
        virtual void Update(const void* data, unsigned int size) const = 0;        
};
