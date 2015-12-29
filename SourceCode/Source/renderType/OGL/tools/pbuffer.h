#ifndef PBUFFER_H
#define PBUFFER_H
#include <GL/glew.h>
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif



#ifdef USE_PBUFFER

#ifdef WIN32
#include <GL/glext.h>
#include <GL/wglext.h>
#endif

class PBuffer {
	
public:
	//Constructor -- width/height - in pixels (currently must be powers of 2)
	//				 colorBits - sets number of bits for red, green, blue and alpha channels
	//				 depthBits - sets number of bits for depth channel
	//				 stencilBits - sets number of bits for stencil
	//				 context - the current device context (can usually pass in  wglGetCurrentDC() )
	//				 inRC - send in the current rendering context if you want the pbuffer to share display lists
	PBuffer(int width, int height, int colorBits, int depthBits, int stencilBits, HDC context, HGLRC inRC = 0);
	~PBuffer();

	//Make PBuffer the current context we are rendering too
	void activate();

	//Switch back to previous context
	void deactivate();

	//Bind the PBuffer texture (functions like glBindTexture(PBUFFER)
	void bindTexture();

	//Unbinds the PBuffer from the texture - need to call this so you can render to Pbuffer again
	void unbindTexture();

	void shareTextureListWith(HGLRC _rendering_context);

	inline int getWidth()
	{ return p_width; }

	inline int getHeight()
	{ return p_height; }

	//Texture id
	GLuint texture_object;

private:

	int p_width, p_height;

	bool isValid;
	//The pbuffer handle
	HPBUFFERARB p_buffer;
	//Device contexts - we keep old ones so we can switch back easy
	HDC pb_device_context, old_device_context;
	//Rendering contexts
	HGLRC pb_rendering_context, old_rendering_context; 
	

	//Private method for initializing all the OpenGL extension function pointers
	bool initPbufferExtensions(); 
	int isExtensionSupported(const char *extension);
};
#endif
#endif