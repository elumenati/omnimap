// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

//#include "Socket.h"  // need socket.cpp socket.h and the lib file
#include <process.h>
#include <string>
#include <vector>
#include "omnimapD3D.h"
#include "OmniMapShaderD3D.h"
#include "ScreenRendererD3D.h"
#include "D3D/dxstdafx.h"
#include <assert.h>
#include <math.h>
#include "OM_ErrorHandling.h"

#include "privatePrimitives.h"

///////////////////////////////////////////////

#include "omnitypedefs.h"



static IDirect3DVertexBuffer9 *bgBuffer = NULL;
static LPDIRECT3DTEXTURE9 logoTexture = NULL;
static IDirect3DStateBlock9* pStateBlock = NULL;




OMNIMAP_UNMANAGEDGLUE_API OmniMapD3D *UnmanagedGlue_OmnimapConstructorD3D9(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice, const char *strStartUpScriptFile, const char *luaResDir);
OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapDestructorD3D9(OmniMapD3D *omnimapHandle);
OMNIMAP_UNMANAGEDGLUE_API int UnmanagedGlue_OmnimapGetChannelTotalD3D9(OmniMapD3D *omnimapHandle);
OMNIMAP_UNMANAGEDGLUE_API OmniMapChannelD3D *UnmanagedGlue_OmnimapGetChannelInfoD3D9(OmniMapD3D *omnimapHandle, int channelNumber, float viewMatrix[16], float projectionMatrix[16]);
OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapBeginRenderChannelD3D9(OmniMapChannelD3D *channelHandle);
OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapEndRenderChannelD3D9(OmniMapChannelD3D *channelHandle);
OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapPostRenderD3D9(OmniMapD3D *omnimapHandle);


OMNIMAP_UNMANAGEDGLUE_API OmniMapD3D *UnmanagedGlue_OmnimapConstructorD3D9(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice, const char *strStartUpScriptFile, const char *luaResDir) {
  const char *strStartUpScriptFileBase = (strStartUpScriptFile) ? strStartUpScriptFile : "Resources/omnimap_startup.lua";
  const char *luaResDirBase = (luaResDir) ? luaResDir : "Resources/";
  return new OmniMapD3D(_resW, _resH, _d3dDevice, strStartUpScriptFileBase, luaResDirBase);
}


OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapDestructorD3D9(OmniMapD3D *omnimapHandle) {
  delete omnimapHandle;
}


OMNIMAP_UNMANAGEDGLUE_API int UnmanagedGlue_OmnimapGetChannelTotalD3D9(OmniMapD3D *omnimapHandle) {
  return omnimapHandle->GetNumChannels();
}

OMNIMAP_UNMANAGEDGLUE_API OmniMapChannelD3D *UnmanagedGlue_OmnimapGetChannelInfoD3D9(OmniMapD3D *omnimapHandle, int channelNumber, float viewMatrix[16], float projectionMatrix[16]) {
  OmniMapChannelD3D *channel = (OmniMapChannelD3D *)omnimapHandle->GetChannel(channelNumber);

  if (viewMatrix) {
    D3DXMATRIX viewMat;
    channel->GetViewMatrix(&viewMat);
    memcpy(viewMatrix, (FLOAT*)viewMat, sizeof(float)*16);
  }
  if (projectionMatrix) {
    D3DXMATRIX projMat;
    channel->GetProjectionMatrix(&projMat);
    memcpy(projectionMatrix, (FLOAT*)projMat, sizeof(float)*16);
  }

  return channel;
}

OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapBeginRenderChannelD3D9(OmniMapChannelD3D *channelHandle) {
  channelHandle->beginRenderToChannel();
}

OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapEndRenderChannelD3D9(OmniMapChannelD3D *channelHandle) {
  channelHandle->endRenderToChannel();
}

OMNIMAP_UNMANAGEDGLUE_API void UnmanagedGlue_OmnimapPostRenderD3D9(OmniMapD3D *omnimapHandle) {
  omnimapHandle->PostRender();
}







