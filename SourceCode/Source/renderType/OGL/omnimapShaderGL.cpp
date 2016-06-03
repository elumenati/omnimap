// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#define MIRROR_BALL 1
#include <string>
#include <vector>
#include "omnimap.h"
#include <assert.h>
#include <math.h>

#include "tools/GLShader.h"

#include "Shaders/MirrorShaderFrag.h"
#include "Shaders/MirrorShaderVert.h"

#include "Shaders/omnimap_vertex.h"
#include "Shaders/omnimap_fragment.h"

#include "Shaders/omnimap_vertexCobra.h"
#include "Shaders/omnimap_fragmentCobra.h"

#include "omnitypedefs.h"
#include "OM_ErrorHandling.h"



#define EH_GL_ASSERT(STR) \
  EH_Zero(checkGLError()); \
  assert(checkGLError() && STR);



extern std::string preprocessorMacroGlobal;



extern int OmniMap_getGL_TEXTUREN_ARB_From_N(int n);





////////////////////////////////////////////////////////
//////////////OmniMap_Shader
///////////////////////////////////
OmniMap_Shader::OmniMap_Shader()
{
	StencilMask_TextureID = 0;
	StencilMask_enable = false;
  _glsl_StencilMaskEnabled=NULL;
  _glsl_StencilMask = NULL;

	VertexShaderFilename = NULL;
	FragmentShaderFilename = NULL;

  OmniMap_ShaderCombo = NULL;
  OmniMap_VertexShader = NULL;
  OmniMap_FragmentShader = NULL;
  glsl_channelTexture0 = NULL;
  glsl_channelTexture1 = NULL;
  glsl_channelTexture2 = NULL;
  glsl_channelTexture3 = NULL;

	glsl_channelTexture4= NULL;
	glsl_channelTexture5= NULL;
	initFirstTime();
}




OmniMap_Shader::~OmniMap_Shader()
{
  EH_DECLARE;
	EH_Log("OmniMap_OmniMapShader unloading...");

	deinit();
  StencilMask_Free();

  if (vertexPrelude) { delete [] vertexPrelude; vertexPrelude = NULL; }
  if (fragmentPrelude) { delete [] fragmentPrelude; fragmentPrelude = NULL; }

  if (VertexShaderFilename) { delete [] VertexShaderFilename; VertexShaderFilename=NULL; }
  if (FragmentShaderFilename ) { delete []FragmentShaderFilename ; FragmentShaderFilename =NULL; }
}



void OmniMap_Shader::zero()
{
}



void OmniMap_Shader::deinit()
{
  if (OmniMap_ShaderCombo) {
    if (OmniMap_VertexShader) OmniMap_ShaderCombo->removeShader(OmniMap_VertexShader);
    if (OmniMap_FragmentShader) OmniMap_ShaderCombo->removeShader(OmniMap_FragmentShader);
    OmniMap_ShaderCombo->flagProgramDeletion();
  }
  if (OmniMap_VertexShader) OmniMap_VertexShader->flagShaderDeletion();
  if (OmniMap_FragmentShader) OmniMap_FragmentShader->flagShaderDeletion();

  if (OmniMap_ShaderCombo) { delete OmniMap_ShaderCombo; OmniMap_ShaderCombo = NULL; }
  if (OmniMap_VertexShader) { delete OmniMap_VertexShader; OmniMap_VertexShader = NULL; }
  if (OmniMap_FragmentShader) { delete OmniMap_FragmentShader; OmniMap_FragmentShader = NULL; }
#ifndef WIN32
  if (glsl_channelTexture0) { delete glsl_channelTexture0; glsl_channelTexture0 = NULL; }
#else
  if (glsl_channelTexture0) { delete glsl_channelTexture0; glsl_channelTexture0 = NULL; }
  if (glsl_channelTexture1) { delete glsl_channelTexture1; glsl_channelTexture1 = NULL; }
  if (glsl_channelTexture2) { delete glsl_channelTexture2; glsl_channelTexture2 = NULL; }
  if (glsl_channelTexture3) { delete glsl_channelTexture3; glsl_channelTexture3 = NULL; }
  if (glsl_channelTexture4) { delete glsl_channelTexture4; glsl_channelTexture4 = NULL; }
  if (glsl_channelTexture5) { delete glsl_channelTexture5; glsl_channelTexture5 = NULL; }
#endif

  if (_glsl_StencilMask) { delete _glsl_StencilMask; _glsl_StencilMask = NULL; }
  if (_glsl_StencilMaskEnabled) { delete _glsl_StencilMaskEnabled; _glsl_StencilMaskEnabled = NULL; }
}



