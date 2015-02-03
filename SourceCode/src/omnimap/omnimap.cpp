// OmniMap Geometry Correction Libs
// (c)2006 Elumenati
//#include "Socket.h"  // need socket.cpp socket.h and the lib file
#include <process.h>
#include <string>
#include <vector>
#include "omnimap.h"
#include <assert.h>
#include <math.h>

#include "gltools/GLShader.h"

#include "gltools/RenderChannel.h"
#include "privatePrimitives.h"
// JTCHANGE: need defn. of SAVE_AFTER_EVERY_MESSAGE
#include "omnimap_luatable.h"

///////////////////////////////////////////////

#include "omnitypedefs.h"


bool do_exit =false;

bool SaveStringList(std::string filename , stringList *List);

/////////////////////////////////////////////////

#include <stdarg.h>
//#ifdef OMNIMAP_EXPORTS
///////////////////
#ifdef __OMNIMAP_BASE_H
bool OmniMap_ispowerof2(int number);
#endif
int OmniMap_getNumberFromGL_TEXTUREID_ARB(int enu);
int OmniMap_getGL_TEXTUREN_ARB_From_N(int n);
//#endif

#ifdef _MANAGED
#pragma managed(push, off)
#endif


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif



OmniMap_Log* LogSystem()
{
	static OmniMap_Log log;
	return &log;
}


void pLuaErrHandler(const char *pError)
{
	LogSystem()->ReportError("Error:\tluascript:\t%s",pError);
}


int OmniMap_getNumberFromGL_TEXTUREID_ARB(int enu)
{
	switch(enu)
	{
		case GL_TEXTURE0_ARB: return 0;break;
		case GL_TEXTURE1_ARB: return 1;break;
		case GL_TEXTURE2_ARB: return 2;break;
		case GL_TEXTURE3_ARB: return 3;break;
		case GL_TEXTURE4_ARB: return 4;break;
		case GL_TEXTURE5_ARB: return 5;break;
		case GL_TEXTURE6_ARB: return 6;break;
		case GL_TEXTURE7_ARB: return 7;break;
		case GL_TEXTURE8_ARB: return 8;break;
		case GL_TEXTURE9_ARB: return 9;break;
	}

	assert(0 && "Error! getNumberFromGL_TEXTUREID_ARB");
	return 0;

}


