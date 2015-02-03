// OmniMap Geometry Correction Libs
// (c)2006 Elumenati

#include <string>
#include <vector>
#include <assert.h>

#include "OmniMapBase.h"
#include "privatePrimitives.h"
#include "omnimap_luatable.h"

///////////////////////////////////////////////

#include "omnitypedefs.h"

stringList  GlobablOmniMap_Log_MessageList;
stringList  GlobablOmniMap_Log_ErrorList;

String_OmniMap_Channel_Map OmniMap_GLOBAL_Channels;
String_OmniMap_Screen_Map OmniMap_GLOBAL_ScreenShapes;

std::string OmniMap_ToString(bool val)
{
	return val?"true":"false";
}
std::string OmniMap_ToString(double val)
{
	char buffer[20];
	sprintf(buffer,"%d",val);
	return buffer;
}
double OmniMap_ToDouble(bool val)
{
	return val?1:0;
}

double OmniMap_ToDouble(std::string val)
{
	float number;
	int retval = sscanf(val.c_str(),"%f",&number);
	if(retval)
		return number;
	// to do check for true false and other types...
	LogSystem()->ReportError("errror in OmniMap_ToDouble(%s)",val);
	return 0;
}

//bool OmniMap_ToBool(std::string val)
//{
  // to do check for "true" "false" etc...
//}
bool OmniMap_ToBool(double val)
{
	return (val != 0.0);
}


bool OmniMap_ispowerof2(int number)
{
  if ( number == 0 ) 
	  return false;
  while( number%2 == 0 )
  {
    number >>= 1;
  }
  return (number == 1);
}

const char * OmniMapBase::CreateMemAndCopy(const char * contents)
{
	char *out = new char[strlen(contents)+1];
    sprintf(out, "%s", contents);
	return out;
}

////////////////////////////////////////////////////////////////
// OmniMapChannelBase
////////////////////////////////////////////////////////////////
OmniMapChannelBase::OmniMapChannelBase()
{
	LogSystem()->ReportMessage("new OmniMap_Channel()");
	SetUpPropertyAccess();
}

/*
OmniMapChannelBase::OmniMapChannelBase(bool use_fbo ,int _res) {
	LogSystem()->ReportMessage("new OmniMap_Channel()");
	if(!OmniMap_ispowerof2(_res))
	{
		LogSystem()->ReportError("error:\tResolution %d needs to be a power of 2.\n",_res);
		_res = OMNIMAP_DEFAULT_RENDERCHANNEL_RESOLUTION;
	}
	SetUpPropertyAccess();
}
*/

OmniMapChannelBase::~OmniMapChannelBase() 
{
}

