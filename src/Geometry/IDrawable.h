#pragma once

#include "GPU/Buffer.h"
#include "GPU/VertexBufferLayout.h"
#include "pch.h"

class IDrawable
{
    public:
        virtual ~IDrawable() = default;
        inline VertexBuffer* GetVB()
        {
            return _vb.get();
        }
        inline IndexBuffer* GetIB()
        {
            return _ib.get();
        }
        inline VertexBufferLayout* GetLayout()
        {
            return _layout.get();
        }
    protected:
        std::unique_ptr<VertexBuffer> _vb;
        std::unique_ptr<IndexBuffer> _ib;
        std::unique_ptr<VertexBufferLayout> _layout;
};
