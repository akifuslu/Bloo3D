#pragma once

#include "Scene/Object.h"

class ObjectInspector
{
    public:
        bool OnGUI();
        void Bind(Object* obj);
        void UnBind();
    private:
        Object* _obj = nullptr;
        bool _open = true;
};
