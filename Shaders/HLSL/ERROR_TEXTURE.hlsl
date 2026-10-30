#include "Global.hlsl"

TextureMeshOutput VS(VertexTexture input)
{
    TextureMeshOutput output;
    
    InstanceDesc instasnceData = InstancingBuffer[input.instanceID];
    
    output.position = mul(float4(input.position, 1.0f), instasnceData.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.uv = input.uv;
    output.materialIndexID = instasnceData.materialIndex;
    
    return output;
}

float4 PS(TextureMeshOutput output) : SV_Target
{
    uint textureIndex = DefaultMaterialTextureBuffer[output.materialIndexID].diffuseTextureIndex;
    Texture2D<float4> colorMap = ResourceDescriptorHeap[textureIndex];
    
    float4 color = colorMap.Sample(LinearSampler, output.uv);
    return color;
}