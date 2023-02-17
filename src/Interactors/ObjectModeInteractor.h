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
        Scene* _scene;
        Mode _mode;
        Axis _axis;
        glm::vec3 _lastpos;
};
