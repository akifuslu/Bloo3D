#pragma once

#include <memory>

class Ray;
class RayHit;
class AABB;

class IRayCastable
{
    public:
        virtual bool RayCast(const Ray& ray, RayHit* hit)
        {
            return false;
        }
        std::unique_ptr<AABB> BoundingBox;
};
