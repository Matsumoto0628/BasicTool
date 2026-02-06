Texture2D image : register(t0);
SamplerState samplerState : register(s0);

struct Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_TARGET
{
    float4 color = image.Sample(samplerState, input.uv);
    return color;
}