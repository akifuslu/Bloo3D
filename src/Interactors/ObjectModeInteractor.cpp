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
                _lastpos = glm::vec3(0);
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
        if(_mode == Mode::MOVE)
        {
            auto center = GetSelectionCenter();
            auto wpos = PositionOnWorldPlane(pos, center);
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
        else if(_mode == Mode::ROTATE)
        {

        }
        else if(_mode == Mode::SCALE)
        {
            auto center = GetSelectionCenter();
            auto wpos = PositionOnWorldPlane(pos, center);
            auto v1 = center - wpos;
            auto v2 = center - _lastpos;

            glm::vec3 scale(1);
            if(_axis == Axis::X)
            {
                scale.y = scale.z = 0;
                v1.y = v1.z = 0;
                v2.y = v2.z = 0;
            }
            if(_axis == Axis::Y)
            {
                scale.x = scale.z = 0;
                v1.x = v1.z = 0;
                v2.x = v2.z = 0;
            }
            if(_axis == Axis::Z)
            {
                scale.x = scale.y = 0;
                v1.x = v1.y = 0;
                v2.x = v2.y = 0;
            }
            auto d1 = length(v1);
            auto d2 = length(v2);
            scale *= (d1 - d2);
            if(length(_lastpos) > 0) // ugly workaround
            {
                for(auto& ob: _scene->selectedObjects)
                {
                    ob->transform.Scale(scale);
                }
            }
            _lastpos = wpos;
        }
    }
}

glm::vec3 ObjectModeInteractor::GetSelectionCenter()
{
    glm::vec3 center(0);
    for(auto& ob: _scene->selectedObjects)
    {
        center += ob->transform.GetLocation();
    }
    center /= _scene->selectedObjects.size();
    return center;
}

glm::vec3 ObjectModeInteractor::PositionOnWorldPlane(glm::ivec2 pos, glm::vec3 center)
{
    auto cam = _scene->editorCamera;
    auto v = center - cam->transform.GetLocation();
    float dist = dot(cam->transform.Forward(), v);
    auto r = cam->ScreenPointToRay(pos);
    float x = dot(cam->transform.Forward(), r.dir);
    return cam->transform.GetLocation() + r.dir * dist / x;
}

