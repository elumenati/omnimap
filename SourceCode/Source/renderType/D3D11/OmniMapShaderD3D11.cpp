
#include <stdio.h>
#include <d3dcompiler.h>
#include "OmniMapD3D11.h"
#include "OmniMapShaderD3D11.h"
#include "Shaders/OmniMapDome11.h"
#include "Shaders/CobraWarp11.h"
#include "OM_ErrorHandling.h"


#ifdef USE_VC7
#define MBSTOWCS(DSIZE, DSTRING, SSIZE, SSTRING) DSIZE = mbstowcs(DSTRING, SSTRING, SSIZE);
#else
#define MBSTOWCS(DSIZE, DSTRING, SSIZE, SSTRING) mbstowcs_s(&DSIZE, DSTRING, SSIZE, SSTRING, _TRUNCATE);
#endif


extern std::string preprocessorMacroGlobal;

////////////////////////////////////////////////////////
//////////////OmniMapShaderD3D11
///////////////////////////////////
ID3D11InputLayout*      OmniMapShaderD3D11::PositionTextureLayout;
ID3D11InputLayout*      OmniMapShaderD3D11::PositionLayout;
ID3D11InputLayout*      OmniMapShaderD3D11::PositionColorLayout;


static char *textureNames[] = {
  "channelTexture0",
  "channelTexture1",
  "channelTexture2",
  "channelTexture3",
  "channelTexture4",
  "channelTexture5"
};



OmniMapShaderD3D11::OmniMapShaderD3D11(ID3D11Device* _d3d11Device, ID3D11DeviceContext *_d3dDeviceContext) :
	OmniMapShaderBase()
{
	VertexShaderFilename=NULL;
	FragmentShaderFilename=NULL;
	fxShaderFilename = NULL;
	//enablePreshader = false;
  d3dDevice = _d3d11Device;
  d3dDeviceContext = _d3dDeviceContext;
	StencilMask_enable = false;
	omniMapTechnique = NULL;
	hudTechnique = NULL;

  channelTexturesSVArr = NULL;

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

  omnimapFX = NULL;

	// TODO: Need to check if shaders are supported somehow
}



OmniMapShaderD3D11::~OmniMapShaderD3D11()
{
  if (maskTexture) StencilMask_Free();
  if (fxShaderFilename) { delete [] fxShaderFilename; fxShaderFilename = NULL; }
  if (channelTexturesSVArr) { delete[] channelTexturesSVArr; channelTexturesSVArr = NULL; }
	SAFE_RELEASE(omnimapFX);
	SAFE_RELEASE(PositionTextureLayout);
	SAFE_RELEASE(PositionLayout);
	SAFE_RELEASE(PositionColorLayout);
}



void OmniMapShaderD3D11::zero()
{
}


void OmniMapShaderD3D11::deinit() {
}



void OmniMapShaderD3D11::setShadersFilenames(const char * _fxShaderFilename ,	const char * _NotUsed )
{
  _NotUsed;
  if (fxShaderFilename) { delete [] fxShaderFilename; fxShaderFilename = NULL; }
	fxShaderFilename  = (char *) OmniMapBase::CreateMemAndCopy(_fxShaderFilename);
	//LogSystem()->ReportMessage("OmniMapShaderD3D new fx file: %s",fxShaderFilename );
}