void OmniMap_Shader::reload()
{
  EH_DECLARE;
  EH_Log("OmniMap_OmniMapShader reloading...");
	deinit();
	init();
}



void OmniMap_Shader::init()
{
  const char *fragmentPreludePtr = fragmentPrelude ? fragmentPrelude : "";
  //GLUniformVar * glsl_CobraWarpWithTrueDimension = NULL;
  EH_DECLARE;

  EH_Log("OmniMap_OmniMapShader loading...");
  EH_GL_ASSERT("Error");

	//Create the GLSL program
	EH_Ptr(OmniMap_ShaderCombo = new GLProgram());


  //Load the Vertex Program
  if (VertexShaderFilename != NULL && FragmentShaderFilename != NULL)
	{
    EH_Ptr(OmniMap_VertexShader = new GLVertexShader(VertexShaderFilename));
	} else
	{
    std::string vertexTotal = preprocessorMacroGlobal;

    if (useMirror) {
      vertexTotal += effectMirrorShaderVert;
    } else if (CobraWarpWithTrueDimension & OMNIMAP_COBRA_RunningOnBackground) {
      vertexTotal += effectomnimap_vertexCobra;
    } else {
      vertexTotal += effectomnimap_vertex;
    }

		EH_Ptr(OmniMap_VertexShader = new GLVertexShader(NULL,vertexTotal.c_str() ));
	}

  EH_Zero(OmniMap_VertexShader->isCompiled());


	//Load the Fragment Program
  if (FragmentShaderFilename != NULL)
  {
    EH_Ptr(OmniMap_FragmentShader = new GLFragmentShader(FragmentShaderFilename, fragmentPreludePtr));
  } else
  {	
    std::string fragTotal = preprocessorMacroGlobal + std::string(fragmentPreludePtr);

    if (useMirror) {
      fragTotal += effectMirrorShaderFrag;
    } else if (CobraWarpWithTrueDimension & OMNIMAP_COBRA_RunningOnBackground) {
      fragTotal += effectomnimap_fragmentCobra;
    } else {
      fragTotal += effectomnimap_fragment;
    }

    EH_Ptr(OmniMap_FragmentShader = new GLFragmentShader(NULL, fragTotal.c_str()));
  }

  EH_Zero(OmniMap_FragmentShader->isCompiled());


	//Link the shaders up to the program
	OmniMap_ShaderCombo->addShader(OmniMap_VertexShader);
  EH_GL_ASSERT("Error");
	OmniMap_ShaderCombo->addShader(OmniMap_FragmentShader);
  EH_GL_ASSERT("Error");
	OmniMap_ShaderCombo->use();
  EH_GL_ASSERT("Error");

	/*
	EH_Ptr(_glsl_debug1 = OmniMap_ShaderCombo->getUniformVar("debug1"), "Error:\t debug1 not found in shader\n");
	*/


	if(StencilMask_enable)// if we are using the 
	{
    // i will have to fix this... but 
    // here is the idea...
    // finalMask is totally optional...
    // if we dont want it we should have a varible in omnimap we can poll to see if we should even look for it
    // only if we want it should we register it and and check for errors...	
		EH_Ptr(_glsl_StencilMask= OmniMap_ShaderCombo->getUniformVar("StencilMask"), "Error:\t StencilMask not found in shader\n");
	} 

	EH_Ptr(_glsl_StencilMaskEnabled = OmniMap_ShaderCombo->getUniformVar("stencilMaskEnabled"), "Error:\t stencilMaskEnabled not found in shader\n");



#ifndef WIN32
	EH_Ptr(glsl_channelTexture0= OmniMap_ShaderCombo->getUniformVar("channelTextures"), "Error:\tglsl_channelTextures not found in shader\n");
#else
	EH_Ptr(glsl_channelTexture0= OmniMap_ShaderCombo->getUniformVar("channelTexture0"), "Error:\tglsl_channelTexture0 not found in shader\n");
	EH_Ptr(glsl_channelTexture1= OmniMap_ShaderCombo->getUniformVar("channelTexture1"), "Error:\tglsl_channelTexture1 not found in shader\n");
//  if(CobraWarpWithTrueDimension & OMNIMAP_COBRA_RunningOnBackground) {
    EH_Ptr(glsl_channelTexture2= OmniMap_ShaderCombo->getUniformVar("channelTexture2"), "Error:\tglsl_channelTexture2 not found in shader\n");
    EH_Ptr(glsl_channelTexture3= OmniMap_ShaderCombo->getUniformVar("channelTexture3"), "Error:\tglsl_channelTexture3 not found in shader\n");
 // }
    EH_Ptr(glsl_channelTexture4= OmniMap_ShaderCombo->getUniformVar("channelTexture4"), "Error:\tglsl_channelTexture4 not found in shader\n");
    EH_Ptr(glsl_channelTexture5= OmniMap_ShaderCombo->getUniformVar("channelTexture5"), "Error:\tglsl_channelTexture5 not found in shader\n");

#endif

/*
  if(1 <= CobraWarpWithTrueDimension ){
    EH_Ptr(glsl_CobraWarpWithTrueDimension = OmniMap_ShaderCombo->getUniformVar  ("CobraWarpWithTrueDimension"), "Error:CobraWarpWithTrueDimension not found in shader\n");
    glsl_CobraWarpWithTrueDimension->set((float)CobraWarpWithTrueDimension);
  }
*/

  EH_GL_ASSERT("Error");


	OmniMap_ShaderCombo->useFixedPipeline();


  EH_Log("OmniMap_OmniMapShader done loading...");

  EH_OnError() {
    EH_Log("Error!\tOmniMap_OmniMapShader failed to load...");
    
    if (OmniMap_VertexShader && !OmniMap_VertexShader->isCompiled()) {
      EH_Log("Error:\tVertex Shader did not compile.\n");
      EH_Log(OmniMap_VertexShader->GetLog().c_str());
    }

    if(OmniMap_FragmentShader && !OmniMap_FragmentShader->isCompiled())
    {
      EH_Log("Error:\tFragment Shader did not compile.\n");
      EH_Log(OmniMap_FragmentShader->GetLog().c_str());
    }
    deinit();
    system("pause");
  }


  //if (glsl_CobraWarpWithTrueDimension) { delete glsl_CobraWarpWithTrueDimension; }
}








