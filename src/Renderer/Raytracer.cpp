#include "Raytracer.h"
#include "Geometry/Mesh.h"
#include "Camera/Camera.h"
#include "Geometry/Ray.h"
#include "Light/Light.h"
#include "Material/Material.h"
#include "GPU/Texture.h"
#include "Scene/Scene.h"


Raytracer::Raytracer(Camera* camera, Texture* target)
{
    _target = target;
    _camera = camera;
    _buffer.resize(camera->GetWidth() * camera->GetHeight() * 4);
}

Raytracer::~Raytracer()
{
}

void Raytracer::OnResize(int width, int height)
{
    _buffer.resize(width * height * 4);
}

int Raytracer::AddMaterial(MaterialBase* mat)
{
    _mats.push_back(mat);
    return _mats.size() - 1;
}

void Raytracer::Refresh()
{
    // if(_onRender)
    // {
    //     _pixelIndex = 0;
    // }
    // else
    // {
    //     Render();
    // }
}

void Raytracer::Render(Scene* scene) 
{
    std::cout << "Render Started" << std::endl;
    _onRender = true;
    _pixelIndex = 0;
    int cores = std::thread::hardware_concurrency() - 1;
    while(cores--)
    {
        _pool.push_task(&Raytracer::RenderInternal, this, scene);
    }
    auto starttime = std::chrono::high_resolution_clock().now();
    _pool.wait_for_tasks();
    _onRender = false;
    _target->Write(_buffer.data());
    auto endtime = std::chrono::high_resolution_clock().now();
    std::chrono::duration<double, std::milli> deltaTime = endtime - starttime;
    std::cout << "Render Finished" << std::endl;
    std::cout << deltaTime.count() << "ms" << std::endl;
}

glm::vec3 Raytracer::GetFragColor(Scene* scene, const Ray& ray, const RayHit& frag)
{
    glm::vec3 color(0.0f);
    if(frag.matIndex == -1) // no material assigned
    {
        return color;
    }

    for(auto& light: scene->GetLights())
    {
        glm::vec3 lightDir = normalize(light->transform.GetLocation() - frag.point);
        // shadow check here
        glm::vec3 luminance = light->GetAttenuation(frag.point);
        color += _mats[frag.matIndex]->Shade({
            .normal = normalize(frag.normal),
            .lightDir = normalize(lightDir),
            .halfDir = normalize(frag.normal + lightDir),
            .viewDir = normalize(-ray.dir),
            .radiance = luminance
        });
    }
    
    return color;
}

void Raytracer::RenderInternal(Scene* scene)
{
    u_int32_t size = _camera->GetWidth() * _camera->GetHeight();
    u_int32_t width = _camera->GetWidth();
    while(true)
    {
        u_int32_t index = _pixelIndex++;
        if(index >= size) // I AM STUPID (Tribute to charles leclerc) (There was a threading error here I was checking for this before incrementing the index)
        {
            return;
        }
        u_int32_t x = index % width;
        u_int32_t y = index / width;                            

        auto ray = _camera->GetRay(x, y);
        RayHit hit;
        if(scene->Raycast(ray, &hit))
        {
            auto color = GetFragColor(scene, ray, hit);
            color *= 255.0f;
            color = glm::clamp(color, 0.0f, 255.0f);
            _buffer[index * 4] = color.x;
            _buffer[index * 4 + 1] = color.y;
            _buffer[index * 4 + 2] = color.z;
            _buffer[index * 4 + 3] = 255;
        }
        else
        {
            _buffer[index * 4] = 0;
            _buffer[index * 4 + 1] = 0;
            _buffer[index * 4 + 2] = 0;
            _buffer[index * 4 + 3] = 255;
        }
    }
}
