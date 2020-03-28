//-----------------------------------------------------------------------------
// #defines
//-----------------------------------------------------------------------------

static const int BLUR_SAMPLE_SIZE = 46;
static const float3 LUMINANCE_CONV = { 0.2125f, 0.7154f, 0.0721f };
static const float PI = 3.14159265358979323846;


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Texture - shadow map
texture postProcessTexture;

// Amount being sampled
int blurSampleAmount;

// Blur inc and total weight
float weightInc;
float totalWeight;
float weightVariance;

// Width and height of buffer
float bufferWidth;
float bufferHeight;

// Color drain variables
float colorDrainDominate = 1;
float colorDrainSubmissive;


//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------

sampler postProcessTextSampler = 
sampler_state
{
    Texture = <postProcessTexture>;
	AddressU = Clamp;
    AddressV = Clamp;
};


//-----------------------------------------------------------------------------
// pixel compute weights for bell curve
//
// param: weightIndex - index of weight
//-----------------------------------------------------------------------------
float GenerateWeight( int weightIndex )
{
	float x = weightInc * (float)weightIndex;
	float weight = (float)(1 / sqrt(2 * PI * weightVariance)) * exp( -(x)/(2 * weightVariance));

	return weight / totalWeight;

}	// GenerateWeight


//-----------------------------------------------------------------------------
// Null shader. Nothing happens
//-----------------------------------------------------------------------------
float4 p_null( float2 uv0 : TEXCOORD0 ) : COLOR0
{
	return tex2D( postProcessTextSampler, uv0 );
}


//-----------------------------------------------------------------------------
// Convert to monochrome
//-----------------------------------------------------------------------------
float4 p_convertToMonochrome( float2 uv0 : TEXCOORD0 ) : COLOR0
{
	float4 color = dot( (float3)tex2D( postProcessTextSampler, uv0 ), LUMINANCE_CONV );
	color.a = 1.f;
 
    return color;
}


//-----------------------------------------------------------------------------
// Convert to Sepia Tone
//-----------------------------------------------------------------------------
float4 p_convertToSepiaTone( float2 uv0 : TEXCOORD0 ) : COLOR0
{
    float4 color = tex2D(postProcessTextSampler, uv0);
 
	float4 outputColor;
    outputColor.r = (color.r * 0.393) + (color.g * 0.769) + (color.b * 0.189);
    outputColor.g = (color.r * 0.349) + (color.g * 0.686) + (color.b * 0.168);    
    outputColor.b = (color.r * 0.272) + (color.g * 0.534) + (color.b * 0.131);
	outputColor.a = 1.f;
 
    return outputColor;
}


//-----------------------------------------------------------------------------
// Horizontal blur pass
//-----------------------------------------------------------------------------
float4 p_blurH( float2 uv0 : TEXCOORD0 ) : COLOR0
{
	float weight = GenerateWeight( 0 );
    float4 color = tex2D( postProcessTextSampler, uv0 ) * weight;
	
    for( int i = 0; i < blurSampleAmount; i++ )
	{
		weight = GenerateWeight( i+1 );
		float2 offset = float2((float)(i+1)/bufferWidth, 0.0);
		
        color += tex2D( postProcessTextSampler, uv0 + offset ) * weight;
		color += tex2D( postProcessTextSampler, uv0 + -offset ) * weight;
	}

	color.a = 1.f;
    
    return color;
}


//-----------------------------------------------------------------------------
// Verticle blur pass
//-----------------------------------------------------------------------------
float4 p_blurV( float2 uv0 : TEXCOORD0 ) : COLOR0
{
    float weight = GenerateWeight( 0 );
    float4 color = tex2D( postProcessTextSampler, uv0 ) * weight;

	int counter = 0;

    for( int i = 0; i < blurSampleAmount; i++ )
	{
		weight = GenerateWeight( i+1 );
		float2 offset = float2(0.0,(float)(i+1)/bufferHeight);
        color += tex2D( postProcessTextSampler, uv0 + offset ) * weight;
		color += tex2D( postProcessTextSampler, uv0 + -offset ) * weight;
	}

	color.a = 1.f;
    
    return color;
}


//-----------------------------------------------------------------------------
// Verticle blur pass with color drain
//-----------------------------------------------------------------------------
float4 p_blurV_colorDrain( float2 uv0 : TEXCOORD0 ) : COLOR0
{
    float weight = GenerateWeight( 0 );
    float4 color = tex2D( postProcessTextSampler, uv0 ) * weight;

    for( int i = 0; i < blurSampleAmount; i++ )
	{
		weight = GenerateWeight( i+1 );
		float2 offset = float2(0.0,(float)(i+1)/bufferHeight);
        color += tex2D( postProcessTextSampler, uv0 + offset ) * weight;
		color += tex2D( postProcessTextSampler, uv0 + (-offset) ) * weight;
	}

	color.a = 1.f;
	color.r = (color.r * colorDrainDominate) + (color.g * colorDrainSubmissive) + (color.b * colorDrainSubmissive);
    color.g = (color.r * colorDrainSubmissive) + (color.g * colorDrainDominate) + (color.b * colorDrainSubmissive);    
    color.b = (color.r * colorDrainSubmissive) + (color.g * colorDrainSubmissive) + (color.b * colorDrainDominate);
    
    return color;
}


//-----------------------------------------------------------------------------
// Produces a degree of color to grey
//-----------------------------------------------------------------------------
float4 p_colorDrain( float2 uv0 : TEXCOORD0 ) : COLOR0
{
    float4 color = tex2D( postProcessTextSampler, uv0 );

	float4 outputColor;
    outputColor.r = (color.r * colorDrainDominate) + (color.g * colorDrainSubmissive) + (color.b * colorDrainSubmissive);
    outputColor.g = (color.r * colorDrainSubmissive) + (color.g * colorDrainDominate) + (color.b * colorDrainSubmissive);    
    outputColor.b = (color.r * colorDrainSubmissive) + (color.g * colorDrainSubmissive) + (color.b * colorDrainDominate);
	outputColor.a = 1.f;
 
    return outputColor;
}


//-----------------------------------------------------------------------------
// Effects
//-----------------------------------------------------------------------------

technique nullEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_null();

		ZEnable = false;
		AlphaBlendEnable = false;
    }
}

technique convertToMonochrome
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_convertToMonochrome();

		ZEnable = false;
		AlphaBlendEnable = false;
    }
}

technique convertToSepiaTone
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_convertToSepiaTone();

		ZEnable = false;
		AlphaBlendEnable = false;
    }
}

technique gaussianBlur
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_blurH();

		ZEnable = false;
		AlphaBlendEnable = false;
    }

	pass p1
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_blurV();

		ZEnable = false;
		AlphaBlendEnable = false;
    }
}

technique gaussianBlurColorDrain
{
	pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_blurH();

		ZEnable = false;
		AlphaBlendEnable = false;
    }

	pass p1
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_blurV();

		ZEnable = false;
		AlphaBlendEnable = false;
    }

    pass p2
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_blurH();

		ZEnable = false;
		AlphaBlendEnable = false;
    }

	pass p3
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 p_blurV_colorDrain();

		ZEnable = false;
		AlphaBlendEnable = false;
    }
}


