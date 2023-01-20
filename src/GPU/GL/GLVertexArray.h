#pragma once

class VertexBuffer;
class VertexBufferLayout;

class GLVertexArray{
    public:
        GLVertexArray();
        ~GLVertexArray();
        void Bind();
        void UnBind();
        void AddBuffer(VertexBuffer* buffer, const VertexBufferLayout& layout);
    private:
        unsigned int _rendererId;
};

