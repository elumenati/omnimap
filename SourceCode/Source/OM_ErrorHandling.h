#ifndef __ERRORHANDLING_H__
#define __ERRORHANDLING_H__

#include <string>
#include <time.h>



#if defined(_MSC_VER)
#include <WinError.h>
#define EH_PRIVATE_PRAGMA __pragma
#define EH_DISABLE_WARNING_CONDITIONAL \
  EH_PRIVATE_PRAGMA(warning(push)) \
  EH_PRIVATE_PRAGMA(warning( disable : 4127 )) /*warning C4127: conditional expression is constant*/
#define EH_RESTORE_WARNING_CONDITIONAL \
  EH_PRIVATE_PRAGMA(warning(pop))
#define EH_IS_WARNING(V) ((long)(V) > 0)
#define EH_IS_ERROR(V) (((V) != EH_ERR_SUCCESS) && !EH_IS_WARNING(V))
#define EH_MAKE_WARNING(E) ((E)&0x7FFFFFFF)
#define EH_MAKE_ERROR(E) ((E)|0x80000000)
#define EH_CURRENT_FUNCTION __FUNCTION__
#else
#define EH_PRIVATE_PRAGMA _Pragma
#define EH_DISABLE_WARNING_CONDITIONAL
#define EH_RESTORE_WARNING_CONDITIONAL
#define EH_IS_WARNING(V) ((long)(V) > 0)
#define EH_IS_ERROR(V) (((V) != EH_ERR_SUCCESS) && !EH_IS_WARNING(V))
#define EH_MAKE_WARNING(E) ((E)&0x7FFFFFFF)
#define EH_MAKE_ERROR(E) ((E)|0x80000000)
#define EH_CURRENT_FUNCTION __PRETTY_FUNCTION__
#endif


#if defined(_MSC_VER)
#define EH_ERR_SUCCESS S_OK
#define EH_ERR_POINTER E_POINTER
#define EH_ERR_FAILED E_FAIL
#define EH_ERR_FILE_NOT_FOUND -ERROR_FILE_NOT_FOUND
#define EH_ERR_PATH_NOT_FOUND -ERROR_PATH_NOT_FOUND
#define EH_ERR_NOT_ENOUGH_MEMORY -ERROR_NOT_ENOUGH_MEMORY
#define EH_ERR_INVALID_PNG E_FAIL
#define EH_ERR_OGL_GENBUFFERS -0x1000
#define EH_ERR_OGL_GENTEXTURES -0x1001
#define EH_ERR_OGL_GENRENDERBUFFERS -0x1002
#define EH_ERR_OGL_GENFRAMEBUFFERS -0x1003
#define EH_ERR_OGL_CREATESHADEROBJECT -0x1004
#define EH_ERR_OGL_CREATEPROGRAMOBJECT -0x1005
#else
#define EH_ERR_SUCCESS 0
#define EH_ERR_POINTER -1
#define EH_ERR_FAILED -2
#define EH_ERR_FILE_NOT_FOUND -3
#define EH_ERR_PATH_NOT_FOUND -4
#define EH_ERR_NOT_ENOUGH_MEMORY -5
#define EH_ERR_INVALID_PNG -6
#define EH_ERR_OGL_GENBUFFERS -0x1000
#define EH_ERR_OGL_GENTEXTURES -0x1001
#define EH_ERR_OGL_GENRENDERBUFFER -0x1002
#define EH_ERR_OGL_GENFRAMEBUFFERS -0x1003
#define EH_ERR_OGL_CREATESHADEROBJECT -0x1004
#define EH_ERR_OGL_CREATEPROGRAMOBJECT -0x1005
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#define EH_GLOBALERRORFLAG_VALUE_DEFAULT EH_FLAG_DEFAULT
#ifndef EH_GLOBALERRORFLAG_VALUE
#define EH_GLOBALERRORFLAG_VALUE EH_GLOBALERRORFLAG_VALUE_DEFAULT
#endif

#define EH_GLOBALERRORFLAG_VAR_DEFAULT __EH_globalErrorFlag
#ifndef EH_GLOBALERRORFLAG_VAR
#define EH_GLOBALERRORFLAG_VAR EH_GLOBALERRORFLAG_VAR_DEFAULT
#endif

