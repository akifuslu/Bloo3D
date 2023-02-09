#pragma once

#include "Scene/Transform.h"
#include "IRayCastable.h"
#include "glm/glm.hpp"
#include "BVH.h"
#include "RenderInfo.h"
#include "Scene/Object.h"

#include "pch.h"

struct Ray;
struct RayHit;

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;

    Vertex(const glm::vec3& pos) : pos(pos), normal(glm::vec3(1, 0, 0)) {} // normals are invalid in this state, normal recalculation must be run in such cases 
    Vertex(const glm::vec3& pos, const glm::vec3& norm) : pos(pos), normal(norm) {}
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

class Mesh : public Object, public IRayCastable
{
    public:
        Mesh();
        virtual ~Mesh() override = default;
        void BuildBVH();
        virtual bool RayCast(const Ray& ray, RayHit* hit) override;
        void AddVertex(glm::vec3 pos);
        void AddVertex(glm::vec3 pos, glm::vec3 norm);
        void AddTriangle(int i, int j, int k);
        int materialIndex;
        RenderInfo renderInfo;
        void UpdateRenderInfo();
    private:
        void Init();
        std::vector<Vertex> _verts;
        std::vector<Triangle> _tris;
        std::vector<unsigned int> _inds; // must be in sync with the Triangle vector, TODO: find a better way to handle GL index buffers
        BVHNode* _bvh;
};