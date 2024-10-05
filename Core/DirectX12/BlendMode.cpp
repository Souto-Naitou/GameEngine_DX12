#include "BlendMode.h"

#include <cassert>

void BlendMode::Configurator::Initialize(BlendModes _mode)
{
	data_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	data_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	data_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	data_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	data_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	Apply(_mode);
}

void BlendMode::Configurator::Apply(BlendModes _mode)
{
	switch (_mode)
	{
	case BlendMode::BlendModes::None:
		data_.RenderTarget[0].BlendEnable = FALSE;
		break;

	case BlendMode::BlendModes::Normal:
		data_.RenderTarget[0].BlendEnable = TRUE;
		data_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		data_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		data_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendMode::BlendModes::Add:
		data_.RenderTarget[0].BlendEnable = TRUE;
		data_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		data_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		data_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::BlendModes::Subtract:
		data_.RenderTarget[0].BlendEnable = TRUE;
		data_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		data_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		data_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::BlendModes::Multiply:
		data_.RenderTarget[0].BlendEnable = TRUE;
		data_.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		data_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		data_.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendMode::BlendModes::Screen:
		data_.RenderTarget[0].BlendEnable = TRUE;
		data_.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		data_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		data_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::BlendModes::COUNT:
		assert(0 && "BlendModeにCOUNTが指定されています。");
		break;
	}

	return;
}
