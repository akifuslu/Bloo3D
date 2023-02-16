#include "ObjectModeInteractor.h"
#include "Scene/Scene.h"
#include "Input.h"

ObjectModeInteractor::ObjectModeInteractor(Scene* scene) 
    : _scene(scene)
{
    
}

void ObjectModeInteractor::OnUpdate() 
{
    if(Input::MouseOnUI() || Input::KeyboardOnUI())
        return;

    if(Input::GetMouseButtonDown(MouseButton::RIGHT))
    {
        bool additive = Input::GetKey(KeyCode::SHIFT);
        auto pos = Input::GetMousePosition();
        auto ray = _scene->editorCamera->ScreenPointToRay(pos);
        RayHit hit;
        if(_scene->Raycast(ray, &hit))
        {
            _scene->SelectObject(hit.object, additive);
        }
        else
        {
            _scene->DeselectAll();
        }
    }   
}
