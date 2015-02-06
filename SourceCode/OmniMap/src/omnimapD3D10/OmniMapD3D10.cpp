
#include <string>
#include <vector>
#include "OmniMapD3D10.h"
#include "OmniMapChannelD3D10.h"
#include "OmniMapShaderD3D10.h"
#include "ScreenRendererD3D10.h"
#include "ColorLogoDDS.h"
#include "OM_Protector.h"

#pragma warning (disable:4244)

static ID3D10Buffer *logoBuffer;
static ID3D10Texture2D *logoTexture;
static ID3D10ShaderResourceView *logoShaderResView;

void OmniMapD3D10::construct(bool doInit)
{
	pStateBlock = NULL;
	//om_protector.unlock("","");

	glViewportsettings0 = OMNIMAP_DEFAULT_glViewportsettings0;
	glViewportsettings1 = OMNIMAP_DEFAULT_glViewportsettings1;
	glViewportsettings2 = OMNIMAP_DEFAULT_glViewportsettings2;
	glViewportsettings3 = OMNIMAP_DEFAULT_glViewportsettings3;
	GL_STATE_CLEAR_AT_STARTFRAME = OMNIMAP_DEFAULT_GL_STATE_CLEAR_AT_STARTFRAME;
	GL_STATE_glDisable_GL_DEPTH_TEST = OMNIMAP_DEFAULT_GL_STATE_glDisable_GL_DEPTH_TEST;
	GL_STATE_glDepthMask_FALSE = OMNIMAP_DEFAULT_GL_STATE_glDepthMask_FALSE;
	
	

	SetUpPropertyAccess();

	ScreenRendererFactory.addScreenRendererCreator("D3D10", &ScreenRendererD3D10::CreateScreenRendererD3D10);


	if (doInit)
		init();
}

OmniMapD3D10::OmniMapD3D10(int _resW, int _resH, ID3D10Device *_d3dDevice, const char* strStartUpScriptFile, 
					   const char *luaResDir, bool doInit) :
	OmniMapBase(_resW, _resH, strStartUpScriptFile, luaResDir)
{
	LogSystem()->ReportMessage("Constructing OmniMapD3D10");

	shaders = NULL;
	logoBuffer = NULL;
	logoTexture = NULL;
	logoShaderResView = NULL;

	graphicsContext = (void *) _d3dDevice;

	fxShaderFilename[0] = '\0';

	d3d10Device = _d3dDevice;

	pStateBlock = NULL;
	pRasterState = NULL;
	construct(doInit);

}

OmniMapD3D10::~OmniMapD3D10()
{

	SAFE_RELEASE(pStateBlock);
	SAFE_RELEASE(pRasterState);

	SAFE_RELEASE(logoBuffer);
	SAFE_RELEASE(logoTexture);
	SAFE_RELEASE(logoShaderResView);

	Clear_Channels();

	if (shaders != NULL)
		delete ((OmniMapShaderD3D10 *) shaders);

	shaders = NULL;

	ClearScreenShapes();

}

void OmniMapD3D10::SetUpPropertyAccess()
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

