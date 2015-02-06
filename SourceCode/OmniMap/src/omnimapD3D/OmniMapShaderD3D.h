#ifndef __OMNIMAP_SHADER_D3D__
#define __OMNIMAP_SHADER_D3D__
/**
 * \brief The DirectX9 implementation of OmniMapShaderBase.
 *
 * This is the DirectX9 implementation of the abstract class OmniMapShaderBase.  This class is responsible for
 * loading/unloading the OmniMap 3DXEffect file that spherically the OmniMapAPID3D channels.
 */
class OMNIMAP_API   OmniMapShaderD3D : public OmniMapShaderBase
{
	friend class OmniMapD3D;

private:

	//D3D Shaders
	ID3DXEffect *omnimapFX;
	LPD3DXEFFECTCOMPILER effectCompiler;
	IDirect3DDevice9* d3dDevice;

	// Fragment Parameters
	D3DXVECTOR4  *planeEqXYZ;
	D3DXVECTOR4  *planeEqU;
	D3DXVECTOR4  *planeEqV;
	//GLUniformVar*  channelTextures;
	D3DXVECTOR3   *userposition;
	//D3DXBOO*  _hlsl_StencilMask;
	//GLUniformVar*  _hlsl_StencilMaskEnabled;

	LPDIRECT3DTEXTURE9 textureTable[OMNIMAP_maxChannelFaces];
	bool enablePreshader;
	IDirect3DTexture9 *maskTexture;

	// todo add a map of lua definible GLUniformVar*

public:
	/**
	 * \brief Construct an OmniMapShaderD3D object.
	 *
	 * @param _d3dDevice The Direct 3D device the application is rendering to.
	 */
	OmniMapShaderD3D(IDirect3DDevice9* _d3dDevice);

	/**
	 * \brief Destroys an OmniMapShaderD3D.  
	 * 
	 * This will release memory used by the Direct 3D effect used by the shader.
	 *
	 */
	~OmniMapShaderD3D();

	/** 
	 * \brief Enables the ID3DXEffect created in the call to OmniMapShaderD3D::init.
	 *
	 */
	void EnableShader();

	/** 
	 * \brief Disables the ID3DXEffect created in the call to OmniMapShaderD3D::init.
	 *
	 */
	void DisableShader();

	void setTexture(int index, LPDIRECT3DTEXTURE9 Texture);

	void StencilMask_Load(char *StencilMask_FileName);
	void StencilMask_Bind(int texture_number_not_arb);

	

private:

	/**
	 * \brief Initialize the Direct3D shader.
	 *
	 * This method will call D3DXCreateEffectFromFile for the file specified 
	 * by a call to OmniMapShaderD3d::setShadersFilenames.
	 */
	void init();

	
	/**
	 * \brief Deinitialize this shader.  
	 *
	 * This will release the ID3DXEffect created by init and zero out pointers.
	 */
	void deinit();
	//void initFirstTime();
	//bool TestIfShadersAreSupported();
	/**
	 * \brief Zero's out pointers used by this shader.
	 */
	void zero();

	
	/**
	 * \brief Set the number of channels to be composited by this shader.
	 *
	 * @param numChannels The number of channels to be composited by this shader.
	 */
	void setNumChannels(int numChannels);

	/**
	 * \brief Set the texture id's that represent the rendered channels.
	 */
	void SetTextureIds(int numChannels);
	void SetUserPosition(OmniVec3 position);
	void SetTextureMatrices(D3DXMATRIX *m, int count);


	void setShadersFilenames(
		const char * _fxShaderFilename ,	const char * _NotUsed );
	

	int numChannels;
	char *fxShaderFilename;
	/**
	 * True to have the Omnimap lib use a stencil mask
	 */
	bool StencilMask_enable;
	void StencilMask_Free();
	

	/////////// export /////////

};


#endif