void OmniMapShaderD3D11::init()
{
  char *effectStr = NULL;
  char numChanStr[3];
  ID3DBlob *errorBlob = NULL;
  ID3DBlob *effectBlob = NULL;
  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  sprintf(numChanStr, "%d", numChannels);
  D3DX11_PASS_DESC PassDesc = {0};
  D3D10_SHADER_MACRO mac[3] =
  {
    { "NumChannels",        numChanStr},
    { "stencilMaskEnabled", StencilMask_enable ? "true" : "false" },
    { NULL,                 NULL }
  };
  // Create our quad vertex input layout
  const D3D11_INPUT_ELEMENT_DESC posTextLayout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  const D3D11_INPUT_ELEMENT_DESC posLayout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  EH_DECLARE;




  EH_Log("OmniMap_OmniMapShader loading...");

  // Read the D3DX effect file
  
  if (fxShaderFilename != NULL)
  {
    // Convert to a wchar_t*
    size_t origsize = strlen(fxShaderFilename) + 1;
    size_t convertedChars = 0;
    wchar_t wcstring[1024];
    MBSTOWCS(convertedChars, wcstring, origsize, fxShaderFilename);

    EH_Test(D3DX11CompileFromFile(wcstring, mac, NULL, NULL, "fx_5_0", dwShaderFlags|D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY, 0, NULL, &effectBlob, &errorBlob, NULL));
    EH_Test(D3DX11CreateEffectFromMemory(effectBlob->GetBufferPointer(), effectBlob->GetBufferSize(), 0, d3dDevice, &omnimapFX));
  }
  else
  {
    std::string effectTotal;
    int *effectPtr = effectArr;
    int effectPtrSize = sizeof(effectArr)/sizeof(int);

    // warning this code has not been tested
    EH_Log("Warning untested code in OmniMapShaderD3D11.cpp");

    if (CobraWarpWithTrueDimension > 0) {
      EH_Log("Elumenati Warp for CobraSimulation enabled.");
      effectPtr = effectArrCobra;
      effectPtrSize = sizeof(effectArrCobra)/sizeof(int);
    }

    EH_Ptr(effectStr = new char[effectPtrSize+1]);
    for (int i = 0; i < effectPtrSize; i++) {
      effectStr[i] = (char) effectPtr[i];
      effectStr[i+1] = 0;
    }

    effectTotal = preprocessorMacroGlobal + std::string(effectStr);

    EH_Test(D3DX11CompileFromMemory(effectTotal.c_str(), effectTotal.length(), NULL, mac, NULL, NULL, "fx_5_0", /*D3DCOMPILE_DEBUG|*/D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY, 0, NULL, &effectBlob, &errorBlob, NULL));
    EH_Test(D3DX11CreateEffectFromMemory(effectBlob->GetBufferPointer(), effectBlob->GetBufferSize(), 0, d3dDevice, &omnimapFX));
  }

 
  EH_Ptr(channelTexturesSVArr = new ID3DX11EffectShaderResourceVariable *[numChannels]);

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

  if (1 <= CobraWarpWithTrueDimension ) {
    ID3DX11EffectScalarVariable *sCobraWarpWithTrueDimension = omnimapFX->GetVariableByName( "CobraWarpWithTrueDimension" )->AsScalar();
    sCobraWarpWithTrueDimension->SetFloat((float)CobraWarpWithTrueDimension);
  }

  EH_Test(hudTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc ));
  EH_Test(d3dDevice->CreateInputLayout( posTextLayout, sizeof(posTextLayout)/sizeof(posTextLayout[0]), 
    PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &PositionTextureLayout ) );

  EH_Test(omniMapTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc ));
  EH_Test(d3dDevice->CreateInputLayout( posLayout, sizeof(posLayout)/sizeof(posLayout[0]), 
    PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &PositionLayout ) );




  EH_OnError() {
    EH_Log("Cannot create effect from shader effect file or string");

    if (errorBlob)
    {
      const char *ep = (const char *) errorBlob->GetBufferPointer();
      if (ep) EH_Log("Error:%s\n", ep);
    }

    if (channelTexturesSVArr) { delete[] channelTexturesSVArr; channelTexturesSVArr = NULL; }
  }



  if (effectStr) delete[] effectStr;
  SAFE_RELEASE(effectBlob);
  SAFE_RELEASE(errorBlob);
}





ID3DX11EffectTechnique *OmniMapShaderD3D11::getOMTechnique()
{
	return omniMapTechnique;
}




void OmniMapShaderD3D11::SetD3D11DeviceContext(ID3D11DeviceContext *_d3dDeviceContext) {
  d3dDeviceContext = _d3dDeviceContext;
}





void OmniMapShaderD3D11::EnableShader()
{
  HRESULT hr;
  // Apply the technique contained in the effect 	
  hr = omniMapTechnique->GetPassByIndex( 0 )->Apply(0, d3dDeviceContext);
}




void OmniMapShaderD3D11::DisableShader()
{
	
}



void OmniMapShaderD3D11::setHudProjMatrix(float *matrix)
{
	HRESULT hr;
	hr = hudProjMatrixSV->SetMatrix(matrix);
}



ID3DX11EffectTechnique *OmniMapShaderD3D11::EnableHUDShader(ID3D11ShaderResourceView* renderTargetSRV)
{
  HRESULT hr;
  hr = hudTextureSV->SetResource( renderTargetSRV );
  d3dDeviceContext->IASetInputLayout( PositionTextureLayout );
  return(hudTechnique);

}



void OmniMapShaderD3D11::setNumChannels(int _numChannels)
{
	numChannels = _numChannels;
}




void OmniMapShaderD3D11::SetViewProjMats(float *projMat, float *viewMat)
{
	projMatrixSV->SetMatrix(projMat);
	viewMatrixSV->SetMatrix(viewMat);
}




void OmniMapShaderD3D11::setTexture(int index, ID3D11ShaderResourceView* texture)
{
	textureTable[index] = texture;
}




