#pragma once

#include "Ray.h"
#include "glm/glm.hpp"
#include <algorithm>

struct AABB
{
    glm::vec3 Extends[2];
    glm::vec3 Center;

    AABB()
    {
        Extends[0] = glm::vec3(FLT_MAX);
        Extends[1] = glm::vec3(FLT_MIN);
    }

    AABB(const AABB& other)
    {
        Extends[0] = other.Extends[0];
        Extends[1] = other.Extends[1];
        Center = other.Center;
    }

    AABB(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
    {
        for (int i = 0; i < 3; i++)
        {
            Extends[0][i] = std::min(p1[i], std::min(p2[i], p3[i]));
        }
        for (int i = 0; i < 3; i++)
        {
            Extends[1][i] = std::max(p1[i], std::max(p2[i], p3[i]));
        }        
        Center = (Extends[0] + Extends[1]) / 2.0f;
    }

    float Area()
    {
        auto e = Extends[1] - Extends[0];
        return e.x * e.y + e.y * e.z + e.z * e.x;
    }

    void Extend(const AABB& other)
    {
        for (int i = 0; i < 3; i++)
        {
            Extends[0][i] = std::min(Extends[0][i], other.Extends[0][i]);
        }
        for (int i = 0; i < 3; i++)
        {
            Extends[1][i] = std::max(Extends[1][i], other.Extends[1][i]);
        }        
        Center = (Extends[0] + Extends[1]) / 2.0f;
    }

    bool Test(const Ray& ray)
    {
        float imin = FLT_MIN;
        float imax = FLT_MAX;
        float t0 = (Extends[ray.Sign[0]].x - ray.Orig.x) * ray.InvDir.x;
        float t1 = (Extends[1 - ray.Sign[0]].x - ray.Orig.x) * ray.InvDir.x;
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        t0 = (Extends[ray.Sign[1]].y - ray.Orig.y) * ray.InvDir.y;
        t1 = (Extends[1 - ray.Sign[1]].y - ray.Orig.y) * ray.InvDir.y;
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        t0 = (Extends[ray.Sign[2]].z - ray.Orig.z) * ray.InvDir.z;
        t1 = (Extends[1 - ray.Sign[2]].z - ray.Orig.z) * ray.InvDir.z;
        if(t0 > t1) std::swap(t0, t1);
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        return true;
    }

};
