#include "FilePathSearcher.h"

#include <filesystem>

namespace fs = std::filesystem;

void FilePathSearcher::Initialize()
{
    searchPaths_.clear();
}

void FilePathSearcher::Finalize()
{
    searchPaths_.clear();
}

void FilePathSearcher::AddSearchPath(const std::string& _path)
{
    for (const auto& path : searchPaths_)
    {
        if (path == _path)
        {
            return;
        }
    }
    searchPaths_.push_back(_path);
}

std::string FilePathSearcher::GetParentPath(const std::string& _fileName)
{
    fs::path directoryPath = {};
    if (!fs::exists(_fileName))
    {
        for (const auto& path : searchPaths_)
        {
            fs::path filePath = path / _fileName;
            if (fs::exists(filePath))
            {
                directoryPath = path;
                break;
            }
        }
    }
    else
    {
        directoryPath = fs::path(_fileName).parent_path().string();
    }

    return directoryPath.string();
}

std::string FilePathSearcher::GetFilePath(const std::string& _fileName)
{
    fs::path filePath = {};
    if (!fs::exists(_fileName))
    {
        for (const auto& path : searchPaths_)
        {
            fs::path tempPath = path / _fileName;
            if (fs::exists(tempPath))
            {
                filePath = tempPath;
                break;
            }
        }
    }
    else
    {
        filePath = fs::path(_fileName);
    }

    return filePath.string();
}