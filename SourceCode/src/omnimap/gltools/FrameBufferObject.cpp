#include "FrameBufferObject.h"
#include "GL/glext.h"

#include <stdio.h>
#include "../omnimap.h"



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
		ImageObject curr_object = image_objects[i];
		if (curr_object.texture_id != NULL) glDeleteTextures(1, &curr_object.texture_id);
		if (curr_object.depth_id   != NULL) glDeleteRenderbuffersEXT(1, &curr_object.depth_id);
		if (curr_object.stencil_id != NULL) glDeleteRenderbuffersEXT(1, &curr_object.stencil_id);
	}
	
	//delete the frame buffer
	glDeleteFramebuffersEXT(1, &framebuffer_id);
}


int
FrameBufferObject::CreateImageObject(unsigned int width, unsigned int height, int depthbuffer_enabled, int stencilbuffer_enabled)
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

	//Create the depth RenderBuffer if requested
	if (depthbuffer_enabled != 0)
	{
		glGenRenderbuffersEXT(1, &new_object.depth_id);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, new_object.depth_id);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, new_object.width, new_object.height);
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


//Binds an image object to the framebuffer from its list - given the id
void 
FrameBufferObject::BindImageObjectToFrameBuffer(int id)
{
	BindImageObjectToFrameBuffer(image_objects[id]);
}

//Bind an already existing ImageObject to this framebuffer
void 
FrameBufferObject::BindImageObjectToFrameBuffer(ImageObject object)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_id); 

	if (object.texture_id != NULL)
	{
		 glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, object.texture_type, object.texture_id, 0);
	}
	if (object.depth_id != NULL)
	{
		 glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object.depth_id);
	}
	if (object.stencil_id != NULL)
	{
		 glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object.stencil_id);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 

	width = object.width;
	height = object.height;

	//printf("BindImageObjectToFrameBuffer - ");
	CheckFramebufferStatus();
}

//Enable rendering to the framebuffer
void 
FrameBufferObject::RenderToFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_id); 

	
	//CheckFramebufferStatus();
}

//Disable rendering to the framebuffer
void 
FrameBufferObject::EndRenderToFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
}

void
FrameBufferObject::BindImageToTexture(int id)
{
	glEnable(image_objects[id].texture_type);
	glBindTexture(image_objects[id].texture_type, image_objects[id].texture_id);
	//	int iddd =image_objects[id].texture_id;
	//	printf("\BindImageToTexture - ");
	//CheckFramebufferStatus();
}

void
FrameBufferObject::DetachImageFromTexture(int id)
{
	glDisable(image_objects[id].texture_type);
}

void 
FrameBufferObject::OutputImageObject(int id)
{
	ImageObject* obj = &image_objects[id];

	LogSystem()->ReportMessage("Image Object #%d", id);
	LogSystem()->ReportMessage("new_object.width        = %d", obj->width);
	LogSystem()->ReportMessage("new_object.height       = %d", obj->height);
	LogSystem()->ReportMessage("new_object.texture_type = %d", obj->texture_type);
	LogSystem()->ReportMessage("new_object.texture_id   = %d", obj->texture_id);
	LogSystem()->ReportMessage("new_object.depth_id     = %d", obj->depth_id);
	LogSystem()->ReportMessage("new_object.stencil_id   = %d", obj->stencil_id);
}

void
FrameBufferObject::BindImageToTexture(ImageObject object)
{
	glBindTexture(object.texture_type, object.texture_id);
}

void 
FrameBufferObject::CheckFramebufferStatus()
{
    GLenum status;
    status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
			LogSystem()->ReportMessage("Framebuffer is complete!");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            LogSystem()->ReportError("Unsupported framebuffer format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            LogSystem()->ReportError("Framebuffer incomplete, missing attachment");
            break;
  //      case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
    //        LogSystem()->ReportError("Framebuffer incomplete, duplicate attachment");
      //      break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            LogSystem()->ReportError("Framebuffer incomplete, attached images must have same dimensions");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            LogSystem()->ReportError("Framebuffer incomplete, attached images must have same format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            LogSystem()->ReportError("Framebuffer incomplete, missing draw buffer");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            LogSystem()->ReportError("Framebuffer incomplete, missing read buffer");
            break;
        default:
           break;
    }
}

bool
FrameBufferObject::ispoweroftwo(int num)
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