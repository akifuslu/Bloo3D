#pragma once

#include "IRayCastable.h"

#include "glm/glm.hpp"
#include <vector>
#include "BVH.h"

class Ray;
class RayHit;

struct Vertex
{
    glm::vec3 Pos;

    Vertex(const glm::vec3& pos) : Pos(pos)
    {

    }
};

class Triangle : public IRayCastable
{
    public:
        Triangle(Vertex* v0, Vertex* v1, Vertex* v2);
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        glm::vec3 Normal;        
    private:
        Vertex* _v0;
        Vertex* _v1;
        Vertex* _v2;
        glm::vec3 _v0v1;
        glm::vec3 _v0v2; 
};

class Mesh : public IRayCastable
{
    public:
        Mesh();
        ~Mesh();
        void BuildBVH();
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        void AddVertex(glm::vec3 pos);
        void AddTriangle(int i, int j, int k);
    private:
        std::vector<Vertex*> _verts;
        std::vector<Triangle*> _tris;
        BVHNode* _bvh;
};