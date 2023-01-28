#pragma once

#include "Ray.h"
#include "glm/glm.hpp"
#include "pch.h"

struct AABB
{
    glm::vec3 extends[2];
    glm::vec3 center;

    AABB()
    {
        extends[0] = glm::vec3(std::numeric_limits<float>::max());
        extends[1] = glm::vec3(std::numeric_limits<float>::lowest());
    }

    AABB(const AABB& other)
    {
        extends[0] = other.extends[0];
        extends[1] = other.extends[1];
        center = other.center;
    }

    AABB(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
    {
        for (int i = 0; i < 3; i++)
        {
            extends[0][i] = std::min(p1[i], std::min(p2[i], p3[i]));
        }
        for (int i = 0; i < 3; i++)
        {
            extends[1][i] = std::max(p1[i], std::max(p2[i], p3[i]));
        }        
        center = (extends[0] + extends[1]) / 2.0f;
    }

    float Area()
    {
        auto e = extends[1] - extends[0];
        return e.x * e.y + e.y * e.z + e.z * e.x;
    }

    void Extend(const AABB& other)
    {
        for (int i = 0; i < 3; i++)
        {
            extends[0][i] = std::min(extends[0][i], other.extends[0][i]);
        }
        for (int i = 0; i < 3; i++)
        {
            extends[1][i] = std::max(extends[1][i], other.extends[1][i]);
        }        
        center = (extends[0] + extends[1]) / 2.0f;
    }

    bool Test(const Ray& ray)
    {
        float imin = -FLT_MAX;
        float imax = FLT_MAX;
        float t0 = (extends[ray.sign[0]].x - ray.orig.x) * ray.invDir.x;
        float t1 = (extends[1 - ray.sign[0]].x - ray.orig.x) * ray.invDir.x;
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        t0 = (extends[ray.sign[1]].y - ray.orig.y) * ray.invDir.y;
        t1 = (extends[1 - ray.sign[1]].y - ray.orig.y) * ray.invDir.y;
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        t0 = (extends[ray.sign[2]].z - ray.orig.z) * ray.invDir.z;
        t1 = (extends[1 - ray.sign[2]].z - ray.orig.z) * ray.invDir.z;
        if(t0 > t1) std::swap(t0, t1);
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        return true;
    }

};
