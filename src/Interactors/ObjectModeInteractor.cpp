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
            Object* obj = _scene->PickObject();
            if(obj != nullptr)
            {
                _scene->SelectObject(obj, additive);
            }
            else
            {
                _scene->DeselectAll();
            }
        }   
        if(_scene->selectedObjects.size() > 0)
        {
            auto pos = Input::GetMousePosition();
            auto center = GetSelectionCenter();
            _startpos = PositionOnWorldPlane(pos, center);
            if(Input::GetKey(KeyCode::G))
            {
                _mode = Mode::MOVE;
                CacheTransforms();
            }
            else if(Input::GetKey(KeyCode::R))
            {
                _mode = Mode::ROTATE;
                CacheTransforms();
            }
            else if(Input::GetKey(KeyCode::S))
            {
                _mode = Mode::SCALE;
                CacheTransforms();
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
        auto center = GetSelectionCenter();
        auto wpos = PositionOnWorldPlane(pos, center);

        if(_mode == Mode::MOVE)
        {
            OnMove(pos, center, wpos);
        }
        else if(_mode == Mode::ROTATE)
        {
            OnRotate(pos, center, wpos);
        }
        else if(_mode == Mode::SCALE)
        {
            OnScale(pos, center, wpos);
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

void ObjectModeInteractor::CacheTransforms()
{
    _locationCache.clear();
    _rotationCache.clear();
    _scaleCache.clear();
    for(auto& ob: _scene->selectedObjects)
    {
        _locationCache.push_back(ob->transform.GetLocation());
        _rotationCache.push_back(ob->transform.GetRotation());
        _scaleCache.push_back(ob->transform.GetScale());
    }
}

void ObjectModeInteractor::OnMove(glm::ivec2 pos, glm::vec3 center, glm::vec3 wpos)
{
    auto delta = wpos - _startpos;
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

    int k = 0;
    for(auto& ob: _scene->selectedObjects)
    {
        ob->transform.SetLocation(_locationCache[k] + delta);
        k++;
    }
}

void ObjectModeInteractor::OnRotate(glm::ivec2 pos, glm::vec3 center, glm::vec3 wpos)
{
    auto v1 = center - wpos;
    auto v2 = center - _startpos;

    auto rot = glm::degrees(glm::eulerAngles(glm::rotation(normalize(v2), normalize(v1))));
    auto rlen = length(rot);
    if(rlen < 0.01f) // preventing NAN
    {
        return;
    }
    if(_axis == Axis::X)
    {
        rot.y = rot.z = 0;
    }
    if(_axis == Axis::Y)
    {
        rot.x = rot.z = 0;
    }
    if(_axis == Axis::Z)
    {
        rot.x = rot.y = 0;
    }

    rot = normalize(rot) * rlen;

    int k = 0;
    for(auto& ob: _scene->selectedObjects)
    {
        ob->transform.SetRotation(_rotationCache[k] + rot);
        k++;
    }
}

void ObjectModeInteractor::OnScale(glm::ivec2 pos, glm::vec3 center, glm::vec3 wpos)
{
    auto v1 = center - wpos;
    auto v2 = center - _startpos;

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
    int k = 0;
    for(auto& ob: _scene->selectedObjects)
    {
        ob->transform.SetScale(_scaleCache[k] + scale);
        k++;
    }
}