void OmniMapD3D10::init()
{
	HRESULT hr;

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

	D3D10_RASTERIZER_DESC CurrentRasterizerState;
    CurrentRasterizerState.FillMode = D3D10_FILL_SOLID;
    CurrentRasterizerState.CullMode = D3D10_CULL_NONE;
    CurrentRasterizerState.FrontCounterClockwise = true;
    CurrentRasterizerState.DepthBias = false;
    CurrentRasterizerState.DepthBiasClamp = 0;
    CurrentRasterizerState.SlopeScaledDepthBias = 0;
    CurrentRasterizerState.DepthClipEnable = !GL_STATE_glDisable_GL_DEPTH_TEST;
    CurrentRasterizerState.ScissorEnable = false;
    CurrentRasterizerState.MultisampleEnable = false;
    CurrentRasterizerState.AntialiasedLineEnable = false;
	CurrentRasterizerState.MultisampleEnable = true;
    hr = d3d10Device->CreateRasterizerState(&CurrentRasterizerState, &pRasterState);

	
	
	shaders = new OmniMapShaderD3D10(d3d10Device);




	((OmniMapShaderD3D10 *)shaders)->CobraWarpWithTrueDimension = (this)->CobraWarpWithTrueDimension;


	if(CobraWarpWithTrueDimension>0){
		LogSystem()->ReportMessage("CobraWarpWithTrueDimension (0 is off 1 is Warp with TrueDimension On, 2 is Warp with TrueDimension off ) = %d",CobraWarpWithTrueDimension);
	}
	if (fxShaderFilename[0] != '\0') {
		LogSystem()->ReportMessage("loading shader %s",fxShaderFilename);

			std::string vertexShaderFilename = luaSupportDir;
			vertexShaderFilename.append(fxShaderFilename);
			((OmniMapShaderD3D10 *) shaders)->setShadersFilenames(vertexShaderFilename.c_str(), NULL);
	}
	((OmniMapShaderD3D10 *) shaders)->setNumChannels(GetNumChannels());
	if(StencilMask_filename)
		((OmniMapShaderD3D10 *)shaders)->StencilMask_Load(StencilMask_filename); // the final mask needs to be done before the shader is compiled....

	shaders->init();

	for (int i = 0; i < GetNumChannels(); i++)
	{
		((OmniMapChannelD3D10 *) this->GetChannel(i))->setOMShader((OmniMapShaderD3D10 *) shaders);
	}

	for(int i = 0; i < GetNumScreenShapes(); i++)
	{
		ScreenGeometryBase *screenShape = this->GetScreenShape(i);
		ScreenRendererD3D10 *renderer = (ScreenRendererD3D10 *) screenShape->GetRenderer();
		renderer->setOMShader((OmniMapShaderD3D10 *)shaders);
	}

	CreateLogoSurface();

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


OmniMapChannelBase *OmniMapD3D10::Create_Channel(const char * name) 
{
	OmniMapChannelD3D10 *channel = new OmniMapChannelD3D10(d3d10Device);
	
	return(channel);
}

void OmniMapD3D10::Clear()
{

	float clearColor[4] = { clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a };
	ID3D10RenderTargetView* apOldRTVs[1] = { NULL };
    ID3D10DepthStencilView* pOldDS = NULL;
    d3d10Device->OMGetRenderTargets( 1, apOldRTVs, &pOldDS );
    d3d10Device->ClearRenderTargetView( apOldRTVs[0], clearColor );
	SAFE_RELEASE(apOldRTVs[0]);
	SAFE_RELEASE(pOldDS);

}

void OmniMapD3D10::SetupHudMatrix()
{


	D3DXMATRIX projMat;
	D3DXMatrixOrthoLH(
		&projMat, 2.0f, 2.0f, -1.0f, 1.0f);
 
	((OmniMapShaderD3D10 *) shaders)->setHudProjMatrix((float *) &projMat);
}


void OmniMapD3D10::DrawHeadsUpDisplay()
{
	
	//d3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	DrawHeadsUpDisplays();
	//d3dDevice->SetRenderState(D3DRS_ZENABLE, true);

}

void OmniMapD3D10::SetProjectiveTexturingMatricies()
{

	
	D3DXMatrixOrthoRH(
		&worldViewProjection, 2.0, 2.0, 0.0, 1.0f);
	
    D3DXMATRIX matView;
    D3DXMatrixLookAtRH( &worldView,
		&D3DXVECTOR3( ProjectorPosition.pos.x, ProjectorPosition.pos.y, ProjectorPosition.pos.z ), // Camera position
        &D3DXVECTOR3( ProjectorPosition.lookAtpos.x, ProjectorPosition.lookAtpos.y, ProjectorPosition.lookAtpos.z ),   // Look-at point
        &D3DXVECTOR3( ProjectorPosition.headsUp.x, ProjectorPosition.headsUp.y, ProjectorPosition.headsUp.z ) ); // Up vector

}

void OmniMapD3D10::SetupAllShaderVaribles()
{

	int N_Channels;
	int TextureIds[OMNIMAP_maxChannelFaces];

	D3DXMATRIX textureMats[6];
	GetShaderVariables(N_Channels, TextureIds);
	((OmniMapShaderD3D10 *) shaders)->SetTextureIds(N_Channels);

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
		((OmniMapChannelD3D10 *) channel)->GetProjectionMatrixRH(&channelProj);

		D3DXMatrixMultiply(pTextureMat, &channelProj, pTextureMat);
		D3DXMATRIX viewMatrix;
		((OmniMapChannelD3D10 *) channel)->GetViewMatrix2(&viewMatrix);
		D3DXMatrixMultiply(pTextureMat, &viewMatrix, pTextureMat);
	}
	((OmniMapShaderD3D10 *) shaders)->SetTextureMatrices(textureMats, N_Channels);

	 
	D3D10_VIEWPORT vp;
	UINT one = 1;
	d3d10Device->RSGetViewports(&one, &vp);
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
		vp.TopLeftX = (int) glViewportsettings0;
		vp.TopLeftY = (int) glViewportsettings1;
		vp.Width = (int) glViewportsettings2;
		vp.Height = (int) glViewportsettings3;
		d3d10Device->RSSetViewports(1, &vp);
		yOffset1 = 0.0;
		yOffset2 = 0.0;
		yScale = 1.0;
	} else {
		yOffset2 = ((glViewportsettings3/2.0)/((float) vp.Height-(float) glViewportsettings1))/2.0;
		yScale = (((float) glViewportsettings2) / ((float) vp.Height));
		yOffset1 = (((float) glViewportsettings1) / ((float) glViewportsettings3));
	}



	
	if (yOffset1 != 0.0) yOffset2 = 0.0;

	((OmniMapShaderD3D10 *) shaders)->SetYOffsets(yScale, yOffset1, yOffset2);
	((OmniMapShaderD3D10 *) shaders)->SetViewProjMats((float *) &worldViewProjection, (float *) &worldView);


	if(StencilMask_filename)
		((OmniMapShaderD3D10 *) shaders)->StencilMask_Bind(6);//+1


}


