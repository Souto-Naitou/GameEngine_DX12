#include "DirectX12.h"

#include <Utility/ConvertString/ConvertString.h>        // wideとの互換
#include <Core/Win32/WinSystem.h>                       // Window関連
#include <Core/DirectX12/Helper/DX12Helper.h>           // ヘルパー
#include <Core/DirectX12/SRVManager.h>                  // SRV管理

#ifdef _DEBUG
#include <imgui_impl_dx12.h>
#endif // DEBUG


#include <cassert>
#include <format>
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


/// <summary>
/// 初期化呼出
/// </summary>


void DirectX12::Initialize()
{
    /// デバッグコントローラの設定
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_))))
    {
        // デバッグレイヤーを有効化する
        debugController_->EnableDebugLayer();
        // GPU側もチェックする
        debugController_->SetEnableGPUBasedValidation(TRUE);
    }

    pSRVManager_ = SRVManager::GetInstance();

    pFramerate_ = FrameRate::GetInstance();
    pFramerate_->Initialize();

    pLogger_ = Logger::GetInstance();

    // ウィンドウハンドルを取得
    hwnd_ = WinSystem::GetInstance()->GetHwnd();

    hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

    if (FAILED(hr_))
    {
        pLogger_->LogError(
            "DirectX12",
            __func__,
            "Failed to create DXGI Factory."
        );

        return;
    }


    /// アダプタ
    ChooseAdapter();


    /// デバイス生成
    DX12Helper::CreateDevice(device_, useAdapter_);


    /// エラー時停止処理
    //#ifdef _DEBUG
    DX12Helper::PauseError(device_, infoQueue_);
    //#endif // _DEBUG


    /// 出力ウィンドウに初期化完了を出力
    pLogger_->LogInfo(
        "DirectX12",
        __func__,
        "DirectX12 Initialized."
    );


    /// コマンド系を生成
    CreateCommandResources();


    /// スワップチェーンの生成とRTVのリソース生成
    CreateSwapChainAndResource();


    /// DSVの生成とステートの設定
    CreateDSVAndSettingState();


    /// フェンスとイベントの生成
    CreateFenceAndEvent();


    /// ビューポートとシザー矩形の設定
    SetViewportAndScissorRect();


    /// DXCの初期化
    CreateDirectXShaderCompiler();


    /// D3D11デバイス群の生成
    CreateD3D11Device();


    /// Direct2Dのファクトリの生成
    CreateD2D1Factory();


    /// D2D1デバイスコンテキストの生成
    CreateID2D1DeviceContext();


    /// D2D1レンダーターゲットの生成
    CreateD2DRenderTarget();
}

void DirectX12::OnResized()
{
    this->ResizeBuffers();
    isResized_ = false;
}

void DirectX12::NewFrame()
{
    // リソースバリアの設定
    DX12Helper::ChangeStateResource(commandList_, swapChainResources_[backBufferIndex_], D3D12_RESOURCE_STATE_RENDER_TARGET);

    /// 描画先のRTV/DSVの設定
    commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, nullptr);

    // 画面全体のクリア
    commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex_], &editorBG_.x, 0, nullptr);

    // 指定した深度で画面全体をクリア (ポストエフェクト用リソースで行うため現在無効)
    // commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    commandList_->RSSetViewports(1, &viewport_);            // Viewportを設定
    commandList_->RSSetScissorRects(1, &scissorRect_);      // Scissorを設定
}

void DirectX12::CommandExecute()
{
    ID3D12GraphicsCommandList* commandList_end = commandList_.Get();
    if (!commandLists_.empty())
    {
        commandList_end = commandLists_.back();
    }
    DX12Helper::ChangeStateResource(
        commandList_end,
        swapChainResources_[backBufferIndex_],
        D3D12_RESOURCE_STATE_PRESENT
    );

    /// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseする
    hr_ = commandList_->Close();
    if (FAILED(hr_))
    {
        pLogger_->LogError(
            "DirectX12",
            __func__,
            "Failed to close command list."
        );
        assert(false && "Failed to close command list");
    }


    /// GPUにコマンドリストの実行を行わせる
    std::vector<ID3D12CommandList*> commandLists = { commandList_.Get()};
    for(auto& cl : commandLists_)
    {
        cl->Close();
        commandLists.push_back(cl);
    }
    commandQueue_->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
}

