#pragma once

#include <NFDistorterInterface.h>

extern NFMemoryAccessInit_Typedef NFMemoryAccessInitFunc;
extern NFMemoryAccessClose_Typedef NFMemoryAccessCloseFunc;
extern NFMemoryAccessTestChange_Typedef NFMemoryAccessTestChangeFunc;
extern NFMemoryAccessSetWarping_Typedef NFMemoryAccessSetWarpingFunc;
extern NFMemoryAccessGetWarping_Typedef NFMemoryAccessGetWarpingFunc;
extern NFMemoryAccessAdquireLock_Typedef NFMemoryAccessAdquireLockFunc;
extern NFMemoryAccessReleaseLock_Typedef NFMemoryAccessReleaseLockFunc;


int loadCobraInterface(char *pathToDll);
void closeCobraInterface(void);
