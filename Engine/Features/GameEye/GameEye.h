#pragma once
#include <Common/structs.h>
#include <Matrix4x4.h>
#include <string>

class GameEye
{
public:
    GameEye();
    ~GameEye();
    void                Update();

public: /// Getter
    const Transform&    GetTransform() const            { return transform_; }
    const Matrix4x4&    GetWorldMatrix() const          { return wMatrix_; }
    const Matrix4x4&    GetViewMatrix() const           { return vMatrix_; }
    const Matrix4x4&    GetProjectionMatrix() const     { return pMatrix_; }
    const Matrix4x4&    GetViewProjectionMatrix() const { return vpMatrix_; }
    const std::string&  GetName() const                 { return name_; }

public: /// Setter
    void SetTransform(const Transform& _transform)      { transform_ = _transform; }
    void SetRotate(const Vector3& _rotate)              { transform_.rotate = _rotate; }
    void SetTranslate(const Vector3& _translate)        { transform_.translate = _translate; }
    void SetFov(float _fov)                             { fovY_ = _fov; }
    void SetAspectRatio(float _aspect)                  { aspectRatio_ = _aspect; }
    void SetNearClip(float _near)                       { nearClip_ = _near; }
    void SetFarClip(float _far)                         { farClip_ = _far; }
    void SetName(const std::string& _name)              { name_ = _name; }

private: /// メンバ変数
    std::string         name_           = "unnamed";

    Transform           transform_      = {};       // 位置、回転、拡大縮小
    Matrix4x4           wMatrix_        = {};       // ワールド行列
    Matrix4x4           vMatrix_        = {};       // ビュー行列
    Matrix4x4           pMatrix_        = {};       // プロジェクション行列
    Matrix4x4           vpMatrix_       = {};       // ビュープロジェクション行列
    float               fovY_           = 0.0f;
    float               aspectRatio_    = 0.0f;
    float               nearClip_       = 0.0f;
    float               farClip_        = 0.0f;

private:
    void DebugWindow();
};