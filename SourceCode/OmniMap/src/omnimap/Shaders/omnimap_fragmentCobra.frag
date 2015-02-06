
#if HAS_OMNIMAP_MULTISAMPLE_9X
#define HAS_OMNIMAP_MULTISAMPLE 1
#endif


#if HAS_OMNIMAP_MULTISAMPLE_9X
vec4 MultiSampleVec(sampler2D  tex , vec2 UV){
	vec3 offset = vec3(-.5,0,.5);
	vec2 offsetNormalized = vec2(-0.70710678118*.5,0.70710678118*.5);
	vec2 DUV = fwidth(UV);
	vec4 mainColor = (
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
	#define MULTISAMPLE_Vin_Vout(t,uv) MultiSampleVec((t),(uv))
#else
	#define MULTISAMPLE_Vin_Vout(t,uv) tex2D((t),(uv))
#endif




//  [ ] Idea for making faster...
//       Use projective texturing....
//       Prerender texture coordinates
//       or compute the texture coordinates in the vertex shader
//       Or create a two linear equations that convert ray.xyz1 -> texcoord.uv

//const int MaxChannels = 3;
//#define MaxChannels 3
varying vec2 PixelPos;
//uniform sampler2D channelTextures[MaxChannels];	//The images from the seperate RenderChannels
uniform sampler2D channelTexture0;
uniform sampler2D channelTexture1;
uniform sampler2D channelTexture2;
uniform sampler2D channelTexture3;
//uniform sampler2D channelTexture4;
//uniform sampler2D channelTexture5;

//uniform bool stencilMaskEnabled;
//uniform sampler2D StencilMask;

varying vec3 VertexPosition_WorldSpace;
varying float R;

//varying vec4  mgl_TexCoord[MaxChannels]; 
//varying vec4 tc0;
//varying vec4 tc1;
//varying vec4 tc2;
//varying vec4 tc3;
//varying vec4 tc4;
//varying vec4 tc5;



void main()
{
	
	gl_FragColor.rgba = vec4(0.0, 0.0, 0.0, 1.0);
	// redo this with projective texturing....

		for(int i = 0; i < MaxChannels; i++)	
		{

			
					vec3 texCoords;
					if (i == 0)
					{
						if (gl_TexCoord[0].w > 0.0)
						{
							texCoords = gl_TexCoord[0].xyz / gl_TexCoord[0].w;
							if (texCoords.s >= 0.0 && texCoords.s <= 1.0 &&
								texCoords.t >= 0.0 && texCoords.t <= 1.0)
								gl_FragColor.rgba = MULTISAMPLE_Vin_Vout(channelTexture0, texCoords.st).rgba;
								//gl_FragColor.rgba = vec4(texCoords.s, texCoords.t, 0.0, 1.0);
						}
					} else if (i == 1) 
					{
						if (gl_TexCoord[1].w > 0.0)
						{
							texCoords = gl_TexCoord[1].xyz / gl_TexCoord[1].w;
							if (texCoords.s >= 0.0 && texCoords.s <= 1.0 &&
								texCoords.t >= 0.0 && texCoords.t <= 1.0)
								gl_FragColor.rgba = MULTISAMPLE_Vin_Vout(channelTexture1, texCoords.st).rgba;
								//gl_FragColor.rgba = vec4(texCoords.s, texCoords.t, 0.0, 1.0);
						}
					} /*else if (i == 2)
					{
						if (gl_TexCoord[2].w > 0.0)
						{
							texCoords = gl_TexCoord[2].xyz / gl_TexCoord[2].w;
							if (texCoords.s >= 0.0 && texCoords.s <= 1.0 &&
								texCoords.t >= 0.0 && texCoords.t <= 1.0)
								gl_FragColor.rgba = MULTISAMPLE_Vin_Vout(channelTexture2, texCoords.st).rgba;
								//gl_FragColor.rgba = vec4(texCoords.s, texCoords.t, 0.0, 1.0);
						}
					} else if (i == 3)
					{
						if (gl_TexCoord[3].w > 0.0)
						{
							texCoords = gl_TexCoord[3].xyz / gl_TexCoord[3].w;
							if (texCoords.s >= 0.0 && texCoords.s <= 1.0 &&
								texCoords.t >= 0.0 && texCoords.t <= 1.0)
								gl_FragColor.rgba = MULTISAMPLE_Vin_Vout(channelTexture3, texCoords.st).rgba;
						}
					}*/// else if (i == 4)
					//{
						//if (tc4.w > 0.0)
						//{
							//texCoords = tc4.xyz / tc4.w;
							//if (texCoords.s >= 0.0 && texCoords.s <= 1.0 &&
								//texCoords.t >= 0.0 && texCoords.t <= 1.0)
								//gl_FragColor.rgba = MULTISAMPLE_Vin_Vout(channelTexture4, texCoords.st).rgba;
						//}
					//} else if (i == 5)
					//{
						//if (tc5.w > 0.0)
						//{
							//texCoords = tc5.xyz / tc5.w;
							//if (texCoords.s >= 0.0 && texCoords.s <= 1.0 &&
								//texCoords.t >= 0.0 && texCoords.t <= 1.0)
								//gl_FragColor.rgba = MULTISAMPLE_Vin_Vout(channelTexture5, texCoords.st).rgba;
						//}
					//}
						
				
	
		}

	//if the mask color is zero we could try doing an early branch exit so we can skip the projective texturing phase..
///	if (stencilMaskEnabled)
//		gl_FragColor.xyz = gl_FragColor.xyz  + .01 * texture2D(StencilMask,PixelPos.xy).xyz;

}
