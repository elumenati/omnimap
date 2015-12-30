static char effectOmniMapDome11[] = { 
"#if HAS_OMNIMAP_MULTISAMPLE_9X\n" 
"#define HAS_OMNIMAP_MULTISAMPLE 1\n" 
"#endif\n" 
"#if HAS_OMNIMAP_MULTISAMPLE_9X\n" 
"#ifdef FILTER_SCALE\n" 
"float samplelength = (FILTER_SCALE);\n" 
"#else\n" 
"float samplelength = 1.0;\n" 
"#endif\n" 
"half4 MultiSampleHalf(sampler2D  tex , half2 UV){\n" 
"	half samplelengthScaled = samplelength *0.75;\n" 
"	half3 offset = half3(-.5,0,.5);\n" 
"	half2 offsetNormalized = half2(-0.70710678118*.5,0.70710678118*.5);\n" 
"	// to do switch to 4x rotated square pattern\n" 
"	\n" 
"	offset = offset*samplelength;\n" 
"	offsetNormalized = offsetNormalized*samplelengthScaled;	\n" 
"	\n" 
"	\n" 
"	half2 DUV = fwidth(UV);\n" 
"	half4 mainColor = (\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.xy ) +\n" 
"		tex2D(tex, UV + DUV *offset.yz) +\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.yy) +\n" 
"					\n" 
"		tex2D(tex, UV + DUV *offset.xy) +\n" 
"		tex2D(tex, UV)+\n" 
"		tex2D(tex, UV + DUV *offset.zy) +\n" 
"					\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.xx)+\n" 
"		tex2D(tex, UV + DUV *offset.yx)+\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.yx) \n" 
"	)*.111111111111111;\n" 
"	return mainColor;\n" 
"}\n" 
"float4 MultiSampleFloat(sampler2D  tex , float2 UV){\n" 
"	float samplelengthScaled = samplelength *0.75;\n" 
"	float3 offset = half3(-.5,0,.5);\n" 
"	float2 offsetNormalized = half2(-0.70710678118*.5,0.70710678118*.5);\n" 
"	// to do switch to 4x rotated square pattern\n" 
"	\n" 
"	offset = offset*samplelength;\n" 
"	offsetNormalized = offsetNormalized*samplelengthScaled;	\n" 
"	\n" 
"	float2 DUV = fwidth(UV);\n" 
"	float4 mainColor = (\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.xy ) +\n" 
"		tex2D(tex, UV + DUV *offset.yz) +\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.yy) +\n" 
"					\n" 
"		tex2D(tex, UV + DUV *offset.xy) +\n" 
"		tex2D(tex, UV)+\n" 
"		tex2D(tex, UV + DUV *offset.zy) +\n" 
"					\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.xx)+\n" 
"		tex2D(tex, UV + DUV *offset.yx)+\n" 
"		tex2D(tex, UV + DUV *offsetNormalized.yx) \n" 
"	)*.111111111111111;\n" 
"	\n" 
"	return mainColor;\n" 
"}\n" 
"#endif\n" 
"#if HAS_OMNIMAP_MULTISAMPLE\n" 
"	#define MULTISAMPLE_Hin_Hout(t,uv) MultiSampleHalf((t),(uv))\n" 
"	#define MULTISAMPLE_Fin_Fout(t,uv) MultiSampleFloat((t),(uv))\n" 
"#else\n" 
"	#define MULTISAMPLE_Hin_Hout(t,uv) tex2D((t),(uv))\n" 
"	#define MULTISAMPLE_Fin_Fout(t,uv) tex2D((t),(uv))\n" 
"#endif\n" 
"//////////////////////////////////////////////\n" 
"////////////  MULTISAMPLE END ////////////////\n" 
"//////////////////////////////////////////////\n" 
"//--------------------------------------------------------------------------------------\n" 
"// Global variables\n" 
"//--------------------------------------------------------------------------------------\n" 
"float4x4 g_mProjection;    // Projection matrix\n" 
"float4x4 g_mWorldView;    // World * View\n" 
"float3 userposition;\n" 
"float yOffset1;\n" 
"float yOffset2;\n" 
"float yScale;\n" 
"Texture2D  channelTexture0;\n" 
"Texture2D  channelTexture1;\n" 
"Texture2D  channelTexture2;\n" 
"Texture2D  channelTexture3;\n" 
"Texture2D  channelTexture4;\n" 
"Texture2D  channelTexture5;\n" 
"Texture2D alphaMaskText;\n" 
"Texture2D hudTexture;\n" 
"SamplerState alphMaskSampler\n" 
"{\n" 
"    Filter = MIN_MAG_MIP_LINEAR;\n" 
"    AddressU = Clamp;\n" 
"    AddressV = Clamp;\n" 
"};\n" 
"SamplerState hudSampler\n" 
"{\n" 
"    Filter = MIN_MAG_MIP_LINEAR;\n" 
"    AddressU = Clamp;\n" 
"    AddressV = Clamp;\n" 
"};\n" 
"sampler Samplers[6] = {\n" 
" sampler_state\n" 
" {\n" 
"  texture = <channelTexture0>;\n" 
"  Filter = MIN_MAG_MIP_LINEAR;\n" 
"  AddressU = Clamp;\n" 
"  AddressV = Clamp;\n" 
" },\n" 
" sampler_state\n" 
" {\n" 
"  texture = <channelTexture1>;\n" 
"  Filter = MIN_MAG_MIP_LINEAR;\n" 
"  AddressU = Clamp;\n" 
"  AddressV = Clamp;\n" 
" },\n" 
" sampler_state\n" 
" {\n" 
"  texture = <channelTexture2>;\n" 
"  Filter = MIN_MAG_MIP_LINEAR;\n" 
"  AddressU = Clamp;\n" 
"  AddressV = Clamp;\n" 
" },\n" 
" sampler_state\n" 
" {\n" 
"  texture = <channelTexture3>;\n" 
"  Filter = MIN_MAG_MIP_LINEAR;\n" 
"  AddressU = Clamp;\n" 
"  AddressV = Clamp;\n" 
" },\n" 
" sampler_state\n" 
" {\n" 
"  texture = <channelTexture4>;\n" 
"  Filter = MIN_MAG_MIP_LINEAR;\n" 
"  AddressU = Clamp;\n" 
"  AddressV = Clamp;\n" 
" },\n" 
" sampler_state\n" 
" {\n" 
"  texture = <channelTexture5>;\n" 
"  Filter = MIN_MAG_MIP_LINEAR;\n" 
"  AddressU = Clamp;\n" 
"  AddressV = Clamp;\n" 
" }\n" 
" };\n" 
"float4x4    textMatrixArray[6];\n" 
"struct HUDVS_IN\n" 
"{\n" 
"    float3 vPos          : POSITION;         \n" 
"    float2 vTexCoord0          : TEXTURE;          \n" 
"};\n" 
"struct HUDVS_OUTPUT\n" 
"{\n" 
"    float4 Position   : SV_POSITION; // vertex position \n" 
"    float4 Diffuse    : COLOR0;      // vertex diffuse color (note that COLOR0 is clamped from 0..1)\n" 
"    float2 TextureUV  : TEXCOORD0;   // vertex texture coords \n" 
"};\n" 
"//--------------------------------------------------------------------------------------\n" 
"// This shader computes standard transform\n" 
"//--------------------------------------------------------------------------------------\n" 
"HUDVS_OUTPUT RenderHUDVS( HUDVS_IN input,\n" 
"                         uniform bool bTexture )\n" 
"{\n" 
"    HUDVS_OUTPUT Output; \n" 
"    // Transform the position from object space to homogeneous projection space\n" 
"    Output.Position.xyz = mul(input.vPos, g_mProjection);\n" 
"    Output.Position.w = 1.0;\n" 
"   \n" 
"    Output.Diffuse.rgb = float3(1.0,1.0,1.0);\n" 
"    Output.Diffuse.a = 1.0f; \n" 
"    \n" 
"    // Just copy the texture coordinate through\n" 
"    if( bTexture ) \n" 
"        Output.TextureUV = input.vTexCoord0; \n" 
"    else\n" 
"        Output.TextureUV = 0; \n" 
"    \n" 
"    return Output;    \n" 
"}\n" 
"//--------------------------------------------------------------------------------------\n" 
"// Pixel shader output structure\n" 
"//--------------------------------------------------------------------------------------\n" 
"struct HUDPS_OUTPUT\n" 
"{\n" 
"    float4 RGBColor : SV_Target;  // Pixel color\n" 
"};\n" 
"//--------------------------------------------------------------------------------------\n" 
"// This shader outputs the pixel's color by modulating the texture's\n" 
"//       color with diffuse material color\n" 
"//--------------------------------------------------------------------------------------\n" 
"HUDPS_OUTPUT RenderHUDPS( HUDVS_OUTPUT In,\n" 
"                         uniform bool bTexture ) \n" 
"{ \n" 
"    HUDPS_OUTPUT Output;\n" 
"    // Lookup mesh texture and modulate it with diffuse\n" 
"    if( bTexture )\n" 
"        Output.RGBColor = hudTexture.Sample(hudSampler, In.TextureUV) * In.Diffuse;\n" 
"    else\n" 
"        Output.RGBColor = In.Diffuse;\n" 
"	//Output.RGBColor = float4(1.0,1.0,1.0,1.0);\n" 
"    return Output;\n" 
"}\n" 
"struct VS_INPUT\n" 
"{\n" 
"	float3 vPos : POSITION;\n" 
"};\n" 
"struct VS_OUTPUT\n" 
"{\n" 
"	float4 pos : SV_POSITION;\n" 
"	float4 VertexPosition_WorldSpace : COLOR0;\n" 
"	float2 PixelPos : COLOR1;\n" 
"	float4 TexCoord0 : TEXCOORD0;\n" 
"	float4 TexCoord1 : TEXCOORD1;\n" 
"	float4 TexCoord2 : TEXCOORD2;\n" 
"	float4 TexCoord3 : TEXCOORD3;\n" 
"	float4 TexCoord4 : TEXCOORD4;\n" 
"	float4 TexCoord5 : TEXCOORD5;\n" 
"};\n" 
"VS_OUTPUT OmniMapVS(VS_INPUT In)\n" 
"{\n" 
"// vertex shader has two goals...\n" 
"// (1) pass worldspace coordinates to fragment shader\n" 
"// (2) set projector space coordinates as gl position\n" 
"// (2a) orthographic projection\n" 
"// (2b) ftheta projection\n" 
"// (2c) Normal Projector\n" 
"// (1)\n" 
"	VS_OUTPUT Output;\n" 
"	float R;\n" 
"	float4 vPos = float4(In.vPos, 1.0);\n" 
"	Output.VertexPosition_WorldSpace.xyz = vPos;\n" 
"	if(false)// (2) a\n" 
"	{\n" 
"		Output.pos = mul(vPos, g_mProjection);	//Position of vertex in projector space\n" 
"		Output.VertexPosition_WorldSpace.w = Output.pos.w;\n" 
"	}\n" 
"	else// (2) b\n" 
"	{\n" 
"		float4x4 tmp = mul(g_mWorldView, g_mProjection);\n" 
"		float4 VertexPosition_projectorSpace_Goal = mul(vPos, tmp);\n" 
"		// F-Theta Warping Code\n" 
"		float Z = VertexPosition_projectorSpace_Goal.z;\n" 
"		float D = length(VertexPosition_projectorSpace_Goal.xyz);\n" 
"		float MyPI = 3.14159265;  \n" 
"		R = (2.0/MyPI) * acos(Z/D);  // Ftheta.\n" 
"		\n" 
"	\n" 
"		/// this line is for converting the ftheta lenses image to !theta image\n" 
"		R =  0.2798*R *R *R *R *R *R  - 0.6875*R *R *R *R *R  + 0.7351*R *R *R *R  - 0.3472*R *R *R  + 0.0977*R *R  + 0.9221*R ;  \n" 
" \n" 
"	\n" 
"		float l = 1.0/ (length(VertexPosition_projectorSpace_Goal.xy)+.0000000000000000000000000000000000001);// bug fix  1\n" 
"		float2 thetavec = VertexPosition_projectorSpace_Goal.xy * float2(l,l); // bug fix  1\n" 
"		\n" 
"		\n" 
"		\n" 
"		Output.pos.xy= thetavec*R;\n" 
"		Output.pos.y = (Output.pos.y + yOffset1);\n" 
"		Output.pos.y *= yScale;\n" 
"		Output.pos.y = (Output.pos.y + yOffset2);\n" 
"		\n" 
"		Output.pos.w =1.0;\n" 
"		Output.pos.z =D/1000.0;\n" 
"		Output.VertexPosition_WorldSpace.w = 1.0;\n" 
"		\n" 
"		Output.PixelPos.xy = Output.pos.xy * float2(.5,-.5) + float2(.5 , .5);\n" 
"	}\n" 
"	\n" 
"	Output.TexCoord0 = float4(0.0,0.0,0.0,1.0);\n" 
"	Output.TexCoord1 = float4(0.0,0.0,0.0,1.0);\n" 
"	Output.TexCoord2 = float4(0.0,0.0,0.0,1.0);\n" 
"	Output.TexCoord3 = float4(0.0,0.0,0.0,1.0);\n" 
"	Output.TexCoord4 = float4(0.0,0.0,0.0,1.0);\n" 
"	Output.TexCoord5 = float4(0.0,0.0,0.0,1.0);\n" 
"	\n" 
"	for (int i = 0; i < NumChannels; i++)\n" 
"	{\n" 
"		//Output.TexCoordArr[i] = mul(vPos, textMatrixArray[i]);\n" 
"		if (i == 0)\n" 
"			Output.TexCoord0 = mul(vPos, textMatrixArray[i]);\n" 
"		else if (i == 1)\n" 
"			Output.TexCoord1 = mul(vPos, textMatrixArray[i]);\n" 
"		else if (i == 2)\n" 
"			Output.TexCoord2 = mul(vPos, textMatrixArray[i]);\n" 
"		else if (i == 3)\n" 
"			Output.TexCoord3 = mul(vPos, textMatrixArray[i]);\n" 
"		else if (i == 4)\n" 
"			Output.TexCoord4 = mul(vPos, textMatrixArray[i]);\n" 
"		else if (i == 5)\n" 
"			Output.TexCoord5 = mul(vPos, textMatrixArray[i]);\n" 
"	}\n" 
"		\n" 
"	return(Output);\n" 
"}\n" 
"//sampler channelSamplers[NumChannels];\n" 
"//--------------------------------------------------------------------------------------\n" 
"// Pixel shader output structure\n" 
"//--------------------------------------------------------------------------------------\n" 
"struct PS_OUTPUT\n" 
"{\n" 
"    float4 RGBColor : SV_Target;  // Pixel color    \n" 
"};\n" 
"Texture2D theTexture;\n" 
"PS_OUTPUT OmniMapFS(VS_OUTPUT In)\n" 
"{\n" 
"	PS_OUTPUT Output;\n" 
"	float x,y;\n" 
"	\n" 
"	\n" 
"	Output.RGBColor = float4(0.0, 0.0, 0.0, 1.0);\n" 
"	for(int i =0; i < NumChannels;i++)	\n" 
"	{\n" 
"			float3 tc = float3(0.0,0.0,0.0);\n" 
"			//tc.xy = In.TexCoordArr[i].xy / In.TexCoordArr[i].w;\n" 
"			//tc.y = 1.0f - tc.y;\n" 
"			//tc.z = In.TexCoordArr[i].w;\n" 
"			if (i == 0)\n" 
"			{\n" 
"				//tc.xy = .5 * In.TexCoord0.xy / In.TexCoord0.w + float2( 0.5, 0.5 );\n" 
"				tc.xy = In.TexCoord0.xy / In.TexCoord0.w;\n" 
"				tc.y = 1.0f - tc.y;\n" 
"				tc.z = In.TexCoord0.w;\n" 
"				//Output.RGBColor = float4(In.TexCoord0.x,In.TexCoord0.y,0.0,1.0);\n" 
"				\n" 
"			} else if (i == 1)\n" 
"			{\n" 
"				//tc.xy = 0.5 * In.TexCoord1.xy / In.TexCoord1.w + float2( 0.5, 0.5 );\n" 
"				tc.xy = In.TexCoord1.xy / In.TexCoord1.w;\n" 
"				tc.y = 1.0f - tc.y;\n" 
"				tc.z = In.TexCoord1.w;\n" 
"				//Output.RGBColor = float4(In.TexCoord1.x,In.TexCoord1.y,0.0,1.0);\n" 
"				\n" 
"			} else if (i == 2)\n" 
"			{\n" 
"				//tc.xy = 0.5 * In.TexCoord2.xy / In.TexCoord2.w + float2( 0.5, 0.5 );\n" 
"				tc.xy = In.TexCoord2.xy / In.TexCoord2.w;\n" 
"				tc.y = 1.0f - tc.y;\n" 
"				tc.z = In.TexCoord2.w;\n" 
"				//Output.RGBColor = float4(In.TexCoord2.x,In.TexCoord2.y,0.0,1.0);\n" 
"				\n" 
"			} else if (i == 3)\n" 
"			{\n" 
"				//tc.xy = 0.5 * In.TexCoord3.xy / In.TexCoord3.w + float2( 0.5, 0.5 );\n" 
"				tc.xy = In.TexCoord3.xy / In.TexCoord3.w;\n" 
"				tc.y = 1.0f - tc.y;\n" 
"				tc.z = In.TexCoord3.w;\n" 
"			} else if (i == 4)\n" 
"			{\n" 
"				//tc.xy = 0.5 * In.TexCoord4.xy / In.TexCoord4.w + float2( 0.5, 0.5 );\n" 
"				tc.xy = In.TexCoord4.xy / In.TexCoord4.w;\n" 
"				tc.y = 1.0f - tc.y;\n" 
"				tc.z = In.TexCoord4.w;\n" 
"			} else if (i == 5)\n" 
"			{\n" 
"				//tc.xy = 0.5 * In.TexCoord5.xy / In.TexCoord5.w + float2( 0.5, 0.5 );\n" 
"				tc.xy = In.TexCoord5.xy / In.TexCoord5.w;\n" 
"				tc.y = 1.0f - tc.y;\n" 
"				tc.z = In.TexCoord5.w;\n" 
"			}\n" 
"			\n" 
"			\n" 
"			if (tc.x >= 0.0 && tc.x <= 1.0 && tc.y >= 0.0 && tc.y <= 1.0 && tc.z > 0.0)\n" 
"			{\n" 
"				Output.RGBColor = float4(MULTISAMPLE_Fin_Fout(Samplers[i], tc.xy).xyz, 1.0);\n" 
"				//if (i == 0)\n" 
"					//Output.RGBColor = float4(channelTexture0.Sample(hudSampler, tc.xy).xyz, 1.0);\n" 
"				//else if (i == 1)\n" 
"					//Output.RGBColor = float4(channelTexture1.Sample(hudSampler, tc.xy).xyz, 1.0);\n" 
"				//else if (i == 2)\n" 
"					//Output.RGBColor = float4(channelTexture2.Sample(hudSampler, tc.xy).xyz, 1.0);\n" 
"			}\n" 
"	}\n" 
"	if (stencilMaskEnabled)\n" 
"		Output.RGBColor.rgb = Output.RGBColor.rgb * alphaMaskText.Sample(hudSampler, In.PixelPos.xy);\n" 
"		//Output.RGBColor.rgb = Output.RGBColor.rgb * tex2D(alphMaskSampler,In.PixelPos.xy).rgb;\n" 
"		//Output.RGBColor.rgb = Output.RGBColor.rgb * tex2D(alphMaskSampler,In.PixelPos.xy).rgb;\n" 
"	return(Output);\n" 
"}\n" 
"technique10 RenderScene\n" 
"{\n" 
"    pass P0\n" 
"    {\n" 
"		SetVertexShader( CompileShader( vs_4_0, OmniMapVS() ) );\n" 
"        SetGeometryShader( NULL );\n" 
"        SetPixelShader( CompileShader( ps_4_0, OmniMapFS() ) );\n" 
"       \n" 
"    }\n" 
"}\n" 
"//--------------------------------------------------------------------------------------\n" 
"// Renders scene to render target using D3D10 Techniques\n" 
"//--------------------------------------------------------------------------------------\n" 
"//--------------------------------------------------------------------------------------\n" 
"// DepthStates\n" 
"//--------------------------------------------------------------------------------------\n" 
"DepthStencilState EnableDepth\n" 
"{\n" 
"    DepthEnable = FALSE;\n" 
"    DepthWriteMask = ALL;\n" 
"    DepthFunc = LESS_EQUAL;\n" 
"};\n" 
"DepthStencilState DisableDepth\n" 
"{\n" 
"    DepthEnable = FALSE;\n" 
"    DepthWriteMask = ZERO;\n" 
"};\n" 
"technique10 RenderHUDWithTexture\n" 
"{\n" 
"    pass P0\n" 
"    {\n" 
"        SetVertexShader( CompileShader( vs_4_0, RenderHUDVS(true ) ) );\n" 
"        SetGeometryShader( NULL );\n" 
"        SetPixelShader( CompileShader( ps_4_0, RenderHUDPS(true) ) );\n" 
"        SetDepthStencilState( DisableDepth, 0 );\n" 
"    }\n" 
"}\n" 
"//--------------------------------------------------------------------------------------\n" 
"// Renders scene to render target using D3D10 Techniques\n" 
"//--------------------------------------------------------------------------------------\n" 
"technique RenderHUDWithoutTexture\n" 
"{\n" 
"    pass P0\n" 
"    {\n" 
"        SetVertexShader( CompileShader( vs_3_0, RenderHUDVS(false ) ) );\n" 
"        //SetGeometryShader( NULL );\n" 
"        SetPixelShader( CompileShader( ps_3_0, RenderHUDPS(false) ) );\n" 
"        //SetDepthStencilState( EnableDepth, 0 );\n" 
"    }\n" 
"}\n" 
}; 
