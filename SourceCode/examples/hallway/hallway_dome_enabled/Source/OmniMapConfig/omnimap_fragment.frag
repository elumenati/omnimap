//  [ ] Idea for making faster...
//       Use projective texturing....
//       Prerender texture coordinates
//       or compute the texture coordinates in the vertex shader
//       Or create a two linear equations that convert ray.xyz1 -> texcoord.uv


#include "OmniMapConfig/shaderconfig.txt"

uniform vec4 Plane_Eq_XYZ[MaxChannels];			// Plane Equation
uniform vec4 Plane_Eq_U[MaxChannels];				// xyz->texture U
uniform vec4 Plane_Eq_V[MaxChannels];				// xyz->texture V
uniform sampler2D channelTextures[MaxChannels];	//The images from the seperate RenderChannels

uniform vec3 userposition;

varying vec3 VertexPosition_WorldSpace;
varying float R;




//I see four easy performance tweaks...
//make MaxChannels automatically match the actual number of channels...
//make IsInside use glsl keywords...
//skip doing	if(xyRadius.w >1.0)		discard;
//precompute RD = VertexPosition_WorldSpace.xyz - userposition_worldspace




// I think i can do this much faster if need be...
bool IsInsideSlow(vec2 l)
{
//	return (l.x >=-0.00001   && l.y >=-0.00001  );

	return (l.x >=-0.00001  && l.x<=1.00001 && l.y >=-0.00001  && l.y<=1.00001);

	

}
float IsInside(vec2 l)
{
	vec2 range = vec2(-0.00001,1.00001);
	vec4 temp = vec4(
		step(l.xy,range.yy),
		step(range.xx,l.xy)
);
	return dot(temp,temp);

}

// I think i can do this much faster if need be...
vec2 FastRayRectangleIntersect(
 vec4 Plane_Eq_XYZ,
 vec4 Plane_Eq_U,
 vec4 Plane_Eq_V,
  vec3 R0,vec3 RD)
{
	float t = -(   dot(R0,Plane_Eq_XYZ.xyz)+Plane_Eq_XYZ.w)/dot(Plane_Eq_XYZ.xyz,RD);
	vec4 pointonplane = vec4(R0+RD*t,1.0);
	vec2 st =vec2(dot(Plane_Eq_U,pointonplane),dot(Plane_Eq_V,pointonplane));

	if(t<0.0) st =vec2(-1.0,-1.0);

	return st;
}







void main()
{
	vec3 RD = VertexPosition_WorldSpace  -userposition;
	//if(R >1.0)
	//	discard;
	//  vec3 userposition_worldspace = userposition;
	//	gl_FragColor.xyzw = vec4(0,0,0,0);  // optional line...
	
	// redo this with projective texturing....
	for(int i =0; i < MaxChannels;i++)	
	{
		vec2 hit = FastRayRectangleIntersect( Plane_Eq_XYZ[i],Plane_Eq_U[i],Plane_Eq_V[i],userposition, RD);
		if(IsInsideSlow(hit))
			gl_FragColor.xyz = texture2D(channelTextures[i], hit).xyz;
			
	//		gl_FragColor.xyz = gl_FragColor.xyz*.5+ VertexPosition_WorldSpace.xyz*.5;
	}
	
	
	
	
	
	
}