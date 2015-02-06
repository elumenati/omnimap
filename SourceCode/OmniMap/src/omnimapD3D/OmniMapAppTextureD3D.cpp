// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#include "OmniMapAppTextureD3D.h"
#include "OmniMapShaderD3D.h"

extern int OmniMap_getGL_TEXTUREN_ARB_From_N(int index);

OmniMapAppTextureD3D::OmniMapAppTextureD3D(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice,
		const char* strStartUpScriptFile,
		const char *luaResDir) :
	OmniMapD3D(_resW, _resH, _d3dDevice, strStartUpScriptFile, luaResDir, false)
{
}

OmniMapAppTextureD3D::~OmniMapAppTextureD3D()
{
	Clear_Channels();
	
}

void OmniMapAppTextureD3D::Clear_Channels()
{
	for (int i = 0; i < GetNumChannels(); i++)
	{
		OmniMapChannelATD3D *channelD3D = (OmniMapChannelATD3D *) GetChannel(i);
		if (channelD3D != NULL)
			delete channelD3D;
	}
	ClearChannelMap();
}

OmniMapChannelBase *OmniMapAppTextureD3D::Create_Channel(const char * name) 
{
	OmniMapChannelATD3D *channel = new OmniMapChannelATD3D(d3dDevice);
	return(channel);
}

OmniMapChannelATD3D::OmniMapChannelATD3D(IDirect3DDevice9 *_d3dDevice) :
	OmniMapChannelD3D(_d3dDevice)
{
}

OmniMapChannelATD3D::~OmniMapChannelATD3D()
{
}
	
void OmniMapChannelATD3D::BindTexture(int index)
{
	HRESULT hr;
	OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
    OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
	OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) );
	OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) );
	omShader->setTexture(index, dynamicTexture);
}

/*	
void OmniMapChannelATD3D::DrawHeadsUpDisplay()
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
*/


void OmniMapChannelATD3D::UnbindTexture(int index)
{
	omShader->setTexture(index, NULL);
}

void OmniMapChannelATD3D::SetTextureID(LPDIRECT3DTEXTURE9 id)
{
	dynamicTexture = id;
}
LPDIRECT3DTEXTURE9 OmniMapChannelATD3D::GetTextureID()
{
	return(dynamicTexture);
}

void OmniMapChannelATD3D::Initialize()
{
	LogSystem()->ReportMessage("OmniMapChannelATD3D::Initialize %d %d (this rez is ignored, the application is required to provide its own channel texture)",xResolution,yResolution);
}

void OmniMapChannelATD3D::beginRenderToChannel()
{
    if (useOmniMapMatrix)
        PushSetupMatricies();

}
void OmniMapChannelATD3D::endRenderToChannel()
{
    if (useOmniMapMatrix)
	    PopSetupMatricies();
}

