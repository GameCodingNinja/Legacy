//======================================================================================//
// filename: FixedFuncShader.fx                                                         //
//                                                                                      //
// author:   Pedro V. Sander                                                            //
//           ATI Research, Inc.                                                         //
//           3D Application Research Group                                              //
//           email: psander@ati.com                                                     //
//                                                                                      //
// Description: A programmable shader that emulates the fixed function pipeline         //
//                                                                                      //
//======================================================================================//
//   (C) 2003 ATI Research, Inc.  All rights reserved.                                  //
//======================================================================================//

#define PI  3.14f

//this file contains light, fog, and texture types
#include "FixedFuncShader.fxh"

// Structs and variables with default values

float4 vMaterialColor = float4(192.f/255.f, 128.f/255.f, 96.f/255.f, 1.f);
float fMaterialPower = 16.f;

float4 vAmbientColor = float4(128.f/255.f, 128.f/255.f, 128.f/255.f, 1.f);

bool bSpecular : register(b0) = false;

bool bTweening : register(b2) = false;
float fTweenFactor = 0.f;

//fog settings
int iFogType = FOG_TYPE_NONE;
float4 vFogColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
float fFogStart = 10.f;
float fFogEnd = 25.f;
float fFogDensity = .02f;
bool bFogRange : register(b4) = false;

int iTexType = TEX_TYPE_NONE;
int iTexGenType = TEXGEN_TYPE_NONE;

struct CLight
{
   int iType;
   float3 vPos;
   float3 vDir;
   float4 vAmbient;
   float4 vDiffuse;
   float4 vSpecular;
   float  fRange;
   float3 vAttenuation; //1, D, D^2;
   float3 vSpot;        //cos(theta/2), cos(phi/2), falloff
};

//initial and range of directional, point and spot lights within the light array
int iLightDirIni;
int iLightDirNum;
int iLightPointIni;
int iLightPointNum;
int iLightSpotIni;
int iLightSpotNum;

CLight lights[5] = {                         //NUM_LIGHTS == 5
   {
      LIGHT_TYPE_DIRECTIONAL,                //type
      float3(0.0f, 0.0f, 0.0f),              //position
      float3(2.0f,-3.0f, 4.0f),              //direction
      float4(0.0f, 0.0f, 0.0f, 0.0f),        //ambient
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //diffuse
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //specular
      1000.f,                                //range
      float3(1.f, 0.f, 0.f),                 //attenuation
      float3(.999f, .996f, 1)                //spot (theta=PI/50, phi=PI/20)
   },
   {
      LIGHT_TYPE_NONE,                       //type
      float3(20.0f, 30.0f, 40.0f),           //position
      float3(-20.0f,-30.0f, -40.0f),         //direction
      float4(0.0f, 0.0f, 0.0f, 0.0f),        //ambient
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //diffuse
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //specular
      1000.f,                                //range
      float3(1.f, 0.f, 0.f),                 //attenuation
      float3(.999f, .996f, 1)                //spot (theta=PI/50, phi=PI/20)
   },
   {
      LIGHT_TYPE_NONE,                       //type
      float3(20.0f, 30.0f, 40.0f),           //position
      float3(0.0f, 0.0f, 0.0f),              //direction
      float4(0.0f, 0.0f, 0.0f, 0.0f),        //ambient
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //diffuse
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //specular
      1000.f,                                //range
      float3(1.f, 0.f, 0.f),                 //attenuation
      float3(.999f, .996f, 1)                //spot (theta=PI/50, phi=PI/20)
   },
   {
      LIGHT_TYPE_NONE,                       //type
      float3(20.0f, 30.0f, 40.0f),           //position
      float3(0.0f, 0.0f, 0.0f),              //direction
      float4(0.0f, 0.0f, 0.0f, 0.0f),        //ambient
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //diffuse
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //specular
      1000.f,                                //range
      float3(1.f, 0.f, 0.f),                 //attenuation
      float3(.999f, .996f, 1)                //spot (theta=PI/50, phi=PI/20)
   },
   {
      LIGHT_TYPE_NONE,                       //type
      float3(20.0f, 30.0f, 40.0f),           //position
      float3(0.0f, 0.0f, 0.0f),              //direction
      float4(0.0f, 0.0f, 0.0f, 0.0f),        //ambient
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //diffuse
      float4(1.0f, 1.0f, 1.0f, 1.0f),        //specular
      1000.f,                                //range
      float3(1.f, 0.f, 0.f),                 //attenuation
      float3(.999f, .996f, 1)                //spot (theta=PI/50, phi=PI/20)
   }
};

//transformation matrices
float4x4 matWorldViewProj  : WORLDVIEWPROJ;
float4x4 matWorldView      : WORLDVIEW;
float4x4 matWorld          : WORLD;
float4x4 matWorldViewIT;
float4x4 matViewIT;

//function output structures
struct VS_OUTPUT
{
   float4 Pos           : POSITION;
   float4 Color         : COLOR0;
   float4 ColorSpec     : COLOR1;
   float4 Tex0          : TEXCOORD0;
   float  Fog           : FOG;
};

