#pragma once
#include <wrl/client.h>
#include <d3d12.h>

struct ResourceStateTracker
{
    Microsoft::WRL::ComPtr<ID3D12Resource> resource;
    D3D12_RESOURCE_STATES state;
    DXGI_FORMAT format;

    void Reset();
    void ChangeState(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState);
};