#define EH_ERRORLOG_DEFAULT(E, POS, ...) __EH_errorLogFunction(E, std::string(POS) + std::string(EH_CURRENT_FUNCTION), __FILE__, __LINE__, "", "", "", "", (EH_IS_WARNING((E).errorCode)) ? "Warning" : (EH_IS_ERROR((E).errorCode)) ? "Error" : "Log", __VA_ARGS__)
//#define EH_ERRORLOG_DEFAULT(E, POS, ...) __EH_errorLogFunction(E, std::string(POS) + EH_CURRENT_FUNCTION, __FILE__, __LINE__, __VERSION_PRODUCT_NAME, __VERSION_FULL_STRING, __VERSION_DATE_STRING, __VERSION_TIME_STRING, (EH_IS_WARNING((E).errorCode)) ? "Warning" : (EH_IS_ERROR((E).errorCode)) ? "Error" : "Log", __VA_ARGS__)
#ifndef EH_ERRORLOG
#define EH_ERRORLOG(E, POS, ...) EH_ERRORLOG_DEFAULT(E, POS, __VA_ARGS__)
#endif



#if defined(_MSC_VER)
#define EH_COMPOSE_ERROR_NUL_FILE FILE *f = fopen("nul", "wb");
#else
#define EH_COMPOSE_ERROR_NUL_FILE FILE *f = fopen("/dev/null", "wb");
#endif



#define EH_COMPOSE_ERROR_MESSAGE(STR) \
std::string STR; \
{ \
va_list ap; \
EH_COMPOSE_ERROR_NUL_FILE; \
\
va_start(ap, formatStr); \
size_t needed = vfprintf(f, formatStr, ap); \
va_end(ap); \
\
fclose(f); \
\
STR.resize(needed); \
\
va_start(ap, formatStr); \
vsprintf(&STR[0], formatStr, ap); \
va_end(ap); \
}



#define EH_DECLARE_DEFAULT_NO_LOG_FUNC()  \
  const int __EH_globalErrorFlag = EH_GLOBALERRORFLAG_VALUE; \
  void __EH_errorLogFunction(EH_DECLARE_REF, const std::string &functionStr, const std::string &fileStr, int line, const std::string &nameStr, const std::string &versionStr, const std::string &dateStr, const std::string &timeStr, const std::string &errorTypeStr) { \
  __EH_errorLogFunction(EH_VAR, functionStr, fileStr, line, nameStr, versionStr, dateStr, timeStr, errorTypeStr, ""); \
  } \
  void __EH_errorLogFunction(EH_DECLARE_REF, const std::string &functionStr, const std::string &fileStr, int line, const std::string &nameStr, const std::string &versionStr, const std::string &dateStr, const std::string &timeStr, const std::string &errorTypeStr, const char *formatStr, ...) {EH_VAR;versionStr;dateStr;timeStr;errorTypeStr;formatStr;}
#define EH_DECLARE_LOG_FUNC() \
  const int __EH_globalErrorFlag = EH_GLOBALERRORFLAG_VALUE; \
  void __EH_errorLogFunction(EH_DECLARE_REF, const std::string &functionStr, const std::string &fileStr, int line, const std::string &nameStr, const std::string &versionStr, const std::string &dateStr, const std::string &timeStr, const std::string &errorTypeStr) { \
    __EH_errorLogFunction(EH_VAR, functionStr, fileStr, line, nameStr, versionStr, dateStr, timeStr, errorTypeStr, ""); \
  } \
  void __EH_errorLogFunction(EH_DECLARE_REF, const std::string &functionStr, const std::string &fileStr, int line, const std::string &nameStr, const std::string &versionStr, const std::string &dateStr, const std::string &timeStr, const std::string &errorTypeStr, const char *formatStr, ...)
