#pragma once

#include "Ray.h"
#include "glm/glm.hpp"
#include <algorithm>

struct AABB
{
    glm::vec3 Bounds[2];
    glm::vec3 Center;

    AABB()
    {
        Bounds[0] = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        Bounds[1] = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
        Center = glm::vec3(0);
    }

    AABB(const AABB& other)
    {
        Bounds[0] = other.Bounds[0];
        Bounds[1] = other.Bounds[1];
        Center = other.Center;
    }

    AABB(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
    {
        for (int i = 0; i < 3; i++)
        {
            Bounds[0][i] = std::min(p1[i], std::min(p2[i], p3[i]));
        }
        for (int i = 0; i < 3; i++)
        {
            Bounds[1][i] = std::max(p1[i], std::max(p2[i], p3[i]));
        }        
        Center = (Bounds[0] + Bounds[1]) / 2.0f;
    }

    void Extend(const AABB& other)
    {
        for (int i = 0; i < 3; i++)
        {
            Bounds[0][i] = std::min(Bounds[0][i], other.Bounds[0][i]);
        }
        for (int i = 0; i < 3; i++)
        {
            Bounds[1][i] = std::max(Bounds[1][i], other.Bounds[1][i]);
        }        
        Center = (Bounds[0] + Bounds[1]) / 2.0f;
    }

    bool Test(const Ray& ray)
    {
        float imin = FLT_MIN;
        float imax = FLT_MAX;
        float t0 = (Bounds[ray.Sign[0]].x - ray.Orig.x) * ray.InvDir.x;
        float t1 = (Bounds[1 - ray.Sign[0]].x - ray.Orig.x) * ray.InvDir.x;
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        t0 = (Bounds[ray.Sign[1]].y - ray.Orig.y) * ray.InvDir.y;
        t1 = (Bounds[1 - ray.Sign[1]].y - ray.Orig.y) * ray.InvDir.y;
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        t0 = (Bounds[ray.Sign[2]].z - ray.Orig.z) * ray.InvDir.z;
        t1 = (Bounds[1 - ray.Sign[2]].z - ray.Orig.z) * ray.InvDir.z;
        if(t0 > t1) std::swap(t0, t1);
        if(t0 > imin) imin = t0;
        if(t1 < imax) imax = t1;
        if(imin > imax) return false;

        return true;
    }

};
