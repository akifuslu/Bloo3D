#pragma once

#include "GPU/Buffer.h"
#include "pch.h"

class GLUniformBuffer : public UniformBuffer
{
    public:
        GLUniformBuffer(const void* data, unsigned int size);
        virtual ~GLUniformBuffer();
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void BindIndex(int index) const override;
        virtual void Update(const void* data, unsigned int size) const override;
    private:
        unsigned int _rendererId;
};
