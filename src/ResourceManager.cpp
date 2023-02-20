#include "ResourceManager.h"

#include "ImageLoader.h"


void ResourceManager::LoadInternals()
{
    for(auto& r: _internalTexLookup)
    {
        int w, h, ch;
        unsigned char* data = ImageLoader::Load(r.second, w, h, ch);
        std::shared_ptr<Texture> tex;
        tex.reset(Texture::Create({
            .width = w,
            .height = h,
            .numChannels = ch
        }));
        tex->Write(data);
        _internalTextures[r.first] = tex;
        ImageLoader::Free(data);
    }
}
