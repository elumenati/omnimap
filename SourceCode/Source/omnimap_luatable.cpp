// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#include "OmniMapBase.h"
#include "omnimap_luatable.h"


#include "omnitypedefs.h"
#include "assert.h"
#include "OM_ErrorHandling.h"
#pragma warning (disable:4706) // warning C4706: assignment within conditional expression


extern String_OmniMap_Screen_Map OmniMap_GLOBAL_ScreenShapes;
extern String_OmniMap_Channel_Map OmniMap_GLOBAL_Channels;
bool SAVE_AFTER_EVERY_MESSAGE =true;


OmniMapBase * OmnimapReference = 0;
CLua *LuaReference = 0;
extern std::string preprocessorMacroGlobal;






#define EH_Ptr_ASSERT(RET, ...) \
  assert((RET)&&__VA_ARGS__); \
  EH_Ptr(RET, __VA_ARGS__);

#define EH_Zero_ASSERT(RET, ...) \
  assert((RET)&&__VA_ARGS__); \
  EH_Zero(RET, __VA_ARGS__);



std::string GetArgumentAsString(int n)
{
	switch(LuaReference->GetArgumentType(n))
	{
	case ARG_TYPE_NIL:
		return "";
	case ARG_TYPE_NUMBER:
		{
			static char buffer[50];
			sprintf(buffer,"%f",LuaReference->GetNumberArgument(n,0));
			return std::string(buffer);
		}
	case ARG_TYPE_STRING:
		return LuaReference->GetStringArgument(n,"");
	case ARG_TYPE_BOOL:
		if(LuaReference->GetBoolArgument(n,true))
			return "true";
		else
			return "false";
	case ARG_TYPE_UNKNOWN:
		return "";
	}
	return "";
}





LuaGlue(_ConsolePrintString)
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "");
	EH_Log(LuaReference->GetStringArgument(1,""));
  EH_OnError() {}
	return 0;
}



LuaGlue( _ClearScreenShapes)
{
  L;
  OmnimapReference->ClearScreenShapes();
	return 0;
}




LuaGlue( _Clear_Channels)
{
  L;
  OmnimapReference->Clear_Channels();
	return 0;
}





LuaGlue( _Create_Channel)
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "");
	EH_Log("LUAGLUE:Create_Channel");
	OmnimapReference->BaseCreateChannel(LuaReference->GetStringArgument(1,""));
  EH_OnError() {}
  return 0;
}




#ifdef SCREEN_FACTORY
LuaGlue( _Create_ScreenShapeDome)
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "");
	OmnimapReference->create_ScreenShape(LuaReference->GetStringArgument(1,""),"dome");
  EH_OnError() {}
  return 0;
}
#else
LuaGlue( _Create_ScreenShapeDome)
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "");
	OmnimapReference->BaseCreateScreenShape(LuaReference->GetStringArgument(1,""), "sphere");
  EH_OnError() {}
  return 0;
}
#endif




LuaGlue( _Create_ScreenShape) // takes two arguments  name and type  for example "dome"
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "");
	OmnimapReference->create_ScreenShape(LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,"dome")); 
  EH_OnError() {}
  return 0;
}





#ifdef OLD_SHADER
LuaGlue( _RecomputePlaneEquations)
{
  L;
  OmnimapReference->RecomputePlaneEquations();
	return 0;
}
#endif




LuaGlue( _SetScreenParameter) // takes a map key and a varible key and a value
{
  L;
  EH_DECLARE;

  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "error: Map key void");
  EH_Zero_ASSERT(LuaReference->GetStringArgument(2,"")[0], "error: Varible key void");
