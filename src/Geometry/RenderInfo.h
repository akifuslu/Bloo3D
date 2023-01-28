#pragma once

#include "GPU/Buffer.h"
#include "GPU/VertexBufferLayout.h"
#include "pch.h"

struct RenderInfo
{
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<IndexBuffer> ib;
    std::unique_ptr<VertexBufferLayout> layout;
    bool needUpdate = false;
};
