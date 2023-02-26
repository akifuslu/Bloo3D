#pragma once

#include "Scene/Object.h"
#include "ObjectInspector.h"
#include "TransformInspector.h"
#include "LightInspector.h"

class InspectorView
{
    public:
        bool OnGUI();
        void Bind(Object* obj);
        void UnBind();
    private:
        Object* _obj = nullptr;
        ObjectInspector _objInsp;
        TransformInspector _transformInsp;
        LightInspector _lightInsp;

};
