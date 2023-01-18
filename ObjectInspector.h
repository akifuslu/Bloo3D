#pragma once

class Object;

class ObjectInspector
{
    public:
        bool OnGUI();
        void Bind(Object* obj);
        void UnBind();
    private:
        Object* _obj;
        bool _open;
};