int OmniMap_getGL_TEXTUREN_ARB_From_N(int n)
{
	switch(n)
	{
		case 0: return GL_TEXTURE0_ARB;break;
		case 1: return GL_TEXTURE1_ARB;break;
		case 2: return GL_TEXTURE2_ARB;break;
		case 3: return GL_TEXTURE3_ARB;break;
		case 4: return GL_TEXTURE4_ARB;break;
		case 5: return GL_TEXTURE5_ARB;break;
		case 6: return GL_TEXTURE6_ARB;break;
		case 7: return GL_TEXTURE7_ARB;break;
		case 8: return GL_TEXTURE8_ARB;break;
		case 9: return GL_TEXTURE9_ARB;break;
	}
	assert(0 && "Error! getNumberFromGL_TEXTUREID_ARB");
	return GL_TEXTURE0_ARB;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////////////
OmniMap_CameraInfo::OmniMap_CameraInfo()
{
	ClipLeft	= OMNIMAP_DEFAULT_ClipLeft;
	ClipRight	= OMNIMAP_DEFAULT_ClipRight;
	ClipTop		= OMNIMAP_DEFAULT_ClipTop;
	ClipBottom  = OMNIMAP_DEFAULT_ClipBottom;
	ClipNear	= OMNIMAP_DEFAULT_ClipNear;
	ClipFar		= OMNIMAP_DEFAULT_ClipFar;

	// to do add in a matrix for the projection matrix
	// check to see if i should be inverting the worldview matrix...

	for(int i=0; i!=4;i++)
		for(int j=0; j!=4;j++)	
			Matrix_WorldView[i*4+j] = (i==j) ;
}


 OmniMap_CameraInfo::~OmniMap_CameraInfo()
{

}

 void OmniMap_CameraInfo::ApplyProjectionMatrix()// assume projection space and identity
{
	glFrustum( ClipLeft,
			   ClipRight,
		       ClipBottom,
		       ClipTop,
		       ClipNear,
		       ClipFar);
}

 void OmniMap_CameraInfo::ApplyViewMatrix() // assume world space
{
	glMultMatrixf(Matrix_WorldView);
}



/*
////////////////////////////////////////////////////////////////////////////
/////////	OmniMap_Console
////////////////////////////////////////////////////////////////////////////

void OmniMap_Console::PushLuaCommand(std::string incommand )
{
		if( parent->ScriptingEngine->RunString(incommand.c_str()))
		{
			
		}
		else
		{
			CommandAndResponseBuffer.push_front("-- Error Running >");
		}
		CommandBuffer.push_front(incommand);		
		CommandAndResponseBuffer.push_front(incommand);		
}

	void OmniMap_Console::DisplayConsole()
	{		
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0.0f, parent->resWidth, 0.0f, parent->resHeight, -10.0f, 10.0f);
		glColor4f(1.0f, 1.0f, 1.0f,1.0f);

		float horz_pos = pos_x;
		float vert_pos = pos_y;
		

		int line = 0;
		


OmniMap_FONT::printText(horz_pos,vert_pos ,(">"+ CurrentCommand).c_str());
//		freetype::printText(parent->output_font, horz_pos,vert_pos ,(">"+ CurrentCommand).c_str());
		for(stringListItor 
			itor  = CommandAndResponseBuffer.begin();
				itor != CommandAndResponseBuffer.end();
				++itor)
				{
					
					vert_pos+=OmniMap_FONT::GetH();//parent->output_font.h ;
					
					OmniMap_FONT::printText(horz_pos,vert_pos , itor->c_str());
//					freetype::printText(parent->output_font, horz_pos,vert_pos , itor->c_str());
					line++;
					if(line> display_lines)
						break;
				}

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	
	}


	

	void OmniMap_Console::SetUpPropertyAccess()
	{
		access.Register("pos_x",&pos_x);
		access.Register("pos_y",&pos_y);
		access.Register("display_lines",&display_lines);
		access.Register("display_Console",&display_Console);
//LogSystem()->ReportMessage("XXXXSetUpPropertyAccess()");
 //static bool doneonce =false;
//if(!doneonce)
//{
//	doneonce=true;
//	for(Omnimap_CPP_PropertyListItor itor = m_properties.GetHashTable()->begin(); itor != m_properties.GetHashTable()->end(); itor++)
//		LogSystem()->ReportMessage("SetUpPropertyAccess() : OmniMap_Console::%s",itor->m_key.c_str());
//}

	}
	OmniMap_Console::OmniMap_Console(OmniMap *_parent)
	{

		parent =_parent;
		pos_x= OMNIMAP_DEFAULT_Console_x;
		pos_y= OMNIMAP_DEFAULT_Console_y;
		display_lines= OMNIMAP_DEFAULT_Console_display_lines;
		display_Console=OMNIMAP_DEFAULT_Console_display_Console;
		SetUpPropertyAccess();
	}
	OmniMap_Console::~OmniMap_Console()
	{
	}


	void OmniMap_Console::RememberLastCommand()
	{
		if(CommandBuffer.size() >=1)
			CurrentCommand = CommandBuffer.front();
	}

	void OmniMap_Console::ClearBuffers()
	{
		CommandBuffer.clear();
		CommandAndResponseBuffer.clear();
	}
	void OmniMap_Console::PushResponse(std::string Response )
	{
		CommandAndResponseBuffer.push_front(Response);		
	}






*/

////////////////////////////////////////////////////////
//////////////OmniMap_Shader
///////////////////////////////////
OmniMap_Shader::OmniMap_Shader()
{
	VertexShaderFilename=0;
	FragmentShaderFilename=0;
	zero();
	initFirstTime();
	
}

OmniMap_Shader::~OmniMap_Shader()
{
	LogSystem()->ReportMessage("OmniMap_OmniMapShader unloading...");

	deinit();
	if(VertexShaderFilename)
		delete [] VertexShaderFilename;
	VertexShaderFilename=0;
	if(FragmentShaderFilename )
		delete []FragmentShaderFilename ;
	FragmentShaderFilename =0;

}



void OmniMap_Shader::reload()
{
	LogSystem()->ReportMessage("OmniMap_OmniMapShader reloading...");
	deinit();
	init();
}
void OmniMap_Shader::init()
{
	LogSystem()->ReportMessage("OmniMap_OmniMapShader loading...");
	bool error = false;
	assert(checkGLError() && "Error");

	//Create the GLSL program
	OmniMap_ShaderCombo = new GLProgram();

	//Load the Vertex Program
	OmniMap_VertexShader = new GLVertexShader(VertexShaderFilename);
	if(!OmniMap_VertexShader->isCompiled())
	{
		LogSystem()->ReportError("Error:\tVertex Shader did not compile.\n");
		LogSystem()->ReportError(OmniMap_VertexShader->GetLog().c_str());
	}
	else
	{
		LogSystem()->ReportMessage(OmniMap_VertexShader->GetLog().c_str());
	}


	//Load the Fragment Program
	OmniMap_FragmentShader = new GLFragmentShader(FragmentShaderFilename);
	if(!OmniMap_FragmentShader->isCompiled())
	{
		LogSystem()->ReportError("Error:\tFragment Shader did not compile.\n");
		LogSystem()->ReportError(OmniMap_FragmentShader->GetLog().c_str());
	}
	else
	{
		LogSystem()->ReportMessage(OmniMap_FragmentShader->GetLog().c_str());
	}




	//Link the shaders up to the program
	OmniMap_ShaderCombo->addShader(OmniMap_VertexShader);
	OmniMap_ShaderCombo->addShader(OmniMap_FragmentShader);
	OmniMap_ShaderCombo->use();




	assert(checkGLError() && "Error");
	glsl_userposition= OmniMap_ShaderCombo->getUniformVar("userposition");
	if(!glsl_userposition )
	{
		LogSystem()->ReportError("Error:\tglsl_userposition not found in shader\n");
		error =true;
	}

	glsl_channelTextures= OmniMap_ShaderCombo->getUniformVar("channelTextures");

	if(!glsl_channelTextures )
	{
		LogSystem()->ReportError("Error:\tglsl_channelTextures not found in shader\n");
		error =true;
	}
	glsl_Plane_Eq_XYZ= OmniMap_ShaderCombo->getUniformVar("Plane_Eq_XYZ");
	if(!glsl_Plane_Eq_XYZ )
	{
		LogSystem()->ReportError("Error:\tglsl_Plane_Eq_XYZ not found in shader\n");
		error =true;
	}
	glsl_Plane_Eq_U= OmniMap_ShaderCombo->getUniformVar("Plane_Eq_U");
	if(!glsl_Plane_Eq_U )
	{
		LogSystem()->ReportError("Error:\tglsl_Plane_Eq_U not found in shader\n");
		error =true;
	}
	glsl_Plane_Eq_V= OmniMap_ShaderCombo->getUniformVar("Plane_Eq_V");
	if(!glsl_Plane_Eq_V )
	{
		LogSystem()->ReportError("Error:\tglsl_Plane_Eq_V not found in shader\n");
		error =true;
	}
	assert(checkGLError() && "Error");

	if(error==true)
	{
		LogSystem()->ReportError("error");
		system("pause");
	}

	OmniMap_ShaderCombo->useFixedPipeline();
	LogSystem()->ReportMessage("OmniMap_OmniMapShader done loading...");


}
void OmniMap_Shader::deinit()
{

	OmniMap_ShaderCombo->flagProgramDeletion();
	OmniMap_VertexShader->flagShaderDeletion();
	OmniMap_FragmentShader->flagShaderDeletion();


	delete OmniMap_ShaderCombo;
	delete OmniMap_VertexShader;
	delete OmniMap_FragmentShader;

	delete glsl_Plane_Eq_XYZ;
	delete glsl_Plane_Eq_U;
	delete glsl_Plane_Eq_V;
	delete glsl_channelTextures;
	delete glsl_userposition;
	zero();
}

void OmniMap_Shader::initFirstTime()
{
	glewInit();
	TestIfShadersAreSupported();
}

bool OmniMap_Shader::TestIfShadersAreSupported()
{

	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	{
		LogSystem()->ReportMessage(" -- OpenGL Shaders Supported");

	}
	else 
	{
		LogSystem()->ReportError("------------------------------------------------");
		LogSystem()->ReportError("ERROR - OpenGL Shaders Not Supported");
		LogSystem()->ReportError("------------------------------------------------");

		return false;
	}


	//Checking card support for multi-texturing
	GLint num_tex_units, num_tex_frags;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &num_tex_units);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &num_tex_frags);
	if (num_tex_units >= 4 && num_tex_frags >= 4)
	{
		LogSystem()->ReportMessage("-- Card supports %d texture units and %d fragment texture units", num_tex_units, num_tex_frags);

	}
	else
	{
		LogSystem()->ReportError("------------------------------------------------");
		LogSystem()->ReportError("ERROR - Card only supports %d texture units and %d fragment texture units", num_tex_units, num_tex_frags);
		LogSystem()->ReportError("------------------------------------------------");
		return false;
	}

	return true;

}


