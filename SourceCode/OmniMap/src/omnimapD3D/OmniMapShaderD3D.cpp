// OmniMap Geometry Correction Libs
// (c)2007 Elumenati
#include "omnimapD3D.h"
#include "OmniMapShaderD3D.h"
#include "D3D/dxstdafx.h"
#include "Shaders/OmniMapDome.h"
#include "Shaders/CobraWarp.h"
#pragma warning (disable:4995)

//static char * textureNames[] = {
	//"chanText0", "chanText1", "chanText2", "chanText3", "chanText4", "chanText5"
//};

extern OM_ProtectorImpl_Cobra cobtest;
extern std::string preprocessorMacroGlobal;
////////////////////////////////////////////////////////
//////////////OmniMapShaderD3D
///////////////////////////////////
OmniMapShaderD3D::OmniMapShaderD3D(IDirect3DDevice9* _d3dDevice) :
	OmniMapShaderBase()
{
	VertexShaderFilename=NULL;
	FragmentShaderFilename=NULL;
	fxShaderFilename = NULL;
	enablePreshader = false;
	d3dDevice = _d3dDevice;
	StencilMask_enable = false;
	zero();
	// TODO: Need to check if shaders are supported somehow
	
}

OmniMapShaderD3D::~OmniMapShaderD3D()
{
	SAFE_RELEASE(omnimapFX);
	SAFE_RELEASE(maskTexture);
}


void OmniMapShaderD3D::zero()
{
	omnimapFX = NULL;
	maskTexture = NULL;
}

void OmniMapShaderD3D::init()
{
	HRESULT hr;
	
	LogSystem()->ReportMessage("OmniMap_OmniMapShader loading...");
	
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
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

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
	
	

	char numChanStr[3];
	char stencilMaskEnabledStr[6];

	sprintf(numChanStr, "%d", numChannels);
	if (StencilMask_enable)
		sprintf(stencilMaskEnabledStr, "true");
	else
		sprintf(stencilMaskEnabledStr, "false");
	D3DXMACRO mac[3] =
	{
		{ "NumChannels", numChanStr},
		{ "stencilMaskEnabled", stencilMaskEnabledStr },
		{ NULL,         NULL }
	};
	
	char *numChannelsStr = new char[4];

	LPD3DXBUFFER parseErrors = NULL;
	LPD3DXBUFFER ppErrorMsgs = NULL;
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
		
		hr = D3DXCreateEffectFromFile( d3dDevice, wcstring, mac, NULL, dwShaderFlags, NULL, &omnimapFX, &ppErrorMsgs);
	
	} else
	{

		char *effectStr = NULL;
		if(CobraWarpWithTrueDimension >0){
			if(   cobtest.IsCobraRegistered() ){
				LogSystem()->ReportError("Elumenati Warp for CobraSimulation enabled.");
			} else{
				LogSystem()->ReportError("CobraWarp features not available, please contact support@elumenati.com to register Omnimap for Cobra Features.");
				CobraWarpWithTrueDimension =0;
			}
		}

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


		std::string combinedString = preprocessorMacroGlobal + std::string(effectStr);

		hr = D3DXCreateEffect(      
			d3dDevice,
			combinedString.c_str(),
			(UINT) combinedString.length(),
			mac,
			NULL,
			0,
			NULL,
			&omnimapFX,
			&ppErrorMsgs
		);
	
	}

	if (FAILED(hr))
	{
		
		LogSystem()->ReportError("Cannot create effect from shader effect file or string");
		if (ppErrorMsgs != NULL)
		{
			const char *ep = (const char *) ppErrorMsgs->GetBufferPointer();
			if (ep)
				LogSystem()->ReportError("Error:%s\n", ep);
		}
		return;
	}
	omnimapFX->SetTechnique("RenderScene");
	if(1 <= CobraWarpWithTrueDimension ){
		omnimapFX->SetFloat("CobraWarpWithTrueDimension",(float)CobraWarpWithTrueDimension);
	}
}

