#pragma once

#include "Geometry/Mesh.h"
#include "Light/PointLight.h"
#include "pch.h"

class Scene
{
    public:
        Scene(){}
        std::vector<std::unique_ptr<Mesh>> Meshes;
        std::vector<std::unique_ptr<PointLight>> Lights;
};
