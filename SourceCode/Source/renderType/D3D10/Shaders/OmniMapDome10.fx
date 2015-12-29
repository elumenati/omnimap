
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
float4x4 g_mProjection;    // Projection matrix
float4x4 g_mWorldView;    // World * View

float3 userposition;
float yOffset1;
float yOffset2;
float yScale;

Texture2D  channelTexture0;
Texture2D  channelTexture1;
Texture2D  channelTexture2;
Texture2D  channelTexture3;
Texture2D  channelTexture4;
Texture2D  channelTexture5;
Texture2D alphaMaskText;
Texture2D hudTexture;

SamplerState alphMaskSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState hudSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};


sampler Samplers[6] = {
 sampler_state
 {
  texture = <channelTexture0>;
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },

 sampler_state
 {
  texture = <channelTexture1>;
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture2>;
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture3>;
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture4>;
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 },
 sampler_state
 {
  texture = <channelTexture5>;
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Clamp;
  AddressV = Clamp;
 }
 };


float4x4    textMatrixArray[6];



struct HUDVS_IN
{
    float3 vPos          : POSITION;         
    float2 vTexCoord0          : TEXTURE;          
};

struct HUDVS_OUTPUT
{
    float4 Position   : SV_POSITION; // vertex position 
    float4 Diffuse    : COLOR0;      // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;   // vertex texture coords 
};
//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
HUDVS_OUTPUT RenderHUDVS( HUDVS_IN input,
                         uniform bool bTexture )
{
    HUDVS_OUTPUT Output; 
    // Transform the position from object space to homogeneous projection space
    Output.Position.xyz = mul(input.vPos, g_mProjection);
    Output.Position.w = 1.0;
   
    Output.Diffuse.rgb = float3(1.0,1.0,1.0);
    Output.Diffuse.a = 1.0f; 
    
    // Just copy the texture coordinate through
    if( bTexture ) 
        Output.TextureUV = input.vTexCoord0; 
    else
        Output.TextureUV = 0; 
    
    return Output;    
}

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct HUDPS_OUTPUT
{
    float4 RGBColor : SV_Target;  // Pixel color
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
HUDPS_OUTPUT RenderHUDPS( HUDVS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    HUDPS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    if( bTexture )
        Output.RGBColor = hudTexture.Sample(hudSampler, In.TextureUV) * In.Diffuse;
    else
        Output.RGBColor = In.Diffuse;
	//Output.RGBColor = float4(1.0,1.0,1.0,1.0);
    return Output;
}



struct VS_INPUT
{
	float3 vPos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 VertexPosition_WorldSpace : COLOR0;
	float2 PixelPos : COLOR1;
	float4 TexCoord0 : TEXCOORD0;
	float4 TexCoord1 : TEXCOORD1;
	float4 TexCoord2 : TEXCOORD2;
	float4 TexCoord3 : TEXCOORD3;
	float4 TexCoord4 : TEXCOORD4;
	float4 TexCoord5 : TEXCOORD5;
};


VS_OUTPUT OmniMapVS(VS_INPUT In)
{
// vertex shader has two goals...
// (1) pass worldspace coordinates to fragment shader
// (2) set projector space coordinates as gl position
// (2a) orthographic projection
// (2b) ftheta projection
// (2c) Normal Projector

// (1)
	VS_OUTPUT Output;
	float R;
	float4 vPos = float4(In.vPos, 1.0);
	Output.VertexPosition_WorldSpace.xyz = vPos;



	if(false)// (2) a
	{
		Output.pos = mul(vPos, g_mProjection);	//Position of vertex in projector space
		Output.VertexPosition_WorldSpace.w = Output.pos.w;
	}
	else// (2) b
	{
		float4x4 tmp = mul(g_mWorldView, g_mProjection);
		float4 VertexPosition_projectorSpace_Goal = mul(vPos, tmp);

		// F-Theta Warping Code
		float Z = VertexPosition_projectorSpace_Goal.z;
		float D = length(VertexPosition_projectorSpace_Goal.xyz);


		float MyPI = 3.14159265;  
		R = (2.0/MyPI) * acos(Z/D);  // Ftheta.
		
	
		/// this line is for converting the ftheta lenses image to !theta image
		R =  0.2798*R *R *R *R *R *R  - 0.6875*R *R *R *R *R  + 0.7351*R *R *R *R  - 0.3472*R *R *R  + 0.0977*R *R  + 0.9221*R ;  
 
	
		float l = 1.0/ (length(VertexPosition_projectorSpace_Goal.xy)+.0000000000000000000000000000000000001);// bug fix  1
		float2 thetavec = VertexPosition_projectorSpace_Goal.xy * float2(l,l); // bug fix  1
		
		
		
		Output.pos.xy= thetavec*R;
		Output.pos.y = (Output.pos.y + yOffset1);
		Output.pos.y *= yScale;
		Output.pos.y = (Output.pos.y + yOffset2);
		
		Output.pos.w =1.0;
		Output.pos.z =D/1000.0;
		Output.VertexPosition_WorldSpace.w = 1.0;
		
		Output.PixelPos.xy = Output.pos.xy * float2(.5,-.5) + float2(.5 , .5);
	}
	
	Output.TexCoord0 = float4(0.0,0.0,0.0,1.0);
	Output.TexCoord1 = float4(0.0,0.0,0.0,1.0);
	Output.TexCoord2 = float4(0.0,0.0,0.0,1.0);
	Output.TexCoord3 = float4(0.0,0.0,0.0,1.0);
	Output.TexCoord4 = float4(0.0,0.0,0.0,1.0);
	Output.TexCoord5 = float4(0.0,0.0,0.0,1.0);
	
	for (int i = 0; i < NumChannels; i++)
	{
		//Output.TexCoordArr[i] = mul(vPos, textMatrixArray[i]);
		if (i == 0)
			Output.TexCoord0 = mul(vPos, textMatrixArray[i]);
		else if (i == 1)
			Output.TexCoord1 = mul(vPos, textMatrixArray[i]);
		else if (i == 2)
			Output.TexCoord2 = mul(vPos, textMatrixArray[i]);
		else if (i == 3)
			Output.TexCoord3 = mul(vPos, textMatrixArray[i]);
		else if (i == 4)
			Output.TexCoord4 = mul(vPos, textMatrixArray[i]);
		else if (i == 5)
			Output.TexCoord5 = mul(vPos, textMatrixArray[i]);
	}
		
	return(Output);
}
//sampler channelSamplers[NumChannels];

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : SV_Target;  // Pixel color    
};


