struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTextureCoordinate : TEXTURE_COORDINATE;
};

Texture2D objectTexture : TEXTURE : register(t0);
SamplerState objectSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET {
    float3 pixelColor = objectTexture.Sample(objectSamplerState, input.inTextureCoordinate);

    return float4(pixelColor, 1.0f);
}