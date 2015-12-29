#pragma once

#include "omnimapbase.h"


#include "SpoutSDK/Source/Spout.h"    // Spout SDK
#include "OmniSpout.h"





class OmniSpout
{
public:
  SpoutSender *spoutsender  ;        // A sender object
  bool bInitialized;                // Initialization result
  bool bMemoryShare;                // Texture share compatibility
  static int fbindex;
  bool needsNewName;

  OmniSpout(char * name);
  ~OmniSpout();

  void Send(GLuint  texID, int width, int height);
  void UpdateSpoutGL(int width, int height);
  void InitGLtexture(GLuint &texID, unsigned int width, unsigned int height);
  void SetSenderName(const char *newsenderName);

private:
  GLuint sendertexture;  // Local OpenGL texture used for sharing main view
  int sendertextureW;
  int sendertextureH;
  char sendername[256];            // Shared memory name
};
