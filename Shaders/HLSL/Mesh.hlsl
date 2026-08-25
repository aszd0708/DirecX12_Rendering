#include "Global.hlsl"

MeshOutput VS(VertexColor input)
{
    MeshOutput output;
    
    output.position = mul(input.position, TransformMatrix.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.color = input.color;
    
    return output;
}

float4 PS(MeshOutput output) : SV_Target
{
    float4 color = output.color;
    return color;
}