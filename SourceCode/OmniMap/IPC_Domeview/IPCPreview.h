#pragma once
#ifndef __IPCPREVIEW_H__
#define __IPCPREVIEW_H__



#include "IPCConfig.h"




struct IPCPreview {
public:
  virtual void APIENTRY resetParams() = 0;
  virtual int APIENTRY newParamsChanged() = 0;
  virtual int APIENTRY isServerOnline() = 0;
  virtual int APIENTRY isClientOnline() = 0;

  virtual int APIENTRY readIsUsed() = 0;
  virtual void APIENTRY readParams() = 0;
  virtual void APIENTRY forceReadParams() = 0;
  virtual void APIENTRY readReopen(IPCConfig *cfg) = 0;

  virtual int APIENTRY writeIsFree() = 0;
  virtual void APIENTRY writeParams() = 0;
  virtual void APIENTRY forceWriteParams() = 0;
  virtual void APIENTRY writeReopen(IPCConfig *cfg) = 0;

  virtual int APIENTRY getVersion() const = 0;
  virtual int APIENTRY getParamsSize() const = 0;

  virtual void APIENTRY setVersion(int version) = 0;
  virtual void APIENTRY setParamsSize(int paramsSize) = 0;

  virtual HRESULT APIENTRY errorCode() = 0;
  virtual void APIENTRY release() = 0;
};




typedef IPCPreview* (APIENTRY *IPCPreviewBuilderFUNC)(); 
typedef IPCPreview* (APIENTRY *IPCPreviewBuilderServerFUNC)(IPCConfig *cfg); 
typedef IPCPreview* (APIENTRY *IPCPreviewBuilderClientFUNC)(IPCConfig *cfg); 



extern "C" DOMEVIEWAPI IPCPreview* APIENTRY IPCPreviewBuilder();
extern "C" DOMEVIEWAPI IPCPreview* APIENTRY IPCPreviewBuilderServer(IPCConfig *cfg);
extern "C" DOMEVIEWAPI IPCPreview* APIENTRY IPCPreviewBuilderClient(IPCConfig *cfg);


#endif
