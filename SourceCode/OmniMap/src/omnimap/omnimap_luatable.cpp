// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati

#include "OmniMapBase.h"
#include "omnimap_luatable.h"


#include "omnitypedefs.h"
#include "assert.h"

extern String_OmniMap_Screen_Map OmniMap_GLOBAL_ScreenShapes;
extern String_OmniMap_Channel_Map OmniMap_GLOBAL_Channels;
bool SAVE_AFTER_EVERY_MESSAGE =true;


OmniMapBase * OmnimapReference = 0;

CLua *LuaReference = 0;

extern std::string preprocessorMacroGlobal;


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
	assert(LuaReference->GetStringArgument(1,"")[0]);
	LogSystem()->ReportMessage(LuaReference->GetStringArgument(1,""));
	return 0;
}

LuaGlue( _ClearScreenShapes)
{
	OmnimapReference->ClearScreenShapes();
	return 0;
}

LuaGlue( _Clear_Channels)
{
	OmnimapReference->Clear_Channels();
	return 0;
}

LuaGlue( _Create_Channel)
{
	assert(	LuaReference->GetStringArgument(1,"")[0] );

	LogSystem()->ReportMessage("LUAGLUE:Create_Channel");
	OmnimapReference->BaseCreateChannel(LuaReference->GetStringArgument(1,""));
	return 0;
}

#ifdef SCREEN_FACTORY
LuaGlue( _Create_ScreenShapeDome)
{
	assert(	LuaReference->GetStringArgument(1,"")[0] );
	OmnimapReference->create_ScreenShape(LuaReference->GetStringArgument(1,""),"dome");
	return 0;
	
}
#else
LuaGlue( _Create_ScreenShapeDome)
{
	assert(	LuaReference->GetStringArgument(1,"")[0] );
	OmnimapReference->BaseCreateScreenShape(LuaReference->GetStringArgument(1,""), "sphere");
	return 0;
	
}
#endif

LuaGlue( _Create_ScreenShape) // takes two arguments  name and type  for example "dome"
{
	assert(	LuaReference->GetStringArgument(1,"")[0] );
	OmnimapReference->create_ScreenShape(LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,"dome")); 
	return 0;
}

#ifdef OLD_SHADER
LuaGlue( _RecomputePlaneEquations)
{
	OmnimapReference->RecomputePlaneEquations();
	return 0;
}
#endif

LuaGlue( _SetScreenParameter) // takes a map key and a varible key and a value
{
	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
	assert(	LuaReference->GetStringArgument(2,"")[0] && "error: Varible key void");
//	assert(	GetArgumentAsString(3)[0] && "error: Value void");

	// there are a few errors that can happen that are not accounted for here...
	// to do 
	// add more errror checking...
	// for example if the ->access doesnt have the varible registered...
	if(!GetArgumentAsString(2)[0])
	{
		LogSystem()->ReportError("Screen parameter is void for [%s]->",LuaReference->GetStringArgument(1,""));
		return 0;
	}
	if(!GetArgumentAsString(3)[0])
	{
		LogSystem()->ReportError("Screen parameter value is void for [%s]->[%s]",LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,""));
		return 0;
	}

	if(!OmniMap_GLOBAL_ScreenShapes[LuaReference->GetStringArgument(1,"")])
	{
		LogSystem()->ReportError("Screen Name not found [%s]",LuaReference->GetStringArgument(1,""));
		return 0;
	}
	LogSystem()->ReportError("Screen:%s %s %s\n",LuaReference->GetStringArgument(1,""),  LuaReference->GetStringArgument(2,""), LuaReference->GetStringArgument(3,""));
	//OmniMap_Screen *screen = OmniMap_GLOBAL_ScreenShapes[LuaReference->GetStringArgument(1,"")];
	

	OmniMap_GLOBAL_ScreenShapes[LuaReference->GetStringArgument(1,"")]->access.SetValue(LuaReference->GetStringArgument(2,""), GetArgumentAsString(3).c_str()
		);

	return 0;
}

LuaGlue( _SetChannelParameter) // takes a map key and a varible key and a value
{
//LogSystem()->ReportError("SetChannelParameter:%s %s %s\n", LuaReference->GetStringArgument(1,""),
		//LuaReference->GetStringArgument(2,""), GetArgumentAsString(3).c_str());	
	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
	assert(	LuaReference->GetStringArgument(2,"")[0] && "error: Varible key void");
	assert(	GetArgumentAsString(3)[0] && "error: Value void");


	
	OmniMapChannelBase *channel = OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")];
	if (channel == NULL)
	{
		LogSystem()->ReportError("Render Channel not found [%s]",LuaReference->GetStringArgument(1,""));
		return 0;
	}
	channel->access.SetValue(LuaReference->GetStringArgument(2,""), GetArgumentAsString(3).c_str());

	//LuaReference->GetStringArgument(3,"")
//	LogSystem()->ReportError("SetChannelParameter:%s %s\n", LuaReference->GetStringArgument(2,""));
	return 0;
}

