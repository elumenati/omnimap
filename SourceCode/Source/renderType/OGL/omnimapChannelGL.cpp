// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#define MIRROR_BALL 1
#include <string>
#include <vector>
#include "omnimap.h"
#include <assert.h>
#include <math.h>
#include "tools/RenderChannel.h"
#include "omnitypedefs.h"
#include "OM_ErrorHandling.h"



extern int OmniMap_getGL_TEXTUREN_ARB_From_N(int n);





///////////////////////////////////////////////////////////
////////OmniMap_Channel
////////////////////////////////////////////////////////////



OmniMap_Channel::OmniMap_Channel()
{
  prc_RenderChannel = NULL;
  useOmniMapMatrix = true;
  xResolution = yResolution = 0;

  renderTarget = OmniMapChannelBase::FRAME_BUFFER_OBJECT;

  SetUpPropertyAccess();
  ConsolidateCameraIntoProjectionMatrix = OMNIMAP_DEFAULT_ConsolidateCameraIntoProjectionMatrix;
}



OmniMap_Channel::~OmniMap_Channel()
{
  EH_DECLARE;
	EH_Log("delete OmniMap_Channel()");
	if (prc_RenderChannel != NULL) delete prc_RenderChannel;
}



void OmniMap_Channel::Initialize()
{
  EH_DECLARE;
  EH_Log("OmniMap_Channel::Initialize");
  EH_Zero(xResolution);
  EH_Zero(yResolution);
  EH_Ptr(prc_RenderChannel = new RenderChannel(xResolution, yResolution, (OmniMapChannelBase::TargetType) renderTarget));

  EH_OnError() {
    EH_Log("Channel width or height set to 0, width,height = %d,%d\n", xResolution, yResolution);
  }
}



void OmniMap_Channel::SetUpPropertyAccess()
{
  access.Register("UseOmniMapMatrix",&useOmniMapMatrix);
	// register this varible... this should be placed into a better function
	// like a derived SetUpPropertyAccess_Additional()
	access.Register("ConsolidateCameraIntoProjectionMatrix",&ConsolidateCameraIntoProjectionMatrix);
	//OmniMapChannelBase::SetUpPropertyAccess();
}



#ifdef OM_PICKING
void OmniMap_Channel::beginRenderPickingToChannel(int mouseX, int mouseY)
{
  //prc_RenderChannel->beginRenderToChannel();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  int viewport[4];
  glViewport(0,0,32,32);
  glGetIntegerv(GL_VIEWPORT,(GLint *) viewport);
  gluPickMatrix(mouseX,mouseY,
    1024,1024,(GLint *) viewport);
  ApplyProjectionMatrix();
  //gluPerspective(90.0,1.0,0.1,1000);
  ApplyViewMatrix();

  glMatrixMode(GL_MODELVIEW);		// Reset GL_MODELVIEW matrix
  glPushMatrix();
  glLoadIdentity();
}


void OmniMap_Channel::endRenderPickingToChannel()
{
  //if (useOmniMapMatrix)
  //PopSetupMatricies();
  //prc_RenderChannel->endRenderToChannel();

  glMatrixMode(GL_PROJECTION);		// Reset projection matrix
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);			// Reset GL_MODELVIEW matrix
  glPopMatrix();
}
#endif




void OmniMap_Channel::beginRenderToChannel()
{
  prc_RenderChannel->beginRenderToChannel();
  if (useOmniMapMatrix)
    PushSetupMatricies();
}



void OmniMap_Channel::endRenderToChannel()
{
  if (useOmniMapMatrix)
    PopSetupMatricies();
  prc_RenderChannel->endRenderToChannel();
}



void OmniMap_Channel::PushSetupMatricies()
{
  glMatrixMode(GL_PROJECTION);		// Set GL_PROJECTION matrix
  glPushMatrix();
  glLoadIdentity();
  ApplyProjectionMatrix();
  if(ConsolidateCameraIntoProjectionMatrix)
    ApplyViewMatrix();

  glMatrixMode(GL_MODELVIEW);		// Reset GL_MODELVIEW matrix
  glPushMatrix();
  glLoadIdentity();
  if(!ConsolidateCameraIntoProjectionMatrix)
    ApplyViewMatrix();
}





