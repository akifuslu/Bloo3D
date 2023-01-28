#pragma once

struct TextureProperties
{
    int unit;
    int width;
    int height;
    int numChannels;
};

class Texture
{
    public:
        static Texture* Create(const TextureProperties& props);
        virtual ~Texture() = default;
        virtual void Write(unsigned char* pixels) = 0;
        virtual void Resize(int width, int height) = 0;
        virtual void Bind(int unit) = 0;
        virtual void UnBind() = 0;
};
