#include "Mesh.h"
#include "Ray.h"
#include "AABB.h"
#include "Scene/Object.h"
#include "GPU/Buffer.h"
#include "GPU/VertexBufferLayout.h"
#include "Logger.h"
#include "pch.h"

Triangle::Triangle(Vertex* v0, Vertex* v1, Vertex* v2)
    : v0(v0), v1(v1), v2(v2)
{
    v0v1 = v1->pos - v0->pos;
    v0v2 = v2->pos - v0->pos;

    v0v1N = v1->normal - v0->normal;
    v0v2N = v2->normal - v0->normal;
    aabb = AABB(v0->pos, v1->pos, v2->pos);
    smooth = true; // by default
}

Mesh::Mesh()
    : Object()
{
    Init();
}

void Mesh::Init()
{
    type = ObjectType::MESH;
    materialIndex = -1;
    // create render info with empty buffers
    renderInfo.vb.reset(VertexBuffer::Create(nullptr, 0));
    renderInfo.ib.reset(IndexBuffer::Create(nullptr, 0));
    // we can create layout right away since its not likely to change
    renderInfo.layout.reset(new VertexBufferLayout());
    renderInfo.layout->Push<float>(3); // vertex positions
    renderInfo.layout->Push<float>(3); // vertex normals
}

void Mesh::BuildBVH()
{
    std::vector<IRayCastable*> rtris; 
    for (size_t i = 0; i < _tris.size(); i++)
    {
        rtris.push_back(&_tris[i]);
    }    
    _bvh = new BVHNode(rtris);
    aabb = AABB(_bvh->aabb);
}

bool Triangle::RayCast(const Ray& ray, RayHit* hit)
{
    auto pvec = cross(ray.dir, v0v2);
    float det = dot(v0v1, pvec);
    if (std::fabs(det) < 1e-6)
    {            
        return false;
    }

    float invDet = 1.0 / det;
    auto tvec = ray.orig - v0->pos;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
        return false;

    auto qvec = cross(tvec, v0v1);
    float v = dot(ray.dir, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;
    float t = dot(v0v2, qvec) * invDet;
    if(t < 0)
    {
        return false;
    }
    hit->distance = t;
    hit->point = v0->pos + u * v0v1 + v * v0v2;
    if(smooth)
    {
        hit->normal = normalize(v0->normal + u * v0v1N + v * v0v2N);
    }
    else
    {
        hit->normal = normal;
    }
    return true;
}

bool Mesh::RayCast(const Ray& ray, RayHit* hit) 
{
    Ray lray = Ray(transform.WorldToLocal() * glm::vec4(ray.orig, 1.0f), normalize(transform.WorldToLocal() * glm::vec4(ray.dir, 0.0f)));
    bool f = _bvh->RayCast(lray, hit);
    if(f)
    {
        hit->matIndex = materialIndex;
        // convert surface properties to world space
        hit->point = transform.LocalToWorld() * glm::vec4(hit->point, 1.0f);   
        // converting normals actually transpose(inverse(localToWorld))
        // inverse of localToWorld is just worldToLocal
        hit->normal = normalize(transpose(transform.WorldToLocal()) * glm::vec4(hit->normal, 0.0f)); 
        hit->object = this;
    }
    return f;
}

void Mesh::AddVertex(glm::vec3 pos) 
{
    _verts.push_back(Vertex(pos));   
    renderInfo.needUpdate = true;
}

void Mesh::AddVertex(glm::vec3 pos, glm::vec3 norm) 
{
    _verts.push_back(Vertex(pos, norm));   
    renderInfo.needUpdate = true;
}

void Mesh::AddTriangle(int i, int j, int k) 
{
    Triangle tri(&_verts[i], &_verts[j], &_verts[k]);
    _tris.push_back(tri);    
    _inds.push_back(i);
    _inds.push_back(j);
    _inds.push_back(k);
    renderInfo.needUpdate = true;
}

void Mesh::UpdateRenderInfo()
{
    if(!renderInfo.needUpdate) // no need to update
        return;
    renderInfo.needUpdate = false;
    // for now we are just doing full upload
    // later we should do partial updates whenever possible
    renderInfo.vb->Update(_verts.data(), _verts.size() * sizeof(Vertex)); // we assume that no aliasing occurs in Vertex struct. TODO: provide a solution for possible aliasing
    renderInfo.ib->Update(_inds.data(), _inds.size());
}

void Mesh::RecalculateNormals()
{
    for(auto& vert: _verts)
    {
        vert.normal = glm::vec3(0);
    }
    for(auto& tri: _tris)
    {
        auto n = normalize(cross(tri.v0v1, tri.v0v2));
        tri.v0->normal += n;
        tri.v1->normal += n;
        tri.v2->normal += n;
    }
    for(auto& vert: _verts)
    {
        vert.normal = normalize(vert.normal);
    }
}
