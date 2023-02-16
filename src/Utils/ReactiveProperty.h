#pragma once

#include <functional>
#include <vector>

template <typename T>
class ReactiveProperty
{
    public:
        ReactiveProperty() : _value() {}
        ReactiveProperty(T value) : _value(value) {}
        ReactiveProperty(const ReactiveProperty& rp) = delete;
        ReactiveProperty& operator=(ReactiveProperty rp) = delete;

        T Get()
        {
            return _value;
        }
        void Set(T value)
        {
            if(value != _value)
            {
                _value = value;
                // Notify
                for(auto& sub: _subs)
                {
                    sub(_value);
                }
            }
        }
        void Subscribe(std::function<void(T)> action)
        {
            _subs.push_back(action);
        }
    private:
        T _value;
        std::vector< std::function<void(T)> > _subs;
};

typedef ReactiveProperty<int> ReactiveInt;
typedef ReactiveProperty<float> ReactiveFloat;
typedef ReactiveProperty<bool> ReactiveBool;

