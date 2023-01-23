#pragma once

#include "GPU/Buffer.h"
#include "pch.h"

class GLVertexBuffer : public VertexBuffer
{
    public:
        GLVertexBuffer(const void* data, unsigned int size);
        virtual ~GLVertexBuffer();
        virtual void Bind() const override;
        virtual void UnBind() const override;
    private:
        unsigned int _rendererId;
};
