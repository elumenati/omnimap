#ifndef __OMNIMAP__H
#define __OMNIMAP__H
/// * Bug alert: the dome renders incorrectly when it points near 0,-1,0 due to a zero angle bug.


//////////////////////////////
////   needs 
////   // make sure to link to static glew and define glewbuild
////   //use glew_s.lib
////   // avoid glew.dll glew.lib	
////
// OmniMap Geometry Correction Libs
// (c)2006, 2010  Elumenati

#ifdef WIN32
#pragma once
#endif
#ifdef __APPLE__
#include <OmniMapAPI/OmniMapBase.h>
#else
#include "OmniMapBase.h"
#endif


//// MUST INCLUDE ////
#define OMNIMAP_DEFAULT_SHADER_VERTEXSHADER "omnimap_vertex.vert"
#define OMNIMAP_DEFAULT_SHADER_FRAGMENTSHADER "omnimap_fragment.frag"
#define OMNINAP_DEFAULT_USE_MIRROR false


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

class GLFragmentShader;  // nondll exported from glshader.h
class GLVertexShader;
class GLShader;
class GLProgram;
class GLAttributeVar;
class GLUniformVar;
class RenderChannel;
class displayHyperbola;
class OmniMap_Screen;
class OmniMap_Channel;
class OmniMap;

/**
 * \brief The OpenGL implementation of OmniMapShaderBase.
 *
 * This is the OpenGL implementation of the abstract class OmniMapShaderBase.  
 */
class OMNIMAP_API   OmniMap_Shader : public OmniMapShaderBase
{
	friend class OmniMap;

protected:

	//GLSL Shaders
	GLProgram* OmniMap_ShaderCombo;
	GLVertexShader* OmniMap_VertexShader;
	GLFragmentShader* OmniMap_FragmentShader;

	// Fragment Parameters
	GLUniformVar*  glsl_Plane_Eq_XYZ;
	GLUniformVar*  glsl_Plane_Eq_U;
	GLUniformVar*  glsl_Plane_Eq_V;

	GLUniformVar*  glsl_channelTexture0;
	GLUniformVar*  glsl_channelTexture1;
	GLUniformVar*  glsl_channelTexture2;
	GLUniformVar*  glsl_channelTexture3;
	GLUniformVar*  glsl_channelTexture4;
	GLUniformVar*  glsl_channelTexture5;

	GLUniformVar*  glsl_userposition;
	GLUniformVar*  _glsl_StencilMask;
	GLUniformVar*  _glsl_StencilMaskEnabled;
	GLUniformVar*  _glsl_debug1;


	// todo add a map of lua definible GLUniformVar*

public:
	/**
	 * Construct an OmniMap_Shader object.
	 */
	OmniMap_Shader();
	/**
	 * Destroy an OmniMap_Shader object.
	 */
	~OmniMap_Shader();
	/**
	 * Enable the shader for subsequent 3D rendering.
	 */
	void EnableShader();
	/**
	 * Disable the shader for subsequent 3D rendering.
	 */
	void DisableShader();

	void setUseMirror(bool _useMirror) { useMirror = _useMirror; }

	

#ifdef ELUMENATI_INTERNAL
	int getShaderVarID(const char *name);
#endif
protected:
	void reload();
	virtual void init();
	void deinit();
	virtual void initFirstTime();
	bool TestIfShadersAreSupported();
	void zero();
	
	void SetTextureIds(int numChannels, int *ids);

	void SetPlaneEquations(int numChannels, float *plane_Eq_XYZ,
		float *plane_Eq_U, float *plane_Eq_V);


	/////////// export /////////


	//mask stuff...
	 virtual void StencilMask_Load(char *StencilMask_FileName);
	 virtual void StencilMask_Free();
	/**
	 * Bind it to the glsl shader's texture 
	 */
	 virtual void StencilMask_Bind(int texture_number_not_arb);

	/**
	 * Opengl ID of the texture mask
	 */
		unsigned int StencilMask_TextureID;
	/**
	 * True to have the Omnimap lib use a stencil mask
	 */
	bool StencilMask_enable;

	bool useMirror;

	



};


