struct Input
{
    float4 pos : SV_POSITION;
    float4 normal : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(Input input) : SV_TARGET
{
    return input.color;
}