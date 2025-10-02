#include "Particle.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float4 textureColor = gTexture.Sample(gSampler, input.texcoord.xy);
    output.color = textureColor * input.color;

    if (textureColor.a <= 0.f || output.color.a <= 0.f)
    {
        discard;
    }

    return output;
}