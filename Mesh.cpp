#include "Mesh.h"
#include "Ray.h"
#include "AABB.h"
#include <span>

Triangle::Triangle(Vertex* v0, Vertex* v1, Vertex* v2)
    : _v0(v0), _v1(v1), _v2(v2)
{
    _v0v1 = v1->Pos - v0->Pos;
    _v0v2 = v2->Pos - v0->Pos;

    Normal = normalize(cross(_v0v1, _v0v2));
    Bounds = AABB(_v0->Pos, _v1->Pos, _v2->Pos);
}

Mesh::Mesh()
{
    MaterialIndex = -1;
}

Mesh::~Mesh()
{
}

void Mesh::BuildBVH()
{
    std::vector<IRayCastable*> rtris; 
    for (size_t i = 0; i < _tris.size(); i++)
    {
        rtris.push_back(_tris[i]);
    }    
    _bvh = new BVHNode(rtris);
    Bounds = AABB(_bvh->Bounds);
}

bool Triangle::RayCast(const Ray& ray, RayHit* hit)
{
    auto pvec = cross(ray.Dir, _v0v2);
    float det = dot(_v0v1, pvec);
    if (std::fabs(det) < 1e-6)
    {            
        return false;
    }

    float invDet = 1.0 / det;
    auto tvec = ray.Orig - _v0->Pos;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
        return false;

    auto qvec = cross(tvec, _v0v1);
    float v = dot(ray.Dir, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;
    float t = dot(_v0v2, qvec) * invDet;
    if(t < 0)
    {
        return false;
    }
    hit->Distance = t;
    hit->Point = _v0->Pos + u * _v0v1 + v * _v0v2;
    hit->Normal = Normal;
    return true;
}

bool Mesh::RayCast(const Ray& ray, RayHit* hit) 
{
    bool f = _bvh->RayCast(ray, hit);
    if(f)
    {
        hit->MatIndex = MaterialIndex;
    }
    return f;
}

void Mesh::AddVertex(glm::vec3 pos) 
{
    Vertex* vert = new Vertex(pos);
    _verts.push_back(vert);   
}

void Mesh::AddTriangle(int i, int j, int k) 
{
    Triangle* tri = new Triangle(_verts[i], _verts[j], _verts[k]);
    _tris.push_back(tri);    
}
