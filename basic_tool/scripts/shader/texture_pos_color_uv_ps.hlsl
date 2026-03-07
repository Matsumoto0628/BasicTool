Texture2D image : register(t0);
SamplerState samplerState : register(s0);

struct Input
{
    float4 pos : SV_POSITION;
    float4 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

float4 main(Input input) : SV_TARGET
{
    return image.Sample(samplerState, input.uv) * input.color;
}