void DirectX12::DisplayFrame()
{
    /// 描く状態から画面に映す状態に遷移
    /// (Direct2dで行われる)

    /// GPUとISに画面の交換を行うよう通知する
    swapChain_->Present(1, 0);

    // バックバッファのインデックスを取得
    backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();

    /// Fenceの値を更新
    fenceValue_++;


    /// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
    commandQueue_->Signal(fence_.Get(), fenceValue_);

    /// GetCompletedValueの初期値はFence作成時に渡した初期値
    if (fence_->GetCompletedValue() < fenceValue_)
    {
        // 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
        fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
        // イベントを待つ
        WaitForSingleObject(fenceEvent_, INFINITE);
    }


    /// フレームレート固定
    pFramerate_->FixFramerate();


    /// 次のフレーム用のコマンドリストを準備
    hr_ = commandAllocator_->Reset();
    assert(SUCCEEDED(hr_));
    hr_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
    assert(SUCCEEDED(hr_));
}

void DirectX12::CopyFromRTV(ID3D12GraphicsCommandList* _commandList)
{
    /// レンダーターゲットからコピー元状態にする
    DX12Helper::ChangeStateResource(_commandList, swapChainResources_[backBufferIndex_], D3D12_RESOURCE_STATE_COPY_SOURCE);

    DX12Helper::ChangeStateResource(_commandList, gameScreenResource_, D3D12_RESOURCE_STATE_COPY_DEST);


    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = swapChainResources_[backBufferIndex_].resource.Get();
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    srcLocation.SubresourceIndex = 0; // コピーするサブリソースインデックス

    D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
    dstLocation.pResource = gameScreenResource_.resource.Get();
    dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLocation.SubresourceIndex = 0;

    /// コピー範囲
    D3D12_BOX srcBox = {};
    srcBox.left = static_cast<UINT>(viewport_.TopLeftX);
    srcBox.top = static_cast<UINT>(viewport_.TopLeftY);
    srcBox.right = static_cast<UINT>(viewport_.Width + viewport_.TopLeftX);
    srcBox.bottom = static_cast<UINT>(viewport_.Height + viewport_.TopLeftY);
    srcBox.front = 0;
    srcBox.back = 1;

    _commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, &srcBox);

    /// バリアを戻す
    DX12Helper::ChangeStateResource(_commandList, swapChainResources_[backBufferIndex_], D3D12_RESOURCE_STATE_RENDER_TARGET);

    DX12Helper::ChangeStateResource(
        _commandList, 
        gameScreenResource_, 
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );

    #ifdef _DEBUG

    /// rtvのクリア
    _commandList->ClearRenderTargetView(rtvHandles_[backBufferIndex_], &editorBG_.x, 0, nullptr);

    #endif // _DEBUG
}

DirectX12::~DirectX12()
{
    const UINT64 fenceSignalValue = ++fenceValue_;
    ID3D12Fence* fence = fence_.Get();

    commandQueue_->Signal(fence, fenceSignalValue);
    if (fence->GetCompletedValue() < fenceSignalValue)
    {
        fence->SetEventOnCompletion(fenceSignalValue, fenceEvent_);
        WaitForSingleObject(fenceEvent_, INFINITE);
    }

    pSRVManager_->Deallocate(gameWndSrvIndex_);
}

void EngineFeature::SetDirectX12(DirectX12* _pDx12)
{
    pDx12_ = _pDx12;
}

DirectX12* EngineFeature::GetDirectX12()
{
    return pDx12_;
}
