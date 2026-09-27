#include "Global.hlsl"

SamplerState LinearSampler : register(CONCAT(s, LINEAR_SAMPLER_REGISTER));

cbuffer TextureIndexBuffer : register(CONCAT(b, TEXTURE_INDEX_REGISTER))
{
    uint textureIndex;
};

TextureMeshOutput VS(VertexTexture input)
{
    TextureMeshOutput output;
    
    output.position = mul(float4(input.position, 1.0f), TransformMatrix.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.uv = input.uv;
    
    return output;
}

float4 PS(TextureMeshOutput output) : SV_Target
{
    Texture2D<float4> colorMap = ResourceDescriptorHeap[textureIndex];
    float4 color = colorMap.Sample(LinearSampler, output.uv);
    return color;
}