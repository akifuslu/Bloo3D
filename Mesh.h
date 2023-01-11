#pragma once

#include "glm/glm.hpp"
#include <vector>

class Ray;
class RayHit;

struct Vertex
{
    glm::vec3 Pos;
};

struct Triangle
{
    unsigned int Inds[3];

    glm::vec3 Normal;

    Vertex v0;
    Vertex v1;
    Vertex v2;

    glm::vec3 v0v1;
    glm::vec3 v0v2; 

    void CacheValues(std::vector<Vertex> verts)
    {
        v0 = verts[Inds[0]];
        v1 = verts[Inds[1]];
        v2 = verts[Inds[2]];

        v0v1 = v1.Pos - v0.Pos;
        v0v2 = v2.Pos - v0.Pos;

        Normal = normalize(cross(v0v1, v0v2));
    }
};

class Mesh
{
    public:
        Mesh();
        bool RayCast(const Ray& ray, RayHit& hit);
        void AddVertex(Vertex vert);
        void AddTriangle(Triangle tri);
    private:
        bool RayCastTriangle(const Ray& ray, const Triangle& tri, RayHit& hit);
        std::vector<Vertex> _verts;
        std::vector<Triangle> _tris;
};