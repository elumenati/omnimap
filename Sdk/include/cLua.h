#pragma once
#include <float.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LUADLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LUADLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
/*
#ifdef WIN32
#ifdef LUADLL_EXPORTS
#define LUADLL_API __declspec(dllexport)
#else
//#pragma comment (lib, "cLua.lib")
#define LUADLL_API __declspec(dllimport)
#endif
#else
#define LUADLL_API
#endif
*/
#define LUADLL_API

struct lua_State;

// use this to create LuaGlue function 
// LuaGlue(_FunctionName)
// {
//   return 0;
// }
#define LuaGlue(n) extern "C" int n(lua_State *L)

extern "C" {
typedef int (*LuaFunctionType)(struct lua_State *pLuaState);
typedef struct 
{
	const char *name;
	int (*func)(lua_State *);
}luaDef;
};

enum LuaArgType
{
	ARG_TYPE_NIL,
	ARG_TYPE_NUMBER,
	ARG_TYPE_STRING,
	ARG_TYPE_BOOL,
	ARG_TYPE_UNKNOWN
};


// This class is exported from the Lua DLL
class LUADLL_API CLua {
public:
	CLua(bool bOpenConsole);
	virtual ~CLua();

	bool		RunScript(const char *pFilename);
	bool		RunString(const char *pCommand);

	bool		AddFunction(const char *pFunctionName, LuaFunctionType pFunction);
	bool		AddFunction(const luaDef *pLuaDefs);

	const char *GetStringArgument(int num, const char *pDefault=NULL);
	double		GetNumberArgument(int num, double dDefault=0.0);
	bool		GetBoolArgument(int num, bool bDefault=false);

	void		PushString(const char *pString);
	void		PushNumber(double value);
	void		PushBool(bool bVal);
	void		PushNil(void);

	LuaArgType	GetArgumentType(int num);

	void		SetErrorHandler(void(*pErrHandler)(const char *pError)) {m_pErrorHandler = pErrHandler;}
	void		ReportError(const char *pErrorString);

	lua_State	*GetScriptContext(void)		{return m_pScriptContext;}

private:
	lua_State	*m_pScriptContext;
	void(*m_pErrorHandler)(const char *pError);

};

class CLuaFCData;

class LUADLL_API CLuaFunctionCall {
public:
	CLuaFunctionCall(CLua *pLua, const char *pFuncName, int numRets=0);
	virtual ~CLuaFunctionCall(){}

	int			Run(void);

	LuaArgType	GetReturnValueType();

	CLuaFunctionCall& operator<<(const char *) ;
	CLuaFunctionCall& operator<<(float) ;
	CLuaFunctionCall& operator<<(double) ;
	CLuaFunctionCall& operator<<(int) ;
	CLuaFunctionCall& operator<<(unsigned int) ;
	CLuaFunctionCall& operator<<(bool) ;

	CLuaFunctionCall& operator>>(const char *&) ;
	CLuaFunctionCall& operator>>(float&) ;
	CLuaFunctionCall& operator>>(double&) ;
	CLuaFunctionCall& operator>>(int&) ;
	CLuaFunctionCall& operator>>(unsigned int&) ;
	CLuaFunctionCall& operator>>(bool&) ;
private:
	CLuaFCData *m_pFCData;

};
