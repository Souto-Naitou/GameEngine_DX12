#pragma once

#include <unordered_map> // unordered_map
#include <typeindex> // type_index

class DIContainer
{
public:
    template <typename T>
    void Register(T* _instance)
    {
        container_[std::type_index(typeid(T))] = _instance;
    }

    template <typename T>
    T* Resolve()
    {
        return static_cast<T*>(container_[std::type_index(typeid(T))]);
    }


private:
    std::unordered_map<std::type_index, void*> container_;
};