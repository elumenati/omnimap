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
#include "privatePrimitives.h"
#include "omnitypedefs.h"
#include "OM_ErrorHandling.h"





///////////////////////////////////////////////////////////
////////OmniMap_Channel
////////////////////////////////////////////////////////////

OmniMapChannelD3D::OmniMapChannelD3D(IDirect3DDevice9 *_d3dDevice ) :
OmniMapChannelBase()
{
  useOmniMapMatrix = true;
  multiSampleQuality = 0;
  d3dDevice = _d3dDevice;
  renderToSurface = NULL;
  dynamicTexture = NULL;
  textureSurface = NULL;
  wireframeBuffer = NULL;
  rtSurface = NULL;
  rtZBuffer = NULL;
  vertexBuffer = NULL;
  deviceBackBuffer = NULL;


  SetUpPropertyAccess();
  ConsolidateCameraIntoProjectionMatrix = OMNIMAP_DEFAULT_ConsolidateCameraIntoProjectionMatrix;

  // register this varible... this should be placed into a better function
  // like a derived SetUpPropertyAccess_Additional()
  access.Register("ConsolidateCameraIntoProjectionMatrix", &ConsolidateCameraIntoProjectionMatrix);
}




OmniMapChannelD3D::~OmniMapChannelD3D()
{
  SAFE_RELEASE(dynamicTexture);
  SAFE_RELEASE(renderToSurface);
  SAFE_RELEASE(textureSurface);
  SAFE_RELEASE(vertexBuffer);
  SAFE_RELEASE(wireframeBuffer);
  SAFE_RELEASE(rtSurface);
  SAFE_RELEASE(rtZBuffer);
  //int refs = d3dDevice->Release();
  //LogSystem()->ReportMessage("delete OmniMap_Channel()");
}





void OmniMapChannelD3D::Initialize()
{
  D3DSURFACE_DESC desc;
  D3DFORMAT zBufferFormat = D3DFMT_UNKNOWN;
  EH_DECLARE;

  EH_Log("Initialize");
  
  EH_Test(D3DXCreateTexture( d3dDevice, 
    xResolution, 
    yResolution,
    1, 
    D3DUSAGE_RENDERTARGET, 
    D3DFMT_A8R8G8B8, 
    D3DPOOL_DEFAULT, 
    &dynamicTexture ), "Channel Failed to create a texture with the D3DUSAGE_RENDERTARGET usage flag set!");

  EH_Test(dynamicTexture->GetSurfaceLevel( 0, &textureSurface ));

  EH_Log("OmniMapChannelD3D: res[%d,%d] multiSampleQuality=%d renderTarget=%d\n",xResolution,yResolution,multiSampleQuality, renderTarget); 

  if (multiSampleQuality > 0 || renderTarget == PBUFFER)
  {
    EH_Log("Creating Render Target: multiSampleQuality=%d renderTarget=%d\n", multiSampleQuality, renderTarget); 

    EH_Test(d3dDevice->CreateRenderTarget(xResolution, yResolution, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONMASKABLE, (DWORD) multiSampleQuality, false, &rtSurface, NULL),
      "Channel Failed create a render target with multiSampleQuality = %d!", multiSampleQuality);
    EH_Zero(getZBufferFormat(zBufferFormat), "Channel Failed to create a zbuffer for the channel");
    EH_Test(d3dDevice->CreateDepthStencilSurface(  xResolution, yResolution,
      zBufferFormat, 
      D3DMULTISAMPLE_NONMASKABLE, (DWORD) multiSampleQuality,
      TRUE, &rtZBuffer, NULL ), "Channel Failed to create a zbuffer for the channel");
  } else 
  {
    EH_Log("Creating RenderToSurface q:%d, rt:%d",  multiSampleQuality, renderTarget); 
    textureSurface->GetDesc( &desc );

    EH_Test(D3DXCreateRenderToSurface( d3dDevice, 
      xResolution, yResolution,
      desc.Format, 
      TRUE, 
      D3DFMT_D16, 
      &renderToSurface ));
  }

  EH_OnError() {
    SAFE_RELEASE(dynamicTexture);
    SAFE_RELEASE(renderToSurface);
    SAFE_RELEASE(textureSurface);
    SAFE_RELEASE(rtSurface);
    SAFE_RELEASE(rtZBuffer);
  }
}




