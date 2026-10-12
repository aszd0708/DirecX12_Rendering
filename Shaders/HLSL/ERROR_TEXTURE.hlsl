#include "Global.hlsl"

MeshOutput VS(VertexTexture input)
{
    MeshOutput output;
    
    output.position = mul(input.position, TransformMatrix.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.color = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return output;
}

float4 PS(MeshOutput output) : SV_Target
{
    float4 color = output.color;
    return color;
}