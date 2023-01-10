#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Ray.h"

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

void Renderer::AddMesh(const Mesh& mesh)
{
    _meshes.push_back(mesh);
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

bool Renderer::RayCast(const Ray& ray)
{
    for (size_t i = 0; i < _meshes.size(); i++)
    {
        if(_meshes[i].RayCast(ray))
        {
            return true;
        }
    }
    return false;
}

void Renderer::Render() 
{
    std::async(&Renderer::RenderInternal, this, _buffer, _width, _height);
}

void Renderer::RenderInternal(unsigned char* buffer, int width, int height)
{
    _onRender = true;
    size_t index = 0;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            auto ray = _camera->GetRay(j, i);
            if(RayCast(ray))
            {
                AssignRGBA(buffer + index, 200, 0, 0, 255);
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
            }
        }        
    }
    _onRender = false;       
}
