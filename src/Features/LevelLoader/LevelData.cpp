#include "LevelData.h"
#include <Utility/JSON/jsonutl.h>

void BlenderLevel::assign_from_json_vec3(const nlohmann::json& _j, Vector3& _vec)
{
    _j[0].get_to<float>(_vec.x);
    _j[1].get_to<float>(_vec.y);
    _j[2].get_to<float>(_vec.z);
}

void BlenderLevel::assign_from_json_vec2(const nlohmann::json& _j, Vector2& _vec)
{
    _j[0].get_to<float>(_vec.x);
    _j[1].get_to<float>(_vec.y);
}

void BlenderLevel::assign_from_json(const nlohmann::json& _j, EulerTransform& _transform)
{
    auto& j_scales = _j.at("scales");
    _transform.scale.x = j_scales[0];
    _transform.scale.y = j_scales[1];
    _transform.scale.z = j_scales[2];

    auto& j_rotation = _j.at("rotation");
    _transform.rotate.x = j_rotation[0];
    _transform.rotate.y = j_rotation[1];
    _transform.rotate.z = j_rotation[2];

    auto& j_translation = _j.at("translation");
    _transform.translate.x = j_translation[0];
    _transform.translate.y = j_translation[1];
    _transform.translate.z = j_translation[2];
}

void BlenderLevel::assign_from_json(const nlohmann::json& _j, Collider& _collider)
{
    const auto& j_type = _j.at("type");
    if (j_type == "BOX")
    {
        BoxCollider boxCollider;
        boxCollider = _j;
        _collider = boxCollider;
    }
    else if (j_type == "SPHERE")
    {
        SphereCollider sphereCollider;
        sphereCollider = _j;
        _collider = sphereCollider;
    }
    else
    {
        throw std::runtime_error("Unknown collider type: " + j_type.get<std::string>());
    }
}

void BlenderLevel::from_json(const nlohmann::json& _j, LevelData& _levelData)
{
    _levelData.name = _j.at("name").get<std::string>();
    for (const auto& j_object : _j.at("objects"))
    {
        _levelData.objects.push_back(j_object);
    }
}

void BlenderLevel::from_json(const nlohmann::json& _j, BoxCollider& _boxCollider)
{
    utl::json::try_assign(_j, "center", assign_from_json_vec3, _boxCollider.center);
    utl::json::try_assign(_j, "size", assign_from_json_vec3, _boxCollider.size);
}

void BlenderLevel::from_json(const nlohmann::json& _j, SphereCollider& _sphereCollider)
{
    utl::json::try_assign(_j, "center", assign_from_json_vec3, _sphereCollider.center);
    utl::json::try_assign(_j, "radius", _sphereCollider.radius);
}

void BlenderLevel::from_json(const nlohmann::json& _j, Object& _object)
{
    // 必須項目
    utl::json::try_assign(_j, "type", _object.type);
    utl::json::try_assign(_j, "name", _object.name);
    assign_from_json(_j.at("transform"), _object.transform);

    // オプション項目
    utl::json::try_assign(_j, "file_name", _object.filename);
    if (_j.contains("collider"))
    {
        _object.collider = std::make_optional<Collider>();
        assign_from_json(_j.at("collider"), _object.collider.value());
    }
    utl::json::try_assign(_j, "uv_tiling", assign_from_json_vec2, _object.uvScale);
    if (_object.uvScale.x == 0.0f || _object.uvScale.y == 0.0f)
    {
        _object.uvScale = Vector2(1.0f, 1.0f); // デフォルト値
    }
}
