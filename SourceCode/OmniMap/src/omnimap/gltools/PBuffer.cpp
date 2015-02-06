#include "PBuffer.h"
#include <stdio.h>
#include "../omnimap.h"
#ifdef USE_PBUFFER
//PBuffer Extensions
PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB = NULL;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB = NULL;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB = NULL;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB = NULL;

//Texture Extensions
PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB = NULL;
PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB = NULL;

//Pixel Format Extensions
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

//Procedure to grab available Extension Strings (to determine what is supported)
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

#define MAX_ATTRIBS 16
#define MAX_PFORMATS 32

int
PBuffer::isExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;

	HDC device = wglGetCurrentDC();
	if (device == NULL)
	{
		LogSystem()->ReportError("------------------------------------------------");
		LogSystem()->ReportError("ERROR - No Current OpenGL Device Context Found!");
		LogSystem()->ReportError("------------------------------------------------");

		return 0;
	}
	extensions = (const GLubyte *) wglGetExtensionsStringARB( device );
	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	start = extensions;
	for (;;) {
		where = (GLubyte *) strstr((const char *) start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return 1;
		start = terminator;
	}
	return 0;
}

bool 
PBuffer::initPbufferExtensions() {

	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtensionsStringARB == NULL)
	{ 
		LogSystem()->ReportError("wglGetExtensionsStringARB not found!");
		return false;
	}
								 
	//PBUFFER EXTENSIONS
	int pbuffer_supported = isExtensionSupported("WGL_ARB_pbuffer");
	if (!pbuffer_supported)	
	{
			LogSystem()->ReportError("------------------------------------------------");
			LogSystem()->ReportError("ERROR - WGL_ARB_pbuffer not supported!");
			LogSystem()->ReportError("------------------------------------------------");

			
			return false;
	}
	else
	{
		//printf("\nWGL_ARB_pbuffer found!");
		wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC) wglGetProcAddress("wglCreatePbufferARB");
		if (wglCreatePbufferARB == NULL)
		{ 
			LogSystem()->ReportError("wglCreatePbufferARB not found!");
			return false;
		}
		wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC) wglGetProcAddress("wglGetPbufferDCARB");
		if (wglGetPbufferDCARB == NULL)
		{ LogSystem()->ReportError("wglGetPbufferDCARB not found!");
		return false; }
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC) wglGetProcAddress("wglReleasePbufferDCARB");
		if (wglReleasePbufferDCARB == NULL)
		{ LogSystem()->ReportError("wglReleasePbufferDCARB not found!");
		return false; }
		wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC) wglGetProcAddress("wglDestroyPbufferARB");
		if (wglDestroyPbufferARB == NULL)
		{ LogSystem()->ReportError("wglDestroyPbufferARB not found!");
		return false; }
		wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC) wglGetProcAddress("wglQueryPbufferARB");
		if (wglQueryPbufferARB == NULL)
		{ LogSystem()->ReportError("wglQueryPbufferARB not found!");
		return false; }
	}
	
	//RENDER TO TEXTURE EXTENSIONS
	int render_to_tex_supported = isExtensionSupported("WGL_ARB_render_texture" );
	if (!render_to_tex_supported)
	{
		LogSystem()->ReportError("WGL_ARB_render_texture not supported!");
		return false;
	}	
	else
	{
		
		wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC) wglGetProcAddress("wglBindTexImageARB");
		if (wglBindTexImageARB == NULL)
		{ LogSystem()->ReportError("wglBindTexImageARB not found!");
		return false; }
		wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC) wglGetProcAddress("wglReleaseTexImageARB");
		if (wglReleaseTexImageARB == NULL)
		{ LogSystem()->ReportError("wglReleaseTexImageARB not found!");
		return false; }
	}

	//PIXEL FORMAT EXTENSIONS
	int pixel_format_supported = isExtensionSupported("WGL_ARB_pixel_format");
	if (!pixel_format_supported)
	{
		LogSystem()->ReportError("\nWGL_ARB_pixel_format not found!");
		return false;
	}
	else
	{
		//printf("\nWGL_ARB_pixel_format supported!");
		wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC) wglGetProcAddress("wglGetPixelFormatAttribivARB");
		if (wglGetPixelFormatAttribivARB == NULL)
		{ LogSystem()->ReportError("wglGetPixelFormatAttribivARB not found!");
		return false; }
		wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC) wglGetProcAddress("wglGetPixelFormatAttribfvARB");
		if (wglGetPixelFormatAttribfvARB == NULL)
		{ LogSystem()->ReportError("wglGetPixelFormatAttribfvARB not found!");
		return false; }
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
		if (wglChoosePixelFormatARB == NULL)
		{ LogSystem()->ReportError("wglChoosePixelFormatARB not found!");
		return false; }
	}

	LogSystem()->ReportMessage("* -- All necessary PBuffer extension procedures found");

	return true;
}

