#ifndef __OMNIAMP_APP_TEXTURE_D3D__
#define __OMNIAMP_APP_TEXTURE_D3D__
// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#include "omnimapD3D.h"

/**
 * \brief The primary class for integrating the OmniMap spherical projection library into
 * a D3D application, in the case where the application wants to supply a texture id
 * into which channels are rendered.
 *
 * This is the primary class for integrating the OmniMap spherical projection library into
 * an D3D application.  In this derivaton of OmniMapD3D, OmniMapChannelATD3D is used to implement
 * channels.  The offscreen texture is created, and rendered by the application.  
 * Once all of the channels are rendered,
 * they are composited into one image and projected onto the screen or screens defined by the
 * list of screens specified with the Create_ScreenShapeDome method.
 */
class OMNIMAP_API OmniMapAppTextureD3D :  public OmniMapD3D
{
public:
	/**
	 * \brief Construct an OmniMapAppTextureD3D object.
	 *
	 * @param _resW The width of the final rendered image.
	 * @param _resH The height of the final rendered image.
	 * @param _d3dDevice The IDirect3DDevice9 the application would like the spherically 
	 *			projected output to be rendered to.
	 * @param strStartUpScriptFile The name of the file containing the startup script for
	 *			the OmniMap library.  On Win32 systems, the default is OmniMapConfig/omnimap_startup.lua.
	 *			The startup file loads some Lua Support files.  The location of these files is given by the luaResDir
	 *			parameter.  It also inlcudes two installation-specific files: omnimap_user_edit.lua, and omnimap_dome_wiz_ai.lua.
	 *			If you decide to move the startup file, you must make sure it includes these files from the correct
	 *			location.
	 * @param luaResDir This is the directory that contains Lua support files.  The default is NULL, in which case
	 *		the directory is determined by getting the Windows registry value "LuaResources" in the key 
	 *		"SOFTWARE\Elumenati\Elumenati OmniMap API\OmniMapAPI".  In the OmniMap API release this directory is
	 *		OmniMapAPI/LuaResources.  If the application developer whishes to move these files, this argument needs to
	 *		be passed to the OmniMap constructor.  The path must use forward slashes as the directory separator.
	 */
	OmniMapAppTextureD3D::OmniMapAppTextureD3D(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice,
		const char* strStartUpScriptFile= OMNIMAP_DEFAULT_STARTUPSCRIPT,
		const char *luaResDir = 0);

	virtual ~OmniMapAppTextureD3D();
	/**
	 * \brief Create a channel for rendering a portion of the final composited, projected image into.
	 *
	 * In this OpenGL, application-supplied texture id implementation, 
	 * OmniMapChannelBase::Create_Channel creates
	 * an OmniMapChannelATD3D object, and returns a pointer to that object.
	 * 
	 * @param name The name of the channel.
	 * @return A pointer to the channel created.  In this derivation, the channel object created
	 * is an OmniMapChannelATD3D.
	 */
	OmniMapChannelBase *Create_Channel(const char * name);

	void Clear_Channels();
	
};

/**
 * \brief This class is the derivation of OmniMapChannelD3D that allows the application
 * to supply its own texture to be the imagery for this channel.
 *
 * The proper setup the D3D projection matrix 
 * with the camera frustum and the D3D model view matrix with the 
 * camera rotation is still done in OmniMapChannelATD3D::beginRenderToChannel.  If the application
 * wishes to set up the camera rotation and projection, it can use the OmniMapChannelD3D methods
 * for accessing those matrices.
 * 
*/
class OMNIMAP_API OmniMapChannelATD3D : public OmniMapChannelD3D
{
public:
	OmniMapChannelATD3D(IDirect3DDevice9 *_d3dDevice);
	virtual ~OmniMapChannelATD3D();
	/**
	 * \brief Setup matrices for rendering this channel.
	 *
	 * This method will be called before rendering to the channel begins. 
	 */
	virtual void beginRenderToChannel();
	/**
	 * \brief Return the matrix to its condition prior to the call to
	 * OmniMapChannelBase::beginRenderToChannel.
	 */
	virtual void endRenderToChannel();
	/**
	 * \brief Make the texture unit for this channel the active texture unit.
	 *
	 * Makes the texture unit for this channel the active texture unit(glActiveTextureARB)
	 * and then binds the texture id that contains the content for this channel to that
	 * active texture unit(glBindTexture).
	 */
	void BindTexture(int index);
	/**
	 * \brief Unbind the texture for this channel.
	 */
	void UnbindTexture(int index);

	/**
	 * \brief Set the texture to be used by this channel.
	 * In this derivation of OmniMapChannelBase, the application supplies the texture to be used by the
	 * channel.  The application is responsible for making sure that the correct image exists in this texture
	 * when OmniMap::PostRender is called.
	 */
	void SetTextureID(LPDIRECT3DTEXTURE9 id);
	/**
	 * \brief Get the texture for this channel.
	 *
	 * @return The texture for this channel.
	 */
	LPDIRECT3DTEXTURE9 GetTextureID();
	/**
	 * \brief Initialize the channel.
	 *
	 * Since the application is responsible for providing the texture, this method does nothing
	 * in this derivation of OmniMapChannelD3D.
	 */
	void Initialize();

};


#endif