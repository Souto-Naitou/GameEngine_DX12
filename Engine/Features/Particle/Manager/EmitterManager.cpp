#include "EmitterManager.h"
#include <type_traits>

const EmitterData& EmitterManager::LoadFile(const std::string& _path)
{
    JsonLoader& loader = JsonLoader::GetInstance();

    EmitterData data = {};
    loader.LoadFile(_path);
    Json::Value root = loader[_path];

    if (root.type == Json::Value::Type::Null)
    {
        loader.LoadFile(kDefaultPath);
        root = loader[kDefaultPath];
        ParseJsonValue(root, data);
        this->SaveFile(_path, data);
    }
    else
    {
        ParseJsonValue(root, data);
    }

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

const EmitterData& EmitterManager::ReloadFile(const std::string& _path)
{
    JsonLoader& loader = JsonLoader::GetInstance();

    if (!loader.IsExist(_path)) return LoadFile(_path);

    loader.ReloadFile(_path);

    Json::Value root = loader[_path];

    EmitterData data = {};
    ParseJsonValue(root, data);

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

void EmitterManager::SaveFile(const std::string& _path, const EmitterData& _data)
{
    JsonLoader& loader = JsonLoader::GetInstance();
    Json::Value data = ParseEmitterData(_data);
    loader.SaveFile(_path, data);
}

void EmitterManager::ParseJsonValue(Json::Value& _loader, EmitterData& _data)
{
    Json::Object obj = {};

    /// [Name]
    _data.name_ = _loader["emitterName"];

    /// [Transform]

    obj = _loader["startScale"];
    _data.startScale_ = ParseVector3(obj);

    obj = _loader["endScale"];
    _data.endScale_ = ParseVector3(obj);

    _data.scaleDelayTime_ = _loader["scaleDelayTime"];


    /// [Common]

    _data.particleLifeTime_ = _loader["particleLifeTime"];

    _data.emitInterval_ = _loader["emitInterval"];

    _data.emitNum_ = _loader["emitNum"];

    _data.emitterLifeTime_ = _loader["emitterLifeTime"];


    /// [Generate]

    _data.enableRandomEmit_ = _loader["enableRandomEmit"];

    obj = _loader["beginPosition"];
    _data.beginPosition_ = ParseVector3(obj);

    obj = _loader["endPosition"];
    _data.endPosition_ = ParseVector3(obj);

    obj = _loader["emitPositionFixed"];
    _data.emitPositionFixed_ = ParseVector3(obj);


    /// [Velocity]

    _data.enableRandomVelocity_ = _loader["enableRandomVelocity"];

    obj = _loader["velocityRandomRangeBegin"];
    _data.velocityRandomRangeBegin_ = ParseVector3(obj);

    obj = _loader["velocityRandomRangeEnd"];
    _data.velocityRandomRangeEnd_ = ParseVector3(obj);

    obj = _loader["velocityFixed"];
    _data.velocityFixed_ = ParseVector3(obj);


    /// [Color]

    obj = _loader["beginColor"];
    _data.beginColor_ = ParseColor(obj);

    obj = _loader["endColor"];
    _data.endColor_ = ParseColor(obj);

    _data.alphaDeltaValue_ = _loader["alphaDeltaValue"];


    /// [Physics]

    obj = _loader["gravity"];
    _data.gravity_ = ParseVector3(obj);

    obj = _loader["resistance"];
    _data.resistance_ = ParseVector3(obj);


    return;
}

Json::Value EmitterManager::ParseEmitterData(const EmitterData& _data)
{
    Json::Value value = {};
    Json::Object root = {};

    /// [Name]
    root["emitterName"] = std::make_shared<Json::Value>(Json::Value::Type::String, _data.name_);

    /// [Transform]

    root["startScale"] = ParseVector3(_data.startScale_);
    root["endScale"] = ParseVector3(_data.endScale_);
    root["scaleDelayTime"] = std::make_shared<Json::Value>(Json::Value::Type::Float, _data.scaleDelayTime_);


    /// [Common]

    root["particleLifeTime"] = std::make_shared<Json::Value>(Json::Value::Type::Float, _data.particleLifeTime_);
    root["emitInterval"] = std::make_shared<Json::Value>(Json::Value::Type::Float, _data.emitInterval_);
    root["emitNum"] = std::make_shared<Json::Value>(Json::Value::Type::Int, _data.emitNum_);
    root["emitterLifeTime"] = std::make_shared<Json::Value>(Json::Value::Type::Float, _data.emitterLifeTime_);


    /// [Generate]


    root["enableRandomEmit"] = std::make_shared<Json::Value>(Json::Value::Type::Bool, _data.enableRandomEmit_);
    root["beginPosition"] = ParseVector3(_data.beginPosition_);
    root["endPosition"] = ParseVector3(_data.endPosition_);
    root["emitPositionFixed"] = ParseVector3(_data.emitPositionFixed_);


    /// [Velocity]

    root["enableRandomVelocity"] = std::make_shared<Json::Value>(Json::Value::Type::Bool, _data.enableRandomVelocity_);
    root["velocityRandomRangeBegin"] = ParseVector3(_data.velocityRandomRangeBegin_);
    root["velocityRandomRangeEnd"] = ParseVector3(_data.velocityRandomRangeEnd_);
    root["velocityFixed"] = ParseVector3(_data.velocityFixed_);


    /// [Color]

    root["beginColor"] = ParseColor(_data.beginColor_);
    root["endColor"] = ParseColor(_data.endColor_);
    root["alphaDeltaValue"] = std::make_shared<Json::Value>(Json::Value::Type::Float, _data.alphaDeltaValue_);


    /// [Physics]

    root["gravity"] = ParseVector3(_data.gravity_);
    root["resistance"] = ParseVector3(_data.resistance_);


    value.type = Json::Value::Type::Object;
    value.value = root;

    return value;
}

Vector3 EmitterManager::ParseVector3(Json::Object& _obj)
{
    float x, y, z;
    x = *_obj["x"]; y = *_obj["y"]; z = *_obj["z"];
    return Vector3(x,y,z);
}

Vector4 EmitterManager::ParseVector4(Json::Object& _obj)
{
    float x, y, z, w;
    x = *_obj["x"]; y = *_obj["y"]; z = *_obj["z"]; w = *_obj["w"];
    return Vector4(x, y, z, w);
}

Color EmitterManager::ParseColor(Json::Object& _obj)
{
    float r, g, b, a;
    r = *_obj["r"]; g = *_obj["g"]; b = *_obj["b"]; a = *_obj["a"];
    return Color(r, g, b, a);
}

std::shared_ptr<Json::Value> EmitterManager::ParseVector3(const Vector3& _vec)
{
    auto result = std::make_shared<Json::Value>();
    Json::Object obj = {};
    auto x = std::make_shared<Json::Value>();
    auto y = std::make_shared<Json::Value>();
    auto z = std::make_shared<Json::Value>();
    x->type = Json::Value::Type::Float;
    y->type = Json::Value::Type::Float;
    z->type = Json::Value::Type::Float;
    x->value = _vec.x;
    y->value = _vec.y;
    z->value = _vec.z;
    obj["x"] = x;
    obj["y"] = y;
    obj["z"] = z;

    result->type = Json::Value::Type::Object;
    result->value = obj;

    return result;
}

std::shared_ptr<Json::Value> EmitterManager::ParseVector4(const Vector4& _vec)
{
    auto result = std::make_shared<Json::Value>();
    Json::Object obj = {};
    auto x = std::make_shared<Json::Value>();
    auto y = std::make_shared<Json::Value>();
    auto z = std::make_shared<Json::Value>();
    auto w = std::make_shared<Json::Value>();

    x->type = Json::Value::Type::Float;
    y->type = Json::Value::Type::Float;
    z->type = Json::Value::Type::Float;
    w->type = Json::Value::Type::Float;

    x->value = _vec.x;
    y->value = _vec.y;
    z->value = _vec.z;
    w->value = _vec.w;

    obj["x"] = x;
    obj["y"] = y;
    obj["z"] = z;
    obj["w"] = w;

    result->type = Json::Value::Type::Object;
    result->value = obj;

    return result;
}

std::shared_ptr<Json::Value> EmitterManager::ParseColor(const Color& _color)
{
    auto result = std::make_shared<Json::Value>();
    Json::Object obj = {};
    auto r = std::make_shared<Json::Value>();
    auto g = std::make_shared<Json::Value>();
    auto b = std::make_shared<Json::Value>();
    auto a = std::make_shared<Json::Value>();

    r->type = Json::Value::Type::Float;
    g->type = Json::Value::Type::Float;
    b->type = Json::Value::Type::Float;
    a->type = Json::Value::Type::Float;

    r->value = static_cast<float>(_color.rgba.r) / 255.0f;
    g->value = static_cast<float>(_color.rgba.g) / 255.0f;
    b->value = static_cast<float>(_color.rgba.b) / 255.0f;
    a->value = static_cast<float>(_color.rgba.a) / 255.0f;
    obj["r"] = r;
    obj["g"] = g;
    obj["b"] = b;
    obj["a"] = a;

    result->type = Json::Value::Type::Object;
    result->value = obj;

    return result;
}
