#include "ScreenQuad.h"


ScreenQuad::ScreenQuad() 
{
    float pos[] = 
    {
        -1.0f,  1.0f, 0.0f, 0.0f, // ul
         1.0f,  1.0f, 1.0f, 0.0f, // ur
         1.0f, -1.0f, 1.0f, 1.0f, // lr
        -1.0f, -1.0f, 0.0f, 1.0f, // ll
    };

    unsigned int inds[] = 
    {
        0, 1, 2,
        0, 2, 3
    };

    renderInfo.vb.reset(VertexBuffer::Create(pos, 16 * sizeof(float)));
    renderInfo.ib.reset(IndexBuffer::Create(inds, 6));

    renderInfo.layout.reset(new VertexBufferLayout());
    renderInfo.layout->Push<float>(2);
    renderInfo.layout->Push<float>(2);
}

