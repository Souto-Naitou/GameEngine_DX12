#pragma once

#include <Core/DirectX12/DirectX12.h>
#include <Features/Model/Loader/IModelLoader.h>
#include <Features/Model/ModelStorage.h> // ModelStorage(Concrete)
#include <Utility/PathResolver/PathResolver.h>

class Particle;

/// モデル管理クラス
/// - ModelLoaderとModelStorageを統括するクラス
class ModelManager
{
public:
    ModelManager() = default;
    ~ModelManager() = default;

    void Initialize();

    void SetModelLoader(IModelLoader* _loader);
    void SetModelStorage(ModelStorage* _storage);

    void AddLoadPath(const std::string& _path); 
    void AddSearchPath(const std::string& _path);

    int LoadAll();

    IModel* Load(const std::string& _path);

private:
    IModelLoader*           pModelLoader_   = nullptr;
    ModelStorage*           pModelStorage_  = nullptr;
    PathResolver            pathResolver_   = {};
    std::list<std::string>  loadPaths_      = {};
};