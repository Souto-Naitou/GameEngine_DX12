#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct RadialBlurOption
{
    float2 center;
    int samples;
    float blurWidth; 
};

ConstantBuffer<RadialBlurOption> gOption : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);

    float2 direction = input.texcoord - gOption.center;
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int sampleIndex = 0; sampleIndex < gOption.samples; ++sampleIndex)
    {
        float2 texcoord = input.texcoord + direction * gOption.blurWidth * float(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    // 平均化
    outputColor *= rcp(float(gOption.samples));
    
    output.color.rgb = outputColor.rgb;
    output.color.a = 1.0f;

    return output;
}