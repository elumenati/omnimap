#ifndef __SCREEN_RENDERERD3D__H
#define __SCREEN_RENDERERD3D__H

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "ScreenGeometryBase.h"

class ScreenRendererD3D : public ScreenRendererBase
{
public :
	ScreenRendererD3D::ScreenRendererD3D();
	ScreenRendererD3D::~ScreenRendererD3D();

	static ScreenRendererBase *ScreenRendererD3D::CreateScreenRendererD3D();
	void ResetGeometry(void *vertexBuffer,
		ScreenGeometryBase::VertexType aVertexType, ScreenGeometryBase::ShapeType aShapeType, int *aVertexCount,
		int numShapes);
	
	void Display(void *context);

private :

	unsigned int vertexType;
	unsigned int vertexSize;
	LPDIRECT3DVERTEXBUFFER9 *g_pVertexBuffers;
	HRESULT CreateVertBuffers(IDirect3DDevice9* pDev9, void* pVertices, 
										  int *NumVertices, int numShapes);
};

#endif