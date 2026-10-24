#include "Global.hlsl"

TextureMeshOutput VS(VertexTexture input)
{
    TextureMeshOutput output;
    
    output.position = mul(float4(input.position, 1.0f), TransformMatrix.W);
    output.worldPosition = output.position;
    output.position = mul(output.position, GlobalMatrix.VP);
    output.uv = input.uv;
    output.materialIndexID = 0;
    
    return output;
}

float4 PS(TextureMeshOutput output) : SV_Target
{
    Texture2D<float4> colorMap = ResourceDescriptorHeap[TextureIndex.textureIndex];
    float4 color = colorMap.Sample(LinearSampler, output.uv);
    return color;
}