#pragma once

#include <Core/DirectX12/IPostEffect.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxcapi.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/ResourceStateTracker/ResourceStateTracker.h>
#include <Vector4.h>
#include <Vector3.h>

/// <ビネット>
/// - PEはPost Effectの略
/// - ApplyメソッドとSettingメソッドはPostEffectクラスで実行する
class PEVignette : public IPostEffect
{
public:
    struct VignetteOption
    {
        Vector3 color;
        float scale;
        float power;
        int enableMultiply;
        Vector3 padding;
    };

public:
    void    Initialize() override;
    void    Release() override;

    void    Enable(bool _flag) override;
    bool    Enabled() const override;

    // Setter (Additional)
    void    SetScale(float _scale) { pOption_->scale = _scale; }
    void    SetPower(float _power) { pOption_->power = _power; }

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
    DirectX12*                                          pDx12_                  = nullptr;

    bool                                                isEnabled_              = true;
    const std::string                                   name_                   = "Vignette";
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
    const std::wstring                                  kVertexShaderPath       = L"EngineResources/Shaders/Vignette.VS.hlsl";
    const std::wstring                                  kPixelShaderPath        = L"EngineResources/Shaders/Vignette.PS.hlsl";

    // Constant buffer view
    Microsoft::WRL::ComPtr<ID3D12Resource>      optionResource_     = nullptr;
    VignetteOption*                             pOption_            = nullptr;


    // Internal functions
    void    CreateRootSignature();
    void    CreatePipelineStateObject();
    void    ToRenderTargetState();
    void    CreateResourceCBuffer();
};