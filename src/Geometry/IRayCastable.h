#pragma once

#include "AABB.h"
#include "pch.h"

struct Ray;
struct RayHit;

class IRayCastable
{
    public:
        virtual ~IRayCastable() = default;
        virtual bool RayCast(const Ray& ray, RayHit* hit)
        {
            return false;
        }
        AABB Bounds;
};