void OmniMapShaderD3D11::SetTextureIds(int numChannels) 
{
  EH_DECLARE;

  for (int i = 0; i < numChannels; i++)
	{
		EH_Test(channelTexturesSVArr[i]->SetResource(textureTable[i]));
	}

  EH_OnError() {
    EH_Log("Cannot set texture in shader\n");
  }
}



void OmniMapShaderD3D11::SetYOffsets(float yScale, float yOffset1, float yOffset2)
{
	yScaleSV->SetFloat(yScale);
	yOffset1SV->SetFloat(yOffset1);
	yOffset2SV->SetFloat(yOffset2);
}




void OmniMapShaderD3D11::SetTextureMatrices(D3DXMATRIX *m, int count)
{
	HRESULT hr;
	hr = projTextureMatricesSV->SetMatrixArray((float *) m, 0, count);
}





void OmniMapShaderD3D11::StencilMask_Load(char *StencilMask_FileName)	
{
  size_t origsize = strlen(StencilMask_FileName) + 1;
  size_t convertedChars = 0;
  wchar_t wcstring[2048];
  ID3D11Resource *pRes = NULL;
  ID3D11Texture2D* pTemp = NULL;
  D3DX11_IMAGE_LOAD_INFO loadInfo;
  D3D11_TEXTURE2D_DESC desc = {0};
  D3D11_MAPPED_SUBRESOURCE mappedTex2D = {0};
  D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
  EH_DECLARE;

  if (maskTexture != NULL) StencilMask_Free();

  EH_Log("Loading stencil mask.\n");

  MBSTOWCS(convertedChars, wcstring, origsize, StencilMask_FileName);
  StencilMask_enable = true;

  ZeroMemory( &loadInfo, sizeof( D3DX11_IMAGE_LOAD_INFO ) );
  loadInfo.Width = D3DX11_FROM_FILE;
  loadInfo.Height  = D3DX11_FROM_FILE;
  loadInfo.Depth  = D3DX11_FROM_FILE;
  loadInfo.FirstMipLevel = 0;
  loadInfo.MipLevels = 1;
  loadInfo.Usage = D3D11_USAGE_STAGING;
  loadInfo.BindFlags = 0;
  loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
  loadInfo.MiscFlags = 0;
  loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  loadInfo.Filter = D3DX11_FILTER_NONE;
  loadInfo.MipFilter = D3DX11_FILTER_NONE;

  ZeroMemory( &desc, sizeof(D3D11_TEXTURE2D_DESC) );
  EH_Test(D3DX11CreateTextureFromFile( d3dDevice, wcstring, &loadInfo, NULL, &pRes, NULL ));
  EH_Test(pRes->QueryInterface( __uuidof( ID3D11Texture2D ), (LPVOID*)&pTemp ));
  pTemp->GetDesc( &desc );

  EH_Zero(DXGI_FORMAT_R8G8B8A8_UNORM == desc.Format);   //make sure we're R8G8B8A8

  if (desc.MipLevels > 4) desc.MipLevels -= 4;

  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.ArraySize = 1;

  EH_Test(d3dDevice->CreateTexture2D( &desc, NULL, &maskTexture));

  for(UINT iMip=0; iMip < desc.MipLevels; iMip++)
  {
    EH_Test(d3dDeviceContext->Map(pRes, iMip, D3D11_MAP_READ, 0, &mappedTex2D));
    d3dDeviceContext->UpdateSubresource( maskTexture, 
      D3D11CalcSubresource( iMip, 0, desc.MipLevels ),
      NULL,
      mappedTex2D.pData,
      mappedTex2D.RowPitch,
      0 );
    d3dDeviceContext->Unmap(pRes, iMip);
  }


  // Create the resource view
  ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
  SRVDesc.Format = desc.Format;
  SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SRVDesc.Texture2D.MipLevels = desc.MipLevels;
  SRVDesc.Texture2D.MostDetailedMip = 0;
  EH_Test(d3dDevice->CreateShaderResourceView( maskTexture, &SRVDesc, &maskTextureSRV ));



  EH_OnError() {
    EH_Log("Cannot load stencil mask.\n");
    StencilMask_Free();
  }


  SAFE_RELEASE( pTemp );
  SAFE_RELEASE( pRes );
}




void OmniMapShaderD3D11::StencilMask_Bind(int texture_number_not_arb)
{
  texture_number_not_arb;
	// load the
	HRESULT hr;
	hr = stencilMaskTextureSV->SetResource(maskTextureSRV);
}



 void OmniMapShaderD3D11::StencilMask_Free()
{
	// delete glid
  SAFE_RELEASE(maskTexture);
  SAFE_RELEASE(maskTextureSRV);
	StencilMask_enable = false;
}