void OmniMapChannelBase::SetUpPropertyAccess()
{

	access.Register("HeadsUpDisplay_Quad.bl.x",&HeadsUpDisplay_Quad.bl.x);
	access.Register("HeadsUpDisplay_Quad.bl.y",&HeadsUpDisplay_Quad.bl.y);
	access.Register("HeadsUpDisplay_Quad.bl.z",&HeadsUpDisplay_Quad.bl.z);

	access.Register("HeadsUpDisplay_Quad.br.x",&HeadsUpDisplay_Quad.br.x);
	access.Register("HeadsUpDisplay_Quad.br.y",&HeadsUpDisplay_Quad.br.y);
	access.Register("HeadsUpDisplay_Quad.br.z",&HeadsUpDisplay_Quad.br.z);

	access.Register("HeadsUpDisplay_Quad.tl.x",&HeadsUpDisplay_Quad.tl.x);
	access.Register("HeadsUpDisplay_Quad.tl.y",&HeadsUpDisplay_Quad.tl.y);
	access.Register("HeadsUpDisplay_Quad.tl.z",&HeadsUpDisplay_Quad.tl.z);

	access.Register("HeadsUpDisplay_Quad.tr.x",&HeadsUpDisplay_Quad.tr.x);
	access.Register("HeadsUpDisplay_Quad.tr.y",&HeadsUpDisplay_Quad.tr.y);
	access.Register("HeadsUpDisplay_Quad.tr.z",&HeadsUpDisplay_Quad.tr.z);

	access.Register("ViewWindow_Quad.bl.x",&ViewWindow_Quad.bl.x);
	access.Register("ViewWindow_Quad.bl.y",&ViewWindow_Quad.bl.y);
	access.Register("ViewWindow_Quad.bl.z",&ViewWindow_Quad.bl.z);

	access.Register("ViewWindow_Quad.br.x",&ViewWindow_Quad.br.x);
	access.Register("ViewWindow_Quad.br.y",&ViewWindow_Quad.br.y);
	access.Register("ViewWindow_Quad.br.z",&ViewWindow_Quad.br.z);

	access.Register("ViewWindow_Quad.tl.x",&ViewWindow_Quad.tl.x);
	access.Register("ViewWindow_Quad.tl.y",&ViewWindow_Quad.tl.y);
	access.Register("ViewWindow_Quad.tl.z",&ViewWindow_Quad.tl.z);

	access.Register("ViewWindow_Quad.tr.x",&ViewWindow_Quad.tr.x);
	access.Register("ViewWindow_Quad.tr.y",&ViewWindow_Quad.tr.y);
	access.Register("ViewWindow_Quad.tr.z",&ViewWindow_Quad.tr.z);

//	access.Register("cam_info.fovY",&(cam_info.fovY));  // removed by clement
//	access.Register("cam_info.AspectRatio",&cam_info.AspectRatio);
	access.Register("cam_info.ClipNear",&cam_info.ClipNear);
	access.Register("cam_info.ClipFar",&cam_info.ClipFar);
	access.Register("cam_info.ClipLeft",&cam_info.ClipLeft);
	access.Register("cam_info.ClipRight",&cam_info.ClipRight);
	access.Register("cam_info.ClipTop",&cam_info.ClipTop);
	access.Register("cam_info.ClipBottom",&cam_info.ClipBottom);


	access.Register("Matrix_WorldView.0.0",&cam_info.Matrix_WorldView[0*4+0]);
	access.Register("Matrix_WorldView.0.1",&cam_info.Matrix_WorldView[1*4+0]);
	access.Register("Matrix_WorldView.0.2",&cam_info.Matrix_WorldView[2*4+0]);
	access.Register("Matrix_WorldView.0.3",&cam_info.Matrix_WorldView[3*4+0]);
	access.Register("Matrix_WorldView.1.0",&cam_info.Matrix_WorldView[0*4+1]);
	access.Register("Matrix_WorldView.1.1",&cam_info.Matrix_WorldView[1*4+1]);
	access.Register("Matrix_WorldView.1.2",&cam_info.Matrix_WorldView[2*4+1]);
	access.Register("Matrix_WorldView.1.3",&cam_info.Matrix_WorldView[3*4+1]);	
	access.Register("Matrix_WorldView.2.0",&cam_info.Matrix_WorldView[0*4+2]);
	access.Register("Matrix_WorldView.2.1",&cam_info.Matrix_WorldView[1*4+2]);
	access.Register("Matrix_WorldView.2.2",&cam_info.Matrix_WorldView[2*4+2]);
	access.Register("Matrix_WorldView.2.3",&cam_info.Matrix_WorldView[3*4+2]);
	access.Register("Matrix_WorldView.3.0",&cam_info.Matrix_WorldView[0*4+3]);
	access.Register("Matrix_WorldView.3.1",&cam_info.Matrix_WorldView[1*4+3]);
	access.Register("Matrix_WorldView.3.2",&cam_info.Matrix_WorldView[2*4+3]);
	access.Register("Matrix_WorldView.3.3",&cam_info.Matrix_WorldView[3*4+3]);
}



void OmniMapChannelBase::RecomputePlaneEquations()
{
	ViewWindow_Quad.QuadToEquations(UPlaneEquation,VPlaneEquation,PlaneEquation);

}

////////////////////////////////////////////////////////////////
// OmniMapScripting
////////////////////////////////////////////////////////////////
#include "omnimap_luatable.h"

void pLuaErrHandler(const char *pError);
CLua *OmniMap_GLOBAL_lua;
OmniMap_Scripting::OmniMap_Scripting(OmniMapBase *omnimap)
{

	LogSystem()->ReportMessage("new OmniMap_Scripting()");

	parent=omnimap;
	if(!OmniMap_GLOBAL_lua)
		OmniMap_GLOBAL_lua = new CLua (false);	
	OmniMap_GLOBAL_lua->SetErrorHandler(pLuaErrHandler);
	LoadFunctionTable();
}

