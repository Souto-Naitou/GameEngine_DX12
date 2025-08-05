#pragma once

#include "DirectX12.h"
#include "SRVManager.h"

#include <wrl/client.h>
#include "ResourceStateTracker/ResourceStateTracker.h"
#include "IPostEffect.h"
#include <vector>
#include <functional>

/// <ポストエフェクトを実行するクラス>
/// - 複数のポストエフェクトを順に適用するためにレンダーテクスチャのチェインを生成する
/// - Bloom -> MotionBlur の順で実行する場合 MotionBlurにBloomのレンダーテクスチャを渡す

class PostEffectExecuter : public EngineFeature
{
public:
    PostEffectExecuter(const PostEffectExecuter&) = delete;
    PostEffectExecuter& operator=(const PostEffectExecuter&) = delete;
    PostEffectExecuter(PostEffectExecuter&&) = delete;
    PostEffectExecuter& operator=(PostEffectExecuter&&) = delete;

    static PostEffectExecuter* GetInstance()
    {
        static PostEffectExecuter instance;
        return &instance;
    }

    void Initialize();
    void Finalize();
    void ApplyPostEffects();
    void NewFrame();
    void PostDraw();
    void Draw();
    void OnResize();
    void OnResizedBuffers();
    void DebugWindow();


public:
    PostEffectExecuter& RegisterPostEffect(IPostEffect* postEffect)
    {
        postEffects_.emplace_back(postEffect);
        return *this;
    }

    PostEffectExecuter& UnregisterPostEffect(IPostEffect* postEffect)
    {
        auto it = std::remove(postEffects_.begin(), postEffects_.end(), postEffect);
        if (it != postEffects_.end())
        {
            postEffects_.erase(it, postEffects_.end());
        }
        return *this;
    }

    ID3D12Resource* GetRenderTexture() const
    {
        return renderTexture_.resource.Get();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE* GetRTVHandle()
    {
        return &rtvHandle_;
    }

    ID3D12GraphicsCommandList* GetCommandList()
    {
        return commandListForDraw_.Get();
    }

    D3D12_GPU_DESCRIPTOR_HANDLE GetRTVHandleGpu() const
    {
        return rtvHandleGpu_;
    }


private:
    PostEffectExecuter() = default;
    ~PostEffectExecuter() = default;

    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Fullscreen.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Fullscreen.PS.hlsl";
    ResourceStateTracker                                renderTexture_          = {};
    uint32_t                                            srvHeapIndex_           = 0;
    uint32_t                                            rtvHeapIndex_           = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE                         rtvHandle_              = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         rtvHandleGpu_           = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         outputHandleGpu_        = {};
    D3D12_INPUT_LAYOUT_DESC                             inputLayoutDesc_        = {};
    D3D12_GRAPHICS_PIPELINE_STATE_DESC                  pipelineStateDesc_      = {};
    D3D12_RASTERIZER_DESC                               rasterizerDesc_         = {};
    Microsoft::WRL::ComPtr<ID3D12RootSignature>         rootSignature_          = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>                    vertexShaderBlob_       = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>                    pixelShaderBlob_        = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>         pso_                    = nullptr;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   commandListForDraw_     = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      commandAllocator_       = nullptr;

    std::vector<IPostEffect*>                           postEffects_           = {};


private:
    void ObtainInstances();
    void CreateRootSignature();
    void CreatePipelineState();
    void CreateCommandList();
    void EnableSolo(const size_t _index);
    void ImGuiCenterTable(const std::function<void()>& _fn);


private:
    RTVHeapCounter*                         rtvHeapCounter_     = nullptr;
    ID3D12Device*                           pDevice_            = nullptr;
    SRVManager*                             pSRVManager_        = nullptr;
    ID3D12GraphicsCommandList*              commandListMain_    = nullptr;
    ID3D12DescriptorHeap*                   dsvHeap_            = {};
    ID3D12DescriptorHeap*                   rtvHeap_            = {};
    D3D12_CPU_DESCRIPTOR_HANDLE             rtvHandleSwapChain_ = {};
    Vector4                                 editorBG_           = {};
};