void OmniMapChannelD3D::GetProjectionMatrixRH(D3DXMATRIX *projMat)
{
  D3DXMatrixPerspectiveOffCenterRH(projMat, cam_info.ClipLeft, cam_info.ClipRight,
    cam_info.ClipBottom, cam_info.ClipTop, 
    cam_info.ClipNear, cam_info.ClipFar);
}



void OmniMapChannelD3D::GetProjectionMatrix(float m[16])
{
  m;
}




void OmniMapChannelD3D::GetProjectionMatrix(D3DXMATRIX *projMat)// assume projection space and identity
{
  D3DXMatrixPerspectiveOffCenterLH(projMat, cam_info.ClipLeft, cam_info.ClipRight,
    cam_info.ClipBottom, cam_info.ClipTop, cam_info.ClipNear, cam_info.ClipFar);
}



void OmniMapChannelD3D::SetNearFar(float fnear, float ffar)
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

}



void OmniMapChannelD3D::ApplyProjectionMatrix()// assume projection space and identity
{
  D3DXMATRIX projectionMatrix;
  D3DXMatrixPerspectiveOffCenterLH(&projectionMatrix, cam_info.ClipLeft, cam_info.ClipRight,
    cam_info.ClipBottom, cam_info.ClipTop, cam_info.ClipNear, cam_info.ClipFar);
  d3dDevice->SetTransform( D3DTS_PROJECTION, &projectionMatrix );
}



void OmniMapChannelD3D::GetViewMatrix(D3DXMATRIX *vm)
{	
  D3DXMatrixIdentity( vm);
  AffMatrix am(cam_info.Matrix_WorldView);
  am.transpose();
  float *amvals = am.returnValues();
  vm->_11 = amvals[0];
  vm->_12 = amvals[1];
  vm->_13 = -amvals[2];

  vm->_21 = amvals[4];
  vm->_22 = amvals[5];
  vm->_23 = -amvals[6];

  vm->_31 = -amvals[8];
  vm->_32 = -amvals[9];
  vm->_33 = amvals[10];

  vm->_41 = amvals[12];
  vm->_42 = amvals[13];
  vm->_43 = -amvals[14];
  vm->_44 = 1.0;
}




void OmniMapChannelD3D::GetViewMatrix2(D3DXMATRIX *vm)
{
  D3DXMatrixIdentity( vm);
  AffMatrix am(cam_info.Matrix_WorldView);
  float *amvals = am.returnValues();
  vm->_11 = amvals[0];
  vm->_12 = amvals[1];
  vm->_13 = amvals[2];
  vm->_14 = amvals[3];

  vm->_21 = amvals[4];
  vm->_22 = amvals[5];
  vm->_23 = amvals[6];
  vm->_24 = amvals[7];

  vm->_31 = amvals[8];
  vm->_32 = amvals[9];
  vm->_33 = amvals[10];
  vm->_34 = amvals[11];

  vm->_41 = amvals[12];
  vm->_42 = amvals[13];
  vm->_43 = amvals[14];
  vm->_44 = amvals[15];
}



