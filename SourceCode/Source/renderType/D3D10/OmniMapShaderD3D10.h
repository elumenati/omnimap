#ifndef _OMNIMAP_SHADER_D3D10__H_
#define _OMNIMAP_SHADER_D3D10__H_
#include "OmniMapBase.h"
#include <d3d10.h>
#include <d3dx10.h>
#include <dxerr.h>

class OMNIMAP_API OmniMapShaderD3D10 : public OmniMapShaderBase
{
	friend class OmniMapD3D10;

public :
	/**
	 * \brief Construct an OmniMapShaderD3D object.
	 *
	 * @param _d3dDevice The Direct 3D device the application is rendering to.
	 */
	OmniMapShaderD3D10(ID3D10Device* _d3d10Device);

	/**
	 * \brief Destroys an OmniMapShaderD3D.  
	 * 
	 * This will release memory used by the Direct 3D effect used by the shader.
	 *
	 */
	~OmniMapShaderD3D10();
	/** 
	 * \brief Apply the technique contained in the OmniMap Shader effect.
	 *
	 * This method enables the shader to be used to project the contents of the 
	 * rendered channels into the dome.
	 */
	void EnableShader();

	/** 
	 * \brief Does nothing in the D3D 10 implementation.
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
	ID3D10EffectTechnique *EnableHUDShader(ID3D10ShaderResourceView* renderTargetSRV);

	void setTexture(int index, ID3D10ShaderResourceView* texture);

	/**
	 * \brief Return the technique used to render the final screen image.
	 *
	 * @return The effect technique that will render the final screen image.
	 */
	ID3D10EffectTechnique *getOMTechnique();

	
	/**
	 * \brief The layout for representing vertex buffers that contain a position followed by texture coordinates.
	 *
	 * The layout is float x,float y,float z,float s, float t. x, y, and z are the position; s and t are the
	 * texture coordinates.  This layout is used by ScreenRendererD3D10 to render the vertex buffer of type:
	 * ScreenGeometryBase::TextureCoordVertex.
	 */
	static ID3D10InputLayout*      PositionTextureLayout;
	/**
	 * \brief The layout for representing vertex buffers that contain a position.
	 *
	 * The layout is float x,float y,float z. x, y, and z are the position.  This layout is used by ScreenRendererD3D10 to render the vertex buffer of type:
	 * ScreenGeometryBase::SimpleVertex.
	 */
	static ID3D10InputLayout*      PositionLayout;
	/**
	 * \brief The layout for representing vertex buffers that contain a position, followed by a color.
	 *
	 * The layout is float x,float y,float z, int color. x, y, and z are the position, 
	 * color is the color of the vertex.
	 */
	static ID3D10InputLayout*      PositionColorLayout;

	

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
	ID3D10EffectMatrixVariable *viewProjMatrixSV;
	ID3D10EffectMatrixVariable *projMatrixSV;
	ID3D10EffectMatrixVariable *viewMatrixSV;
	ID3D10EffectMatrixVariable *projTextureMatricesSV;
	ID3D10EffectScalarVariable *numChannelsSV;
	ID3D10EffectScalarVariable *yScaleSV;
	ID3D10EffectScalarVariable *yOffset1SV;
	ID3D10EffectScalarVariable *yOffset2SV;
	ID3D10EffectShaderResourceVariable* channelTexturesSV;
	ID3D10EffectShaderResourceVariable* texture0SV;
	ID3D10EffectShaderResourceVariable *stencilMaskTextureSV;
	ID3D10EffectShaderResourceVariable **channelTexturesSVArr;
	ID3D10EffectScalarVariable *stencilMaskEnabledSV;

	ID3D10EffectShaderResourceVariable *hudTextureSV;
	ID3D10EffectMatrixVariable *hudProjMatrixSV;



	char *fxShaderFilename;
	/**
	 * True to have the Omnimap lib use a stencil mask
	 */
	bool StencilMask_enable;
	void StencilMask_Free();


	//D3D Shaders
	ID3D10Effect *omnimapFX;
	ID3D10Device *d3d10Device;
	ID3D10EffectTechnique *omniMapTechnique;
	ID3D10EffectTechnique *hudTechnique;
	
	
	ID3D10Texture2D *maskTexture;
	ID3D10ShaderResourceView *maskTextureSRV;
	ID3D10ShaderResourceView *textureTable[OMNIMAP_maxChannelFaces];

	void setHudProjMatrix(float *matrix);
	


};

#endif