OmniMap_Scripting::~OmniMap_Scripting()
{
	LogSystem()->ReportMessage("delete OmniMap_Scripting()");

	if(!OmniMap_GLOBAL_lua)
		delete OmniMap_GLOBAL_lua;
	OmniMap_GLOBAL_lua =0;
}
void OmniMap_Scripting::LoadFunctionTable()
{
	LoadLuaFunctionTable(OmniMap_GLOBAL_lua,(OmniMap *) parent);// defined in omnimap_luatable.h
}

bool OmniMap_Scripting::RunScript(const char *strfile)
{
	LogSystem()->ReportMessage("lua->RunScript(%s)",strfile);

	OmniMap_GLOBAL_lua->RunScript(strfile);
	// to do add error handling
	return true;
}

bool OmniMap_Scripting::RunString(const char *strChunk)
{
	LogSystem()->ReportMessage("lua->RunString(%s)",strChunk);

	OmniMap_GLOBAL_lua->RunString(strChunk);

	
	// to do add error handling
	return true;
}
bool OmniMap_Scripting::RunString(char *fmt, ...)
{
	char buffer[300];
	va_list argp;
	va_start(argp, fmt);
	vsprintf_s(buffer, fmt, argp);
	va_end(argp);
	OmniMap_GLOBAL_lua->RunString(buffer);
	// to do add error handling
	return true;
}

void OmniMapShaderBase::setShadersFilenames(
	const char * _VertexShaderFilename ,	const char * _FragmentShaderFilename )
{
	if(VertexShaderFilename)
		delete [] VertexShaderFilename;
	VertexShaderFilename=0;
	if(FragmentShaderFilename )
		delete []FragmentShaderFilename ;
	FragmentShaderFilename =0;


	VertexShaderFilename   = (char *) OmniMapBase::CreateMemAndCopy(_VertexShaderFilename);
	FragmentShaderFilename = (char *) OmniMapBase::CreateMemAndCopy(_FragmentShaderFilename);
	LogSystem()->ReportMessage("OmniMap_OmniMapShader new vertex shader: %s and pixel shader %s",_VertexShaderFilename,_FragmentShaderFilename );

}
/*
void SetShaderVariable(char *variableName, int value)
{
	glsl_variable = OmniMap_ShaderCombo->getUniformVar(variableName);
	if(!glsl_userposition )
	{
		LogSystem()->ReportError("Error:\tglsl_userposition not found in shader\n");
		error =true;
	}
}
*/

#ifdef  notdef
void OmniMap_Screen::BaseInit()
{
	LogSystem()->ReportMessage("new OmniMap_Screen:base");
	needsrecompile = true;

	access.Register("needsrecompile",&needsrecompile);
	DisplayList = glGenLists(1);
}
void OmniMap_Screen::BaseDestroy()
{
	LogSystem()->ReportMessage("delete OmniMap_Screen:base");
	glDeleteLists(DisplayList,1);
}


void OmniMap_Screen::Display()
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
#endif

OmniMap_Screen::OmniMap_Screen() //OmniMap_ScreenType _type
{

	LogSystem()->ReportMessage("OmniMap_Screen baseclass constructor Called");

	//BaseInit();
}
OmniMap_Screen::~OmniMap_Screen() 
{
	LogSystem()->ReportMessage("OmniMap_Screen baseclass destructor Called");
	//BaseDestroy();	
}


////////////////////////////////////////////////////////////////
// OmniMapBase
////////////////////////////////////////////////////////////////
OmniMapBase::OmniMapBase(int _resW, int _resH,const char* strStartUpScriptFile, const char *luaResDir)
{
	// Get the LuaSupportDir from the Windows Registry
	if (luaResDir == NULL)
	{
		HKEY hkey;
		char luaDir[1024];
		std::string luaDirStr;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Elumenati\\Elumenati OmniMap API\\OmniMapAPI", 
			0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS)
		{
	        
			DWORD dwType = REG_SZ;
			DWORD dwSize = 1024;
			RegQueryValueEx(hkey, "LuaResources", NULL, &dwType, (PBYTE)&luaDir, &dwSize);
			//std::string setLuaDir = "LuaSupportDir = \"";
			//setLuaDir.append(luaDir);
			luaSupportDir = new char[strlen(luaDir)+1];
			strcpy((char *) luaSupportDir, luaDir);
			
		}

	} 
	else
	{
		luaSupportDir = luaResDir;
	}
	StartUpScript = (char *) CreateMemAndCopy(strStartUpScriptFile);
	


	LogSystem()->ReportMessage("new Omnimap");

