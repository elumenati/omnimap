static char effectMirrorShaderVert[] = { 
"varying vec2 PixelPos;\n" 
"void main() {\n" 
"	\n" 
"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n" 
"	gl_TexCoord[0].x = 1.0 - vec2(gl_MultiTexCoord0).x;\n" 
"	gl_TexCoord[0].y = vec2(gl_MultiTexCoord0).y;\n" 
"	PixelPos.xy = gl_Position.xy * vec2(.5,-.5) + vec2(.5 , .5);\n" 
"}\n" 
}; 