LuaGlue(_InitializeChannel)
{
	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Channel key void");
	LogSystem()->ReportMessage("LUAGLUE:InitializeChannel[%s]",LuaReference->GetStringArgument(1,""));
	OmniMapChannelBase *channel = OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")];
	if(channel == NULL)
	{
		LogSystem()->ReportError("Render Channel not found [%s]",LuaReference->GetStringArgument(1,""));
		return 0;
	}
	channel->Initialize();
	return(0);
}

LuaGlue(_SetOmnimapParameter)
{
//	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
//	assert(	LuaReference->GetStringArgument(2,"")[0] && "error: Value void");
//	LogSystem()->ReportError("Omnimap:%s %s\n", LuaReference->GetStringArgument(1,""),GetArgumentAsString(2).c_str());
	OmnimapReference->access.SetValue(LuaReference->GetStringArgument(1,""),GetArgumentAsString(2).c_str());
	return 0;
}

LuaGlue(_GetOmnimapParameter)
{
	
	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
	LuaReference->PushString(OmnimapReference->access.GetValue(LuaReference->GetStringArgument(1,"")));
	return 1;
}
LuaGlue(_GetOmnimapNumberParameter)
{
	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
	LuaReference->PushNumber(  OmnimapReference->access.GetNumber(LuaReference->GetStringArgument(1,"")));
	return 1;
}


/*
LuaGlue(_SetOmnimapConsoleParameter)
{
	assert(	LuaReference->GetStringArgument(1,"")[0] && "error: Map key void");
	assert(	LuaReference->GetStringArgument(2,"")[0] && "error: Value void");
	OmnimapReference->console->SetValue(LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,""));

	return 0;
}
*/


LuaGlue( _system)
{
	system( LuaReference->GetStringArgument(1,"") );
	return 0;
}
LuaGlue( _Clear)
{
	OmnimapReference->Clear();
	return 0;
}



