#pragma once

#include "glm/glm.hpp"
#include <vector>

class Ray;

struct Triangle
{
    unsigned int Inds[3];
};

struct Vertex
{
    glm::vec3 Pos;
};

class Mesh
{
    public:
        Mesh();
        bool RayCast(const Ray& ray);
        void AddVertex(Vertex vert);
        void AddTriangle(Triangle tri);
    private:
        bool RayCastTriangle(const Ray& ray, const Triangle& tri);
        std::vector<Vertex> _verts;
        std::vector<Triangle> _tris;
};