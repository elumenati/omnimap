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
#include "ColorLogoDDS.h"
#include "OM_Protector.h"

#include "../omnimap/privatePrimitives.h"
// JTCHANGE: need defn. of SAVE_AFTER_EVERY_MESSAGE
#include "../omnimap/omnimap_luatable.h"
#include "D3DTools/SimpleDomeD3D.h"

///////////////////////////////////////////////

#include "../omnimap/omnitypedefs.h"



bool SaveStringList(std::string filename , stringList *List);

/////////////////////////////////////////////////

#include <stdarg.h>
//#ifdef OMNIMAP_EXPORTS
///////////////////

bool OmniMap_ispowerof2(int number);

int OmniMap_getNumberFromGL_TEXTUREID_ARB(int enu);
int OmniMap_getGL_TEXTUREN_ARB_From_N(int n);
//#endif

#ifdef _MANAGED
#pragma managed(push, off)
#endif




#ifdef _MANAGED
#pragma managed(pop)
#endif


void OmniMapChannelD3D::GetProjectionMatrixRH(D3DXMATRIX *projMat)
{
	D3DXMatrixPerspectiveOffCenterRH(projMat, cam_info.ClipLeft, cam_info.ClipRight,
			cam_info.ClipBottom, cam_info.ClipTop, 
			cam_info.ClipNear, cam_info.ClipFar);
}

void OmniMapChannelD3D::GetProjectionMatrix(float m[16])
{
	

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

	
	SetUpPropertyAccess();
	ConsolidateCameraIntoProjectionMatrix = OMNIMAP_DEFAULT_ConsolidateCameraIntoProjectionMatrix;

	// register this varible... this should be placed into a better function
	// like a derived SetUpPropertyAccess_Additional()
	access.Register("ConsolidateCameraIntoProjectionMatrix",&ConsolidateCameraIntoProjectionMatrix);

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
	LogSystem()->ReportMessage("delete OmniMap_Channel()");
}

