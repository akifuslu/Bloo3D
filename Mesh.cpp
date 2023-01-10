#include "Mesh.h"
#include "Ray.h"

Mesh::Mesh() 
{
    
}

bool Mesh::RayCastTriangle(const Ray& ray, const Triangle& tri)
{
    auto v0 = _verts[tri.Inds[0]];
    auto v1 = _verts[tri.Inds[1]];
    auto v2 = _verts[tri.Inds[2]];

    auto v0v1 = v1.Pos - v0.Pos;
    auto v0v2 = v2.Pos - v0.Pos;

    auto pvec = cross(ray.Dir, v0v2);
    float det = dot(v0v1, pvec);
    if (std::fabs(det) < 1e-9)
    {            
        return false;
    }

    float invDet = 1.0 / det;
    auto tvec = ray.Orig - v0.Pos;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
        return false;

    auto qvec = cross(tvec, v0v1);
    float v = dot(ray.Dir, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;
    float t = dot(v0v2, qvec) * invDet;
    if(t < 0)
    {
        return false;
    }
    // hit.T = t;
    // hit.Point = (*V0) + u * ((*V1) - (*V0)) + v * ((*V2) - (*V0));
    return true;
}

bool Mesh::RayCast(const Ray& ray) 
{
    for (size_t i = 0; i < _tris.size(); i++)
    {
        if(RayCastTriangle(ray, _tris[i]))
        {
            return true;
        }
    }
    return false;
}

void Mesh::AddVertex(Vertex vert) 
{
    _verts.push_back(vert);   
}

void Mesh::AddTriangle(Triangle tri) 
{
    _tris.push_back(tri);    
}
