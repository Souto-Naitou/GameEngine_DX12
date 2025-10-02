#include "SceneArgs.h"

std::any& SceneArgs::Get(const std::string& _key)
{
    return argDataMap_.at(_key);
}

const std::any& SceneArgs::Get(const std::string& _key) const
{
    return argDataMap_.at(_key);
}

void SceneArgs::Set(const std::string& _key, const std::any& _value)
{
    argDataMap_[_key] = _value;
}

void SceneArgs::Reset()
{
    argDataMap_.clear();
}

void SceneArgs::Erase(const std::string& _key)
{
    argDataMap_.erase(_key);
}
