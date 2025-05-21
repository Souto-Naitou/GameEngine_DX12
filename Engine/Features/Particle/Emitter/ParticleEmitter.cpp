#include "ParticleEmitter.h"

#ifdef _DEBUG
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiTemplates/ImGuiTemplates.h>
#include <sstream>
#endif // _DEBUG

#include <Features/Particle/ParticleManager.h>
#include <Features/RandomGenerator/RandomGenerator.h>
#include <Features/Particle/Manager/EmitterManager.h>

const uint32_t ParticleEmitter::kDefaultReserveCount_;

EmitterData& EmitterData::operator=(const EmitterData & _rv)
{
    if (&_rv == this) return *this;
    name_                           = _rv.name_;
    startScale_                     = _rv.startScale_;
    endScale_                       = _rv.endScale_;
    emitInterval_                   = _rv.emitInterval_;
    emitNum_                        = _rv.emitNum_;
    emitterLifeTime_                = _rv.emitterLifeTime_;
    particleLifeTime_               = _rv.particleLifeTime_;
    scaleDelayTime_                 = _rv.scaleDelayTime_;
    beginPosition_                  = _rv.beginPosition_;
    endPosition_                    = _rv.endPosition_;
    emitPositionFixed_              = _rv.emitPositionFixed_;
    beginColor_                     = _rv.beginColor_;
    endColor_                       = _rv.endColor_;
    alphaDeltaValue_                = _rv.alphaDeltaValue_;
    velocityRandomRangeBegin_       = _rv.velocityRandomRangeBegin_;
    velocityRandomRangeEnd_         = _rv.velocityRandomRangeEnd_;
    velocityFixed_                  = _rv.velocityFixed_;
    gravity_                        = _rv.gravity_;
    resistance_                     = _rv.resistance_;
    enableRandomVelocity_           = _rv.enableRandomVelocity_;
    enableRandomEmit_               = _rv.enableRandomEmit_;

    return *this;
}

void ParticleEmitter::Initialize(const std::string& _modelPath, const std::string& _jsonPath, bool _manualMode)
{
    auto size = sizeof(ParticleEmitter);
    size;

#ifdef _DEBUG
    std::stringstream ss;
    ss << "0x" << std::hex << this;
    ptrHex_ = ss.str();

    name_ = particleName_ + "##" + ptrHex_;
    DebugManager::GetInstance()->SetComponent("ParticleEmitter", name_, std::bind(&ParticleEmitter::DebugWindow, this));
#endif // _DEBUG

    winTools_ = WinTools::GetInstance();

    jsonPath_ = _jsonPath;

    timer_.Start();
    reloadTimer_.Start();
    particle_ = &ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize(_modelPath);
    particle_->reserve(kDefaultReserveCount_);

    fromJsonData_ = EmitterManager::GetInstance()->LoadFile(jsonPath_);
    emitterData_ = fromJsonData_;

    auto addr = std::addressof(emitterData_);
    auto addr2 = std::addressof(fromJsonData_);
    addr;
    addr2;

    if (!emitterData_.name_.empty())
    {
        particleName_ = emitterData_.name_;
        name_ = particleName_ + "##" + ptrHex_;
    }

    aabb_ = std::make_unique<AABB>();
    aabb_->Initialize();

    isManualMode_ = _manualMode;
}

void ParticleEmitter::Update()
{
    if (timer_.GetNow<float>() > emitterData_.emitInterval_)
    {
        if (emitterData_.emitNum_ < 0)
        {
            emitterData_.emitNum_ = 0;
        }
        for (int32_t i = 0; i < emitterData_.emitNum_; ++i)
        {
            if (!isManualMode_)
            {
                EmitParticle();
            }
        }
        isEmitRequest_ = false;
        timer_.Reset();
        timer_.Start();
    }

    emitterData_ = fromJsonData_;
    emitterData_.emitPositionFixed_ = fromJsonData_.emitPositionFixed_ + position_;
    emitterData_.beginPosition_ = fromJsonData_.beginPosition_ + position_;
    emitterData_.endPosition_ = fromJsonData_.endPosition_ + position_;

    aabb_->SetMinMax(emitterData_.beginPosition_, emitterData_.endPosition_);
}