void OmniMap_Shader::zero()
{
	OmniMap_ShaderCombo=0;
	OmniMap_VertexShader=0;
	OmniMap_FragmentShader=0;
	glsl_Plane_Eq_XYZ=0;
	glsl_Plane_Eq_U=0;
	glsl_Plane_Eq_V=0;
	glsl_channelTextures=0;
	glsl_userposition=0;
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

void OmniMap_Shader::SetTextureIds(int numChannels, int *ids)
{
	//glsl_numChannels->set(numChannels);
	glsl_channelTextures->set1v(numChannels, ids);
}
	

void OmniMap_Shader::SetPlaneEquations(int numChannels, float *plane_Eq_XYZ,
	float *plane_Eq_U, float *plane_Eq_V)
{
	glsl_Plane_Eq_XYZ->set4v(numChannels,plane_Eq_XYZ);
	glsl_Plane_Eq_U->set4v(numChannels,plane_Eq_U);
	glsl_Plane_Eq_V->set4v(numChannels,plane_Eq_V);
}
	

void OmniMap_Shader::SetUserPosition(OmniVec3 position)
{
	glsl_userposition->set((float)position.x,(float)position.y,(float)position.z);
}
//////////////////////////////////////////////////////////////////////
//////////////
///////////////////////////////////////////////////////////////


void OmniMap_Screen_Dome::BaseInit()
{
	LogSystem()->ReportMessage("new OmniMap_Screen:base");
	needsrecompile = true;

	access.Register("needsrecompile",&needsrecompile);
	DisplayList = glGenLists(1);
}
void OmniMap_Screen_Dome::BaseDestroy()
{
	LogSystem()->ReportMessage("delete OmniMap_Screen:base");
	glDeleteLists(DisplayList,1);
}


void OmniMap_Screen_Dome::Display()
{
	if(needsrecompile)
	{
		needsrecompile =false;
		glNewList(DisplayList,GL_COMPILE_AND_EXECUTE);
		Render();
		glEndList();
	}
	else
	{
		glCallList(DisplayList);
	}

}

/*
OmniMap_Screen_Dome::OmniMap_Screen_() //OmniMap_ScreenType _type
{

	LogSystem()->ReportMessage("OmniMap_Screen baseclass constructor Called");

	BaseInit();
}
OmniMap_Screen_Dome::~OmniMap_Screen() 
{
	LogSystem()->ReportMessage("OmniMap_Screen baseclass destructor Called");
	BaseDestroy();	
}
*/

///////////////////////////////////////////////////////////
////////
////////////////////////////////////////////////////////////
#include "gltools/simpledome.h"	
void OmniMap_Screen_Dome::Render()
{
		dome->display();
}

void OmniMap_Screen_Dome::SetUpPropertyAccess()
{
	LogSystem()->ReportMessage("derived OmniMap_Screen_Dome::SetUpPropertyAccess()");

	access.Register("Dome_Radius",&dome->Radius);
	access.Register("Dome_tesselation",&dome->tesselation);
	access.Register("Dome_Center.x",&dome->Center.x);
	access.Register("Dome_Center.y",&dome->Center.y);
	access.Register("Dome_Center.z",&dome->Center.z);
	access.Register("Dome_ApexDirection.x",&dome->ApexDirection.x);
	access.Register("Dome_ApexDirection.y",&dome->ApexDirection.y);
	access.Register("Dome_ApexDirection.z",&dome->ApexDirection.z);
}

OmniMap_Screen_Dome::OmniMap_Screen_Dome()
{
	BaseInit();
	dome = new displayHyperbola();
	SetUpPropertyAccess();
	LogSystem()->ReportMessage("new OmniMap_Screen:OmniMap_Screen_Dome");


}
OmniMap_Screen_Dome::~OmniMap_Screen_Dome()
{
	BaseDestroy();
	LogSystem()->ReportMessage("delete OmniMap_Screen:OmniMap_Screen_Dome");

	delete dome;
}


	///////////////////////////////////////////////////////////
	////////OmniMap_Screen_Quad
	////////////////////////////////////////////////////////////
/*


		void OmniMap_Screen_Quad::Render()
	{
		quad->RenderQuadTesselated(quad_tesselation,quad_tesselation);
	}	
		void OmniMap_Screen_Quad::SetUpPropertyAccess()
	{
		//printf("derived SetUpPropertyAccess()\n");
		Register("quad_bl.x",&quad->bl.x);
		Register("quad_tl.x",&quad->tl.x);
		Register("quad_tr.x",&quad->tr.x);
		Register("quad_br.x",&quad->br.x);
		Register("quad_tesselation",&quad_tesselation);
	}

	OmniMap_Screen_Quad::OmniMap_Screen_Quad()
	{
		quad_tesselation = OMNIMAP_DEFAULT_QUADLIST_TESSELATION;
		quad= new OmniQuad;
		SetUpPropertyAccess();
		LogSystem()->ReportMessage("new OmniMap_Screen:OmniMap_Screen_Quad");
	}
	OmniMap_Screen_Quad::~OmniMap_Screen_Quad()
	{
		delete quad;
		LogSystem()->ReportMessage("delete OmniMap_Screen:OmniMap_Screen_Quad");
	}
*/
///////////////////////////////////////////////////////////
////////OmniMap_Channel
////////////////////////////////////////////////////////////

OmniMap_Channel::OmniMap_Channel( )
{
	SetUpPropertyAccess();
	//prc_RenderChannel = new RenderChannel(_res,use_fbo	);//OMNIMAP_DEFAULT_RENDERCHANNEL_HORIZONTAL_FOV, OMNIMAP_DEFAULT_RENDERCHANNEL_VERTICAL_FOV, OMNIMAP_DEFAULT_RENDERCHANNEL_NEAR_CLIP, OMNIMAP_DEFAULT_RENDERCHANNEL_FAR_CLIP,

	ConsolidateCameraIntoProjectionMatrix = OMNIMAP_DEFAULT_ConsolidateCameraIntoProjectionMatrix;

	// register this varible... this should be placed into a better function
	// like a derived SetUpPropertyAccess_Additional()
	access.Register("ConsolidateCameraIntoProjectionMatrix",&ConsolidateCameraIntoProjectionMatrix);

}

OmniMap_Channel::~OmniMap_Channel()
{
	LogSystem()->ReportMessage("delete OmniMap_Channel()");

	delete prc_RenderChannel;
}

void OmniMap_Channel::Initialize()
{
    prc_RenderChannel = new RenderChannel(resolution,TRUE);
}

void OmniMap_Channel::SetUpPropertyAccess()
{
	access.Register("resolution",&resolution);
	OmniMapChannelBase::SetUpPropertyAccess();
}

void OmniMap_Channel::beginRenderToChannel()
{

	prc_RenderChannel->beginRenderToChannel();

//assert(checkGLError() &&"checkGLError() ");
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);// should be computed in world space or object space
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);// Not eye space because eye space is not consistent across channels

    PushSetupMatricies();

}
void OmniMap_Channel::endRenderToChannel()
{

	PopSetupMatricies();
	glPopAttrib();
	prc_RenderChannel->endRenderToChannel();

}



