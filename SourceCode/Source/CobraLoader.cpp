#include "CobraLoader.h"
#include <OM_ErrorHandling.h>
#include <string>



static HMODULE _cobraModule = NULL;

NFMemoryAccessInit_Typedef NFMemoryAccessInitFunc = NULL;
NFMemoryAccessClose_Typedef NFMemoryAccessCloseFunc = NULL;
NFMemoryAccessTestChange_Typedef NFMemoryAccessTestChangeFunc = NULL;
NFMemoryAccessSetWarping_Typedef NFMemoryAccessSetWarpingFunc = NULL;
NFMemoryAccessGetWarping_Typedef NFMemoryAccessGetWarpingFunc = NULL;
NFMemoryAccessAdquireLock_Typedef NFMemoryAccessAdquireLockFunc = NULL;
NFMemoryAccessReleaseLock_Typedef NFMemoryAccessReleaseLockFunc = NULL;




int loadCobraInterface(char *pathToDll) {
  std::string finalPath;
  EH_DECLARE;

  if (pathToDll == NULL) finalPath = ".\\";
  else finalPath = pathToDll + "\\";

  EH_Ptr(_cobraModule = LoadLibrary(pathToDll + "NFDistorterInterface.dll"));
  EH_Ptr(NFMemoryAccessInitFunc = (NFMemoryAccessInit_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessInit"));
  EH_Ptr(NFMemoryAccessCloseFunc = (NFMemoryAccessClose_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessClose"));
  EH_Ptr(NFMemoryAccessTestChangeFunc = (NFMemoryAccessTestChange_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessTestChange"));
  EH_Ptr(NFMemoryAccessSetWarpingFunc = (NFMemoryAccessSetWarping_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessSetWarping"));
  EH_Ptr(NFMemoryAccessGetWarpingFunc = (NFMemoryAccessGetWarping_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessGetWarping"));
  EH_Ptr(NFMemoryAccessAdquireLockFunc = (NFMemoryAccessAdquireLock_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessAdquireLock"));
  EH_Ptr(NFMemoryAccessReleaseLockFunc = (NFMemoryAccessReleaseLock_Typedef) GetProcAddress(_cobraModule, "NFMemoryAccessReleaseLock"));

  EH_OnError() {}

  return EH_ERRORCODE;
}


void closeCobraInterface(void) {
  if (_cobraModule) { FreeLibrary(_cobraModule); _cobraModule = NULL; }
  NFMemoryAccessInitFunc = NULL;
  NFMemoryAccessCloseFunc = NULL;
  NFMemoryAccessTestChangeFunc = NULL;
  NFMemoryAccessSetWarpingFunc = NULL;
  NFMemoryAccessGetWarpingFunc = NULL;
  NFMemoryAccessAdquireLockFunc = NULL;
  NFMemoryAccessReleaseLockFunc = NULL;
}
