#pragma once

#include "../omnimap.h"


#include "Spout.h"    // Spout SDK
#include "OmniSpout.h"





class OmniSpout
{
public:
    SpoutSender *spoutsender  ;        // A sender object
    char sendername[256];            // Shared memory name
    GLuint sendertexture;            // Local OpenGL texture used for sharing
    bool bInitialized;                // Initialization result
    bool bMemoryShare;                // Texture share compatibility
static int fbindex;
	OmniSpout(){
		LogSystem()->ReportMessage("Building Spout");
		LogSystem()->ReportError("Building Spout");
		spoutsender = new SpoutSender;        // A sender object
		sendername[0]=0;            // Shared memory name
		sendertexture =0;            // Local OpenGL texture used for sharing
		bInitialized=false;                // Initialization result
		bMemoryShare=false;
		sprintf(sendername,"clement%d",fbindex++);
	}

	~OmniSpout(){
		spoutsender->ReleaseSender();
		spoutsender=0;
	}

	void Send(GLuint  texID, int width, int height){

		if(!bInitialized) {
			char buff[512];
			sprintf(buff,"spout init %s %d %d", sendername, width, height);
			bInitialized = spoutsender->CreateSender(sendername, width, height);
			LogSystem()->ReportMessage(buff);
			// Detect texture share compatibility (optional)
			bMemoryShare = spoutsender->GetMemoryShareMode();
		}
		if(bInitialized) {
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
			spoutsender->SendTexture(texID, GL_TEXTURE_2D, width, height);
		}
	}
};