#define EH_DEFAULT_ERROR_LOG(PATH) { \
  EH_COMPOSE_ERROR_MESSAGE(str); \
  FILE *f = fopen(PATH, "at"); \
  if (f) { \
    time_t rawtime = time(NULL); \
    char *strtime = ctime(&rawtime); \
    strtime[strlen(strtime)-1] = 0; \
    fprintf(f, "(0x%08X)(%s)(%s;%s;%s)(%s)(%s)(%05d)(%c:0x%08X)(%s) -> %s\n", GetCurrentThreadId(), &nameStr[0], &versionStr[0], &dateStr[0], &timeStr[0], strtime, &fileStr[0], line, errorTypeStr[0], EH_ERRORCODE, &functionStr[0], &str[0]); \
    fclose(f); \
  } \
}


#define EH_FLAG_DEFAULT                     0x0000
#define EH_FLAG_DONT_LOG_ON_ERROR           0x0001
#define EH_FLAG_FORCE_PRE_LOG               0x0002
#define EH_FLAG_FORCE_POS_LOG               0x0004
#define EH_FLAG_FORCE_FULL_LOG              (EH_FLAG_FORCE_PRE_LOG|EH_FLAG_FORCE_POS_LOG)
#define EH_FLAG_DONT_JUMP_ON_ERROR          0x0008
#define EH_FLAG_DONT_SET_ERRORCODE          0x0010
#define EH_FLAG_ONLY_TEST_IF_ERROR_SUCCESS  0x0020
#define EH_FLAG_ONLY_TEST_IF_NO_ERROR       0x0040   // can be warning
#define EH_FLAG_DONT_RESTORE_LAST_ERRORCODE 0x0080
#define EH_FLAG_USE_WARNING_MODE            0x0100
#define EH_FLAG_SIMPLE_LOG_IS_USED          0x0200


#define EH_STRUCT struct __EH_VAR_STRUCT
#define EH_STRUCT_REF EH_STRUCT &
#define EH_STRUCT_PTR EH_STRUCT *
#define EH_DECLARE EH_STRUCT __EH_errorStruct = {(long)EH_ERR_SUCCESS, EH_FLAG_DEFAULT}
#define EH_DECLARE_REF EH_STRUCT_REF __EH_errorStruct
#define EH_DECLARE_PTR EH_STRUCT_PTR __EH_errorStruct
#define EH_DECLARE_NOINIT EH_STRUCT __EH_errorStruct
#define EH_VAR __EH_errorStruct
#define EH_ERRORCODE EH_VAR.errorCode
#define EH_ERRORFLAG EH_VAR.errorFlag


#define EH_SetErrorCode(V) (EH_ERRORCODE = V)
#define EH_SetErrorFlag(V) (EH_ERRORFLAG = V)
#define EH_Default(E) { (E).errorCode = (long)EH_ERR_SUCCESS; (E).errorFlag = (long)EH_FLAG_DEFAULT; }
#define EH_GotoOnError() goto __EH_exitError

#define EH_Log(...) { \
  EH_DISABLE_WARNING_CONDITIONAL \
  if (EH_GLOBALERRORFLAG_VAR & (EH_FLAG_FORCE_PRE_LOG|EH_FLAG_FORCE_POS_LOG|EH_FLAG_SIMPLE_LOG_IS_USED)) EH_ERRORLOG(EH_VAR, "LOG ", __VA_ARGS__); \
  else if ((EH_ERRORCODE != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR)) || (EH_ERRORFLAG & (EH_FLAG_FORCE_PRE_LOG|EH_FLAG_FORCE_POS_LOG))) EH_ERRORLOG(EH_VAR, "LOG ", __VA_ARGS__); \
  EH_RESTORE_WARNING_CONDITIONAL \
}

