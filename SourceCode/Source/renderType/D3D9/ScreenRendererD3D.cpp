
#include "D3D/dxstdafx.h"
#include "OmniMapBase.h"
#include "ScreenRendererD3D.h"
#include "OM_ErrorHandling.h"


#define D3DFVF_XYZ_VERTEX_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_XYZ_VERTEX (D3DFVF_XYZ)



ScreenRendererBase *ScreenRendererD3D::CreateScreenRendererD3D()
{
	return (new ScreenRendererD3D());
}


static void clearVertexBuffer(LPDIRECT3DVERTEXBUFFER9 **vBuffer, int numShapes) {
  if ((*vBuffer) != NULL) {
    for (int i = 0; i < numShapes; i++) SAFE_RELEASE((*vBuffer)[i]);
    delete[] (*vBuffer);
    (*vBuffer) = NULL;
  }
}



ScreenRendererD3D::ScreenRendererD3D()
{
  vertexBuffer = NULL;
  g_pVertexBuffers = NULL;
  vertexType = 0;
  vertexSize = 0;
}




ScreenRendererD3D::~ScreenRendererD3D()
{
  clearVertexBuffer(&g_pVertexBuffers, numShapes);
}



void ScreenRendererD3D::ResetGeometry(void *aVertexBuffer, ScreenGeometryBase::VertexType aVertexType, 
                                      ScreenGeometryBase::ShapeType aShapeType, int *aVertexCounts, int aNumShapes)
{
  clearVertexBuffer(&g_pVertexBuffers, numShapes);

  vertexCounts = aVertexCounts;
  vertexBuffer = aVertexBuffer;
  numShapes = aNumShapes;
  vertexType = aVertexType;
  shapeType = aShapeType;

  switch (aVertexType)
  {
  case ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT :
    vertexType = D3DFVF_XYZ_VERTEX;
    vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
    break;
  default :
    vertexType = D3DFVF_XYZ_VERTEX;
    vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
    break;
  }
}



void ScreenRendererD3D::Display(void *p)
{
	IDirect3DDevice9 *d3dDevice = (IDirect3DDevice9 *) p;

	if (g_pVertexBuffers == NULL) CreateVertBuffers(d3dDevice, vertexBuffer,  vertexCounts, numShapes);
	
  d3dDevice->SetFVF( vertexType );

	for (int i = 0; i < numShapes; i++)
	{
		d3dDevice->SetStreamSource( 0, g_pVertexBuffers[i], 0, vertexSize );
		if (shapeType == ScreenGeometryBase::VBT_TRIANGLES)
			d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, vertexCounts[i]/3);
		else
			d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, vertexCounts[i]-2);
	}
}




HRESULT ScreenRendererD3D::CreateVertBuffers(IDirect3DDevice9* pDev9, void* pVertices, 
										  int *NumVertices, int numShapes)
{
  int bufferSize = 0;
  unsigned char *verts = NULL;
  EH_DECLARE;

	EH_Ptr(g_pVertexBuffers = new LPDIRECT3DVERTEXBUFFER9[numShapes]);

  memset(g_pVertexBuffers, 0, sizeof(LPDIRECT3DVERTEXBUFFER9) * numShapes);

  for (int i = 0; i < numShapes; i++)
	{
		if (vertexType == D3DFVF_XYZ_VERTEX)
		{
			bufferSize = vertexSize * NumVertices[i];
			EH_Test(pDev9->CreateVertexBuffer( bufferSize,
												  D3DUSAGE_WRITEONLY,
												  vertexType,
												  D3DPOOL_DEFAULT,
												  &g_pVertexBuffers[i],
												  NULL ));

      EH_Test(g_pVertexBuffers[i]->Lock( 0, 0, (void**)&verts, 0 ));
      for (int j = 0; j < bufferSize; j++)
        verts[j] = ((unsigned char *) pVertices)[j];
      g_pVertexBuffers[i]->Unlock();
		}

		pVertices = ((unsigned char *) pVertices) + (vertexCounts[i]*vertexSize);
	}

  EH_OnError() {
    clearVertexBuffer(&g_pVertexBuffers, numShapes);
  }

	return EH_ERRORCODE;
}