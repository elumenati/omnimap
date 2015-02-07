// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#pragma unmanaged
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>

#include "OmniMapBase.h"
#include "privatePrimitives.h"
#include "omnimap_luatable.h"
#include "OM_DomeScreen.h"
#include "OM_CylinderScreen.h"
#include "OM_ToroidScreen.h"
#include "OM_PartialDomeScreen.h"
#include "OM_PlaneScreen.h"

#include "OM_ConeScreen.h"

#include "ElumSphere.h"
#include "png/lodepng.h"

#if defined(USE_VC7)
#define sprintf_s sprintf
#define vsprintf_s vsprintf
#endif
#ifdef __APPLE__
#include <stdarg.h>
#include "CoreFoundation/CoreFoundation.h"
#endif
#ifdef WIN32
#include <windows.h>
#endif

///////////////////////////////////////////////

#include "omnitypedefs.h"

#include "EasyBMP.h"
#include "logo-solo.h"

#define MyPI 3.14159265

OmniMapScreenRendererFactory OmniMapBase::ScreenRendererFactory;

stringList  GlobablOmniMap_Log_MessageList;
stringList  GlobablOmniMap_Log_ErrorList;

String_OmniMap_Channel_Map OmniMap_GLOBAL_Channels;
String_OmniMap_Screen_Map OmniMap_GLOBAL_ScreenShapes;

OMNIMAP_API OM_Protector om_protector;
OM_ProtectorImpl_Cobra cobtest;


std::string preprocessorMacroGlobal("");



#ifdef _WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


/////// spout ////////////////////
#include "../omnimap/spout/OmniSpout.h"
OmniSpout *omniSpout =0;
bool useSpoutInRenderChannels = false;
bool useSpoutInFinalPassCamera = false;
///////////////////////////////////

std::string GetResourcesDirectory_toString(OmniMapBase *b, char const *suggestion)
{
	std::string resPath = "OmniMap/Resources/"; // this should be relative
	// get current directory long format...
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
	     sprintf(cCurrentPath,"./");
    }

	LogSystem()->ReportMessage("WARNING We should convert the relative directory path to an absoulte directory path");

	if(suggestion)
	{
		LogSystem()->ReportMessage("Using host applications suggested resource directory (%s)",suggestion);
		resPath = std::string(suggestion);
	}



	LogSystem()->ReportMessage("Current Path(%s)",cCurrentPath);
	LogSystem()->ReportMessage("Resource Path(%s)",resPath.c_str());
	return resPath;
}




#ifdef WIN32
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
#endif

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
	LogSystem()->ReportError("errror in OmniMap_ToDouble(%s)",val.c_str());
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

OmniMap_Log* LogSystem()
{
	static OmniMap_Log log;
	return &log;
}

