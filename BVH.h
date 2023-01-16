#pragma once

#include "IRayCastable.h"
#include <memory>
#include <vector>
#include <span>
#include <algorithm>
#include <iostream>
#include <future>

#define LOGVEC3(x) std::cout << x[0] << " , " << x[1] << " , " << x[2] << std::endl;

class BVHNode : public IRayCastable 
{
    public:
        BVHNode(std::span<IRayCastable*> entities)
        {
            Bounds = AABB(entities[0]->Bounds);
            for(int i = 0; i < entities.size(); i++)
            {
                Bounds.Extend(entities[i]->Bounds);
            }
            
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
                int axis = 0; //GetMaxAxis(entities, &c);

                if(Bounds.Extends[1][1] - Bounds.Extends[0][1] > Bounds.Extends[1][0] - Bounds.Extends[0][0]) axis = 1;
                if(Bounds.Extends[1][2] - Bounds.Extends[0][2] > Bounds.Extends[1][axis] - Bounds.Extends[0][axis]) axis = 2;

                c = Bounds.Center[axis];

                int mid = Split(entities, c, axis);
                int rem = entities.size() - mid;
                Left  = new BVHNode(entities.subspan(0, mid));
                Right = new BVHNode(entities.subspan(mid, rem));
            }
        }

        int Split(std::span<IRayCastable*> entities, float c, int axis)
        {
            // int mid = 0;
            // for(int i = 0; i < entities.size(); i++)
            // {
            //     if(entities[i]->Bounds.Center[axis] < c)
            //     {
            //         auto tmp = entities[i];
            //         entities[i] = entities[mid];
            //         entities[mid] = tmp;
            //         mid++;
            //     }
            // }
            int i = 0;
            int j = entities.size() - 1;
            while (i <= j)
            {
                if (entities[i]->Bounds.Center[axis] < c)
                    i++;
                else
                    std::swap( entities[i], entities[j--] );
            }
            int mid = i;
            if(mid == 0 || mid == entities.size())
            { 
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

