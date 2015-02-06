#pragma once
#ifndef __IPCDOMEVIEW_H__
#define __IPCDOMEVIEW_H__


#include "IPCConfig.h"
#include "IPCPlugin.h"
#include "IPCPreview.h"



struct IPCDomeView {
public:
  virtual int APIENTRY isFree(int width, int height, int format) = 0;
  virtual void * APIENTRY getBuffer() = 0;
  virtual void APIENTRY done() = 0;

  virtual IPCConfig  * APIENTRY getIPCConfig()  const = 0;   // main communication channel (mostly to pass keep alive signals)
  virtual IPCPlugin  * APIENTRY getIPCPlugin()  const = 0;   // Information that goes to domeview
  virtual IPCPreview * APIENTRY getIPCPreview() const = 0;   // Information that arrives from domeview

  virtual HRESULT APIENTRY errorCode() = 0;
  virtual void APIENTRY release() = 0;
};



typedef IPCDomeView* (APIENTRY *IPCDomeViewBuilderDummyFUNC)(); 
typedef IPCDomeView* (APIENTRY *IPCDomeViewBuilderFUNC)(int id); 
typedef IPCDomeView* (APIENTRY *IPCDomeViewBuilderSlotsFUNC)(int id, int slots); 



extern "C" DOMEVIEWAPI IPCDomeView* APIENTRY IPCDomeViewBuilderDummy();
extern "C" DOMEVIEWAPI IPCDomeView* APIENTRY IPCDomeViewBuilder(int id);
extern "C" DOMEVIEWAPI IPCDomeView* APIENTRY IPCDomeViewBuilderSlots(int id, int slots);


#endif
