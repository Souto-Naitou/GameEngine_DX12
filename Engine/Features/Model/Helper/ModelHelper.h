#pragma once

#include <string>

struct ModelData;
struct MaterialData;

namespace ModelHelper
{
    ModelData LoadObjFile(const std::string& _directoryPath, const std::string& _filename, const std::string& _texturePath = {});

    MaterialData LoadMaterialTemplateFile(const std::string& _directoryPath, const std::string& _filename, const std::string& _texturePath = {});
}