/**
 * \brief Defines a single render channel for rendering a portion of the final composited 
 * screen image.  This is the OpenGL implementaion derived from the abstract class OmniMapChannelBase.
 *
 * The OmniMap API's final rendered image is a composite of 1 to 4 render channels.  Those
 * channels' attributes are defined by the OmniMap_Channel class.  The primary attributes of
 * a channel are the camera frustum, and viewing angle which are defined in the cam_info 
 * public member.  It is also possible to have each channel displayed in an inset (heads up display)
 * on the final rendered image.  
 * 
 * The channel attributes are LUA-scriptable.  The parameters that can be configure via
 * LUA script are:

 */
class OMNIMAP_API   OmniMap_Channel : public OmniMapChannelBase
{

public: 

	friend class OmniMap;

public:
	/**
	 * Construct an OmniMap_Channel Object.
	 */
	OmniMap_Channel();
	/**
	 * Destroy this channel
	 */
	~OmniMap_Channel();
	
	/**
	 * Set the near and far clip planes and recalculate the 
	 * rest of the frustum using those new values
	 *
	 * @param fnear The near clip plane.
	 * @param ffar The far clip plane.
	 */
	void SetNearFar(float fnear, float ffar);

	void GetProjTextMatrix(float m[16]);

	void DrawBox(float x, float y, float size);

protected:
	RenderChannel *prc_RenderChannel;
	void PushSetupMatricies();
	void PopSetupMatricies();
	void SetUpPropertyAccess();


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
	 * \brief Initialize the channel.
	 *
	 * It is required that the resolution of the channel be set before calling this method.
	 */
	void Initialize();
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

	virtual unsigned int GetTextureID();
	virtual unsigned int GetFrameBufferID();

	virtual void GetProjectionMatrix(float m[16]);

#ifdef OM_PICKING
	void beginRenderPickingToChannel(int mouseX, int mouseY);
	void endRenderPickingToChannel();
#endif

};


/**
 * \brief The primary class for integrating the OmniMap spherical projection library into
 * an OpenGL application
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
 *
 * Dome configuration parameters
 * scriptable from LUA are owned by the OmniMap class. 
 * Channel and screen specifics can also be configured using LUA scripts. See the OmniMapScreenDomeD3D
 * and OmniMap_Channel documentation for details on those parameters.
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
 *		<li><b>glViewportX</b> : The X value of the lower left corner of the OpenGL viewport rectangle, in pixels.
 *		</li>
 *		<li><b>glViewportY</b> : The y value of the lower left hand corner of the OpenGL viewport
 *		</li>
 *		<li><b>glViewportWidth</b> : The width of the OpenGL viewport
 *		</li>
 *		<li><b>glViewportHeight</b> : The height of the OpenGL viewport
 *		</li>
 *		<li><b>GL_STATE_CLEAR_AT_STARTFRAME</b> : True to have the OmniMap library clear
 *			the screen before calling the applications render function.
 *		</li>
 *		<li><b>GL_STATE_glDisable_GL_DEPTH_TEST</b> : True to have the OmniMap library disable OpenGL
 *			depth testing.
 *		</li>
 *		<li><b>GL_STATE_glDepthMask_FALSE</b> : True to have the OpenGL depth mask set to false.
 *		</li>
 *		<li><b>SAVE_AFTER_EVERY_MESSAGE</b>
 *		</li>
 *	</ul>
 */
class OMNIMAP_API  OmniMap : public OmniMapBase
{
public:


protected:
	void SetUpPropertyAccess();

	/*
	void LogCppProperties(std::string precursor)
	{
		typedef std::list<HashTable<Property>::HashTableNode<Property> > Omnimap_CPP_PropertyList ;
		typedef std::list<HashTable<Property>::HashTableNode<Property> > * pOmnimap_CPP_PropertyList ;
		typedef Omnimap_CPP_PropertyList::iterator Omnimap_CPP_PropertyListItor;
		for(int i =0; i != m_properties.GetTableSize(); i++)
			for(Omnimap_CPP_PropertyListItor itor = m_properties.GetHashTable()[i].begin(); itor != m_properties.GetHashTable()[i].end(); itor++)
				LogSystem()->ReportMessage("%s%s",precursor.c_str(),itor->m_key.c_str());
	}
	*/
	



public: // lua needs access to this
	/**
	 * True to have the OmniMap library clear
	 * the screen before compositing the channel images to the spherical projection.
	 */
	bool GL_STATE_CLEAR_AT_STARTFRAME;
	/**
	 * True to have the OmniMap library disable OpenGL depth testing.
	 */
	bool GL_STATE_glDisable_GL_DEPTH_TEST; 
	/**
	 * True to have the OmniMap library disable OpenGL depth buffer writing.
	 */
	bool GL_STATE_glDepthMask_FALSE; 

