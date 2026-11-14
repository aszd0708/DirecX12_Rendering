#include "ShaderShared.h"

#define PI 3.14159265359

////////////////////
/// GlobalDesces ///
////////////////////

SamplerState LinearSampler : register(CONCAT(s, LINEAR_SAMPLER_REGISTER));

struct GlobalMatrixDesc
{
    matrix V;
    matrix P;
    matrix VP;
    matrix VInv;
};

struct TransformMatrixDesc
{
    matrix W;
};

cbuffer GlobamMatrixBuffer : register(CONCAT(b, GLOBAL_REGISTER))
{
    GlobalMatrixDesc GlobalMatrix;
};

cbuffer TransformMatrixBuffer : register(CONCAT(b, TRANSFORM_REGISTER))
{
    TransformMatrixDesc TransformMatrix;
}

struct TextureIndexDesc
{
    uint textureIndex;
};

cbuffer TextureIndexBuffer : register(CONCAT(b, TEXTURE_INDEX_REGISTER))
{
    TextureIndexDesc TextureIndex;
};

//////////////
// Function //
//////////////

float3 CameraPosition()
{
    return GlobalMatrix.VInv._41_42_43;
}

float3 CameraDirection(float3 worldPosition)
{
    float3 cameraPosition = CameraPosition();
    float3 viewVector = normalize(cameraPosition - worldPosition);
    return viewVector;
}

///////////////
// MeshBuffer//
///////////////

struct VertexError
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID;
};

struct VertexColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexTexture
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    uint instanceID : SV_InstanceID; 
};

struct MeshOutput
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 color : COLOR;
};

struct TextureMeshOutput
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 uv : TEXCOORD;
    nointerpolation uint materialIndexID : TEXINDEX;
};

// GlobalBuffer.h InstanceDesc 와 같은 구조여야 함
struct InstanceDesc
{
    matrix W;
    uint materialIndex;
    
    // For Animation
    uint paletStartIndex;
    
    uint padding[2];
};

StructuredBuffer<InstanceDesc> InstancingBuffer : register(CONCAT(t, INSTANCE_REGISTER));

// For Default Shader Material Index
struct DefaultMaterialTextureDesc
{
    uint normalTextureIndex;
    uint diffuseTextureIndex;
    
    uint padding[2];
    
    uint maxTextureCountPadding[MAX_TEXTURE_COUNT - DEFAULT_MATERIAL_TEX_COUNT - 4];
};

StructuredBuffer<DefaultMaterialTextureDesc> DefaultMaterialTextureBuffer : register(CONCAT(t, MATERIAL_REGISTER));