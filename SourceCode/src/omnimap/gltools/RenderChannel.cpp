#include "RenderChannel.h"
#include <math.h>
#include "../omnimap.h"


	int RenderChannel::getTextureId()
	{
		if(!usepbuffer)
			return fbuffer->getOpenGL_TextureID(image_id);
		else
			return pbuffer->texture_object;
	}

RenderChannel::RenderChannel(//float horizontal_fov, float vertical_fov, float near_plane, float far_plane,
int _Resolution,
bool use_frame_buffer_object
							 
							 ) 
{
	Resolution = _Resolution;
	usepbuffer = !use_frame_buffer_object;

	//float deg_to_rads = 3.14159265f / 180.0f; 
	//float horizontal_fov_rads = horizontal_fov * deg_to_rads;
	//float vertical_fov_rads = vertical_fov * deg_to_rads;

	//frustum = new Frustum(horizontal_fov_rads, vertical_fov_rads, near_plane, far_plane);




	//pbuffer = new PBuffer(1024, 1024, 8, 24, 8, wglGetCurrentDC(), wglGetCurrentContext() );
	

	//image_id = fbuffer->CreateImageObject(1024, 1024, true, false);
	pbuffer = 0;
	fbuffer =0;
	if(usepbuffer )
	{
		pbuffer = new PBuffer(Resolution, Resolution, 8, 24, 8, wglGetCurrentDC(), wglGetCurrentContext());
	}
	else
	{	
		fbuffer = new FrameBufferObject();
		image_id = fbuffer->CreateImageObject(Resolution, Resolution, true, false);
		fbuffer->BindImageObjectToFrameBuffer( image_id );
		fbuffer->OutputImageObject(image_id);

	}



	//transform = new AffMatrix();

}

RenderChannel::~RenderChannel()
{
	delete fbuffer;
//	delete frustum;
//	delete transform;
	delete pbuffer;
}

/*
void
RenderChannel::setTransform(AffMatrix* _transform)
{
	delete transform;
	this->transform = _transform;
}

void 
RenderChannel::setFOVS(float horizontal_fov, float vertical_fov, float near_plane, float far_plane)
{
	float deg_to_rads = 3.14159265f / 180.0f; 
	float horizontal_fov_rads = horizontal_fov * deg_to_rads;
	float vertical_fov_rads = vertical_fov * deg_to_rads;

	frustum->set(horizontal_fov_rads, vertical_fov_rads, near_plane, far_plane);
}

void
RenderChannel::setFrustumAngles(float left_angle, float right_angle, float bottom_angle, float top_angle)
{
	frustum->setFrustumAngles(left_angle, right_angle, bottom_angle, top_angle);
	//frustum->printParameters();
}

void 
RenderChannel::setFrustumSides(float left, float right, float bottom, float top)
{
	frustum->setFrustumSides(left, right, bottom, top);
}*/

void 
RenderChannel::beginRenderToChannel()
{
	//Begin drawing to PBuffer

	if(usepbuffer)
	{
		pbuffer->activate();
		glEnable(GL_DEPTH_TEST);
	}
	else
		fbuffer->RenderToFrameBuffer();
/*
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	*/
	glPushAttrib(GL_VIEWPORT_BIT);
	if(usepbuffer)
		glViewport(0, 0,pbuffer->getWidth(),pbuffer->getHeight());
	else
		glViewport(0, 0, fbuffer->GetWidth(), fbuffer->GetHeight());

	//glEnable(GL_DEPTH_TEST);
}



void
RenderChannel::endRenderToChannel()
{
	glPopAttrib();
/*
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
*/
	if(usepbuffer)
		pbuffer->deactivate();
	else
		fbuffer->EndRenderToFrameBuffer();
}

/*
void
RenderChannel::set_GL_Matrices()
{
	//Set up the Projection Frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float frustum_val[6];
	frustum->returnParameters(frustum_val);
	glFrustum(frustum_val[0], frustum_val[1], frustum_val[2], frustum_val[3], frustum_val[4], frustum_val[5]);

	//Set up the ModelView transform (move the world into eye coordinates given this channel's position and orientation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(transform->returnValues());

}

void
RenderChannel::returnTransformValues(AffMatrix& mat)
{
	float frustum_val[6];
	frustum->returnParameters(frustum_val);
	AffMatrix proj;
	proj.setFrustumGL(frustum_val[0], frustum_val[1], frustum_val[2], frustum_val[3], frustum_val[4], frustum_val[5]);

	//Combine the two (remember they're transposed because of OpenGL, so do the multiply in opposite order MODELVIEW * PROJECTION)
	//AffMatrix* total = new AffMatrix(proj.preMultiply(*transform));

	mat = (*transform) * proj;
	//return total;
}
*/

void
RenderChannel::bindTexture()
{
	if(usepbuffer)
	pbuffer->bindTexture();
	else
	fbuffer->BindImageToTexture(image_id);
}


void
RenderChannel::unbindTexture()
{
	if(usepbuffer)
		pbuffer->unbindTexture();
	else
	{
	//	system("pause");
		// this function needs to be used very carfully
		// this function doesnt unbind, but rather disables
		fbuffer->DetachImageFromTexture(image_id);

	}
}
/*
void
RenderChannel::display()
{
	float frustum_val[6];
	frustum->returnParameters(frustum_val);
	//frustum->printParameters();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glMultMatrixf(transform->returnInverse().returnValues());

	float scale = -frustum_val[5] / -frustum_val[4];

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//Near Plane
	glBegin(GL_LINE_LOOP);
	glVertex3f(frustum_val[0], frustum_val[2], -frustum_val[4]);
	glVertex3f(frustum_val[1], frustum_val[2], -frustum_val[4]);
	glVertex3f(frustum_val[1], frustum_val[3], -frustum_val[4]);
	glVertex3f(frustum_val[0], frustum_val[3], -frustum_val[4]);
	glEnd();

	//Far Plane
	glBegin(GL_LINE_LOOP);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[3], -frustum_val[5]);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[3], -frustum_val[5]);
	glEnd();

	//Connectors
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[3], -frustum_val[5]);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[3], -frustum_val[5]);
	glEnd();


	glColor4f(0.0f, 0.5f, 1.0f, 0.25f);
	glBegin(GL_TRIANGLES);
	//left side
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[3], -frustum_val[5]);

	//top side
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[3], -frustum_val[5]);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[3], -frustum_val[5]);

	//right side
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[3], -frustum_val[5]);

	//bottom side
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale * frustum_val[0], scale * frustum_val[2], -frustum_val[5]);
	glVertex3f(scale * frustum_val[1], scale * frustum_val[2], -frustum_val[5]);
	glEnd();

	

	glPopMatrix();
}
*/