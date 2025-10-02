#include "./StaticSamplerDesc.h"

using _me = StaticSamplerDesc;

StaticSamplerDesc::StaticSamplerDesc()
{
    desc_.MipLODBias = 0.0f;                                    // ミップマップのオフセット
    desc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;         // 比較なし
    desc_.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE; // ボーダーカラー
    desc_.MinLOD = 0.0f;                                        // 最小ミップレベル
    desc_.MaxLOD = D3D12_FLOAT32_MAX;                           // 最大ミップレベル
    desc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;     // ピクセルシェーダーで使用
}

#pragma region "Setters"

    _me& StaticSamplerDesc::SetFilter(D3D12_FILTER _filter)
    {
        desc_.Filter = _filter;
        return *this;
    }

    _me& StaticSamplerDesc::SetAddressU(D3D12_TEXTURE_ADDRESS_MODE _addressU)
    {
        desc_.AddressU = _addressU;
        return *this;
    }

    _me& StaticSamplerDesc::SetAddressV(D3D12_TEXTURE_ADDRESS_MODE _addressV)
    {
        desc_.AddressV = _addressV;
        return *this;
    }

    _me& StaticSamplerDesc::SetAddressW(D3D12_TEXTURE_ADDRESS_MODE _addressW)
    {
        desc_.AddressW = _addressW;
        return *this;
    }

    _me& StaticSamplerDesc::SetAddressUVW(D3D12_TEXTURE_ADDRESS_MODE _addressUVW)
    {
        desc_.AddressU = _addressUVW;
        desc_.AddressV = _addressUVW;
        desc_.AddressW = _addressUVW;
        return *this;
    }

    _me& StaticSamplerDesc::SetMipLODBias(float _mipLODBias)
    {
        desc_.MipLODBias = _mipLODBias;
        return *this;
    }

    _me& StaticSamplerDesc::SetMaxAnisotropy(UINT _maxAnisotropy)
    {
        desc_.MaxAnisotropy = _maxAnisotropy;
        return *this;
    }

    _me& StaticSamplerDesc::SetComparisonFunc(D3D12_COMPARISON_FUNC _comparisonFunc)
    {
        desc_.ComparisonFunc = _comparisonFunc;
        return *this;
    }

    _me& StaticSamplerDesc::SetBorderColor(D3D12_STATIC_BORDER_COLOR _color)
    {
        desc_.BorderColor = _color;
        return *this;
    }

    _me& StaticSamplerDesc::SetMinLOD(float _minLOD)
    {
        desc_.MinLOD = _minLOD;
        return *this;
    }

    _me& StaticSamplerDesc::SetMaxLOD(float _maxLOD)
    {
        desc_.MaxLOD = _maxLOD;
        return *this;
    }

    _me& StaticSamplerDesc::SetShaderRegister(UINT _shaderRegister)
    {
        desc_.ShaderRegister = _shaderRegister;
        return *this;
    }

    _me& StaticSamplerDesc::SetRegisterSpace(UINT _registerSpace)
    {
        desc_.RegisterSpace = _registerSpace;
        return *this;
    }

    _me& StaticSamplerDesc::SetShaderVisibility(D3D12_SHADER_VISIBILITY _shaderVisibility)
    {
        desc_.ShaderVisibility = _shaderVisibility;
        return *this;
    }

#pragma endregion // "Setters"

#pragma region "Presets"

    _me& StaticSamplerDesc::PresetLinearClamp()
    {
        return SetFilter(D3D12_FILTER_MIN_MAG_MIP_LINEAR)
            .SetAddressUVW(D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    }

    _me& StaticSamplerDesc::PresetLinearWrap()
    {
        return SetFilter(D3D12_FILTER_MIN_MAG_MIP_LINEAR)
            .SetAddressUVW(D3D12_TEXTURE_ADDRESS_MODE_WRAP);
    }

    _me& StaticSamplerDesc::PresetPointClamp()
    {
        return SetFilter(D3D12_FILTER_MIN_MAG_MIP_POINT)
            .SetAddressUVW(D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    }

    _me& StaticSamplerDesc::PresetPointWrap()
    {
        return SetFilter(D3D12_FILTER_MIN_MAG_MIP_POINT)
            .SetAddressUVW(D3D12_TEXTURE_ADDRESS_MODE_WRAP);
    }

#pragma endregion // "Presets"