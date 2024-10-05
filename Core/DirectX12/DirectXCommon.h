#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

#include <wrl.h>
#include <Windows.h>
#include <stdint.h>
#include <string>
#include <memory>

#include <PipelineState.h>

class DirectXCommon
{
public:
    DirectXCommon(const DirectXCommon&) = delete;
    DirectXCommon(const DirectXCommon&&) = delete;
    DirectXCommon& operator=(const DirectXCommon&) = delete;
    DirectXCommon& operator=(const DirectXCommon&&) = delete;

    static DirectXCommon* GetInstance() { static DirectXCommon instance; return &instance;}

    /// <summary>
    /// 初期化
    /// </summary>
    /// <returns>成否</returns>
    int Initialize();
    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();

private:
    DirectXCommon() = default;
    ~DirectXCommon() = default;

    uint32_t clientWidth_ = 1280u;
    uint32_t clientHeight_ = 720u;

    HRESULT hr_ = 0;
    HWND hwnd_ = {};

    Microsoft::WRL::ComPtr<ID3D12Debug1>                debugController_                = nullptr;      // デバッグコントローラ
    Microsoft::WRL::ComPtr<IDXGIFactory7>               dxgiFactory_                    = nullptr;      // DXGIファクトリ
    Microsoft::WRL::ComPtr<IDXGIAdapter4>               useAdapter_                     = nullptr;      // 使うアダプタ
    Microsoft::WRL::ComPtr<ID3D12Device>                device_                         = nullptr;      // デバイス
    Microsoft::WRL::ComPtr<ID3D12InfoQueue>             infoQueue_                      = nullptr;      
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          commandQueue_                   = nullptr;      // コマンドキュー
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      commandAllocator_               = nullptr;      // コマンドアロケータ
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   commandList_                    = nullptr;      // コマンドアロケータ
    Microsoft::WRL::ComPtr<IDXGISwapChain4>             swapChain_                      = nullptr;      // スワップチェーン
    Microsoft::WRL::ComPtr<ID3D12Resource>              swapChainResources[2]           = {};           // スワップチェーンリソース
    Microsoft::WRL::ComPtr<ID3D12Fence>                 fence_                          = nullptr;      // フェンス
    Microsoft::WRL::ComPtr<ID3D12RootSignature>         rootSignature_                  = nullptr;      // ルート署名
    Microsoft::WRL::ComPtr<ID3D12Resource>              depthStencilResource_           = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        rtvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        srvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob>                    signatureBlob_                  = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob>                    errorBlob_                      = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        dsvDescriptorHeap_              = nullptr;
    D3D12_ROOT_PARAMETER                                rootParameters_[4]              = {};
    D3D12_COMMAND_QUEUE_DESC                            commandQueueDesc_               = {};           // コマンドキューの設定
    DXGI_SWAP_CHAIN_DESC1                               swapChainDesc_                  = {};           // スワップチェーンの設定
    D3D12_RENDER_TARGET_VIEW_DESC                       rtvDesc_                        = {};           // RTVの設定
    D3D12_DEPTH_STENCIL_VIEW_DESC                       dsvDesc_                        = {};
    D3D12_DEPTH_STENCIL_DESC                            depthStencilDesc_               = {};
    HANDLE                                              fenceEvent_                     = {};
    uint64_t                                            fenceValue_                     = 0u;            // フェンス値
    std::unique_ptr<PipelineState::Configurator>        pPipelineConfig_                = nullptr;

private: /// ======= プライベート関数
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
        const Microsoft::WRL::ComPtr<ID3D12Device>& _device,
        D3D12_DESCRIPTOR_HEAP_TYPE _heapType,
        UINT _numDescriptors,
        bool _shaderVisible
    );

    Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(
        const Microsoft::WRL::ComPtr<ID3D12Device>& _device,
        int32_t _width,
        int32_t _height
    );
};