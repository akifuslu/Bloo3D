#pragma once

#include "Camera/Camera.h"
#include "Geometry/Mesh.h"
#include "Light/Light.h"
#include "pch.h"

class Scene
{
    public:
        Scene(){}
        std::vector<Mesh*> meshes;
        Camera* mainCam;
};