// Determine whether we can support multisampling on a A16B16G16R16F render target
void OmniMapChannelD3D::setupMultiSample()
{
	D3DMULTISAMPLE_TYPE g_MaxMultiSampleType;
	DWORD       g_dwMultiSampleQuality = 0; 
	HRESULT hr;
	D3DCAPS9 Caps;
	IDirect3D9 *pD3D9;
	hr = d3dDevice->GetDirect3D(&pD3D9);
	d3dDevice->GetDeviceCaps( &Caps );
    //g_bUseMultiSampleFloat16 = false;
	
    g_MaxMultiSampleType = D3DMULTISAMPLE_NONE;

    for( D3DMULTISAMPLE_TYPE imst = D3DMULTISAMPLE_2_SAMPLES; imst <= D3DMULTISAMPLE_16_SAMPLES; imst = (D3DMULTISAMPLE_TYPE)(imst + 1) )
    {
        DWORD msQuality = 0;
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
}

bool OmniMapChannelD3D::getZBufferFormat(D3DFORMAT &retval)
{
	HRESULT hr;
	D3DCAPS9 Caps;
    IDirect3D9 *pD3D;
	hr = d3dDevice->GetDirect3D(&pD3D);
	d3dDevice->GetDeviceCaps( &Caps );
	D3DDISPLAYMODE displayMode;
	pD3D->GetAdapterDisplayMode(Caps.AdapterOrdinal, &displayMode);

    if( SUCCEEDED( pD3D->CheckDeviceFormat( Caps.AdapterOrdinal, Caps.DeviceType,
                                            displayMode.Format, D3DUSAGE_DEPTHSTENCIL,
                                             D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
    {
        if( SUCCEEDED( pD3D->CheckDepthStencilMatch( Caps.AdapterOrdinal, Caps.DeviceType,
                                                displayMode.Format, D3DFMT_A8R8G8B8, 
                                                D3DFMT_D32 ) ) )
        {
            retval = D3DFMT_D32;
			return(true);
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
			return(true);
        }
    }
	return(false);
}

void OmniMapChannelD3D::Initialize()
{
	LogSystem()->ReportMessage("OmniMapChannelD3D::Initialize");
	HRESULT hr;
	hr = D3DXCreateTexture( d3dDevice, 
		                    xResolution, 
		                    yResolution,
							1, 
							D3DUSAGE_RENDERTARGET, 
							D3DFMT_A8R8G8B8, 
							D3DPOOL_DEFAULT, 
 							&dynamicTexture );

	if( FAILED(hr) )
	{
		LogSystem()->ReportError("Channel Failed to create a texture with the D3DUSAGE_RENDERTARGET usage flag set!");
    }

    dynamicTexture->GetSurfaceLevel( 0, &textureSurface );
LogSystem()->ReportMessage("OmniMapChannelD3D: res[%d,%d] multiSampleQuality=%d renderTarget=%d\n",xResolution,yResolution,multiSampleQuality, renderTarget); 
		
	if (multiSampleQuality > 0 || renderTarget == PBUFFER)
	{
		LogSystem()->ReportMessage("Creating Render Target: multiSampleQuality=%d renderTarget=%d\n",
				multiSampleQuality, renderTarget); 
		hr = d3dDevice->CreateRenderTarget(xResolution, yResolution, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONMASKABLE,
				(DWORD) multiSampleQuality, false, &rtSurface, NULL);
		if( FAILED(hr) )
		{
			LogSystem()->ReportError("Channel Failed create a render target with multiSampleQuality = %d!", multiSampleQuality);
		} else
		{
			D3DFORMAT zBufferFormat;
			if (getZBufferFormat(zBufferFormat))
			{
				hr = d3dDevice->CreateDepthStencilSurface(  xResolution, yResolution,
												   zBufferFormat, 
												   D3DMULTISAMPLE_NONMASKABLE, (DWORD) multiSampleQuality,
												   TRUE, &rtZBuffer, NULL );
				if( FAILED(hr) )
				{
					LogSystem()->ReportError("Channel Failed to create a zbuffer for the channel");
				}
			} else
				LogSystem()->ReportError("Channel Failed to create a zbuffer for the channel");
		}
	} else 
	{
		LogSystem()->ReportMessage("Creating RenderToSurface q:%d, rt:%d",
				multiSampleQuality, renderTarget); 
		D3DSURFACE_DESC desc;
		textureSurface->GetDesc( &desc );

		hr = D3DXCreateRenderToSurface( d3dDevice, 
		                            xResolution, yResolution,
		                            desc.Format, 
		                            TRUE, 
		                            D3DFMT_D16, 
		                            &renderToSurface );
	}
	vertexBuffer = NULL;
		
	 

}

void OmniMapChannelD3D::SetUpPropertyAccess()
{
	
	access.Register("MultiSampleQuality",&multiSampleQuality);
	//OmniMapChannelBase::SetUpPropertyAccess();
}



struct LogoVertex
{
	float x, y, z;
	//DWORD color;
	float tu, tv;
	
	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ /*| D3DFVF_DIFFUSE */| D3DFVF_TEX1 
	};
};

IDirect3DVertexBuffer9 *bgBuffer;
LPDIRECT3DTEXTURE9 logoTexture;
void OmniMapD3D::InitBG()
{
	HRESULT hr;
	hr = d3dDevice->CreateVertexBuffer( 4*sizeof(LogoVertex),0, LogoVertex::FVF_Flags,
                                      D3DPOOL_DEFAULT, &bgBuffer, NULL );


	void *pVertices = NULL;

    bgBuffer->Lock( 0, 4*sizeof(LogoVertex), (void**)&pVertices, 0 );
	LogoVertex *p = (LogoVertex *) pVertices;

	// Vert 1
	p->x = -1.0; p->y = -5.0; p->z = 0.0;
	//p->color = 0x7f00ffff;
	p->tu = 0.0; p->tv = 0.0;
	// Vert 2
	p++;
	p->x = -1.0; p->y = -3.0; p->z = 0.0;
	//p->color = 0x7f00ffff;
	p->tu = 0.0; p->tv = -1.0;
	// Vert 3
	p++;
	p->x = 1.0; p->y = -5.0; p->z = 0.0;
	//p->color = 0x7f00ffff;
	p->tu = 1.0; p->tv = 0.0;
	// Vert 4
	p++;
	p->x = 1.0; p->y = -3.0; p->z = 0.0;
	//p->color = 0x7f00ffff;
	p->tu = 1.0; p->tv = -1.0;
    bgBuffer->Unlock();

	for (int i = 131; i < 128+(128*128*4); i += 4)
	{
		if (logoData[i] != '\0')
			logoData[i] = '\133';
	}
	hr = D3DXCreateTextureFromFileInMemory(d3dDevice, logoData, sizeof(logoData), &logoTexture);

}



void OmniMapChannelD3D::CreateHUDSurface()
{
	d3dDevice->CreateVertexBuffer( 4*sizeof(Vertex),0, Vertex::FVF_Flags,
                                      D3DPOOL_DEFAULT, &vertexBuffer, NULL );

	d3dDevice->CreateVertexBuffer( 4*sizeof(WFVertex),0, WFVertex::FVF_WF_FLAGS,
                                      D3DPOOL_DEFAULT, &wireframeBuffer, NULL );

	void *pVertices = NULL;

    vertexBuffer->Lock( 0, 4*sizeof(Vertex), (void**)&pVertices, 0 );
	Vertex *p = (Vertex *) pVertices;

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

	wireframeBuffer->Lock( 0, 4*sizeof(Vertex), (void**)&pVertices, 0 );
	WFVertex *p1 = (WFVertex *) pVertices;

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
	
}


void OmniMapChannelD3D::beginRenderToChannel()
{
	HRESULT hr;

	if (multiSampleQuality > 0 || renderTarget == PBUFFER)
	{
		hr = d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &deviceBackBuffer);
		if (FAILED(hr))
			LogSystem()->ReportError("Cannot get back buffer in beginRenderToChannel.");
		hr = d3dDevice->SetRenderTarget(0, rtSurface);
		if (FAILED(hr))
			LogSystem()->ReportError("Cannot set render target.");
		hr = d3dDevice->SetDepthStencilSurface( rtZBuffer);
		if (FAILED(hr))
			LogSystem()->ReportError("Cannot set depth stencil surface.");
		//hr = d3dDevice->BeginScene();
	} else if (renderTarget == BACK_BUFFER)
	{
		hr = d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &deviceBackBuffer);
	} else
	{
		hr = renderToSurface->BeginScene( textureSurface, NULL );
	}
	
	if (FAILED(hr))
		LogSystem()->ReportError("Cannot set render surface.");
        
    if (useOmniMapMatrix)
        PushSetupMatricies();
}


void OmniMapChannelD3D::endRenderToChannel()
{
	HRESULT hr;
    if (useOmniMapMatrix)
	    PopSetupMatricies();

	if (multiSampleQuality > 0 || renderTarget == PBUFFER)
	{
		//d3dDevice->EndScene();
		RECT r;
		r.top = 0;
		r.bottom = xResolution;
		r.left = 0;
		r.right = yResolution;
		d3dDevice->StretchRect(rtSurface, (const RECT *) &r, textureSurface, (const RECT *) &r, D3DTEXF_LINEAR);
		hr = d3dDevice->SetRenderTarget(0, deviceBackBuffer);
		SAFE_RELEASE(deviceBackBuffer);

		
	} else if (renderTarget == BACK_BUFFER)
	{
		RECT r;
		r.top = 0;
		r.bottom = xResolution;
		r.left = 0;
		r.right = yResolution;
		d3dDevice->StretchRect(deviceBackBuffer, (const RECT *) &r, textureSurface, (const RECT *) &r, D3DTEXF_LINEAR);
		SAFE_RELEASE(deviceBackBuffer);
	} else
	{
		renderToSurface->EndScene(0);
	}
	

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

	std::string setLuaDir = "LuaSupportDir = \"";
	setLuaDir.append(luaSupportDir);
	setLuaDir.append("\"");
	size_t index = 0;
	while ((index = setLuaDir.find("\\", index)) != std::string::npos)
	{
		setLuaDir.replace(index, 1, "/");
		index++;
	}
	
	ScriptingEngine->RunString(setLuaDir.c_str());
	ScriptingEngine->RunScript(StartUpScript);

	shaders = new OmniMapShaderD3D(d3dDevice);

	((OmniMapShaderD3D *)shaders)->CobraWarpWithTrueDimension = (this)->CobraWarpWithTrueDimension;


	if(CobraWarpWithTrueDimension>0){
		LogSystem()->ReportMessage("CobraWarpWithTrueDimension (0 is off 1 is Warp with TrueDimension On, 2 is Warp with TrueDimension off ) = %d",CobraWarpWithTrueDimension);
	}
	if (fxShaderFilename[0] != '\0') {
		LogSystem()->ReportMessage("loading shader %s",fxShaderFilename);
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
	LogSystem()->ReportMessage("------------OMNIMAP LOADED----------------");
	LogSystem()->ReportMessage("glViewportsettings %f %f %f %f",glViewportsettings0,glViewportsettings1,glViewportsettings2,glViewportsettings3);
	LogSystem()->ReportMessage("GL_STATE_CLEAR_AT_STARTFRAME, GL_STATE_glDisable_GL_DEPTH_TEST,GL_STATE_glDepthMask_FALSE %d %d %d",	GL_STATE_CLEAR_AT_STARTFRAME, 
		GL_STATE_glDisable_GL_DEPTH_TEST,GL_STATE_glDepthMask_FALSE );
	LogSystem()->ReportMessage("width %d",		resWidth);
	LogSystem()->ReportMessage("height %d",		resHeight);
	LogSystem()->ReportMessage("displayHUD  %d",displayHUD );
	LogSystem()->ReportMessage("clear color %f %f %f %f",clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a );
	LogSystem()->ReportMessage("AudiencePosition %f %f %f\n",AudiencePosition.x,AudiencePosition.y,AudiencePosition.z);
	LogSystem()->ReportMessage("ProjectorPosition.pos %f %f %f\n",ProjectorPosition.pos.x,ProjectorPosition.pos.y,ProjectorPosition.pos.z);
	LogSystem()->ReportMessage("ProjectorPosition.headsUp %f %f %f\n",ProjectorPosition.headsUp.x,ProjectorPosition.headsUp.y,ProjectorPosition.headsUp.z);
	LogSystem()->ReportMessage("ProjectorPosition.lookAtpos %f %f %f\n",ProjectorPosition.lookAtpos.x,ProjectorPosition.lookAtpos.y,ProjectorPosition.lookAtpos.z);
	LogSystem()->ReportMessage("startupscript : %s",StartUpScript);
	LogSystem()->ReportMessage("------------------------------");

}


OmniMapD3D::OmniMapD3D(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice, const char* strStartUpScriptFile, 
					   const char *luaResDir, bool doInit) :
	OmniMapBase(_resW, _resH, strStartUpScriptFile, luaResDir)
{
	LogSystem()->ReportMessage("Constructing OmniMapD3D");

	graphicsContext = (void *) _d3dDevice;
	fxShaderFilename[0] = '\0';
	d3dDevice = _d3dDevice;

	pStateBlock = NULL;

	glViewportsettings0 = OMNIMAP_DEFAULT_glViewportsettings0;
	glViewportsettings1 = OMNIMAP_DEFAULT_glViewportsettings1;
	glViewportsettings2 = OMNIMAP_DEFAULT_glViewportsettings2;
	glViewportsettings3 = OMNIMAP_DEFAULT_glViewportsettings3;
	GL_STATE_CLEAR_AT_STARTFRAME = OMNIMAP_DEFAULT_GL_STATE_CLEAR_AT_STARTFRAME;
	GL_STATE_glDisable_GL_DEPTH_TEST = OMNIMAP_DEFAULT_GL_STATE_glDisable_GL_DEPTH_TEST;
	GL_STATE_glDepthMask_FALSE = OMNIMAP_DEFAULT_GL_STATE_glDepthMask_FALSE;

	SetUpPropertyAccess();
	ScreenRendererFactory.addScreenRendererCreator("D3D", &ScreenRendererD3D::CreateScreenRendererD3D);

	if (doInit)
		init();
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


OmniMapD3D::~OmniMapD3D()
{
	Clear_Channels();
	delete ((OmniMapShaderD3D *) shaders);
	SAFE_RELEASE(pStateBlock);
	SAFE_RELEASE(logoTexture);
	SAFE_RELEASE(bgBuffer);
	shaders = NULL;
	ClearScreenShapes();
	//SAFE_RELEASE(d3dDevice);
}


OmniMapChannelBase *OmniMapD3D::Create_Channel(const char * name) 
{
	OmniMapChannelD3D *channel = new OmniMapChannelD3D(d3dDevice);
	return(channel);
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


void OmniMapD3D::SetupAllShaderVaribles()
{
	int N_Channels;
	int TextureIds[OMNIMAP_maxChannelFaces];

	D3DXMATRIX textureMats[6];
	GetShaderVariables(N_Channels, TextureIds);
	((OmniMapShaderD3D *) shaders)->SetTextureIds(N_Channels);

	for (int i = 0; i < N_Channels; i++)
	{
		OmniMapChannelBase *channel = this->GetChannel(i);
		const char *channame = this->GetChannelName(channel);
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

void OmniMapChannelD3D::setOMShader(OmniMapShaderD3D *shader)
{
	omShader = shader;
}

void OmniMapChannelD3D::BindTexture(int index)
{
	HRESULT hr;
	OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
    OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
	OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) );
	OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) );
	omShader->setTexture(index, dynamicTexture);
	//OmniMapD3D::textureTable[index] = dynamicTexture;
}


void OmniMapChannelD3D::UnbindTexture(int index)
{
	omShader->setTexture(index,  NULL);

}


void OmniMapChannelD3D::DrawHeadsUpDisplay()
{

	HRESULT hr;
	if (vertexBuffer == NULL) 
	{
		CreateHUDSurface();
	}
	
	hr = d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    hr = d3dDevice->SetStreamSource( 0, wireframeBuffer, 0, sizeof(WFVertex) );
    hr = d3dDevice->SetFVF( WFVertex::FVF_WF_FLAGS );
	hr = d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );
	if (FAILED(hr))
		LogSystem()->ReportError("Cannot render HUD");

	hr = d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    hr = d3dDevice->SetTexture( 0, dynamicTexture );
    hr = d3dDevice->SetStreamSource( 0, vertexBuffer, 0, sizeof(Vertex) );
    hr = d3dDevice->SetFVF( Vertex::FVF_Flags );
	hr = d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );
	if (FAILED(hr))
		LogSystem()->ReportError("Cannot render HUD");

}


