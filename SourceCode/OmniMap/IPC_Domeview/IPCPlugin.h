#pragma once
#ifndef __IPCPLUGIN_H__
#define __IPCPLUGIN_H__



#include "IPCConfig.h"



#define IPC_DomeViewPlugin_VideoFlag        0x01000000
#define IPC_DomeViewPlugin_MsnFlag          0x02000000   // PLEASE DO NOT USE THIS YET! MESSENGER STILL IN BETA :D
#define IPC_DomeViewPlugin_OmniMapFlag      0x04000000


#define IPC_DomeViewPlugin_Video(I)         (I|IPC_DomeViewPlugin_VideoFlag)
#define IPC_DomeViewPlugin_Msn(I)           (I|IPC_DomeViewPlugin_MsnFlag)   // PLEASE DO NOT USE THIS YET! MESSENGER STILL IN BETA :D


enum {
  IPC_DomeViewPlugin_WebCam = 0,
  IPC_DomeViewPlugin_DVD,
  IPC_DomeViewPlugin_AfterEffects,
  IPC_DomeViewPlugin_PhotoShop,
  IPC_DomeViewPlugin_Premiere,
  IPC_DomeViewPlugin_Celestia,
  IPC_DomeViewPlugin_Stellarium,
  IPC_DomeViewPlugin_Cinema4D,
  IPC_DomeViewPlugin_Maya,
  IPC_DomeViewPlugin_3DStudio,
  IPC_DomeViewPlugin_Quake3Arena
};



enum {
  IPC_DomeViewPlugin_OmniMap_UnrealTournament = IPC_DomeViewPlugin_OmniMapFlag,
  IPC_DomeViewPlugin_OmniMap_Multigen,
  IPC_DomeViewPlugin_OmniMap_FlightSims,
  IPC_DomeViewPlugin_OmniMap_GeoFusion,
  IPC_DomeViewPlugin_OmniMap_SecondLife,
  IPC_DomeViewPlugin_OmniMap_NatureView
  // Each program/plugin must have a unique id
  // I've added some programs, feel free to add more!
};


#define IPC_DomeViewPlugin_Dummy            0x00FFFFFF    // USE THIS DEFINE TO DO TESTS!





enum {
  NO_MIRROR = 0,
  MIRROR_X = 1,
  MIRROR_Y = 2
};


#define FormatFlag_8Bits  0x00000001
#define FormatFlag_16Bits 0x00000002
#define FormatFlag_24Bits 0x00000003
#define FormatFlag_32Bits 0x00000004
#define FormatFlag_RGB    0x10000000
#define FormatFlag_YUV    0x20000000
#define FormatFlag_Swap   0x40000000
#define FormatFlag_X8     0x00000010
#define FormatFlag_Y8     0x00000100
#define FormatFlag_Z8     0x00001000
#define FormatFlag_A8     0x00010000

#define Format_RGB  (FormatFlag_RGB | FormatFlag_24Bits | FormatFlag_X8 | FormatFlag_Y8 | FormatFlag_Z8)
#define Format_RGBA (FormatFlag_RGB | FormatFlag_32Bits | FormatFlag_X8 | FormatFlag_Y8 | FormatFlag_Z8 | FormatFlag_A8)
#define Format_BGR  (FormatFlag_RGB | FormatFlag_24Bits | FormatFlag_X8 | FormatFlag_Y8 | FormatFlag_Z8 | FormatFlag_Swap)
#define Format_BGRA (FormatFlag_RGB | FormatFlag_32Bits | FormatFlag_X8 | FormatFlag_Y8 | FormatFlag_Z8 | FormatFlag_A8 | FormatFlag_Swap)
#define Format_YUV  (FormatFlag_YUV | FormatFlag_24Bits | FormatFlag_X8 | FormatFlag_Y8 | FormatFlag_Z8)

#define FormatMask_Depth  0x0000000F


enum {
  PARAMS_EMPTY = 0,         //  Ignore parameters passed by IPC channel (Plugin Window widgets will be active!)

  PARAMS_FILL = 1,          //  Using this option Plugin Window will have all it's widgets gray out! (parameters must be filled correctly by caller!)
                            //  Widgets will reflect parameters values!

  PARAMS_FILL_DISCARD = 2   //  Using this option Plugin Window will have all it's widgets gray out! (parameters must be filled correctly by caller!)
                            //  Widgets will *NOT* reflect parameters values!
};




enum {
  PROJECTION_None = 0,
  PROJECTION_AzimuthalEquidistant = 1,
  PROJECTION_AzimuthalEquidistantOffAxis,
  PROJECTION_Aitoff,
  PROJECTION_HammerAitoff,
  PROJECTION_Gnomonic,
  PROJECTION_Stereographic,
  PROJECTION_Orthographic,
  PROJECTION_VerticalPerspective,
  PROJECTION_CylindricalEquidistant,
  PROJECTION_CylindricalEqualArea,
  PROJECTION_CenterCylindrical,
  PROJECTION_PanoramicEquidistant,
  PROJECTION_PanoramicEqualArea,
  PROJECTION_CenterPanoramic,
  PROJECTION_Total
};








