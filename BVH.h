#pragma once

#include "IRayCastable.h"
#include <memory>
#include <vector>
#include <span>
#include <algorithm>


template<typename T>
class BVHNode : public IRayCastable 
{
    public:
        BVHNode(std::span<T> entities)
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
                int mid = Split(entities, c, axis);
                int rem = entities.size() - mid;
                Left  = std::make_shared<BVHNode<T>>(entities.subspan(0, mid));
                Right = std::make_shared<BVHNode<T>>(entities.subspan(mid, rem));
            }
            BoundingBox = std::make_unique<AABB>();
            BoundingBox->Extend(*Left->BoundingBox.get());
            BoundingBox->Extend(*Right->BoundingBox.get());
        }

        int Split(std::span<T> entities, float c, int axis)
        {
            int mid = 0;
            for(int i = 0; i < entities.size(); i++)
            {
                if(entities[i]->BoundingBox->Center[axis] < c)
                {
                    auto tmp = entities[i];
                    entities[i] = entities[mid];
                    entities[mid] = tmp;
                    mid++;
                }
            }
            if(mid == 0 || mid == entities.size()) mid = entities.size() / 2;
            return mid;
        }

        int GetMaxAxis(std::span<T> entities, float* c)
        {
            float mins[3] {FLT_MAX, FLT_MAX, FLT_MAX};
            float maxs[3] {FLT_MIN, FLT_MIN, FLT_MIN};
            for (size_t i = 0; i < entities.size(); i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    mins[j] = std::min(mins[j], entities[i]->BoundingBox->Bounds[0][j]);   
                    maxs[j] = std::max(maxs[j], entities[i]->BoundingBox->Bounds[1][j]); 
                }                
            }            
            float d0 = maxs[0] - mins[0];
            float d1 = maxs[1] - mins[1];
            float d2 = maxs[2] - mins[2];

            int axis = d0 > d1 ? 0 : 1;
            axis = d2 > axis ? 2 : axis;
            *c = (maxs[axis] + mins[axis]) / 2;
            return axis;
        }

        virtual bool RayCast(const Ray& ray, RayHit* hit) override
        {
            if(!BoundingBox->Test(ray))
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
        std::shared_ptr<IRayCastable> Left;
        std::shared_ptr<IRayCastable> Right;        
};

