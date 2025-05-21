#pragma once

#pragma warning(disable: 4324) // 構造体のメンバがアライメントのためにパディングされる

#include <Timer/Timer.h>
#include <Features/Particle/Particle.h>
#include <Features/Primitive/AABB.h>
#include <WinTools/WinTools.h>
#include <Features/GameEye/GameEye.h>
#include <Vector3.h>
#include <Vector4.h>
#include <string>
#include <memory>
#include <cstdint>

struct EmitterData
{
    ~EmitterData()
    {
        //_CrtDbgBreak();
    }

    EmitterData& operator= (const EmitterData& _rv);
    void Copy(const EmitterData& _rv, const EmitterData* _pDest = nullptr)
    {
        // コピー先とthisが一致するか確認
        if (_pDest != nullptr)
        {
            if (_pDest != this) __debugbreak();
        }

        operator=(_rv);

        return;
    }

    std::string     name_                       = {};                   // 名前
    Vector3         startScale_                 = {};                   // 開始スケール
    Vector3         endScale_                   = {};                   // 終了スケール

    float           emitInterval_               = {};                   // 発生間隔
    int32_t         emitNum_                    = {};                   // 発生数
    float           emitterLifeTime_            = {};                   // エミッタ寿命
    float           particleLifeTime_           = {};                   // パーティクル寿命
    float           scaleDelayTime_             = {};                   // スケール遅延時間
    Vector3         beginPosition_              = {};                   // 発生開始位置
    Vector3         endPosition_                = {};                   // 発生終了位置
    Vector3         emitPositionFixed_          = {};                   // ランダム発生しない場合の発生位置
    Vector4         beginColor_                 = {};                   // 開始色
    Vector4         endColor_                   = {};                   // 開始色
    float           alphaDeltaValue_            = {};                   // 透明度の変化量
    Vector3         velocityRandomRangeBegin_   = {};                   // 速度ランダム範囲
    Vector3         velocityRandomRangeEnd_     = {};                   // 速度ランダム範囲
    Vector3         velocityFixed_              = {};                   // 速度固定
    Vector3         gravity_                    = {};                   // 重力
    Vector3         resistance_                 = {};                   // 抵抗
    bool            enableRandomVelocity_       = {};                   // ランダム速度
    bool            enableRandomEmit_           = {};                   // ランダム発生
};


class ParticleEmitter
{
public:
    ParticleEmitter() = default;
    ~ParticleEmitter() = default;

    void Initialize(const std::string& _modelPath, const std::string& _jsonPath, bool _manualMode = false);
    void Update();
    void Draw();
    void Finalize();

    void Emit();

public: /// Setter
    void SetPosition(const Vector3& _position) { position_ = _position; }
    void SetGameEye(GameEye* _eye) { this->ModifyGameEye(_eye); }
    void SetEnableBillboard(bool _enable) { particle_->SetEnableBillboard(_enable); }

public: /// Getter
    //EmitterData& GetEmitterData() { return emitterData_; }

private:
    static constexpr uint32_t   kDefaultReserveCount_   = 6000u;
    std::string                 name_                   = {};               // 名前
    std::string                 ptrHex_                 = {};               // ポインタの16進数表記
    std::string                 particleName_           = {};               // 名前
    std::string                 jsonPath_               = {};               // JSONファイルパス
    Timer                       timer_                  = {};               // 計測用タイマー
    Timer                       reloadTimer_            = {};               // リロード用タイマー
    double                      reloadInterval_         = 1.0;              // リロード間隔
    EmitterData                 emitterData_            = {};               // エミッタデータ
    EmitterData                 fromJsonData_           = {};
    Particle*                   particle_               = nullptr;
    std::unique_ptr<AABB>       aabb_                   = nullptr;
    bool                        jsonFileExist_          = true;
    WinTools*                   winTools_               = nullptr;
    bool                        isManualMode_           = false;
    bool                        isEmitRequest_          = false;
    Vector3                     position_               = {};


private:
    void EmitParticle();


private:
    void DebugWindow();
    void ModifyGameEye(GameEye* _eye);
};