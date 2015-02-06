#ifndef __SCREEN_RENDERERD3D__H
#define __SCREEN_RENDERERD3D__H

#include <dxgi.h>
#include <d3d9types.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <dxerr.h>
#include "ScreenGeometryBase.h"

class OmniMapShaderD3D10;

class ScreenRendererD3D10 : public ScreenRendererBase
{
public :
	ScreenRendererD3D10::ScreenRendererD3D10();
	ScreenRendererD3D10::~ScreenRendererD3D10();

	static ScreenRendererBase *ScreenRendererD3D10::CreateScreenRendererD3D10();
	void ResetGeometry(void *vertexBuffer,
		ScreenGeometryBase::VertexType aVertexType, ScreenGeometryBase::ShapeType aShapeType, int *aVertexCount,
		int numShapes);
	
	void Display(void *context);

	void setOMShader(OmniMapShaderD3D10 *shader);

private :

	unsigned int vertexType;
	unsigned int vertexSize;
	ID3D10Buffer **g_pVertexBuffers;
	HRESULT CreateVertBuffers(ID3D10Device* pDev10, void* pVertices, 
										  int *NumVertices, int numShapes);
	ID3D10InputLayout*      d3d10Layout;
	OmniMapShaderD3D10 *omShader;
};

#endif