void OmniMapChannelD3D::ApplyViewMatrix() // assume world space
{
  D3DXMATRIX viewMatrix;
  D3DXMatrixIdentity( &viewMatrix);

  AffMatrix am(cam_info.Matrix_WorldView);
  am.transpose();
  AffMatrix ami;
  //am.setLookAtGL(OmniVec3(0.0f, 2.0f, -2.50f ), OmniVec3(0.0, 0.0, 0.0), OmniVec3(0.0,1.0,0.0));
  float *amvals = am.returnValues();
  float amarr[16];
  for (int i = 0; i < 16; i++)
    amarr[i] = amvals[i];

  viewMatrix._11 = amarr[0];
  viewMatrix._12 = amarr[1];
  viewMatrix._13 = -amarr[2];

  viewMatrix._21 = amarr[4];
  viewMatrix._22 = amarr[5];
  viewMatrix._23 = -amarr[6];

  viewMatrix._31 = -amarr[8];
  viewMatrix._32 = -amarr[9];
  viewMatrix._33 = amarr[10];

  viewMatrix._41 = amarr[12];
  viewMatrix._42 = amarr[13];
  viewMatrix._43 = -amarr[14];
  viewMatrix._44 = 1.0;

  d3dDevice->MultiplyTransform( D3DTS_PROJECTION, &viewMatrix );
}



// Determine whether we can support multisampling on a A16B16G16R16F render target
void OmniMapChannelD3D::setupMultiSample()
{
  D3DMULTISAMPLE_TYPE g_MaxMultiSampleType;
  DWORD g_dwMultiSampleQuality = 0; 
  D3DCAPS9 Caps;
  IDirect3D9 *pD3D9 = NULL;
  EH_DECLARE;

  EH_Test(d3dDevice->GetDirect3D(&pD3D9));
  EH_Test(d3dDevice->GetDeviceCaps( &Caps ));
  //g_bUseMultiSampleFloat16 = false;

  g_MaxMultiSampleType = D3DMULTISAMPLE_NONE;

  for( D3DMULTISAMPLE_TYPE imst = D3DMULTISAMPLE_2_SAMPLES; imst <= D3DMULTISAMPLE_16_SAMPLES; imst = (D3DMULTISAMPLE_TYPE)(imst + 1) )
  {
    DWORD msQuality = 0;
    ZeroMemory(&Caps, sizeof(D3DCAPS9));
    if( SUCCEEDED( pD3D9->CheckDeviceMultiSampleType( Caps.AdapterOrdinal, 
      Caps.DeviceType, 
      D3DFMT_A8R8G8B8, 
      TRUE, 
      imst, &msQuality ) ) )
    {
      //g_bUseMultiSampleFloat16 = true;
      g_MaxMultiSampleType = imst;
      if( msQuality > 0 )
        g_dwMultiSampleQuality = msQuality-1;
      else
        g_dwMultiSampleQuality = msQuality;
    }
  }

  EH_OnError() {}
  
  SAFE_RELEASE(pD3D9);
}