//	assert(	GetArgumentAsString(3)[0] && "error: Value void");

	// there are a few errors that can happen that are not accounted for here...
	// to do 
	// add more errror checking...
	// for example if the ->access doesnt have the varible registered...
  EH_Zero(GetArgumentAsString(2)[0], "Screen parameter is void for [%s]->",LuaReference->GetStringArgument(1,""));
  EH_Zero(GetArgumentAsString(3)[0], "Screen parameter value is void for [%s]->[%s]",LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,""));
  EH_Zero(OmniMap_GLOBAL_ScreenShapes[LuaReference->GetStringArgument(1,"")], "Screen Name not found [%s]",LuaReference->GetStringArgument(1,""));

  EH_Log("Screen:%s %s %s\n",LuaReference->GetStringArgument(1,""),  LuaReference->GetStringArgument(2,""), LuaReference->GetStringArgument(3,""));
	//OmniMap_Screen *screen = OmniMap_GLOBAL_ScreenShapes[LuaReference->GetStringArgument(1,"")];
	
	OmniMap_GLOBAL_ScreenShapes[LuaReference->GetStringArgument(1,"")]->access.SetValue(LuaReference->GetStringArgument(2,""), GetArgumentAsString(3).c_str());

  EH_OnError() {}

	return 0;
}



LuaGlue( _SetChannelParameter) // takes a map key and a varible key and a value
{
  L;
  OmniMapChannelBase *channel = NULL;
  EH_DECLARE;

  //LogSystem()->ReportError("SetChannelParameter:%s %s %s\n", LuaReference->GetStringArgument(1,""),
	//LuaReference->GetStringArgument(2,""), GetArgumentAsString(3).c_str());	
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "error: Map key void");
  EH_Zero_ASSERT(LuaReference->GetStringArgument(2,"")[0], "error: Varible key void");
  EH_Zero_ASSERT(GetArgumentAsString(3)[0], "error: Value void");

	
	EH_Ptr(channel = OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Render Channel not found [%s]",LuaReference->GetStringArgument(1,""));
	channel->access.SetValue(LuaReference->GetStringArgument(2,""), GetArgumentAsString(3).c_str());

	//LuaReference->GetStringArgument(3,"")
//	LogSystem()->ReportError("SetChannelParameter:%s %s\n", LuaReference->GetStringArgument(2,""));

  EH_OnError() {}

	return 0;
}




LuaGlue(_InitializeChannel)
{
  L;
  OmniMapChannelBase *channel = NULL;
  EH_DECLARE;

  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "error: Channel key void");
	EH_Log("LUAGLUE:InitializeChannel[%s]",LuaReference->GetStringArgument(1,""));
	EH_Ptr(channel = OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Render Channel not found [%s]",LuaReference->GetStringArgument(1,""));

  channel->Initialize();

  EH_OnError() {}

  return(0);
}




LuaGlue(_SetOmnimapParameter)
{
  L;
//	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
//	assert(	LuaReference->GetStringArgument(2,"")[0] && "error: Value void");
//	LogSystem()->ReportError("Omnimap:%s %s\n", LuaReference->GetStringArgument(1,""),GetArgumentAsString(2).c_str());
	OmnimapReference->access.SetValue(LuaReference->GetStringArgument(1,""),GetArgumentAsString(2).c_str());
	return 0;
}



LuaGlue(_GetOmnimapParameter)
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "error: Map key void");
	LuaReference->PushString(OmnimapReference->access.GetValue(LuaReference->GetStringArgument(1,"")));
  EH_OnError() {}
  return 1;
}




LuaGlue(_GetOmnimapNumberParameter)
{
  L;
  EH_DECLARE;
  EH_Zero_ASSERT(LuaReference->GetStringArgument(1,"")[0], "error: Map key void");
	LuaReference->PushNumber(  OmnimapReference->access.GetNumber(LuaReference->GetStringArgument(1,"")));
  EH_OnError() {}
  return 1;
}




/*
LuaGlue(_SetOmnimapConsoleParameter)
{
  L;
  assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
	assert(	LuaReference->GetStringArgument(2,"")[0] && "error: Value void");
	OmnimapReference->console->SetValue(LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,""));

	return 0;
}
*/




LuaGlue( _system)
{
  L;
	system( LuaReference->GetStringArgument(1,"") );
	return 0;
}




LuaGlue( _Clear)
{
  L;
  OmnimapReference->Clear();
	return 0;
}





void myprint(std::string p)
{
  EH_DECLARE;
//	if(OmnimapReference->network->started)
//		OmnimapReference->network->PushResponse(p);
	EH_Log(p.c_str());
}




