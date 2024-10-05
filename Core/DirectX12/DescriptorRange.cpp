#include <DescriptorRange.h>

void DescriptorRange::Configurator::Initialize()
{
    /// ディスクリプタテーブルのディスクリプタレンジを設定する (SRVを通じてシェーダにリソースへのアクセスを提供・t0)
    data_[0].BaseShaderRegister = 0; // 0から始まる
    data_[0].NumDescriptors = 1; // 数は1つ
    data_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
    data_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
    data_[0].RegisterSpace = 0;
}