bool OmniMapChannelD3D::getZBufferFormat(D3DFORMAT &retval)
{
  bool ret = false;
  D3DCAPS9 Caps;
  D3DDISPLAYMODE displayMode = {0};
  IDirect3D9 *pD3D = NULL;
  EH_DECLARE;

  ZeroMemory(&Caps, sizeof(D3DCAPS9));

  EH_Test(d3dDevice->GetDirect3D(&pD3D));
  EH_Test(d3dDevice->GetDeviceCaps( &Caps ));
  EH_Test(pD3D->GetAdapterDisplayMode(Caps.AdapterOrdinal, &displayMode));

  if( SUCCEEDED( pD3D->CheckDeviceFormat( Caps.AdapterOrdinal, Caps.DeviceType,
    displayMode.Format, D3DUSAGE_DEPTHSTENCIL,
    D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
  {
    if( SUCCEEDED( pD3D->CheckDepthStencilMatch( Caps.AdapterOrdinal, Caps.DeviceType,
      displayMode.Format, D3DFMT_A8R8G8B8, 
      D3DFMT_D32 ) ) )
    {
      retval = D3DFMT_D32;
      ret = true;
      EH_GotoOnError(); // this will jump to EH_OnError, in this case no error was detected so EH_OnError block is not processed.
    }
  }

  if( SUCCEEDED( pD3D->CheckDeviceFormat( Caps.AdapterOrdinal, Caps.DeviceType,
    displayMode.Format, D3DUSAGE_DEPTHSTENCIL,
    D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
  {
    if( SUCCEEDED( pD3D->CheckDepthStencilMatch( Caps.AdapterOrdinal, Caps.DeviceType,
      displayMode.Format, D3DFMT_A8R8G8B8, 
      D3DFMT_D16 ) ) )
    {
      retval = D3DFMT_D16;
      ret = true;
      EH_GotoOnError(); // this will jump to EH_OnError, in this case no error was detected so EH_OnError block is not processed.
    }
  }

  EH_OnError() {}

  SAFE_RELEASE(pD3D);

  return ret;
}




void OmniMapChannelD3D::SetUpPropertyAccess()
{
	access.Register("MultiSampleQuality",&multiSampleQuality);
	//OmniMapChannelBase::SetUpPropertyAccess();
}






void OmniMapChannelD3D::CreateHUDSurface()
{
  Vertex *p = NULL;
  WFVertex *p1 = NULL;
  EH_DECLARE;

  EH_Test(d3dDevice->CreateVertexBuffer( 4*sizeof(Vertex),0, Vertex::FVF_Flags,
    D3DPOOL_DEFAULT, &vertexBuffer, NULL ));

  EH_Test(d3dDevice->CreateVertexBuffer( 4*sizeof(WFVertex),0, WFVertex::FVF_WF_FLAGS,
    D3DPOOL_DEFAULT, &wireframeBuffer, NULL ));

  EH_Test(vertexBuffer->Lock( 0, 4*sizeof(Vertex), (void**)&p, 0 ));

  // Vert 1
  p->x = HeadsUpDisplay_Quad.bl.x;
  p->y = HeadsUpDisplay_Quad.bl.y;
  p->z = HeadsUpDisplay_Quad.bl.z;
  p->tu = 0.0;
  p->tv = 1.0;
  // Vert 2
  p++;
  p->x = HeadsUpDisplay_Quad.tl.x;
  p->y = HeadsUpDisplay_Quad.tl.y;
  p->z = HeadsUpDisplay_Quad.tl.z;
  p->tu = 0.0;
  p->tv = 0.0;
  // Vert 3
  p++;
  p->x = HeadsUpDisplay_Quad.br.x;
  p->y = HeadsUpDisplay_Quad.br.y;
  p->z = HeadsUpDisplay_Quad.br.z;
  p->tu = 1.0;
  p->tv = 1.0;
  // Vert 4
  p++;
  p->x = HeadsUpDisplay_Quad.tr.x;
  p->y = HeadsUpDisplay_Quad.tr.y;
  p->z = HeadsUpDisplay_Quad.tr.z;
  p->tu = 1.0;
  p->tv = 0.0;

  vertexBuffer->Unlock();

  EH_Test(wireframeBuffer->Lock( 0, 4*sizeof(WFVertex), (void**)&p1, 0 ));

  // Vert 1
  p1->x = HeadsUpDisplay_Quad.bl.x;
  p1->y = HeadsUpDisplay_Quad.bl.y;
  p1->z = HeadsUpDisplay_Quad.bl.z;
  p1->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );
  // Vert 2
  p1++;
  p1->x = HeadsUpDisplay_Quad.tl.x;
  p1->y = HeadsUpDisplay_Quad.tl.y;
  p1->z = HeadsUpDisplay_Quad.tl.z;
  p1->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );
  // Vert 3
  p1++;
  p1->x = HeadsUpDisplay_Quad.br.x;
  p1->y = HeadsUpDisplay_Quad.br.y;
  p1->z = HeadsUpDisplay_Quad.br.z;
  p1->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );
  // Vert 4
  p1++;
  p1->x = HeadsUpDisplay_Quad.tr.x;
  p1->y = HeadsUpDisplay_Quad.tr.y;
  p1->z = HeadsUpDisplay_Quad.tr.z;
  p1->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );

  wireframeBuffer->Unlock();

  EH_OnError() {
    SAFE_RELEASE(vertexBuffer);
    SAFE_RELEASE(wireframeBuffer);
  }
}




void OmniMapChannelD3D::beginRenderToChannel()
{
  EH_DECLARE;

  if (multiSampleQuality > 0 || renderTarget == PBUFFER)
  {
    EH_Test(d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &deviceBackBuffer),
      "Cannot get back buffer in beginRenderToChannel.");
    EH_Test(d3dDevice->SetRenderTarget(0, rtSurface),
      "Cannot set render target.");
    EH_Test(d3dDevice->SetDepthStencilSurface( rtZBuffer),
      "Cannot set depth stencil surface.");
  } else if (renderTarget == BACK_BUFFER)
  {
    EH_Test(d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &deviceBackBuffer));
  } else
  {
    EH_Test(renderToSurface->BeginScene( textureSurface, NULL ));
  }

  EH_OnError() {
    EH_Log("Cannot set render surface.");
  }

  if (useOmniMapMatrix)
    PushSetupMatricies();
}