const char * OmniMapBase::CreateMemAndCopy(const char * contents)
{
	char *out = new char[strlen(contents)+1];
    sprintf(out, "%s", contents);
	return out;
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

////////////////////////////////////////////////////////////////
// OmniMapChannelBase
////////////////////////////////////////////////////////////////
OmniMapChannelBase::OmniMapChannelBase()
{
	LogSystem()->ReportMessage("new OmniMapChannelBase::OmniMapChannelBase()");
	SetUpPropertyAccess();
	xResolution =2048;
	yResolution =2048;
}

OmniMapChannelBase::~OmniMapChannelBase() 
{
	LogSystem()->ReportMessage("deleting OmniMap_Channel()");
}

void OmniMapChannelBase::SetUpPropertyAccess()
{
	access.Register("RenderTarget", (int *) &renderTarget);

    access.Register("UseOmniMapMatrix",&useOmniMapMatrix);

	access.Register("XResolution", &xResolution);
	access.Register("YResolution", &yResolution);

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


void OmniMapChannelBase::SetEulerAngles(float x, float y, float z)
{
	float cx = cosf(x);
	float cy = cosf(y);
	float cz = cosf(z);
	float sx = sinf(x);
	float sy = sinf(y);
	float sz = sinf(z);
	
	cam_info.Matrix_WorldView[0] = cy * cz;
	cam_info.Matrix_WorldView[1] = cx * sz + sx * sy * cz;
	cam_info.Matrix_WorldView[2] = sx * sz - cx * sy * cz;
	cam_info.Matrix_WorldView[4] = -cy * sz;
	cam_info.Matrix_WorldView[5] = cx * cz - sx * sy * sz;
	cam_info.Matrix_WorldView[6] = sx * cz + cx * sy * sz;
	cam_info.Matrix_WorldView[8] = sy;
	cam_info.Matrix_WorldView[9] = -sx * cy;
	cam_info.Matrix_WorldView[10] = cx * cy;
}


void OmniMapChannelBase::GetEulerAngles(float *x, float *y, float *z)
{
	float sy = cam_info.Matrix_WorldView[8];
	if (sy < 1.0F)
	{
		if (sy > -1.0F)
		{
			*x = -atan2f(cam_info.Matrix_WorldView[9], cam_info.Matrix_WorldView[10]);
			*y = asinf(sy);
			*z = -atan2f(cam_info.Matrix_WorldView[4], cam_info.Matrix_WorldView[0]);
		}
		else
		{
			*x = 0.0F;
			*y = (float) -3.14159265/2.0;
			*z = atan2f(cam_info.Matrix_WorldView[1], cam_info.Matrix_WorldView[5]);
		}
	}
	else
	{
		*x = 0.0F;
		*y = (float) 3.14159265/2.0;
		*z = atan2f(cam_info.Matrix_WorldView[1], cam_info.Matrix_WorldView[5]);
	}
}

void OmniMapChannelBase::SetCameraInfo(float fovUp, float fovDown, float fovLeft, float fovRight,
								float nearv, float farv)
{
	cam_info.ClipTop = tanf( fovUp * 3.14159f/360.0f * 2.0f) * nearv;
	cam_info.ClipBottom = -tanf( fovDown * 3.14159f/360.0f * 2.0f) * nearv;
	cam_info.ClipLeft = -tanf(fovLeft * 3.14159f/360.0f * 2.0f) * nearv;
	cam_info.ClipRight = tanf(fovRight * 3.14159f/360.0f * 2.0f) * nearv;
	cam_info.ClipNear = nearv;
	cam_info.ClipFar = farv;
}

void OmniMapChannelBase::SetCameraInfo(float fovy ,
								float apectRatio, float nearv, float farv)
{
	cam_info.ClipTop = tanf( fovy * 3.14159f/360.0f) * nearv; 
	cam_info.ClipBottom = -cam_info.ClipTop;
	cam_info.ClipLeft   = apectRatio*cam_info.ClipBottom;
	cam_info.ClipRight  = apectRatio*cam_info.ClipTop;
	cam_info.ClipNear = nearv;
	cam_info.ClipFar = farv;

}

void OmniMapChannelBase::SetChannelOrientation(OmniVec3 headsUp, OmniVec3 viewVec)
{
	OmniVec3 huXvv = headsUp.cross(viewVec);
	cam_info.Matrix_WorldView[0] = huXvv.x;
	cam_info.Matrix_WorldView[4] = huXvv.y;
	cam_info.Matrix_WorldView[8] = huXvv.z;
	
	cam_info.Matrix_WorldView[1] = headsUp.x;
	cam_info.Matrix_WorldView[5] = headsUp.y;
	cam_info.Matrix_WorldView[9] = headsUp.z;

	cam_info.Matrix_WorldView[2] = -viewVec.x;
	cam_info.Matrix_WorldView[6] = -viewVec.y;
	cam_info.Matrix_WorldView[10] = -viewVec.z;

}

void OmniMapChannelBase::GetViewMatrix(float m[16])
{
	for (int i = 0; i < 16; i++)
	{
		m[i] = cam_info.Matrix_WorldView[i];
	}

}

#ifdef OM_PICKING
bool OmniMapChannelBase::screenXYToChannelXY(float screenGeomLoc[4], int &channelX, int &channelY)
{
	float gpoint[4];
	for (int i = 0; i < 4; i++)
		gpoint[i] = screenGeomLoc[i];

	float textMat[16];
	GetProjTextMatrix(textMat);
	AffMatrix m(textMat);
	m.transformPoint(gpoint);

	float x = gpoint[0]/gpoint[3];
	float y = gpoint[1]/gpoint[3];
	channelX = (x) * (float) this->xResolution;
	channelY = ((y) * (float) this->yResolution);
	if ((x >= 0.0) && (x <= 1.0) &&
 		(y >= 0.0) && (y <= 1.0)/* && screenGeomLoc[2] > 0.0*/)
	{
		return(true);
	} 
	return(false);
}
#endif




////////////////////////////////////////////////////////////////
// OmniMapScripting
////////////////////////////////////////////////////////////////
#include "omnimap_luatable.h"

void pLuaErrHandler(const char *pError)
{
	LogSystem()->ReportError("Error:\tluascript:\t%s",pError);
}

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
	//LogSystem()->ReportMessage("delete OmniMap_Scripting()");

	if(OmniMap_GLOBAL_lua)
		delete OmniMap_GLOBAL_lua;
	OmniMap_GLOBAL_lua =0;
	
}
void OmniMap_Scripting::LoadFunctionTable()
{
	LoadLuaFunctionTable(OmniMap_GLOBAL_lua,(OmniMapBase *) parent);// defined in omnimap_luatable.h
}

bool OmniMap_Scripting::RunScript(const char *strfile)
{
	LogSystem()->ReportMessage("lua->RunScript(%s)",strfile);

	if (OmniMap_GLOBAL_lua->RunScript(strfile) == false)
	{
		return false;
	}
	
	return true;
}

bool OmniMap_Scripting::RunString(const char *strChunk)
{
	LogSystem()->ReportMessage("lua->RunString(%s)",strChunk);

	if (!OmniMap_GLOBAL_lua->RunString(strChunk))
	{
		return false;
	}

	return true;
}
bool OmniMap_Scripting::RunString(char *fmt, ...)
{
	char buffer[300];
	va_list argp;
	va_start(argp, fmt);
#ifdef WIN32
	vsprintf_s(buffer, fmt, argp);
#else
    sprintf(buffer, fmt, argp);
#endif
	va_end(argp);
	OmniMap_GLOBAL_lua->RunString(buffer);
	// to do add error handling
	return true;
}

OmniMapShaderBase::OmniMapShaderBase()  
{
		vertexPrelude = NULL;
		fragmentPrelude = NULL;
		CobraWarpWithTrueDimension =0;
}

void OmniMapShaderBase::setShadersPrelude(const char *vertPrelude, const char *fragPrelude)
{
	if (fragPrelude != NULL) {
		fragmentPrelude = new char[strlen(fragPrelude)+1];
		strcpy((char *)fragmentPrelude, fragPrelude);
	}
	if (vertPrelude != NULL) {
		vertexPrelude = new char[strlen(vertPrelude)+1];
		strcpy((char *)vertexPrelude, vertPrelude);
	}
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






OmniMap_Screen::OmniMap_Screen() //OmniMap_ScreenType _type
{

	LogSystem()->ReportMessage("OmniMap_Screen baseclass constructor Called");
	renderer = NULL;
}

OmniMap_Screen::~OmniMap_Screen() 
{
	LogSystem()->ReportMessage("OmniMap_Screen baseclass destructor Called");		
}


////////////////////////////////////////////////////////////////
// OmniMapBase
////////////////////////////////////////////////////////////////
OmniMapBase::OmniMapBase(int _resW, int _resH,const char* strStartUpScriptFile, const char *luaResDir)
{
	graphicsContext = NULL;
	StencilMask_filename = NULL;
	shaders = NULL;
	char *defaultConfig = NULL;
	CobraWarpWithTrueDimension=0;
	//shaderPreprocessorMacros="";
	//protector.unlock("TheElumenati","5dd87a69");
	
	LogSystem()->ReportMessage("Constructing OmnimapBase");


	std::string resourceDirectory = GetResourcesDirectory_toString(this,luaResDir);
	luaSupportDir = new char[resourceDirectory.length()+1];
	memcpy(luaSupportDir,resourceDirectory.c_str(),resourceDirectory.length()+1);
	
/*
#if defined(WIN32)
	// Get the LuaSupportDir from the Windows Registry
	
	HKEY hkey;
	char luaDir[1024];
	char defConfig[1024];
	std::string luaDirStr;
#ifdef UNICODE
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Elumenati\\Elumenati OmniMap API\\OmniMapAPI", 
#else
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Elumenati\\Elumenati OmniMap API\\OmniMapAPI", 
#endif
			0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = 1024;
	    if (luaResDir == NULL)
		{
			
#ifdef UNICODE
			RegQueryValueEx(hkey, L"LuaResources", NULL, &dwType, (PBYTE)&luaDir, &dwSize);
			DWORD i;
            luaSupportDir = new char[(dwSize/2)+1];
			for (i = 0; i < dwSize; i+=2)
			{
				luaSupportDir[i/2] = luaDir[i];
			}
			luaSupportDir[(i/2)-1] = '\0';
#else
			RegQueryValueEx(hkey, "LuaResources", NULL, &dwType, (PBYTE)&luaDir, &dwSize);
			luaSupportDir = new char[strlen(luaDir)+1];
			strcpy((char *) luaSupportDir, luaDir);
#endif		
		} else {
			if (luaResDir == NULL)
				LogSystem()->ReportError("No OmniMap Resource directory specified in constructor call or Windows Registry\n");
			else 
				luaSupportDir = (char *) luaResDir;
		}

#ifdef UNICODE
		long retval = RegQueryValueEx(hkey, L"DefaultConfig", NULL, &dwType, (PBYTE)&defConfig, &dwSize);
#else
		RegQueryValueEx(hkey, "DefaultConfig", NULL, &dwType, (PBYTE)&defConfig, &dwSize);
#endif

#ifdef UNICODE
		if (retval == ERROR_SUCCESS)
		{
			defaultConfig = new char[(dwSize/2)+1];
			int i = 0;
			for (i = 0; i < dwSize; i+=2)
			{
				defaultConfig[i/2] = defConfig[i];
			}
			defaultConfig[(i/2)-1] = '\0';
		}   
#endif

	} else {
		luaSupportDir = (char *) luaResDir;
	}
	
#elif defined(__APPLE__)
	if (luaResDir == NULL)
	{

		CFURLRef    resourceURL;
		CFStringRef bundleID =  CFSTR ("com.elumenati.omnimapapi");

	
		CFBundleRef omnimapAPIBundle = CFBundleGetBundleWithIdentifier (
			bundleID
		);
		CFURLRef bundleURL = CFBundleCopyBundleURL(omnimapAPIBundle);
		const CFStringRef bundlePath  = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
		resourceURL = CFBundleCopyResourcesDirectoryURL( omnimapAPIBundle);
		const CFStringRef relResPath = CFURLCopyFileSystemPath(resourceURL, kCFURLPOSIXPathStyle);
		CFStringRef format = CFSTR("%s/%s/");
	

		int length = CFStringGetMaximumSizeForEncoding(CFStringGetLength(bundlePath), kCFStringEncodingASCII);
		char *bundleStr = (char *) malloc(length + 1);
		Boolean success = CFStringGetCString(bundlePath, bundleStr, length + 1, kCFStringEncodingASCII);
	
		length = CFStringGetMaximumSizeForEncoding(CFStringGetLength(relResPath), kCFStringEncodingASCII);
		char *relResStr = (char *) malloc(length + 1);
		success = CFStringGetCString(relResPath, relResStr, length + 1, kCFStringEncodingASCII);

		CFStringRef fullResPath = CFStringCreateWithFormat (NULL, NULL, format, 
			bundleStr,
			relResStr);
			
		//delete bundleStr;
		//delete relResStr;
	
		length = CFStringGetMaximumSizeForEncoding(CFStringGetLength(fullResPath), kCFStringEncodingASCII);
		char *p = (char *) malloc(length + 1);
		success = CFStringGetCString(fullResPath, p, length + 1, kCFStringEncodingASCII);

		luaSupportDir = (char *) CreateMemAndCopy(p);
	} 
	else
	{
		luaSupportDir = (char *) CreateMemAndCopy(luaResDir);
	}

#else
    luaSupportDir = (char *) CreateMemAndCopy(luaResDir);
#endif


	*/
	if (strStartUpScriptFile == NULL)
	{
		strStartUpScriptFile = "omnimap_startup.lua";
		StartUpScript = (char *) malloc(strlen(luaSupportDir) + strlen(strStartUpScriptFile) + 1);
		sprintf(StartUpScript, "%s%s", luaSupportDir, strStartUpScriptFile);
	} else
	{
		StartUpScript = (char *) CreateMemAndCopy(strStartUpScriptFile);
	}
	


	LogSystem()->ReportMessage("new Omnimap");

#ifdef _DEBUG
	LogSystem()->ReportMessage("Omnimap Debug DLL");
	LogSystem()->ReportError("Omnimap Debug DLL");
#else
	LogSystem()->ReportMessage("Omnimap Release DLL");
	LogSystem()->ReportError("Omnimap Release DLL");
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
	std::string s("Screenshape = ");
	if (defaultConfig != NULL)
	{
		s.append("\"");
		s.append(defaultConfig);
		s.append("\"");
		ScriptingEngine->RunString(s.c_str());	
	}
	

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


	ScreenshapeFactory.addScreenShapeCreator("cone", &OM_ConeScreen::CreateOMCone);

	ScreenshapeFactory.addScreenShapeCreator("dome", &OM_DomeScreen::CreateOMDome);
	ScreenshapeFactory.addScreenShapeCreator("partial_dome", &OM_PartialDomeScreen::CreateOMPartialDome);
	ScreenshapeFactory.addScreenShapeCreator("cylinder", &OM_CylinderScreen::CreateOMCylinder);
	ScreenshapeFactory.addScreenShapeCreator("toroid", &OM_ToroidScreen::CreateOMToroidScreen);
	ScreenshapeFactory.addScreenShapeCreator("sphere", &SphereShape::CreateElumSphere);
	ScreenshapeFactory.addScreenShapeCreator("plane", &OM_PlaneScreen::CreateOMPlane);
	


}

OmniMapBase::~OmniMapBase()
{

	//LogSystem()->ReportMessage("delete Omnimap");
	//LogSystem()->ReportMessage("Printing out final values for omnimap"); access.LogAllVariblesAndValues();

	if (shaders != NULL)
	{
		//LogSystem()->ReportMessage("Printing out final values for shaders");
		//shaders->access.LogAllVariblesAndValues();
		delete shaders;			
		//LogSystem()->ReportMessage("~Omnimap::delete shaders;");
	}

	ClearScreenShapes();	//LogSystem()->ReportMessage("~Omnimap::ClearScreenShapes();");   // log all vars inside

	Clear_Channels();		//LogSystem()->ReportMessage("~Omnimap::Clear_Channels();");		// log all vars inside

	delete ScriptingEngine;	//LogSystem()->ReportMessage("~Omnimap::delete ScriptingEngine;"); // no string accessor 

	delete []StartUpScript;	//LogSystem()->ReportMessage("~Omnimap::delete []StartUpScript;"); // no string accessor 

	if(StencilMask_filename) {	delete [] StencilMask_filename; StencilMask_filename = 0;}

}

void OmniMapBase::SetChannelEulerAngles(OmniMapChannelBase *channel, float x, float y, float z)
{
	channel->SetEulerAngles(x, y, z);
}

void OmniMapBase::SetChannelOrientation(OmniMapChannelBase *channel, OmniVec3 headsUp, OmniVec3 viewVec)
{
	channel->SetChannelOrientation(headsUp, viewVec);
}

void OmniMapBase::SetCameraInfo(OmniMapChannelBase *channel, float fovUp, float fovDown, float fovLeft, float fovRight,
								float nearv, float farv)
{
	channel->SetCameraInfo(fovUp, fovDown, fovLeft, fovRight, nearv, farv);
}


void OmniMapBase::SetCameraInfo(OmniMapChannelBase *channel, float fovy ,
								float aspectRatio, float nearv, float farv)
{
	channel->SetCameraInfo(fovy, aspectRatio, nearv, farv);
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

	
	access.Register("RendererType", &rendererType[0]);

	

	/// cobra
	access.Register("CobraWarpWithTrueDimension", &CobraWarpWithTrueDimension);
	
	access.Register("useSpoutInRenderChannels",&useSpoutInRenderChannels);
	access.Register("useSpoutInFinalPassCamera",&useSpoutInFinalPassCamera);
}

CLua *OmniMapBase::GetCLuaRef()
{
	return OmniMap_GLOBAL_lua;
}

OmniMapChannelBase *OmniMapBase::GetChannel(int index)
{
	int i =0;
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++, i++)
	{
		if(i == index)
			return itr->second;
	}
	return 0;
}


OmniMapChannelBase *OmniMapBase::GetChannel(const char * strChannelName) // this
{
	String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.find(std::string(strChannelName));
	if(itr == OmniMap_GLOBAL_Channels.end()) 
		return NULL;
	else
		return itr->second;
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




	void OmniMapBase::create_ScreenShape(const char * name, const char * type)
	{
		// prevent name conflict
		String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.find(name);
		if(itr != OmniMap_GLOBAL_ScreenShapes.end())
		{
			LogSystem()->ReportError("Error:\t ScreenShape Name Conflict %s!\n",name);
			delete itr->second;
		}
		OmniMap_GLOBAL_ScreenShapes[name] = ScreenshapeFactory.Create_ScreenShape(type?type:"dome");
		OmniMap_GLOBAL_ScreenShapes[name]->SetRenderer(
			ScreenRendererFactory.Create_ScreenRenderer(rendererType));
	}

	OmniMap_Screen * OmniMapBase::GetScreenShape(const char * name)
	{
		String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.find(name);
		if(itr == OmniMap_GLOBAL_ScreenShapes.end())
		{
			LogSystem()->ReportError("Error:\t could not find screenshape %s!\n",name);
			return 0;
		}
		return OmniMap_GLOBAL_ScreenShapes[name];
	}


	OmniMap_Screen * OmniMapBase::GetScreenShape(int index)
	{
		int i =0;
		for(String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.begin(); itr!= OmniMap_GLOBAL_ScreenShapes.end();itr++, i++)
		{
			if(i == index)
				return itr->second;
		}
		return 0;
	}



void OmniMapBase::BaseCreateScreenShape(const char *name, const char *type)
{
	String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.find(name);
	if(itr !=OmniMap_GLOBAL_ScreenShapes.end())
	{
		LogSystem()->ReportError("Error:\t ScreenShape Name Conflict %s!\n",name);
		delete itr->second;
	}
	OmniMap_GLOBAL_ScreenShapes[name] = ScreenshapeFactory.Create_ScreenShape(type);
	OmniMap_Screen *s = OmniMap_GLOBAL_ScreenShapes[name];
	OmniMap_GLOBAL_ScreenShapes[name]->SetRenderer(
		ScreenRendererFactory.Create_ScreenRenderer(rendererType));
}

#ifdef ELUMENATI_INTERNAL
const char *OmniMapBase::getScreenName(OmniMap_Screen *screen)
{
	for(String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.begin(); itr != OmniMap_GLOBAL_ScreenShapes.end();itr++)
	{
		if (itr->second == screen)
			return itr->first.c_str();
	}
	
	return NULL;

}
#endif


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
	{
		LogSystem()->ReportMessage("Printing out final values for channel %s",itr->first.c_str());
		itr->second->access.LogAllVariblesAndValues();
		delete itr->second;
	}
	OmniMap_GLOBAL_Channels.clear();
}

void OmniMapBase::ClearChannelMap()
{
	OmniMap_GLOBAL_Channels.clear();
}

void OmniMapBase::ClearScreenShapes()
{
	for(String_OmniMap_Screen_Map_itor itr = OmniMap_GLOBAL_ScreenShapes.begin();	itr != OmniMap_GLOBAL_ScreenShapes.end();itr++)
	{
		LogSystem()->ReportMessage("Printing out final values for screenshape %s",itr->first.c_str());
		itr->second->access.LogAllVariblesAndValues();
		delete itr->second;
	}
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

#ifdef OLD_SHADER
void OmniMapBase::RecomputePlaneEquations()
{	
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		itr->second->RecomputePlaneEquations();
}
#endif

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
		itr->second->Display(graphicsContext);
}

void OmniMapBase::GetShaderVariables(int &N_Channels, int *TextureIds)
{
	N_Channels =  (int) OmniMap_GLOBAL_Channels.size();
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
	}
}

