#pragma once

#include "stb_image.h"

#include "pch.h"

namespace ImageLoader
{

    unsigned char* Load(const std::string& filepath, int& width, int& height, int& channels)
    {
        return stbi_load(filepath.c_str(), &width, &height, &channels, 4);
    }
}

