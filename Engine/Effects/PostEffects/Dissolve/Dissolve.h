#pragma once

#include <Core/DirectX12/IPostEffect.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxcapi.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/ResourceStateTracker/ResourceStateTracker.h>
#include <string>
#include <Core/DirectX12/TextureResource/TextureResource.h>
#include <Vector4.h>

struct alignas(16) DissolveOption
{
    float threshold;
    float edgeThresholdOffset;
    float padding[2];
    Vector4 colorDissolve;
    Vector4 colorEdge;
};

/// <ボックスフィルタ>
/// - ApplyメソッドとSettingメソッドはPostEffectクラスで実行する
class Dissolve :
    public IPostEffect,
    public EngineFeature
{
public:
    void    Initialize() override;
    void    Finalize() override;

    void    Enable(bool _flag) override;
    bool    Enabled() const override;

    void    SetTextureResource(const TextureResource& _texResource);

private:
    // PostEffectクラスがアクセスする
    void    Apply() override;
    void    Setting() override;
    void    OnResizeBefore() override;
    void    OnResizedBuffers() override;
    void    ToShaderResourceState() override;
    void    DebugOverlay() override;

    // Setters
    void    SetInputTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle) override;

    // Getters
    D3D12_GPU_DESCRIPTOR_HANDLE     GetOutputTextureHandle() const override;
    const std::string&              GetName() const override;

private:
    ID3D12Device*                                       device_                 = nullptr;
    ID3D12GraphicsCommandList*                          commandList_            = nullptr;

    bool                                                isEnabled_              = false;
    const std::string                                   name_                   = "Dissolve";
    ResourceStateTracker                                renderTexture_          = {};
    Microsoft::WRL::ComPtr<IDxcBlob>                    vertexShaderBlob_       = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>                    pixelShaderBlob_        = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>         pso_                    = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature>         rootSignature_          = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE                         rtvHandleCpu_           = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         rtvHandleGpu_           = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         inputGpuHandle_         = {};
    uint32_t                                            rtvHeapIndex_           = 0;
    uint32_t                                            srvHeapIndex_           = 0;
    const std::wstring                                  kVertexShaderPath       = L"EngineResources/Shaders/Dissolve.VS.hlsl";
    const std::wstring                                  kPixelShaderPath        = L"EngineResources/Shaders/Dissolve.PS.hlsl";
    TextureResource                                     maskTexture_            = {};

    // Constant buffers
    Microsoft::WRL::ComPtr<ID3D12Resource>              optionResource_         = nullptr;
    DissolveOption*                                     pOption_                = nullptr;

    // Internal functions
    void    CreateRootSignature();
    void    CreatePipelineStateObject();
    void    ToRenderTargetState();
    void    CreateResourceCBuffer();
    void    CheckValidation() const;
};