#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <Core/DirectX12/DirectX12.h>
#include <Features/GameEye/GameEye.h>

class LineSystem : public EngineFeature
{
public:
    LineSystem(const LineSystem&) = delete;
    LineSystem& operator=(const LineSystem&) = delete;
    LineSystem(LineSystem&&) = delete;
    LineSystem& operator=(LineSystem&&) = delete;

    static LineSystem* GetInstance()
    {
        static LineSystem instance;
        return &instance;
    }

    void Initialize();
    void PresentDraw();


private:
    LineSystem() = default;
    ~LineSystem() = default;


public: /// Getter
    GameEye** GetSharedGameEye() { return &pDefaultGameEye_; }


public: /// Setter
    void SetGlobalEye(GameEye* _pGameEye) { pDefaultGameEye_ = _pGameEye; }


private: /// メンバ変数
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Line.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Line.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;
    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;

    D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1] = {};
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};
    D3D12_BLEND_DESC blendDesc_ = {};
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc_ = {};

private: /// 処理郡
    void CreateRootSignature();
    void CreatePipelineState();
    void SetInputLayout();
    void SetBlendDesc();
    void SetDSVDesc();

private: /// 他クラスのインスタンス
    GameEye* pDefaultGameEye_ = nullptr;
    ID3D12Device* device_ = nullptr;
    IDxcUtils* dxcUtils_ = nullptr;
    IDxcCompiler3* dxcCompiler_ = nullptr;
    IDxcIncludeHandler* includeHandler_ = nullptr;
};