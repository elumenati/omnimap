
#include "FrameBufferObject.h"

#include <stdio.h>
#include "../omnimap.h"
#include "OM_ErrorHandling.h"



#define EH_LogError(...) {\
  EH_SetErrorCode(EH_ERR_FAILED); \
  EH_Log(__VA_ARGS__); \
  EH_SetErrorCode(EH_ERR_SUCCESS); \
}


FrameBufferObject::FrameBufferObject()
{
	glewInit();
	glGenFramebuffersEXT(1, &framebuffer_id);
	
}

FrameBufferObject::~FrameBufferObject()
{
	
	//delete the image objects
	for (unsigned int i = 0; i < image_objects.size(); i++)
	{
		ImageObject &curr_object = image_objects[i];
		if (curr_object.texture_id != NULL) glDeleteTextures(1, &curr_object.texture_id);
		if (curr_object.depth_id   != NULL) glDeleteRenderbuffersEXT(1, &curr_object.depth_id);
		if (curr_object.stencil_id != NULL) glDeleteRenderbuffersEXT(1, &curr_object.stencil_id);
	}

  image_objects.clear();
	
	//delete the frame buffer
	glDeleteFramebuffersEXT(1, &framebuffer_id);
}



#ifdef TEXTURE_RECTANGLE
int FrameBufferObject::CreateImageObject(unsigned int width, unsigned int height, 
										 int depthbuffer_enabled, int stencilbuffer_enabled)
{	

	//Create the Image object
	ImageObject new_object;
	new_object.width		= width;
	new_object.height		= height;
	new_object.texture_type	= NULL;
	new_object.texture_id	= NULL;
	new_object.depth_id		= NULL;
	new_object.stencil_id	= NULL;

	//First determine if the requested dimensions are power of two
	if (ispoweroftwo(width) && ispoweroftwo(height))
	{
		new_object.texture_type = GL_TEXTURE_2D;	//Power of two enum
		LogSystem()->ReportMessage("Using GL_TEXTURE_2D");
	}
	else
	{
		new_object.texture_type = 0x84F5;			//Non power of two enum
	}

	glGenTextures(1, &new_object.texture_id);
	new_object.texture_id;
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, new_object.texture_id);
	new_object.texture_type = GL_TEXTURE_RECTANGLE_ARB;
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//Create the depth RenderBuffer if requested
	if (depthbuffer_enabled != 0)
	{
		glGenRenderbuffersEXT(1, &new_object.depth_id);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, new_object.depth_id);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, new_object.width, new_object.height);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_id);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, new_object.depth_id);
	}

	//glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);

	//Push this new image object onto the list
	image_objects.push_back(new_object);

	

	//Return the 'index' for the id of this texture
	return (int)image_objects.size() - 1;
			
}

#else
int FrameBufferObject::CreateImageObject(unsigned int width, unsigned int height, int depthbuffer_enabled, int stencilbuffer_enabled)
{
	//Create the Image object
	ImageObject new_object;
  EH_DECLARE;

	new_object.width		= width;
	new_object.height		= height;
	new_object.texture_type	= NULL;
	new_object.texture_id	= NULL;
	new_object.depth_id		= NULL;
	new_object.stencil_id	= NULL;

	EH_Log("CreateImageObject %d %d\n", width, height);

	//First determine if the requested dimensions are power of two
	if (ispoweroftwo(width) && ispoweroftwo(height))
	{
		new_object.texture_type = GL_TEXTURE_2D;	//Power of two enum
		EH_Log("Using GL_TEXTURE_2D");
	}
	else
	{
		new_object.texture_type = 0x84F5;			//Non power of two enum
	}


	//Create texture 
	glGenTextures(1, &new_object.texture_id);
    glBindTexture(new_object.texture_type, new_object.texture_id);
    glTexImage2D(new_object.texture_type, 0, GL_RGBA, new_object.width, new_object.height, 0, GL_RGBA, GL_FLOAT, NULL);
	//GLenum filterMode = (new_object.texture_type == 0x84F5) ? GL_NEAREST : GL_LINEAR;
	//GLenum filterMode = GL_NEAREST;
	GLfloat filterMode = GL_LINEAR;
    glTexParameterf(new_object.texture_type, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameterf(new_object.texture_type, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameterf(new_object.texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP
    glTexParameterf(new_object.texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP
#ifdef OM_MULTISAMPLE
	if (1) {
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, framebuffer_id);
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, 2, GL_RGBA8, 
			new_object.width, new_object.height);
		CheckFramebufferStatus();
	}
#endif
	//Create the depth RenderBuffer if requested
	if (depthbuffer_enabled != 0)
	{
		glGenRenderbuffersEXT(1, &new_object.depth_id);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, new_object.depth_id);
		
		
#ifdef OM_MULTISAMPLE
		if (1) {
			//samples=2, format=GL_DEPTH_COMPONENT24, width=256, height=256
			glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, 2, GL_DEPTH_COMPONENT16, 
				new_object.width, new_object.height);
			CheckFramebufferStatus();
		} else
#endif
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, new_object.width, new_object.height);


		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_id);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
			GL_RENDERBUFFER_EXT, new_object.depth_id);
		//CheckFramebufferStatus();

	}
	//Create the stencil RenderBuffer if requested
	if (stencilbuffer_enabled != 0)
	{
		glGenRenderbuffersEXT(1, &new_object.stencil_id);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, new_object.stencil_id);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX,  new_object.width, new_object.height);
	}

	//Push this new image object onto the list
	image_objects.push_back(new_object);

	

	//Return the 'index' for the id of this texture
	return (int)image_objects.size() - 1;
}
#endif



