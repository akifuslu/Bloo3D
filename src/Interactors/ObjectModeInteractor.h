#pragma once

class Scene;

class ObjectModeInteractor
{
    public:
        ObjectModeInteractor(Scene* scene);
        void OnUpdate();
    private:
        Scene* _scene;
};