struct IPCPlugin {
public:
  virtual void APIENTRY resetParams() = 0;
  virtual int APIENTRY newParamsChanged() = 0;
  virtual int APIENTRY isServerOnline() = 0;
  virtual int APIENTRY isClientOnline() = 0;

  virtual int APIENTRY readIsUsed() = 0;
  virtual void APIENTRY readParams() = 0;
  virtual void APIENTRY forceReadParams() = 0;
  virtual void APIENTRY readBuffer(void *buffer, int size) = 0;
  virtual void * APIENTRY getReadBuffer() = 0;
  virtual void APIENTRY readEnd() = 0;
  virtual void APIENTRY readReopen(IPCConfig *cfg) = 0;

  virtual int APIENTRY writeIsFree() = 0;
  virtual void APIENTRY writeParams() = 0;
  virtual void APIENTRY forceWriteParams() = 0;
  virtual void APIENTRY writeBuffer(void *buffer, int size) = 0;
  virtual void *APIENTRY getWriteBuffer() = 0;
  virtual void APIENTRY writeEnd() = 0;
  virtual void APIENTRY writeReopen(IPCConfig *cfg) = 0;

  virtual int APIENTRY getVersion() const = 0;
  virtual int APIENTRY getParamsSize() const = 0;
  virtual int APIENTRY getWidth() const = 0;
  virtual int APIENTRY getHeight() const = 0;
  virtual int APIENTRY getFormat() const = 0;
  virtual int APIENTRY getParamsFilled() const = 0;
  virtual double APIENTRY getAlt() const = 0;
  virtual double APIENTRY getAzi() const = 0;
  virtual double APIENTRY getRotation() const = 0;
  virtual int APIENTRY getMirror() const = 0;
  virtual double APIENTRY getAngularWidth() const = 0;
  virtual double APIENTRY getAngularHeight() const = 0;
  virtual int APIENTRY getProjection() const = 0;
  virtual int APIENTRY getOptimalSphericalAngle() const = 0;
  virtual int APIENTRY getOptimalEscape() const = 0;
  virtual double APIENTRY getEscape() const = 0;
  virtual double APIENTRY getApertureAngle() const = 0;
  virtual double APIENTRY getSphericalAngle() const = 0;
  virtual double APIENTRY getCylindricalSphericalAngle() const = 0;

  virtual void APIENTRY setVersion(int version) = 0;
  virtual void APIENTRY setParamsSize(int paramsSize) = 0;
  virtual void APIENTRY setWidth(int width) = 0;
  virtual void APIENTRY setHeight(int height) = 0;
  virtual void APIENTRY setFormat(int format) = 0;
  virtual void APIENTRY setParamsFilled(int paramsFilled) = 0;
  virtual void APIENTRY setAlt(double alt) = 0;
  virtual void APIENTRY setAzi(double azi) = 0;
  virtual void APIENTRY setRotation(double rotation) = 0;
  virtual void APIENTRY setMirror(int mirror) = 0;
  virtual void APIENTRY setAngularWidth(double angularWidth) = 0;
  virtual void APIENTRY setAngularHeight(double angularHeight) = 0;
  virtual void APIENTRY setProjection(int projection) = 0;
  virtual void APIENTRY setOptimalSphericalAngle(int optimalSphericalAngle) = 0;
  virtual void APIENTRY setOptimalEscape(int optimalEscape) = 0;
  virtual void APIENTRY setEscape(double escape) = 0;
  virtual void APIENTRY setApertureAngle(double apertureAngle) = 0;
  virtual void APIENTRY setSphericalAngle(double sphericalAngle) = 0;
  virtual void APIENTRY setCylindricalSphericalAngle(double cylindricalSphericalAngle) = 0;

  virtual HRESULT APIENTRY errorCode() = 0;
  virtual void APIENTRY release() = 0;
};



typedef IPCPlugin* (APIENTRY *IPCPluginBuilderFUNC)(); 
typedef IPCPlugin* (APIENTRY *IPCPluginBuilderServerFUNC)(IPCConfig *cfg); 
typedef IPCPlugin* (APIENTRY *IPCPluginBuilderClientFUNC)(IPCConfig *cfg); 



extern "C" DOMEVIEWAPI IPCPlugin* APIENTRY IPCPluginBuilder();
extern "C" DOMEVIEWAPI IPCPlugin* APIENTRY IPCPluginBuilderServer(IPCConfig *cfg);
extern "C" DOMEVIEWAPI IPCPlugin* APIENTRY IPCPluginBuilderClient(IPCConfig *cfg);


#endif
