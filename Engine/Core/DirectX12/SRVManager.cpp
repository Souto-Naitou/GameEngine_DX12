#include "SRVManager.h"
#include <Core/DirectX12/Helper/DX12Helper.h>

const uint32_t SRVManager::kMaxSRVCount_ = 512u;

void SRVManager::Initialize(DirectX12* _pDx12)
{
    pDx12_ = _pDx12;
    pDescHeap_ = DX12Helper::CreateDescriptorHeap(
        pDx12_->GetDevice(),
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        kMaxSRVCount_,
        D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
    );
    descriptorSize_ = pDx12_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SRVManager::PresentDraw()
{
    ID3D12DescriptorHeap* descriptorHeaps[] = { pDescHeap_.Get() };
    pDx12_->GetCommandList()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

uint32_t SRVManager::Allocate()
{
    assert(currentIndex_ < kMaxSRVCount_ && "インデックスが最大値を超えています");

    uint32_t index = currentIndex_;
    currentIndex_++;
    return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t _index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle = pDescHeap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += (descriptorSize_ * _index);
    return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t _index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle = pDescHeap_->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += (descriptorSize_ * _index);
    return handle;
}

void SRVManager::SetGraphicsRootDescriptorTable(UINT _rootParameterIndex, uint32_t _srvIndex)
{
    pDx12_->GetCommandList()->SetGraphicsRootDescriptorTable(_rootParameterIndex, GetGPUDescriptorHandle(_srvIndex));
}

void SRVManager::CreateForTexture2D(uint32_t _index, ID3D12Resource* _pTexture, DXGI_FORMAT _format, UINT _mipLevels)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = _format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = _mipLevels;

    pDx12_->GetDevice()->CreateShaderResourceView(_pTexture, &srvDesc, GetCPUDescriptorHandle(_index));
}

void SRVManager::CreateForStructuredBuffer(uint32_t _index, ID3D12Resource* _pBuffer, UINT _numElements, UINT _stride)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    srvDesc.Buffer.NumElements = _numElements;
    srvDesc.Buffer.StructureByteStride = _stride;

    pDx12_->GetDevice()->CreateShaderResourceView(_pBuffer, &srvDesc, GetCPUDescriptorHandle(_index));
}