//Binds an image object to the framebuffer from its list - given the id
void FrameBufferObject::BindImageObjectToFrameBuffer(int id)
{
	BindImageObjectToFrameBuffer(image_objects[id]);
}



//Bind an already existing ImageObject to this framebuffer
void FrameBufferObject::BindImageObjectToFrameBuffer(ImageObject object)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_id); 

	if (object.texture_id != NULL)
	{
#ifdef TEXTURE_RECTANGLE
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, 
								object.texture_id, 0);
#else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, object.texture_type, 
			object.texture_id, 0);
#endif
	}
	if (object.depth_id != NULL)
	{
		 glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object.depth_id);
	}
	if (object.stencil_id != NULL)
	{
		 glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object.stencil_id);
	}
	//CheckFramebufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 

	width = object.width;
	height = object.height;

	//printf("BindImageObjectToFrameBuffer - ");
	//CheckFramebufferStatus();
}



//Enable rendering to the framebuffer
void FrameBufferObject::RenderToFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_id); 

	
	//CheckFramebufferStatus();
}



//Disable rendering to the framebuffer
void FrameBufferObject::EndRenderToFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
	

}



void FrameBufferObject::BindImageToTexture(int id)
{
	glEnable(image_objects[id].texture_type);
#ifdef TEXTURE_RECTANGLE
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, image_objects[id].texture_id);
#else
	glBindTexture(image_objects[id].texture_type, image_objects[id].texture_id);
#endif
	//	int iddd =image_objects[id].texture_id;
	//	printf("\BindImageToTexture - ");
	//CheckFramebufferStatus();
}



void FrameBufferObject::DetachImageFromTexture(int id)
{
	glDisable(image_objects[id].texture_type);
}



void FrameBufferObject::OutputImageObject(int id)
{
  ImageObject &obj = image_objects[id];
  EH_DECLARE;

	EH_Log("Image Object #%d", id);
	EH_Log("new_object.width        = %d", obj.width);
	EH_Log("new_object.height       = %d", obj.height);
	EH_Log("new_object.texture_type = %d", obj.texture_type);
	EH_Log("new_object.texture_id   = %d", obj.texture_id);
	EH_Log("new_object.depth_id     = %d", obj.depth_id);
	EH_Log("new_object.stencil_id   = %d", obj.stencil_id);
}



void FrameBufferObject::BindImageToTexture(ImageObject object)
{
	glBindTexture(object.texture_type, object.texture_id);
}



void FrameBufferObject::CheckFramebufferStatus()
{
  EH_DECLARE;
  switch(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      EH_Log("Framebuffer is complete!");
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      EH_LogError("Unsupported framebuffer format");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      EH_LogError("Framebuffer incomplete, missing attachment");
      break;
      //      case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
      //        EH_LogError("Framebuffer incomplete, duplicate attachment");
      //      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      EH_LogError("Framebuffer incomplete, attached images must have same dimensions");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      EH_LogError("Framebuffer incomplete, attached images must have same format");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      EH_LogError("Framebuffer incomplete, missing draw buffer");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      EH_LogError("Framebuffer incomplete, missing read buffer");
      break;
    default:
      break;
  }
}



bool FrameBufferObject::ispoweroftwo(int num)
{
	int pow = 1;
	for (int i = 0; i < 32; i++)
	{
		pow *= 2;
		if (pow == num)
			return true;
	}
	return false;
}
