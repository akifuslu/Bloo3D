#pragma once

#include "IRayCastable.h"
#include <memory>
#include <vector>
#include <span>
#include <algorithm>
#include <iostream>

class BVHNode : public IRayCastable 
{
    public:
        BVHNode(std::span<IRayCastable*> entities)
        {
            if(entities.size() == 1)
            {
                Left = entities[0];
                Right = entities[0];
            }
            else if(entities.size() == 2)
            {
                Left = entities[0];
                Right = entities[1];
            }
            else
            {
                float c;
                int axis = GetMaxAxis(entities, &c);
                //std::cout << axis << std::endl;
                int mid = Split(entities, c, axis);
                int rem = entities.size() - mid;
                Left  = new BVHNode(entities.subspan(0, mid));
                Right = new BVHNode(entities.subspan(mid, rem));
            }
            Bounds = AABB();
            Bounds.Extend(Left->Bounds);
            Bounds.Extend(Right->Bounds);
        }

        int Split(std::span<IRayCastable*> entities, float c, int axis)
        {
            int mid = 0;
            for(int i = 0; i < entities.size(); i++)
            {
                if(entities[i]->Bounds.Center[axis] < c)
                {
                    auto tmp = entities[i];
                    entities[i] = entities[mid];
                    entities[mid] = tmp;
                    mid++;
                }
            }
            if(mid == 0 || mid == entities.size())
            { 
                //std::cout << "SEX" << std::endl;
                mid = entities.size() / 2;
            }
            return mid;
        }

        int GetMaxAxis(std::span<IRayCastable*> entities, float* c)
        {
            float mins[3] {FLT_MAX, FLT_MAX, FLT_MAX};
            float maxs[3] {FLT_MIN, FLT_MIN, FLT_MIN};
            for (size_t i = 0; i < entities.size(); i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    mins[j] = std::min(mins[j], entities[i]->Bounds.Extends[0][j]);   
                    maxs[j] = std::max(maxs[j], entities[i]->Bounds.Extends[1][j]); 
                }                
            }            
            float d0 = maxs[0] - mins[0];
            float d1 = maxs[1] - mins[1];
            float d2 = maxs[2] - mins[2];

            int axis = 0;
            if(d1 > d0 && d1 > d2) axis = 1;
            if(d2 > d0 && d2 > d1) axis = 2;

            *c = (maxs[axis] + mins[axis]) / 2;
            return axis;
        }

        virtual bool RayCast(const Ray& ray, RayHit* hit) override
        {
            if(!Bounds.Test(ray))
            {
                return false;
            }
            RayHit lhit;
            RayHit rhit;
            bool l = Left->RayCast(ray, &lhit);
            bool r = Right->RayCast(ray, &rhit);

            *hit = lhit.Distance < rhit.Distance ? lhit : rhit; 
            return l || r;
        }
        IRayCastable* Left;
        IRayCastable* Right;        
};

