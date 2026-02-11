Texture2D srv : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBufferBlur : register(b0)
{
    float2 texelSize;
    float2 dir;
};

struct Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_Target
{
    float3 result = 0;
    float2 offset = dir * texelSize;
    result += srv.Sample(samLinear, input.uv).rgb * 0.5;
    result += srv.Sample(samLinear, input.uv + offset).rgb * 0.25;
    result += srv.Sample(samLinear, input.uv - offset).rgb * 0.25;
    return float4(result, 1.0);
}