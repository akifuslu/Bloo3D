#include "Scene.h"
#include "Input.h"
#include "Styles.h"
#include "glm/gtx/string_cast.hpp"

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

Object* Scene::PickObject()
{
    // pick object using mouse coordinates

    // we will use raycasting for meshes
    auto pos = Input::GetMousePosition();
    auto ray = editorCamera->ScreenPointToRay(pos);
    RayHit hit;
    bool meshHit = Raycast(ray, &hit);

    // since lights are point, we will use the screen space distance to decide its a pick
    float ldist = FLT_MAX;
    bool lightHit = false;
    Light* cLight = nullptr;
    for(auto& light: _lights)
    {
        glm::vec4 spos = editorCamera->GetViewProj() * light->transform.LocalToWorld() * glm::vec4(0, 0, 0, 1);
        glm::vec2 lpos(spos.x, spos.y);
        lpos /= spos.w;
        lpos.x = (lpos.x + 1) * editorCamera->GetWidth() / 2;
        lpos.y = (1 - lpos.y) * editorCamera->GetHeight() / 2;
        if(distance(glm::vec2(pos), lpos) < Styles::PointSize)
        {
            lightHit = true;
            float dist = distance(light->transform.GetLocation(), editorCamera->transform.GetLocation());
            if(dist < ldist)
            {
                ldist = dist;
                cLight = light;
            }
        }
    }

    // pick the closest mesh or light
    if(meshHit && lightHit)
    {
        return hit.distance < ldist ? hit.object : cLight;
    }
    else if(meshHit)
    {
        return hit.object;
    }
    else if(lightHit)
    {
        return cLight;
    }
    return nullptr;
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
    activeObject.Set(nullptr);
    for(auto& s: selectedObjects)
    {
        s->selected = false;
    }
    selectedObjects.clear();
}
