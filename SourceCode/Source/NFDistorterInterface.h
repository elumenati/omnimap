#ifndef __NFDISTORTERINTERFACE_H__
#define __NFDISTORTERINTERFACE_H__


typedef int (NFMemoryAccessInit_Typedef)(void);
typedef void (NFMemoryAccessClose_Typedef)(void);
typedef int (NFMemoryAccessTestChange_Typedef)(void);
typedef int (NFMemoryAccessSetWarping_Typedef)(int warping);
typedef int (NFMemoryAccessGetWarping_Typedef)(void);
typedef void *(NFMemoryAccessAdquireLock_Typedef)(int desiredMeshWith, int desiredMeshHeight);
typedef void (NFMemoryAccessReleaseLock_Typedef)(void);


int NFMemoryAccessInit(void);
void NFMemoryAccessClose(void);
int NFMemoryAccessTestChange(void);
int NFMemoryAccessSetWarping(int warping);
int NFMemoryAccessGetWarping(void);
void *NFMemoryAccessAdquireLock(int desiredMeshWith, int desiredMeshHeight);
void NFMemoryAccessReleaseLock(void);


#endif
