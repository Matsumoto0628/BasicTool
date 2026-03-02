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
    float4 color = srv.Sample(samLinear, input.uv);
    float lmc = luminance(color.rgb);
    float threshold = 1.0;
    
    float4 result = (lmc > threshold) ? color : float4(0, 0, 0, 0);
    return result;
}