void OmniMapD3D::LoadAndPush_HUD_Matricies()
{


	D3DXMATRIX projMat;

	D3DXMatrixOrthoRH(
		&projMat, 2.0f, 2.0f, -1.0f, 1.0f);

	d3dDevice->SetTransform( D3DTS_PROJECTION, &projMat );

	D3DXMATRIX matView;
    D3DXMatrixIdentity( &matView );

	// Identity for both world and view matrices
	d3dDevice->SetTransform( D3DTS_VIEW, &matView );

	d3dDevice->SetTransform( D3DTS_WORLD, &matView );
}


void OmniMapD3D::LoadAndPush_ProjectiveTexturing_Matricies()
{
	//HRESULT hr;
	
	D3DXMatrixOrthoRH(
		&worldViewProjection, 2.0, 2.0, 0.0, 1.0f);
	
    D3DXMATRIX matView;
    D3DXMatrixLookAtRH( &worldView,
		&D3DXVECTOR3( ProjectorPosition.pos.x, ProjectorPosition.pos.y, ProjectorPosition.pos.z ), // Camera position
        &D3DXVECTOR3( ProjectorPosition.lookAtpos.x, ProjectorPosition.lookAtpos.y, ProjectorPosition.lookAtpos.z ),   // Look-at point
        &D3DXVECTOR3( ProjectorPosition.headsUp.x, ProjectorPosition.headsUp.y, ProjectorPosition.headsUp.z ) ); // Up vector

}

