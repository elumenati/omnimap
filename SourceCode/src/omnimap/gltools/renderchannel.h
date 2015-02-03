#ifndef RENDER_CHANNEL_H
#define RENDER_CHANNEL_H

#include "FrameBufferObject.h"
#include "../GraphicsPrimitives.h"
#include "PBuffer.h"
//#include "GraphicsTools/Frustum.h"



class RenderChannel {

public:

	//Creates a default render channel given a horizontal & vertical field of view, near and far plane distances
	//FOVs are in degrees
	RenderChannel(//float horizontal_fov, float vertical_fov, float near_plane, float far_plane,
		
		int _Resolution = 512,
		bool use_frame_buffer_object = true
		
		);
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
	PBuffer* pbuffer;
	FrameBufferObject* fbuffer;
	int RenderChannel::getTextureId();
private:
	int Resolution ;
	bool usepbuffer ;
	int image_id;

};

#endif