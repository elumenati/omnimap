
#include "OmniMapBase.h"
#include "ScreenRendererD3D.h"
#include "D3D/dxstdafx.h"

#define D3DFVF_XYZ_VERTEX_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_XYZ_VERTEX (D3DFVF_XYZ)

ScreenRendererBase *ScreenRendererD3D::CreateScreenRendererD3D()
{
	return (new ScreenRendererD3D());
}

ScreenRendererD3D::ScreenRendererD3D()
{
	vertexBuffer = NULL;
	g_pVertexBuffers = NULL;
}

ScreenRendererD3D::~ScreenRendererD3D()
{
	if (g_pVertexBuffers != NULL)
	{
		for (int i = 0; i < numShapes; i++)
			SAFE_RELEASE(g_pVertexBuffers[i]);
	}
}

void ScreenRendererD3D::ResetGeometry(void *aVertexBuffer, ScreenGeometryBase::VertexType aVertexType, 
									  ScreenGeometryBase::ShapeType aShapeType, int *aVertexCounts, int aNumShapes)
{
	if (g_pVertexBuffers != NULL)
	{
		for (int i = 0; i < numShapes; i++)
			SAFE_RELEASE(g_pVertexBuffers[i]);
	}

	vertexCounts = aVertexCounts;
	vertexBuffer = aVertexBuffer;
	numShapes = aNumShapes;
	vertexType = aVertexType;
	shapeType = aShapeType;
	
	
	g_pVertexBuffers = NULL;
	
	switch (aVertexType)
	{
	case ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT :
		vertexType = D3DFVF_XYZ_VERTEX;
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
		break;
	default :
		vertexType = D3DFVF_XYZ_VERTEX;
	}
}

void ScreenRendererD3D::Display(void *p)
{
	IDirect3DDevice9 *d3dDevice = (IDirect3DDevice9 *) p;
	if (g_pVertexBuffers == NULL)
	{
		CreateVertBuffers(d3dDevice, vertexBuffer,  vertexCounts, numShapes);
	}
	
	for (int i = 0; i < numShapes; i++)
	{
		d3dDevice->SetStreamSource( 0, g_pVertexBuffers[i], 0, vertexSize );
			d3dDevice->SetFVF( vertexType );
		if (shapeType == ScreenGeometryBase::VBT_TRIANGLES)
			d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, vertexCounts[i]/3);
		else
			d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, vertexCounts[i]-2);
	}
}


HRESULT ScreenRendererD3D::CreateVertBuffers(IDirect3DDevice9* pDev9, void* pVertices, 
										  int *NumVertices, int numShapes)
{
	HRESULT hr = 0;
	int bufferSize;
	g_pVertexBuffers = new LPDIRECT3DVERTEXBUFFER9[numShapes];
	for (int i = 0; i < numShapes; i++)
	{
		if (vertexType == D3DFVF_XYZ_VERTEX)
		{
			bufferSize = vertexSize * NumVertices[i];
			hr = pDev9->CreateVertexBuffer( bufferSize,
												  D3DUSAGE_WRITEONLY,
												  vertexType,
												  D3DPOOL_DEFAULT,
												  &g_pVertexBuffers[i],
												  NULL );
			if( FAILED( hr ) )

			{
				// TO DO: Respond to failure of CreateVertexBuffer
				return hr;
			}
		}
		unsigned char *verts = NULL;

		if( FAILED( g_pVertexBuffers[i]->Lock( 0, 0, (void**)&verts, 0 ) ) )
		{
			// TO DO: Respond to the failure of calling Lock on our Vertex Buffer
			return hr;
		}
		else
		{
			for (int j = 0; j < bufferSize; j++)
				verts[j] = ((unsigned char *) pVertices)[j];
			g_pVertexBuffers[i]->Unlock();
		}
		pVertices = ((unsigned char *) pVertices) + (vertexCounts[i]*vertexSize);
	}
	return S_OK;
}