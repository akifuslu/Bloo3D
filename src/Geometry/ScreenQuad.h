#pragma once

#include "IDrawable.h"
#include "pch.h"

class ScreenQuad : public IDrawable
{
    public:
        ScreenQuad();
        virtual ~ScreenQuad() override;    
};
