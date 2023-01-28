#pragma once

#include "Scene/Transform.h"
#include "IRayCastable.h"
#include "glm/glm.hpp"
#include "BVH.h"

#include "pch.h"

struct Ray;
struct RayHit;

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    bool hasNormal;

    Vertex(const glm::vec3& pos) : pos(pos)
    {
        hasNormal = false;
    }

    Vertex(const glm::vec3& pos, const glm::vec3& norm) : pos(pos), normal(norm) 
    {
        hasNormal = true;
    }
};

class Triangle : public IRayCastable
{
    public:
        Triangle(Vertex* v0, Vertex* v1, Vertex* v2);
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        glm::vec3 normal;        
        bool smooth;
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
        virtual ~Mesh() override;
        void BuildBVH();
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        void AddVertex(glm::vec3 pos);
        void AddVertex(glm::vec3 pos, glm::vec3 norm);
        void AddTriangle(int i, int j, int k);
        int materialIndex;
        Transform transform;
    private:
        std::vector<Vertex*> _verts;
        std::vector<Triangle*> _tris;
        BVHNode* _bvh;
};