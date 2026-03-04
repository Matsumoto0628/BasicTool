Texture2D image : register(t0);
SamplerState samplerState : register(s0);

struct Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_TARGET
{
    float alpha = image.Sample(samplerState, input.uv).a;
    clip(alpha - 0.01f);
    return 0;
}