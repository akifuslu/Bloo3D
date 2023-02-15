#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "pch.h"
#include "Logger.h"

class Object;

class Transform
{
    public:
        Transform(Object* obj) 
            : _object(obj),
              _location(0), 
              _rotation(0), 
              _scale(1),
              _localToWorld(1),
              _worldToLocal(1),
              _t_worldToLocal(1),
              _parent(nullptr)
        {}
        ~Transform() = default;
        Transform(const Transform& other) = delete;
        Transform& operator=(const Transform& other) = delete;
        std::function<void()> onUpdate;

        inline Object* GetObject() const
        {
            return _object;
        }
        inline glm::vec3 GetLocation() const
        {
            return _location;
        }
        inline glm::vec3 GetRotation() const
        {
            return _rotation;
        }
        inline glm::vec3 GetScale() const
        {
            return _scale;            
        }
        inline Transform* GetParent() const
        {
            return _parent;
        }
        inline void SetLocation(glm::vec3 loc)
        {
            _location = loc;
            RebuildMat();
        }
        inline void SetRotation(glm::vec3 rot)
        {
            _rotation = rot;
            RebuildMat();
        }
        inline void SetScale(glm::vec3 sca)
        {
            _scale = sca;
            RebuildMat();
        }
        inline void SetParent(Transform* parent)
        {
            if(_parent == parent) // no parent change
                return;
            if(parent == this)
            {
                LOGERROR("Can't set parent of the object to itself!");
                return;
            }
            if(_parent != nullptr) // remove child from the previous parent
            {
                _parent->_child.remove(this);
            }
            _parent = parent;
            _parent->_child.push_back(this);
            RebuildMat();
        }
        inline const std::list<Transform*> GetChilds() const
        {
            return _child;
        }
        inline glm::vec3 Right() const
        {
            return _localToWorld[0];
        }
        inline glm::vec3 Up() const
        {
            return _localToWorld[1];
        }
        inline glm::vec3 Forward() const
        {
            return _localToWorld[2];
        }
        inline glm::mat4 LocalToWorld() const
        {
            return _localToWorld;
        }
        inline glm::mat4 WorldToLocal() const
        {
            return _worldToLocal;
        }
        inline glm::mat4 T_WorldToLocal() const
        {
            return _t_worldToLocal;
        }
    private:
        void RebuildMat()
        { 
            _localToWorld = glm::translate(_location) * glm::toMat4(glm::quat(glm::radians(_rotation))) * glm::scale(_scale);
            Transform* tmp = _parent;
            while(tmp != nullptr)
            {
                _localToWorld = tmp->_localToWorld * _localToWorld;
                tmp = tmp->_parent;
            }

            _worldToLocal = inverse(_localToWorld);
            _t_worldToLocal = transpose(_worldToLocal);
            if(onUpdate != nullptr)
            {
                onUpdate();
            }
            for(auto& ch: _child) // inform child transforms to rebuild mats
            {
                ch->RebuildMat();
            }
        }        
        Object* _object;
        glm::vec3 _location;
        glm::vec3 _rotation;
        glm::vec3 _scale;
        glm::mat4 _localToWorld;
        glm::mat4 _worldToLocal;
        glm::mat4 _t_worldToLocal; // used for transforming normal vectors to world space (transpose(inverse(localToWorld)) = transpose(worldToLocal))
        Transform* _parent;
        std::list<Transform*> _child;        
};
