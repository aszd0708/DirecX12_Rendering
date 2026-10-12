#include "Global.hlsl"

MeshOutput VS(VertexError input)
{
    MeshOutput output;
    
    InstanceDesc instasnceData = InstancingBuffer[input.instanceID];
    
    output.position = mul(float4(input.position, 1.0f), instasnceData.W);
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