void ParticleEmitter::Draw()
{
#ifdef _DEBUG
    if (emitterData_.enableRandomEmit_)
    {
        aabb_->Draw();
    }
#endif // _DEBUG
}

void ParticleEmitter::EmitParticle()
{
    RandomGenerator* random = RandomGenerator::GetInstance();

    particle_->emplace_back({});
    auto& parameter = particle_->GetParticleData();
    auto& datum = parameter.back();

    /// 初期トランスフォーム
    if (emitterData_.enableRandomEmit_)
    {
        datum.transform_.translate = Vector3(
            random->Generate(emitterData_.beginPosition_.x, emitterData_.endPosition_.x),
            random->Generate(emitterData_.beginPosition_.y, emitterData_.endPosition_.y),
            random->Generate(emitterData_.beginPosition_.z, emitterData_.endPosition_.z));
    }
    else
    {
        datum.transform_.translate = emitterData_.emitPositionFixed_;
    }

    /// スケール
    datum.transform_.scale = emitterData_.startScale_;
    datum.startScale_ = emitterData_.startScale_;
    datum.endScale_ = emitterData_.endScale_;
    datum.scaleDelayTime_ = emitterData_.scaleDelayTime_;

    /// ライフタイム
    datum.lifeTime_ = emitterData_.particleLifeTime_;

    /// 速度
    if (emitterData_.enableRandomVelocity_)
    {
        datum.velocity_ = Vector3(
            random->Generate(emitterData_.velocityRandomRangeBegin_.x, emitterData_.velocityRandomRangeEnd_.x),
            random->Generate(emitterData_.velocityRandomRangeBegin_.y, emitterData_.velocityRandomRangeEnd_.y),
            random->Generate(emitterData_.velocityRandomRangeBegin_.z, emitterData_.velocityRandomRangeEnd_.z)
        );
    }
    else
    {
        datum.velocity_ = emitterData_.velocityFixed_;
    }

    // 初期の色
    datum.beginColor_ = emitterData_.beginColor_;
    // 終了の色
    datum.endColor_ = emitterData_.endColor_;

    // アルファ値の変化量
    datum.alphaDeltaValue_ = emitterData_.alphaDeltaValue_;
    // 消去条件
    datum.deleteCondition_ = ParticleDeleteCondition::LifeTime;
    datum.accGravity_ = emitterData_.gravity_;
    datum.accResistance_ = emitterData_.resistance_;

    aabb_->SetMinMax(emitterData_.beginPosition_, emitterData_.endPosition_);
}