OmniMapD3D::OmniMapD3D(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice, const char* strStartUpScriptFile, 
					   const char *luaResDir, bool doInit) :
	OmniMapBase(_resW, _resH, strStartUpScriptFile, luaResDir)
{
  EH_DECLARE;
	EH_Log("Constructing OmniMapD3D");

	graphicsContext = (void *) _d3dDevice;
	fxShaderFilename[0] = '\0';
	d3dDevice = _d3dDevice;

  bgBuffer = NULL;
  logoTexture = NULL;
  pStateBlock = NULL;

	glViewportsettings0 = OMNIMAP_DEFAULT_glViewportsettings0;
	glViewportsettings1 = OMNIMAP_DEFAULT_glViewportsettings1;
	glViewportsettings2 = OMNIMAP_DEFAULT_glViewportsettings2;
	glViewportsettings3 = OMNIMAP_DEFAULT_glViewportsettings3;
	GL_STATE_CLEAR_AT_STARTFRAME = OMNIMAP_DEFAULT_GL_STATE_CLEAR_AT_STARTFRAME;
	GL_STATE_glDisable_GL_DEPTH_TEST = OMNIMAP_DEFAULT_GL_STATE_glDisable_GL_DEPTH_TEST;
	GL_STATE_glDepthMask_FALSE = OMNIMAP_DEFAULT_GL_STATE_glDepthMask_FALSE;

	SetUpPropertyAccess();
  strcpy(rendererType, "D3D");
  ScreenRendererFactory.addScreenRendererCreator("D3D", &ScreenRendererD3D::CreateScreenRendererD3D);

	if (doInit)
		init();
}




OmniMapD3D::~OmniMapD3D()
{
	Clear_Channels();
  if (shaders) { delete ((OmniMapShaderD3D *) shaders); shaders = NULL; }
  SAFE_RELEASE(bgBuffer);
	SAFE_RELEASE(logoTexture);
  SAFE_RELEASE(pStateBlock);
  ClearScreenShapes();
	//SAFE_RELEASE(d3dDevice);
}





void OmniMapD3D::SetUpPropertyAccess()
{
  ///////////////
  access.Register("glViewportsettings0",&glViewportsettings0);
  access.Register("glViewportsettings1",&glViewportsettings1);
  access.Register("glViewportsettings2",&glViewportsettings2);
  access.Register("glViewportsettings3",&glViewportsettings3); 
  access.Register("GL_STATE_CLEAR_AT_STARTFRAME",&GL_STATE_CLEAR_AT_STARTFRAME); 
  access.Register("GL_STATE_glDisable_GL_DEPTH_TEST",&GL_STATE_glDisable_GL_DEPTH_TEST); 
  access.Register("GL_STATE_glDepthMask_FALSE",&GL_STATE_glDepthMask_FALSE); 
  access.Register("FXShaderFile", &fxShaderFilename[0]);
  OmniMapBase::SetUpPropertyAccess();
}




