#include "Mesh.h"
#include "Ray.h"
#include "AABB.h"

Triangle::Triangle(const std::shared_ptr<Vertex>& v0, const std::shared_ptr<Vertex>& v1, const std::shared_ptr<Vertex>& v2)
    : _v0(v0), _v1(v1), _v2(v2)
{
    _v0v1 = v1->Pos - v0->Pos;
    _v0v2 = v2->Pos - v0->Pos;

    Normal = normalize(cross(_v0v1, _v0v2));
    BoundingBox = std::make_unique<AABB>(_v0->Pos, _v1->Pos, _v2->Pos);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::BuildBVH()
{
    BoundingBox = std::make_unique<AABB>();
    for (size_t i = 0; i < _tris.size(); i++)
    {
        BoundingBox->Extend(*_tris[i]->BoundingBox);
    }    
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
    if(!BoundingBox->Test(ray))
    {        
        return false;
    }

    bool hasHit = false;
    for (size_t i = 0; i < _tris.size(); i++)
    {
        RayHit nhit;
        if(_tris[i]->RayCast(ray, &nhit))
        {
            if(nhit.Distance < hit->Distance)
            {
                *hit = nhit;
                hasHit = true;
            }
        }
    }
    return hasHit;
}

void Mesh::AddVertex(glm::vec3 pos) 
{
    std::shared_ptr<Vertex> vert = std::make_shared<Vertex>(pos);
    _verts.push_back(vert);   
}

void Mesh::AddTriangle(int i, int j, int k) 
{
    std::unique_ptr<Triangle> tri = std::make_unique<Triangle>(_verts[i], _verts[j], _verts[k]);
    _tris.push_back(std::move(tri));    
}
