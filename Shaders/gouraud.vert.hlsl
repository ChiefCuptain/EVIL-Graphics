#include "Lighting.hlsli"

cbuffer VertexUniforms : register(b0, space1)
{
    float4x4 mvp;
    float4x4 model;
    float3 cameraPosition;
    float padding1;
}

cbuffer LightUniforms : register(b1, space1)
{
    Light light;
}

cbuffer MaterialUniforms : register(b2, space1)
{
    Material material;
}

struct Input
{
    float3 position : TEXCOORD0;
    float2 texcoord : TEXCOORD1;
    float3 normal : TEXCOORD2;
};

struct Output
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 diffuse : TEXCOORD1;
    float3 specular : TEXCOORD2;
};

Output main(Input input)
{
    Output output;
    output.texcoord = (input.texcoord * material.tiling) + material.offset;
    output.position = mul(mvp, float4(input.position, 1.0f));
    float3 worldPosition = mul(model, float4(input.position, 1.0f)).xyz;
    float3 radiance = light.color * light.intensity;
    float attenuation;
    float3 L = getLightVector(light, worldPosition, attenuation);
    float3 N = normalize(mul((float3x3)model, input.normal));
    // Diffuse

    float NdotL = saturate(dot(N, L));
 
    float3 diffuse = radiance * NdotL * attenuation;
    
    output.diffuse = light.ambient + diffuse;
    
    // SPECULAR
    output.specular = 0;

    if (NdotL > 0)
    {
        float3 R = reflect(-L, N);
        float3 V = normalize(cameraPosition - worldPosition);
        float3 H = normalize(V + L);
        float HdotN = saturate(dot(R, V));
        output.specular = pow(HdotN, material.shininess) * radiance * attenuation;

    }

    return output;
}