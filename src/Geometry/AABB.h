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
        float tmin = 0.0f, tmax = INFINITY;

        for (int i = 0; i < 3; ++i) 
        {
            float t1 = (extends[0][i] - ray.orig[i]) * ray.invDir[i];
            float t2 = (extends[1][i] - ray.orig[i]) * ray.invDir[i];

            tmin = std::max(tmin, std::min(std::min(t1, t2), tmax));
            tmax = std::min(tmax, std::max(std::max(t1, t2), tmin));
        }

        return tmin < tmax;
    }

};
