
#if HAS_OMNIMAP_MULTISAMPLE_9X
#define HAS_OMNIMAP_MULTISAMPLE 1
#endif

#if HAS_OMNIMAP_MULTISAMPLE_9X

#ifdef FILTER_SCALE
float samplelength = (FILTER_SCALE);
#else
float samplelength = 1.0;
#endif


half4 MultiSampleHalf(sampler2D  tex , half2 UV){
	half samplelengthScaled = samplelength *0.75;
	half3 offset = half3(-.5,0,.5);
	half2 offsetNormalized = half2(-0.70710678118*.5,0.70710678118*.5);
	// to do switch to 4x rotated square pattern
	
	offset = offset*samplelength;
	offsetNormalized = offsetNormalized*samplelengthScaled;	
	
	
	half2 DUV = fwidth(UV);
	half4 mainColor = (
		tex2D(tex, UV + DUV *offsetNormalized.xy ) +
		tex2D(tex, UV + DUV *offset.yz) +
		tex2D(tex, UV + DUV *offsetNormalized.yy) +
					
		tex2D(tex, UV + DUV *offset.xy) +
		tex2D(tex, UV)+
		tex2D(tex, UV + DUV *offset.zy) +
					
		tex2D(tex, UV + DUV *offsetNormalized.xx)+
		tex2D(tex, UV + DUV *offset.yx)+
		tex2D(tex, UV + DUV *offsetNormalized.yx) 
	)*.111111111111111;
	return mainColor;
}
float4 MultiSampleFloat(sampler2D  tex , float2 UV){
	float samplelengthScaled = samplelength *0.75;
	float3 offset = half3(-.5,0,.5);
	float2 offsetNormalized = half2(-0.70710678118*.5,0.70710678118*.5);
	// to do switch to 4x rotated square pattern
	
	offset = offset*samplelength;
	offsetNormalized = offsetNormalized*samplelengthScaled;	
	
	float2 DUV = fwidth(UV);
	float4 mainColor = (
		tex2D(tex, UV + DUV *offsetNormalized.xy ) +
		tex2D(tex, UV + DUV *offset.yz) +
		tex2D(tex, UV + DUV *offsetNormalized.yy) +
					
		tex2D(tex, UV + DUV *offset.xy) +
		tex2D(tex, UV)+
		tex2D(tex, UV + DUV *offset.zy) +
					
		tex2D(tex, UV + DUV *offsetNormalized.xx)+
		tex2D(tex, UV + DUV *offset.yx)+
		tex2D(tex, UV + DUV *offsetNormalized.yx) 
	)*.111111111111111;
	
	return mainColor;
}
#endif


#if HAS_OMNIMAP_MULTISAMPLE
	#define MULTISAMPLE_Hin_Hout(t,uv) MultiSampleHalf((t),(uv))
	#define MULTISAMPLE_Fin_Fout(t,uv) MultiSampleFloat((t),(uv))
#else
	#define MULTISAMPLE_Hin_Hout(t,uv) tex2D((t),(uv))
	#define MULTISAMPLE_Fin_Fout(t,uv) tex2D((t),(uv))
#endif

//////////////////////////////////////////////
////////////  MULTISAMPLE END ////////////////
//////////////////////////////////////////////



//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 g_mProjection : VIEWPROJECTION;    // Projection matrix
float4x4 g_mWorldView : WORLD;    // World * View


float yOffset1;		// Lens offset Values
float yOffset2;
float yScale;		

texture  channelTexture0;
texture  channelTexture1;
texture  channelTexture2;
texture  channelTexture3;
texture  channelTexture4;
texture  channelTexture5;
texture alphaMaskText;

sampler2D alphMaskSampler =
sampler_state
{
    Texture = <alphaMaskText>;
    magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};


sampler Samplers[6] = {
 sampler_state
 {
  texture = <channelTexture0>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },

 sampler_state
 {
  texture = <channelTexture1>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture2>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture3>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture4>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture5>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 }
 };



float4x4    textMatrixArray[NumChannels];

