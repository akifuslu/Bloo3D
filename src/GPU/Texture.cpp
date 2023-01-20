#include "Texture.h"

#include "GPU/GL/GLTexture.h"

Texture* Texture::Create(const TextureProperties& props) 
{
    return new GLTexture(props);
}
