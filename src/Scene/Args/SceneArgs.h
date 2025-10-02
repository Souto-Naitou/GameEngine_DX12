#pragma once

#include <Interfaces/ISceneArgs.h>
#include <unordered_map>

// ISceneArgsの簡易実装クラス
// - コンテナとしてstd::unordered_mapを使用
class SceneArgs : public ISceneArgs
{
public:
    std::any& Get(const std::string& _key) override;
    const std::any& Get(const std::string& _key) const override;
    void Set(const std::string& _key, const std::any& _value) override;
    void Reset() override;
    void Erase(const std::string& _key) override;

private:
    std::unordered_map<std::string, std::any> argDataMap_;
};