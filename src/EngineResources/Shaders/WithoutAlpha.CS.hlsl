
/// SRV 
Texture2D<float4> inputTexture : register(t0);

/// UAV
RWTexture2D<float4> outputTexture : register(u0);

[numthreads(16, 16, 1)]
void CSMain(uint3 dtid : SV_DispatchThreadID)
{
    // ピクセルカラーを取得
    float4 color = inputTexture.Load(int3(dtid.xy, 0));
        
    // 出力テクスチャに書き込み
    outputTexture[dtid.xy] = float4(color.xyz, 1.0);
}