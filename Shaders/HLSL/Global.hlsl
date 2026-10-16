#include "ShaderShared.h"

#define CONCAT_IMPL(a, b)   a ## b
#define CONCAT(a, b)        CONCAT_IMPL(a, b)

////////////////////
/// GlobalDesces ///
////////////////////

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
    
    uint padding[3];
};

StructuredBuffer<InstanceDesc> InstancingBuffer : register(CONCAT(t, INSTANCE_REGISTER));

// For Default Shader Material Index
// MaterialInfo.h DefaultMaterialTextureDesc 와 같은 구조여야 함
struct DefaultMaterialTextureDesc
{
    uint normalTextureIndex;
    uint diffuseTextureIndex;
    
    uint padding[2];
};

StructuredBuffer<DefaultMaterialTextureDesc> DefaultMaterialTextureBuffer : register(CONCAT(t, MATERIAL_REGISTER));