int OmniMapBase::GetNumScreenShapes()
{
	return (int) OmniMap_GLOBAL_ScreenShapes.size();
}

int OmniMapBase::GetNumChannels()
{
	return (int) OmniMap_GLOBAL_Channels.size();
}

void OmniMapBase::ForEachChannel(void fun(OmniMapChannelBase *chan, void *userData), void *userData)
{
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		fun(itr->second, userData);
}

void OmniMapBase::ForEachChannel(void fun(OmniMapChannelBase *chan))
{
	for(String_OmniMap_Channel_Map_itor itr = OmniMap_GLOBAL_Channels.begin();	itr != OmniMap_GLOBAL_Channels.end();itr++)
		fun(itr->second);
}

/*
bool OmniMapBase::XYZToLensXYZ(float xyz[3], float xyzLens[3])
{
}
*/
#ifdef OM_PICKING
#include "OM_DomeScreen.h"
#define Saturate(x) ((x)>=0?((x)>1?1:(x)):(0))
bool OmniMapBase::screenXYToChannelXY(OmniMapChannelBase * channel, 
									  int x, int y, int &channelX, int &channelY)
{
	
	float fx = (float) x;
	float fy = (float) y;
	int w = glViewportsettings2/2;
	int h = (glViewportsettings3)/2;

	float nx = ((fx - this->glViewportsettings0) - w)/w;
/*
	float ny = (((glViewportsettings3) - fy)-h)/h;

	float screenHt = glViewportsettings1 + glViewportsettings3;
	float ny = y - glViewportsettings1;
	ny = ny/glViewportsettings3;
	ny = (ny *2.0)-1.0;
*/
	float y1 = this->resHeight - y;
	y1 = y1 - glViewportsettings1;
	float ny = y1/glViewportsettings3;
	ny = (ny *2.0)-1.0;
	

	OM_DomeScreen *screen = (OM_DomeScreen *) this->GetScreenShape(0);
	

	float screenGeomLoc[4];
	bool onSurface = screen->xyTo3D(nx, ny, screenGeomLoc);
	if (!onSurface)
		return false;
	screenGeomLoc[3] = 1.0;
/*
	screenGeomLoc[2] = cos((MyPI/2.0) * sqrt((nx*nx) + (ny*ny)));
	screenGeomLoc[1] = (ny * sqrt(1.0f - screenGeomLoc[2])) / sqrt((nx*nx) + (ny*ny));
	screenGeomLoc[0] = (nx * sqrt(1.0f - screenGeomLoc[2])) / sqrt((nx*nx) + (ny*ny));
	screenGeomLoc[2] = -screenGeomLoc[2];
*/	
	return(channel->screenXYToChannelXY(screenGeomLoc, channelX, channelY));
}

