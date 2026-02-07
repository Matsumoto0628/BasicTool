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
    float alpha = image.Sample(samplerState, input.uv).a;
    clip(alpha - 0.01); // 不透明だけ Z に書く
    return 0;
}
