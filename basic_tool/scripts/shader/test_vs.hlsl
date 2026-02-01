cbuffer WVP : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
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
    output.pos = mul(float4(input.pos, 1), World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Proj);
    return output;
}