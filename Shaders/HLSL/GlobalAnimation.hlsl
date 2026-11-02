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