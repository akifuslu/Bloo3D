#pragma once

#include "glm/glm.hpp"
#include "pch.h"
class Scene;

class ObjectModeInteractor
{
    enum class Mode
    {
        NONE,
        MOVE,
        ROTATE,
        SCALE
    };

    enum class Axis
    {
        NONE,
        X,
        Y,
        Z
    };

    public:
        ObjectModeInteractor(Scene* scene);
        void OnUpdate();
    private:
        glm::vec3 GetSelectionCenter();
        glm::vec3 PositionOnWorldPlane(glm::ivec2 pos, glm::vec3 center);
        void OnMove(glm::ivec2 pos, glm::vec3 center, glm::vec3 wpos);
        void OnRotate(glm::ivec2 pos, glm::vec3 center, glm::vec3 wpos);
        void OnScale(glm::ivec2 pos, glm::vec3 center, glm::vec3 wpos);
        void CacheTransforms();
        Scene* _scene;
        Mode _mode;
        Axis _axis;
        glm::vec3 _startpos;
        std::vector<glm::vec3> _locationCache;
        std::vector<glm::vec3> _rotationCache;
        std::vector<glm::vec3> _scaleCache;
};
