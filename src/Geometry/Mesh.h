#pragma once

#include "IRayCastable.h"
#include "glm/glm.hpp"
#include "BVH.h"

#include <vector>

class Ray;
class RayHit;
class Object;

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    bool HasNormal;

    Vertex(const glm::vec3& pos) : Pos(pos)
    {
        HasNormal = false;
    }

    Vertex(const glm::vec3& pos, const glm::vec3& norm) : Pos(pos), Normal(norm) 
    {
        HasNormal = true;
    }
};

class Triangle : public IRayCastable
{
    public:
        Triangle(Vertex* v0, Vertex* v1, Vertex* v2);
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        glm::vec3 Normal;        
        bool Smooth;
    private:
        Vertex* _v0;
        Vertex* _v1;
        Vertex* _v2;
        glm::vec3 _v0v1;
        glm::vec3 _v0v2; 
        glm::vec3 _v0v1N;
        glm::vec3 _v0v2N; 
};

class Mesh : public IRayCastable
{
    public:
        Mesh();
        ~Mesh();
        void BuildBVH();
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        void AddVertex(glm::vec3 pos);
        void AddVertex(glm::vec3 pos, glm::vec3 norm);
        void AddTriangle(int i, int j, int k);
        int MaterialIndex;
        void SetParent(Object* parent);
        Object* GetParent() const;
    private:
        std::vector<Vertex*> _verts;
        std::vector<Triangle*> _tris;
        BVHNode* _bvh;
        Object* _parent;
};