void myprint(std::string p)
{
//	if(OmnimapReference->network->started)
//		OmnimapReference->network->PushResponse(p);
	
	
	LogSystem()->ReportMessage(p.c_str());
}
LuaGlue( Debug_Print)
{
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
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	
	{
		LogSystem()->ReportError("Channel Not Found for create channel data: %s",LuaReference->GetStringArgument(1,""));
		return 0;
	}

	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddBOOL(LuaReference->GetStringArgument(2,""));
	return 0;
}
LuaGlue( _SetChannelData_BOOL)// (ChannelKEY,VarKey,Value) // takes a map key and a varible key and a value
{
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	{LogSystem()->ReportError("Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));return 0;}
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")))
	{
		LogSystem()->ReportError("Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)",
			
			LuaReference->GetStringArgument(1,""),
			LuaReference->GetStringArgument(2,""));
		return 0;
	}
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->Set(LuaReference->GetBoolArgument(3,false));
	return 0;
}



LuaGlue( _CreateChannelData_INT)// (ChannelKEY,VarKey) // takes a map key and a varible key
{
	//error checking 
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	{LogSystem()->ReportError("Channel Not Found for create channel data: %s",LuaReference->GetStringArgument(1,""));return 0;}

	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddINT(LuaReference->GetStringArgument(2,""));
	return 0;
}
LuaGlue( _SetChannelData_INT)// (ChannelKEY,VarKey,Value) // takes a map key and a varible key and a value
{
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	{LogSystem()->ReportError("Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));return 0;}
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")))
	{
		LogSystem()->ReportError("Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)",
			
			LuaReference->GetStringArgument(1,""),
			LuaReference->GetStringArgument(2,""));
		return 0;
	}
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->Set((int)LuaReference->GetNumberArgument(3,0));
	return 0;
}

LuaGlue( _CreateChannelData_FLOAT)// (ChannelKEY,VarKey) // takes a map key and a varible key
{
	//error checking 
	if(!
		OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]
	)
	{
		LogSystem()->ReportError("Channel Not Found for create channel data: %s",LuaReference->GetStringArgument(1,""));return 0;}

	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddFLOAT(LuaReference->GetStringArgument(2,""));
	return 0;
}
LuaGlue( _SetChannelData_FLOAT) // (ChannelKEY,VarKey,Value) // takes a map key and a varible key and a value
{
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	{LogSystem()->ReportError("Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));return 0;}
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")))
	{
		LogSystem()->ReportError("Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)",
			
			LuaReference->GetStringArgument(1,""),
			LuaReference->GetStringArgument(2,""));
		return 0;
	}
	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->Set((float)LuaReference->GetNumberArgument(3,0.0f));
	return 0;
}

LuaGlue( _CreateChannelData_STRING)// (ChannelKEY,VarKey)  // takes a map key and a varible key
{
	//error checking 
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	
	{
		LogSystem()->ReportError("Channel Not Found for create channel data: %s",LuaReference->GetStringArgument(1,""));
		assert("error"&&0);
		return 0;
	}

	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.AddSTRING(LuaReference->GetStringArgument(2,""));
	return 0;
}
LuaGlue( _SetChannelData_STRING)// (ChannelKEY,VarKey,Value)  // takes a map key and a varible key and a value
{	
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")])	{LogSystem()->ReportError("Channel Not Found for create Set Channel data: %s",LuaReference->GetStringArgument(1,""));return 0;}
	if(!OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,"")))
	{
		LogSystem()->ReportError("Varible Key Not Found for _SetChannelData_: OmniMap_GLOBAL_Channels[%s]->ChannelMetaData.IndexDataMap(%s)",
			
			LuaReference->GetStringArgument(1,""),
			LuaReference->GetStringArgument(2,""));
		return 0;
	}

	OmniMap_GLOBAL_Channels[LuaReference->GetStringArgument(1,"")]->
		ChannelMetaData.IndexDataMap(LuaReference->GetStringArgument(2,""))->
			Set(LuaReference->GetStringArgument(3,"MissingValue"));
	return 0;
}

LuaGlue(_StencilMask_filename_set)
{
	if(OmnimapReference)
		OmnimapReference->StencilMask_filename_set(LuaReference->GetStringArgument(1,""));
	return 0;
}

LuaGlue(_Unlock) // Please Call Unlock from lua with the two codes
{
	if(OmnimapReference)
	{

		if(!om_protector.unlock(LuaReference->GetStringArgument(1,""),LuaReference->GetStringArgument(2,"")))
		{
			LogSystem()->ReportError("Omnimap not unlocked. Invalid code.");
		}else
		{
			LogSystem()->ReportError("Omnimap unlocked.  Thank you for supporting The Elumenati.");
		}
			
	}
	return 0;
}



LuaGlue( ClearShaderPreprocessorMacro)
{
	preprocessorMacroGlobal = std::string("");
	return 0;
}

LuaGlue( SetShaderPreprocessorMacro)
{
	const char* string = LuaReference->GetStringArgument(1,"");
	preprocessorMacroGlobal =  preprocessorMacroGlobal + "\r\n"+ std::string(string );
	return 0;
}


void LoadLuaFunctionTable(CLua *l,OmniMapBase * _pointertoomnimap)
{
	assert(l&&"Error void _lua");
	assert(_pointertoomnimap&&"Error void _pointertoomnimap");
	OmnimapReference = (OmniMapBase *) _pointertoomnimap;
	LuaReference =l;
	l->AddFunction("ClearScreenShapes",_ClearScreenShapes);
	l->AddFunction("Create_ScreenShapeDome",_Create_ScreenShapeDome);
	l->AddFunction("Create_ScreenShape",_Create_ScreenShape);

	l->AddFunction("SetShaderPreprocessorMacro",SetShaderPreprocessorMacro);
	l->AddFunction("ClearShaderPreprocessorMacro",ClearShaderPreprocessorMacro);


	l->AddFunction("Unlock",_Unlock);// THIS FUNCTION IS FOR REMOVING THE WATERMARK

#ifdef OmniMap_QUAD_UPGRADE
	l->AddFunction("Create_ScreenShapeQuad",_Create_ScreenShapeQuad);
#endif
	l->AddFunction("Clear_Channels",_Clear_Channels);
	l->AddFunction("Create_Channel",_Create_Channel);
#ifdef OLD_SHADER
	l->AddFunction("RecomputePlaneEquations",_RecomputePlaneEquations);
#endif
	l->AddFunction("SetScreenParameter",_SetScreenParameter);
	l->AddFunction("SetChannelParameter",_SetChannelParameter);
	l->AddFunction("SetOmnimapParameter",_SetOmnimapParameter);
	l->AddFunction("GetOmnimapParameter",_GetOmnimapParameter);
	l->AddFunction("GetOmnimapNumberParameter",_GetOmnimapNumberParameter);
	l->AddFunction("InitializeChannel", _InitializeChannel);
	// console
	l->AddFunction("ConsolePrintString"			,_ConsolePrintString);
	l->AddFunction("system"	,_system);
 	l->AddFunction("Clear"	,_Clear);
 	l->AddFunction("print"	,Debug_Print);
 	l->AddFunction("Print"	,Debug_Print);
	///////////////////////////////////////////////
////  GLUE FOR PER CHANNEL DATA...
///////////////////////////////////////////////
 	

		l->AddFunction("CreateChannelData_BOOL",_CreateChannelData_BOOL);
		l->AddFunction("SetChannelData_BOOL",_SetChannelData_BOOL);

		l->AddFunction("CreateChannelData_INT",_CreateChannelData_INT);
		l->AddFunction("SetChannelData_INT",_SetChannelData_INT);

		l->AddFunction("CreateChannelData_FLOAT",_CreateChannelData_FLOAT);
		l->AddFunction("SetChannelData_FLOAT",_SetChannelData_FLOAT);
		l->AddFunction("CreateChannelData_STRING",_CreateChannelData_STRING);
		l->AddFunction("SetChannelData_STRING",_SetChannelData_STRING);

		l->AddFunction("StencilMask_filename_set",_StencilMask_filename_set);

		

}
