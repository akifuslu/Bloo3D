#pragma once

#include "glm/glm.hpp"

struct RayHit
{
    glm::vec3 Point;
    glm::vec3 Normal;
    float Distance;

    RayHit()
    {
        Distance = FLT_MAX;
    }
};

struct Ray
{
    glm::vec3 Orig;
    glm::vec3 Dir;

    Ray()
    {

    }

    Ray(glm::vec3 orig, glm::vec3 dir)
    {
        Orig = orig;
        Dir = dir;
    }
};
