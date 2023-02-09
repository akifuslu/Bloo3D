#pragma once

#include "Scene/Object.h"
#include "ObjectInspector.h"
#include "TransformInspector.h"

class InspectorView
{
    public:
        bool OnGUI();
        void Bind(Object* obj);
        void UnBind();
    private:
        Object* _obj;
        bool _open;
        ObjectInspector _objInsp;
        TransformInspector _transformInsp;

};