struct COLOR_PAIR
{
   float4 Color         : COLOR0;
   float4 ColorSpec     : COLOR1;
};


//-----------------------------------------------------------------------------
// Name: DoDirLight()
// Desc: Directional light computation
//-----------------------------------------------------------------------------
COLOR_PAIR DoDirLight(float3 N, float3 V, int i)
{
   COLOR_PAIR Out;
   float3 L = mul((float3x3)matViewIT, -normalize(lights[i].vDir));
   float NdotL = dot(N, L);
   Out.Color = lights[i].vAmbient;
   Out.ColorSpec = 0;
   if(NdotL > 0.f)
   {
      //compute diffuse color
      Out.Color += NdotL * lights[i].vDiffuse;

      //add specular component
      if(bSpecular)
      {
         float3 H = normalize(L + V);   //half vector
         Out.ColorSpec = pow(max(0, dot(H, N)), fMaterialPower) * lights[i].vSpecular;
      }
   }
   return Out;
}

//-----------------------------------------------------------------------------
// Name: DoPointLight()
// Desc: Point light computation
//-----------------------------------------------------------------------------
COLOR_PAIR DoPointLight(float4 vPosition, float3 N, float3 V, int i)
{
   float3 L = mul((float3x3)matViewIT, normalize((lights[i].vPos-(float3)mul(matWorld,vPosition))));
   COLOR_PAIR Out;
   float NdotL = dot(N, L);
   Out.Color = lights[i].vAmbient;
   Out.ColorSpec = 0;
   float fAtten = 1.f;
   if(NdotL >= 0.f)
   {
      //compute diffuse color
      Out.Color += NdotL * lights[i].vDiffuse;

      //add specular component
      if(bSpecular)
      {
         float3 H = normalize(L + V);   //half vector
         Out.ColorSpec = pow(max(0, dot(H, N)), fMaterialPower) * lights[i].vSpecular;
      }

      float LD = length(lights[i].vPos-(float3)mul(matWorld,vPosition));
      if(LD > lights[i].fRange)
      {
         fAtten = 0.f;
      }
      else
      {
         fAtten *= 1.f/(lights[i].vAttenuation.x + lights[i].vAttenuation.y*LD + lights[i].vAttenuation.z*LD*LD);
      }
      Out.Color *= fAtten;
      Out.ColorSpec *= fAtten;
   }
   return Out;
}

//-----------------------------------------------------------------------------
// Name: DoSpotLight()
// Desc: Spot light computation
//-----------------------------------------------------------------------------
COLOR_PAIR DoSpotLight(float4 vPosition, float3 N, float3 V, int i)
{
   float3 L = mul((float3x3)matViewIT, normalize((lights[i].vPos-(float3)mul(matWorld,vPosition))));
   COLOR_PAIR Out;
   float NdotL = dot(N, L);
   Out.Color = lights[i].vAmbient;
   Out.ColorSpec = 0;
   float fAttenSpot = 1.f;
   if(NdotL >= 0.f)
   {
      //compute diffuse color
      Out.Color += NdotL * lights[i].vDiffuse;

      //add specular component
      if(bSpecular)
      {
         float3 H = normalize(L + V);   //half vector
         Out.ColorSpec = pow(max(0, dot(H, N)), fMaterialPower) * lights[i].vSpecular;
      }

      float LD = length(lights[i].vPos-(float3)mul(matWorld,vPosition));
      if(LD > lights[i].fRange)
      {
         fAttenSpot = 0.f;
      }
      else
      {
         fAttenSpot *= 1.f/(lights[i].vAttenuation.x + lights[i].vAttenuation.y*LD + lights[i].vAttenuation.z*LD*LD);
      }

      //spot cone computation
      float3 L2 = mul((float3x3)matViewIT, -normalize(lights[i].vDir));
      float rho = dot(L, L2);
      fAttenSpot *= pow(saturate((rho - lights[i].vSpot.y)/(lights[i].vSpot.x - lights[i].vSpot.y)), lights[i].vSpot.z);

      Out.Color *= fAttenSpot;
      Out.ColorSpec *= fAttenSpot;
   }
   return Out;
}