void OmniMapD3D10::PostRender()
{
	HRESULT hr;


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
	D3D10_STATE_BLOCK_MASK stateBlockMask;
	D3D10StateBlockMaskEnableAll(&stateBlockMask);
	hr =  D3D10CreateStateBlock(d3d10Device, &stateBlockMask,
		&pStateBlock);

	pStateBlock->Capture();

	if(GL_STATE_CLEAR_AT_STARTFRAME)
		Clear();

	// hr = d3d10Device->BeginScene();
	// Still need to make sure we are rendering to final buffer here.

	// Set us up for multisampling
   
    d3d10Device->RSSetState(pRasterState);

	shaders->EnableShader();

	// B.2  set the textures
	BindAllChannelTextures();

	SetProjectiveTexturingMatricies();

	SetupAllShaderVaribles();

	DisplayScreenShapes();

	shaders->DisableShader();

	if (this->displayHUD)
	{
		// Render HUD
		SetupHudMatrix();
		DrawHeadsUpDisplay();
	}
	if (om_protector.isProtected())
		DrawBG();


	pStateBlock->Apply();
	SAFE_RELEASE(pStateBlock);

}




struct TexVertex10
{
	D3DXVECTOR3 pos;
    D3DXVECTOR2 tex;
};

void OmniMapD3D10::CreateTextureFromFile(ID3D10Device *d3dDevice, char *filename, ID3D10ShaderResourceView **shaderResView, 
								 ID3D10Texture2D **texture)
{
	HRESULT hr;
	ID3D10Resource *pRes = NULL;
	size_t origsize = strlen(filename) + 1;
	const size_t newsize = 1000;
	size_t convertedChars = 0;
	wchar_t wcstring[2048];
#ifdef USE_VC7
	convertedChars = mbstowcs(wcstring, StencilMask_FileName, origsize);
#else
	mbstowcs_s(&convertedChars, wcstring, origsize, filename, _TRUNCATE);
#endif
	


	
	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof( D3DX10_IMAGE_LOAD_INFO ) );
	loadInfo.Width = D3DX_FROM_FILE;
	loadInfo.Height  = D3DX_FROM_FILE;
	loadInfo.Depth  = D3DX_FROM_FILE;
	loadInfo.FirstMipLevel = 0;
	loadInfo.MipLevels = 1;
	loadInfo.Usage = D3D10_USAGE_STAGING;
	loadInfo.BindFlags = 0;
	loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
	loadInfo.MiscFlags = 0;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	loadInfo.Filter = D3DX10_FILTER_NONE;
	loadInfo.MipFilter = D3DX10_FILTER_NONE;

	D3D10_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(D3D10_TEXTURE2D_DESC) );
	hr = D3DX10CreateTextureFromFile( d3dDevice, wcstring, &loadInfo, NULL, &pRes, NULL );

    if( pRes )
    {
        ID3D10Texture2D* pTemp;
		
        pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&pTemp );
		
        pTemp->GetDesc( &desc );

		D3D10_MAPPED_TEXTURE2D mappedTex2D;
        if(DXGI_FORMAT_R8G8B8A8_UNORM != desc.Format)   //make sure we're R8G8B8A8
            return;

        //if(desc.MipLevels > 4)
            //desc.MipLevels -= 4;

        desc.Usage = D3D10_USAGE_DEFAULT;
        desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.ArraySize = 1;


       hr = d3d10Device->CreateTexture2D( &desc, NULL, texture);
	
		 for(UINT iMip=0; iMip < desc.MipLevels; iMip++)
            {
				//pTemp->Unmap( iMip );
                hr = pTemp->Map( iMip, D3D10_MAP_READ, 0, &mappedTex2D );
                    
				d3d10Device->UpdateSubresource( *texture, 
                                                D3D10CalcSubresource( iMip, 0, desc.MipLevels ),
                                                NULL,
                                                mappedTex2D.pData,
                                                mappedTex2D.RowPitch,
                                                0 );

                pTemp->Unmap( iMip );
            }


		SAFE_RELEASE( pTemp );

        SAFE_RELEASE( pRes );
      
	}
		// Create the resource view
    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = desc.MipLevels;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    hr = (d3d10Device->CreateShaderResourceView( *texture, &SRVDesc, shaderResView ));

}

