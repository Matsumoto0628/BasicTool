cbuffer Transform : register(b0)
{
    float4x4 WorldViewProj;
};

struct Input
{
    float3 pos : POSITION;
};

struct Output
{
    float4 pos : SV_POSITION;
};

Output main(Input input)
{
    Output output;
    output.pos = mul(float4(input.pos, 1), WorldViewProj);
    return output;
}