#pragma once
#include <unordered_map>
#include <filesystem>
#include <memory>

#include "./IModel.h"

// 読み込み済みモデル保持クラス
// - ファイルから一度読み込んだモデルのデータを保持するクラス
class ModelStorage
{
public:
    void Finalize();

    /// <summary>
    /// モデルデータを取得
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    /// <returns>モデルデータ</returns>
    IModel* FindModel(const std::string& _path);

    bool IsLoaded(const std::filesystem::path& _path);

    IModel* AddModel(const std::filesystem::path& _path, std::shared_ptr<IModel> _model);

private:
    std::unordered_map<std::filesystem::path, std::shared_ptr<IModel>> models_;
};