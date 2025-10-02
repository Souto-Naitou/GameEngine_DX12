#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gTextureMask : register(t1);
SamplerState gSampler : register(s0);

struct DissolveOption
{
    float threshold;
    float edgeThresholdOffset;
    float4 colorDissolve;
    float4 colorEdge;
};

ConstantBuffer<DissolveOption> gOptions : register(b0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float mask = gTextureMask.Sample(gSampler, input.texcoord);
    
    // maskが閾値以下ならば
    if (mask <= gOptions.threshold)
    {
        output.color = gOptions.colorDissolve; // 指定された色で塗りつぶす
        output.color.a = 1.0; // アルファ値を1に設定
        return output; // ここでdiscardする代わりに、色を設定して返す
    }
    
    float edge = 1.0f - smoothstep(gOptions.threshold, gOptions.threshold + gOptions.edgeThresholdOffset, mask);
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    output.color.rgb += edge * gOptions.colorEdge.rgb; // エッジの色を加算
    output.color.a = 1.0; // アルファ値を1に設定
    
    return output;
}