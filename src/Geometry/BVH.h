#pragma once

#include "IRayCastable.h"
#include "pch.h"

#define LOGVEC3(x) std::cout << x[0] << " , " << x[1] << " , " << x[2] << std::endl;

class BVHNode : public IRayCastable 
{
    public:
        BVHNode(std::span<IRayCastable*> entities)
        {
            aabb = AABB(entities[0]->aabb);
            for(int i = 0; i < entities.size(); i++)
            {
                aabb.Extend(entities[i]->aabb);
            }
            
            if(entities.size() == 1)
            {
                left = entities[0];
                right = entities[0];
            }
            else if(entities.size() == 2)
            {
                left = entities[0];
                right = entities[1];
            }
            else
            {
                float bestPos, bestCost = FLT_MAX;
                int bestAxis = -1;                
                int planeCount = 4;

                for (int axis = 0; axis < 3; axis++)
                {
                    float scale = (aabb.extends[1][axis] - aabb.extends[0][axis]) / planeCount;
                    for (int i = 0; i < planeCount; i++)
                    {
                        float pos = aabb.extends[0][axis] + i * scale;
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
                    if(aabb.extends[1][1] - aabb.extends[0][1] > aabb.extends[1][0] - aabb.extends[0][0]) bestAxis = 1;
                    if(aabb.extends[1][2] - aabb.extends[0][2] > aabb.extends[1][bestAxis] - aabb.extends[0][bestAxis]) bestAxis = 2;

                    bestPos = aabb.center[bestAxis];
                }

                int mid = Split(entities, bestPos, bestAxis);
                int rem = entities.size() - mid;
                left  = new BVHNode(entities.subspan(0, mid));
                right = new BVHNode(entities.subspan(mid, rem));
            }
        }

        float EvaluateSAH(std::span<IRayCastable*> entities, float pos, int axis)
        {
            AABB left, right;
            int lc = 0, rc = 0;
            for (size_t i = 0; i < entities.size(); i++)
            {
                if(entities[i]->aabb.center[axis] < pos)
                {
                    left.Extend(entities[i]->aabb);
                    lc++;
                }
                else
                {
                    right.Extend(entities[i]->aabb);
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
                if (entities[i]->aabb.center[axis] < c)
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
            if(!aabb.Test(ray))
            {
                return false;
            }
            RayHit lhit;
            RayHit rhit;
            bool l = left->RayCast(ray, &lhit);
            bool r = right->RayCast(ray, &rhit);
            *hit = lhit.distance < rhit.distance ? lhit : rhit; 
            return l || r;
        }
        IRayCastable* left;
        IRayCastable* right;        
};

