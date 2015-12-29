#include "OmniSpout.h"
#include "OM_ErrorHandling.h"



int OmniSpout::fbindex=0;



OmniSpout::OmniSpout(char * name){
  EH_DECLARE;
  EH_Log("Building Spout");
  spoutsender = NULL;          // A sender object
  sendername[0]=0;            // Shared memory name
  sendertexture =0;            // Local OpenGL texture used for sharing
  sendertextureW=0;
  sendertextureH=0;
  bInitialized=false;                // Initialization result
  bMemoryShare=false;
  if(fbindex ==0){
    sprintf(sendername,"%s",name);
  }else{
    sprintf(sendername,"%s%d",name,fbindex);
  }
  needsNewName=true;
  fbindex++;
}




OmniSpout::~OmniSpout(){
  if (bInitialized && spoutsender) spoutsender->ReleaseSender();
  if (spoutsender) { delete spoutsender; spoutsender=NULL; }
  if (sendertexture) {
    glDeleteTextures(1, &sendertexture);
    sendertexture=0;
  }
}




void OmniSpout::Send(GLuint  texID, int width, int height){
  EH_DECLARE;

  if(!bInitialized) {
    EH_Log("spout init %s %d %d", sendername, width, height);
    EH_Ptr(spoutsender = new SpoutSender);        
    EH_Zero(bInitialized = spoutsender->CreateSender(sendername, width, height));
    // Detect texture share compatibility (optional)
    bMemoryShare = spoutsender->GetMemoryShareMode();
  }

    // Send the texture out for all receivers to use
    //
    // Notes :
    // (1)    If a host calls SendTexture with a framebuffer object bound,
    //        include the FBO id in the SendTexture call so that the binding is restored
    //        afterwards because Spout makes use of its own FBO for intermediate rendering.
    // (2)    openGL/DirectX coordinates make our texture come out inverted so the texture
    //        is inverted    when transferring it to the shared texture. You can specify false
    //        to disable this default then the result comes out apparently inverted.
    //
  EH_Zero(spoutsender->SendTexture(texID, GL_TEXTURE_2D, width, height));

  EH_OnError() {
    if (bInitialized && spoutsender) spoutsender->ReleaseSender();
    if (spoutsender) { delete spoutsender; spoutsender=NULL; }
    bInitialized = false;
  }
}



void OmniSpout::UpdateSpoutGL(int width, int height){
  if(width!=sendertextureW||height!=sendertextureH){
    sendertextureH=height;
    sendertextureW=width;
    InitGLtexture(sendertexture,width,height);
  }
  // push
  int prevTextureBinding;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTextureBinding);

  // do 
  glBindTexture(GL_TEXTURE_2D, sendertexture); 
  glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);

  // pop
  glBindTexture(GL_TEXTURE_2D, prevTextureBinding); 

  Send(sendertexture,width, height);
}



void OmniSpout::InitGLtexture(GLuint &texID, unsigned int width, unsigned int height)
{
  if(texID != 0) { glDeleteTextures(1, &texID); texID = 0; }

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}




void OmniSpout::SetSenderName(const char *newsenderName){
  sprintf(sendername,"%s",newsenderName);
  needsNewName = false;
}