void OmniMapD3D10::CreateTextureFromMemory(ID3D10Device *d3dDevice, unsigned char *textureData, int size, ID3D10ShaderResourceView **shaderResView, 
								 ID3D10Texture2D **texture)
{
	HRESULT hr;
	ID3D10Resource *pRes = NULL;

	D3D10_TEXTURE2D_DESC dstex;
	ZeroMemory( &dstex, sizeof( D3D10_TEXTURE2D_DESC ) );
	dstex.Width = 128;
	dstex.Height = 128;
	dstex.MipLevels = 1;
	dstex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dstex.Usage = D3D10_USAGE_DEFAULT;
	dstex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	dstex.CPUAccessFlags = 0;
	dstex.MiscFlags = 0;
	dstex.ArraySize = 1;
	D3DX10_IMAGE_INFO imginfo;
	hr = D3DX10GetImageInfoFromMemory(textureData, size, NULL, &imginfo, &hr);

	D3DX10_IMAGE_LOAD_INFO imgLoadInfo;
	ZeroMemory( &imgLoadInfo, sizeof(imgLoadInfo) );
	imgLoadInfo.Width = imginfo.Width;
	imgLoadInfo.Height = imginfo.Height;
	imgLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	imgLoadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE;
	imgLoadInfo.FirstMipLevel = 0;
	imgLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	imgLoadInfo.MipFilter = D3DX10_FILTER_NONE;
	imgLoadInfo.MipLevels = 1;
	imgLoadInfo.pSrcInfo = &imginfo;
	imgLoadInfo.Usage = D3D10_USAGE_DYNAMIC ;

	hr = D3DX10CreateTextureFromMemory(d3dDevice, textureData, size, &imgLoadInfo, NULL, &pRes, &hr);

	D3D10_TEXTURE2D_DESC desc;

    if( pRes )
    {
        ID3D10Texture2D* pTemp;
		
        pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&pTemp );
		
        pTemp->GetDesc( &desc );

		*texture = pTemp;
        SAFE_RELEASE( pRes );
      
	}
	
		// Create the resource view
    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = desc.MipLevels;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    hr = (d3d10Device->CreateShaderResourceView( *texture, &SRVDesc, shaderResView ));

}



void OmniMapD3D10::CreateLogoSurface()
{

	HRESULT hr;
	TexVertex10 svQuad[4];
    svQuad[0].pos = D3DXVECTOR3(-.1f, -.8f, 1.0f);
    svQuad[0].tex = D3DXVECTOR2(0.0f, 1.0f);
    svQuad[1].pos = D3DXVECTOR3(-.1f, -.6f, 1.0f);
    svQuad[1].tex = D3DXVECTOR2(0.0f, 0.0f);
    svQuad[2].pos = D3DXVECTOR3(.1f, -.8f, 1.0f);
    svQuad[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	svQuad[3].pos = D3DXVECTOR3(.1f, -.6f, 1.0f);
    svQuad[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	D3D10_BUFFER_DESC vbdesc =
    {
        4*sizeof(TexVertex10),
        D3D10_USAGE_IMMUTABLE,
        D3D10_BIND_VERTEX_BUFFER,
        0,
        0
    };

	D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = svQuad;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    hr = d3d10Device->CreateBuffer( &vbdesc, &InitData, &logoBuffer );

	for (int i = 131; i < 128+(128*128*4); i += 4)
	{
		if (logoData[i] != '\0')
			logoData[i] = '\133';
	}
	this->CreateTextureFromMemory(d3d10Device, (unsigned char *) logoData, sizeof(logoData), &logoShaderResView, &logoTexture);
	//this->CreateTextureFromFile(d3d10Device, "..\\Media\\ColorLogoAlpha.dds", &logoShaderResView, &logoTexture);
}


void OmniMapD3D10::DrawBG()
{



	D3DXMATRIX projMat;
	D3DXMatrixOrthoLH(
		&projMat, 2.0f, 2.0f, -1.0f, 1.0f);
 
	((OmniMapShaderD3D10 *) shaders)->setHudProjMatrix((float *) &projMat);
	UINT uStrides = sizeof(TexVertex10);
    UINT uOffsets = 0;
    ID3D10Buffer *pBuffers[1] = { logoBuffer };
	d3d10Device->IASetVertexBuffers( 0, 1, pBuffers, &uStrides, &uOffsets );
    d3d10Device->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
    d3d10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

    // Draw
    D3D10_TECHNIQUE_DESC techDesc;

    ID3D10EffectTechnique* pTech = ((OmniMapShaderD3D10 *) shaders)->EnableHUDShader(logoShaderResView);

    pTech->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        pTech->GetPassByIndex( p )->Apply(0);
        d3d10Device->Draw( 4, 0 );
    }

}

