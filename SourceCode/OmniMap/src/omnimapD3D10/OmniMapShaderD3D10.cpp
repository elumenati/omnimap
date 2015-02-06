
#include <stdio.h>
#include <d3dx9.h>
#include "OmniMapD3D10.h"
#include "OmniMapShaderD3D10.h"
#include "Shaders/OmniMapDome10.h"
#include "Shaders/CobraWarp10.h"

extern OM_ProtectorImpl_Cobra cobtest;
extern std::string preprocessorMacroGlobal;

////////////////////////////////////////////////////////
//////////////OmniMapShaderD3D
///////////////////////////////////
ID3D10InputLayout*      OmniMapShaderD3D10::PositionTextureLayout;
ID3D10InputLayout*      OmniMapShaderD3D10::PositionLayout;
ID3D10InputLayout*      OmniMapShaderD3D10::PositionColorLayout;

OmniMapShaderD3D10::OmniMapShaderD3D10(ID3D10Device* _d3d10Device) :
	OmniMapShaderBase()
{
	VertexShaderFilename=NULL;
	FragmentShaderFilename=NULL;
	fxShaderFilename = NULL;
	//enablePreshader = false;
	d3d10Device = _d3d10Device;
	StencilMask_enable = false;
	omniMapTechnique = NULL;
	hudTechnique = NULL;


	viewProjMatrixSV = NULL;
	projMatrixSV = NULL;
	viewMatrixSV = NULL;
	projTextureMatricesSV = NULL;
	channelTexturesSV = NULL;
	numChannelsSV = NULL;
	yScaleSV = NULL;
	yOffset1SV = NULL;
	yOffset2SV = NULL;

	stencilMaskEnabledSV = NULL;
	stencilMaskTextureSV = NULL;

	hudProjMatrixSV = NULL;
	hudTextureSV = NULL;

	PositionTextureLayout = NULL;
	PositionLayout = NULL;
	PositionColorLayout = NULL;

	maskTexture = NULL;
	maskTextureSRV = NULL;

	zero();
	// TODO: Need to check if shaders are supported somehow
	
}

OmniMapShaderD3D10::~OmniMapShaderD3D10()
{
	SAFE_RELEASE(omnimapFX);
	SAFE_RELEASE(maskTexture);
	SAFE_RELEASE(maskTextureSRV);
	SAFE_RELEASE(PositionTextureLayout);
	SAFE_RELEASE(PositionLayout);
	SAFE_RELEASE(PositionColorLayout);
}

void OmniMapShaderD3D10::zero()
{
	omnimapFX = NULL;
	maskTexture = NULL;
	PositionTextureLayout = NULL;
	PositionLayout = NULL;
	PositionColorLayout = NULL;

}

void OmniMapShaderD3D10::setShadersFilenames(
	const char * _fxShaderFilename ,	const char * _NotUsed )
{
	if(fxShaderFilename)
		delete [] fxShaderFilename;
	fxShaderFilename=0;

	fxShaderFilename   = (char *) OmniMapBase::CreateMemAndCopy(_fxShaderFilename);
	
	//LogSystem()->ReportMessage("OmniMapShaderD3D new fx file: %s",fxShaderFilename );
}

static char *textureNames[] = {
	"channelTexture0",
	"channelTexture1",
	"channelTexture2",
	"channelTexture3",
	"channelTexture4",
	"channelTexture5"
};

