//////////////////////////////
////   needs 
////   // make sure to link to static glew and define glewbuild
////   //use glew_s.lib
////   // avoid glew.dll glew.lib	
////
// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati


#pragma once
#include "OmniMapBase.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <windows.h>
#include <string>

#if defined(USE_VC7)
#define sprintf_s sprintf
#define vsprintf_s vsprintf
#endif
#define OMNIMAP_V(x) { hr = x; if( FAILED(hr) ) { LogSystem()->ReportError("D3D Error:%s %d %d %s\n",  __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }



/*
#ifdef OMNIMAP_EXPORTS
#define OMNIMAP_API __declspec(dllexport)
#else
#pragma comment (lib, "OmniMap.lib")
#define OMNIMAP_API __declspec(dllimport)
#endif
*/


//// MUST INCLUDE ////
#define OMNIMAP_DEFAULT_SHADER_VERTEXSHADER "OmniMapDome.fx"
#define OMNIMAP_DEFAULT_SHADER_FRAGMENTSHADER NULL


#define OMNIMAP_DEFAULT_glViewportsettings0 0
#define OMNIMAP_DEFAULT_glViewportsettings1 0
#define OMNIMAP_DEFAULT_glViewportsettings2 100
#define OMNIMAP_DEFAULT_glViewportsettings3 100


#define OMNIMAP_DEFAULT_GL_STATE_CLEAR_AT_STARTFRAME true
#define OMNIMAP_DEFAULT_GL_STATE_glDisable_GL_DEPTH_TEST false
#define OMNIMAP_DEFAULT_GL_STATE_glDepthMask_FALSE false

#define OMNIMAP_maxChannelFaces 6


#define OMNIMAP_DEFAULT_CHANNEL_USE_FBO true
/////////////////////////////////
///////////////////////
/////////////////////////////////
/////////////////////////////////




/////////////////////////////////
/////////////////////////////////
/////////////////////////////////

class HyperbolaD3D;
class OmniMap_Screen;
class OmniMapChannelD3D;
class OmniMapShaderD3D;


struct OMNIMAP_API D3DXMATRIX;


/**
 * \brief Defines a single render channel for rendering a portion of the final composited 
 * screen image.  This is the DirectX9 implementaion derived from the abstract class OmniMapChannelBase.
 *
 * The OmniMap API's final rendered image is a composite of 1 to 4 render channels.  Those
 * channels' attributes are defined by the OmniMap_Channel class.  The primary attributes of
 * a channel are the camera frustum, and viewing angle which are defined in the cam_info 
 * public member.  It is also possible to have each channel displayed in an inset (heads up display)
 * on the final rendered image.  
 * 
 * Scriptable parameters can be set and accessed through the OmniMapChannelBase::access
 * member variable.  These variable can also be set via the LUA configuration files using the LUA function:
 * SetChannelParameter. See the  OmniMapChannelBase class for base class defined scriptable parameters.
 * The DirectX specific channel attributes are listed here.  They are:
 * <ul>
 *	<li>
 *		<b>MultiSampleQuality</b> : The value set here will be used for the Direct3D call 
 *			CreateRenderTarget when the render target for the channels are set up. 
 *			Be careful not to set this number to high.
 *			Read the Microsoft documentation on CreateRenderTarget for more information.
 *	</li>
 * </ul>
 */
class OMNIMAP_API   OmniMapChannelD3D : public OmniMapChannelBase
{

public: 

	friend class OmniMapD3D;

public:
	/**
	 * \brief Construct an OmniMapChannelD3D Object.
	 *
	 * @param _d3dDevice The IDirect3DDevice9 the application is using for Direct3D rendering.
	 */
	OmniMapChannelD3D(IDirect3DDevice9* _d3dDevice);
	/**
	 * Destroy this channel
	 */
	virtual ~OmniMapChannelD3D();
protected:

	IDirect3DDevice9       *d3dDevice;
	LPD3DXRENDERTOSURFACE   renderToSurface;
	LPDIRECT3DTEXTURE9      dynamicTexture;
	LPDIRECT3DSURFACE9      textureSurface;
	IDirect3DSurface9 *rtSurface;
	IDirect3DSurface9 *deviceBackBuffer;
	IDirect3DSurface9 *rtZBuffer;
	
	void CreateHUDSurface();
	
	void PushSetupMatricies();
	void PopSetupMatricies();
	void SetUpPropertyAccess();
    
