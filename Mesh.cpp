#include "Mesh.h"
#include "Ray.h"

Mesh::Mesh() 
{
    
}

bool Mesh::RayCastTriangle(const Ray& ray, const Triangle& tri, RayHit& hit)
{
    auto pvec = cross(ray.Dir, tri.v0v2);
    float det = dot(tri.v0v1, pvec);
    if (std::fabs(det) < 1e-6)
    {            
        return false;
    }

    float invDet = 1.0 / det;
    auto tvec = ray.Orig - tri.v0.Pos;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
        return false;

    auto qvec = cross(tvec, tri.v0v1);
    float v = dot(ray.Dir, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;
    float t = dot(tri.v0v2, qvec) * invDet;
    if(t < 0)
    {
        return false;
    }
    hit.Distance = t;
    hit.Point = tri.v0.Pos + u * tri.v0v1 + v * tri.v0v2;
    hit.Normal = tri.Normal;
    return true;
}

bool Mesh::RayCast(const Ray& ray, RayHit& hit) 
{
    bool hasHit = false;
    for (size_t i = 0; i < _tris.size(); i++)
    {
        RayHit nhit;
        if(RayCastTriangle(ray, _tris[i], nhit))
        {
            if(nhit.Distance < hit.Distance)
            {
                hit = nhit;
                hasHit = true;
            }
        }
    }
    return hasHit;
}

void Mesh::AddVertex(Vertex vert) 
{
    _verts.push_back(vert);   
}

void Mesh::AddTriangle(Triangle tri) 
{
    tri.CacheValues(_verts);
    _tris.push_back(tri);    
}
