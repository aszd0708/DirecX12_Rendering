struct VertexColor
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUT VS(VertexColor input)
{
    VS_OUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}

float4 PS(VS_OUT output) : SV_Target
{
    return output.color;
}

