#include "Vignette.h"
#include <cassert>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/PostEffect.h>
#include <Effects/PostEffects/Helper/PostEffectHelper.h>
#include <Core/DirectX12/SRVManager.h>
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <imgui.h>

void PEVignette::Initialize()
{
    pDx12_ = DirectX12::GetInstance();
    device_ = pDx12_->GetDevice();
    commandList_ = PostEffect::GetInstance()->GetCommandList();

    // レンダーテクスチャの生成
    Helper::CreateRenderTexture(device_, renderTexture_, rtvHandleCpu_, rtvHeapIndex_);
    renderTexture_.resource->SetName(L"VignetteRenderTexture");

    // レンダーテクスチャのSRVを生成
    Helper::CreateSRV(renderTexture_, rtvHandleGpu_, srvHeapIndex_);

    // ルートシグネチャの生成
    this->CreateRootSignature();

    // パイプラインステートの生成
    this->CreatePipelineStateObject();

    // 設定用リソースの生成と初期化
    this->CreateResourceCBuffer();
}

void PEVignette::Enable(bool _flag)
{
    isEnabled_ = _flag;
}

void PEVignette::SetInputTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle)
{
    inputGpuHandle_ = _gpuHandle;
}

bool PEVignette::Enabled() const
{
    return isEnabled_;
}

D3D12_GPU_DESCRIPTOR_HANDLE PEVignette::GetOutputTextureHandle() const
{
    return rtvHandleGpu_;
}

const std::string& PEVignette::GetName() const
{
    return name_;
}

void PEVignette::Apply()
{
    commandList_->DrawInstanced(3, 1, 0, 0); // 三角形を1つ描画
}

void PEVignette::Release()
{
}

void PEVignette::Setting()
{
    // レンダーテクスチャをレンダーターゲット状態に変更
    this->ToRenderTargetState();

    // レンダーターゲットを設定 (自分が所有するテクスチャに対して設定)
    commandList_->OMSetRenderTargets(1, &rtvHandleCpu_, FALSE, nullptr);

    // PSOとルートシグネチャを設定
    commandList_->SetGraphicsRootSignature(rootSignature_.Get());
    commandList_->SetPipelineState(pso_.Get());

    // 入力テクスチャのSRVを設定する（自分が所有するテクスチャのSRVではないため注意)
    commandList_->SetGraphicsRootDescriptorTable(0, inputGpuHandle_);

    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList_->SetGraphicsRootConstantBufferView(1, optionResource_->GetGPUVirtualAddress());
}

void PEVignette::OnResizeBefore()
{
    SRVManager::GetInstance()->Deallocate(srvHeapIndex_);
    renderTexture_.resource.Reset();
    renderTexture_.state = D3D12_RESOURCE_STATE_PRESENT;
}

void PEVignette::OnResizedBuffers()
{
    // レンダーテクスチャの生成
    Helper::CreateRenderTexture(device_, renderTexture_, rtvHandleCpu_, rtvHeapIndex_);
    // レンダーテクスチャのSRVを生成
    Helper::CreateSRV(renderTexture_, rtvHandleGpu_, srvHeapIndex_);
}

void PEVignette::ToShaderResourceState()
{
    // レンダーテクスチャをシェーダーリソース状態に変更
    DX12Helper::ChangeStateResource(
        commandList_,
        renderTexture_,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
}

void PEVignette::DebugOverlay()
{
    #ifdef _DEBUG
    ImGui::DragFloat("Scale", &pOption_->scale, 0.01f, FLT_MIN);
    ImGui::DragFloat("Power", &pOption_->power, 0.01f, FLT_MIN);
    ImGui::ColorEdit3("Power", &pOption_->color.x);
    ImGui::Checkbox("Multiply blending", reinterpret_cast<bool*>(&pOption_->enableMultiply));

    #endif //_DEBUG
}

void PEVignette::CreateRootSignature()
{
    D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
    descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
    descriptorRange[0].NumDescriptors = 1; // 数は1つ
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

    /// RootSignature作成
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
    descriptionRootSignature.Flags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // RootParameter作成。複数設定できるので配列
    D3D12_ROOT_PARAMETER rootParameters[2] = {};
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // DescriptorTableを使う
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;              // Tableの中身の配列を指定
    rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);  // Tableで利用する数

    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[1].Descriptor.ShaderRegister = 0;


    descriptionRootSignature.pParameters = rootParameters;                              // ルートパラメータ配列へのポインタ
    descriptionRootSignature.NumParameters = _countof(rootParameters);                  // 配列の長さ

    D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
    staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;              // 異方性フィルタリング
    staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].MipLODBias = 0.0f;                                    // ミップマップのオフセット
    staticSamplers[0].MaxAnisotropy = 16;                                   // 最大異方性
    staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;         // 比較なし
    staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE; // ボーダーカラー
    staticSamplers[0].MinLOD = 0.0f;                                        // 最小ミップレベル
    staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                           // 最大ミップレベル
    staticSamplers[0].ShaderRegister = 0;                                   // サンプラーのレジスタ番号
    staticSamplers[0].RegisterSpace = 0;                                    // レジスタスペース
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;     // ピクセルシェーダーで使用

    descriptionRootSignature.pStaticSamplers = staticSamplers;
    descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

    // シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "Object3dSystem",
            "CreateRootSignature",
            reinterpret_cast<char*>(errorBlob->GetBufferPointer())
        );

        assert(false);
    }
    // バイナリをもとに生成
    hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));
}

void PEVignette::CreatePipelineStateObject()
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
            "PostEffect",
            "CreatePipelineState",
            "Failed to create pipeline state"
        );
        assert(false);
    }

    return;
}

void PEVignette::ToRenderTargetState()
{
    // レンダーテクスチャをレンダーターゲット状態に変更
    DX12Helper::ChangeStateResource(
        commandList_,
        renderTexture_,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
}

void PEVignette::CreateResourceCBuffer()
{
    optionResource_ = DX12Helper::CreateBufferResource(device_, sizeof(VignetteOption));
    optionResource_->Map(0, nullptr, reinterpret_cast<void**>(&pOption_));

    pOption_->scale = 16.0f;
    pOption_->power = 0.8f;
    pOption_->color = { 0.0f, 0.0f, 0.0f };
}
