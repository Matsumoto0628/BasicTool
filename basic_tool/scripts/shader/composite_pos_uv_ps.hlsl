Texture2D srvHDR : register(t0);
Texture2D srvBloom : register(t1);
SamplerState samLinear : register(s0);

struct Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : SV_Target
{
    float4 hdr = srvHDR.Sample(samLinear, input.uv);
    float4 bloom = srvBloom.Sample(samLinear, input.uv);
    
    float bloomIntensity = 0.4;
    float3 color = hdr + bloom * bloomIntensity;
    float alpha = saturate(dot(color, float3(0.2126, 0.7152, 0.0722)));
    
    return float4(color, alpha);
}