#pragma once

#include "glm/glm.hpp"

class Object;

struct RayHit
{
    glm::vec3 point;
    glm::vec3 normal;
    float distance;
    int matIndex;
    Object* object;

    RayHit()
    {
        distance = FLT_MAX;
        matIndex = -1;
        object = nullptr;
    }
};

struct Ray
{
    glm::vec3 orig;
    glm::vec3 dir;
    glm::vec3 invDir;
    glm::ivec3 sign;

    Ray(glm::vec3 orig, glm::vec3 dir)
        :orig(orig), dir(dir)
    {
        invDir = 1.0f / dir;
        sign[0] = (dir.x > 0 ? 0 : 1);
        sign[1] = (dir.y > 0 ? 0 : 1);
        sign[2] = (dir.z > 0 ? 0 : 1);
    }
};
