#include "Scene.h"


bool Scene::Raycast(const Ray& ray, RayHit* hit)
{
    bool hasHit = false;
    for (auto& mesh: _meshes)
    {
        RayHit nhit;
        if(mesh->RayCast(ray, &nhit))
        {
            if(nhit.distance < hit->distance)
            {
                *hit = nhit;
                hasHit = true;
            }
        }
    }
    return hasHit;
}

void Scene::AddObject(Object* obj)
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

void Scene::RemoveObject(Object* obj)
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

void Scene::SelectObject(Object* obj, bool additive)
{
    if(additive)
    {
        if(obj->selected)
        {
            activeObject.Set(obj);
        }
    }
    else
    {
        DeselectAll();
        activeObject.Set(obj);        
    }
    obj->selected = true;
    selectedObjects.push_back(obj);
}

void Scene::DeselectAll()
{
    for(auto& s: selectedObjects)
    {
        s->selected = false;
    }
    selectedObjects.clear();
}
