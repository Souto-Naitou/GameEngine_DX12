#pragma once

#include <Common/HRESULT_ASSERT.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/SRVManager.h>
#include <d3d12.h>
#include <wrl.h>
#ifdef _DEBUG
#include <imgui.h>
#endif //_DEBUG

class Viewport : public EngineFeature
{
public:
    Viewport() = default;
    ~Viewport() = default;


    void Initialize();
    void Compute();
    void OnResizedBuffers();
    void DrawWindow();


public: /// Getter
    uint32_t GetOutputSRVIndex() const { return outputSRVIndex_; }
    Vector2 GetViewportPos() const { return vpPos_; }
    Vector2 GetViewportSize() const { return vpSize_; }


private:
    void CreateRootSignature();
    void CreatePSO();
    void CreateSRV();
    void CreateUAV();


private:
    HRESULT_ASSERT hr_ = {};

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> csBlob_ = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;
    D3D12_VIEWPORT viewport_ = {};

    uint32_t inputSRVIndex_ = 0u;
    uint32_t outputSRVIndex_ = 0u;
    uint32_t outputUAVIndex_ = 0u;
    
    static constexpr wchar_t kComputeShaderPath_[] = L"EngineResources/Shaders/WithoutAlpha.CS.hlsl";

    Vector2 windowPos_ = { 0, 0 };
    Vector2 vpPos_ = { 0, 0 };
    Vector2 vpSize_ = { 0, 0 };
    #ifdef _DEBUG
    ImVec2  nextContentRegionSize_ = {};
    #endif 

    // DirectX 12 objects
    ID3D12Device*               device_         = nullptr;
    IDxcUtils*                  dxcUtils_       = nullptr;
    IDxcCompiler3*              dxcCompiler_    = nullptr;
    IDxcIncludeHandler*         includeHandler_ = nullptr;
    ID3D12GraphicsCommandList*  commandList_    = nullptr;
    ResourceStateTracker*       inputTexture_   = nullptr;
    ResourceStateTracker*       outputTexture_  = nullptr;
    SRVManager*                 pSRVManager_    = nullptr;
};