void OmniMapD3D::init()
{
  EH_DECLARE;

  std::string setLuaDir = "LuaSupportDir = \"";
  setLuaDir.append(luaSupportDir);
  setLuaDir.append("\"");
  size_t index = 0;
  while ((index = setLuaDir.find("\\", index)) != std::string::npos)
  {
    setLuaDir.replace(index, 1, "/");
    index++;
  }

  EH_Zero(ScriptingEngine->RunString(setLuaDir.c_str()));
  EH_Zero(ScriptingEngine->RunScript(StartUpScript));

  EH_Ptr(shaders = new OmniMapShaderD3D(d3dDevice));

  ((OmniMapShaderD3D *)shaders)->CobraWarpWithTrueDimension = (this)->CobraWarpWithTrueDimension;


  if(CobraWarpWithTrueDimension>0){
    EH_Log("CobraWarpWithTrueDimension (0 is off 1 is Warp with TrueDimension On, 2 is Warp with TrueDimension off ) = %d",CobraWarpWithTrueDimension);
  }
  if (fxShaderFilename[0] != '\0') {
    EH_Log("loading shader %s",fxShaderFilename);
    std::string vertexShaderFilename = luaSupportDir;
    vertexShaderFilename.append(fxShaderFilename);
    ((OmniMapShaderD3D *) shaders)->setShadersFilenames(vertexShaderFilename.c_str(), NULL);
  }
  ((OmniMapShaderD3D *) shaders)->setNumChannels(GetNumChannels());
  
  if(StencilMask_filename)
    ((OmniMapShaderD3D *)shaders)->StencilMask_Load(StencilMask_filename); // the final mask needs to be done before the shader is compiled....
  
  shaders->init();

  for (int i = 0; i < GetNumChannels(); i++)
  {
    ((OmniMapChannelD3D *) this->GetChannel(i))->setOMShader((OmniMapShaderD3D *) shaders);
  }

  InitBG();
  EH_Log("------------OMNIMAP LOADED----------------");

  EH_OnError() {
    EH_Log("------------OMNIMAP FAILED TO LOADED----------------");
    if (shaders) { delete ((OmniMapShaderD3D *) shaders); shaders = NULL; }
  }

  EH_Log("glViewportsettings %f %f %f %f",glViewportsettings0,glViewportsettings1,glViewportsettings2,glViewportsettings3);
  EH_Log("GL_STATE_CLEAR_AT_STARTFRAME, GL_STATE_glDisable_GL_DEPTH_TEST,GL_STATE_glDepthMask_FALSE %d %d %d",	GL_STATE_CLEAR_AT_STARTFRAME, GL_STATE_glDisable_GL_DEPTH_TEST,GL_STATE_glDepthMask_FALSE );
  EH_Log("width %d",		resWidth);
  EH_Log("height %d",		resHeight);
  EH_Log("displayHUD  %d",displayHUD );
  EH_Log("clear color %f %f %f %f",clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a );
  EH_Log("AudiencePosition %f %f %f\n",AudiencePosition.x,AudiencePosition.y,AudiencePosition.z);
  EH_Log("ProjectorPosition.pos %f %f %f\n",ProjectorPosition.pos.x,ProjectorPosition.pos.y,ProjectorPosition.pos.z);
  EH_Log("ProjectorPosition.headsUp %f %f %f\n",ProjectorPosition.headsUp.x,ProjectorPosition.headsUp.y,ProjectorPosition.headsUp.z);
  EH_Log("ProjectorPosition.lookAtpos %f %f %f\n",ProjectorPosition.lookAtpos.x,ProjectorPosition.lookAtpos.y,ProjectorPosition.lookAtpos.z);
  EH_Log("startupscript : %s",StartUpScript);
  EH_Log("------------------------------");
}




#ifdef OmniMap_QUAD_UPGRADE
void OmniMap::Create_ScreenShapeQuad(std::string name)
{
	// prevent name conflict
	String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.find(name);
	if(itr !=OmniMap_GLOBAL_ScreenShapes.end())
	{
		LogSystem()->ReportError("Error:\t ScreenShape Name Conflict %s!\n",name.c_str());
		delete itr->second;
	}

	OmniMap_GLOBAL_ScreenShapes[name]= new OmniMap_Screen_Quad(); 
}
#endif





OmniMapChannelBase *OmniMapD3D::Create_Channel(const char * name) 
{
  name;
  return new OmniMapChannelD3D(d3dDevice);
}





void OmniMapD3D::Clear_Channels()
{
  for (int i = 0; i < GetNumChannels(); i++)
  {
    OmniMapChannelD3D *channelD3D = (OmniMapChannelD3D *) GetChannel(i);
    if (channelD3D != NULL)
      delete channelD3D;
  }
  ClearChannelMap();
}