struct VS_OUTPUT
{
	float4 pos : POSITION;					// Screen space position of vertex
	float2 PixelPos : COLOR1;				// Pixel position for the alpha mask
	float4 texCoordArray[NumChannels] : TEXCOORD0;		// Texture coordinates for each channel
};


VS_OUTPUT OmniMapVS(float4 vPos : POSITION)
{

	VS_OUTPUT Output;
	float R;
	//Output.VertexPosition_WorldSpace = vPos;


	
	float4x4 tmp = mul(g_mWorldView, g_mProjection);
	float4 VertexPosition_projectorSpace_Goal = mul(vPos, tmp);

	// F-Theta Warping Code
	float Z = VertexPosition_projectorSpace_Goal.z;
	float D = length(VertexPosition_projectorSpace_Goal.xyz);
	
	// Radius at this vertex
	R = (2.0/3.14159265) * acos(Z/D);  // Ftheta.
	

	// Converting the ftheta lenses image to !theta image to account for fisheye lens warping
	R =  0.2798*R *R *R *R *R *R  - 0.6875*R *R *R *R *R  + 0.7351*R *R *R *R  - 0.3472*R *R *R  + 0.0977*R *R  + 0.9221*R ;  

	float l = 1.0/ (length(VertexPosition_projectorSpace_Goal.xy)+.0000000000000000000000000000000000001);// bug fix  1
	
	// Direction vector for this vertex, in screen space, from the center of the lens.
	float2 thetavec = VertexPosition_projectorSpace_Goal.xy * float2(l,l); 
	
	// Calculate screen pixel coordinates.
	Output.pos.xy= thetavec*R;
	
	// Offset the lens.  Accommodates shifting of the lens vertically.
	// In OpenGL, this is done by simply offsetting the viewport, but
	// D3D does not like viewports that are off the screen, so we do it here
	// in the shader.
	Output.pos.y = (Output.pos.y + yOffset1);
	Output.pos.y *= yScale;
	Output.pos.y = (Output.pos.y + yOffset2);
	
	Output.pos.w =1.0;
	Output.pos.z =D/1000.0;
	//Output.VertexPosition_WorldSpace.w = 1.0;
	
	
	// Screen space -> Texture map coordinate for stencil mask.
	Output.PixelPos.xy = Output.pos.xy * float2(.5,-.5) + float2(.5 , .5);

	// Apply each channel's texture projection matrix to the vertex.
	for (int i = 0; i < NumChannels; i++)
	{
		Output.texCoordArray[i] = mul(vPos, textMatrixArray[i]);	
	}
		
	return(Output);
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};



PS_OUTPUT OmniMapFS(VS_OUTPUT In)
{
	PS_OUTPUT Output;
	float x,y;
	
	// Set the fragment color to black in case this position on the
	// screen is not intersected by any projected channel
	Output.RGBColor = float4(0.0, 0.0, 0.0, 1.0);
	for(int i =0; i < NumChannels;i++)	
	{
		// Calculate the texture coordinate for the i'th channel
		// texture
		float3 tc;
		tc.xy = In.texCoordArray[i].xy / In.texCoordArray[i].w;
		tc.y = 1.0f - tc.y;
		tc.z = In.texCoordArray[i].w;
		
		// If the texture coordinate is in the range [0,1] for both u and v, 
		// and the z value is > 1, this is the channel to use for projecting to
		// this fragment.
		if (tc.x >= 0.0 && tc.x <= 1.0 && tc.y >= 0.0 && tc.y <= 1.0 && tc.z > 0.0)
		{
			// Set the output color
			Output.RGBColor = float4(MULTISAMPLE_Fin_Fout(Samplers[i], tc.xy).xyz, 1.0);
		}

	}
	
	// If the stencil mask is enabled, apply the stencil mask color
	// The stencil mask is a screen space mask
	if (stencilMaskEnabled)
		Output.RGBColor.rgb = Output.RGBColor.rgb * tex2D(alphMaskSampler,In.PixelPos.xy).rgb;
	return(Output);
}



technique RenderScene
{
    pass P0
    {
        VertexShader = compile vs_3_0 OmniMapVS();
        PixelShader  = compile ps_3_0 OmniMapFS();
    }
}
