#include "Lighting.hlsli"
#include "Uniforms.hlsli"
Texture2D<float4> u_texture : register(t0, space2);
SamplerState u_sampler : register(s0, space2);

cbuffer ObjectUniforms : register(OBJECT_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Object object;
}

cbuffer FrameUniforms : register(FRAME_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Frame frame;
}

cbuffer LightUniforms : register(LIGHT_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Lights lights;
}

cbuffer MaterialUniforms : register(MATERIAL_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Material material;
}

struct Input
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 normal : TEXCOORD2;
};

struct Output
{
    float4 color : SV_Target0;
};

Output main(Input input)
{
    Output output;
    output.color = u_texture.Sample(u_sampler, input.texcoord);
    return output;
}