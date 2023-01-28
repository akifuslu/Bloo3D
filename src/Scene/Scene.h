#pragma once

#include "Geometry/Mesh.h"
#include "Light/Light.h"
#include "pch.h"

class Scene
{
    public:
        Scene(){}
        std::vector<Mesh*> meshes;
        std::vector<Light*> lights;
};
