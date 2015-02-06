// Lua.cpp : Defines the entry point for the DLL application.
//


#ifdef WIN32
#include "stdafx.h"
#endif

extern "C" {
#include "../cLua/LuaDist/lua-5.1/src/lua.h"
#include "../cLua/LuaDist/lua-5.1/src/lualib.h"
#include "../cLua/LuaDist/lua-5.1/src/lauxlib.h"
}

#include "cLua.h"
#include <list>
#include <string>
//#include "WinConsole.h"



// KLUDGE: Used to return strings, should do something dynamic here
static std::string stringBuf;
#ifdef WIN32
HANDLE g_hModule;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	g_hModule = hModule;
    return TRUE;
}
#endif


static void defaultErrorHandler(const char *pError)
{
	//CWinConsole::Write(pError);
}

CLua::CLua(bool bOpenConsole)
{
	m_pErrorHandler = defaultErrorHandler;

	m_pScriptContext = lua_open();
		luaL_openlibs(m_pScriptContext);

	if(bOpenConsole)
	{
		//CWinConsole::StartConsole(NULL, this);
	}

	
		
}


CLua::~CLua()
{
	//CWinConsole::StopConsole();
	if(m_pScriptContext)
		lua_close(m_pScriptContext);
}


bool CLua::RunScript(const char *pFname)
{
	if (0 != luaL_loadfile(m_pScriptContext, pFname))
	{
		if(m_pErrorHandler)
		{
			char buf[256];
			sprintf(buf, "Lua Error - Script Load\nScript Name:%s\nError Message:%s\n", pFname, luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
		}

		return false;
	}
	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		if(m_pErrorHandler)
		{
			char buf[256];
			sprintf(buf, "Lua Error - Script Run\nScript Name:%s\nError Message:%s\n", pFname, luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
		}

		return false;
	}
	return true;

}

bool CLua::RunString(const char *pCommand)
{
	if (0 != luaL_loadbuffer(m_pScriptContext, pCommand, strlen(pCommand), NULL))
	{
		if(m_pErrorHandler)
		{
			char buf[256];
			sprintf(buf, "Lua Error - String Load\nError Message:%s\nString:", luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
			m_pErrorHandler(pCommand);
		}

		return false;
	}
	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		if(m_pErrorHandler)
		{
			char buf[256];
			sprintf(buf, "Lua Error - String Run\nError Message:%s\nString:", luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
			m_pErrorHandler(pCommand);
		}

		return false;
	}
	return true;
}

bool CLua::AddFunction(const char *pFunctionName, LuaFunctionType pFunction)
{
	lua_register(m_pScriptContext, pFunctionName, pFunction);
	return true;
}

bool CLua::AddFunction(const luaDef *pLuaDefs)
{
	for(int i=0; pLuaDefs[i].name; i++)
	{
		AddFunction(pLuaDefs[i].name, pLuaDefs[i].func);
	}
	return true;
}


const char *CLua::GetStringArgument(int num, const char *pDefault)
{
	return luaL_optstring(m_pScriptContext, num, pDefault);

}

double CLua::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

bool CLua::GetBoolArgument(int num, bool bDefault)
{
	if (lua_isnoneornil(m_pScriptContext, num)) return bDefault;

	return lua_toboolean(m_pScriptContext, num) != 0;
}

LuaArgType CLua::GetArgumentType(int num)
{
	int lType = lua_type(m_pScriptContext, num);
	LuaArgType ret = ARG_TYPE_UNKNOWN;

	switch(lType)
	{
	case LUA_TNIL:
		ret = ARG_TYPE_NIL;
		break;
	case LUA_TBOOLEAN:
		ret = ARG_TYPE_BOOL;
		break;
	case LUA_TNUMBER:
		ret = ARG_TYPE_NUMBER;
		break;
	case LUA_TSTRING:
		ret = ARG_TYPE_STRING;
		break;
	}

	return ret;
}

void CLua::PushBool(bool bVal)
{
	lua_pushboolean(m_pScriptContext, bVal);
}

void CLua::PushNil()
{
	lua_pushnil(m_pScriptContext);
}

void CLua::PushString(const char *pString)
{
	lua_pushstring(m_pScriptContext, pString);
}

void CLua::PushNumber(double value)
{
	lua_pushnumber(m_pScriptContext, value);
}


void CLua::ReportError(const char *pErrorString)
{
	m_pErrorHandler(pErrorString);
}

/******************************************************/

class CLuaValue
{
public:
	CLuaValue() {m_type=ARG_TYPE_NIL;}
	~CLuaValue() {}

	LuaArgType m_type;

	std::string m_string;
	double m_number;
	bool m_bool;
};

class CLuaFCData
{
public:
	CLuaFCData() {}
	~CLuaFCData() {}

	std::list<CLuaValue> m_lstArgs;
	std::list<CLuaValue> m_lstRets;

	char *m_sFunctionName;
	bool m_bExe;
	CLua *m_pLua;
	int m_numRets;

};

CLuaFunctionCall::CLuaFunctionCall(CLua *pLua, const char *pFuncName, int numRets)
{
	m_pFCData = new CLuaFCData;

	m_pFCData->m_bExe = false;
	m_pFCData->m_sFunctionName = new char[strlen(pFuncName)+1]; 
	strcpy(m_pFCData->m_sFunctionName, pFuncName);
	m_pFCData->m_pLua = pLua;
	m_pFCData->m_numRets = numRets;
}

CLuaFunctionCall& 	CLuaFunctionCall::operator<<(const char *s)
{
	CLuaValue work;
	work.m_type = ARG_TYPE_STRING;
	work.m_string = s;
	m_pFCData->m_lstArgs.push_back(work);

	return *this;
}

CLuaFunctionCall& CLuaFunctionCall::operator<<(float f)
{
	CLuaValue work;
	work.m_type = ARG_TYPE_NUMBER;
	work.m_number = (double) f;
	m_pFCData->m_lstArgs.push_back(work);

	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator<<(double d)
{
	CLuaValue work;
	work.m_type = ARG_TYPE_NUMBER;
	work.m_number = d;
	m_pFCData->m_lstArgs.push_back(work);

	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator<<(int i) 
{
	CLuaValue work;
	work.m_type = ARG_TYPE_NUMBER;
	work.m_number = (double) i;
	m_pFCData->m_lstArgs.push_back(work);

	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator<<(unsigned int i) 
{
	CLuaValue work;
	work.m_type = ARG_TYPE_NUMBER;
	work.m_number = (double) i;
	m_pFCData->m_lstArgs.push_back(work);

	return *this;
}

CLuaFunctionCall& CLuaFunctionCall::operator<<(bool b) 
{
	CLuaValue work;
	work.m_type = ARG_TYPE_BOOL;
	work.m_bool = b;
	m_pFCData->m_lstArgs.push_back(work);

	return *this;
}

LuaArgType	CLuaFunctionCall::GetReturnValueType()
{
	LuaArgType ret = ARG_TYPE_NIL;

	if(m_pFCData->m_lstRets.size())
	{
		ret = (*m_pFCData->m_lstRets.begin()).m_type;
	}
	return ret;
}

CLuaFunctionCall& CLuaFunctionCall::operator>>(const char *&s) 
{
	stringBuf = "error";

	if(m_pFCData->m_lstRets.size())
	{
		CLuaValue work = (*m_pFCData->m_lstRets.begin());
		m_pFCData->m_lstRets.pop_front();

		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			if(work.m_bool)
				stringBuf = "true";
			else
				stringBuf = "false";
			break;
		case ARG_TYPE_NUMBER:
			{
				char buf[32];
				sprintf(buf, "%f", work.m_number);
				stringBuf = buf;
			}
			break;
		case ARG_TYPE_STRING:
			stringBuf = work.m_string;
			break;
		}
	}
	s = stringBuf.c_str();

	return *this;
}

CLuaFunctionCall& CLuaFunctionCall::operator>>(float &f) 
{
	if(m_pFCData->m_lstRets.size())
	{
		CLuaValue work = (*m_pFCData->m_lstRets.begin());
		m_pFCData->m_lstRets.pop_front();

		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			if(work.m_bool)
				f = 0;
			else
				f = 1;
			break;
		case ARG_TYPE_NUMBER:
			f = (float) work.m_number;
			break;
		case ARG_TYPE_STRING:
			f = FLT_MAX;
			break;
		}
	}
	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator>>(double &d) 
{
	if(m_pFCData->m_lstRets.size())
	{
		CLuaValue work = (*m_pFCData->m_lstRets.begin());
		m_pFCData->m_lstRets.pop_front();

		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			if(work.m_bool)
				d = 0;
			else
				d = 1;
			break;
		case ARG_TYPE_NUMBER:
			d = work.m_number;
			break;
		case ARG_TYPE_STRING:
			d = FLT_MAX;
			break;
		}
	}
	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator>>(int &i)
{
	if(m_pFCData->m_lstRets.size())
	{
		CLuaValue work = (*m_pFCData->m_lstRets.begin());
		m_pFCData->m_lstRets.pop_front();

		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			if(work.m_bool)
				i = 0;
			else
				i = 1;
			break;
		case ARG_TYPE_NUMBER:
			i = (int) work.m_number;
			break;
		case ARG_TYPE_STRING:
			i = 0xffffffff;
			break;
		}
	}
	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator>>(unsigned int &i) 
{
	if(m_pFCData->m_lstRets.size())
	{
		CLuaValue work = (*m_pFCData->m_lstRets.begin());
		m_pFCData->m_lstRets.pop_front();

		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			if(work.m_bool)
				i = 0;
			else
				i = 1;
			break;
		case ARG_TYPE_NUMBER:
			i = (unsigned int) work.m_number;
			break;
		case ARG_TYPE_STRING:
			i = 0xffffffff;
			break;
		}
	}
	return *this;
}
CLuaFunctionCall& CLuaFunctionCall::operator>>(bool &b) 
{
	if(m_pFCData->m_lstRets.size())
	{
		CLuaValue work = (*m_pFCData->m_lstRets.begin());
		m_pFCData->m_lstRets.pop_front();

		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			b = work.m_bool;
			break;
		case ARG_TYPE_NUMBER:
			b = (work.m_number != 0);
			break;
		case ARG_TYPE_STRING:
			b = false;
			break;
		}
	}
	return *this;
}

int	CLuaFunctionCall::Run(void)
{
	lua_State *L = m_pFCData->m_pLua->GetScriptContext();

	/* push functions and arguments */
	lua_getglobal(L, m_pFCData->m_sFunctionName);  /* function to be called */

	std::list<CLuaValue>::iterator it = m_pFCData->m_lstArgs.begin();
	while(it != m_pFCData->m_lstArgs.end())
	{
		CLuaValue work = (*it);
		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			lua_pushboolean(L, work.m_bool);   /* push argument */
			break;
		case ARG_TYPE_NUMBER:
			lua_pushnumber(L, work.m_number);   /* push argument */
			break;
		case ARG_TYPE_STRING:
			lua_pushstring(L, work.m_string.c_str());   /* push argument */
			break;
		case ARG_TYPE_NIL:
			lua_pushnil(L);
			break;
		}
		++it;
	}
	/* do the call (size() arguments, numRets results) */
	if (lua_pcall(L, (int) m_pFCData->m_lstArgs.size(), m_pFCData->m_numRets, 0) != 0)
	{
		//error
		std::string error = "Lua Error - Function Call:";
		error += luaL_checkstring(L, -1);
		m_pFCData->m_pLua->ReportError(error.c_str());
	}

	for(int i=0; i<m_pFCData->m_numRets; i++)
	{
		// get return value and add to our list of returns
		CLuaValue work;
		work.m_type = m_pFCData->m_pLua->GetArgumentType(-1-i);
		switch(work.m_type)
		{
		case ARG_TYPE_BOOL:
			work.m_bool = lua_toboolean(L, -1-i) != 0;   
			break;
		case ARG_TYPE_NUMBER:
			work.m_number = lua_tonumber(L, -1-i);   
			break;
		case ARG_TYPE_STRING:
			work.m_string = lua_tostring(L, -1-i);   
			break;
		}
		m_pFCData->m_lstRets.push_front(work);
	}
	m_pFCData->m_bExe = true;
	return (int) m_pFCData->m_lstRets.size();
}
