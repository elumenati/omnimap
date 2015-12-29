
varying vec2 PixelPos;

void main() {
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0].x = 1.0 - vec2(gl_MultiTexCoord0).x;
	gl_TexCoord[0].y = vec2(gl_MultiTexCoord0).y;
	PixelPos.xy = gl_Position.xy * vec2(.5,-.5) + vec2(.5 , .5);
}