#ifndef __SCREEN_RENDEREROGL__H
#define __SCREEN_RENDEREROGL__H


#ifdef WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#ifdef __APPLE__
#include "OmniMapAPI/ScreenGeometryBase.h"
#else
#include "ScreenGeometryBase.h"
#endif

#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API ScreenRendererOGL : public ScreenRendererBase
#else
class ScreenRendererOGL : public ScreenRendererBase
#endif
{
public :
	ScreenRendererOGL();
    ~ScreenRendererOGL() {};

	static ScreenRendererBase *CreateScreenRendererOGL();
	void ResetGeometry(void *vertexBuffer,
		ScreenGeometryBase::VertexType aVertexType, ScreenGeometryBase::ShapeType shapeType, int *aVertexCounts,
		int aNumShapes);
	
	void Display(void *context);

private :
	unsigned int vertexSize;
	GLint displayList;
	unsigned int vertexType;
	unsigned int vboID;
	int texCoordStride, colorStride, vertexStride;
};

#endif