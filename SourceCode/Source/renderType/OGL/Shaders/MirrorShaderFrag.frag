
//#define MaxChannels 3


// float MirrorRadius = .1;
//vec3 MirrorCenter = vec3(0,0,0); // Mirror Center
//float Grace = 1.0;
//vec3 ProjectorOrigin= vec3(0,0,-.99); // Projector Origin
//vec3 ProjectorRayUp = vec3( 0,1,0);

//vec3 AudiencePosition = vec3(0,0,0);
float DomeRadius = 1.0;
vec3 DomeCenter = vec3(0,0,0); // dome Center



// The images from the seperate RenderChannels
// For this hack, order assummed to be left, right, top.
//uniform sampler2D channelTextures[MaxChannels];	
uniform sampler2D channelTexture0;
uniform sampler2D channelTexture1;
uniform sampler2D channelTexture2;
uniform sampler2D channelTexture3;

uniform bool stencilMaskEnabled;
uniform bool debug1;
uniform sampler2D StencilMask;
varying vec3 VertexPosition_WorldSpace;
varying float R;
varying vec2 PixelPos;
uniform bool useTexture;



float RaySphereIntersection(vec3 o, vec3 d, vec3 c, float R ) {
	vec3 oc = o-c;
	float discriminant = dot(d,oc)*dot(d,oc) - dot(d,d) * (dot(oc,oc)-R*R);
	
	// assume D>0 and there are 2 hits.
	float PlusT = (dot(-d,oc) + sqrt(discriminant) ) / dot(d,d);
	float NegT = (dot(-d,oc) - sqrt(discriminant) ) / dot(d,d);

	if(discriminant <= 0.0)
		return 0.0;
	else
	{
		if(NegT > 0.0)
			return NegT;
		else
			return PlusT;
	}
}

vec4 ApplyMirrorTest(float4x4 texturematrix, sampler2D channelTexture,vec4 ViewRay, vec4 inColor)
{
		vec4 leftCoordProj 	= texturematrix * ViewRay;
		vec2 leftTexCoords 	= leftCoordProj.xy / leftCoordProj.w;

		if (leftCoordProj.w >= 0.0 && leftTexCoords.x >= 0.0 && leftTexCoords.x <= 1.0 && leftTexCoords.y >= 0.0 && leftTexCoords.y <= 1.0)
			inColor = texture2D(channelTexture, vec2(leftTexCoords.x, leftTexCoords.y));
        return inColor;
}
	
void main()  {
	vec3 ProjectorRayForward = normalize(MirrorCenter-ProjectorOrigin);
	vec3 ProjectorRayRight = normalize(cross(ProjectorRayForward,ProjectorRayUp));
	ProjectorRayUp =	normalize(cross(ProjectorRayRight ,ProjectorRayForward));

	// Ray Direction
	vec3 d  = normalize(  MirrorCenter + 
									ProjectorRayUp   * Grace * mix(-MirrorRadius,MirrorRadius,gl_TexCoord[0].y)+
									ProjectorRayRight * Grace * mix(-MirrorRadius,MirrorRadius,gl_TexCoord[0].x)
									-ProjectorOrigin
									);


	float t = RaySphereIntersection(ProjectorOrigin,d,MirrorCenter,MirrorRadius);
	vec3 p = ProjectorOrigin+t*d;
	vec3 n = (p-MirrorCenter)/ MirrorRadius;
	n= normalize(n);
	
	vec3 oDome = p; // Point on the mirror
	vec3 dDome = reflect(d,n);// Reflect the d ray off of n
		
	dDome= normalize(dDome);
	float   tDome = RaySphereIntersection(oDome,dDome,DomeCenter,DomeRadius);
	vec3  pDome = oDome+tDome*dDome;
	vec4 ViewRay = vec4(normalize(pDome-AudiencePosition),1);
	vec4 outColor = vec4(0.0,0.0,0.0,1.0);
	
	
	outColor = ApplyMirrorTest(gl_TextureMatrix[0], channelTexture0, ViewRay, outColor);
	outColor = ApplyMirrorTest(gl_TextureMatrix[1], channelTexture1, ViewRay, outColor);
	outColor = ApplyMirrorTest(gl_TextureMatrix[2], channelTexture2, ViewRay, outColor);
	
	if(MaxChannels>=4)
		outColor = ApplyMirrorTest(gl_TextureMatrix[3], channelTexture3, ViewRay, outColor);
	
	/*
	for(int i = 0; i < MaxChannels; i++) {
		if (i == 0) {
			vec4 leftCoordProj 	= gl_TextureMatrix[i] * ViewRay;
			vec2 leftTexCoords 	= leftCoordProj.xy / leftCoordProj.w;
			vec4 outColorLeft = texture2D(channelTexture0, vec2(leftTexCoords.x, leftTexCoords.y));
			if (leftCoordProj.w >= 0.0&&leftTexCoords.x >= 0.0 && leftTexCoords.x <= 1.0 && leftTexCoords.y >= 0.0 && leftTexCoords.y <= 1.0)
				outColor = outColorLeft;
		} else if (i == 1) {
			vec4 rightCoordProj	= gl_TextureMatrix[i] * ViewRay;
			vec2 rightTexCoords = rightCoordProj.xy / rightCoordProj.w;
			vec4 outColorRight = texture2D(channelTexture1, vec2(rightTexCoords.x, rightTexCoords.y));
			if (rightCoordProj.w >= 0.0&&rightTexCoords.x >= 0.0 && rightTexCoords.x <= 1.0 && rightTexCoords.y >= 0.0 && rightTexCoords.y <= 1.0)
				outColor = outColorRight;
		} else if (i ==2) {
			vec4 topCoordProj 	= gl_TextureMatrix[i] * ViewRay;
			vec2 topTexCoords 	= topCoordProj.xy / topCoordProj.w;
			vec4 outColorTop = texture2D(channelTexture2, vec2(topTexCoords.x, topTexCoords.y));
			if (topCoordProj.w >= 0.0&& topTexCoords.x >= 0.0 && topTexCoords.x <= 1.0 && topTexCoords.y >= 0.0 && topTexCoords.y <= 1.0)
				outColor = outColorTop;
		} else if (i == 3) {
			vec4 bottomCoordProj = gl_TextureMatrix[i] * ViewRay;
			vec2 bottomTexCoords 	= bottomCoordProj.xy / bottomCoordProj.w;
			vec4 outColorBottom = texture2D(channelTexture3, vec2(bottomTexCoords.x, bottomTexCoords.y));
			if (bottomCoordProj.w >= 0.0&& bottomCoordProj.x >= 0.0 && bottomCoordProj.x <= 1.0 && bottomCoordProj.y >= 0.0 && bottomCoordProj.y <= 1.0)
				outColor = outColorBottom;
		}
	}
	*/

	if (debug1)
		gl_FragColor.xyz = outColor.xyz;		
	else if (stencilMaskEnabled)
		gl_FragColor.xyz = outColor.xyz * texture2D(StencilMask,PixelPos.xy).xyz;
	else
		gl_FragColor.xyz = outColor.xyz;
}


