Texture2D srv : register(t0);
SamplerState samLinear : register(s0);

float luminance(float3 color)
{
    return dot(color, float3(0.2126, 0.7152, 0.0722));
}

struct Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_Target
{
    float3 result = srv.Sample(samLinear, input.uv).rgb;
    float lmc = luminance(result);
    float threshold = 1.0;
    
    float3 bloom = (lmc > threshold) ? result : float3(0, 0, 0);
    return float4(bloom, 1.0);
}