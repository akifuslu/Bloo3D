#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Ray.h"

#include<iostream>


Renderer::Renderer(int width, int height)
{
    _camera = new Camera(
        glm::vec3(0, 0, -5),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 0),
        60,
        glm::ivec2(width, height)
    );
}

Renderer::~Renderer()
{
    delete _camera;
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

void Renderer::Render(unsigned char* buffer, int width, int height, int frame) 
{
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
        }        
    }
       
}
