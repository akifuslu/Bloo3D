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

    _vb.reset(VertexBuffer::Create(pos, 16 * sizeof(float)));
    _ib.reset(IndexBuffer::Create(inds, 6));

    _layout.reset(new VertexBufferLayout());
    _layout->Push<float>(2);
    _layout->Push<float>(2);
}

ScreenQuad::~ScreenQuad() 
{
    
}