Texture2D theTexture;
PS_OUTPUT OmniMapFS(VS_OUTPUT In)
{
	PS_OUTPUT Output;
	float x,y;
	
	
	Output.RGBColor = float4(0.0, 0.0, 0.0, 1.0);
	for(int i =0; i < NumChannels;i++)	
	{
			float3 tc = float3(0.0,0.0,0.0);
			//tc.xy = In.TexCoordArr[i].xy / In.TexCoordArr[i].w;
			//tc.y = 1.0f - tc.y;
			//tc.z = In.TexCoordArr[i].w;

			if (i == 0)
			{
				//tc.xy = .5 * In.TexCoord0.xy / In.TexCoord0.w + float2( 0.5, 0.5 );
				tc.xy = In.TexCoord0.xy / In.TexCoord0.w;
				tc.y = 1.0f - tc.y;
				tc.z = In.TexCoord0.w;
				//Output.RGBColor = float4(In.TexCoord0.x,In.TexCoord0.y,0.0,1.0);
				
			} else if (i == 1)
			{
				//tc.xy = 0.5 * In.TexCoord1.xy / In.TexCoord1.w + float2( 0.5, 0.5 );
				tc.xy = In.TexCoord1.xy / In.TexCoord1.w;
				tc.y = 1.0f - tc.y;
				tc.z = In.TexCoord1.w;
				//Output.RGBColor = float4(In.TexCoord1.x,In.TexCoord1.y,0.0,1.0);
				
			} else if (i == 2)
			{
				//tc.xy = 0.5 * In.TexCoord2.xy / In.TexCoord2.w + float2( 0.5, 0.5 );
				tc.xy = In.TexCoord2.xy / In.TexCoord2.w;
				tc.y = 1.0f - tc.y;
				tc.z = In.TexCoord2.w;
				//Output.RGBColor = float4(In.TexCoord2.x,In.TexCoord2.y,0.0,1.0);
				
			} else if (i == 3)
			{
				//tc.xy = 0.5 * In.TexCoord3.xy / In.TexCoord3.w + float2( 0.5, 0.5 );
				tc.xy = In.TexCoord3.xy / In.TexCoord3.w;
				tc.y = 1.0f - tc.y;
				tc.z = In.TexCoord3.w;
			} else if (i == 4)
			{
				//tc.xy = 0.5 * In.TexCoord4.xy / In.TexCoord4.w + float2( 0.5, 0.5 );
				tc.xy = In.TexCoord4.xy / In.TexCoord4.w;
				tc.y = 1.0f - tc.y;
				tc.z = In.TexCoord4.w;
			} else if (i == 5)
			{
				//tc.xy = 0.5 * In.TexCoord5.xy / In.TexCoord5.w + float2( 0.5, 0.5 );
				tc.xy = In.TexCoord5.xy / In.TexCoord5.w;
				tc.y = 1.0f - tc.y;
				tc.z = In.TexCoord5.w;
			}
			
			
			if (tc.x >= 0.0 && tc.x <= 1.0 && tc.y >= 0.0 && tc.y <= 1.0 && tc.z > 0.0)
			{
				Output.RGBColor = float4(MULTISAMPLE_Fin_Fout(Samplers[i], tc.xy).xyz, 1.0);
				//if (i == 0)
					//Output.RGBColor = float4(channelTexture0.Sample(hudSampler, tc.xy).xyz, 1.0);
				//else if (i == 1)
					//Output.RGBColor = float4(channelTexture1.Sample(hudSampler, tc.xy).xyz, 1.0);
				//else if (i == 2)
					//Output.RGBColor = float4(channelTexture2.Sample(hudSampler, tc.xy).xyz, 1.0);
			}

	}
	if (stencilMaskEnabled)
		Output.RGBColor.rgb = Output.RGBColor.rgb * alphaMaskText.Sample(hudSampler, In.PixelPos.xy);
		//Output.RGBColor.rgb = Output.RGBColor.rgb * tex2D(alphMaskSampler,In.PixelPos.xy).rgb;
		//Output.RGBColor.rgb = Output.RGBColor.rgb * tex2D(alphMaskSampler,In.PixelPos.xy).rgb;
	return(Output);
}



technique10 RenderScene
{
    pass P0
    {
		SetVertexShader( CompileShader( vs_4_0, OmniMapVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, OmniMapFS() ) );
       
    }
}

//--------------------------------------------------------------------------------------
// Renders scene to render target using D3D10 Techniques
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// DepthStates
//--------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};
technique10 RenderHUDWithTexture
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, RenderHUDVS(true ) ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, RenderHUDPS(true) ) );

        SetDepthStencilState( DisableDepth, 0 );
    }
}

//--------------------------------------------------------------------------------------
// Renders scene to render target using D3D10 Techniques
//--------------------------------------------------------------------------------------
technique RenderHUDWithoutTexture
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_3_0, RenderHUDVS(false ) ) );
        //SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_3_0, RenderHUDPS(false) ) );

        //SetDepthStencilState( EnableDepth, 0 );
    }
}
