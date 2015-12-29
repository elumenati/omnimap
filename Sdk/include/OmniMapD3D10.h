#ifndef __OMNIMAP__D3D10__H_
#define __OMNIMAP__D3D10__H_

#include <d3d9.h>

#include <dxgi.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <dxerr.h>
#include "OmniMapBase.h"


//// MUST INCLUDE ////
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define OMNIMAP_DEFAULT_SHADER_VERTEXSHADER "OmniMapDome10.fx"
#define OMNIMAP_DEFAULT_SHADER_FRAGMENTSHADER NULL


#define OMNIMAP_DEFAULT_glViewportsettings0 0
#define OMNIMAP_DEFAULT_glViewportsettings1 0
#define OMNIMAP_DEFAULT_glViewportsettings2 1024
#define OMNIMAP_DEFAULT_glViewportsettings3 1024


#define OMNIMAP_DEFAULT_GL_STATE_CLEAR_AT_STARTFRAME true
#define OMNIMAP_DEFAULT_GL_STATE_glDisable_GL_DEPTH_TEST false
#define OMNIMAP_DEFAULT_GL_STATE_glDepthMask_FALSE false

#define OMNIMAP_maxChannelFaces 6


#define OMNIMAP_DEFAULT_CHANNEL_USE_FBO true

struct OMNIMAP_API D3DXMATRIX;


/**
 * \brief The primary class for integrating the OmniMap spherical projection library into
 * a DirectX10 application
 *
 * This is the primary class for integrating the OmniMap spherical projection library into
 * an DirectX10 application.  The OmniMap class creates the individual render channels specified
 * by calls to the Create_Channel method.  Those channels are
 * rendered by the application via the callback from OmniMapD3D10::ForEachChannel. At the beginning of
 * the callback for each channel, the application is expected to call OmniMapChannelD3D10::beginRenderToChannel.
 * In OmniMapChannelD3D10::beginRenderToChannel, the OmniMap library redirects rendering to its
 * off screen render target.  Each channel has a viewing matrix offset and a projection matrix.  The viewing offset
 * needs to be used to offset the application's viewing matrix.  The channel's offset can be accessed via a call to
 * OmniMapChannelD3D10::GetViewMatrix.  The projection matrix is obtained by calling OmniMapChannelD3D10::GetProjMatrix.
 * Once all of the channels are rendered,
 * they are composited into one image and projected onto the output screen by calling OmniMapD3D10::PostRender.
 *
 * Dome configuration parameters
 * scriptable from LUA are owned by the OmniMapD3D10 class. 
 * Channel and screen specifics can also be configured using LUA scripts. 
 * The LUA-scriptable parameters for the OmniMapD3D10 class are:<br>
 * From OmniMapBase:
 * 	<ul>
 *		<li><b>clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a</b> : 
 *			The clear color to be used to clear the frame before the final compositing of the
 *			 channel images into the final spherical projection.
 *		</li>
 *		<li><b>AudiencePosition_x, AudiencePosition_y, AudiencePosition_z</b> : The audience position
 *		</li>
 *		<li><b>ProjectorPosition.lookAtpos.x, ProjectorPosition.lookAtpos.y, ProjectorPosition.lookAtpos.z</b> :
 *			The direction in which the projecter is pointed.
 *		</li>
 *		<li><b>ProjectorPosition.headsUp.x, ProjectorPosition.headsUp.y, ProjectorPosition.headsUp.z</b> :
 *			The up vector for the projector.
 *		</li>
 *		<li><b>ProjectorPosition.pos.x, ProjectorPosition.pos.y, ProjectorPosition.pos.z</b> :
 *			The position of the projector.
 *		</li>
 *		<li><b>displayHUD</b> : True to show the results of the render channels in a heads up
 *			display, False to not show them.
 *		</li>
 *		<li><b>resWidth</b> : The width of the projector screen
 *		</li>
 *		<li><b>resHeight</b> : The height of the projector screen.
 *		</li>
 * </ul>
 * From OmniMapD3D10 :
 * <ul>
 *		<li><b>glViewportX</b> : The X value of the lower left corner of the viewport rectangle, in pixels.
 *		</li>
 *		<li><b>glViewportY</b> : The y value of the lower left hand corner of the viewport
 *		</li>
 *		<li><b>glViewportWidth</b> : The width of the viewport
 *		</li>
 *		<li><b>glViewportHeight</b> : The height of the viewport
 *		</li>
 *		<li><b>GL_STATE_CLEAR_AT_STARTFRAME</b> : True to have the OmniMap library clear
 *			the screen before calling the applications render function.
 *		</li>
 *		<li><b>GL_STATE_glDisable_GL_DEPTH_TEST</b> : True to have the OmniMap library disable
 *			depth testing.
 *		</li>
 *		<li><b>GL_STATE_glDepthMask_FALSE</b> : True to have the depth mask set to false.
 *		</li>
 *		<li><b>SAVE_AFTER_EVERY_MESSAGE</b>
 *		</li>
 *	</ul>
 */
