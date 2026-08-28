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

cbuffer GlobamMatrixBuffer : register(b0)
{
    GlobalMatrixDesc GlobalMatrix;
};

cbuffer TransformMatrixBuffer : register(b1)
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