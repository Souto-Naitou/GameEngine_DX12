#include "./PipelineStateObject.h"
#include <stdexcept>

using _me = PipelineStateObject;
using _ref = _me&;

_ref PipelineStateObject::SetDesc(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& _desc)
{
    desc_ = _desc;
    return *this;
}

_ref PipelineStateObject::SetRootSignature(ID3D12RootSignature* _pRootSignature)
{
    desc_.pRootSignature = _pRootSignature;
    return *this;
}

_ref PipelineStateObject::SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& _inputLayoutDesc)
{
    desc_.InputLayout = _inputLayoutDesc;
    return *this;
}

_ref PipelineStateObject::SetVertexShader(const void* _pShaderByteCode, size_t _length)
{
    desc_.VS.pShaderBytecode = _pShaderByteCode;
    desc_.VS.BytecodeLength = _length;
    return *this;
}

_ref PipelineStateObject::SetPixelShader(const void* _pShaderByteCode, size_t _length)
{
    desc_.PS.pShaderBytecode = _pShaderByteCode;
    desc_.PS.BytecodeLength = _length;
    return *this;
}

_ref PipelineStateObject::SetRasterizerState(const D3D12_RASTERIZER_DESC& _rasterizerDesc)
{
    desc_.RasterizerState = _rasterizerDesc;
    return *this;
}

_ref PipelineStateObject::SetBlendState(const D3D12_BLEND_DESC& _blendDesc)
{
    desc_.BlendState = _blendDesc;
    return *this;
}

_ref PipelineStateObject::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& _depthStencilDesc)
{
    desc_.DepthStencilState = _depthStencilDesc;
    return *this;
}

_ref PipelineStateObject::SetSampleMask(UINT _sampleMask)
{
    desc_.SampleMask = _sampleMask;
    return *this;
}

_ref PipelineStateObject::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE _topologyType)
{
    desc_.PrimitiveTopologyType = _topologyType;
    return *this;
}

_ref PipelineStateObject::SetRenderTargetFormats(UINT _numRenderTargets, const DXGI_FORMAT* _rtvFormats, DXGI_FORMAT _dsvFormat)
{
    desc_.NumRenderTargets = _numRenderTargets;
    for (UINT i = 0; i < _numRenderTargets; ++i)
    {
        desc_.RTVFormats[i] = _rtvFormats[i];
    }
    desc_.DSVFormat = _dsvFormat;
    return *this;
}

_ref PipelineStateObject::SetSampleDesc(const DXGI_SAMPLE_DESC& _sampleDesc)
{
    desc_.SampleDesc = _sampleDesc;
    return *this;
}

void PipelineStateObject::Build(ID3D12Device* _device)
{
    HRESULT hr = _device->CreateGraphicsPipelineState(&desc_, IID_PPV_ARGS(&pso_));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create graphics pipeline state.");
    }
}
