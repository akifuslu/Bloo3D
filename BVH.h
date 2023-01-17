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
                float bestPos, bestCost = FLT_MAX;
                int bestAxis = -1;                
                int planeCount = 4;

                for (int axis = 0; axis < 3; axis++)
                {
                    float scale = (Bounds.Extends[1][axis] - Bounds.Extends[0][axis]) / planeCount;
                    for (int i = 0; i < planeCount; i++)
                    {
                        float pos = Bounds.Extends[0][axis] + i * scale;
                        float cost = EvaluateSAH(entities, pos, axis);   
                        if(cost < bestCost)
                        {
                            bestCost = cost;
                            bestPos = pos;
                            bestAxis = axis;
                        }
                    }                    
                }
                
                if(bestCost == FLT_MAX)
                {
                    bestAxis = 0;
                    if(Bounds.Extends[1][1] - Bounds.Extends[0][1] > Bounds.Extends[1][0] - Bounds.Extends[0][0]) bestAxis = 1;
                    if(Bounds.Extends[1][2] - Bounds.Extends[0][2] > Bounds.Extends[1][bestAxis] - Bounds.Extends[0][bestAxis]) bestAxis = 2;

                    bestPos = Bounds.Center[bestAxis];
                }

                int mid = Split(entities, bestPos, bestAxis);
                int rem = entities.size() - mid;
                Left  = new BVHNode(entities.subspan(0, mid));
                Right = new BVHNode(entities.subspan(mid, rem));
            }
        }

        float EvaluateSAH(std::span<IRayCastable*> entities, float pos, int axis)
        {
            AABB left, right;
            int lc = 0, rc = 0;
            for (size_t i = 0; i < entities.size(); i++)
            {
                if(entities[i]->Bounds.Center[axis] < pos)
                {
                    left.Extend(entities[i]->Bounds);
                    lc++;
                }
                else
                {
                    right.Extend(entities[i]->Bounds);
                    rc++;
                }
            }
            float cost = lc * left.Area() + rc * right.Area();
            return cost > 0 ? cost : FLT_MAX;
        }

        int Split(std::span<IRayCastable*> entities, float c, int axis)
        {
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

