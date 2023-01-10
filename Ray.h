#pragma once

#include "glm/glm.hpp"

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