void OmniMapChannelD3D::endRenderToChannel()
{
  EH_DECLARE;

  if (useOmniMapMatrix)
    PopSetupMatricies();

  if (multiSampleQuality > 0 || renderTarget == PBUFFER)
  {
    RECT r;
    r.top = 0;
    r.bottom = xResolution;
    r.left = 0;
    r.right = yResolution;
    EH_Test(d3dDevice->StretchRect(rtSurface, (const RECT *) &r, textureSurface, (const RECT *) &r, D3DTEXF_LINEAR));
    EH_Test(d3dDevice->SetRenderTarget(0, deviceBackBuffer));
  } else if (renderTarget == BACK_BUFFER)
  {
    RECT r;
    r.top = 0;
    r.bottom = xResolution;
    r.left = 0;
    r.right = yResolution;
    EH_Test(d3dDevice->StretchRect(deviceBackBuffer, (const RECT *) &r, textureSurface, (const RECT *) &r, D3DTEXF_LINEAR));
  } else
  {
    EH_Test(renderToSurface->EndScene(0));
  }

  EH_OnError() {
    EH_Log("Cannot draw to surface.");
  }

  SAFE_RELEASE(deviceBackBuffer);
}





void OmniMapChannelD3D::PushSetupMatricies()
{
  d3dDevice->GetTransform(D3DTS_PROJECTION, &saveProjMatrix);
  ApplyProjectionMatrix();
  d3dDevice->GetTransform(D3DTS_VIEW, &saveViewMatrix);
  if(ConsolidateCameraIntoProjectionMatrix)
    ApplyViewMatrix();
}




void OmniMapChannelD3D::PopSetupMatricies()
{
	d3dDevice->SetTransform(D3DTS_PROJECTION, &saveProjMatrix);
	d3dDevice->SetTransform(D3DTS_VIEW, &saveViewMatrix);
}




void OmniMapChannelD3D::GetProjTextMatrix(float m[16])
{
	D3DXQUATERNION tmpQuat;
	D3DXMATRIX textureMat;
	D3DXMATRIX tmpMat;
	D3DXMatrixIdentity(&textureMat);
	const D3DXVECTOR3 zeroVec(0.0,0.0,0.0);
	const D3DXVECTOR3 transVec(.5,.5,.5);

	D3DXMatrixAffineTransformation(&textureMat, .5, &zeroVec, D3DXQuaternionIdentity(&tmpQuat),
		&transVec);	
	
	D3DXMATRIX channelProj;
	GetProjectionMatrixRH(&channelProj);

	D3DXMatrixMultiply(&textureMat, &channelProj, &textureMat);
	D3DXMATRIX viewMatrix;
	GetViewMatrix2(&viewMatrix);
	D3DXMatrixMultiply(&textureMat, &viewMatrix, &textureMat);
	for (int i = 0; i < 16; i++)
	{
		m[i] = textureMat[i];
	}
}



