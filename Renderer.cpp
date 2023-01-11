#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Ray.h"
#include "PointLight.h"

#include <iostream>
#include <future>

Renderer::Renderer(Camera* camera, unsigned char* buffer, int width, int height)
{
    _camera = camera;
    _buffer = buffer;
    _width = width;
    _height = height;
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

void Renderer::AddMesh(const Mesh& mesh)
{
    _meshes.push_back(mesh);
}

void Renderer::AddLight(const PointLight& light)
{
    _lights.push_back(light);
}

void Renderer::Refresh()
{
    if(_onRender)
    {
        _refresh = true;
    }
    else
    {
        Render();
    }
}

bool Renderer::RayCast(const Ray& ray, RayHit& hit)
{
    bool hasHit = false;
    for (size_t i = 0; i < _meshes.size(); i++)
    {
        RayHit nhit;
        if(_meshes[i].RayCast(ray, nhit))
        {
            if(nhit.Distance < hit.Distance)
            {
                hit = nhit;
                hasHit = true;
            }
        }
    }
    return hasHit;
}

void Renderer::Render() 
{
    std::async(&Renderer::RenderInternal, this, _buffer, _width, _height);
}

void Renderer::RenderInternal(unsigned char* buffer, int width, int height)
{
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    _onRender = true;
    size_t index = 0;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            auto ray = _camera->GetRay(j, i);
            RayHit hit;
            if(RayCast(ray, hit))
            {
                glm::vec3 color(0);
                for (size_t k = 0; k < _lights.size(); k++)
                {
                    glm::vec3 lightDir = normalize(_lights[k].GetPos() - hit.Point);
                    glm::vec3 luminance = _lights[k].GetLuminance(hit.Point);
                    // DIFFUSE
                    float teta = dot(lightDir, hit.Normal);
                    teta = teta < 0 ? 0 : teta;
                    color += luminance * teta * 255.0f;                                                
                }
                AssignRGBFromVec3(buffer + index, color);
            }
            else
            {
                AssignRGBA(buffer + index, 0, 0, 0, 255);
            }
            index += 4;

            if(_refresh)
            {
                _refresh = false;
                i = 0;
                j = 0;
                index = 0;
                std::cout << "Refreshin Render" << std::endl;
            }
        }        
    }
    _onRender = false;       
    std::cout << "Render Complete" << std::endl;
}
