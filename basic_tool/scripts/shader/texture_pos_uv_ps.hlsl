Texture2D image : register(t0);
SamplerState samplerState : register(s0);

cbuffer Color : register(b1)
{
    float4 color;
};

struct Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_TARGET
{
    return image.Sample(samplerState, input.uv) * color;
}