#pragma once

#include <Scene/SceneBase.h>
#include <string>

#include <memory>
#include "./ISceneArgs.h"

/// <summary>
/// シーンファクトリ インターフェース
/// </summary>
class ISceneFactory
{
public:
    virtual ~ISceneFactory() = default;

    virtual std::unique_ptr<SceneBase> CreateScene(const std::string& _sceneName, ISceneArgs* _pArgs) = 0;
};