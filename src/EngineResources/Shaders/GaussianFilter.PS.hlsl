#include "Fullscreen.hlsli"

static const float PI = 3.14159265f;

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct GaussianFilterOption
{
    int kernelSize;
    float sigma; // σはガウス関数の標準偏差
};

ConstantBuffer<GaussianFilterOption> gOptions : register(b0); // カーネルのサイズを指定する定数バッファ

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

float gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponent) * rcp(denominator);
}

PixelShaderOutput main(VertexShaderOutput input)
{   
    PixelShaderOutput output;
    
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 texelSize = float2(rcp(float(width)), rcp(float(height)));
    
    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    int kernelSize = gOptions.kernelSize;
    int halfKernelSize = kernelSize / 2;
    float weight = 0.0f;
    
    for (int i = -halfKernelSize; i <= halfKernelSize; ++i)
    {
        for (int j = -halfKernelSize; j <= halfKernelSize; ++j)
        {
            // オフセット
            float2 offset = float2(i, j) * texelSize;
            
            // ガウス関数で重みを計算
            float gaussianWeight = gauss(float(i), float(j), gOptions.sigma);
            weight += gaussianWeight;
            
            // 現在のtexcoordを算出
            float2 texcoord = input.texcoord + offset;
            // 色に掛けて加算
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += fetchColor * gaussianWeight;
        }
    }
    
    output.color.rgb *= rcp(weight);
    output.color.a = 1.0f;
    
    return output;
}