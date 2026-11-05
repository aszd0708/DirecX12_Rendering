#include "ShaderShared.h"

////////////////////
// AnimationBuffer//
////////////////////

struct AnimationModel
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    
    uint instanceID : SV_InstanceID;
    float4 blendWeight : BLENDWEIGHT;
    float4 blendIndices : BLENDINDICES;
};

StructuredBuffer<matrix> AnimationBuffer : register(CONCAT(t, ANIMATION_REGISTER));

matrix GetAnimationMatrix(uint paletIndex, float4 blendWeights, float4 blendIndices)
{
    float indices[4] = { blendIndices.x, blendIndices.y, blendIndices.z, blendIndices.w };
    float weights[4] = { blendWeights.x, blendWeights.y, blendWeights.z, blendWeights.w };
    
    matrix transform = 0;
    
    for (int i = 0; i < 4; i++)
    {
        uint index = indices[i];
        float weight = weights[i];
        matrix paletMatrix = AnimationBuffer[paletIndex + index];

        transform += (paletMatrix * weight);
    }
    
    return transform;
}