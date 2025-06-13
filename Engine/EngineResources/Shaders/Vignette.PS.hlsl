#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct VignetteOption
{
    float3 color;
    float scale;
    float power;
    int enableMultiply;
    
    float3 padding;
};

ConstantBuffer<VignetteOption> gOption : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // 周囲を0に、中心になるほど明るくなるようにする
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);

    // 中心の最大値が暗すぎるためScaleで調整する (cbv)
    float vignette = correct.x * correct.y * gOption.scale;
    
    // サチュレーションで超えないようにしながら、いい感じに
    vignette = saturate(pow(vignette, gOption.power));
    
    // 係数として乗算
    float3 color_blend_alpha = lerp(gOption.color.rgb, output.color.rgb, vignette);
    float3 color_blend_multiply = output.color.rgb * lerp(gOption.color.rgb, float3(1,1,1), vignette);
    
    output.color.rgb = lerp(color_blend_alpha, color_blend_multiply, float(gOption.enableMultiply));
    
    output.color.a = 1.0f;

    return output;
}