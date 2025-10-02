#pragma once
#include <any>
#include <string>

class ISceneArgs
{
public:
    virtual ~ISceneArgs() {}
    virtual std::any& Get(const std::string& _key) = 0;
    virtual const std::any& Get(const std::string& _key) const = 0;
    virtual void Set(const std::string& _key, const std::any& _value) = 0;
    virtual void Reset() = 0;
    virtual void Erase(const std::string& _key) = 0;
};