void OmniMapChannelD3D::GetProjTextMatrix(D3DXMATRIX *ptMat)
{
	D3DXQUATERNION tmpQuat;
	D3DXMATRIX textureMat;
	D3DXMATRIX tmpMat;
	D3DXMatrixIdentity(&textureMat);
	const D3DXVECTOR3 zeroVec(0.0,0.0,0.0);
	const D3DXVECTOR3 transVec(.5,.5,.5);

	D3DXMatrixAffineTransformation(&textureMat, .5, &zeroVec, D3DXQuaternionIdentity(&tmpQuat),
		&transVec);	
	
	D3DXMATRIX channelProj;
	GetProjectionMatrixRH(&channelProj);

	D3DXMatrixMultiply(&textureMat, &channelProj, &textureMat);
	D3DXMATRIX viewMatrix;
	GetViewMatrix2(&viewMatrix);
	D3DXMatrixMultiply(ptMat, &viewMatrix, &textureMat);
	
}



#ifdef xOM_PICKING
bool OmniMapChannelD3D::screenXYToChannelXY(float screenGeomLoc[4], int &channelX, int &channelY)
{
	

	D3DXMATRIX textMat;
	GetProjTextMatrix(&textMat);
	
	
	CONST D3DXVECTOR3 screenPt(screenGeomLoc[0], screenGeomLoc[1], screenGeomLoc[2]);
	D3DXVECTOR4 gpoint;
	D3DXVec3Transform(
		&gpoint,
		&screenPt,
		&textMat
	);
	

	float x = gpoint[0]/gpoint[3];
	float y = gpoint[1]/gpoint[3];
	channelX = (x) * (float) this->xResolution;
	channelY = ((y) * (float) this->yResolution);
	if ((x >= 0.0) && (x <= 1.0) &&
 		(y >= 0.0) && (y <= 1.0)/* && screenGeomLoc[2] > 0.0*/)
	{
		return(true);
	} 
	return(false);
}
#endif




void OmniMapChannelD3D::setOMShader(OmniMapShaderD3D *shader)
{
	omShader = shader;
}




void OmniMapChannelD3D::BindTexture(int index)
{
  EH_DECLARE;
	EH_Test( d3dDevice->SetSamplerState( index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
  EH_Test( d3dDevice->SetSamplerState( index, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
  EH_Test( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) );
	EH_Test( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) );
	omShader->setTexture(index, dynamicTexture);
	//OmniMapD3D::textureTable[index] = dynamicTexture;
  EH_OnError() {}
}




void OmniMapChannelD3D::UnbindTexture(int index)
{
	omShader->setTexture(index,  NULL);
}




void OmniMapChannelD3D::DrawHeadsUpDisplay()
{
  EH_DECLARE;

	if (vertexBuffer == NULL) CreateHUDSurface();
	
	EH_Test(d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));
  EH_Test(d3dDevice->SetStreamSource( 0, wireframeBuffer, 0, sizeof(WFVertex) ));
  EH_Test(d3dDevice->SetFVF( WFVertex::FVF_WF_FLAGS ));
	EH_Test(d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 ));

	EH_Test(d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
  EH_Test(d3dDevice->SetTexture( 0, dynamicTexture ));
  EH_Test(d3dDevice->SetStreamSource( 0, vertexBuffer, 0, sizeof(Vertex) ));
  EH_Test(d3dDevice->SetFVF( Vertex::FVF_Flags ));
	EH_Test(d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 ));

  EH_OnError() {
    EH_Log("Cannot render HUD");
  }
}



