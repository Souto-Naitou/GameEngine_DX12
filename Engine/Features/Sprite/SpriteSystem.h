#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <list>

#include <ClassStyles/SingletonStyle.h>
#include <BaseClasses/ObjectSystemBase.h>

class SpriteSystem : 
    public SingletonStyle<SpriteSystem>,
    public ObjectSystemBase
{
    friend class SingletonStyle<SpriteSystem>;

public:
    struct CommandListData
    {
        ID3D12Resource* materialResource;
        ID3D12Resource* transformationMatrixResource;
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
        D3D12_VERTEX_BUFFER_VIEW* pVBV;
        D3D12_INDEX_BUFFER_VIEW* pIBV;
    };

    // Common functions
    void Initialize() override;
    void PresentDraw();
    void DrawCall();
    void Sync();

    // Setter
    void AddCommandListData(const CommandListData& _data);

private:
    void CreateRootSignature();
    void CreatePipelineState();

    // DirectX objects and paths
    static constexpr wchar_t                    kVertexShaderPath[]     = L"EngineResources/Shaders/Sprite.VS.hlsl";
    static constexpr wchar_t                    kPixelShaderPath[]      = L"EngineResources/Shaders/Sprite.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_          = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_  = nullptr;
    std::list<CommandListData>                  commandListDatas_       = {};
};