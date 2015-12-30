// OmniMap Geometry Correction Libs
// (c)2007 Elumenati
#include "omnimapD3D.h"
#include "OmniMapShaderD3D.h"
#include "D3D/dxstdafx.h"
#include "Shaders/OmniMapDome.h"
#include "Shaders/CobraWarp.h"
#include "OM_ErrorHandling.h"
#pragma warning (disable:4995)


#ifdef USE_VC7
#define MBSTOWCS(DSIZE, DSTRING, SSIZE, SSTRING) DSIZE = mbstowcs(DSTRING, SSTRING, SSIZE);
#else
#define MBSTOWCS(DSIZE, DSTRING, SSIZE, SSTRING) mbstowcs_s(&DSIZE, DSTRING, SSIZE, SSTRING, _TRUNCATE);
#endif



extern std::string preprocessorMacroGlobal;


////////////////////////////////////////////////////////
//////////////OmniMapShaderD3D
///////////////////////////////////


static char *textureNames[] = {
  "channelTexture0",
  "channelTexture1",
  "channelTexture2",
  "channelTexture3",
  "channelTexture4",
  "channelTexture5"
};



OmniMapShaderD3D::OmniMapShaderD3D(IDirect3DDevice9* _d3dDevice) :
	OmniMapShaderBase()
{
	VertexShaderFilename=NULL;
	FragmentShaderFilename=NULL;
	fxShaderFilename = NULL;
	enablePreshader = false;
	d3dDevice = _d3dDevice;
	StencilMask_enable = false;

  omnimapFX = NULL;
  maskTexture = NULL;
	// TODO: Need to check if shaders are supported somehow
}




OmniMapShaderD3D::~OmniMapShaderD3D()
{
  if (maskTexture) StencilMask_Free();
  if (fxShaderFilename) { delete [] fxShaderFilename; fxShaderFilename = NULL; }
  SAFE_RELEASE(omnimapFX);
}




void OmniMapShaderD3D::zero()
{
}



void OmniMapShaderD3D::deinit()
{
}




void OmniMapShaderD3D::setShadersFilenames(
  const char * _fxShaderFilename ,	const char * _NotUsed )
{
  _NotUsed;
  if (fxShaderFilename) { delete [] fxShaderFilename; fxShaderFilename = NULL; }
  fxShaderFilename  = (char *) OmniMapBase::CreateMemAndCopy(_fxShaderFilename);
  //LogSystem()->ReportMessage("OmniMapShaderD3D new fx file: %s",fxShaderFilename );
}



void OmniMapShaderD3D::init()
{
  char numChanStr[3];
  LPD3DXBUFFER ppErrorMsgs = NULL;
  DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
  sprintf(numChanStr, "%d", numChannels);
  D3DXMACRO mac[3] =
  {
    { "NumChannels",        numChanStr},
    { "stencilMaskEnabled", StencilMask_enable ? "true" : "false" },
    { NULL,                 NULL }
  };
  EH_DECLARE;



  EH_Log("OmniMap_OmniMapShader loading...");

  // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
  // shader debugger. Debugging vertex shaders requires either REF or software vertex 
  // processing, and debugging pixel shaders requires REF.  The 
  // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
  // shader debugger.  It enables source level debugging, prevents instruction 
  // reordering, prevents dead code elimination, and forces the compiler to compile 
  // against the next higher available software target, which ensures that the 
  // unoptimized shaders do not exceed the shader model limitations.  Setting these 
  // flags will cause slower rendering since the shaders will be unoptimized and 
  // forced into software.  See the DirectX documentation for more information about 
  // using the shader debugger.

#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
  dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
  dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

  // Preshaders are parts of the shader that the effect system pulls out of the 
  // shader and runs on the host CPU. They should be used if you are GPU limited. 
  // The D3DXSHADER_NO_PRESHADER flag disables preshaders.
  if( !enablePreshader )
    dwShaderFlags |= D3DXSHADER_NO_PRESHADER;




  // Read the D3DX effect file

  if (fxShaderFilename != NULL)
  {
    // Convert to a wchar_t*
    size_t origsize = strlen(fxShaderFilename) + 1;
    size_t convertedChars = 0;
    wchar_t wcstring[1024];
    MBSTOWCS(convertedChars, wcstring, origsize, fxShaderFilename);

    EH_Test(D3DXCreateEffectFromFile( d3dDevice, wcstring, mac, NULL, dwShaderFlags, NULL, &omnimapFX, &ppErrorMsgs));
  }
  else
  {
    std::string effectTotal = preprocessorMacroGlobal;

    if (CobraWarpWithTrueDimension > 0) {
      EH_Log("Elumenati Warp for CobraSimulation enabled.");
      effectTotal += effectCobraWarp;
    } else {
      effectTotal += effectOmniMapDome;
    }

    EH_Test(D3DXCreateEffect(d3dDevice, effectTotal.c_str(), (UINT) effectTotal.length(), mac, NULL, 0, NULL, &omnimapFX, &ppErrorMsgs));
  }


  EH_Test(omnimapFX->SetTechnique("RenderScene"));
  if(1 <= CobraWarpWithTrueDimension ){
    omnimapFX->SetFloat("CobraWarpWithTrueDimension", (float)CobraWarpWithTrueDimension);
  }


  EH_OnError() {
    EH_Log("Cannot create effect from shader effect file or string");

    if (ppErrorMsgs)
    {
      const char *ep = (const char *) ppErrorMsgs->GetBufferPointer();
      if (ep) EH_Log("Error:%s\n", ep);
    }
  }



  SAFE_RELEASE(ppErrorMsgs);
}




