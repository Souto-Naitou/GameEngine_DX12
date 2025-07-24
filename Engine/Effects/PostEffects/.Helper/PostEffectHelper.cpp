#include "PostEffectHelper.h"
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Core/Win32/WinSystem.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/SRVManager.h>

void Helper::CreateRenderTexture(DirectX12* _pDx12, ID3D12Device* _pDevice, ResourceStateTracker& _resource, D3D12_CPU_DESCRIPTOR_HANDLE& _rtvHandle, uint32_t& _rtvHeapIndex)
{
    const auto kFormat = DirectX12::kRenderTargetFormat_;

    _resource.resource = DX12Helper::CreateRenderTextureResource(
        _pDevice,
        WinSystem::clientWidth,
        WinSystem::clientHeight,
        kFormat,
        _pDx12->GetEditorBGColor()
    );
    _resource.format = kFormat;
    _resource.state = D3D12_RESOURCE_STATE_RENDER_TARGET;

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = kFormat;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    if (_rtvHandle.ptr == 0)
    {
        _rtvHeapIndex = _pDx12->GetRTVHeapCounter()->Allocate("PostEffectRTV");
        _rtvHandle = _pDx12->GetRTVHeapCounter()->GetRTVHandle(_rtvHeapIndex);
    }

    _pDevice->CreateRenderTargetView(
        _resource.resource.Get(),
        &rtvDesc,
        _rtvHandle
    );
    _resource.resource->SetName(L"Unnamed_RenderTexture");
}

void Helper::CreateSRV(ResourceStateTracker& _resource, D3D12_GPU_DESCRIPTOR_HANDLE& _srvHandle, uint32_t& _srvHeapIndex)
{
    auto pSRVManager = SRVManager::GetInstance();

    _srvHeapIndex = pSRVManager->Allocate();

    pSRVManager->CreateForTexture2D(
        _srvHeapIndex,
        _resource.resource.Get(),
        DirectX12::kRenderTargetFormat_,
        1
    );

    _srvHandle = pSRVManager->GetGPUDescriptorHandle(_srvHeapIndex);
}

void Helper::CreateCommandList(ID3D12Device* _pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& _commandList,  Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& _Allocator)
{
    _pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_Allocator.GetAddressOf()));
    _pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _Allocator.Get(), nullptr, IID_PPV_ARGS(_commandList.GetAddressOf()));
}