void OmniMapD3D::SetupAllShaderVaribles()
{
  int N_Channels;
  int TextureIds[OMNIMAP_maxChannelFaces];
  D3DXMATRIX textureMats[OMNIMAP_maxChannelFaces];

  GetShaderVariables(N_Channels, TextureIds);
  ((OmniMapShaderD3D *) shaders)->SetTextureIds(N_Channels);

  for (int i = 0; i < N_Channels; i++)
  {
    OmniMapChannelBase *channel = this->GetChannel(i);
    D3DXQUATERNION tmpQuat;
    D3DXMATRIX *pTextureMat = &textureMats[i];
    D3DXMATRIX tmpMat;
    D3DXMatrixIdentity(pTextureMat);
    const D3DXVECTOR3 zeroVec(0.0,0.0,0.0);
    const D3DXVECTOR3 transVec(.5,.5,.5);

    D3DXMatrixAffineTransformation(pTextureMat, .5, &zeroVec, D3DXQuaternionIdentity(&tmpQuat),
      &transVec);	

    D3DXMATRIX channelProj;
    ((OmniMapChannelD3D *) channel)->GetProjectionMatrixRH(&channelProj);

    D3DXMatrixMultiply(pTextureMat, &channelProj, pTextureMat);
    D3DXMATRIX viewMatrix;
    ((OmniMapChannelD3D *) channel)->GetViewMatrix2(&viewMatrix);
    D3DXMatrixMultiply(pTextureMat, &viewMatrix, pTextureMat);
  }
  ((OmniMapShaderD3D *) shaders)->SetTextureMatrices(textureMats, N_Channels);


  D3DVIEWPORT9 vp;
  d3dDevice->GetViewport(&vp);
  //float yOffset2 = ((glViewportsettings3/2.0)/((float) vp.Height-(float) glViewportsettings1))/2.0;
  float yOffset1, yOffset2, yScale;

  if (((glViewportsettings0 + glViewportsettings2) <= vp.Width)
    && (glViewportsettings0 >= 0)
    && ((glViewportsettings1 + glViewportsettings3) <= vp.Height)
    && (glViewportsettings1 >= 0))
  {
    //xOffset1 = 0.0;
    //xOffset2 = 0.0;
    //xScale = ((float) glViewportsettings2)/ ((float) vp.Width);
    vp.X = (DWORD) glViewportsettings0;
    vp.Y = (DWORD) glViewportsettings1;
    vp.Width = (DWORD) glViewportsettings2;
    vp.Height = (DWORD) glViewportsettings3;
    d3dDevice->SetViewport(&vp);
    yOffset1 = 0.0;
    yOffset2 = 0.0;
    yScale = 1.0;
  } else {
    yOffset2 = (float) ((glViewportsettings3/2.0f)/((float) vp.Height-(float) glViewportsettings1))/2.0f;
    yScale = (((float) glViewportsettings2) / ((float) vp.Height));
    yOffset1 = (((float) glViewportsettings1) / ((float) glViewportsettings3));
  }

  //float xOffset1 = (((float) glViewportsettings0) / vp.Width);
  //float xOffset2 = ((glViewportsettings2/2.0)/((float) vp.Width-(float) glViewportsettings0))/2.0;
  //float xScale = (((float) glViewportsettings3) / ((float) vp.Width));


  if (yOffset1 != 0.0) yOffset2 = 0.0;

  ((OmniMapShaderD3D *) shaders)->omnimapFX->SetFloat( "yScale", yScale);
  ((OmniMapShaderD3D *) shaders)->omnimapFX->SetFloat( "yOffset1", yOffset1);
  ((OmniMapShaderD3D *) shaders)->omnimapFX->SetFloat( "yOffset2", yOffset2);
  ((OmniMapShaderD3D *) shaders)->omnimapFX->SetMatrix( "g_mProjection", &worldViewProjection );
  ((OmniMapShaderD3D *) shaders)->omnimapFX->SetMatrix( "g_mWorldView", &worldView );

  if(StencilMask_filename)
    ((OmniMapShaderD3D *) shaders)->StencilMask_Bind(6);//+1
}