void OmniMapShaderD3D10::init()
{
	HRESULT hr;
	
	LogSystem()->ReportMessage("OmniMap_OmniMapShader loading...");

    // Read the D3DX effect file
	
	

	char numChanStr[3];
	char stencilMaskEnabledStr[6];

	sprintf(numChanStr, "%d", numChannels);
	if (StencilMask_enable)
		sprintf(stencilMaskEnabledStr, "true");
	else
		sprintf(stencilMaskEnabledStr, "false");
	D3D10_SHADER_MACRO mac[3] =
	{
		{ "NumChannels", numChanStr},
		{ "stencilMaskEnabled", stencilMaskEnabledStr },
		{ NULL,         NULL }
	};
	
	char *numChannelsStr = new char[4];

	ID3D10Blob *errorBlob = NULL;
	ID3D10Blob *effectBlob = NULL;
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	// Convert to a wchar_t*
	if (fxShaderFilename != NULL)
	{
		size_t origsize = strlen(fxShaderFilename) + 1;
		const size_t newsize = 1000;
		size_t convertedChars = 0;
		wchar_t wcstring[1024];
#ifdef USE_VC7
		convertedChars = mbstowcs(wcstring, fxShaderFilename, origsize);
#else
		mbstowcs_s(&convertedChars, wcstring, origsize, fxShaderFilename, _TRUNCATE);
#endif
		
		hr = D3DX10CreateEffectFromFile( wcstring, mac, NULL, "fx_2_0", dwShaderFlags, 0,
			d3d10Device, NULL, NULL, &omnimapFX, NULL, NULL );
	
	} else
	{

		// warning this code has not been tested
	LogSystem()->ReportError("Warning untested code in OmniMapShaderD3D10.cpp");


if(CobraWarpWithTrueDimension >0){
			if(   cobtest.IsCobraRegistered() ){
				LogSystem()->ReportError("Elumenati Warp for CobraSimulation enabled.");
			} else{
				LogSystem()->ReportError("CobraWarp features not available, please contact support@elumenati.com to register Omnimap for Cobra Features.");
				CobraWarpWithTrueDimension =0;
			}
		}


		char *effectStr = NULL;



		if(0 == CobraWarpWithTrueDimension ){
			effectStr = new char[(sizeof(effectArr)/sizeof(int))+1];	
			int i = 0;
			for (i = 0; i < sizeof(effectArr)/sizeof(int); i++) {
				effectStr[i] = (char) effectArr[i];
			}
			effectStr[i] = (char) 0;
		}else if(1 <= CobraWarpWithTrueDimension ){
			effectStr = new char[(sizeof(effectArrCobra)/sizeof(int))+1];	
			int i = 0;
			for (i = 0; i < sizeof(effectArrCobra)/sizeof(int); i++) {
				effectStr[i] = (char) effectArrCobra[i];
			}
			effectStr[i] = (char) 0;
		}

		std::string effectTotal = preprocessorMacroGlobal + std::string(effectStr);
		hr = D3DX10CreateEffectFromMemory( effectTotal.c_str(), effectTotal.length(), "None", 
			 mac, NULL, "fx_4_0", D3D10_SHADER_DEBUG|D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, 
			 d3d10Device, NULL, NULL, &omnimapFX, &errorBlob, NULL );
		
		if (FAILED(hr))
		{
			LogSystem()->ReportError("Cannot create effect from shader effect file or string");
			if (errorBlob != NULL)
			{
				const char *ep = (const char *) errorBlob->GetBufferPointer();
 				if (ep)
					LogSystem()->ReportError("Error:%s\n", ep);
			}
			return;
		}
	}

	// Obtain technique objects
    omniMapTechnique = omnimapFX->GetTechniqueByName( "RenderScene" );
	hudTechnique = omnimapFX->GetTechniqueByName( "RenderHUDWithTexture" );

	// Get the variables
	viewProjMatrixSV = omnimapFX->GetVariableByName( "projViewMatrix" )->AsMatrix();
	projMatrixSV = omnimapFX->GetVariableByName( "g_mProjection" )->AsMatrix();
	viewMatrixSV = omnimapFX->GetVariableByName( "g_mWorldView" )->AsMatrix();
	projTextureMatricesSV = omnimapFX->GetVariableByName( "textMatrixArray" )->AsMatrix();
	channelTexturesSV = omnimapFX->GetVariableByName( "channelTextures" )->AsShaderResource();
	numChannelsSV = omnimapFX->GetVariableByName( "numChannels" )->AsScalar();
	yScaleSV = omnimapFX->GetVariableByName( "yScale" )->AsScalar();
	yOffset1SV = omnimapFX->GetVariableByName( "yOffset1" )->AsScalar();
	yOffset2SV = omnimapFX->GetVariableByName( "yOffset2" )->AsScalar();
	stencilMaskEnabledSV = omnimapFX->GetVariableByName( "stencilMaskEnabled" )->AsScalar();
	channelTexturesSVArr = new ID3D10EffectShaderResourceVariable *[numChannels];
	for (int i = 0; i < numChannels; i++)
	{
		channelTexturesSVArr[i] = omnimapFX->GetVariableByName( textureNames[i] )->AsShaderResource();
	}

	if (StencilMask_enable)
	{
		stencilMaskTextureSV = omnimapFX->GetVariableByName( "alphaMaskText" )->AsShaderResource();
	}

	hudProjMatrixSV = omnimapFX->GetVariableByName( "g_mProjection" )->AsMatrix();
	hudTextureSV = omnimapFX->GetVariableByName( "hudTexture" )->AsShaderResource();
	
	
	// Create our quad vertex input layout
    const D3D10_INPUT_ELEMENT_DESC posTextLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };

	const D3D10_INPUT_ELEMENT_DESC posColLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };

	const D3D10_INPUT_ELEMENT_DESC posLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };


    D3D10_PASS_DESC PassDesc;
    hudTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    hr = ( d3d10Device->CreateInputLayout( posTextLayout, sizeof(posTextLayout)/sizeof(posTextLayout[0]), 
		PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &PositionTextureLayout ) );

	omniMapTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    hr = ( d3d10Device->CreateInputLayout( posLayout, sizeof(posLayout)/sizeof(posLayout[0]), 
		PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &PositionLayout ) );

	if(1 <= CobraWarpWithTrueDimension ){

		ID3D10EffectScalarVariable  *sCobraWarpWithTrueDimension= omnimapFX->GetVariableByName( "CobraWarpWithTrueDimension" )->AsScalar();

	sCobraWarpWithTrueDimension->SetFloat(CobraWarpWithTrueDimension);
	}
}