void OmniMap_Channel::PushSetupMatricies()
{

		glMatrixMode(GL_PROJECTION);		// Set GL_PROJECTION matrix
		glPushMatrix();
		glLoadIdentity();
		cam_info.ApplyProjectionMatrix();
		if(ConsolidateCameraIntoProjectionMatrix)
			cam_info.ApplyViewMatrix();

		glMatrixMode(GL_MODELVIEW);		// Reset GL_MODELVIEW matrix
		glPushMatrix();
		glLoadIdentity();
		if(!ConsolidateCameraIntoProjectionMatrix)
			cam_info.ApplyViewMatrix();

}
void OmniMap_Channel::PopSetupMatricies()
{

		glMatrixMode(GL_PROJECTION);		// Reset projection matrix
		glPopMatrix();						
		glMatrixMode(GL_MODELVIEW);			// Reset GL_MODELVIEW matrix
		glPopMatrix();

}

	///////////////////////////////////////////////////////////
	////////
	////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
////////
////////////////////////////////////////////////////////////





void OmniMap::SetUpPropertyAccess()
{
	///////////////
	access.Register("glViewportsettings0",&glViewportsettings0);
	access.Register("glViewportsettings1",&glViewportsettings1);
	access.Register("glViewportsettings2",&glViewportsettings2);
	access.Register("glViewportsettings3",&glViewportsettings3); 
	access.Register("GL_STATE_CLEAR_AT_STARTFRAME",&GL_STATE_CLEAR_AT_STARTFRAME); 
	access.Register("GL_STATE_glDisable_GL_DEPTH_TEST",&GL_STATE_glDisable_GL_DEPTH_TEST); 
	access.Register("GL_STATE_glDepthMask_FALSE",&GL_STATE_glDepthMask_FALSE); 
	OmniMapBase::SetUpPropertyAccess();

}


