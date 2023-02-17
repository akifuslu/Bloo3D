#include "ObjectModeInteractor.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Logger.h"

ObjectModeInteractor::ObjectModeInteractor(Scene* scene) 
    : _scene(scene)
{
    _mode = Mode::NONE;   
    _axis = Axis::NONE;
}

void ObjectModeInteractor::OnUpdate() 
{
    if(Input::MouseOnUI() || Input::KeyboardOnUI())
        return;

    if(_mode == Mode::NONE)
    {
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
        if(_scene->selectedObjects.size() > 0)
        {
            if(Input::GetKey(KeyCode::G))
            {
                _lastpos = glm::vec3(0);
                _mode = Mode::MOVE;
            }
            else if(Input::GetKey(KeyCode::R))
            {
                _mode = Mode::ROTATE;
            }
            else if(Input::GetKey(KeyCode::S))
            {
                _mode = Mode::SCALE;
            }
        }
    }
    else
    {
        if(Input::GetMouseButtonDown(MouseButton::LEFT))
        {
            _mode = Mode::NONE;
            _axis = Axis::NONE;
        }
        else if(Input::GetKeyDown(KeyCode::X))
        {
            _axis = (_axis == Axis::X) ? Axis::NONE : Axis::X;
        }
        else if(Input::GetKeyDown(KeyCode::Y))
        {
            _axis = (_axis == Axis::Y) ? Axis::NONE : Axis::Y;
        }
        else if(Input::GetKeyDown(KeyCode::Z))
        {
            _axis = (_axis == Axis::Z) ? Axis::NONE : Axis::Z;
        }

        auto pos = Input::GetMousePosition();
        auto cam = _scene->editorCamera;
        if(_mode == Mode::MOVE)
        {
            glm::vec3 center(0);
            for(auto& ob: _scene->selectedObjects)
            {
                center += ob->transform.GetLocation();
            }
            center /= _scene->selectedObjects.size();
            auto v = center - cam->transform.GetLocation();
            float dist = dot(cam->transform.Forward(), v);
            auto r = cam->ScreenPointToRay(pos);
            float x = dot(cam->transform.Forward(), r.dir);
            auto wpos = cam->transform.GetLocation() + r.dir * dist / x;
            auto delta = wpos - _lastpos;
            if(_axis == Axis::X)
            {
                delta.y = delta.z = 0;
            }
            if(_axis == Axis::Y)
            {
                delta.x = delta.z = 0;
            }
            if(_axis == Axis::Z)
            {
                delta.x = delta.y = 0;
            }

            if(length(_lastpos) > 0) // ugly workaround
            {
                for(auto& ob: _scene->selectedObjects)
                {
                    ob->transform.Translate(delta);
                }
            }
            _lastpos = wpos;
        }
    }
}
