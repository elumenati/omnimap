#ifndef __OMNIAMP_APP_TEXTURE_OGL__
#define __OMNIAMP_APP_TEXTURE_OGL__
// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati


#include "omnimap.h"

/**
 * \brief The primary class for integrating the OmniMap spherical projection library into
 * an OpenGL application, in the case where the application wants to supply a texture id
 * into which channels are rendered.
 *
 * This is the primary class for integrating the OmniMap spherical projection library into
 * an OpenGL application.  The OmniMap class creates the individual render channels specified
 * by calls to the Create_Channel method.  Those channels are
 * rendered by the application via the callback from ForEachChannel. Prior to executing the
 * callback for each channel, the OmniMap library sets up the OpenGL projection matrix 
 * with the camera frustum
 * and the OpenGL model view matrix with the camera rotation for the channel being rendered.  
 * Once all of the channels are rendered,
 * they are composited into one image and projected onto the screen or screens defined by the
 * list of screens specified with the Create_ScreenShapeDome method.
 */
class OMNIMAP_API OmniMapAppTextureOGL :  public OmniMap
{
public:
	/**
	 * \brief Construct an OmniMapAppTextureOGL object.
	 *
	 * @param _resW The width of the final rendered image.
	 * @param _resH The height of the final rendered image.
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
	OmniMapAppTextureOGL(int _resW, int _resH, 
		const char* strStartUpScriptFile= OMNIMAP_DEFAULT_STARTUPSCRIPT,
		const char *luaResDir = 0);
	/**
	 * \brief Create a channel for rendering a portion of the final composited, projected image into.
	 *
	 * In this OpenGL, application-supplied texture id implementation, 
	 * OmniMapChannelBase::Create_Channel creates
	 * an OmniMapChannelATOGL object, and returns a pointer to that object.
	 * 
	 * @param name The name of the channel.
	 * @return A pointer to the channel created.  In this derivation, the channel object created
	 * is an OmniMapChannelATOGL.
	 */
	OmniMapChannelBase *Create_Channel(const char * name);
#ifdef ELUMENATI_INTERNAL
	virtual ~OmniMapAppTextureOGL() {}
#endif
protected:

#ifdef ELUMENATI_INTERNAL
	void popTextureMatrices();
#endif
	
};

class OMNIMAP_API OmniMapChannelATOGL : public OmniMap_Channel
{
public:
	OmniMapChannelATOGL();
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
	 * \brief Set the texture id to be used by this channel.
	 * In this derivation of OmniMapChannelBase, the application supplies the texture to be used by the
	 * channel.  The application is responsible for making sure that the correct image exists in this texture
	 * when OmniMap::PostRender is called.
	 */
	void SetTexureID(unsigned int id);
	/**
	 * \brief Get the texture id for this channel.
	 *
	 * @return The texture id for this channel.
	 */
	unsigned int GetTextureID();
	/**
	 * \brief Initialize the channel.
	 */
	void Initialize();

	void DrawHeadsUpDisplay();
private:
	unsigned int textureID;
protected:

};


#endif