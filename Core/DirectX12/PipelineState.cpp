#include "PipelineState.h"

#include <cassert>

PipelineState::Configurator::Configurator()
{
    // ブレンドモード
    pBlendModeCfg_ = std::make_unique<BlendMode::Configurator>();

    pInputLayoutDesc_ = std::make_unique<InputLayout::Configurator>();

    pRootSignature_ = std::make_unique<RootSignature::Configurator>();

    pVertexShaderBlob_ = std::make_unique<VertexShaderBlob::Configurator>();

    pPixelShaderBlob_ = std::make_unique<PixelShaderBlob::Configurator>();

    pRasterizerState_ = std::make_unique<RasterizerState::Configurator>();

    pDepthStencil_ = std::make_unique<DepthStencil::Configurator>();
}

void PipelineState::Configurator::Initialize(Types _type, ID3D12Device* _device)
{
    DxcUtils::Configurator::GetInstance()->Initialize();
    DxcCompiler::Configurator::GetInstance()->Initialize();
    IncludeHandler::Configurator::GetInstance()->Initialize();

    pBlendModeCfg_->Initialize(BlendMode::BlendModes::None);
    pInputLayoutDesc_->Initialize();
    pRootSignature_->Initialize(_device);
    pVertexShaderBlob_->Initialize();
    pPixelShaderBlob_->Initialize();
    pRasterizerState_->Initialize();
    pDepthStencil_->Initialize();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
    // RootSignature
    pipelineStateDesc.pRootSignature = pRootSignature_->Get();
    // InputLayout
    pipelineStateDesc.InputLayout = *pInputLayoutDesc_->Get();
    // VertexShader
    pipelineStateDesc.VS = { pVertexShaderBlob_->Get()->GetBufferPointer(), pVertexShaderBlob_->Get()->GetBufferSize()};
    // PixelShader
    pipelineStateDesc.PS = { pPixelShaderBlob_->Get()->GetBufferPointer(), pPixelShaderBlob_->Get()->GetBufferSize()};
    // BlendState
    pipelineStateDesc.BlendState = *pBlendModeCfg_->Get();
    // RasterizerState
    pipelineStateDesc.RasterizerState = *pRasterizerState_->Get();
    // 書き込むRTVの情報
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    // 利用するトポロジ（形状）のタイプ。三角形
    pipelineStateDesc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // どのように画面に色を打ち込むかの設定（気にしなくて良い）
    pipelineStateDesc.SampleDesc.Count = 1;
    pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    // DepthStencilの設定
    pipelineStateDesc.DepthStencilState = *pDepthStencil_->Get();
    pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    /// 実際に生成
    HRESULT hr_ = _device->CreateGraphicsPipelineState(&pipelineStateDesc,
        IID_PPV_ARGS(&data_));
    assert(SUCCEEDED(hr_));
    if (!data_) return;
}

void PipelineState::Configurator::Apply(Types _type)
{
    switch (_type)
    {
    case PipelineState::Types::None:
        break;
    case PipelineState::Types::Normal:
        break;
    case PipelineState::Types::Add:
        break;
    case PipelineState::Types::Subtract:
        break;
    case PipelineState::Types::Multiply:
        break;
    case PipelineState::Types::Screen:
        break;
    default:
        break;
    }
}