#ifdef _DEBUG
	LogSystem()->ReportMessage("Omnimap Debug DLL");
	LogSystem()->ReportError("Omnimap Debug DLL");
#else
	LogSystem()->ReportMessage("Omnimap Release DLL");
	LogSystem()->ReportError("Omnimap Debug DLL");
#endif		// add options for static lib in the future...



	resWidth = _resW;
	resHeight= _resH;

	displayHUD = OMNIMAP_DEFAULT_displayHUD;
	// load defaults;	
	clearcolor_r = OMNIMAP_DEFAULT_clearcolor_r;
	clearcolor_g = OMNIMAP_DEFAULT_clearcolor_g;
	clearcolor_b = OMNIMAP_DEFAULT_clearcolor_b;
	clearcolor_a = OMNIMAP_DEFAULT_clearcolor_a;


	
	ScriptingEngine = new OmniMap_Scripting(this);

	AudiencePosition.x = OMNIMAP_DEFAULT_AudiencePosition_x;
	AudiencePosition.y = OMNIMAP_DEFAULT_AudiencePosition_y;
	AudiencePosition.z = OMNIMAP_DEFAULT_AudiencePosition_z;

	ProjectorPosition.pos.x = OMNIMAP_DEFAULT_ProjectorPosition_x;
	ProjectorPosition.pos.y = OMNIMAP_DEFAULT_ProjectorPosition_y;
	ProjectorPosition.pos.z = OMNIMAP_DEFAULT_ProjectorPosition_z;

	ProjectorPosition.headsUp.x = OMNIMAP_DEFAULT_HeadsUpPosition_x;
	ProjectorPosition.headsUp.y = OMNIMAP_DEFAULT_HeadsUpPosition_y;
	ProjectorPosition.headsUp.z = OMNIMAP_DEFAULT_HeadsUpPosition_z;

	ProjectorPosition.lookAtpos.x = OMNIMAP_DEFAULT_LookAtPosition_x;
	ProjectorPosition.lookAtpos.y = OMNIMAP_DEFAULT_LookAtPosition_y;
	ProjectorPosition.lookAtpos.z = OMNIMAP_DEFAULT_LookAtPosition_z;

#ifdef NOTYET

	SetUpPropertyAccess();
	shaders = new OmniMap_Shader();


	ScriptingEngine->RunScript(StartUpScript);

	LogSystem()->ReportMessage("------------OMNIMAP LOADED----------------");

	LogSystem()->ReportMessage("width %d",		resWidth);
	LogSystem()->ReportMessage("height %d",		resHeight);
	LogSystem()->ReportMessage("displayHUD  %d",displayHUD );
	LogSystem()->ReportMessage("clear color %f %f %f %f",clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a );
	LogSystem()->ReportMessage("AudiencePosition %f %f %f\n",AudiencePosition.x,AudiencePosition.y,AudiencePosition.z);
	LogSystem()->ReportMessage("ProjectorPosition.pos %f %f %f\n",ProjectorPosition.pos.x,ProjectorPosition.pos.y,ProjectorPosition.pos.z);
	LogSystem()->ReportMessage("ProjectorPosition.headsUp %f %f %f\n",ProjectorPosition.headsUp.x,ProjectorPosition.headsUp.y,ProjectorPosition.headsUp.z);
	LogSystem()->ReportMessage("ProjectorPosition.lookAtpos %f %f %f\n",ProjectorPosition.lookAtpos.x,ProjectorPosition.lookAtpos.y,ProjectorPosition.lookAtpos.z);
	LogSystem()->ReportMessage("startupscript : %s",StartUpScript);
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
	{
		LogSystem()->ReportMessage(itr->first.c_str());
		itr->second->beginRenderToChannel();
		itr->second->endRenderToChannel();
	}
	LogSystem()->ReportMessage("------------------------------");
