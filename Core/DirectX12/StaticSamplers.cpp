#include <StaticSamplers.h>

void StaticSamplers::Configurator::Initialize()
{
    data_[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                         // BilinearFilter
    data_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                       // 0 ~ 1の範囲外をリピート
    data_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    data_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    data_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;                     // 比較しない
    data_[0].MaxLOD = D3D12_FLOAT32_MAX;                                       // ありったけのーを使う
    data_[0].ShaderRegister = 0;                                               // レジスタ番号0を使用する
    data_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderを使う
}