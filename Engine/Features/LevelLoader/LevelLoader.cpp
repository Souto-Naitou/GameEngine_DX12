#include "LevelLoader.h"
#include <fstream>

void LevelLoader::LoadLevel(const std::string& _filename)
{
    json jsondata;

    LoadJson(_filename, jsondata);


}

void LevelLoader::LoadJson(const std::string& _filename, json& _json)
{
    const std::string fullpath = directory_ + _filename + extension_;

    std::fstream file(fullpath);

    if (file.is_open() == false)
    {
        // ファイル開けてない
        __debugbreak;
    };

    file >> _json;

    file.close();
}

void LevelLoader::DeserializeJsonData(json& _json)
{
    std::string jsonname = _json["name"].get<std::string>();
    if (jsonname != "scene") __debugbreak;

    for (json& object : _json["objects"])
    {
        if (object != "type") __debugbreak;

        // 種別を取得
        std::string type = object["type"].get<std::string>();

        // 種類ごとの処理
        if (type == "MESH")
        {
            levelData_->objects.emplace_back(LevelData::ObjectData{});
            auto& objectData = levelData_->objects.back();


        }
    }
}

void LevelLoader::DeserialiceMeshData(json& _object, LevelData::ObjectData& _objectData)
{
    if (_object == "file_name")
    {
        _objectData.filename = _object["file_name"];
    }
}