	IDirect3DVertexBuffer9 *vertexBuffer;
	IDirect3DVertexBuffer9 *wireframeBuffer;
	int multiSampleQuality;
	void setupMultiSample();

	/**
	 * \brief True to put camera rotation into OpenGL projection matrix.
	 *
	 * True to maintain SPI matrix compatibility. 
	 * Defaults to false
	 * see http://www.sjbaker.org/steve/omniv/projection_abuse.html for more information
	 */
	bool ConsolidateCameraIntoProjectionMatrix;
public:
	/**
	 * \brief Set the near and far clipping planes for this channel.
	 *
	 * Sets the near and far clipping planes for the channel, and recalculates the left, right, top, and bottom
	 * planes.
	 * @param fnear The near clipping plane.
	 * @param ffar The far clipping plane.
	 */
	void SetNearFar(float fnear, float ffar);
	

	/**
	 * \brief Initialize the channel.
	 *
	 * It is required that the resolution of the channel and the renderTarget be set before 
	 * calling this method.
	 */
	void Initialize();

	/**
	 * \brief Make the texture unit for this channel the active texture unit.
	 *
	 * Makes the texture unit for this channel the active texture unit(glActiveTextureARB)
	 * and then binds the texture id that contains the content for this channel to that
	 * active texture unit.
	 */
	void BindTexture(int index);
	/**
	 * \brief Unbind the texture for this channel.
	 */
	void UnbindTexture(int index);
	/**
	 * \brief Draw the contents of this channel to the heads up display.
	 */
	void DrawHeadsUpDisplay();
	/**
	 * \brief Setup matrices and graphics state for rendering this channel.
	 *
	 * This method will be called before rendering to the channel begins. 
	 */
	virtual void beginRenderToChannel();
	/**
	 * \brief Return the matrix and graphics state to its condition prior to the call to
	 * OmniMapChannelBase::beginRenderToChannel.
	 */
	virtual void endRenderToChannel();

	/**
	 * \brief Apply this channel's projection matrix. 
	 *
	 * Multiplies the current matrix by the matrix generated by the viewing frustum
	 * defined by the cam_info class member.
	 */
	virtual void ApplyProjectionMatrix();
	/**
	 * \brief Apply this channel's view matrix.
	 *
	 * Multiplies the current matrix by the matrix in cam_info.Matrix_WorldView.
	 */
	virtual void ApplyViewMatrix();

	/**
	 * \brief Get the view matrix that represent's the camera rotation for this channel.
	 *
	 * @param vm A pointer to the D3DXMATRIX that will be set to the camera rotation for this channel.
	 */
	 void  GetViewMatrix(D3DXMATRIX *vm);

	 /**
	 * \brief Get the left hand projection matrix that represent's the camera viewing frustum for this channel.
	 *
	 * @param vm A pointer to the D3DXMATRIX that will be set to the camera viewing frustum for this channel.
	 */
	 void  GetProjectionMatrix(D3DXMATRIX *vm);

	 /**
	 * \brief Get the right hand projection matrix that represent's the camera viewing frustum for this channel.
	 *
	 * @param projMat A pointer to the D3DXMATRIX that will be set to the camera viewing frustum for this channel.
	 */
	 void GetProjectionMatrixRH(D3DXMATRIX *projMat);

	/**
	 * \brief Get the projective texturing matrix used to project this channel onto the output screen shape.
	 *
	 * This method returns the matrix in a 16 element float array.
	 * @param m The returned matrix.
	 */
	void GetProjTextMatrix(float m[16]);

	/**
	 * \brief Get the projection matrix.
	 *
	 * This method returns the projection matrix in a 16 element float array.
	 *
	 * @param m The returned matrix.
	 */
	void GetProjectionMatrix(float m[16]);

	/**
	 * \brief Get the projective texturing matrix used to project this channel onto the output screen shape.
	 *
	 * This method returns the matrix in a D3DXMATRIX.
	 * @param ptMat The returned matrix.
	 */
	void GetProjTextMatrix(D3DXMATRIX *ptMat);

protected:
	D3DXMATRIX  saveViewMatrix;
	D3DXMATRIX  saveProjMatrix;

	bool getZBufferFormat(D3DFORMAT &retval);
	void GetViewMatrix2(D3DXMATRIX *vm);
	OmniMapShaderD3D *omShader;
	void setOMShader(OmniMapShaderD3D *shader);

};