#endif

}

OmniMapBase::~OmniMapBase()
{
	LogSystem()->ReportMessage("delete Omnimap");
	delete shaders;			LogSystem()->ReportMessage("~Omnimap::delete shaders;");
	ClearScreenShapes();	LogSystem()->ReportMessage("~Omnimap::ClearScreenShapes();");
	Clear_Channels();		LogSystem()->ReportMessage("~Omnimap::Clear_Channels();");
	delete ScriptingEngine;	LogSystem()->ReportMessage("~Omnimap::delete ScriptingEngine;");
	delete []StartUpScript;	LogSystem()->ReportMessage("~Omnimap::delete []StartUpScript;");

}



void OmniMapBase::SetUpPropertyAccess()
{
	///////////////

	access.Register("clearcolor_r",&clearcolor_r);
	access.Register("clearcolor_g",&clearcolor_g);
	access.Register("clearcolor_b",&clearcolor_b);
	access.Register("clearcolor_a",&clearcolor_a);

	access.Register("AudiencePosition_x",&AudiencePosition.x);
	access.Register("AudiencePosition_y",&AudiencePosition.y);
	access.Register("AudiencePosition_z",&AudiencePosition.z);

	access.Register("ProjectorPosition.lookAtpos.x",&ProjectorPosition.lookAtpos.x);
	access.Register("ProjectorPosition.lookAtpos.y",&ProjectorPosition.lookAtpos.y);
	access.Register("ProjectorPosition.lookAtpos.z",&ProjectorPosition.lookAtpos.z);

	access.Register("ProjectorPosition.headsUp.x",&ProjectorPosition.headsUp.x);
	access.Register("ProjectorPosition.headsUp.y",&ProjectorPosition.headsUp.y);
	access.Register("ProjectorPosition.headsUp.z",&ProjectorPosition.headsUp.z);

	access.Register("ProjectorPosition.pos.x",&ProjectorPosition.pos.x);
	access.Register("ProjectorPosition.pos.y",&ProjectorPosition.pos.y);
	access.Register("ProjectorPosition.pos.z",&ProjectorPosition.pos.z);
	access.Register("displayHUD",&displayHUD);
	access.Register("resWidth",&resWidth);
	access.Register("resHeight",&resHeight);
	access.Register("SAVE_AFTER_EVERY_MESSAGE",&SAVE_AFTER_EVERY_MESSAGE);

}


OmniMapChannelBase *OmniMapBase::GetChannel(const char * strChannelName) // this
{
	return OmniMap_GLOBAL_Channels[strChannelName];
}

void OmniMapBase::BaseCreateChannel(const char * name)
{
	// prevent name conflict
	String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.find(name); 
	if(itr != OmniMap_GLOBAL_Channels.end()) 
	{
		LogSystem()->ReportError("Error:\t OmniMap_Channel Name Conflict %s!\n",name);
		delete itr->second;
	}
	OmniMap_GLOBAL_Channels[name] = Create_Channel(name);
}

void OmniMapBase::BaseCreateScreenShape(const char * name)
{
	// prevent name conflict
	String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.find(name);
	if(itr !=OmniMap_GLOBAL_ScreenShapes.end())
	{
		LogSystem()->ReportError("Error:\t ScreenShape Name Conflict %s!\n",name);
		delete itr->second;
	}
	OmniMap_GLOBAL_ScreenShapes[name] = Create_ScreenShape(name);
}

const char * OmniMapBase::GetChannelName(OmniMapChannelBase *channel)
{
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		if (itr->second == channel)
			return itr->first.c_str();
	
	return NULL;

}

void OmniMapBase::Clear_Channels()
{	
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		delete (*itr).second;
	OmniMap_GLOBAL_Channels.clear();
}

void OmniMapBase::ClearScreenShapes()
{
	for(String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.begin();	itr != OmniMap_GLOBAL_ScreenShapes.end();itr++)
		delete itr->second;
	OmniMap_GLOBAL_ScreenShapes.clear(); 
}

void OmniMapBase::BindAllChannelTextures()
{
	int index=0 ;
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++, index++)
	{
		itr->second->BindTexture(index);
	}
}

