#include "ModelManager.h"
#include <Features/Model/Helper/ModelHelper.h>
#include <filesystem>
#include <Features/Particle/Particle.h>
#include <algorithm>

void ModelManager::Initialize()
{
}

void ModelManager::AddSearchPath(const std::string& _path)
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

void ModelManager::AddAutoLoadPath(const std::string& _path)
{
    for (const auto& path : searchPaths_)
    {
        if (path == _path)
        {
            return;
        }
    }



    AddSearchPath(_path);

    autoLoadPaths_.push_back(_path);
}

void ModelManager::LoadAllModel()
{
    for (const auto& path : autoLoadPaths_)
    {
        for (auto itr = std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied);
            itr != std::filesystem::recursive_directory_iterator();
            ++itr){
            std::filesystem::path objPath = itr->path();
            if (objPath.extension() != ".obj")
            {
                continue;
            }
            AddAutoLoadPath(objPath.parent_path().string());
            LoadModel(objPath.string());
        }
    }
}

std::string ModelManager::GetDirectoryPath(std::string _fileName)
{
    std::string directoryPath = {};

    if (!std::filesystem::exists(_fileName))
    {
        std::filesystem::path argPath = _fileName;
        for (const auto& strPath : searchPaths_)
        {
            std::filesystem::path path = strPath;
            std::filesystem::path searchPath = path / argPath;
            if (std::filesystem::exists(searchPath))
            {
                directoryPath = strPath;
                break;
            }
        }
    }

    return directoryPath;
}

void ModelManager::LoadModel(const std::string& _filePath)
{
    std::filesystem::path fullpath = GetDirectoryPath(_filePath);
    if (fullpath.empty()) fullpath = _filePath;
    else fullpath += "/" + _filePath;

    for ( auto& model : models_ )
    {
        std::filesystem::path fsModelPath = GetLowerPath(model.first.string());
        std::filesystem::path fsFullPath = GetLowerPath(fullpath.string());

        if ( fsModelPath == fsFullPath )
        {
            return;
        }
    }

    std::unique_ptr<Model> model = std::make_unique<Model>();

    model->Initialize(fullpath.string());
    models_.emplace(fullpath, std::move(model));
}

Model* ModelManager::FindModel(const std::string& _filePath)
{
    std::string fullpath = GetDirectoryPath(_filePath);
    if (fullpath.empty()) fullpath = _filePath;
    else fullpath += "/" + _filePath;

    auto lowerFullPath = GetLowerPath(fullpath);

    Model* result = nullptr;

    for ( auto& model : models_ )
    {
        auto lowerModelPath = GetLowerPath(model.first.string());
        if ( lowerModelPath == lowerFullPath )
        {
            result = model.second.get();
            break;
        }
    }

    return result;
}

std::filesystem::path ModelManager::GetLowerPath(const std::string& _path)
{
    std::filesystem::path result = _path;
    std::string str = _path;

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    result = str;

    return result;
}

void ModelManager::Update()
{
}
