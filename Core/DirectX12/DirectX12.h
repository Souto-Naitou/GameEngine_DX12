#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

#include <wrl.h>
#include <Windows.h>
#include <stdint.h>
#include <string>
#include <memory>

#include "ReakChecker.h"

class DirectX12
{
public:
    DirectX12(const DirectX12&) = delete;
    DirectX12(const DirectX12&&) = delete;
    DirectX12& operator=(const DirectX12&) = delete;
    DirectX12& operator=(const DirectX12&&) = delete;

    static DirectX12* GetInstance()
    {
        static D3DResourceLeakChecker leakchecker;
        static DirectX12 instance; return &instance;
    }

    void Initialize();

private:
    DirectX12() = default;
    ~DirectX12() = default;

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
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        rtvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        srvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        dsvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        descriptorHeaps_                = nullptr;
    D3D12_RESOURCE_BARRIER                              barrier_                        = {};
    D3D12_COMMAND_QUEUE_DESC                            commandQueueDesc_               = {};           // コマンドキューの設定
    DXGI_SWAP_CHAIN_DESC1                               swapChainDesc_                  = {};           // スワップチェーンの設定
    D3D12_RENDER_TARGET_VIEW_DESC                       rtvDesc_                        = {};           // RTVの設定
    HANDLE                                              fenceEvent_                     = {};
    uint64_t                                            fenceValue_                     = 0u;            // フェンス値

    uint32_t clientWidth_ = 1280u;
    uint32_t clientHeight_ = 720u;

    uint32_t kDescriptorSizeSRV;
    uint32_t kDescriptorSizeRTV;
    uint32_t kDescriptorSizeDSV;


private:

    /// <summary>
    /// アダプタの選択
    /// </summary>
    void ChooseAdapter();


    /// <summary>
    /// Command[List/Queue/Allocator]を生成する
    /// </summary>
    void CreateCommandResources();


    /// <summary>
    /// スワップチェーンの生成
    /// </summary>
    void CreateSwapChainAndResource();


    /// <summary>
    /// フェンスの生成とイベントの生成
    /// </summary>
    void CreateFenceAndEvent();
};