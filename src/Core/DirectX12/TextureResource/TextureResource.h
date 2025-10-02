#pragma once

#include <wrl/client.h>
#include <d3d12.h>
#include <cstdint>
#include <string>

class TextureResource
{
public:
    TextureResource() = default;
    ~TextureResource() = default;

    // Setters
    void SetResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& _resource);
    void SetSRV(uint32_t _srvIndex, D3D12_CPU_DESCRIPTOR_HANDLE _srvHandleCPU, D3D12_GPU_DESCRIPTOR_HANDLE _srvHandleGPU);
    void SetResourceName(const std::string& _name);

    // Getters
    [[nodiscard]] ID3D12Resource*               GetResource() const;
    [[nodiscard]] uint32_t                      GetSRVIndex() const;
    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE   GetSRVHandleCPU() const;
    [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE   GetSRVHandleGPU() const;

private:
    // Resource
    Microsoft::WRL::ComPtr<ID3D12Resource>  resource_ = nullptr;

    // SRV
    uint32_t                                srvIndex_       = 0u;
    D3D12_CPU_DESCRIPTOR_HANDLE             srvHandleCPU_   = {};
    D3D12_GPU_DESCRIPTOR_HANDLE             srvHandleGPU_   = {};
};