ID3D10EffectTechnique *OmniMapShaderD3D10::getOMTechnique()
{
	return omniMapTechnique;
}

void OmniMapShaderD3D10::EnableShader()
{
		HRESULT hr;
	    // Apply the technique contained in the effect 	
		hr = omniMapTechnique->GetPassByIndex( 0 )->Apply(0);
		
}

void OmniMapShaderD3D10::DisableShader()
{
	
}

void OmniMapShaderD3D10::setHudProjMatrix(float *matrix)
{
	HRESULT hr;
	hr = hudProjMatrixSV->SetMatrix(matrix);
}

ID3D10EffectTechnique *OmniMapShaderD3D10::EnableHUDShader(ID3D10ShaderResourceView* renderTargetSRV)
{

	HRESULT hr;
	   
    hr = hudTextureSV->SetResource( renderTargetSRV );
	d3d10Device->IASetInputLayout( PositionTextureLayout );
	return(hudTechnique);
    
}

void OmniMapShaderD3D10::setNumChannels(int _numChannels)
{
	numChannels = _numChannels;
}

void OmniMapShaderD3D10::SetViewProjMats(float *projMat, float *viewMat)
{
	projMatrixSV->SetMatrix(projMat);
	viewMatrixSV->SetMatrix(viewMat);
}

void OmniMapShaderD3D10::setTexture(int index, ID3D10ShaderResourceView* texture)
{
	textureTable[index] = texture;
}

void OmniMapShaderD3D10::SetTextureIds(int numChannels) {

	HRESULT hr;
	for (int i = 0; i < numChannels; i++)
	{
		hr = channelTexturesSVArr[i]->SetResource(textureTable[i]);
		if (FAILED(hr))
			LogSystem()->ReportError("Cannot set texture in shader\n");
	}

}

void OmniMapShaderD3D10::SetYOffsets(float yScale, float yOffset1, float yOffset2)
{
	yScaleSV->SetFloat(yScale);
	yOffset1SV->SetFloat(yOffset1);
	yOffset2SV->SetFloat(yOffset2);
}

void OmniMapShaderD3D10::SetTextureMatrices(D3DXMATRIX *m, int count)
{
	HRESULT hr;
	hr = projTextureMatricesSV->SetMatrixArray((float *) m, 0, count);
}

 void OmniMapShaderD3D10::StencilMask_Load(char *StencilMask_FileName)	
{
	HRESULT hr;
	if(maskTexture != NULL)
	{
		StencilMask_Free();
	}

	StencilMask_enable = true;
	
	size_t origsize = strlen(StencilMask_FileName) + 1;
	const size_t newsize = 1000;
	size_t convertedChars = 0;
	wchar_t wcstring[2048];
#ifdef USE_VC7
	convertedChars = mbstowcs(wcstring, StencilMask_FileName, origsize);
#else
	mbstowcs_s(&convertedChars, wcstring, origsize, StencilMask_FileName, _TRUNCATE);
#endif
	


    ID3D10Resource *pRes = NULL;
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
    hr = D3DX10CreateTextureFromFile( d3d10Device, wcstring, &loadInfo, NULL, &pRes, NULL );
    if( pRes )
    {
        ID3D10Texture2D* pTemp;
        pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&pTemp );
		
        pTemp->GetDesc( &desc );

		D3D10_MAPPED_TEXTURE2D mappedTex2D;
        if(DXGI_FORMAT_R8G8B8A8_UNORM != desc.Format)   //make sure we're R8G8B8A8
            return;

        if(desc.MipLevels > 4)
            desc.MipLevels -= 4;

        desc.Usage = D3D10_USAGE_DEFAULT;
        desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.ArraySize = 1;

        hr = d3d10Device->CreateTexture2D( &desc, NULL, &maskTexture);

		 for(UINT iMip=0; iMip < desc.MipLevels; iMip++)
            {
                pTemp->Map( iMip, D3D10_MAP_READ, 0, &mappedTex2D );
                    
                d3d10Device->UpdateSubresource( maskTexture, 
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
    hr = (d3d10Device->CreateShaderResourceView( maskTexture, &SRVDesc, &maskTextureSRV ));

}

void OmniMapShaderD3D10::StencilMask_Bind(int texture_number_not_arb)
{
	// load the
	HRESULT hr;
	hr = stencilMaskTextureSV->SetResource(maskTextureSRV);
}

 void OmniMapShaderD3D10::StencilMask_Free()
{
	// delete glid
	SAFE_RELEASE(maskTexture);
	maskTexture=NULL;
	StencilMask_enable = false;

}