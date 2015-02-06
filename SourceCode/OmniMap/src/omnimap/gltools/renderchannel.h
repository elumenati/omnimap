#ifndef RENDER_CHANNEL_H
#define RENDER_CHANNEL_H

#ifdef _WINDOWS
#include <windows.h>
#endif

#include "FrameBufferObject.h"
#include "../GraphicsPrimitives.h"
#include "PBuffer.h"
#include "OmniMapBase.h"
//#include "GraphicsTools/Frustum.h"

class BackBufferTexture;

#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API RenderChannel {
#else
class RenderChannel {
#endif

public:

	

	//Creates a default render channel given a horizontal & vertical field of view, near and far plane distances
	//FOVs are in degrees
	RenderChannel(int _Resolution = 512, bool use_frame_buffer_object = true);

	RenderChannel(int _xResolution, int _yResolution, 
		OmniMapChannelBase::TargetType targetType = OmniMapChannelBase::FRAME_BUFFER_OBJECT);
	~RenderChannel();

	//Set the local transform of this render channel face 
	//void setTransform(AffMatrix* transform);

	//set the FOVs for the RenderChannel's frustum
//	void setFOVS(float horizontal_fov, float vertical_fov, float near_plane, float far_plane);
//	void setFrustumAngles(float left_angle, float right_angle, float bottom_angle, float top_angle);
//	void setFrustumSides(float left, float right, float bottom, float top);

	

	//Load identities and set up the Render Channels Projection and Modelview Matrices
	//void set_GL_Matrices();




//	void returnTransformValues(AffMatrix& mat);

//	AffMatrix* transform;


	void display();


		//Prepare this channel to be rendered to
	void beginRenderToChannel();
	//End rendering to this channel
	void endRenderToChannel();
	//Bind this Channel to texture
	void bindTexture();

	//Unbind this channel from texture
	void unbindTexture();
#ifdef USE_PBUFFER
	PBuffer* pbuffer;
#endif
	FrameBufferObject* fbuffer;
	int getTextureId();
	unsigned int getFrameBufferId();
private:
	int xResolution ;
	int yResolution ;
	bool usepbuffer ;
	int image_id;
	BackBufferTexture *backBuffer;
	OmniMapChannelBase::TargetType target;

};

#endif