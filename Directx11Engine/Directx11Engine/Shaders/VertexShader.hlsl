cbuffer cBuffer : register(b0) 
{
	float4x4 matrix_f4x4;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTextureCoordinate : TEXTURE_COORDINATE;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTextCoordinate : TEXTURE_COORDINATE;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;

    output.outPosition = mul(float4(input.inPos, 1.0f), matrix_f4x4);
    output.outTextCoordinate = input.inTextureCoordinate;

    return output;
}
