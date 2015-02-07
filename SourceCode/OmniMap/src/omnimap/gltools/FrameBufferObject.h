#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

#include <GL/glew.h>
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif


#include <vector> 

#include "Spout.h"    // Spout SDK

//-------ImageObject structure-------------
//
//  This object stores information relative to the image
//  the framebuffer draws to.  It stores the width&height
//  of the frame along with its internal format. It also
//  mantains an ID for a depth or stencil renderbuffer
//  if it is requested.
//
//-----------------------------------------
typedef struct ImageObject {
	
	unsigned int width;
	unsigned int height;
	GLenum texture_type;

	GLuint texture_id;
	GLuint depth_id;
	GLuint stencil_id;

} ImageObject;


class FrameBufferObject {

public:

    SpoutSender *spoutsender  ;        // A sender object
    char sendername[256];            // Shared memory name
    GLuint sendertexture;            // Local OpenGL texture used for sharing
    bool bInitialized;                // Initialization result
    bool bMemoryShare;                // Texture share compatibility

	//Default constructor - allocates a framebufferobject with no attached textures or renderBuffers
	FrameBufferObject();

	//Destroys the framebuffer and any allocated textures and renderBuffers
	~FrameBufferObject();

	// This method creates the image object for the framebuffer to bind and render to
	// width, height = The width and height of the image in pixels (doesn't have to be power of two)
	// depthbuffer_enabled   : 0 (No depth buffer),   1 (24-bit depth buffer)
	// stencilbuffer_enabled : 0 (No stencil buffer), 1 (8-bit stencil buffer)
	int CreateImageObject(unsigned int width, unsigned int height, int depthbuffer_enabled, int stencilbuffer_enabled);

	//Binds an image object to the framebuffer from its list - given the id
	void BindImageObjectToFrameBuffer(int id);
	//Bind an already existing ImageObject to this framebuffer
	void BindImageObjectToFrameBuffer(ImageObject object);

	//Enable rendering to the framebuffer
	void RenderToFrameBuffer();

	//Disable rendering to the framebuffer
	void EndRenderToFrameBuffer();

	//Attaches the FrameBuffer image to current texture 
	void BindImageToTexture(int id); 
	void BindImageToTexture(ImageObject object); 
	void DetachImageFromTexture(int id);

	void OutputImageObject(int id);

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }


	int getOpenGL_TextureID(int localId)
	{
		return image_objects[localId].texture_id;
	}

	int getOpenGL_FrameBufferID()
	{
		return framebuffer_id;
	}

private:

	//Data Members
	GLuint framebuffer_id;						//Framebuffer id
	std::vector<ImageObject> image_objects;		//List of imageobjects associated with this framebuffer
	int width, height;

	//Private Methods
	void CheckFramebufferStatus();				//Private method to determine proper framebuffer setup
	bool ispoweroftwo(int num);					//Determines if a number is a power of two (True - yes, False - no)
};




#endif


