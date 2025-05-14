#pragma once

#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <unordered_map>
#include <filesystem>
#include <JsonParser/JsonLoader.h>
#include <memory>
#include <string>

class EmitterManager
{
public:
    EmitterManager(const EmitterManager&) = delete;
    EmitterManager& operator=(const EmitterManager&) = delete;
    EmitterManager(EmitterManager&&) = delete;
    EmitterManager& operator=(EmitterManager&&) = delete;

    static EmitterManager* GetInstance()
    {
        static EmitterManager instance;
        return &instance;
    }

    const EmitterData& LoadFile(const std::string& _path);
    const EmitterData& ReloadFile(const std::string& _path);
    void SaveFile(const std::string& _path, const EmitterData& _data);

    static std::shared_ptr<Json::Value> ParseVector3(const Vector3& _vec);
    static std::shared_ptr<Json::Value> ParseVector4(const Vector4& _vec);
    static std::shared_ptr<Json::Value> ParseColor(const Color& _color);

    static Vector3 ParseVector3(Json::Object& _obj);
    static Vector4 ParseVector4(Json::Object& _obj);
    static Color ParseColor(Json::Object& _obj);

private:
    EmitterManager() = default;
    ~EmitterManager() = default;

    void ParseJsonValue(Json::Value& _loader, EmitterData& _data);
    Json::Value ParseEmitterData(const EmitterData& _data);


private:
    std::unordered_map<std::filesystem::path, EmitterData> emitterMap_;
    const std::string kDefaultPath = "EngineResources/Json/EmitterDefault.json";

};