void OmniMap_Channel::PopSetupMatricies()
{
  glMatrixMode(GL_PROJECTION);		// Reset projection matrix
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);			// Reset GL_MODELVIEW matrix
  glPopMatrix();
}





unsigned int OmniMap_Channel::GetTextureID()
{
  return(prc_RenderChannel->getTextureId());
}




unsigned int OmniMap_Channel::GetFrameBufferID()
{
  return(prc_RenderChannel->getFrameBufferId());
}




void OmniMap_Channel::BindTexture(int index)
{
  glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(index));
  prc_RenderChannel->bindTexture();
}




void OmniMap_Channel::UnbindTexture(int index)
{
  glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(index));
  prc_RenderChannel->unbindTexture();
}





void OmniMap_Channel::DrawHeadsUpDisplay()
{
  glDisable(GL_CULL_FACE); 
  glEnable(GL_TEXTURE_2D);
  //glDisable(GL_DEPTH_CLAMP_NV);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_LIGHTING);
  prc_RenderChannel->bindTexture();

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
  prc_RenderChannel->unbindTexture();

  glColor4f(.5,.5,.5,1);
  glBegin(GL_LINE_STRIP);
  glVertex3d(HeadsUpDisplay_Quad.bl.x,HeadsUpDisplay_Quad.bl.y,HeadsUpDisplay_Quad.bl.z);
  glVertex3d(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y,HeadsUpDisplay_Quad.tl.z);
  glVertex3d(HeadsUpDisplay_Quad.tr.x,HeadsUpDisplay_Quad.tr.y,HeadsUpDisplay_Quad.tr.z);
  glVertex3d(HeadsUpDisplay_Quad.br.x,HeadsUpDisplay_Quad.br.y,HeadsUpDisplay_Quad.br.z);
  glEnd();
  glColor4f(1,1,1,1);
}





void OmniMap_Channel::GetProjectionMatrix(float m[16])
{
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  ApplyProjectionMatrix();
  glGetFloatv(GL_TEXTURE_MATRIX, m);
  glPopMatrix();
}




void OmniMap_Channel::GetProjTextMatrix(float m[16])
{
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(.5f, .5f, .5f);
  glScalef(.5f, .5f, .5f);
  ApplyProjectionMatrix();
  ApplyViewMatrix();
  glGetFloatv(GL_TEXTURE_MATRIX, m);
  glPopMatrix();
}




void OmniMap_Channel::DrawBox(float x, float y, float size)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE); 
  glMatrixMode(GL_PROJECTION);

  glPushMatrix();
  glLoadIdentity();
  glOrtho(0,1023,0,1023,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor4f(1.0,1.0,1.0,1.0);
  glBegin(GL_QUADS);
  glVertex3d(x-size, y-size, 0.0);
  glVertex3d(x-size, y+size, 0.0);
  glVertex3d(x+size, y+size, 0.0);
  glVertex3d(x+size, y-size, 0.0);
  glEnd();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}


void OmniMap_Channel::ApplyProjectionMatrix()// assume projection space and identity
{
  glFrustum( cam_info.ClipLeft,
    cam_info.ClipRight,
    cam_info.ClipBottom,
    cam_info.ClipTop,
    cam_info.ClipNear,
    cam_info.ClipFar);
}




void OmniMap_Channel::ApplyViewMatrix() // assume world space
{
  glMultMatrixf(cam_info.Matrix_WorldView);
}





void OmniMap_Channel::SetNearFar(float fnear, float ffar)
{
  float fovy = atan(cam_info.ClipTop/cam_info.ClipNear);
  float aspectRatio = (cam_info.ClipRight - cam_info.ClipLeft) /
    (cam_info.ClipTop - cam_info.ClipBottom);

  cam_info.ClipTop = tan(fovy) * fnear;		
  cam_info.ClipBottom = -cam_info.ClipTop;
  cam_info.ClipLeft   = aspectRatio*cam_info.ClipBottom;
  cam_info.ClipRight  = aspectRatio*cam_info.ClipTop;
  cam_info.ClipNear = fnear;
  cam_info.ClipFar = ffar;
  //RecomputePlaneEquations();
}

