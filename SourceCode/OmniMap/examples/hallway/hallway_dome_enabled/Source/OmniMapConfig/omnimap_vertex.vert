#ifdef STENCILMASK_ENABLE
varying vec2 PixelPos;
#endif


varying vec3 VertexPosition_WorldSpace;
varying float R;


/*
to do
check if MyPI is allready defined
look at assembly code...
skip computing  xyRadius
optimize the const vars
*/


void main()
{
// vertex shader has two goals...
// (1) pass worldspace coordinates to fragment shader
// (2) set projector space coordinates as gl position
// (2a) orthographic projection
// (2b) ftheta projection
// (2c) Normal Projector

// (1)
	VertexPosition_WorldSpace.xyz = gl_Vertex.xyz;


//	gl_Position = ftransform();	//Position of vertex in projector space
	if(false)// (2) a
	{
		gl_Position = ftransform();	//Position of vertex in projector space
	
	
		//xyRadius.xyzw =gl_Position.xyzw;
		R = gl_Position.w;
		
		gl_Position.w = 1.0;
		

	}
	else// (2) b
	{
		vec4 VertexPosition_projectorSpace_Goal = ftransform();

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
		//gl_Position.z =D;			// original2   
		//gl_Position.z =D/1000.0;    // bug fix 2a fix for small clipping plane
		gl_Position.z =-D/1000.0;   // bug fix 2b fix for reverse z depth


		//xyRadius.xyz = VertexPosition_projectorSpace_Goal.xyz;
		//Radius = R;

	}

#ifdef STENCILMASK_ENABLE
PixelPos.xy = gl_Position.xy * vec2(.5,-.5) + vec2(.5 , .5);//+ 
#endif
}
