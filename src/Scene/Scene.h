#pragma once

#include "Camera/Camera.h"
#include "Geometry/Mesh.h"
#include "Light/Light.h"
#include "Utils/ReactiveProperty.h"
#include "pch.h"

class Scene
{
    public:
        Scene(){}
        std::vector<Object*> objects;
        std::vector<Mesh*> meshes;
        Camera* mainCam;
        ReactiveProperty<Object*> selectedObject;
    
};