	bool useMirror;
	float mirrorRadius;
	OmniVec3 mirrorCenter;
	float mirrorGrace;
	OmniVec3 mirrorProjectorPosition;
	OmniVec3 mirrorProjectorUp;
	



public:
	void SetupScriptingCommands();
	/**
	 * Create a channel for rendering a portion of the final composited, projected image into.
	 * In this OpenGL implementation, OmniMapChannelBase::Create_Channel returns a pointer to
	 * an OmniMap_Channel object.
	 * 
	 * @param name The name of the channel.
	 */
	OmniMapChannelBase *Create_Channel(const char * name);


	/**
	 * \brief Create a screen shape for compositing and projecting the rendered channels to
	 * the display.
	 * 
	 * For this implementation of the absract class OmniMapBase, 
	 * this method creates an OmniMapScreenDomeD3D object.
	 * This enables derived implementations of the OmniMapBase
	 * class to create customm screens derived from OmniMap_Screen.
	 */
	//OmniMap_Screen *Create_ScreenShape(const char * name);
	//OmniMap_Screen *Create_ScreenShape(const char * name,const char * type);  // optional type argument   dome, cylinder, quad, etc...




public:

	/**
	 * Construct an OmniMap object.
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
	 * @param doInit Deafults to true. If true, initialize channels and shaders during construction.  If false, OmniMapD3D::init must
	 *		be called after construction.  Derived classes should call this constructor
	 *		with doInit set to false so that during initialization, methods overridden in the deriving class will be
	 *		used in favor of the inherited classes methods.
	 */
	OmniMap(int _resW, int _resH, const char* strStartUpScriptFile = OMNIMAP_DEFAULT_STARTUPSCRIPT,
		const char *luaResDir = "OmniMapConfig/Resources/", bool doInit = true);
	void init();
	/**
	 * Destroys an OmniMap object.  
	 */
	virtual ~OmniMap();
	/**
	 * Composite and draw the images rendered to the channels into the final image.  Draw the
	 * Heads up display if displayHUD is true. When finished, PostRender
	 * resets the OpenGL context to the way it was found before channel rendering began.
	 */
	virtual void PostRender(); 

protected:
	char fragmentShaderFilename[512];
	char vertexShaderFilename[512];
#ifdef ELUMENATI_INTERNAL	
	virtual
#endif
		void SetupAllShaderVaribles();
	/**
	 * \brief Draws the heads up display.
	 *
	 * The heads up display shows the each channel without projection warping 
	 * in a small window.
	 */
	virtual void DrawHeadsUpDisplay();
	/**
	 * Load and push the heads up display matricies.
	 */
	virtual void LoadAndPush_HUD_Matricies();
	/**
	 * Load and push the projective texturing matrices
	 */
	virtual void LoadAndPush_ProjectiveTexturing_Matricies();
	/**
	 * Pop Matrices
	 */
	virtual void PopMatricies();
	/**
	 * Clear the screen
	 */
	virtual void Clear();
	/**
	 * Creates a viewing matrix derived from an eye point, a reference point indicating the
	 * center of the scene, and an UP vector all defined by the OmniMapBase::ProjectorPosition data member.
	 * 
	 */
	void doGluLookAt();
	///////////////////////////////////////

	unsigned int logoTextureID;
	void LoadBGTexture();
	//void RenderBG(unsigned int tid, float x, float y, float w, float h, float r, float s);
	void RenderBG(float x, float y, float w, float h, float r, float s);

};




/////////////////////////////////////////////////////////////////////////////////

//////////
// todo add a map of lua definible GLUniformVar*
//////////
#endif



