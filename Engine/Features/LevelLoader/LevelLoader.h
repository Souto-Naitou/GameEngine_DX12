#pragma once
#include <string>

#include <JsonParser/JsonLoader.h>

class LevelLoader
{
    public:
    LevelLoader() = default;
    ~LevelLoader() = default;
    void LoadLevel(const std::string& _filename);

private:
    const std::string extension_ = ".json";
    const std::string directory_ = "Resources/Level/";


};