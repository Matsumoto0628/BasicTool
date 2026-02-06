cbuffer WVP : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
};

struct Input
{
    float4 pos : POSITION;
    float4 normal : TEXCOORD0;
    float4 color : TEXCOORD1;
};

struct Output
{
    float4 pos : SV_POSITION;
    float4 normal : TEXCOORD0;
    float4 color : TEXCOORD1;
};

Output main(Input input)
{
    Output output;
    output.pos = mul(input.pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Proj);
    output.normal = input.normal;
    output.color = input.color;
    return output;
}