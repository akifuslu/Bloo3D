#include "Mesh.h"
#include "Ray.h"
#include "AABB.h"
#include "Scene/Object.h"
#include <span>

Triangle::Triangle(Vertex* v0, Vertex* v1, Vertex* v2)
    : _v0(v0), _v1(v1), _v2(v2)
{
    _v0v1 = v1->Pos - v0->Pos;
    _v0v2 = v2->Pos - v0->Pos;

    if(v0->HasNormal && v1->HasNormal && v2->HasNormal)
    {
        Smooth = true;
        _v0v1N = v1->Normal - v0->Normal;
        _v0v2N = v2->Normal - v0->Normal;
    }
    else
    {
        Smooth = false;
        Normal = normalize(cross(_v0v1, _v0v2));
    }
    Bounds = AABB(_v0->Pos, _v1->Pos, _v2->Pos);
}

Mesh::Mesh()
{
    _parent = nullptr;
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
    if(Smooth)
    {
        hit->Normal = normalize(_v0->Normal + u * _v0v1N + v * _v0v2N);
    }
    else
    {
        hit->Normal = Normal;
    }
    return true;
}

bool Mesh::RayCast(const Ray& ray, RayHit* hit) 
{
    Ray lray = ray;
    if(_parent != nullptr)
    {
        lray = Ray(_parent->WorldToLocal() * glm::vec4(ray.Orig, 1.0f), normalize(_parent->WorldToLocal() * glm::vec4(ray.Dir, 0.0f)));
    }
    bool f = _bvh->RayCast(lray, hit);
    if(f)
    {
        hit->MatIndex = MaterialIndex;
        if(_parent != nullptr)
        {
            // convert surface properties to world space
            hit->Point = _parent->LocalToWorld() * glm::vec4(hit->Point, 1.0f);   
            // converting normals actually transpose(inverse(localToWorld))
            // inverse of localToWorld is just worldToLocal
            hit->Normal = normalize(transpose(_parent->WorldToLocal()) * glm::vec4(hit->Normal, 0.0f)); 
        }
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

void Mesh::SetParent(Object* parent)
{
    _parent = parent;
}

Object* Mesh::GetParent() const
{
    return _parent;
}
