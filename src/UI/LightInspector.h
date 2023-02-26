#pragma once

class Light;

class LightInspector
{
    public:
        bool OnGUI();
        void Bind(Light* obj);
        void UnBind();
    private:
        Light* _light = nullptr;
};
