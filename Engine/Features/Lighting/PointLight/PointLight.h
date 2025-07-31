#pragma once
#include <memory>

#include <Common/structs.h>

#include <Vector4.h>
#include <Vector3.h>
#include <Features/Model/IModel.h>

class Object3d;

struct alignas(16) PointLightForGPU
{
    int enablePointLight    = 0; //!< ポイントライトの有効無効
    float padding0[3]       = {};
    Vector4 color           = {}; //!< ライトの色
    Vector3 position        = {}; //!< ライトの位置
    float padding2[1]       = {}; //!< パディング
    float intensity         = 0.0f; //!< 輝度
};

class PointLight
{
public:
    PointLight() = default;
    ~PointLight() = default;

    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    PointLight& SetIconModel(IModel* pModel);           //!< アイコンのモデルを設定

    bool IsEnable()                 const   { return enable_; }         //!< ポイントライトが有効かどうかを取得
    const Vector3& GetPosition()    const   { return position; }        //!< ライトの位置を取得
    const Vector4& GetColor()       const   { return color; }           //!< ライトの色を取得
    float GetIntensity()            const   { return intensity; }       //!< 輝度を取得

    bool&       IsEnable()                  { return enable_; }         //!< ポイントライトの有効無効を取得
    Vector3&    GetPosition()               { return position; }        //!< ライトの位置を取得
    Vector4&    GetColor()                  { return color; }           //!< ライトの色を取得
    float&      GetIntensity()              { return intensity; }       //!< 輝度を取得

    PointLightForGPU GetDataForGPU() const;                         //!< GPU用のデータを取得

private:
    bool    enable_                     = 0;            //!< ポイントライトの有効無効
    Vector4 color                       = {};           //!< ライトの色
    Vector3 position                    = {};           //!< ライトの位置
    float   intensity                   = 0.0f;         //!< 輝度
    std::unique_ptr<Object3d> pIcon_    = nullptr;
    IModel* pIconModel_                 = nullptr;      //!< アイコンのモデル
};