#ifndef _OMNIMAP_SHADER_D3D11__H_
#define _OMNIMAP_SHADER_D3D11__H_
#include "OmniMapBase.h"
#include <d3dx9math.h>
#include <d3d11.h>
#include <d3dx11.h>
// ADD TO INCLUDE PATH
//C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Samples\C++\Effects11\Inc 
// ADD TO LIB PATH
//C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Samples\C++\Effects11\Release
// also you must compile the sln file in effects 11 as release
#include <d3dx11effect.h>
#include <dxerr.h>

class OMNIMAP_API OmniMapShaderD3D11 : public OmniMapShaderBase
{
	friend class OmniMapD3D11;

public :
	/**
	 * \brief Construct an OmniMapShaderD3D object.
	 *
	 * @param _d3dDevice The Direct 3D device the application is rendering to.
	 */
	OmniMapShaderD3D11(ID3D11Device* _d3d11Device, ID3D11DeviceContext *_d3dDeviceContext);

	/**
	 * \brief Destroys an OmniMapShaderD3D.  
	 * 
	 * This will release memory used by the Direct 3D effect used by the shader.
	 *
	 */
	~OmniMapShaderD3D11();
	/** 
	 * \brief Apply the technique contained in the OmniMap Shader effect.
	 *
	 * This method enables the shader to be used to project the contents of the 
	 * rendered channels into the dome.
	 */
  void EnableShader();

	/** 
	 * \brief Does nothing in the D3D 11 implementation.
	 *
	 */
	void DisableShader();

	/**
	 * \brief Load the Stencil mask.
	 *
	 * Loads the stencil mask image from the given file.
	 * @param StencilMask_FileName The file name of the texture to be loaded.
	 */
	void StencilMask_Load(char *StencilMask_FileName);

	/**
	* \brief Bind the stencil mask to a texture.
	*/
	void StencilMask_Bind(int texture_number_not_arb);

	/**
	 * \brief Enable the heads-up display shader.
	 *
	 * Enables the technique used to render the heads-up display that shows the contents of each
	 * channel in a small window.
	 * @param renderTargetSRV The shader resource view for the render target texture to be displayed
	 *	in this heads-up window.
	 * @return
	 */
	ID3DX11EffectTechnique *EnableHUDShader(ID3D11ShaderResourceView* renderTargetSRV);

	void setTexture(int index, ID3D11ShaderResourceView* texture);

	/**
	 * \brief Return the technique used to render the final screen image.
	 *
	 * @return The effect technique that will render the final screen image.
	 */
	ID3DX11EffectTechnique *getOMTechnique();

	
  virtual void SetD3D11DeviceContext(ID3D11DeviceContext *_d3dDeviceContext);


	/**
	 * \brief The layout for representing vertex buffers that contain a position followed by texture coordinates.
	 *
	 * The layout is float x,float y,float z,float s, float t. x, y, and z are the position; s and t are the
	 * texture coordinates.  This layout is used by ScreenRendererD3D11 to render the vertex buffer of type:
	 * ScreenGeometryBase::TextureCoordVertex.
	 */
	static ID3D11InputLayout*      PositionTextureLayout;
	/**
	 * \brief The layout for representing vertex buffers that contain a position.
	 *
	 * The layout is float x,float y,float z. x, y, and z are the position.  This layout is used by ScreenRendererD3D11 to render the vertex buffer of type:
	 * ScreenGeometryBase::SimpleVertex.
	 */
	static ID3D11InputLayout*      PositionLayout;
	/**
	 * \brief The layout for representing vertex buffers that contain a position, followed by a color.
	 *
	 * The layout is float x,float y,float z, int color. x, y, and z are the position, 
	 * color is the color of the vertex.
	 */
	static ID3D11InputLayout*      PositionColorLayout;

	
protected:

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
	void SetYOffsets(float yScale, float yOffset1, float yOffset2);
	void SetViewProjMats(float *projMat, float *viewMat);


	void setShadersFilenames(
		const char * _fxShaderFilename ,	const char * _NotUsed );
	

	int numChannels;
	ID3DX11EffectMatrixVariable *viewProjMatrixSV;
	ID3DX11EffectMatrixVariable *projMatrixSV;
	ID3DX11EffectMatrixVariable *viewMatrixSV;
	ID3DX11EffectMatrixVariable *projTextureMatricesSV;
	ID3DX11EffectScalarVariable *numChannelsSV;
	ID3DX11EffectScalarVariable *yScaleSV;
	ID3DX11EffectScalarVariable *yOffset1SV;
	ID3DX11EffectScalarVariable *yOffset2SV;
	ID3DX11EffectShaderResourceVariable* channelTexturesSV;
	ID3DX11EffectShaderResourceVariable* texture0SV;
	ID3DX11EffectShaderResourceVariable *stencilMaskTextureSV;
	ID3DX11EffectShaderResourceVariable **channelTexturesSVArr;
	ID3DX11EffectScalarVariable *stencilMaskEnabledSV;

	ID3DX11EffectShaderResourceVariable *hudTextureSV;
	ID3DX11EffectMatrixVariable *hudProjMatrixSV;



	char *fxShaderFilename;
	/**
	 * True to have the Omnimap lib use a stencil mask
	 */
	bool StencilMask_enable;
	void StencilMask_Free();


	//D3D Shaders
	ID3DX11Effect *omnimapFX;
  ID3D11Device *d3dDevice;
  ID3D11DeviceContext *d3dDeviceContext;
	ID3DX11EffectTechnique *omniMapTechnique;
	ID3DX11EffectTechnique *hudTechnique;
	
	
	ID3D11Texture2D *maskTexture;
	ID3D11ShaderResourceView *maskTextureSRV;
	ID3D11ShaderResourceView *textureTable[OMNIMAP_maxChannelFaces];

	void setHudProjMatrix(float *matrix);
	


};

#endif