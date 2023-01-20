#pragma once

struct TextureProperties
{
    int Unit;
    int Width;
    int Height;
    int NumChannels;
};

class Texture
{
    public:
        static Texture* Create(const TextureProperties& props);
        virtual ~Texture() = default;
        virtual void Write(unsigned char* pixels) = 0;
        virtual void Bind(int unit) = 0;
        virtual void UnBind() = 0;
};