//-----------------------------------------------------------------------------
// Name: vs_main()
// Desc: The vertex shader
//-----------------------------------------------------------------------------
VS_OUTPUT vs_main (float4 vPosition  : POSITION0, 
                           float4 vPosition2 : POSITION1, 
                           float3 vNormal    : NORMAL0, 
                           float3 vNormal2   : NORMAL1, 
                           float2 tc         : TEXCOORD0)
{
   VS_OUTPUT Out = (VS_OUTPUT) 0;

   if(bTweening)
   {
      vPosition = (1.f-fTweenFactor) * vPosition          + fTweenFactor * vPosition2;
      vNormal   = (1.f-fTweenFactor) * normalize(vNormal) + fTweenFactor * normalize(vNormal2);
   }

   vNormal = normalize(vNormal);
   Out.Pos = mul(matWorldViewProj, vPosition);

   float3 P = mul(matWorldView, vPosition);           //position in view space
   float3 N = mul((float3x3)matWorldViewIT, vNormal); //normal in view space
   float3 V = -normalize(P);                          //viewer

   //automatic texture coordinate generation
   Out.Tex0 = float4((2.f * dot(V,N) * N - V) * (iTexGenType == TEXGEN_TYPE_CAMERASPACEREFLECTIONVECTOR)
            + N * (iTexGenType == TEXGEN_TYPE_CAMERASPACENORMAL)
            + P * (iTexGenType == TEXGEN_TYPE_CAMERASPACEPOSITION), 0);
   Out.Tex0.xy += tc * (iTexGenType == TEXGEN_TYPE_NONE);

   //light computation
   Out.Color = vAmbientColor;
   Out.ColorSpec = 0;

   //directional lights
   for(int i = 0; i < iLightDirNum; i++)
   {
      COLOR_PAIR ColOut = DoDirLight(N, V, i+iLightDirIni);
      Out.Color += ColOut.Color;
      Out.ColorSpec += ColOut.ColorSpec;
   }

   //point lights
   for(int i = 0; i < iLightPointNum; i++)
   {
      COLOR_PAIR ColOut = DoPointLight(vPosition, N, V, i+iLightPointIni);
      Out.Color += ColOut.Color;
      Out.ColorSpec += ColOut.ColorSpec;
   }

   //spot lights
   for(int i = 0; i < iLightSpotNum; i++)
   {
      COLOR_PAIR ColOut = DoSpotLight(vPosition, N, V, i+iLightSpotIni);
      Out.Color += ColOut.Color;
      Out.ColorSpec += ColOut.ColorSpec;
   }

   //apply material color
   Out.Color *= vMaterialColor;
   Out.ColorSpec *= vMaterialColor;

   //saturate
   Out.Color = min(1, Out.Color);
   Out.ColorSpec = min(1, Out.ColorSpec);

   //apply fog
   float d;
   if(bFogRange)
      d = length(P);
   else
      d = P.z;

   Out.Fog = 1.f * (iFogType == FOG_TYPE_NONE)
             + 1.f/exp(d * fFogDensity) * (iFogType == FOG_TYPE_EXP)
             + 1.f/exp(pow(d * fFogDensity, 2)) * (iFogType == FOG_TYPE_EXP2)
             + saturate((fFogEnd - d)/(fFogEnd - fFogStart)) * (iFogType == FOG_TYPE_LINEAR);

   return Out;
}

// Techniques

//the technique to set the state for the fixed function shader
technique basic
{
   pass P0
   {
      AMBIENT = (vAmbientColor);
      SPECULARENABLE = (bSpecular);
      FOGENABLE = (iFogType != FOG_TYPE_NONE);
      FOGCOLOR = (vFogColor);
   }
}

//the technique for the programmable shader (simply sets the vertex shader)
technique basic_with_shader
{
   pass P0
   {
      SPECULARENABLE = (bSpecular);
      FOGENABLE = (iFogType != FOG_TYPE_NONE);
      FOGCOLOR = (vFogColor);
      VertexShader = compile vs_2_0 vs_main();
   }
}

TEXTURE tex1;
TEXTURE tex2;

//Sampler for the diff mode
sampler DiffSampler1 = sampler_state
{
   Texture = (tex1);

   MinFilter = Point;
   MagFilter = Point;
   MipFilter = Point;
   AddressU  = Wrap;
   AddressV  = Wrap;
   AddressW  = Wrap;
   MaxAnisotropy = 8;
};

sampler DiffSampler2 = sampler_state
{
   Texture = (tex2);

   MinFilter = Point;
   MagFilter = Point;
   MipFilter = Point;
   AddressU  = Wrap;
   AddressV  = Wrap;
   AddressW  = Wrap;
   MaxAnisotropy = 8;
};

bool bDiffSensitivity = false;

//-----------------------------------------------------------------------------
// Name: ps_diff()
// Desc: Pixel shader for the diff mode
//       Tiny errors: green. Larger errors: yellow to red.
//-----------------------------------------------------------------------------
float4 ps_diff (float2 tcBase : TEXCOORD0) : COLOR
{
   float E = length(tex2D(DiffSampler1, tcBase) - tex2D(DiffSampler2, tcBase))/sqrt(3);
   float4 C = float4(0.f,0.f,0.f,E);
   
   if(E > 0.f)
   {
      if(E <= 1.f/255.f)
      {
         if(bDiffSensitivity)
         {
            C = float4(0.f,1.f,0.f,E);
         }
      }
      else
      {
         C = lerp(float4(1.f,1.f,0.f,E), float4(1.f,0.f,0.f,E),E);
      }
   }
   return C;
}

//technique for the diff mode
technique technique_diff
{
   pass P0
   {
      PixelShader = compile ps_2_0 ps_diff();
   }
}