void OmniMapBase::UnBindAllChannelTextures()
{
	int index=0;
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++, index++)
	{
		itr->second->UnbindTexture(index);
	}
}


void OmniMapBase::RecomputePlaneEquations()
{	
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		itr->second->RecomputePlaneEquations();
}

void OmniMapBase::DrawHeadsUpDisplays()
{
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
	{
		itr->second->DrawHeadsUpDisplay();
	}
}

void OmniMapBase::DisplayScreenShapes()
{
	for(String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.begin();	itr != OmniMap_GLOBAL_ScreenShapes.end();itr++)
		itr->second->Display();
}

void OmniMapBase::GetShaderVariables(int &N_Channels, int *TextureIds, 
									  float *Plane_Eq_XYZ, 
									  float *Plane_Eq_U, 
									  float *Plane_Eq_V)
{
	N_Channels = (GLint) OmniMap_GLOBAL_Channels.size();
	int index=0;
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++, index++)
	{

		if(index >= OMNIMAP_maxChannelFaces)
		{
			LogSystem()->ReportError("Error:\t %d = index < OMNIMAP_maxChannelFaces\tMax Faces is:%d",index,OMNIMAP_maxChannelFaces);
			N_Channels = index;
			break;
		}
		TextureIds[index] =index;

		for(int i = 0; i!=4; i++)
		{
			Plane_Eq_XYZ[index*4+i] = itr->second->PlaneEquation[i];
			Plane_Eq_U[index*4+i]   = itr->second->UPlaneEquation[i];
			Plane_Eq_V[index*4+i]   = itr->second->VPlaneEquation[i];
		}
	}
}

