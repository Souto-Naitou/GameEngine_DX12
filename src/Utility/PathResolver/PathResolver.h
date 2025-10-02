#pragma once

#include <string>
#include <filesystem>
#include <vector>

class PathResolver
{
public:
    PathResolver() = default;
    ~PathResolver() = default;

    void Initialize();
    void Finalize();

    /// <summary>
    /// 検索パスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddSearchPath(const std::string& _path);


    /// <summary>
    /// ファイルの親ディレクトリを取得
    /// </summary>
    /// <returns>親ディレクトリのパス</returns>
    [[nodiscard]]
    std::string GetParentPath(const std::string& _fileName);


    /// <summary>
    /// 検索パスから検索し、ファイルパスを取得
    /// </summary>
    /// <param name="_fileName">ファイル名</param>
    /// <returns>ファイルパス</returns>
    [[nodiscard]]
    std::string GetFilePath(const std::string& _fileName);

private:
    std::vector<std::filesystem::path> searchPaths_ = {};
};