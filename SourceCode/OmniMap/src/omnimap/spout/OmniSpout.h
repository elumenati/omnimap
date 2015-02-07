#pragma once

#include "../omnimap.h"


#include "Spout.h"    // Spout SDK
#include "OmniSpout.h"





class OmniSpout
{
public:
	SpoutSender *spoutsender  ;        // A sender object
	char sendername[256];            // Shared memory name
	bool bInitialized;                // Initialization result
	bool bMemoryShare;                // Texture share compatibility
	static int fbindex;
	OmniSpout(char * name){
		LogSystem()->ReportMessage("Building Spout");
		LogSystem()->ReportError("Building Spout");
		spoutsender = new SpoutSender;        // A sender object
		sendername[0]=0;            // Shared memory name
		sendertexture =0;            // Local OpenGL texture used for sharing
		sendertextureW=0;
		sendertextureH=0;
		bInitialized=false;                // Initialization result
		bMemoryShare=false;
		sprintf(sendername,"clement%d",fbindex++);
	}

	~OmniSpout(){
		spoutsender->ReleaseSender();
		spoutsender=0;
		if(sendertexture != 0) {
			glDeleteTextures(1, &sendertexture);
			sendertexture=0;
		}

	}

	void SetName(char *name){
		sprintf(sendername,name);
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
	GLuint sendertexture;  // Local OpenGL texture used for sharing main view
	int sendertextureW;
	int sendertextureH;
	void UpdateSpoutGL(int width, int height){
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

	void InitGLtexture(GLuint &texID, unsigned int width, unsigned int height)
	{
		if(texID != 0) glDeleteTextures(1, &texID);

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