void OmniMap_Shader::initFirstTime()
{
  EH_DECLARE;
	EH_Test(glewInit());
	TestIfShadersAreSupported();
  EH_OnError() {}
}





bool OmniMap_Shader::TestIfShadersAreSupported()
{
  GLint num_tex_units = 0, num_tex_frags = 0;
  bool shaders = 0;
  EH_DECLARE;

#ifdef __APPLE__
  shaders = (GL_ARB_vertex_shader && GL_ARB_fragment_shader);
#else
  shaders = (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader);
#endif

  //Checking card support for multi-texturing
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &num_tex_units);
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &num_tex_frags);

  EH_Zero(shaders);
  EH_Log(" -- OpenGL Shaders Supported");
  EH_Zero(num_tex_units >= 4 && num_tex_frags >= 4);
  EH_Log("-- Card supports %d texture units and %d fragment texture units", num_tex_units, num_tex_frags);

  EH_OnError() {
    if (!shaders) {
      EH_Log("------------------------------------------------");
      EH_Log("ERROR - OpenGL Shaders Not Supported");
      EH_Log("------------------------------------------------");
    }
    if (num_tex_units < 4 || num_tex_frags < 4) {
      EH_Log("------------------------------------------------");
      EH_Log("ERROR - Card only supports %d texture units and %d fragment texture units", num_tex_units, num_tex_frags);
      EH_Log("------------------------------------------------");
    }
    return false;
  }

  return true;
}




void OmniMap_Shader::EnableShader()
{
	// todo push and pop the previous shader...
	glUseProgramObjectARB(OmniMap_ShaderCombo->getProgramHandle());
}



