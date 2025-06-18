#pragma once

#include <Core/DirectX12/IPostEffect.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxcapi.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/ResourceStateTracker/ResourceStateTracker.h>
#include <Vector4.h>
#include <Vector3.h>
#include <Vector2.h>

/// <ビネット>
/// - ApplyメソッドとSettingメソッドはPostEffectクラスで実行する
class RadialBlur : public IPostEffect
{
public:
    struct alignas(16) RadialBlurOption
    {
        Vector2 center = Vector2(0.5f, 0.5f); // 中心位置 (0.0f ~ 1.0f)
        int samples = 16; // サンプル数
        float blurWidth = 0.01f; // 幅 (0.0f ~ 1.0f)
    };

public:
    void    Initialize() override;
    void    Release() override;

    void    Enable(bool _flag) override;
    bool    Enabled() const override;

    // Setter (Additional)
    void    SetCenter(float _center) { pOption_->center = _center; }
    void    SetSamples(int _sample) { pOption_->samples = _sample; }
    void    SetBlurWidth(float _width) { pOption_->blurWidth = _width; }

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

    bool                                                isEnabled_              = false;
    const std::string                                   name_                   = "RadialBlur";
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
    const std::wstring                                  kVertexShaderPath       = L"EngineResources/Shaders/RadialBlur.VS.hlsl";
    const std::wstring                                  kPixelShaderPath        = L"EngineResources/Shaders/RadialBlur.PS.hlsl";

    // Constant buffer view
    Microsoft::WRL::ComPtr<ID3D12Resource>              optionResource_     = nullptr;
    RadialBlurOption*                                   pOption_            = nullptr;


    // Internal functions
    void    CreateRootSignature();
    void    CreatePipelineStateObject();
    void    ToRenderTargetState();
    void    CreateResourceCBuffer();
};