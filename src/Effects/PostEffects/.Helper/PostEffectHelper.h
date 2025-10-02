#pragma once

#include <Core/DirectX12/ResourceStateTracker/ResourceStateTracker.h>
#include <d3d12.h>
#include <cstdint>

class DirectX12;

namespace Helper
{
    void CreateRenderTexture(
        DirectX12*                      _pDx12,
        ID3D12Device*                   _pDevice,
        ResourceStateTracker&           _resource,
        D3D12_CPU_DESCRIPTOR_HANDLE&    _rtvHandle,
        uint32_t&                       _rtvHeapIndex
    );

    void CreateSRV(
        ResourceStateTracker&           _resource,
        D3D12_GPU_DESCRIPTOR_HANDLE&    _srvHandle,
        uint32_t&                       _srvHeapIndex
    );

    void CreateCommandList(
        ID3D12Device* _pDevice,
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& _commandList,
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& _Allocator
    );
}