void OmniMap_Shader::DisableShader()
{
	// todo push and pop the previous shader...
	OmniMap_ShaderCombo->useFixedPipeline();
}



#ifdef ELUMENATI_INTERNAL
GLint OmniMap_Shader::getShaderVarID(const char *name)
{
	return OmniMap_ShaderCombo->getUniformVar(name)->getLocation();
}
#endif



void OmniMap_Shader::SetTextureIds(int numChannels, int *ids)
{
	//glsl_numChannels->set(numChannels);
#ifndef WIN32
	if (glsl_channelTexture0 != 0)
		glsl_channelTexture0->set1v(numChannels,  (const GLint * ) ids);
#else
	for (int i = 0; i < numChannels; i++)
	{
		if (i == 0 && glsl_channelTexture0 != NULL)
			glsl_channelTexture0->set((GLint)ids[0]);
		else if (i == 1 && glsl_channelTexture1 != NULL)
			glsl_channelTexture1->set((GLint)ids[1]);
		else if (i == 2 && glsl_channelTexture2 != NULL)
			glsl_channelTexture2->set((GLint)ids[2]);
		else if (i == 3 && glsl_channelTexture3 != NULL)
			glsl_channelTexture3->set((GLint)ids[3]);
		else if (i == 4 && glsl_channelTexture4 != NULL)
			glsl_channelTexture4->set((GLint)ids[4]);
		else if (i == 5 && glsl_channelTexture5 != NULL)
			glsl_channelTexture5->set((GLint)ids[5]);
	}
#endif
}




	
void OmniMap_Shader::StencilMask_Load(char *StencilMask_FileName)	
{
  BaseImage mask(StencilMask_FileName);
  EH_DECLARE;

  EH_Log("Loading stencil mask (%s).\n", StencilMask_FileName);

  if(StencilMask_TextureID>0)
  {
    StencilMask_Free();
  }

  StencilMask_enable = true;

  // load to opengl...
  // make sure to remember to dele the stuff	
  // bitmap->load(FinalMaskFileName)

  EH_Ptr(mask.bytes);

  glGenTextures(1,(GLuint *) &StencilMask_TextureID);
  glBindTexture(GL_TEXTURE_2D, StencilMask_TextureID);

  if(mask.hasAlpha){
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, mask.w,mask.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, mask.bytes);
  }else{
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, mask.w,mask.h, 0, GL_RGB, GL_UNSIGNED_BYTE, mask.bytes);
  }
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  EH_GL_ASSERT("error loading image");

  EH_OnError() {
    EH_Log("Cannot load stencil mask.\n");
    StencilMask_Free();
  }
}



void OmniMap_Shader::StencilMask_Bind(int texture_number_not_arb)
{
  // load the 
  int number = texture_number_not_arb;
  if (number > 0)
  {
    glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(number));
    glBindTexture(GL_TEXTURE_2D, StencilMask_TextureID);
/*
    if(!_glsl_StencilMask)
    {
      _glsl_StencilMask= OmniMap_ShaderCombo->getUniformVar("StencilMask");
    }
*/
    _glsl_StencilMask->set1v(1,(GLint*)&number);

    if(_glsl_StencilMaskEnabled!=NULL){
      _glsl_StencilMaskEnabled->set((GLint) 1);
    }

  } else
  {
    if(_glsl_StencilMaskEnabled!=NULL){
      _glsl_StencilMaskEnabled->set((GLint) 0);
    }
  }

  //		_glsl_StencilMask->set( (GLint) StencilMask_TextureID  );
}




void OmniMap_Shader::StencilMask_Free()
{
  // delete glid
  if(StencilMask_TextureID>0)	
    glDeleteTextures(1,(const GLuint *) &StencilMask_TextureID);
  StencilMask_TextureID=0;
  StencilMask_enable = false;
}




//	 void OmniMap_Shader::SetUpPropertyAccess()
//	{
//		LogSystem()->ReportError("OmniMap_Shader::SetUpPropertyAccess()");
//		access.Register("useFinalMask", &useFinalMask); 
		// i would like to register FinalMaskFileName... but i cant because we need to use it b4 we 
//////// something like this		access.Register("",FinalMaskFileName

//	}





