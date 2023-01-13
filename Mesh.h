#pragma once

#include "IRayCastable.h"

#include "glm/glm.hpp"
#include <vector>
#include <memory>
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
        Triangle(const std::shared_ptr<Vertex>& v0, const std::shared_ptr<Vertex>& v1, const std::shared_ptr<Vertex>& v2);
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        glm::vec3 Normal;        
    private:
        std::shared_ptr<Vertex> _v0;
        std::shared_ptr<Vertex> _v1;
        std::shared_ptr<Vertex> _v2;
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
        std::vector<std::shared_ptr<Vertex>> _verts;
        std::vector<std::shared_ptr<Triangle>> _tris;
        std::shared_ptr<BVHNode<std::shared_ptr<Triangle>>> _bvh;
};