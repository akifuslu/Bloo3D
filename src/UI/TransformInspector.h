#pragma once

#include "Scene/Transform.h"

class TransformInspector
{
    public:
        bool OnGUI();
        void Bind(Transform* transform);
        void UnBind();
    private:
        Transform* _transform;
        bool _open;
};
