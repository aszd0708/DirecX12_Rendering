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

struct VertexColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexTexture
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
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
};