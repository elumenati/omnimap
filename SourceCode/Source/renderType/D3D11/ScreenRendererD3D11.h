#ifndef __SCREEN_RENDERERD3D11__H
#define __SCREEN_RENDERERD3D11__H

#include <dxgi.h>
#include <d3dx11.h>
#include <dxerr.h>
#include "ScreenGeometryBase.h"

class OmniMapShaderD3D11;

class ScreenRendererD3D11 : public ScreenRendererBase
{
public :
	ScreenRendererD3D11::ScreenRendererD3D11();
	ScreenRendererD3D11::~ScreenRendererD3D11();

	static ScreenRendererBase *ScreenRendererD3D11::CreateScreenRendererD3D11();
	void ResetGeometry(void *vertexBuffer, ScreenGeometryBase::VertexType aVertexType, ScreenGeometryBase::ShapeType aShapeType, int *aVertexCount, int numShapes);
	void Display(void *context);
	void setOMShader(OmniMapShaderD3D11 *shader);

private :
	unsigned int vertexType;
	unsigned int vertexSize;
	ID3D11Buffer **g_pVertexBuffers;
	HRESULT CreateVertBuffers(ID3D11DeviceContext* pDevContext11, void* pVertices, int *NumVertices, int numShapes);
	ID3D11InputLayout* d3d11Layout;
	OmniMapShaderD3D11* omShader;

  UINT *vertexBufferStrides;
  UINT *vertexBufferOffsets;
};

#endif