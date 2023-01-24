#pragma once

#include "GPU/Texture.h"

class GLTexture : public Texture
{
    public:
        GLTexture(const TextureProperties& props);
        virtual ~GLTexture() override;
        virtual void Write(unsigned char* pixels) override;
        virtual void Resize(int width, int height) override;
        virtual void Bind(int unit) override;
        virtual void UnBind() override;
    private:
        unsigned int _rendererId;
        unsigned int _unit;
        int _width;
        int _height;
};
