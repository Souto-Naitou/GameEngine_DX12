#include "Line.hlsli"

struct userColor
{
    float4 color;
};

ConstantBuffer<userColor> gColor : register(b0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gColor.color;
    return output;
}