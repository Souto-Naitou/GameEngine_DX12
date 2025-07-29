#pragma once
#include <string>

#include <Features/LevelLoader/SceneObjects.h>
#include <Features/Model/ModelManager.h>

namespace Helper::Level
{
    SceneObjects LoadScene(const std::string& _path, ModelManager* _pModelManager);
}