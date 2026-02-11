Texture2D srvHDR : register(t0);
Texture2D srvBloom : register(t1);
SamplerState samLinear : register(s0);

float3 toneMap(float3 color)
{
    return color / (color + 1.0);
}

struct Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_Target
{
    float3 hdr = srvHDR.Sample(samLinear, input.uv).rgb;
    float3 bloom = srvBloom.Sample(samLinear, input.uv).rgb;
    
    float bloomIntensity = 1.2;
    float3 color = hdr + bloom * bloomIntensity;
    
    return float4(color, 1.0);
}