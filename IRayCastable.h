#pragma once

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
        AABB* Box;
};