bool OmniMapBase::screenXYToChannelXY(int x, int y, OmniMapChannelBase * &channel, int &channelX, int &channelY)
{
	bool ret = false;
	for (int i = 0; i < GetNumChannels(); i++)
	{
		OmniMapChannelBase *chan = GetChannel(i);
		const char * name = this->GetChannelName(chan);
		ret = screenXYToChannelXY(chan, x, y, channelX, channelY);
		if (ret) {
			channel  = chan;
			break;
		}
	}
	return ret;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////OmniMap_Log
/////////////////////////////////////////////////////////////////////////////////////////
bool CreateEmptyFile(std::string filename )
{
	FILE * pFile;
#ifdef _WINDOWS
#if defined(USE_VC7)
    pFile = fopen(filename.c_str(),"wt");
#else
	fopen_s (&pFile, filename.c_str(),"wt");
#endif
#else
     pFile = fopen(filename.c_str(),"wt");
#endif
	if (pFile!=NULL)
	{
		fclose (pFile);
		return 1;
	}
	else
	{
		return 0;
	}
}

bool AppendStringToFile(std::string filename , char *text)
{

	FILE * pFile;
#if defined(USE_VC7)  || !defined(WIN32)
    pFile = fopen(filename.c_str(),"at+");
#else
	fopen_s (&pFile, filename.c_str(),"at+");
#endif
	if (pFile!=NULL)
	{
		fputs (text,pFile);
		fputs ("\n",pFile);
		fclose (pFile);
		return 1;
	}
	else
	{
		return 0;
	}
}



bool SaveStringList(std::string filename , stringList *List)
{

	FILE * pFile;
#if defined(USE_VC7) || !defined(WIN32)
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
// delete logs...

CreateEmptyFile("OmnimapMessages.txt");
CreateEmptyFile("OmnimapErrors.txt");



	ReportMessage("Message Log Created");
	ReportMessage("OmniMap");
	ReportMessage("Last Compiled on %s %s",__DATE__,__TIME__);

	ReportError("Error Log Created");
	ReportError("OmniMap");
	ReportError("Last Compiled on %s %s",__DATE__,__TIME__);

}
OmniMap_Log::~OmniMap_Log()
{

//if( ! (SAVE_AFTER_EVERY_MESSAGE))
//{
	SaveStringList("OmnimapMessages.txt",&GlobablOmniMap_Log_MessageList);
	SaveStringList("OmnimapErrors.txt",&GlobablOmniMap_Log_ErrorList);
//}
}





#define MaxBufferLength 10000
static char ReportMessagebuffer[MaxBufferLength];
void OmniMap_Log::ReportMessage(const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
		
	if(strlen(fmt)>MaxBufferLength/2)
		ReportMessage("warning max message length is %d",MaxBufferLength);
	
#if defined(WIN32)
	vsprintf_s(ReportMessagebuffer, fmt, argp);
#else
    vsprintf(ReportMessagebuffer, fmt, argp);
#endif
	va_end(argp);
	GlobablOmniMap_Log_MessageList.push_back(ReportMessagebuffer);
//		if(omnimapNet)
//			omnimapNet->PushResponse(ReportMessagebuffer);

	if(SAVE_AFTER_EVERY_MESSAGE)
	{
		AppendStringToFile("OmnimapMessages.txt",ReportMessagebuffer);
	//	SaveStringList("OmnimapMessages.txt",&GlobablOmniMap_Log_MessageList);
	}

	ReportMessagebuffer[0]=0;

}
void OmniMap_Log::ReportError(const char *fmt, ...)
{
	if(strlen(fmt)>MaxBufferLength/2)
		ReportError("warning max message length is %d",MaxBufferLength);
	va_list argp;
	
	va_start(argp, fmt);
#if defined(WIN32)
	vsprintf_s(ReportMessagebuffer, fmt, argp);
#else
    vsprintf(ReportMessagebuffer, fmt, argp);
#endif
	va_end(argp);
	GlobablOmniMap_Log_ErrorList.push_back(ReportMessagebuffer);
//		if(omnimapNet)
//			omnimapNet->PushErrorResponse(ReportMessagebuffer);
	////////////////////
		
	////////////////////
	if(SAVE_AFTER_EVERY_MESSAGE)
	{
	//	SaveStringList("OmnimapErrors.txt",&GlobablOmniMap_Log_ErrorList);

		AppendStringToFile("OmnimapErrors.txt",ReportMessagebuffer);
	}
	ReportMessagebuffer[0]=0;
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
		
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVariableNumber(%s) string %s was passed out",VarName,OmniMap_GLOBAL_lua->GetStringArgument(1,"GetVaribleNumberERROR"));
		assert(0 &&"error in OmniMap_Scripting::GetVariableNumber()");
		return 0;
	case ARG_TYPE_NIL:
	case ARG_TYPE_UNKNOWN:
	default:
		LogSystem()->ReportError("error in OmniMap_Scripting::GetVariableNumber(%s)",VarName);
		assert(0 &&"error in OmniMap_Scripting::GetVariableNumber()");
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

OMNIMAP_API  OmniMap_Log*  LogSystem();






	BaseImage::BaseImage(const char * filename)
	{
		hasAlpha =false;
		bytes =0;
		h = w = 0;
		//load file
		if(!Baseloadfile(filename))
		{
			LogSystem()->ReportError("Error loading image %s.  Returning Null Image",filename);
		}
	}; 
	BaseImage::~BaseImage(){if(bytes)delete []bytes;}; // if error... load null object...

	bool BaseImage::Baseloadfile(const char * filename)
	{
		// read the image
		BMP Input;
		//SetEasyBMPwarningsOff();
		if(Input.ReadFromFile( filename ))
		{
		//	SetEasyBMPwarningsON();
			h = Input.TellHeight();
			w =  Input.TellWidth();

			if(bytes)
				delete []bytes;
			bytes =  new unsigned char [Input.TellHeight()*Input.TellWidth() *3];
			hasAlpha =false;

			for( int j=0 ; j < Input.TellHeight() ; j++)
			{
				for( int i=0 ; i < Input.TellWidth() ; i++)
				{
					bytes[(j*Input.TellWidth()+i)*3+0] = Input(i,j)->Red ;
					bytes[(j*Input.TellWidth()+i)*3+1] = Input(i,j)->Green;
					bytes[(j*Input.TellWidth()+i)*3+2] = Input(i,j)->Blue ;
				}
			}
			return true;
		}
		
		 unsigned error;
		 unsigned char* image;
		unsigned width, height;
		  error = lodepng_decode32_file(&image, &width, &height, filename);
		
		if(!error)
		{
			h = height;
			w = width;
			if(bytes)
				delete []bytes;
			bytes =  new unsigned char [w*h*4];
			hasAlpha =true;

			memcpy(bytes,image,w*h*4);
			free(image);
			return true;
		}else{
		  LogSystem()->ReportError("error loading image (%s) = %s",filename, lodepng_error_text(error));
		  loadNullObject();
		  free(image);
		  return false;
		}

	}
	void BaseImage::loadNullObject() // the null object is a big red X
	{
		if(bytes)
			delete []bytes;

		h = w = 100;
		bytes= new unsigned char[h*w*3];


		for(int i =0; i!= h; i++)
			for(int j =0; j!= w; j++)
			for(int c =0; c!= 3; c++)
			{
			
				if((abs(i-j)<=2||abs(i-(w-j-1))<=2)&&c==0)
					bytes[(i*w+j)*3+c] = 255;
				else
				{
					bytes[(i*w+j)*3+c] = 60;

				}
			}		
	}





//void OmniMapShaderBase::SetUpPropertyAccess()
//	{
//		LogSystem()->ReportError("OmniMapShaderBase::SetUpPropertyAccess()");
		// nothing to register!
//	}


void OmniMapBase::StencilMask_filename_set(const char * FileName) // relative to lua support directory...
{
		if(StencilMask_filename) {	
			delete [] StencilMask_filename; 
			StencilMask_filename = 0;
		}

		std::string pathtest(FileName);
		if (pathtest.find(std::string(":")) != std::string::npos) { //fully qualified path
			StencilMask_filename = new char[strlen(FileName)+3];
			sprintf(StencilMask_filename,"%s",FileName);  
		}else{ // append lua support directory...
			StencilMask_filename = new char[strlen(FileName)+strlen(luaSupportDir)+3];
			sprintf(StencilMask_filename,"%s%s",luaSupportDir,FileName); 
		}
		// to do... make sure file exists... 
}



