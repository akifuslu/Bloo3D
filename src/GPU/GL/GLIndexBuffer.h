#pragma once

#include "GPU/Buffer.h"

class GLIndexBuffer : public IndexBuffer
{
    public:
        GLIndexBuffer(const void* data, unsigned int count);
        virtual ~GLIndexBuffer() override;
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual int GetCount() const override;
        virtual void Update(const void* data, unsigned int count) override;
    private:
        unsigned int _rendererId;
        unsigned int _count;
};