#define EH_TestErr(E, RET, ...) { \
  EH_DISABLE_WARNING_CONDITIONAL \
  long _shouldDoTest = TRUE; \
  if (EH_ERRORFLAG & EH_FLAG_ONLY_TEST_IF_ERROR_SUCCESS) _shouldDoTest = EH_ERRORCODE == EH_ERR_SUCCESS; \
  if (EH_ERRORFLAG & EH_FLAG_ONLY_TEST_IF_NO_ERROR) _shouldDoTest = !EH_IS_ERROR(EH_ERRORCODE); \
  if (_shouldDoTest) { \
    long _saveError = EH_ERRORCODE; \
    if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) && ((EH_ERRORFLAG & EH_FLAG_FORCE_PRE_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_PRE_LOG))) EH_ERRORLOG(EH_VAR, "PRE ", __VA_ARGS__); \
    if (EH_SetErrorCode(RET) != EH_ERR_SUCCESS) { \
      EH_SetErrorCode(E); \
      if (EH_ERRORCODE != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_USE_WARNING_MODE)) EH_SetErrorCode(EH_MAKE_ERROR(E)); \
      if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) || (EH_ERRORFLAG & EH_FLAG_FORCE_POS_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_POS_LOG)) EH_ERRORLOG(EH_VAR, "POS ", __VA_ARGS__); \
      if (!EH_IS_WARNING(EH_ERRORCODE) && !(EH_ERRORFLAG & EH_FLAG_DONT_JUMP_ON_ERROR)) goto __EH_exitError; \
    } else { \
      EH_SetErrorCode(EH_ERR_SUCCESS); \
      if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) && ((EH_ERRORFLAG & EH_FLAG_FORCE_POS_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_POS_LOG))) EH_ERRORLOG(EH_VAR, "POS ", __VA_ARGS__); \
    } \
    if ((_saveError != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_DONT_RESTORE_LAST_ERRORCODE)) || (EH_ERRORFLAG & EH_FLAG_DONT_SET_ERRORCODE)) EH_SetErrorCode(_saveError); \
  } \
  EH_RESTORE_WARNING_CONDITIONAL \
}
#define EH_CmpErr(E, ERR, RET, ...) { \
  EH_DISABLE_WARNING_CONDITIONAL \
  long _shouldDoTest = TRUE; \
  if (EH_ERRORFLAG & EH_FLAG_ONLY_TEST_IF_ERROR_SUCCESS) _shouldDoTest = EH_ERRORCODE == EH_ERR_SUCCESS; \
  if (EH_ERRORFLAG & EH_FLAG_ONLY_TEST_IF_NO_ERROR) _shouldDoTest = !EH_IS_ERROR(EH_ERRORCODE); \
  if (_shouldDoTest) { \
    long _saveError = EH_ERRORCODE; \
    if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) && ((EH_ERRORFLAG & EH_FLAG_FORCE_PRE_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_PRE_LOG))) EH_ERRORLOG(EH_VAR, "PRE ", __VA_ARGS__); \
    if ((RET) == (ERR)) { \
      EH_SetErrorCode(E); \
      if (EH_ERRORCODE != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_USE_WARNING_MODE)) EH_SetErrorCode(EH_MAKE_ERROR(E)); \
      if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) || (EH_ERRORFLAG & EH_FLAG_FORCE_POS_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_POS_LOG)) EH_ERRORLOG(EH_VAR, "POS ", __VA_ARGS__); \
      if (!EH_IS_WARNING(EH_ERRORCODE) && !(EH_ERRORFLAG & EH_FLAG_DONT_JUMP_ON_ERROR)) goto __EH_exitError; \
    } else { \
      EH_SetErrorCode(EH_ERR_SUCCESS); \
      if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) && ((EH_ERRORFLAG & EH_FLAG_FORCE_POS_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_POS_LOG))) EH_ERRORLOG(EH_VAR, "POS ", __VA_ARGS__); \
    } \
    if ((_saveError != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_DONT_RESTORE_LAST_ERRORCODE)) || (EH_ERRORFLAG & EH_FLAG_DONT_SET_ERRORCODE)) EH_SetErrorCode(_saveError); \
  } \
  EH_RESTORE_WARNING_CONDITIONAL \
}
#define EH_NotCmpErr(E, ERR, RET, ...) { \
  EH_DISABLE_WARNING_CONDITIONAL \
  long _shouldDoTest = TRUE; \
  if (EH_ERRORFLAG & EH_FLAG_ONLY_TEST_IF_ERROR_SUCCESS) _shouldDoTest = EH_ERRORCODE == EH_ERR_SUCCESS; \
  if (EH_ERRORFLAG & EH_FLAG_ONLY_TEST_IF_NO_ERROR) _shouldDoTest = !EH_IS_ERROR(EH_ERRORCODE); \
  if (_shouldDoTest) { \
    long _saveError = EH_ERRORCODE; \
    if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) && ((EH_ERRORFLAG & EH_FLAG_FORCE_PRE_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_PRE_LOG))) EH_ERRORLOG(EH_VAR, "PRE ", __VA_ARGS__); \
    if ((RET) != (ERR)) { \
      EH_SetErrorCode(E); \
      if (EH_ERRORCODE != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_USE_WARNING_MODE)) EH_SetErrorCode(EH_MAKE_ERROR(E)); \
      if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) || (EH_ERRORFLAG & EH_FLAG_FORCE_POS_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_POS_LOG)) EH_ERRORLOG(EH_VAR, "POS ", __VA_ARGS__); \
      if (!EH_IS_WARNING(EH_ERRORCODE) && !(EH_ERRORFLAG & EH_FLAG_DONT_JUMP_ON_ERROR)) goto __EH_exitError; \
    } else { \
      EH_SetErrorCode(EH_ERR_SUCCESS); \
      if (!(EH_ERRORFLAG & EH_FLAG_DONT_LOG_ON_ERROR) && ((EH_ERRORFLAG & EH_FLAG_FORCE_POS_LOG) || (EH_GLOBALERRORFLAG_VAR & EH_FLAG_FORCE_POS_LOG))) EH_ERRORLOG(EH_VAR, "POS ", __VA_ARGS__); \
    } \
    if ((_saveError != EH_ERR_SUCCESS && !(EH_ERRORFLAG & EH_FLAG_DONT_RESTORE_LAST_ERRORCODE)) || (EH_ERRORFLAG & EH_FLAG_DONT_SET_ERRORCODE)) EH_SetErrorCode(_saveError); \
  } \
  EH_RESTORE_WARNING_CONDITIONAL \
}
#define EH_ZeroErr(E, RET, ...) EH_CmpErr(E, 0, RET, __VA_ARGS__)
#define EH_NotZeroErr(E, RET, ...) EH_NotCmpErr(E, 0, RET, __VA_ARGS__)
#define EH_PtrErr(E, RET, ...) EH_CmpErr(E, NULL, RET, __VA_ARGS__)