void ParticleEmitter::DebugWindow()
{
#ifdef _DEBUG

    char path[512] = "";
    std::memcpy(path, jsonPath_.c_str(), jsonPath_.size());

    ImGui::Text("Name : %s", particleName_.c_str());
    if (ImGui::CollapsingHeader("一般"))
    {
        if (ImGui::Button("マニュアル発生"))
        {
            EmitParticle();
        }
        if (ImGui::InputText("ファイルパス", path, sizeof(path)))
        {
            jsonFileExist_ = std::filesystem::directory_entry(path).exists();
        }

        if (ImGui::Button("保存"))
        {
            EmitterManager::GetInstance()->SaveFile(path, fromJsonData_);
        }
        ImGui::SameLine();
        if (ImGui::Button("読み込み"))
        {
            if (std::filesystem::directory_entry(path).exists())
            {
                fromJsonData_ = EmitterManager::GetInstance()->ReloadFile(path);
                /// 名前が空でないなら
                if (!fromJsonData_.name_.empty())
                {
                    name_ = fromJsonData_.name_;
                    name_ = fromJsonData_.name_ + "##" + ptrHex_;
                }
                jsonFileExist_ = true;
            }
            else
            {
                DebugManager::GetInstance()->PushLog("ファイルが存在しません");
                jsonFileExist_ = false;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("参照"))
        {
            std::string temp = winTools_->OpenFileDialog();
            if (!temp.empty() && std::filesystem::path(temp).extension() == ".json")
            {
                std::memcpy(path, temp.c_str(), temp.size());
                jsonFileExist_ = true;
            }
            else jsonFileExist_ = false;
        }
        if (!jsonFileExist_)
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ファイルが存在しないか、拡張子がjsonではありません。");
        }

        ImGui::DragFloat("パーティクル寿命", &fromJsonData_.particleLifeTime_, 0.1f, 0.0f, FLT_MAX);

        ImGui::DragFloat("発生間隔", &fromJsonData_.emitInterval_, 0.02f, 0.02f, FLT_MAX);

        ImGui::InputInt("発生数", (int*)&fromJsonData_.emitNum_);

        ImGui::ColorEdit4("開始色", &fromJsonData_.beginColor_.x);
        ImGui::SameLine();
        if (ImGui::Button("入れ替え##Color"))
        {
            Vector4 temp = fromJsonData_.beginColor_;
            fromJsonData_.beginColor_ = fromJsonData_.endColor_;
            fromJsonData_.endColor_ = temp;
        }

        ImGui::ColorEdit4("終了色", &fromJsonData_.endColor_.x);
        ImGui::SameLine();
        if (ImGui::Button("同期##Color"))
        {
            fromJsonData_.endColor_ = fromJsonData_.beginColor_;
        }

        ImGui::SliderFloat("透明度の変化量", &fromJsonData_.alphaDeltaValue_, -0.2f, 0.0f);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("変形"))
    {
        ImGui::DragFloat3("開始スケール", &fromJsonData_.startScale_.x, 0.01f);
        ImGui::SameLine();
        if (ImGui::Button("入れ替え##Scale"))
        {
            Vector3 temp = fromJsonData_.startScale_;
            fromJsonData_.startScale_ = fromJsonData_.endScale_;
            fromJsonData_.endScale_ = temp;
        }

        ImGui::DragFloat3("終了スケール", &fromJsonData_.endScale_.x, 0.01f);
        ImGui::SameLine();
        if (ImGui::Button("同期##Scale"))
        {
            fromJsonData_.endScale_ = fromJsonData_.startScale_;
        }
        ImGui::DragFloat("スケール遅延時間", &fromJsonData_.scaleDelayTime_, 0.01f);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("生成場所"))
    {
        ImGui::Checkbox("ランダム範囲生成", &fromJsonData_.enableRandomEmit_);
        if (fromJsonData_.enableRandomEmit_)
        {
            ImGui::DragFloat3("発生開始地点", &fromJsonData_.beginPosition_.x, 0.01f);
            ImGui::DragFloat3("発生終了地点", &fromJsonData_.endPosition_.x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("発生位置", &fromJsonData_.emitPositionFixed_.x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("速度"))
    {
        ImGui::Checkbox("速度のランダマイズ", &fromJsonData_.enableRandomVelocity_);
        if (fromJsonData_.enableRandomVelocity_)
        {
            ImGui::DragFloat3("速度ランダム範囲-開始", &fromJsonData_.velocityRandomRangeBegin_.x, 0.01f);
            ImGui::DragFloat3("速度ランダム範囲-終了", &fromJsonData_.velocityRandomRangeEnd_.x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("速度", &fromJsonData_.velocityFixed_.x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("物理"))
    {
        ImGui::DragFloat3("重力", &fromJsonData_.gravity_.x, 0.01f);
        ImGui::DragFloat3("抵抗", &fromJsonData_.resistance_.x, 0.01f);

    }

    jsonPath_ = path;
#endif // _DEBUG
}

void ParticleEmitter::ModifyGameEye(GameEye* _eye)
{
    particle_->SetGameEye(_eye);
    aabb_->SetGameEye(_eye);
}

void ParticleEmitter::Finalize()
{
#ifdef _DEBUG
    DebugManager::GetInstance()->DeleteComponent("ParticleEmitter", name_.c_str());
#endif

    ParticleManager::GetInstance()->ReserveDeleteParticle(particle_);
    particle_ = nullptr;
}

void ParticleEmitter::Emit()
{
    if (isManualMode_)
    {
        this->Update();
        for (int32_t i = 0; i < emitterData_.emitNum_; ++i)
        {
            EmitParticle();
        }
    }
}