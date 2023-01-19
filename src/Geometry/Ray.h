#pragma once

#include "glm/glm.hpp"

struct RayHit
{
    glm::vec3 Point;
    glm::vec3 Normal;
    float Distance;
    int MatIndex;

    RayHit()
    {
        Distance = FLT_MAX;
        MatIndex = -1;
    }
};

struct Ray
{
    glm::vec3 Orig;
    glm::vec3 Dir;
    glm::vec3 InvDir;
    int Sign[3];

    Ray()
    {

    }

    Ray(glm::vec3 orig, glm::vec3 dir)
    {
        Orig = orig;
        Dir = dir;
        InvDir = glm::vec3(1/Dir.x, 1/Dir.y, 1/Dir.z);
        Sign[0] = (Dir.x > 0 ? 0 : 1);
        Sign[1] = (Dir.y > 0 ? 0 : 1);
        Sign[2] = (Dir.z > 0 ? 0 : 1);
    }
};
