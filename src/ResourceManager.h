#pragma once

#include "GPU/Texture.h"
#include "pch.h"

enum InternalTextureTag
{
    POINT_LIGHT_GIZMO,
    DIRECTIONAL_LIGHT_GIZMO
};

class ResourceManager
{    
    private:
        std::unordered_map<InternalTextureTag, std::string> _internalTexLookup = 
        {
            {POINT_LIGHT_GIZMO, s_BasePath + "/res/textures/point_light.png"}
        };
    public:
        static ResourceManager& Instance()
        {
            static ResourceManager rm;
            return rm;
        }
        ResourceManager(ResourceManager const&) = delete;
        void operator=(ResourceManager const&)  = delete;      
        void LoadInternals();
        inline std::shared_ptr<Texture> GetInternalTexture(InternalTextureTag tag)
        {
            if(_internalTextures.find(tag) != _internalTextures.end())
            {
                return _internalTextures[tag];
            }
            return std::shared_ptr<Texture>(); // not found
        }
    private:
        ResourceManager() {}
        std::unordered_map<InternalTextureTag, std::shared_ptr<Texture>> _internalTextures;        
};
