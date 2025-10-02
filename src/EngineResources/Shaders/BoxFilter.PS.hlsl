#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct BoxFilterOption
{
    int kernelSize;
};

ConstantBuffer<BoxFilterOption> gOptions : register(b0);                        // カーネルのサイズを指定する定数バッファ

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 texelSize = float2(rcp(float(width)), rcp(float(height)));
    
    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    int kernelSize = gOptions.kernelSize;
    int halfKernelSize = kernelSize / 2;
    float weight = 1.0f / (kernelSize * kernelSize);
    
    for (int i = -halfKernelSize; i <= halfKernelSize; ++i)
    {
        for (int j = -halfKernelSize; j <= halfKernelSize; ++j)
        {
            float2 offset = float2(i, j) * texelSize;
            // 現在のtexcoordを算出
            float2 texcoord = input.texcoord + offset;
            // 色に掛けて加算
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += fetchColor * weight;
        }
    }
    output.color.a = 1.0f;
    return output;
}