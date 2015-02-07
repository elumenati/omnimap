#include "RenderChannel.h"
#include <math.h>
#include "../omnimap.h"
#include "BackBufferTexture.h"


 bool useSpoutInRenderChannels = false;
	int RenderChannel::getTextureId()
	{
		if(!usepbuffer)
			return fbuffer->getOpenGL_TextureID(image_id);
#ifdef USE_PBUFFER
		else
			return pbuffer->texture_object;
#endif
	}

	unsigned int RenderChannel::getFrameBufferId()
	{
		return fbuffer->getOpenGL_FrameBufferID();
	}

	RenderChannel::RenderChannel(int _xResolution, int _yResolution,OmniMapChannelBase::TargetType _target ) 
{
	xResolution = _xResolution;
	yResolution = _yResolution;
	usepbuffer = (target == OmniMapChannelBase::PBUFFER);

#ifdef USE_PBUFFER
	pbuffer = 0;
#endif
	fbuffer =0;
	target = _target;
	switch (target)
	{
#ifdef USE_PBUFFER
	case OmniMapChannelBase::PBUFFER:
		{
			LogSystem()->ReportMessage("Using PBUFFER\n");
			pbuffer = new PBuffer(xResolution, yResolution, 8, 24, 8, wglGetCurrentDC(), wglGetCurrentContext());
			break;
			//HACK
			//pbuffer = new PBuffer(1280, 1024, 8, 24, 8, wglGetCurrentDC(), wglGetCurrentContext());
		}
#endif
	case OmniMapChannelBase::FRAME_BUFFER_OBJECT:
		{	
			fbuffer = new FrameBufferObject();
			image_id = fbuffer->CreateImageObject(xResolution, yResolution, true, false);
			//HACK
			//image_id = fbuffer->CreateImageObject(1280, 1024, true, false);
			fbuffer->BindImageObjectToFrameBuffer( image_id );
			fbuffer->OutputImageObject(image_id);
			break;
		}
	case OmniMapChannelBase::BACK_BUFFER:
		{
			backBuffer = new BackBufferTexture();
			backBuffer->CreateTexture(xResolution, yResolution);
			break;
		}
	default:
		break;
	}



	// BUILD SPOUT
	if(useSpoutInRenderChannels ){
			LogSystem()->ReportMessage("useSpoutInRenderChannels");
		omniSpout = new OmniSpout();
	}else{
		omniSpout =0;
	}
	//transform = new AffMatrix();

}


RenderChannel::RenderChannel(int _Resolution, bool use_frame_buffer_object ) 
{
	xResolution = yResolution = _Resolution;
	usepbuffer = !use_frame_buffer_object;
#ifdef USE_PBUFFER
	pbuffer = 0;
#endif
	fbuffer =0;
#ifdef USE_PBUFFER
	if(usepbuffer )
	{
		target = OmniMapChannelBase::PBUFFER;
		pbuffer = new PBuffer(xResolution, yResolution, 8, 24, 8, wglGetCurrentDC(), wglGetCurrentContext());
		//HACK
		//pbuffer = new PBuffer(1280, 1024, 8, 24, 8, wglGetCurrentDC(), wglGetCurrentContext());
	}
	else
#endif
	{	
		target = OmniMapChannelBase::FRAME_BUFFER_OBJECT;
		fbuffer = new FrameBufferObject();
		image_id = fbuffer->CreateImageObject(xResolution, yResolution, true, false);
		//HACK
		//image_id = fbuffer->CreateImageObject(1280, 1024, true, false);
		fbuffer->BindImageObjectToFrameBuffer( image_id );
		fbuffer->OutputImageObject(image_id);

	}



	//transform = new AffMatrix();

	// BUILD SPOUT
	if(useSpoutInRenderChannels ){
			LogSystem()->ReportMessage("useSpoutInRenderChannels");
		omniSpout = new OmniSpout();
	}else{
		omniSpout =0;
	}
}

RenderChannel::~RenderChannel()
{
	if(omniSpout!=0){
		delete(omniSpout);
		omniSpout = 0;
	}
	if (fbuffer)
		delete fbuffer;
//	delete frustum;
//	delete transform;
#ifdef USE_PBUFFER
	if (pbuffer)
		delete pbuffer;
#endif
}



void 
RenderChannel::beginRenderToChannel()
{
	//Begin drawing to PBuffer
#ifdef USE_PBUFFER
	if(target == OmniMapChannelBase::PBUFFER)
	{
		pbuffer->activate();
		glEnable(GL_DEPTH_TEST);
	} else 
#endif
	if (target == OmniMapChannelBase::FRAME_BUFFER_OBJECT)
	{
		fbuffer->RenderToFrameBuffer();
	}

	glPushAttrib(GL_VIEWPORT_BIT);
#ifdef USE_PBUFFER	
	if(target == OmniMapChannelBase::PBUFFER)
		glViewport(0, 0,pbuffer->getWidth(),pbuffer->getHeight());

	else 
#endif
	if (target == OmniMapChannelBase::FRAME_BUFFER_OBJECT)
		glViewport(0, 0, fbuffer->GetWidth(), fbuffer->GetHeight());
	if (target == OmniMapChannelBase::BACK_BUFFER)
		backBuffer->setViewport();

	//glEnable(GL_DEPTH_TEST);
}



void
RenderChannel::endRenderToChannel()
{
	glPopAttrib();
#ifdef USE_PBUFFER
	if(target == OmniMapChannelBase::PBUFFER)
		pbuffer->deactivate();
	else 
#endif
	if (target == OmniMapChannelBase::FRAME_BUFFER_OBJECT)
		fbuffer->EndRenderToFrameBuffer();
	else if (target == OmniMapChannelBase::BACK_BUFFER)
		backBuffer->CopyFB2Texture();
}

void RenderChannel::UpdateSpout(){
	if(omniSpout!=0){
		if(target == OmniMapChannelBase::FRAME_BUFFER_OBJECT){
			omniSpout->Send(fbuffer->getOpenGL_TextureID(0),fbuffer->GetWidth(),fbuffer->GetHeight());
		} else if(target == OmniMapChannelBase::PBUFFER){
			LogSystem()->ReportMessage("Testing PBUFFER\n");
			omniSpout->Send(pbuffer->texture_object,pbuffer->getWidth(),pbuffer->getHeight());
		} else if (target == OmniMapChannelBase::BACK_BUFFER){

			LogSystem()->ReportMessage("BACK_BUFFER not yet supported\n");
		}
	}
}

void
RenderChannel::bindTexture()
{
#ifdef USE_PBUFFER
	if(target == OmniMapChannelBase::PBUFFER)
		pbuffer->bindTexture();
	else 
#endif
	if (target == OmniMapChannelBase::FRAME_BUFFER_OBJECT)
		fbuffer->BindImageToTexture(image_id);
	else if (target == OmniMapChannelBase::BACK_BUFFER)
		backBuffer->BindTexture();

	
}


void
RenderChannel::unbindTexture()
{
#ifdef USE_PBUFFER
	if(target == OmniMapChannelBase::PBUFFER)
		pbuffer->unbindTexture();
	else 
#endif
	if (target == OmniMapChannelBase::FRAME_BUFFER_OBJECT)
	{
		fbuffer->DetachImageFromTexture(image_id);
	} else if (target == OmniMapChannelBase::BACK_BUFFER)
		backBuffer->UnBindTexture();

}
