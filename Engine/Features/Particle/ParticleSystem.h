#pragma once

#include <wrl.h>
#include <Core/DirectX12/DirectX12.h>
#include <Features/GameEye/GameEye.h>
#include <BaseClasses/ObjectSystemBase.h>
#include <list>

class ParticleSystem : public ObjectSystemBase
{
public:
    struct CommandListData
    {
        D3D12_VERTEX_BUFFER_VIEW* pVBV = nullptr;
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = {};
        D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandle = {};
        UINT vertexCount = 0;
        UINT instanceCount = 0;
    };

public:
    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;
    ParticleSystem(const ParticleSystem&&) = delete;
    ParticleSystem& operator=(const ParticleSystem&&) = delete;

    static ParticleSystem* GetInstance()
    {
        static ParticleSystem instance;
        return &instance;
    }

    void Initialize();
    void PresentDraw();

    void DrawCall() override;
    void Sync() override;


public: /// Setter
    void SetDefaultGameEye(GameEye* _pGameEye) { pDefaultGameEye_ = _pGameEye; }
    void AddCommandListData(CommandListData& _pData) { commandListDatas_.emplace_back(_pData); }
    void SetRTVHandle(D3D12_CPU_DESCRIPTOR_HANDLE* _rtvHandle) { rtvHandle_ = _rtvHandle; }


public: /// Getter
    GameEye* GetDefaultGameEye() const { return pDefaultGameEye_; }


public: /// 公開定数
    static constexpr uint32_t kMaxInstance_ = 256u;


private: /// メンバ変数
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Particle.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Particle.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
    GameEye* pDefaultGameEye_ = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle_ = nullptr;

    std::list<CommandListData> commandListDatas_;


private: /// 非公開関数
    ParticleSystem() = default;
    void CreateRootSignature();
    void CreatePipelineState();


private:
    DirectX12* pDx12_ = nullptr;
};