void OmniMapD3D::PopMatricies()
{

}


void OmniMapD3D::Clear()
{
	HRESULT hr;
	 hr = d3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a), 1.0f, 0 );
	 if (FAILED(hr))
		 LogSystem()->ReportError("Cannot clear\n");

}

void OmniMapD3D::DrawBG(float scale, float textureScale)
{
	HRESULT hr = 0;
	D3DXMATRIX projMat;

	D3DXMatrixOrthoRH(
		&projMat, 2.0f, 2.0f, -1.0f, 1.0f);

	d3dDevice->SetTransform( D3DTS_PROJECTION, &projMat );
	

	D3DXMATRIX matView;
    D3DXMatrixIdentity( &matView );

	// Identity for both world and view matrices
	d3dDevice->SetTransform( D3DTS_VIEW, &matView );
	d3dDevice->SetTransform( D3DTS_WORLD, &matView );
	d3dDevice->SetTransform( D3DTS_WORLD, 
		D3DXMatrixScaling(&matView, scale, scale, scale) );
	d3dDevice->SetTransform( D3DTS_TEXTURE0, 
		D3DXMatrixScaling(&matView, textureScale, textureScale, textureScale) );

	hr = d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);		//alpha blending enabled
	hr = d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);	//source blend factor
	hr = d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	//destination blend factor
	hr = d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	hr = d3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, 
                                 D3DTTFF_COUNT2 );


	hr = d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    hr = d3dDevice->SetTexture( 0, logoTexture );
    hr = d3dDevice->SetStreamSource( 0, bgBuffer, 0, sizeof(LogoVertex) );
    hr = d3dDevice->SetFVF( Vertex::FVF_Flags );
	hr = d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );
	if (FAILED(hr))
		LogSystem()->ReportError("Cannot render HUD");


}

void OmniMapD3D::PostRender()
{
	HRESULT hr = 0;

	
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
		hr = d3dDevice->CreateStateBlock( D3DSBT_ALL, &pStateBlock );
	
	hr = d3dDevice->BeginScene();
	hr = d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    hr = d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    hr = d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	hr = d3dDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
	hr = d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	
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
	if (om_protector.isProtected())
		DrawBG(.2f, 1.0f);
	d3dDevice->EndScene();

	pStateBlock->Apply();
	SAFE_RELEASE(pStateBlock);


}