void OmniMapShaderD3D::setNumChannels(int _numChannels)
{
	numChannels = _numChannels;
}

void OmniMapShaderD3D::deinit()
{
	SAFE_RELEASE(omnimapFX);
	zero();
}

void OmniMapShaderD3D::setShadersFilenames(
	const char * _fxShaderFilename ,	const char * _NotUsed )
{
	if(fxShaderFilename)
		delete [] fxShaderFilename;
	fxShaderFilename=0;

	fxShaderFilename   = (char *) OmniMapBase::CreateMemAndCopy(_fxShaderFilename);
	
	//LogSystem()->ReportMessage("OmniMapShaderD3D new fx file: %s",fxShaderFilename );
}



void OmniMapShaderD3D::setTexture(int index, LPDIRECT3DTEXTURE9 texture)
{
	textureTable[index] = texture;
}

void OmniMapShaderD3D::EnableShader()
{
		UINT cPasses;
		HRESULT hr;
	    // Apply the technique contained in the effect 
		
		//OMNIMAP_V(omnimapFX->Begin(&cPasses, 0));
		OMNIMAP_V(omnimapFX->Begin(&cPasses, 0));
        
		OMNIMAP_V( omnimapFX->BeginPass(0));


		
}

void OmniMapShaderD3D::DisableShader()
{
	HRESULT hr;
	OMNIMAP_V( omnimapFX->EndPass() );
 
    OMNIMAP_V( omnimapFX->End() );

}

static char *textureNames[] = {
	"channelTexture0",
	"channelTexture1",
	"channelTexture2",
	"channelTexture3",
	"channelTexture4",
	"channelTexture5"
};


void OmniMapShaderD3D::SetTextureIds(int numChannels)
{
	HRESULT hr;
	for (int i = 0; i < numChannels; i++)
	{
		hr = omnimapFX->SetTexture(textureNames[i], textureTable[i]);
		if (FAILED(hr))
			LogSystem()->ReportError("Cannot set texture in shader\n");
	}	
}
	


void OmniMapShaderD3D::SetUserPosition(OmniVec3 position)
{
	HRESULT hr;
	D3DXVECTOR3 userPosition(position.x, position.y, position.z);
	OMNIMAP_V( omnimapFX->SetValue( "userposition", userPosition, sizeof(D3DXVECTOR4)));
}

void OmniMapShaderD3D::SetTextureMatrices(D3DXMATRIX *m, int count)
{
	HRESULT hr;
	OMNIMAP_V( omnimapFX->SetMatrixArray( "textMatrixArray", (const D3DXMATRIX *) m, count ) );
}

void OmniMapShaderD3D::StencilMask_Free()
{
	// delete glid

	maskTexture=NULL;
	StencilMask_enable = false;

}
	
 void OmniMapShaderD3D::StencilMask_Load(char *StencilMask_FileName)	
{
		if(maskTexture != NULL)
		{
			StencilMask_Free();
		}

		StencilMask_enable = true;
		//size_t szFilename = strlen(StencilMask_FileName);
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




}
 void OmniMapShaderD3D::StencilMask_Bind(int texture_number_not_arb)
{
	// load the
	HRESULT hr;
	OMNIMAP_V( d3dDevice->SetSamplerState( 3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
    OMNIMAP_V( d3dDevice->SetSamplerState( 3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
	OMNIMAP_V( d3dDevice->SetSamplerState( 3, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) );
	OMNIMAP_V( d3dDevice->SetSamplerState( 3, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) );


	// this is needed for Cobra Cobrasim mask displacement maskTexture
	bool displacement = true;
	if(displacement){
		hr = d3dDevice->SetTexture(D3DVERTEXTEXTURESAMPLER0, maskTexture) ;
	}else{
		hr = d3dDevice->SetTexture(3, maskTexture);
	}
}