void OmniMapD3D::DrawHeadsUpDisplay()
{
	d3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	DrawHeadsUpDisplays();
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);
}




void OmniMapD3D::LoadAndPush_HUD_Matricies()
{
  D3DXMATRIX projMat;

  D3DXMatrixOrthoRH(&projMat, 2.0f, 2.0f, -1.0f, 1.0f);
  d3dDevice->SetTransform( D3DTS_PROJECTION, &projMat );

  D3DXMATRIX matView;
  D3DXMatrixIdentity( &matView );

  // Identity for both world and view matrices
  d3dDevice->SetTransform( D3DTS_VIEW, &matView );
  d3dDevice->SetTransform( D3DTS_WORLD, &matView );
}




void OmniMapD3D::LoadAndPush_ProjectiveTexturing_Matricies()
{
  D3DXMATRIX matView;
  D3DXVECTOR3 camPos( ProjectorPosition.pos.x, ProjectorPosition.pos.y, ProjectorPosition.pos.z ); // Camera position
  D3DXVECTOR3 camLookat( ProjectorPosition.lookAtpos.x, ProjectorPosition.lookAtpos.y, ProjectorPosition.lookAtpos.z );   // Look-at point
  D3DXVECTOR3 camUp( ProjectorPosition.headsUp.x, ProjectorPosition.headsUp.y, ProjectorPosition.headsUp.z ); // Up vector

  D3DXMatrixOrthoRH(&worldViewProjection, 2.0, 2.0, 0.0, 1.0f);
  D3DXMatrixLookAtRH( &worldView, &camPos, &camLookat, &camUp);
}





void OmniMapD3D::PopMatricies()
{
}





void OmniMapD3D::Clear()
{
  d3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a), 1.0f, 0 );
}





void OmniMapD3D::PostRender()
{
  // Post Render does a few things
  // A0 Clear
  // B1 set the projective shader
  // B2 Set up Shaders 
  // B3 textures
  // B4 Set up matricies
  // C1 Display Screen
  // D0 disable shaders
  // D1 Disable Textures
  // D2 Set active texutre to 0

  // E0 PushAndSetupMatrix
  // E1 draw lua console
  // E2 draw heads up display
  // E7 PopMatrix
  //  8 

  // A. 0 Clear

  //if (pStateBlock == NULL)
  d3dDevice->CreateStateBlock( D3DSBT_ALL, &pStateBlock );

  d3dDevice->BeginScene();
  d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
  d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  d3dDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
  d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

  if(GL_STATE_CLEAR_AT_STARTFRAME)
    Clear();
  //DrawBG(1.0, 10.0);

  // B.1 set the projective shader
  shaders->EnableShader();

  // B.2  set the textures
  BindAllChannelTextures();

  // B.4 Set up matricies
  LoadAndPush_ProjectiveTexturing_Matricies();

  // B.3 Shader Parameters
  SetupAllShaderVaribles();


  // C.1 Display World

  DisplayScreenShapes();

  // Pop Matricies
  //PopMatricies();

  // D0 disable shaders
  shaders->DisableShader(); // use fixed pipeline

  // D1 Disable Textures
  UnBindAllChannelTextures();

  // E0 PushAndSetupMatrix
  LoadAndPush_HUD_Matricies();

  // E1 draw lua console
  //		if(console->display_Console)
  //		  console->DisplayConsole();
  // E2 Draw HUD

  if(displayHUD)
    DrawHeadsUpDisplay();

  // E7 PopMatrix
  PopMatricies();
  d3dDevice->EndScene();

  pStateBlock->Apply();

  SAFE_RELEASE(pStateBlock);
}






void OmniMapD3D::InitBG()
{
}




void OmniMapD3D::DrawBG(float scale, float textureScale)
{
}

