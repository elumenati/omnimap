#include "OmniMapBase.h"
#include "ScreenRendererOGL.h"
#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER
#endif


ScreenRendererBase *ScreenRendererOGL::CreateScreenRendererOGL()
{
	return (new ScreenRendererOGL());
}


ScreenRendererOGL::ScreenRendererOGL()
{
	vboID = 0;
}



void ScreenRendererOGL::ResetGeometry(void *aVertexBuffer, ScreenGeometryBase::VertexType aVertexType, 
									  ScreenGeometryBase::ShapeType aShapeType,int *aVertexCounts, int aNumShapes)				 
{
	vertexCounts = aVertexCounts;
	numShapes = aNumShapes;
	vertexBuffer = aVertexBuffer;
	vertexType = aVertexType;
	shapeType = aShapeType;
	switch (aVertexType)
	{
	case ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT :
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
		vertexType = GL_V3F;
		break;
	case ScreenGeometryBase::VBT_VERTEX_TEXCOORD_FLT :
		vertexSize = sizeof(ScreenGeometryBase::TextureCoordVertex);
		vertexType = GL_T2F_V3F;
		break;
	default :
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
		vertexType = GL_V3F;
	}
}



void ScreenRendererOGL::Display(void *p)
{
  p;
	void *vp = vertexBuffer;
#ifdef VERTEX_BUFFER
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
#endif
	for (int i = 0; i < numShapes; i++)
	{
#ifdef VERTEX_BUFFER
		
		glInterleavedArrays( vertexType, 0, vp );
		//glVertexPointer( 3, GL_FLOAT, 0, vp );
#endif
		if (shapeType == ScreenGeometryBase::VBT_TRIANGLES)
#ifdef VERTEX_BUFFER
			glDrawArrays( GL_TRIANGLES, 0, vertexCounts[i]);
#else
		{
			ScreenGeometryBase::SimpleVertex *tp = (ScreenGeometryBase::SimpleVertex *) vp;
			glBegin(GL_TRIANGLES);
			for (int t = 0; t < vertexCounts[i]; t++)
			{
				glVertex3f(tp->x, tp->y, tp->z);
				tp++;
			}
			glEnd();
		}
#endif
		else
		{
#ifdef VERTEX_BUFFER
			glDrawArrays( GL_TRIANGLE_STRIP, 0, vertexCounts[i]);
#else
			ScreenGeometryBase::SimpleVertex *tp = (ScreenGeometryBase::SimpleVertex *) vp;
			glBegin(GL_TRIANGLE_STRIP);
			for (int t = 0; t < vertexCounts[i]; t++)
			{
				glVertex3f(tp->x, tp->y, tp->z);
				tp++;
			}
			glEnd();

			
#endif
		}

		vp = ((unsigned char *) vp) + (vertexCounts[i] * vertexSize);
		
	}
#ifdef VERTEX_BUFFER
	glPopClientAttrib();
#endif
}