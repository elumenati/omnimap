static char effectomnimap_vertexCobra[] = { 
"//#ifdef STENCILMASK_ENABLE\n" 
"varying vec2 PixelPos;\n" 
"//#endif\n" 
"//JT\n" 
"varying vec3 VertexPosition_WorldSpace;\n" 
"varying float R;\n" 
"uniform float CobraWarpWithTrueDimension = 1;\n" 
"uniform sampler2D StencilMask;\n" 
"void main()\n" 
"{\n" 
"	VertexPosition_WorldSpace.xyz = gl_Vertex.xyz;\n" 
"		vec4 VertexPosition_projectorSpace_Goal = ftransform();\n" 
"	float3 vPos = VertexPosition_WorldSpace.xyz;\n" 
"	float sphericalR = length(vPos.xyz);\n" 
"	float  sphericalPhi =  acos(vPos.y/sphericalR ) / 3.14 ;	\n" 
"	float  sphericalTheta = atan2(-vPos.x,-vPos.z) / 3.14;\n" 
"		// F-Theta Warping Code\n" 
"		float Z = VertexPosition_projectorSpace_Goal.z;\n" 
"		float D = length(VertexPosition_projectorSpace_Goal.xyz);\n" 
"		float MyPI = 3.14159265;  \n" 
"		R = (2.0/MyPI) * acos(Z/D);  // Ftheta.\n" 
"	\n" 
"	\n" 
"		/// this line is for converting the ftheta lenses image to !theta image\n" 
"		// if(usebangtheta)\n" 
"		R =  0.2798*R *R *R *R *R *R  - 0.6875*R *R *R *R *R  + 0.7351*R *R *R *R  - 0.3472*R *R *R  + 0.0977*R *R  + 0.9221*R ;  \n" 
" \n" 
"	\n" 
"		float l =1.0/ (length(VertexPosition_projectorSpace_Goal.xy)+.0000000000000000000000000000000000001);// bug fix  1\n" 
"		vec2 thetavec = VertexPosition_projectorSpace_Goal.xy *vec2(l,l); // bug fix  1\n" 
"		\n" 
"		//float2 thetavec =normalize(VertexPosition_projectorSpace_Goal.xy);  // orginial 1\n" 
"		// gl_position is VertexPosition_projectorSpace_OpiticalDistort_precorrect\n" 
"		\n" 
"		\n" 
"	\n" 
"		\n" 
"		gl_Position.xy= thetavec * R;\n" 
"		gl_Position.w =1.0;\n" 
"		gl_Position.z =-D/1000.0;   // bug fix 2b fix for reverse z depth\n" 
"		// R > 1.0 for verticies behind the lens.  R will approach inf as it becomes exactly behind the lens.\n" 
"		// Use this trick to discard triangles that can potentially be directly behind the lens.\n" 
"		if(R > 2.0)\n" 
"			gl_Position.z =-100000.0;\n" 
"		gl_TexCoord[0] = (gl_TextureMatrix[0]) * gl_Vertex;\n" 
"		gl_TexCoord[1] = (gl_TextureMatrix[1]) * gl_Vertex;\n" 
"		gl_TexCoord[2] = (gl_TextureMatrix[2]) * gl_Vertex;\n" 
"		gl_TexCoord[3] = (gl_TextureMatrix[3]) * gl_Vertex;\n" 
"		//gl_TexCoord[4] = (gl_TextureMatrix[4]) * gl_Vertex;\n" 
"		//gl_TexCoord[5] = (gl_TextureMatrix[5]) * gl_Vertex;\n" 
"	\n" 
"			// 0 off\n" 
"			// 1 warp with true on \n" 
"			// 2 warp with true off\n" 
"#ifdef FOV_H\n" 
"			float goalThetaMin = 180.0/(-.5* FOV_H);\n" 
"#else\n" 
"			float goalThetaMin = 180.0/-75.0;\n" 
"#endif\n" 
"#ifdef FOV_V\n" 
"			float goalPhiMax= 180.0/(-.5* FOV_V);\n" 
"#else\n" 
"			float goalPhiMax= 180.0/-33.0;\n" 
"#endif\n" 
"	\n" 
"			float magicNumberOffset = -2.75+.0174;\n" 
"			gl_Position.x =  sphericalTheta * goalThetaMin;\n" 
"			gl_Position.y = sphericalPhi*goalPhiMax +magicNumberOffset;\n" 
"			\n" 
"			\n" 
"		if(CobraWarpWithTrueDimension >1.5){\n" 
"			float2 texSamplePoint = gl_Position.xy*.5+.5;\n" 
"			float4 Offset = texture2DLod(StencilMask,texSamplePoint,0);\n" 
"			float n = 0.005;\n" 
"			Offset.xy = Offset.xy  +Offset.zw *n;\n" 
"			gl_Position.x = Offset.x*2-1;\n" 
"			gl_Position.y = Offset.y*2-1;\n" 
"		}\n" 
"			gl_Position.z = 1;\n" 
"			gl_Position.w =  1;\n" 
"		\n" 
"}\n" 
}; 