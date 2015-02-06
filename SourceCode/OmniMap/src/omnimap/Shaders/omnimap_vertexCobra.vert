//#ifdef STENCILMASK_ENABLE
varying vec2 PixelPos;
//#endif

//JT



varying vec3 VertexPosition_WorldSpace;
varying float R;


uniform float CobraWarpWithTrueDimension = 1;


uniform sampler2D StencilMask;


void main()
{
	VertexPosition_WorldSpace.xyz = gl_Vertex.xyz;




		vec4 VertexPosition_projectorSpace_Goal = ftransform();

	float3 vPos = VertexPosition_WorldSpace.xyz;
	float sphericalR = length(vPos.xyz);
	float  sphericalPhi =  acos(vPos.y/sphericalR ) / 3.14 ;	
	float  sphericalTheta = atan2(-vPos.x,-vPos.z) / 3.14;




		// F-Theta Warping Code
		float Z = VertexPosition_projectorSpace_Goal.z;
		float D = length(VertexPosition_projectorSpace_Goal.xyz);


		float MyPI = 3.14159265;  
		R = (2.0/MyPI) * acos(Z/D);  // Ftheta.
	
	
		/// this line is for converting the ftheta lenses image to !theta image

		// if(usebangtheta)
		R =  0.2798*R *R *R *R *R *R  - 0.6875*R *R *R *R *R  + 0.7351*R *R *R *R  - 0.3472*R *R *R  + 0.0977*R *R  + 0.9221*R ;  
 
	
		float l =1.0/ (length(VertexPosition_projectorSpace_Goal.xy)+.0000000000000000000000000000000000001);// bug fix  1
		vec2 thetavec = VertexPosition_projectorSpace_Goal.xy *vec2(l,l); // bug fix  1
		
		//float2 thetavec =normalize(VertexPosition_projectorSpace_Goal.xy);  // orginial 1
		// gl_position is VertexPosition_projectorSpace_OpiticalDistort_precorrect
		
		
	
		
		gl_Position.xy= thetavec * R;
		gl_Position.w =1.0;
		gl_Position.z =-D/1000.0;   // bug fix 2b fix for reverse z depth

		// R > 1.0 for verticies behind the lens.  R will approach inf as it becomes exactly behind the lens.
		// Use this trick to discard triangles that can potentially be directly behind the lens.
		if(R > 2.0)
			gl_Position.z =-100000.0;

		gl_TexCoord[0] = (gl_TextureMatrix[0]) * gl_Vertex;
		gl_TexCoord[1] = (gl_TextureMatrix[1]) * gl_Vertex;
		gl_TexCoord[2] = (gl_TextureMatrix[2]) * gl_Vertex;
		gl_TexCoord[3] = (gl_TextureMatrix[3]) * gl_Vertex;
		//gl_TexCoord[4] = (gl_TextureMatrix[4]) * gl_Vertex;
		//gl_TexCoord[5] = (gl_TextureMatrix[5]) * gl_Vertex;






	
			// 0 off
			// 1 warp with true on 
			// 2 warp with true off


#ifdef FOV_H
			float goalThetaMin = 180.0/(-.5* FOV_H);
#else
			float goalThetaMin = 180.0/-75.0;
#endif

#ifdef FOV_V
			float goalPhiMax= 180.0/(-.5* FOV_V);
#else
			float goalPhiMax= 180.0/-33.0;
#endif

	
			float magicNumberOffset = -2.75+.0174;
			gl_Position.x =  sphericalTheta * goalThetaMin;
			gl_Position.y = sphericalPhi*goalPhiMax +magicNumberOffset;
			

			


		if(CobraWarpWithTrueDimension >1.5){
			float2 texSamplePoint = gl_Position.xy*.5+.5;
			float4 Offset = texture2DLod(StencilMask,texSamplePoint,0);
			float n = 0.005;
			Offset.xy = Offset.xy  +Offset.zw *n;
			gl_Position.x = Offset.x*2-1;
			gl_Position.y = Offset.y*2-1;
		}


			gl_Position.z = 1;
			gl_Position.w =  1;
		

}