LuaGlue( Debug_Print)
{
  L;
	int n = (int)LuaReference->GetNumberArgument(0,0);
	for(int i = 1; i<=n ;i++)
	{
		std::string s = LuaReference->GetStringArgument(i,"");
		if(! s.length())
			break;
	    if (i>2)
			myprint("\t");
	    myprint(s);
	}
	myprint("\n");
	return 0;
}




///////////////////////////////////////////////
////  GLUE FOR PER CHANNEL DATA...
///////////////////////////////////////////////



LuaGlue( _CreateChannelData_BOOL)// (ChannelKEY,VarKey) // takes a map key and a varible key
{
	//error checking 
  L;
  EH_DECLARE;
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create channel data: %s",LuaReference->GetStringArgument(1,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddBOOL(LuaReference->GetStringArgument(2,""));
  EH_OnError() {}
	return 0;
}


LuaGlue( _SetChannelData_BOOL)// (ChannelKEY,VarKey,Value) // takes a map key and a varible key and a value
{
  L;
  EH_DECLARE;
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));	
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")), 
    "Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)", LuaReference->GetStringArgument(1,""), LuaReference->GetStringArgument(2,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->Set(LuaReference->GetBoolArgument(3,false));
  EH_OnError() {}
  return 0;
}



LuaGlue( _CreateChannelData_INT)// (ChannelKEY,VarKey) // takes a map key and a varible key
{
  L;
  EH_DECLARE;
	//error checking 
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Channel data: %s",LuaReference->GetStringArgument(1,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddINT(LuaReference->GetStringArgument(2,""));
  EH_OnError() {}
  return 0;
}



LuaGlue( _SetChannelData_INT)// (ChannelKEY,VarKey,Value) // takes a map key and a varible key and a value
{
  L;
  EH_DECLARE;
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));	
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")), 
    "Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)", LuaReference->GetStringArgument(1,""), LuaReference->GetStringArgument(2,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->Set((int)LuaReference->GetNumberArgument(3,0));
  EH_OnError() {}
  return 0;
}




LuaGlue( _CreateChannelData_FLOAT)// (ChannelKEY,VarKey) // takes a map key and a varible key
{
  L;
  EH_DECLARE;
	//error checking 
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Channel data: %s",LuaReference->GetStringArgument(1,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddFLOAT(LuaReference->GetStringArgument(2,""));
  EH_OnError() {}
  return 0;
}




LuaGlue( _SetChannelData_FLOAT) // (ChannelKEY,VarKey,Value) // takes a map key and a varible key and a value
{
  L;
  EH_DECLARE;
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));	
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")), 
    "Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)", LuaReference->GetStringArgument(1,""), LuaReference->GetStringArgument(2,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->Set((float)LuaReference->GetNumberArgument(3,0.0f));
  EH_OnError() {}
  return 0;
}




LuaGlue( _CreateChannelData_STRING)// (ChannelKEY,VarKey)  // takes a map key and a varible key
{
  L;
  EH_DECLARE;
	//error checking 
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Channel data: %s",LuaReference->GetStringArgument(1,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddSTRING(LuaReference->GetStringArgument(2,""));
  EH_OnError() {}
  return 0;
}



LuaGlue( _SetChannelData_STRING)// (ChannelKEY,VarKey,Value)  // takes a map key and a varible key and a value
{	
  L;
  EH_DECLARE;
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")], "Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));	
  EH_Zero(OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")), 
    "Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)", LuaReference->GetStringArgument(1,""), LuaReference->GetStringArgument(2,""));	
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->
		ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->
			Set(LuaReference->GetStringArgument(3,"MissingValue"));
  EH_OnError() {}
  return 0;
}




LuaGlue(_StencilMask_filename_set)
{
  L;
	if(OmnimapReference)
		OmnimapReference->StencilMask_filename_set(LuaReference->GetStringArgument(1,""));
	return 0;
}




LuaGlue(_Unlock) // Please Call Unlock from lua with the two codes
{
  L;
  EH_DECLARE;
  EH_Log("Omnimap unlocked.  Thank you for supporting The Elumenati.");
  return 0;
}







