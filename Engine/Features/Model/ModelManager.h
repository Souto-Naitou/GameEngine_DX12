#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <Core/DirectX12/DirectX12.h>
#include <list>
#include <functional>
#include <queue>
#include "Model.h"
#include <filesystem>

class Particle;

class ModelManager
{
public:
    ModelManager(ModelManager&) = delete;
    ModelManager(ModelManager&&) = delete;
    ModelManager& operator=(ModelManager&) = delete;
    ModelManager& operator=(ModelManager&&) = delete;

    static ModelManager* GetInstance()
    {
        static ModelManager instance;
        return &instance;
    }

    void Initialize();
    void Update();

    /// <summary>
    /// 検索パスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddSearchPath(const std::string& _path);

    /// <summary>
    /// 自動読み込みパスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddAutoLoadPath(const std::string& _path);

    /// <summary>
    /// 全てのモデルデータを読み込む
    /// </summary>
    void LoadAllModel();

    /// <summary>
    /// 検索パスを取得
    /// </summary>
    /// <returns>検索パス</returns>
    std::string GetDirectoryPath(std::string _fileName);

    /// <summary>
    /// モデルデータを読み込む
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    void LoadModel(const std::string& _filePath);

    /// <summary>
    /// モデルデータを取得
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    /// <returns>モデルデータ</returns>
    Model* FindModel(const std::string& _filePath);

private:
    std::list<std::string> searchPaths_;
    std::list<std::string> autoLoadPaths_;

    std::unordered_map<std::filesystem::path, std::unique_ptr<Model>> models_;

    std::queue<Particle*> uploadQueueParticle_;

    ModelManager() = default;
    ~ModelManager() = default;

    std::filesystem::path GetLowerPath(const std::string& _path);
};