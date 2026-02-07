cbuffer WVP : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
};

struct Input
{
    float4 pos : POSITION;
    float4 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

struct Output
{
    float4 pos : SV_POSITION;
    float4 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

Output main(Input input)
{
    Output output;
    output.pos = mul(input.pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Proj);
    output.color = input.color;
    output.uv = input.uv;
    return output;
}