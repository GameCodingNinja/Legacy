
//-----------------------------------------------------------------------------
// #defines
//-----------------------------------------------------------------------------

// Make sure this matches in lightlst.h
#define MAX_LIGHTS          5  // 12 max for shader 2, more for shader 3

#define ELT_NONE            0
#define ELT_DIRECTIONAL     1
#define ELT_POINT_INFINITE  2
#define ELT_POINT_RADIUS    3
#define ELT_SPECULAR        4

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Lighting only matrix - rigid body transform
float4x4 worldMatrix;

// Pos only matrix
float4x4 worldViewProjMatrix;
texture material;

// Light related globals
bool enableLights;
int lightType[MAX_LIGHTS];
float4 lightPos[MAX_LIGHTS];
float4 lightDif[MAX_LIGHTS];
bool lightEnable[MAX_LIGHTS];
float lightRadius[MAX_LIGHTS];
float4 ambient;
int lightCount;

//-----------------------------------------------------------------------------
// Structor Definitions
//-----------------------------------------------------------------------------

struct VS_INPUT
{
	float4 pos	  : POSITION;
	float2 uv0    : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 pos    : POSITION;
	float2 uv0    : TEXCOORD0;
    float4 color  : COLOR0;
};

struct PS_OUTPUT
{
	float4 color : COLOR;
};

//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler textureSampler = 
sampler_state
{
    Texture = <material>;
};

//-----------------------------------------------------------------------------
// Point Radius light shader function
//-----------------------------------------------------------------------------
float4 CalcPointRadiusLight( int i, float4 vPos, float3 vNormal )
{
	float4 color = float4( 0.f, 0.f, 0.f, 0.f );
	
	// Light vector
	float3 lVec = (float3)(lightPos[i] - vPos);
	
	// Get light distance
	float dist = sqrt(length(lVec));
	
	float3 dir = (float3)normalize( lVec );
	float diffuse = max(0.0, dot(dir, vNormal));
	
	if( diffuse > 0.0f )
	{
		if( dist < lightRadius[i] )
		{
			color = float4( (float3)lightDif[i] * diffuse, 1.0 );
		}
		else
		{
			// Use (lightRadius[i] / dist) as a scaler against the diffuse
			color = float4( (float3)(lightDif[i] * diffuse) * (lightRadius[i] / dist), 1.0 );
		}
	}

	return color;
}

//-----------------------------------------------------------------------------
// Point Infinite light shader function
//-----------------------------------------------------------------------------
float4 CalcPointInfiniteLight( int i, float4 vPos, float3 vNormal )
{
	float4 color = float4( 0.f, 0.f, 0.f, 0.f );
	
	float3 dir = (float3)normalize( lightPos[i] - vPos );
	float diffuse = max(0.0, dot(dir, vNormal));
	
	if( diffuse > 0.0f )
	{
		color = float4( (float3)lightDif[i] * diffuse, 1.0 );
	}

	return color;
}

//-----------------------------------------------------------------------------
// Directional light shader function
//-----------------------------------------------------------------------------
float4 CalcDirLight( int i, float3 vNormal )
{
	float4 color = float4( 0.f, 0.f, 0.f, 0.f );
	float diffuse = max(0.0, dot(lightPos[i], vNormal));
	
	if( diffuse > 0.0f )
	{
		color = float4( (float3)lightDif[i] * diffuse, 1.0 );
	}
	
	return color;
}

//-----------------------------------------------------------------------------
// Directional light shader function
//-----------------------------------------------------------------------------
float4 CalcLight( int i, float4 vPos, float3 vNormal )
{
	float4 color = float4( 0.f, 0.f, 0.f, 0.0 );
	
	if( lightEnable[i] )
	{
		if( lightType[i] == ELT_POINT_INFINITE )
		{
			color = CalcPointInfiniteLight( i, vPos, vNormal );
		}
		else if( lightType[i] == ELT_POINT_RADIUS )
		{
			color = CalcPointRadiusLight( i, vPos, vNormal );
		}
		else if( lightType[i] == ELT_DIRECTIONAL )
		{
			color = CalcDirLight( i, vNormal );
		}
	}
	
	return color;
}

//-----------------------------------------------------------------------------
// Main vertex shader function
//-----------------------------------------------------------------------------
VS_OUTPUT v_shader( VS_INPUT IN )
{
    VS_OUTPUT OUT;

	// Add the ambient
	OUT.color = ambient;
	
	if( enableLights )
	{
		float4 vPos = mul( IN.pos, worldMatrix );
		float3 vNor = mul( IN.normal, (float3x3)worldMatrix );
		
		for( int i = 0; i < lightCount; i++ )
		{
			OUT.color += CalcLight( i, vPos, vNor );
		}
		
		//OUT.color += CalcLight( 0, vPos, vNor );
		//OUT.color += CalcLight( 1, vPos, vNor );
		//OUT.color += CalcLight( 2, vPos, vNor );
		//OUT.color += CalcLight( 3, vPos, vNor );
		//OUT.color += CalcLight( 4, vPos, vNor );
	}
	 
	OUT.pos = mul( IN.pos, worldViewProjMatrix );

	OUT.uv0 = IN.uv0;
	OUT.color = min(1, OUT.color);

	return OUT;
}

//-----------------------------------------------------------------------------
// Main pixel shader function
//-----------------------------------------------------------------------------
PS_OUTPUT p_shader( VS_OUTPUT IN )
{
    PS_OUTPUT OUT;

	OUT.color = tex2D( textureSampler, IN.uv0 ) * IN.color;

    return OUT;
}

//-----------------------------------------------------------------------------
// Effects
//-----------------------------------------------------------------------------

technique effect0
{
    pass Pass0
    {
		VertexShader = compile vs_3_0 v_shader();
		PixelShader  = compile ps_3_0 p_shader();
    }
}