LuaGlue( ClearShaderPreprocessorMacro)
{
  L;
  preprocessorMacroGlobal = std::string("");
	return 0;
}



LuaGlue( SetShaderPreprocessorMacro)
{
  L;
	const char* string = LuaReference->GetStringArgument(1,"");
	preprocessorMacroGlobal =  preprocessorMacroGlobal + "\r\n"+ std::string(string );
	return 0;
}





void LoadLuaFunctionTable(CLua *l,OmniMapBase * _pointertoomnimap)
{
  EH_DECLARE;

	EH_Ptr_ASSERT(OmnimapReference = (OmniMapBase *) _pointertoomnimap, "Error NULL _pointertoomnimap");
	EH_Ptr_ASSERT(LuaReference = l, "Error NULL _lua");

  EH_Zero(l->AddFunction("ClearScreenShapes",_ClearScreenShapes));
  EH_Zero(l->AddFunction("Create_ScreenShapeDome",_Create_ScreenShapeDome));
  EH_Zero(l->AddFunction("Create_ScreenShape",_Create_ScreenShape));

  EH_Zero(l->AddFunction("SetShaderPreprocessorMacro",SetShaderPreprocessorMacro));
  EH_Zero(l->AddFunction("ClearShaderPreprocessorMacro",ClearShaderPreprocessorMacro));

  EH_Zero(l->AddFunction("Unlock",_Unlock));// THIS FUNCTION IS FOR REMOVING THE WATERMARK

#ifdef OmniMap_QUAD_UPGRADE
  EH_Zero(l->AddFunction("Create_ScreenShapeQuad",_Create_ScreenShapeQuad));
#endif
  EH_Zero(l->AddFunction("Clear_Channels",_Clear_Channels));
  EH_Zero(l->AddFunction("Create_Channel",_Create_Channel));
#ifdef OLD_SHADER
  EH_Zero(l->AddFunction("RecomputePlaneEquations",_RecomputePlaneEquations));
#endif
  EH_Zero(l->AddFunction("SetScreenParameter",_SetScreenParameter));
  EH_Zero(l->AddFunction("SetChannelParameter",_SetChannelParameter));
  EH_Zero(l->AddFunction("SetOmnimapParameter",_SetOmnimapParameter));
  EH_Zero(l->AddFunction("GetOmnimapParameter",_GetOmnimapParameter));
  EH_Zero(l->AddFunction("GetOmnimapNumberParameter",_GetOmnimapNumberParameter));
  EH_Zero(l->AddFunction("InitializeChannel", _InitializeChannel));
  // console
  EH_Zero(l->AddFunction("ConsolePrintString"			,_ConsolePrintString));
  EH_Zero(l->AddFunction("system"	,_system));
  EH_Zero(l->AddFunction("Clear"	,_Clear));
  EH_Zero(l->AddFunction("print"	,Debug_Print));
  EH_Zero(l->AddFunction("Print"	,Debug_Print));
  ///////////////////////////////////////////////
  ////  GLUE FOR PER CHANNEL DATA...
  ///////////////////////////////////////////////


  EH_Zero(l->AddFunction("CreateChannelData_BOOL",_CreateChannelData_BOOL));
  EH_Zero(l->AddFunction("SetChannelData_BOOL",_SetChannelData_BOOL));

  EH_Zero(l->AddFunction("CreateChannelData_INT",_CreateChannelData_INT));
  EH_Zero(l->AddFunction("SetChannelData_INT",_SetChannelData_INT));

  EH_Zero(l->AddFunction("CreateChannelData_FLOAT",_CreateChannelData_FLOAT));
  EH_Zero(l->AddFunction("SetChannelData_FLOAT",_SetChannelData_FLOAT));
  EH_Zero(l->AddFunction("CreateChannelData_STRING",_CreateChannelData_STRING));
  EH_Zero(l->AddFunction("SetChannelData_STRING",_SetChannelData_STRING));

  EH_Zero(l->AddFunction("StencilMask_filename_set",_StencilMask_filename_set));

  EH_OnError() {}
}
