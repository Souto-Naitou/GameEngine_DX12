#include "SeparatedGaussianFilter.h"
#include <cassert>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/PostEffect.h>
#include <Effects/PostEffects/.Helper/PostEffectHelper.h>
#include <Core/DirectX12/SRVManager.h>
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Core/DirectX12/RootParameters/RootParameters.h>
#include <Core/DirectX12/StaticSamplerDesc/StaticSamplerDesc.h>
#include <imgui.h>
#include <Math/Functions.hpp>

void SeparatedGaussianFilter::Initialize()
{
    device_ = pDx12_->GetDevice();
    commandList_ = PostEffectExecuter::GetInstance()->GetCommandList();

    // レンダーテクスチャの生成
    Helper::CreateRenderTexture(pDx12_, device_, horizontalGaussTexture_, horizontalHandleCpu_, horizontalHeapIndex_);
    horizontalGaussTexture_.resource->SetName(L"SeparatedGaussianFilterHorizontalGaussTexture");
    Helper::CreateRenderTexture(pDx12_, device_, renderTexture_, rtvHandleCpu_, rtvHeapIndex_);
    renderTexture_.resource->SetName(L"SeparatedGaussianFilterRenderTexture");

    // レンダーテクスチャのSRVを生成
    Helper::CreateSRV(horizontalGaussTexture_, horizontalHandleGpu_, horizontalSrvHeapIndex_);
    Helper::CreateSRV(renderTexture_, rtvHandleGpu_, srvHeapIndex_);

    // ルートシグネチャの生成
    this->CreateRootSignature();

    // パイプラインステートの生成
    this->CreatePipelineStateObject();

    // 設定用リソースの生成と初期化
    this->CreateResourceCBuffer();
}

void SeparatedGaussianFilter::Enable(bool _flag)
{
    isEnabled_ = _flag;
}

void SeparatedGaussianFilter::SetInputTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle)
{
    inputGpuHandle_ = _gpuHandle;
}

bool SeparatedGaussianFilter::Enabled() const
{
    return isEnabled_;
}

D3D12_GPU_DESCRIPTOR_HANDLE SeparatedGaussianFilter::GetOutputTextureHandle() const
{
    return rtvHandleGpu_;
}

const std::string& SeparatedGaussianFilter::GetName() const
{
    return name_;
}

void SeparatedGaussianFilter::Apply()
{
    commandList_->DrawInstanced(3, 1, 0, 0); // 三角形を1つ描画

    // 縦方向のガウスフィルタを適用
    this->_ToShaderResourceState(horizontalGaussTexture_);
    this->_ToRenderTargetState(renderTexture_);
    this->_Setting(horizontalHandleGpu_, rtvHandleCpu_, execInfoResourceVertical_.Get());

    commandList_->DrawInstanced(3, 1, 0, 0); // 三角形を1つ描画
}

void SeparatedGaussianFilter::Finalize()
{
}

void SeparatedGaussianFilter::Setting()
{
    this->_ToRenderTargetState(horizontalGaussTexture_);
    this->_Setting(inputGpuHandle_, horizontalHandleCpu_, execInfoResourceHorizontal_.Get());
}

void SeparatedGaussianFilter::OnResizeBefore()
{
    SRVManager::GetInstance()->Deallocate(srvHeapIndex_);
    SRVManager::GetInstance()->Deallocate(horizontalSrvHeapIndex_);
    horizontalGaussTexture_.Reset();
    renderTexture_.Reset();
}

void SeparatedGaussianFilter::OnResizedBuffers()
{
    // レンダーテクスチャの生成
    Helper::CreateRenderTexture(pDx12_, device_, horizontalGaussTexture_, horizontalHandleCpu_, horizontalHeapIndex_);
    horizontalGaussTexture_.resource->SetName(L"SeparatedGaussianFilterHorizontalGaussTexture");
    Helper::CreateRenderTexture(pDx12_, device_, renderTexture_, rtvHandleCpu_, rtvHeapIndex_);
    renderTexture_.resource->SetName(L"SeparatedGaussianFilterRenderTexture");

    // レンダーテクスチャのSRVを生成
    Helper::CreateSRV(horizontalGaussTexture_, horizontalHandleGpu_, horizontalSrvHeapIndex_);
    Helper::CreateSRV(renderTexture_, rtvHandleGpu_, srvHeapIndex_);
}

void SeparatedGaussianFilter::ToShaderResourceState()
{
    this->_ToShaderResourceState(renderTexture_);
}

void SeparatedGaussianFilter::DebugOverlay()
{
    #ifdef _DEBUG

    bool isChanged = false;

    if (ImGui::SliderInt("Kernel Size", reinterpret_cast<int*>(&pOption_->kernelSize), 3, 31, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
        pOption_->kernelSize = (pOption_->kernelSize / 2) * 2 + 1;
        isChanged = true;
    }

    if (ImGui::DragFloat("Sigma", &sigma_, 0.01f, 0.1f, 0.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
    {
        isChanged = true;
    }

    if (isChanged)
    {
        this->CreateKernel();
    }

    #endif // _DEBUG
}

void SeparatedGaussianFilter::CreateRootSignature()
{
    /// RootSignature作成
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
    descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // RootParameter作成。複数設定できるので配列
    RootParameters<3> rootParameters = {};
    rootParameters
        .SetParameter(0, "t0")
        .SetParameter(1, "b0")
        .SetParameter(2, "b1");

    descriptionRootSignature.pParameters = rootParameters.GetParams();      // ルートパラメータ配列へのポインタ
    descriptionRootSignature.NumParameters = rootParameters.GetSize();      // 配列の長さ

    StaticSamplerDesc staticSamplerDesc = {};
    staticSamplerDesc
        .PresetPointWrap()
        .SetMaxAnisotropy(16)
        .SetShaderRegister(0)
        .SetRegisterSpace(0);

    descriptionRootSignature.pStaticSamplers = &staticSamplerDesc.Get();
    descriptionRootSignature.NumStaticSamplers = 1;

    // シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "SeparatedGaussianFilter",
            __func__,
            reinterpret_cast<char*>(errorBlob->GetBufferPointer())
        );

        assert(false);
    }
    // バイナリをもとに生成
    hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));
}