OmniMap::OmniMap(int _resW, int _resH,const char* strStartUpScriptFile, const char *luaResDir) :
	OmniMapBase(_resW, _resH, strStartUpScriptFile, luaResDir)
{

	//p = new pString_OmniMap_Channel_Map();

	glViewportsettings0 = OMNIMAP_DEFAULT_glViewportsettings0;
	glViewportsettings1 = OMNIMAP_DEFAULT_glViewportsettings1;
	glViewportsettings2 = OMNIMAP_DEFAULT_glViewportsettings2;
	glViewportsettings3 = OMNIMAP_DEFAULT_glViewportsettings3;
	GL_STATE_CLEAR_AT_STARTFRAME = OMNIMAP_DEFAULT_GL_STATE_CLEAR_AT_STARTFRAME;
	GL_STATE_glDisable_GL_DEPTH_TEST =OMNIMAP_DEFAULT_GL_STATE_glDisable_GL_DEPTH_TEST;
	GL_STATE_glDepthMask_FALSE = OMNIMAP_DEFAULT_GL_STATE_glDepthMask_FALSE;

	SetUpPropertyAccess();
	glGetError();
	//fprintf(stderr, "glGetError:%x\n", glGetError());
	
//		console = new OmniMap_Console(this);
	
	
	std::string setLuaDir = "LuaSupportDir = \"";
	setLuaDir.append(luaSupportDir);
	setLuaDir.append("\"");
	int index = 0;
	while ((index = setLuaDir.find("\\", index)) != std::string::npos)
	{
		setLuaDir.replace(index, 1, "/");
		index++;
	}
	std::string vertexShaderFilename = luaSupportDir;
	vertexShaderFilename.append(OMNIMAP_DEFAULT_SHADER_VERTEXSHADER);
	std::string fragmentShaderFilename = luaSupportDir;
	fragmentShaderFilename.append(OMNIMAP_DEFAULT_SHADER_FRAGMENTSHADER);
	
	
	ScriptingEngine->RunString(setLuaDir.c_str());
	ScriptingEngine->RunScript(StartUpScript);
//		output_font.textures = 0;
//		output_font.init(strFont.c_str(), font_size);
//		OmniMap_FONT::ZeroTexture();
//		OmniMap_FONT::init(strFont.c_str(), font_size);
	// load fonts now
	shaders = new OmniMap_Shader();
	shaders->setShadersFilenames(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
	shaders->init();

	LogSystem()->ReportMessage("------------OMNIMAP LOADED----------------");
	LogSystem()->ReportMessage("glViewportsettings %f %f %f %f",glViewportsettings0,glViewportsettings1,glViewportsettings2,glViewportsettings3);
	LogSystem()->ReportMessage("GL_STATE_CLEAR_AT_STARTFRAME, GL_STATE_glDisable_GL_DEPTH_TEST,GL_STATE_glDepthMask_FALSE %d %d %d",	GL_STATE_CLEAR_AT_STARTFRAME, 
		GL_STATE_glDisable_GL_DEPTH_TEST,GL_STATE_glDepthMask_FALSE );
	LogSystem()->ReportMessage("width %d",		resWidth);
	LogSystem()->ReportMessage("height %d",		resHeight);
	//LogSystem()->ReportMessage("fontname %s",strFont);
	//LogSystem()->ReportMessage("fontsize %f",font_size);
	LogSystem()->ReportMessage("displayHUD  %d",displayHUD );
	LogSystem()->ReportMessage("clear color %f %f %f %f",clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a );
	LogSystem()->ReportMessage("AudiencePosition %f %f %f\n",AudiencePosition.x,AudiencePosition.y,AudiencePosition.z);
	LogSystem()->ReportMessage("ProjectorPosition.pos %f %f %f\n",ProjectorPosition.pos.x,ProjectorPosition.pos.y,ProjectorPosition.pos.z);
	LogSystem()->ReportMessage("ProjectorPosition.headsUp %f %f %f\n",ProjectorPosition.headsUp.x,ProjectorPosition.headsUp.y,ProjectorPosition.headsUp.z);
	LogSystem()->ReportMessage("ProjectorPosition.lookAtpos %f %f %f\n",ProjectorPosition.lookAtpos.x,ProjectorPosition.lookAtpos.y,ProjectorPosition.lookAtpos.z);
	LogSystem()->ReportMessage("startupscript : %s",StartUpScript);
#ifndef __OMNIMAP_BASE_H
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
	{
		LogSystem()->ReportMessage(itr->first.c_str());
		itr->second->beginRenderToChannel();
		itr->second->endRenderToChannel();
	}
#endif
	LogSystem()->ReportMessage("------------------------------");

}


void OmniMap::SetupScriptingCommands()
{


}
OmniMap::~OmniMap()
{
}


OmniMapChannelBase *OmniMap::Create_Channel(const char * name) 
{
	OmniMap_Channel *channel = new OmniMap_Channel();
	return(channel);
}




OmniMap_Screen *OmniMap::Create_ScreenShape(const char * name)
{
	OmniMap_Screen_Dome *p = new OmniMap_Screen_Dome();
	return(p);
}

#ifdef OmniMap_QUAD_UPGRADE
void OmniMap::Create_ScreenShapeQuad(std::string name)
{
	// prevent name conflict
	String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.find(name);
	if(itr !=OmniMap_GLOBAL_ScreenShapes.end())
	{
		LogSystem()->ReportError("Error:\t ScreenShape Name Conflict %s!\n",name.c_str());
		delete itr->second;
	}

	OmniMap_GLOBAL_ScreenShapes[name]= new OmniMap_Screen_Quad(); 
}
#endif

void OmniMap::SetupAllShaderVaribles()
{

	GLint N_Channels;
	GLint TextureIds[OMNIMAP_maxChannelFaces];
	float Plane_Eq_XYZ[OMNIMAP_maxChannelFaces*4];
	float Plane_Eq_U[OMNIMAP_maxChannelFaces*4];
	float Plane_Eq_V[OMNIMAP_maxChannelFaces*4];

	GetShaderVariables(N_Channels, TextureIds, Plane_Eq_XYZ, Plane_Eq_U, Plane_Eq_V);
	
	((OmniMap_Shader *) shaders)->SetTextureIds(N_Channels,TextureIds);
	((OmniMap_Shader *) shaders)->SetPlaneEquations(N_Channels, Plane_Eq_XYZ, Plane_Eq_U, Plane_Eq_V);
	((OmniMap_Shader *) shaders)->SetUserPosition(AudiencePosition);

}

void OmniMap::DrawHeadsUpDisplay()
{
	glDisable(GL_DEPTH_TEST);
	DrawHeadsUpDisplays();
	glEnable(GL_DEPTH_TEST);
}


void OmniMap_Channel::BindTexture(int index)
{
	glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(index));
	prc_RenderChannel->bindTexture();
}


