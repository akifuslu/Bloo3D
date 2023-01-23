#pragma once

#include "pch.h"

struct VertexBufferLayoutElement
{
    int Type;
    int Count;
    int Normalized;
    int Size;
};


class VertexBufferLayout
{
    public:
        VertexBufferLayout();
        template <typename T> 
        void Push(int count);
        const std::vector<VertexBufferLayoutElement>& GetElements() const;
        unsigned int GetStride() const;
    private:
        std::vector<VertexBufferLayoutElement> _elements;
        unsigned int _stride;
};
