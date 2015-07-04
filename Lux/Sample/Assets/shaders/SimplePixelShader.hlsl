/*
Shader generated by Savvy - The Smart Shader Cross Compiler.
Author: Apostol Dadachev
Version: 1.0
*/


struct Light
{
	float4 m_Color;
	float3 m_Direction;
	float m_Intensity;
	float3 m_Position;
};

struct Material
{
	float4 m_DiffuseColor;
	float4 m_SpecularColor;
	float4 m_EmissiveColor;
	float4 m_AmbientColor;
	float m_Specular;
};

cbuffer LightBuffer : register(b0)
{
	Light light;
	float3 EyePos;
};

cbuffer MaterialBuffer : register(b1)
{
	Material materialProp;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

struct InputData
{
	float3 Normal : NORMAL;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
	float4 WorldPos : WORLD_POS;
	float4 gl_Position : SV_Position;
};

//--------------------------------------------------------------------------------------
// Blinn-Phong Lighting Reflection Model
//--------------------------------------------------------------------------------------
float4 calcBlinnPhongLighting(Material mat, Light currLight, float3 N, float3 H )
{
    float4 Ia = mat.m_AmbientColor;
    float4 Id = mat.m_DiffuseColor * saturate(dot(N,-currLight.m_Direction));
    float4 Is = mat.m_SpecularColor * pow(saturate(dot(N,H)), mat.m_Specular);
 
    return (Ia + (Id + Is) * currLight.m_Color) * currLight.m_Intensity;
}

float4 main(InputData IN) : SV_TARGET
{
    IN.Normal = normalize(IN.Normal);
    float3 V = normalize(EyePos - (float3)IN.WorldPos);
    float3 R = reflect( light.m_Direction, IN.Normal);
	float3 H = normalize( -light.m_Direction + V );
    //calculate lighting
    float4 lighting = calcBlinnPhongLighting(materialProp, light, IN.Normal, H);
	float4 retCol = lighting * DiffuseTexture.Sample(DiffuseTextureSampler, IN.TexCoord);
	return retCol;
}

