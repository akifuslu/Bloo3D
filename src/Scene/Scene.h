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
        inline void AddObject(Object* obj)
        {
            switch(obj->type)
            {
                case ObjectType::MESH:
                _meshes.push_back(dynamic_cast<Mesh*>(obj));
                break;
                case ObjectType::LIGHT:
                _lights.push_back(dynamic_cast<Light*>(obj));
                break;
                default:
                break;
            }
            _objects.push_back(obj);
        }
        inline void RemoveObject(Object* obj)
        {
            switch(obj->type)
            {
                case ObjectType::MESH:
                _meshes.remove(dynamic_cast<Mesh*>(obj));
                break;
                case ObjectType::LIGHT:
                _lights.remove(dynamic_cast<Light*>(obj));
                break;
                default:
                break;
            }
            _objects.remove(obj);
        }
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
        ReactiveProperty<Object*> selectedObject;
    private:
        std::list<Object*> _objects;
        std::list<Mesh*> _meshes;
        std::list<Light*> _lights;
};
