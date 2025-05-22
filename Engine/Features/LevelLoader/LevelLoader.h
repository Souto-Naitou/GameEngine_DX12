#pragma once
#include <string>

#include <nlohmann/json.hpp>
#include <memory>

struct LevelData
{
    struct ObjectData
    {
        std::string filename = {};
    };

    std::vector<ObjectData> objects;
};

class LevelLoader
{
    public:
    LevelLoader() = default;
    ~LevelLoader() = default;
    void LoadLevel(const std::string& _filename);

private:
    const std::string extension_ = ".json";
    const std::string directory_ = "Resources/Level/";

private:
    using json = nlohmann::json;
    std::unique_ptr<LevelData> levelData_ = nullptr;

private:
    void LoadJson(const std::string& _filename, json& _json);
    void DeserializeJsonData(json& _json);
    void DeserialiceMeshData(json& _object, LevelData::ObjectData& _objectData);
};