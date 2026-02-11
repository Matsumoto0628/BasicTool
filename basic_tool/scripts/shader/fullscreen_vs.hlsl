// InputLayoutとVertexBufferが不要

struct Output
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

Output main(uint id : SV_VertexID) // SV_VertexIDを使うために5.0にしたが、ほかのシェーダーは4.0のまま
{
    Output output;
    
    float2 pos[3] =
    {
        float2(-1.0, -1.0),
        float2(-1.0, 3.0),
        float2(3.0, -1.0)
    };

    output.pos = float4(pos[id], 0.0, 1.0);
    output.uv = output.pos.xy * 0.5f + 0.5f; // NDC → uv
    output.uv.y = 1.0f - output.uv.y; // 上下反転
    return output;
}