void OmniMapBase::ForEachChannel(void fun(OmniMapChannelBase *chan))
{
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		fun(itr->second);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////OmniMap_Log
/////////////////////////////////////////////////////////////////////////////////////////
bool SaveStringList(std::string filename , stringList *List)
{

	FILE * pFile;
#if defined(USE_VC7)
    pFile = fopen(filename.c_str(),"wt");
#else
	fopen_s (&pFile, filename.c_str(),"wt");
#endif
	if (pFile!=NULL)
	{

		for ( stringListItor itr = List->begin(); itr != List->end(); itr++)
		{
			fputs (itr->c_str(),pFile);
			fputs ("\n",pFile);
		}
		fclose (pFile);
		return 1;
	}
	else
	{
		return 0;
	}
}

OmniMap_Log::OmniMap_Log()
{

	ReportMessage("Message Log Created");
	ReportMessage("OmniMap");
	ReportMessage("Last Compiled on %s %s",__DATE__,__TIME__);

	ReportError("Error Log Created");
	ReportError("OmniMap");
	ReportError("Last Compiled on %s %s",__DATE__,__TIME__);

}
OmniMap_Log::~OmniMap_Log()
{

	SaveStringList("OmnimapMessages.txt",&GlobablOmniMap_Log_MessageList);
	SaveStringList("OmnimapErrors.txt",&GlobablOmniMap_Log_ErrorList);
}





#define MaxBufferLength 10000
static char ReportMessagebuffer[MaxBufferLength];
void OmniMap_Log::ReportMessage(const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
		
	if(strlen(fmt)>MaxBufferLength/2)
		ReportMessage("warning max message length is %d",MaxBufferLength);
	

	vsprintf_s(ReportMessagebuffer, fmt, argp);
	va_end(argp);
	GlobablOmniMap_Log_MessageList.push_back(ReportMessagebuffer);
	ReportMessagebuffer[0]=0;
//		if(omnimapNet)
//			omnimapNet->PushResponse(ReportMessagebuffer);

	if(SAVE_AFTER_EVERY_MESSAGE)
		SaveStringList("OmnimapMessages.txt",&GlobablOmniMap_Log_MessageList);
}
void OmniMap_Log::ReportError(const char *fmt, ...)
{
	if(strlen(fmt)>MaxBufferLength/2)
		ReportError("warning max message length is %d",MaxBufferLength);
	va_list argp;
	va_start(argp, fmt);
	vsprintf_s(ReportMessagebuffer, fmt, argp);
	va_end(argp);
	GlobablOmniMap_Log_ErrorList.push_back(ReportMessagebuffer);
//		if(omnimapNet)
//			omnimapNet->PushErrorResponse(ReportMessagebuffer);
	////////////////////
		
	////////////////////
	ReportMessagebuffer[0]=0;
	if(SAVE_AFTER_EVERY_MESSAGE)
		SaveStringList("OmnimapErrors.txt",&GlobablOmniMap_Log_ErrorList);
}
void OmniMap_Log::clear()
{
	GlobablOmniMap_Log_MessageList.clear();
	GlobablOmniMap_Log_ErrorList.clear();
}


/// these functions 
const char * OmniMap_Scripting::GetVariableString(const char * VarName)
	{
        std::string Accessor = ("return " +std::string(VarName)).c_str();
		bool success = OmniMap_GLOBAL_lua->RunString(Accessor.c_str());
		int type = OmniMap_GLOBAL_lua->GetArgumentType(1);

	switch(type)
	{
	case ARG_TYPE_NUMBER:
        return OmniMapBase::CreateMemAndCopy(OmniMap_ToString(OmniMap_GLOBAL_lua->GetNumberArgument(1,0)).c_str());
	case ARG_TYPE_BOOL:
			return OmniMapBase::CreateMemAndCopy(OmniMap_ToString(OmniMap_GLOBAL_lua->GetBoolArgument(1,0)).c_str());
	case ARG_TYPE_STRING:
		return OmniMap_GLOBAL_lua->GetStringArgument(1,"NoGood");
		break;
	case ARG_TYPE_NIL:
	case ARG_TYPE_UNKNOWN:
	default:
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVaribleString(%s)",VarName);
		return "error in OmniMap_Scripting::GetVaribleString(...)";
		break;
	}


}	
	

double OmniMap_Scripting::GetVariableNumber(const char * VarName)
	{
        std::string Accessor = ("return " +std::string(VarName)).c_str();
		bool success = OmniMap_GLOBAL_lua->RunString(Accessor.c_str());
		int type = OmniMap_GLOBAL_lua->GetArgumentType(1);

	switch(type)
	{
	case ARG_TYPE_NUMBER:
		return OmniMap_GLOBAL_lua->GetNumberArgument(1,0);
	case ARG_TYPE_BOOL:
		return OmniMap_ToDouble(OmniMap_GLOBAL_lua->GetBoolArgument(1,0));
		break;
	case ARG_TYPE_STRING:
		
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVaribleNumber(%s) string %s was passed out",VarName,OmniMap_GLOBAL_lua->GetStringArgument(1,"GetVaribleNumberERROR"));
		assert(0 &&"error in OmniMap_Scripting::GetVaribleNumber()");
		return 0;
	case ARG_TYPE_NIL:
	case ARG_TYPE_UNKNOWN:
	default:
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVaribleNumber(%s)",VarName);
		assert(0 &&"error in OmniMap_Scripting::GetVaribleNumber()");
		return 0;
		break;
	}
	}

bool OmniMap_Scripting::GetVariableBool(const char * VarName)
	{
        std::string Accessor = ("return " +std::string(VarName)).c_str();
		bool success = OmniMap_GLOBAL_lua->RunString(Accessor.c_str());
		int type = OmniMap_GLOBAL_lua->GetArgumentType(1);

	switch(type)
	{
		case ARG_TYPE_BOOL:
		return OmniMap_GLOBAL_lua->GetBoolArgument(1,0);
		case ARG_TYPE_NUMBER:
				OmniMap_ToBool(OmniMap_GLOBAL_lua->GetNumberArgument(1,0));
		case ARG_TYPE_NIL:

			LogSystem()->ReportError("error in OmniMap_Scripting::GetVaribleBool(%s) null was passed to it.... If this is intentional explicitly set var to false",VarName);
			return false;
		
		break;
		case ARG_TYPE_STRING:
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVaribleBool(%s) string %s was passed out",VarName,OmniMap_GLOBAL_lua->GetStringArgument(1,"GetVaribleBoolERROR"));
		assert(0&&"error in OmniMap_Scripting::GetVaribleBool(...)");
		return false;
		break;
		case ARG_TYPE_UNKNOWN:
	default:
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVaribleBool(%s)",VarName);
		assert(0&&"error in OmniMap_Scripting::GetVaribleBool(...)");
		return 0;
		break;
	}
}