#define EH_Test(RET, ...) EH_TestErr(EH_ERRORCODE, RET, __VA_ARGS__)
#define EH_Cmp(ERR, RET, ...) EH_CmpErr(EH_ERR_FAILED, ERR, RET, __VA_ARGS__)
#define EH_NotCmp(ERR, RET, ...) EH_NotCmpErr(EH_ERR_FAILED, ERR, RET, __VA_ARGS__)
#define EH_Zero(RET, ...) EH_ZeroErr(EH_ERR_FAILED, RET, __VA_ARGS__)
#define EH_NotZero(RET, ...) EH_NotZeroErr(EH_ERR_FAILED, RET, __VA_ARGS__)
#define EH_Ptr(RET, ...) EH_PtrErr(EH_ERR_POINTER, RET, __VA_ARGS__)

#define EH_OnError() __EH_exitError: EH_SetErrorFlag(EH_FLAG_DONT_JUMP_ON_ERROR); if (EH_IS_ERROR(EH_ERRORCODE))
#define EH_OnWarning() if (EH_IS_WARNING(EH_ERRORCODE))


struct __EH_VAR_STRUCT { long errorCode; long errorFlag; };
void __EH_errorLogFunction(EH_DECLARE_REF, const std::string &functionStr, const std::string &fileStr, int line, const std::string &nameStr, const std::string &versionStr, const std::string &dateStr, const std::string &timeStr, const std::string &errorTypeStr);
void __EH_errorLogFunction(EH_DECLARE_REF, const std::string &functionStr, const std::string &fileStr, int line, const std::string &nameStr, const std::string &versionStr, const std::string &dateStr, const std::string &timeStr, const std::string &errorTypeStr, const char *formatStr, ...);
extern const int __EH_globalErrorFlag;


#endif

