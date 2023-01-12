#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Ray.h"
#include "PointLight.h"

#include <iostream>
#include <future>
#include <thread>

Renderer::Renderer(std::shared_ptr<Camera> camera, unsigned char* buffer, int width, int height)
{
    _camera = camera;
    _buffer = buffer;
    _width = width;
    _height = height;
    _size = _width * _height;
}

Renderer::~Renderer()
{
    
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
    color = glm::clamp(color, 0.0f, 255.0f);
    pixel[0] = color.r;
    pixel[1] = color.g;
    pixel[2] = color.b;
    pixel[3] = 255;
}

void Renderer::AddMesh(std::shared_ptr<Mesh> mesh)
{
    _meshes.push_back(mesh);
}

void Renderer::AddLight(std::shared_ptr<PointLight> light)
{
    _lights.push_back(light);
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
    _finishedThreads = 0;
    int cores = std::thread::hardware_concurrency();
    while(cores--)
    {
        _futures.push_back(std::async(&Renderer::RenderInternal, this));
    }

    auto time = std::chrono::high_resolution_clock().now();
    _futureCompletion = std::async([=, start = time]()
    {
        while(true)
        {
            if(_finishedThreads == _futures.size())
            {
                _futures.clear();
                _onRender = false;
                std::chrono::duration<double, std::milli> deltaTime = std::chrono::high_resolution_clock().now() - start;
                std::cout << "Render Finished" << std::endl;
                std::cout << deltaTime.count() << "ms" << std::endl;
                break;
            }
        }

    });
}

void Renderer::RenderInternal()
{
    while(_pixelIndex < _size)
    {
        int index = _pixelIndex++;
        int x = index % _width;
        int y = index / _width;                            

        auto ray = _camera->GetRay(x, y);
        RayHit hit;
        if(RayCast(ray, &hit))
        {
            glm::vec3 color(0);
            for (size_t k = 0; k < _lights.size(); k++)
            {
                glm::vec3 lightDir = normalize(_lights[k]->GetPos() - hit.Point);
                glm::vec3 luminance = _lights[k]->GetLuminance(hit.Point);
                // DIFFUSE
                float teta = dot(lightDir, hit.Normal);
                teta = teta < 0 ? 0 : teta;
                color += luminance * teta * 255.0f;                                                
            }
            AssignRGBFromVec3(_buffer + index * 4, color);
        }
        else
        {
            AssignRGBA(_buffer + index * 4, 0, 0, 0, 255);
        }
    }
    _finishedThreads++;    
}