struct WFVertex
{
	float x, y, z;
	DWORD diffuse;

	enum WFFVF
	{
		FVF_WF_FLAGS = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
};

struct Vertex
{
	float x, y, z;
	float tu, tv;
	
	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
	};
};


/**
 * \brief The primary class for integrating the OmniMap spherical projection library into
 * a DirectX9 application
 *
 * This is the primary class for integrating the OmniMap spherical projection library into
 * an DirectX9 application.  The OmniMap class creates the individual render channels specified
 * by calls to the Create_Channel method.  Those channels are
 * rendered by the application via the callback from ForEachChannel. At the beginning of
 * the callback for each channel, the application is expected to call OmniMapChannelD3D10::beginRenderToChannel.
 * In OmniMapChannelD3D10::beginRenderToChannel, the OmniMap library redirects rendering to its
 * off screen render target and sets up the DirectX9 projection matrix 
 * with the camera frustum
 * and the DirectX9 model view matrix with the camera rotation for the channel being rendered.  
 * Once all of the channels are rendered,
 * they are composited into one image and projected onto the screen.
 *
 * Dome configuration parameters
 * scriptable from LUA are owned by the OmniMap class. 
 * Channel and screen specifics can also be configured using LUA scripts.
 * The LUA-scriptable parameters for the OmniMap class are:<br>
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
 * From OmniMap :
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
class OMNIMAP_API  OmniMapD3D : public OmniMapBase
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
	 *		OmniMapAPI/LuaResources.  If the application developer whishes to move these files, this argument needs to
	 *		be passed to the OmniMap constructor.  The path must use forward slashes as the directory separator.
	 * @param doInit Deafults to true. If true, initialize channels and shaders during construction.  If false, OmniMapD3D::init must
	 *		be called after construction.  Derived classes should call this constructor
	 *		with doInit set to false so that during initialization, methods overridden in the deriving class will be
	 *		used in favor of the inherited classes methods.
	 */
	OmniMapD3D::OmniMapD3D(int _resW, int _resH, IDirect3DDevice9 *_d3dDevice, 
		const char* strStartUpScriptFile = "OmniMapD3DConfig/omnimap_startup.lua",
		const char *luaResDir = "OmniMapD3DConfig/Resources/",
		bool doInit = true);
	/**
	 * Initialize the OmniMapD3D object via execution of the LUA configuration scripts.  Should only be called if the 
	 * doInit argument to the constructor was false.
	 */
	void init();
	/**
	 * Destroys an OmniMap object.  
	 */
	virtual ~OmniMapD3D();
	/**
	 * Composite and draw the images rendered to the channels into the final image.  Draw the
	 * Heads up display if displayHUD is true. When finished, PostRender
	 * resets the Direct3D context to the way it was found before channel rendering began.
	 */
	void OmniMapD3D::PostRender(); 

		/**
	 * \brief Clear the list of channels.
	 *
	 * This method will delete all channels and clear the list of channels.
	 */
	void Clear_Channels();
	/**
	 * Create a channel for rendering a portion of the final composited, projected image into.
	 * In this Direct3D implementation, OmniMapChannelD3D::Create_Channel returns a pointer to
	 * an OmniMapChannelD3D object.
	 * 
	 * @param name The name of the channel.
	 */
	OmniMapChannelBase *Create_Channel(const char * name);


protected:
	void SetUpPropertyAccess();
	
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



	void OmniMapD3D::SetupAllShaderVaribles();
	/**
	 * \brief Draws the heads up display.
	 *
	 * The heads up display shows the each channel without projection warping 
	 * in a small window.
	 */
	virtual void DrawHeadsUpDisplay();
	virtual void LoadAndPush_HUD_Matricies();
	virtual void LoadAndPush_ProjectiveTexturing_Matricies();
	virtual void PopMatricies();
	virtual void Clear();

	
	D3DXMATRIX worldViewProjection;
	D3DXMATRIX worldView;
	//IDirect3DStateBlock9* pStateBlock; // this was added and broke the api compatibility (sizeof (OmniMapD3D) was diferent from the original dll)
	IDirect3DDevice9 *d3dDevice;

	void DrawBG(float scale, float textureScale);
	void InitBG();


	
	///////////////////////////////////////
};



/////////////////////////////////////////////////////////////////////////////////

//////////
// todo add a map of lua definible GLUniformVar*
//////////




