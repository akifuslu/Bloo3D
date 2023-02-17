#pragma once

#include "glm/glm.hpp"
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
        Scene* _scene;
        Mode _mode;
        Axis _axis;
        glm::vec3 _lastpos;
};