class OMNIMAP_API OmniMapD3D10 : public OmniMapBase
{
public:
	/**
	 * Construct an OmniMapD3D object.
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
	 *		Resources directory.  If the application developer whishes to move these files, this argument needs to
	 *		be passed to the OmniMap constructor.  The path must use forward slashes as the directory separator.
	 * @param doInit Deafults to true. If true, initialize channels and shaders during construction.  If false, OmniMapD3D::init must
	 *		be called after construction.  Derived classes should call this constructor
	 *		with doInit set to false so that during initialization, methods overridden in the deriving class will be
	 *		used in favor of the inherited classes methods.
	 */
	OmniMapD3D10(int _resW, int _resH, ID3D10Device *_d3dDevice, 
		const char* strStartUpScriptFile = "OmniMapD3D10Config/omnimap_startup.lua",
		const char *luaResDir = "OmniMapD3D10Config/Resources/",
		bool doInit = true);

	/**
	 * Initialize the OmniMapD3D object via execution of the LUA configuration scripts.  Should only be called if the 
	 * doInit argument to the constructor was false.
	 */
	void init();
	/**
	 * Destroys an OmniMap object.  
	 */
	virtual ~OmniMapD3D10();

	/**
	 * Create a channel for rendering a portion of the final composited, projected image into.
	 * In this Direct3D 10 implementation, OmniMapChannelD3D10::Create_Channel returns a pointer to
	 * an OmniMapChannelD3D10 object.
	 * 
	 * @param name The name of the channel.
	 */
	virtual OmniMapChannelBase *Create_Channel(const char * name);

	/**
	 * \brief Composite and draw the images rendered to the channels into the final image.  
	 *
	 * Draw the Heads up display if displayHUD is true. When finished, PostRender
	 * resets the Direct3D context to the way it was found before channel rendering began.
	 */
	virtual void PostRender();

	/**
	 * \brief Clear the screen.
	 *
	 * This method uses the color defined by clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a set in the 
	 * LUA configuration scripts.
	 */
	virtual void Clear();

	void CreateTextureFromFile(ID3D10Device *d3dDevice, char *filename, ID3D10ShaderResourceView **shaderResView, 
								 ID3D10Texture2D **texture);
	void CreateTextureFromMemory(ID3D10Device *d3dDevice, unsigned char *textureData, int size, ID3D10ShaderResourceView **shaderResView, 
								 ID3D10Texture2D **texture);
	
protected:
	
	virtual void construct(bool doInit);
	virtual void SetUpPropertyAccess();
	virtual void SetupHudMatrix();
	virtual void DrawHeadsUpDisplay();
	virtual void SetProjectiveTexturingMatricies();
	virtual void SetupAllShaderVaribles();
	void CreateLogoSurface();
	void DrawBG();
	ID3D10Device *d3d10Device;
	ID3D10StateBlock *pStateBlock;
	ID3D10RasterizerState *pRasterState;

	ID3D10EffectMatrixVariable *viewProjMatrix;
	D3DXMATRIX worldViewProjection;
	D3DXMATRIX worldView;

	/**
	 * True to have the OmniMap library clear
	 * the screen before compositing the channel images to the spherical projection.
	 */
	bool GL_STATE_CLEAR_AT_STARTFRAME;
	/**
	 * True to have the OmniMap library disable depth testing.
	 */
	bool GL_STATE_glDisable_GL_DEPTH_TEST; 
	/**
	 * True to have the OmniMap librarydisable depth buffer writing.
	 */
	bool GL_STATE_glDepthMask_FALSE;

	char fxShaderFilename[512];

	

};
#endif