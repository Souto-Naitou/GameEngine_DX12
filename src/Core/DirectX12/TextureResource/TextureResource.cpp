#include "TextureResource.h"

#include <utility>

void TextureResource::SetResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& _resource)
{
    resource_ = std::move(_resource);
}

void TextureResource::SetSRV(uint32_t _srvIndex, D3D12_CPU_DESCRIPTOR_HANDLE _srvHandleCPU, D3D12_GPU_DESCRIPTOR_HANDLE _srvHandleGPU)
{
    srvIndex_     = _srvIndex;
    srvHandleCPU_ = _srvHandleCPU;
    srvHandleGPU_ = _srvHandleGPU;
}

void TextureResource::SetResourceName(const std::string& _name)
{
    if (resource_)
    {
        resource_->SetName(std::wstring(_name.begin(), _name.end()).c_str());
    }
}

ID3D12Resource* TextureResource::GetResource() const
{
    return resource_.Get();
}

uint32_t TextureResource::GetSRVIndex() const
{
    return srvIndex_;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureResource::GetSRVHandleCPU() const
{
    return srvHandleCPU_;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureResource::GetSRVHandleGPU() const
{
    return srvHandleGPU_;
}
