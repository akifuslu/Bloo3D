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
        bool Raycast(const Ray& ray, RayHit* hit);
        void AddObject(Object* obj);
        void RemoveObject(Object* obj);
        void SelectObject(Object* obj, bool additive=false);
        void DeselectAll();
        inline const std::list<Object*>& GetObjects() const
        {
            return _objects;
        }
        inline const std::list<Mesh*>& GetMeshes() const
        {
            return _meshes;
        }
        inline const std::list<Light*>& GetLights() const
        {
            return _lights;
        }
        Camera* editorCamera;
        DirectionalLight* editorLight;
        ReactiveProperty<Object*> activeObject;
        std::list<Object*> selectedObjects;
    private:
        std::list<Object*> _objects;
        std::list<Mesh*> _meshes;
        std::list<Light*> _lights;
};
