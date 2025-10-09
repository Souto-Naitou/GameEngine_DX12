#pragma once
#include <d3d12.h>
#include <string>
#include <memory>


class IPostEffect
{
public:
    IPostEffect() = default;
    virtual ~IPostEffect() = default;

    // Initialize the post effect
    // Please use the "Helper::PostEffect::CreatePostEffect" function to create an instance.
    virtual void    Initialize() = 0;
    // Release resources
    virtual void    Finalize() = 0;

    virtual void    Enable(bool _flag) = 0;
    virtual bool    Enabled() const = 0;

    // Setter
    virtual void    SetInputTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle) = 0;
    // Getter
    virtual D3D12_GPU_DESCRIPTOR_HANDLE GetOutputTextureHandle() const = 0;
    virtual const std::string& GetName() const = 0;

    // Draw setting
    virtual void    Setting() = 0;
    // Apply the post effect
    virtual void    Apply() = 0;
    // Change the state of the render texture to shader resource state
    virtual void    ToShaderResourceState() = 0;
    // On resize event
    virtual void    OnResizeBefore() = 0;
    virtual void    OnResizedBuffers() = 0;
    virtual void    DebugOverlay() = 0;
};

class DirectX12;
namespace Helper::PostEffect
{
    template <typename T>
    [[nodiscard]]
    inline std::unique_ptr<T> CreatePostEffect(DirectX12* _pDx12)
    {
        auto pEffect = std::make_unique<T>();
        pEffect->SetDirectX12(_pDx12);
        pEffect->Initialize();
        return pEffect;
    }
}

