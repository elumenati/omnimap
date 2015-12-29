
#include "BackBufferTexture.h"
#include <GL/glew.h>
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include <assert.h>
#include "OM_ErrorHandling.h"



extern bool checkGLError();
BackBufferTexture::BackBufferTexture()
{
  textureID = 0;
}

BackBufferTexture::~BackBufferTexture()
{
  if (textureID) { glDeleteTextures(1, &textureID); textureID = 0; }
}

void BackBufferTexture::CreateTexture(int _width, int _height)
{
  EH_DECLARE;
	width = _width;
	height = _height;
	if (ispoweroftwo(width) && ispoweroftwo(height))
	{
		targetType = GL_TEXTURE_2D;
	} else
	{
		targetType = GL_TEXTURE_2D;
	}
	
  glGenTextures(1, (GLuint *) &textureID);
  EH_Zero(textureID);
  glBindTexture(targetType, textureID);
  glTexParameteri(targetType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(targetType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(targetType, 0, GL_RGBA, width, height, 0, GL_RGBA, 
    GL_UNSIGNED_BYTE, 0);

  EH_OnError() {}

  assert(checkGLError() &&"checkGLError() ");
}

void BackBufferTexture::setViewport()
{
	glViewport(0, 0, width, height);
}

void BackBufferTexture::CopyFB2Texture()
{
	assert(checkGLError() &&"checkGLError() ");
	glReadBuffer(GL_BACK);
	glBindTexture(targetType, textureID);
    glCopyTexImage2D(targetType, 0, GL_RGBA, 0, 0, width, height, 0);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
	assert(checkGLError() &&"checkGLError() ");
}

void BackBufferTexture::BindTexture()
{
	glBindTexture(targetType, textureID);
	assert(checkGLError() &&"checkGLError() ");
}

void BackBufferTexture::UnBindTexture()
{

}

bool
BackBufferTexture::ispoweroftwo(int num)
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