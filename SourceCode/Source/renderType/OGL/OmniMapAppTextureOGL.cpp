// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#include <GL/glew.h>
#ifdef WIN32
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include "OmniMapAppTextureOGL.h"
#include "OM_ErrorHandling.h"




extern int OmniMap_getGL_TEXTUREN_ARB_From_N(int index);

OmniMapAppTextureOGL::OmniMapAppTextureOGL(int _resW, int _resH,
		const char* strStartUpScriptFile,
		const char *luaResDir) :
	OmniMap(_resW, _resH, strStartUpScriptFile, luaResDir, false)
{
}


OmniMapChannelBase *OmniMapAppTextureOGL::Create_Channel(const char * name) 
{
  name;
	return new OmniMapChannelATOGL();
}

#ifdef ELUMENATI_INTERNAL
void OmniMapAppTextureOGL::popTextureMatrices()
{
	for (int i = 0; i < this->GetNumChannels(); i++)
	{
		OmniMapChannelBase *channel = this->GetChannel(i);
		glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(i));
		channel->BindTexture(i);
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
	}
}

#endif



OmniMapChannelATOGL::OmniMapChannelATOGL() :
	OmniMap_Channel()
{
}
	


void OmniMapChannelATOGL::BindTexture(int index)
{
	glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(index));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
}



void OmniMapChannelATOGL::DrawHeadsUpDisplay()
{
  glDisable(GL_CULL_FACE); 
  glEnable(GL_TEXTURE_2D);
  //glDisable(GL_DEPTH_CLAMP_NV);
  //glDisable(GL_STENCIL_TEST);
  //glDisable(GL_LIGHTING);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glColor4f(1.0,1.0,1.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0 ,0.0);
  glVertex3d(HeadsUpDisplay_Quad.bl.x,HeadsUpDisplay_Quad.bl.y,HeadsUpDisplay_Quad.bl.z);
  glTexCoord2f(0 ,1.0);
  glVertex3d(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y,HeadsUpDisplay_Quad.tl.z);
  glTexCoord2f(1.0 ,1.0);
  glVertex3d(HeadsUpDisplay_Quad.tr.x,HeadsUpDisplay_Quad.tr.y,HeadsUpDisplay_Quad.tr.z);
  glTexCoord2f(1.0 ,0);
  glVertex3d(HeadsUpDisplay_Quad.br.x,HeadsUpDisplay_Quad.br.y,HeadsUpDisplay_Quad.br.z);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glColor4f(.5,.5,.5,1);
  glBegin(GL_LINE_STRIP);
  glVertex3d(HeadsUpDisplay_Quad.bl.x,HeadsUpDisplay_Quad.bl.y,HeadsUpDisplay_Quad.bl.z);
  glVertex3d(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y,HeadsUpDisplay_Quad.tl.z);
  glVertex3d(HeadsUpDisplay_Quad.tr.x,HeadsUpDisplay_Quad.tr.y,HeadsUpDisplay_Quad.tr.z);
  glVertex3d(HeadsUpDisplay_Quad.br.x,HeadsUpDisplay_Quad.br.y,HeadsUpDisplay_Quad.br.z);
  glEnd();
  glColor4f(1,1,1,1);

}


void OmniMapChannelATOGL::UnbindTexture(int index)
{
  index;
}



void OmniMapChannelATOGL::SetTexureID(unsigned int id)
{
  textureID = id;
}


unsigned int OmniMapChannelATOGL::GetTextureID()
{
  return(textureID);
}



void OmniMapChannelATOGL::Initialize()
{
  EH_DECLARE;
  EH_Log("OmniMapChannelATOGL::Initialize");
}


void OmniMapChannelATOGL::beginRenderToChannel()
{
  if (useOmniMapMatrix)
    PushSetupMatricies();
}



void OmniMapChannelATOGL::endRenderToChannel()
{
  if (useOmniMapMatrix)
    PopSetupMatricies();
}

