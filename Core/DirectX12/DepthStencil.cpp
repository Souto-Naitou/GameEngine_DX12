#include <DepthStencil.h>

void DepthStencil::Configurator::Initialize()
{
    /// DepthStencilStateの設定
// Depthの機能を有効にする
    current_.DepthEnable = true;
    // 書き込みする
    current_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    // 比較関数はLessEqual。つまり、近ければ描画される
    current_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}