void OmniMap_Channel::UnbindTexture(int index)
{
	glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(index));
	prc_RenderChannel->unbindTexture();
}


void OmniMap_Channel::DrawHeadsUpDisplay()
{
	
	prc_RenderChannel->bindTexture();
	//HeadsUpDisplay_Quad.RenderQuadUV();
	glBegin(GL_QUADS);
		glTexCoord2f(0 ,0);
		glVertex3d(HeadsUpDisplay_Quad.bl.x,HeadsUpDisplay_Quad.bl.y,HeadsUpDisplay_Quad.bl.z);
		glTexCoord2f(0 ,1);
		glVertex3d(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y,HeadsUpDisplay_Quad.tl.z);
		glTexCoord2f(1 ,1);
		glVertex3d(HeadsUpDisplay_Quad.tr.x,HeadsUpDisplay_Quad.tr.y,HeadsUpDisplay_Quad.tr.z);
		glTexCoord2f(1 ,0);
		glVertex3d(HeadsUpDisplay_Quad.br.x,HeadsUpDisplay_Quad.br.y,HeadsUpDisplay_Quad.br.z);
	glEnd();
	prc_RenderChannel->unbindTexture();

	glColor4f(0,0,0,1);
	glBegin(GL_LINE_STRIP);
	glVertex3d(HeadsUpDisplay_Quad.bl.x,HeadsUpDisplay_Quad.bl.y,HeadsUpDisplay_Quad.bl.z);
	glVertex3d(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y,HeadsUpDisplay_Quad.tl.z);
	glVertex3d(HeadsUpDisplay_Quad.tr.x,HeadsUpDisplay_Quad.tr.y,HeadsUpDisplay_Quad.tr.z);
	glVertex3d(HeadsUpDisplay_Quad.br.x,HeadsUpDisplay_Quad.br.y,HeadsUpDisplay_Quad.br.z);
	glEnd();
	glColor4f(1,1,1,1);

}