PBuffer::PBuffer(int width, int height, int colorBits, int depthBits, int stencilBits, HDC current_device, HGLRC inRC)
{
	//printf("\n* -- Initializing PBuffer");
	//initializing to false.. if it passes the test it'll go true
	isValid = false;

	initPbufferExtensions();

	int pf_attr[] =
	{
		WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
		WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE, // P-buffer will be used as a texture
		WGL_RED_BITS_ARB,   colorBits,      // Red channel bits
		WGL_GREEN_BITS_ARB, colorBits,      // Green channel bits
		WGL_BLUE_BITS_ARB,  colorBits,      // Blue channel bits
		WGL_ALPHA_BITS_ARB, colorBits,      // Alpha channel bits
		WGL_DEPTH_BITS_ARB, depthBits,      // Depth bits
		WGL_STENCIL_BITS_ARB, stencilBits,  // Stencil bits
		WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
		0                                   // Zero terminates the list
	};

	unsigned int count = 0;
	int pixelFormat;
	wglChoosePixelFormatARB(current_device, (const int*)pf_attr, NULL, 1, &pixelFormat, &count);
	if (count == 0)
	{
		LogSystem()->ReportError("Could not find an acceptable pixel format!");
		return;
	}

	int pb_attr[] =
	{
		//WGL_PBUFFER_LARGEST_ARB, true,
		WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB, // Our p-buffer will have a texture format of RGBA
		WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,   // Of texture target will be GL_TEXTURE_2D
		0                                             // Zero terminates the list
	};

	//Create the pbuffer
	p_buffer = wglCreatePbufferARB(current_device, pixelFormat, width, height, pb_attr);
	if (!p_buffer)
	{
		LogSystem()->ReportError("Error - PBuffer not created");
	}
	//Get device context for Pbuffer
	pb_device_context = wglGetPbufferDCARB(p_buffer);
	if (!pb_device_context)
	{
		LogSystem()->ReportError("Error - PBuffer Device Context not found");
	}
	
	pb_rendering_context = wglCreateContext(pb_device_context);
	if (!pb_rendering_context)
	{
		LogSystem()->ReportError("Error - PBuffer Rendering Context not created");
	}

	if (inRC != 0)
	{
		//wglShareLists(pb_rendering_context, inRC);
		int share = wglShareLists(inRC, pb_rendering_context);

		if (share)
			LogSystem()->ReportError("Successfully shared display lists");
		else
			LogSystem()->ReportError("FAILED to share display list!");
	}

	//Example of querying -- 
	int actual_height, actual_width;
	wglQueryPbufferARB(p_buffer, WGL_PBUFFER_WIDTH_ARB, &actual_width);
	wglQueryPbufferARB(p_buffer, WGL_PBUFFER_HEIGHT_ARB, &actual_height);
	
	LogSystem()->ReportMessage("* -- PBuffer created with dimensions = %d %d", actual_width, actual_height);

	p_width = actual_width; p_height = actual_height;

	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	
	isValid = true;
}

PBuffer::~PBuffer()
{
	wglDeleteContext(pb_rendering_context);
	wglReleasePbufferDCARB(p_buffer, pb_device_context);
	wglDestroyPbufferARB(p_buffer);
}

void 
PBuffer::shareTextureListWith(HGLRC _rendering_context)
{
	wglShareLists(_rendering_context, this->pb_rendering_context);
}



void
PBuffer::activate()
{
	int lost = 0;
    wglQueryPbufferARB(p_buffer, WGL_PBUFFER_LOST_ARB, &lost);
    if(lost != 0 )
    {
		LogSystem()->ReportError("PBuffer was lost due to Display Mode Switch!");
		return;
    }

	old_device_context = wglGetCurrentDC();
    old_rendering_context = wglGetCurrentContext();
	wglMakeCurrent(pb_device_context, pb_rendering_context);
}

void
PBuffer::deactivate()
{
	wglMakeCurrent(old_device_context, old_rendering_context);
}


void 
PBuffer::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, texture_object);
	if(!wglBindTexImageARB(p_buffer, WGL_FRONT_LEFT_ARB) )
	{
		LogSystem()->ReportError("Error - Can't bind p-buffer to render texture!");
			return;
	}
}

void 
PBuffer::unbindTexture()
{
	if(!wglReleaseTexImageARB(p_buffer, WGL_FRONT_LEFT_ARB))
	{
		LogSystem()->ReportError("Error - Can't release p-buffer from render texture!");
		return;
	}
}
#endif
