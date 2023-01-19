#pragma once

#include "AABB.h"
#include <memory>

class Ray;
class RayHit;

class IRayCastable
{
    public:
        virtual bool RayCast(const Ray& ray, RayHit* hit)
        {
            return false;
        }
        AABB Bounds;
};
