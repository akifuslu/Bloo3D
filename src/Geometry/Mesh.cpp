#include "Mesh.h"
#include "Ray.h"
#include "AABB.h"
#include "Scene/Object.h"
#include "pch.h"

Triangle::Triangle(Vertex* v0, Vertex* v1, Vertex* v2)
    : _v0(v0), _v1(v1), _v2(v2)
{
    _v0v1 = v1->pos - v0->pos;
    _v0v2 = v2->pos - v0->pos;

    if(v0->hasNormal && v1->hasNormal && v2->hasNormal)
    {
        smooth = true;
        _v0v1N = v1->normal - v0->normal;
        _v0v2N = v2->normal - v0->normal;
    }
    else
    {
        smooth = false;
        normal = normalize(cross(_v0v1, _v0v2));
    }
    aabb = AABB(_v0->pos, _v1->pos, _v2->pos);
}

Mesh::Mesh()
{
    materialIndex = -1;
}

Mesh::~Mesh()
{
    for (size_t i = 0; i < _verts.size(); i++)
    {
        delete _verts[i];
    }
    for (size_t i = 0; i < _tris.size(); i++)
    {
        delete _tris[i];
    }    
}

void Mesh::BuildBVH()
{
    std::vector<IRayCastable*> rtris; 
    for (size_t i = 0; i < _tris.size(); i++)
    {
        rtris.push_back(_tris[i]);
    }    
    _bvh = new BVHNode(rtris);
    aabb = AABB(_bvh->aabb);
}

bool Triangle::RayCast(const Ray& ray, RayHit* hit)
{
    auto pvec = cross(ray.dir, _v0v2);
    float det = dot(_v0v1, pvec);
    if (std::fabs(det) < 1e-6)
    {            
        return false;
    }

    float invDet = 1.0 / det;
    auto tvec = ray.orig - _v0->pos;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
        return false;

    auto qvec = cross(tvec, _v0v1);
    float v = dot(ray.dir, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;
    float t = dot(_v0v2, qvec) * invDet;
    if(t < 0)
    {
        return false;
    }
    hit->distance = t;
    hit->point = _v0->pos + u * _v0v1 + v * _v0v2;
    if(smooth)
    {
        hit->normal = normalize(_v0->normal + u * _v0v1N + v * _v0v2N);
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
    }
    return f;
}

void Mesh::AddVertex(glm::vec3 pos) 
{
    Vertex* vert = new Vertex(pos);
    _verts.push_back(vert);   
}

void Mesh::AddVertex(glm::vec3 pos, glm::vec3 norm) 
{
    Vertex* vert = new Vertex(pos, norm);
    _verts.push_back(vert);   
}

void Mesh::AddTriangle(int i, int j, int k) 
{
    Triangle* tri = new Triangle(_verts[i], _verts[j], _verts[k]);
    _tris.push_back(tri);    
}
