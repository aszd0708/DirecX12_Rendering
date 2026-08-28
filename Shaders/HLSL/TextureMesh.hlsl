#include "Global.hlsl"

Texture2D ColorMap : register(t0);
SamplerState LinearSampler : register(s0);

TextureMeshOutput VS(VertexTexture input)
{
    TextureMeshOutput output;
    
    output.position = mul(float4(input.position, 1.0f), TransformMatrix.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.uv = input.uv;
    
    return output;
}

float4 PS(TextureMeshOutput output) : SV_Target
{
    float4 color = ColorMap.Sample(LinearSampler, output.uv);
    //float4 color = float4(output.uv.x, output.uv.y, 0.0f, 1.0f);
    return color;
}