#include "Renderer.h"
#include "Geometry/Mesh.h"
#include "Camera/Camera.h"
#include "Geometry/Ray.h"
#include "Light/PointLight.h"
#include "Material/Material.h"
#include "GPU/Texture.h"


Renderer::Renderer(Camera* camera, Texture* target)
{
    _target = target;
    _camera = camera;
    _buffer.resize(camera->GetWidth() * camera->GetHeight() * 4);
}

Renderer::~Renderer()
{
}

void Renderer::OnResize(int width, int height)
{
    _buffer.resize(width * height * 4);
}

void AssignRGBA(unsigned char* pixel, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    pixel[3] = a;
}

void AssignRGBFromVec3(unsigned char* pixel, glm::vec3 color)
{
    color *= 255.0f;
    color = glm::clamp(color, 0.0f, 255.0f);
    pixel[0] = color.r;
    pixel[1] = color.g;
    pixel[2] = color.b;
    pixel[3] = 255;
}

int Renderer::AddMesh(Mesh* mesh)
{
    _meshes.push_back(mesh);
    return _meshes.size() - 1;
}

int Renderer::AddLight(PointLight* light)
{
    _lights.push_back(light);
    return _lights.size() - 1;
}

int Renderer::AddMaterial(MaterialBase* mat)
{
    _mats.push_back(mat);
    return _mats.size() - 1;
}

void Renderer::Refresh()
{
    if(_onRender)
    {
        _pixelIndex = 0;
    }
    else
    {
        Render();
    }
}

bool Renderer::RayCast(const Ray& ray, RayHit* hit)
{
    bool hasHit = false;
    for (size_t i = 0; i < _meshes.size(); i++)
    {
        RayHit nhit;
        if(_meshes[i]->RayCast(ray, &nhit))
        {
            if(nhit.Distance < hit->Distance)
            {
                *hit = nhit;
                hasHit = true;
            }
        }
    }
    return hasHit;
}

void Renderer::Render() 
{
    std::cout << "Render Started" << std::endl;
    _onRender = true;
    _pixelIndex = 0;
    int cores = std::thread::hardware_concurrency() - 1;
    while(cores--)
    {
        _pool.push_task(&Renderer::RenderInternal, this);
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

glm::vec3 Renderer::GetFragColor(const Ray& ray, const RayHit& frag)
{
    glm::vec3 color(0.0f);
    if(frag.MatIndex == -1) // no material assigned
    {
        return color;
    }

    for (size_t i = 0; i < _lights.size(); i++)
    {
        glm::vec3 lightDir = normalize(_lights[i]->GetPos() - frag.Point);
        // shadow check here
        glm::vec3 luminance = _lights[i]->GetLuminance(frag.Point);
        color += _mats[frag.MatIndex]->Shade({
            .Normal = normalize(frag.Normal),
            .LightDir = normalize(lightDir),
            .HalfDir = normalize(frag.Normal + lightDir),
            .ViewDir = normalize(-ray.Dir),
            .Radiance = luminance
        });
    }
    
    return color;
}

void Renderer::RenderInternal()
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
        if(RayCast(ray, &hit))
        {
            auto color = GetFragColor(ray, hit);
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