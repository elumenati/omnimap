#pragma once
#ifndef __IPCCONFIG_H__
#define __IPCCONFIG_H__



#include <Windows.h>



#ifndef DOMEVIEWAPI
# ifdef DOMEVIEWAPI_EXPORT
#  define DOMEVIEWAPI __declspec(dllexport)
# elif  DOMEVIEWAPI_IMPORT
#  define DOMEVIEWAPI __declspec(dllimport)
# else
#  define DOMEVIEWAPI
# endif
#endif




#define CONFIG_STREAM_ID "9EDB8065-E04E-4dd6-A60E-2B1D3BB82112"



#define CREATEGUID(S) {\
  GUID temp;\
  CoCreateGuid(&temp);\
  sprintf(S, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", temp.Data1, temp.Data2, temp.Data3, \
  temp.Data4[0], temp.Data4[1], temp.Data4[2], temp.Data4[3], temp.Data4[4], temp.Data4[5], temp.Data4[6], temp.Data4[7]);\
}




enum {
  ACTION_NONE = 0,
  ACTION_REGISTRY,
  ACTION_REOPEN,
  ACTION_CLOSE
};






struct IPCConfig {
public:
  virtual void APIENTRY resetParams() = 0;

  virtual int APIENTRY readIsUsed() = 0;
  virtual void APIENTRY readParams() = 0;
  virtual void APIENTRY forceReadParams() = 0;

  virtual int APIENTRY writeIsFree() = 0;
  virtual void APIENTRY writeParams() = 0;
  virtual void APIENTRY forceWriteParams() = 0;

  virtual void APIENTRY generateNewPluginGuid() = 0;
  virtual void APIENTRY generateNewPreviewGuid() = 0;

  virtual int APIENTRY getVersion() const = 0;
  virtual int APIENTRY getAction() const = 0;
  virtual int APIENTRY getId() const = 0;
  virtual char * APIENTRY getPluginGuid() const = 0;
  virtual char * APIENTRY getPreviewGuid() const = 0;
  virtual char * APIENTRY getOldPluginGuid() const = 0;
  virtual char * APIENTRY getOldPreviewGuid() const = 0;
  virtual int APIENTRY getPluginSlots() const = 0;
  virtual int APIENTRY getPluginSlotsSize() const = 0;
  virtual int APIENTRY getPreviewSlots() const = 0;
  virtual int APIENTRY getPreviewSlotsSize() const = 0;


  virtual void APIENTRY setVersion(int version) = 0;
  virtual void APIENTRY setAction(int action) = 0;
  virtual void APIENTRY setId(int id) = 0;
  virtual void APIENTRY setPluginGuid(char *pluginGuid) = 0;
  virtual void APIENTRY setPreviewGuid(char *previewGuid) = 0;
  virtual void APIENTRY setOldPluginGuid(char *oldPluginGuid) = 0;
  virtual void APIENTRY setOldPreviewGuid(char *oldPreviewGuid) = 0;
  virtual void APIENTRY setPluginSlots(int pluginSlots) = 0;
  virtual void APIENTRY setPluginSlotsSize(int pluginSlotsSize) = 0;
  virtual void APIENTRY setPreviewSlots(int previewSlots) = 0;
  virtual void APIENTRY setPreviewSlotsSize(int previewSlotsSize) = 0;

  virtual HRESULT APIENTRY errorCode() = 0;
  virtual void APIENTRY release() = 0;
};



typedef IPCConfig* (APIENTRY *IPCConfigBuilderServerFUNC)(char *name); 
typedef IPCConfig* (APIENTRY *IPCConfigBuilderClientFUNC)(char *name); 



extern "C" DOMEVIEWAPI IPCConfig* APIENTRY IPCConfigBuilderServer(char *name);
extern "C" DOMEVIEWAPI IPCConfig* APIENTRY IPCConfigBuilderClient(char *name);


#endif