void OmniMap::LoadAndPush_HUD_Matricies()
{
		glViewport(	0,
				0,
				resWidth,/// aspectratio
				resHeight );
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1,1,-1,1,-1,1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();	
		assert(checkGLError() &&"checkGLError() ");	
}

void OmniMap::LoadAndPush_ProjectiveTexturing_Matricies()
{
	
	assert(checkGLError() &&"checkGLError() ");
	// Load Matricies
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if (glViewportsettings2 <1.0f)
		glViewportsettings2= (float)this->resWidth;
	if(glViewportsettings3<1.0f)
		glViewportsettings3 =(float) this->resHeight;
		

	glViewport((int)	glViewportsettings0,
		(int)glViewportsettings1,
		(int)glViewportsettings2,
		(int)glViewportsettings3);
	glOrtho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
	
	assert(checkGLError() &&"checkGLError() ");

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	ProjectorPosition.doGluLookAt();
}

void OmniMap::PopMatricies()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


void OmniMap::Clear()
{
	glClearColor(clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 
}
void OmniMap::PostRender()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR); 
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR); 

	// Post Render does a few things
	// A0 Clear
	// B1 set the projective shader
	// B2 Set up Shaders 
	// B3 textures
	// B4 Set up matricies
	// C1 Display Screen
	// D0 disable shaders
	// D1 Disable Textures
	// D2 Set active texutre to 0

	// E0 PushAndSetupMatrix
	// E1 draw lua console
	// E2 draw heads up display
	// E7 PopMatrix
	//  8 

	// A. 0 Clear
	if(GL_STATE_CLEAR_AT_STARTFRAME)
		Clear();

	// B.1 set the projective shader
	shaders->EnableShader();

	// B.2  set the textures
	BindAllChannelTextures();

	// B.3 Shader Parameters
	SetupAllShaderVaribles();

	// B.4 Set up matricies
	LoadAndPush_ProjectiveTexturing_Matricies();

	if(GL_STATE_glDisable_GL_DEPTH_TEST)
		glDisable(GL_DEPTH_TEST);
	if(GL_STATE_glDepthMask_FALSE)
		glDepthMask(FALSE);
	// C.1 Display World

	DisplayScreenShapes();

	// Pop Matricies
	PopMatricies();

	// D0 disable shaders
	shaders->DisableShader(); // use fixed pipeline

	// D1 Disable Textures
	UnBindAllChannelTextures();


	// D2 Set ARB0
	// this might be wrong
	glActiveTextureARB(OmniMap_getGL_TEXTUREN_ARB_From_N(0));

	// E0 PushAndSetupMatrix
	LoadAndPush_HUD_Matricies();

	// E1 draw lua console
		//		if(console->display_Console)
		//		  console->DisplayConsole();
	// E2 Draw HUD

	if(displayHUD)
		DrawHeadsUpDisplay();

	if(GL_STATE_glDisable_GL_DEPTH_TEST)
		glEnable(GL_DEPTH_TEST);
	if(GL_STATE_glDepthMask_FALSE)
		glDepthMask(TRUE);
	// E7 PopMatrix
	PopMatricies();
	assert(checkGLError() &&"checkGLError() ");
	glPopAttrib();
}



