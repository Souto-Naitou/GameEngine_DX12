#pragma once

#include <Features/Model/IModel.h>
#include <Core/DirectX12/DirectX12.h>

#include <memory>
#include <string>


// モデルローダーインターフェイス
// - モデルをファイルから読み込む
class IModelLoader : public EngineFeature
{
public:
    virtual ~IModelLoader() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;

    virtual std::shared_ptr<IModel> LoadModel(const std::string& _path) = 0;
};