void OmniMapShaderD3D::setNumChannels(int _numChannels)
{
	numChannels = _numChannels;
}




void OmniMapShaderD3D::setTexture(int index, LPDIRECT3DTEXTURE9 texture)
{
	textureTable[index] = texture;
}




void OmniMapShaderD3D::EnableShader()
{
  UINT cPasses;
  EH_DECLARE;

  // Apply the technique contained in the effect 
  EH_Test(omnimapFX->Begin(&cPasses, 0));
  EH_Test(omnimapFX->BeginPass(0));
  EH_OnError() {}
}




void OmniMapShaderD3D::DisableShader()
{
  EH_DECLARE;
	EH_Test( omnimapFX->EndPass() );
  EH_Test( omnimapFX->End() );
  EH_OnError() {}
}



void OmniMapShaderD3D::SetTextureIds(int numChannels)
{
  EH_DECLARE;

  for (int i = 0; i < numChannels; i++)
  {
    EH_Test(omnimapFX->SetTexture(textureNames[i], textureTable[i]));
  }

  EH_OnError() {
    EH_Log("Cannot set texture in shader\n");
  }
}
	


void OmniMapShaderD3D::SetUserPosition(OmniVec3 position)
{
	D3DXVECTOR3 userPosition(position.x, position.y, position.z);
  EH_DECLARE;
  EH_Test( omnimapFX->SetValue( "userposition", userPosition, sizeof(D3DXVECTOR4)));
  EH_OnError() {}
}



void OmniMapShaderD3D::SetTextureMatrices(D3DXMATRIX *m, int count)
{
  EH_DECLARE;
	EH_Test( omnimapFX->SetMatrixArray( "textMatrixArray", (const D3DXMATRIX *) m, count ) );
  EH_OnError() {}
}



	


void OmniMapShaderD3D::StencilMask_Load(char *StencilMask_FileName)	
{
  size_t origsize = strlen(StencilMask_FileName) + 1;
  size_t convertedChars = 0;
  wchar_t wcstring[2048];
  EH_DECLARE;

  if (maskTexture != NULL) StencilMask_Free();

  EH_Log("Loading stencil mask (%s).\n", StencilMask_FileName);

  MBSTOWCS(convertedChars, wcstring, origsize, StencilMask_FileName);
  StencilMask_enable = true;

  EH_Test(D3DXCreateTextureFromFile(d3dDevice, wcstring, &maskTexture)); 

  EH_OnError() {
    EH_Log("Cannot load stencil mask.\n");
    StencilMask_Free();
  }

#if 0
  if(maskTexture != NULL)
		{
			StencilMask_Free();
		}

		StencilMask_enable = true;
    // Convert to a wchar_t*
    size_t origsize = strlen(StencilMask_FileName) + 1;
		const size_t newsize = 1000;
		size_t convertedChars = 0;
		wchar_t wcstring[2048];
#ifdef USE_VC7
		convertedChars = mbstowcs(wcstring, StencilMask_FileName, origsize);
#else
		mbstowcs_s(&convertedChars, wcstring, origsize, StencilMask_FileName, _TRUNCATE);
#endif
		/*
		HRESULT hr = D3DXCreateTextureFromFileEx(d3dDevice, wcstring, D3DX_FROM_FILE, D3DX_FROM_FILE, 1, 0,
                                     D3DFMT_R8G8B8, D3DPOOL_SCRATCH,
                                     D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &maskTexture);
									 */
		HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, wcstring,&maskTexture); 
#endif
}




void OmniMapShaderD3D::StencilMask_Bind(int texture_number_not_arb)
{
  // load the
  EH_DECLARE;
  /*
  //bool displacement = true;
  EH_Test( d3dDevice->SetSamplerState( 3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
  EH_Test( d3dDevice->SetSamplerState( 3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
  EH_Test( d3dDevice->SetSamplerState( 3, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) );
  EH_Test( d3dDevice->SetSamplerState( 3, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) );

  // this is needed for Cobra Cobrasim mask displacement maskTexture
  if(displacement){
    EH_Test(d3dDevice->SetTexture(D3DVERTEXTEXTURESAMPLER0, maskTexture)) ;
  }else{
    EH_Test(d3dDevice->SetTexture(3, maskTexture));
  }
*/
  EH_Test(omnimapFX->SetTexture("alphaMaskText", maskTexture));

  EH_OnError() {}
}



void OmniMapShaderD3D::StencilMask_Free()
{
  // delete glid
  SAFE_RELEASE(maskTexture);
  StencilMask_enable = false;
}