void SeparatedGaussianFilter::CreatePipelineStateObject()
{
    IDxcUtils* dxcUtils = pDx12_->GetDxcUtils();
    IDxcCompiler3* dxcCompiler = pDx12_->GetDxcCompiler();
    IDxcIncludeHandler* includeHandler = pDx12_->GetIncludeHandler();

    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
    inputLayoutDesc.pInputElementDescs = nullptr;
    inputLayoutDesc.NumElements = 0;

    /// BlendStateの設定
    D3D12_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


    /// RasterizerStateの設定
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.MultisampleEnable = TRUE;  // アンチエイリアス有効化
    rasterizerDesc.AntialiasedLineEnable = TRUE;  // ラインのアンチエイリアス有効化

    /// ShaderをCompileする
    vertexShaderBlob_ = DX12Helper::CompileShader(kVertexShaderPath, L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
    assert(vertexShaderBlob_ != nullptr);

    pixelShaderBlob_ = DX12Helper::CompileShader(kPixelShaderPath, L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
    assert(pixelShaderBlob_ != nullptr);

    /// PSOを生成する
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();    // RootSignature
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;    // InputLayout
    graphicsPipelineStateDesc.VS = { vertexShaderBlob_.Get()->GetBufferPointer(), vertexShaderBlob_.Get()->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { pixelShaderBlob_.Get()->GetBufferPointer(), pixelShaderBlob_.Get()->GetBufferSize() };
    graphicsPipelineStateDesc.BlendState = blendDesc;            // BlendState
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;    // RasterizerState
    // 書き込むRTVの情報
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    // 利用するトポロジ（形状）のタイプ。三角形
    graphicsPipelineStateDesc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // どのように画面に色を打ち込むかの設定（気にしなくて良い）
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    // DepthStencilの設定
    graphicsPipelineStateDesc.DepthStencilState = {};
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // 実際に生成
    HRESULT hr = device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pso_));
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "SeparatedGaussianFilter",
            __func__,
            "Failed to create pipeline state"
        );
        assert(false);
    }

    return;
}

void SeparatedGaussianFilter::_ToRenderTargetState(ResourceStateTracker& _resource)
{
    // レンダーテクスチャをレンダーターゲット状態に変更
    _resource.ChangeState(commandList_, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void SeparatedGaussianFilter::_ToShaderResourceState(ResourceStateTracker& _resource)
{
    _resource.ChangeState(commandList_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void SeparatedGaussianFilter::CreateResourceCBuffer()
{
    optionResource_ = DX12Helper::CreateBufferResource(device_, sizeof(SeparatedGaussianFilterOption));
    optionResource_->Map(0, nullptr, reinterpret_cast<void**>(&pOption_));

    // 縦と横それぞれの実行情報を格納するリソースを作成
    execInfoResourceHorizontal_ = DX12Helper::CreateBufferResource(device_, sizeof(SeparatedGaussianFilterExecInfo));
    execInfoResourceHorizontal_->Map(0, nullptr, reinterpret_cast<void**>(&pExecInfoHorizontal_));
    execInfoResourceVertical_ = DX12Helper::CreateBufferResource(device_, sizeof(SeparatedGaussianFilterExecInfo));
    execInfoResourceVertical_->Map(0, nullptr, reinterpret_cast<void**>(&pExecInfoVertical_));

    // 初期化
    pOption_->kernelSize = 3; // カーネルサイズの初期値

    pExecInfoHorizontal_->direction[0] = 1;
    pExecInfoHorizontal_->direction[1] = 0;

    pExecInfoVertical_->direction[0] = 0; // 垂直方向
    pExecInfoVertical_->direction[1] = 1; // 垂直方向

    this->CreateKernel();
}

void SeparatedGaussianFilter::_Setting(D3D12_GPU_DESCRIPTOR_HANDLE _inputGpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE _outputCpuHandle, ID3D12Resource* _execInfoResource)
{
    // レンダーターゲットを設定 (自分が所有するテクスチャに対して設定)
    commandList_->OMSetRenderTargets(1, &_outputCpuHandle, FALSE, nullptr);

    // PSOとルートシグネチャを設定
    commandList_->SetGraphicsRootSignature(rootSignature_.Get());
    commandList_->SetPipelineState(pso_.Get());

    // 入力テクスチャのSRVを設定する（自分が所有するテクスチャのSRVではないため注意)
    commandList_->SetGraphicsRootDescriptorTable(0, _inputGpuHandle);

    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList_->SetGraphicsRootConstantBufferView(1, optionResource_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootConstantBufferView(2, _execInfoResource->GetGPUVirtualAddress());
}

void SeparatedGaussianFilter::CreateKernel()
{
    // カーネルサイズに応じて重みを計算
    int kernelSize = pOption_->kernelSize;
    int halfKernelSize = kernelSize / 2;
    float sum = 0.0f;
    for (int i = 0; i < kernelSize; ++i)
    {
        int x = i - halfKernelSize;
        // 1次元のガウス関数
        float w = std::exp(-0.5f * (x * x) / (sigma_ * sigma_));
        pOption_->weights[i].value = w;
        sum += w;
    }

    // 正規化（合計が1になるように）
    for (int i = 0; i < kernelSize; ++i)
    {
        pOption_->weights[i].value /= sum;
    }
}
