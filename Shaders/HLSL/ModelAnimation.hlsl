#include "Global.hlsl"
#include "GlobalAnimation.hlsl"

TextureMeshOutput VS(AnimationModel input)
{
    TextureMeshOutput output;
    
    InstanceDesc instanceData = InstancingBuffer[input.instanceID];
    
    uint paletStartIdx = instanceData.paletStartIndex;
    
    matrix animatedMatrix = GetAnimationMatrix(paletStartIdx, input.blendWeight, input.blendIndices);
    
    float4 animatedPosition = mul(float4(input.position, 1.0f), animatedMatrix);
    output.position = mul(animatedPosition, instanceData.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.uv = input.uv;
    output.materialIndexID = instanceData.materialIndex;
    
    return output;
}

float4 PS(TextureMeshOutput output) : SV_Target
{
    uint textureIndex = DefaultMaterialTextureBuffer[output.materialIndexID].diffuseTextureIndex;
    Texture2D<float4> colorMap = ResourceDescriptorHeap[textureIndex];
    
    float4 color = colorMap.Sample(LinearSampler, output.uv);
    return color;
}