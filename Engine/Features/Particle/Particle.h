#pragma once

#include <string>

#include <Core/DirectX12/DirectX12.h>
#include <Common/structs.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <Features/Model/Model.h>
#include "ParticleSystem.h"
#include <Matrix4x4.h>
#include <Timer/Timer.h>
#include <vector>
#include <Vector4.h>
#include <Vector3.h>
#include <cstdint>

enum class ParticleDeleteCondition : int
{
    LifeTime,
    ZeroAlpha,
};

struct ParticleData
{
    Timer                                   timer_              = {};
    Transform                               transform_          = {};
    Vector3                                 startScale_         = {};
    Vector3                                 endScale_           = {};
    Vector3                                 acceleration_       = {};
    Vector4                                 currentColor_       = {};
    Vector4                                 beginColor_         = {};
    Vector4                                 endColor_           = {};
    float                                   scaleDelayTime_     = 0.0f;
    float                                   alphaDeltaValue_    = 0.0f;
    float                                   lifeTime_           = 0.0f;
    float                                   currentLifeTime_    = 0.0f;
    Vector3                                 accResistance_      = {};
    Vector3                                 accGravity_         = {};
    Vector3                                 velocity_           = {};
    ParticleDeleteCondition                 deleteCondition_    = ParticleDeleteCondition::LifeTime;
};

class Particle
{
public:
    Particle() = default;

    void Initialize(const std::string& _filepath);
    void Draw();
    void Update();
    void Finalize();


public: /// Setter
    void SetName(const std::string& _name) { name_ = _name; }
    void SetEnableBillboard(bool _enable) { enableBillboard_ = _enable; }
    void SetGameEye(GameEye* _pGameEye) { pGameEye_ = _pGameEye; }


public: /// Getter
    const std::string& GetName() const { return name_; }
    bool GetEnableBillboard() const { return enableBillboard_; }
    auto& GetParticleData() { return particleData_; }
    bool IsAbleDelete() const { return particleData_.empty(); }


public: /// container operator
    void reserve(size_t _size, bool _isInit = false);
    void emplace_back(const ParticleData& _data);


private:
    /// Common
    std::string                             name_                               = {};

    /// GameEye
    GameEye*                                pGameEye_                           = nullptr;

    /// Instancing
    Microsoft::WRL::ComPtr<ID3D12Resource>  instancingResource_                 = nullptr;
    ParticleForGPU*                         instancingData_                     = nullptr;
    uint32_t                                currentInstancingSize_              = 0u;

    /// Model
    std::string                             modelPath_                          = {};
    Model*                                  pModel_                             = nullptr;
    ModelData*                              pModelData_                         = nullptr;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_                   = {};

    /// SRV
    uint32_t                                srvIndex_                           = 0u;
    D3D12_CPU_DESCRIPTOR_HANDLE             srvCpuHandle_                       = {};
    D3D12_GPU_DESCRIPTOR_HANDLE             srvGpuHandle_                       = {};
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSRVHandleGPU_                = {};

    /// Billboard
    Matrix4x4                               backToFrontMatrix_                  = {};
    Matrix4x4                               billboardMatrix_                    = {};
    bool                                    enableBillboard_                    = false;

    /// Parameter
    std::vector<ParticleData>               particleData_                       = {};


private: /// 他クラスのインスタンス
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* pDevice_ = nullptr;
    ParticleSystem* pSystem_ = nullptr;


private:
    void CreateParticleForGPUResource();
    void CreateSRV();
    void GetModelData();
    void InitializeTransform();
    void ParticleDataUpdate(std::vector<ParticleData>::iterator& _itr);
    void DebugWindow();
    float EaseOutCubic(float t);
    float EaseOutQuad(float t);

private: /// delete condition
    bool ParticleDeleteByCondition(std::vector<ParticleData>::iterator& _itr);
    bool DeleteByLifeTime(std::vector<ParticleData>::iterator& _itr);
    bool DeleteByZeroAlpha(std::vector<ParticleData>::iterator& _itr);

};