
#include "OmniMapBase.h"
#include "OmniMapD3D10.h"
#include "ScreenRendererD3D10.h"
#include "OmniMapShaderD3D10.h"


#define D3DFVF_XYZ_VERTEX_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_XYZ_VERTEX (D3DFVF_XYZ)

ScreenRendererBase *ScreenRendererD3D10::CreateScreenRendererD3D10()
{
	return (new ScreenRendererD3D10());
}

ScreenRendererD3D10::ScreenRendererD3D10()
{
	vertexBuffer = NULL;
	g_pVertexBuffers = NULL;
	d3d10Layout = NULL;
}

ScreenRendererD3D10::~ScreenRendererD3D10()
{
	if (g_pVertexBuffers != NULL)
	{
		for (int i = 0; i < numShapes; i++)
		{
			SAFE_RELEASE(g_pVertexBuffers[i]);
		}
	}
}

void ScreenRendererD3D10::ResetGeometry(void *aVertexBuffer, ScreenGeometryBase::VertexType aVertexType, 
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
		d3d10Layout = OmniMapShaderD3D10::PositionLayout;
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
		break;
	default :
		d3d10Layout = OmniMapShaderD3D10::PositionLayout;
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
	}
}

void ScreenRendererD3D10::Display(void *p)
{
	ID3D10Device *d3dDevice = (ID3D10Device *) p;
	if (g_pVertexBuffers == NULL)
	{
		CreateVertBuffers(d3dDevice, vertexBuffer,  vertexCounts, numShapes);
	}

	
	UINT *uStrides = new UINT[numShapes];
	UINT *uOffsets = new UINT[numShapes];
	int totalVerts = 0;
	for (int i = 0; i < numShapes; i++)
	{
		uStrides[i] = vertexSize;
		uOffsets[i] = 0;
		totalVerts += this->vertexCounts[i];
	}
	
	d3dDevice->IASetInputLayout( d3d10Layout );

	for (int i = 0; i < numShapes; i++)
	{
		d3dDevice->IASetVertexBuffers( 0, 1, &(g_pVertexBuffers[i]), uStrides, uOffsets );
		d3dDevice->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
		if (this->shapeType == ScreenGeometryBase::VBT_TRIANGLES)
			d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		else
			d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		

		// Draw
		D3D10_TECHNIQUE_DESC techDesc;

		ID3D10EffectTechnique* pTech = omShader->getOMTechnique();

		pTech->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			pTech->GetPassByIndex( p )->Apply(0);
			d3dDevice->Draw( vertexCounts[i], 0 );
		}
	}
	delete(uStrides);
	delete(uOffsets);
}

void ScreenRendererD3D10::setOMShader(OmniMapShaderD3D10 *shader)
{
	omShader = shader;
}

HRESULT ScreenRendererD3D10::CreateVertBuffers(ID3D10Device* pDev10, void* pVertices, 
										  int *NumVertices, int numShapes)
{
	HRESULT hr;
	int bufferSize;
	g_pVertexBuffers = new ID3D10Buffer *[numShapes];
	for (int i = 0; i < numShapes; i++)
	{
		if (d3d10Layout == OmniMapShaderD3D10::PositionLayout)
		{
			bufferSize = vertexSize * NumVertices[i];
			D3D10_BUFFER_DESC vbdesc =
			{
				bufferSize,
				D3D10_USAGE_IMMUTABLE,
				D3D10_BIND_VERTEX_BUFFER,
				0,
				0
			};
			D3D10_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = pVertices;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			hr = pDev10->CreateBuffer( &vbdesc, &InitData, &g_pVertexBuffers[i] );

			if (FAILED(hr))
			{
				// TO DO: Respond to failure of CreateVertexBuffer
				return hr;
			}
		}
		unsigned char *verts = NULL;

		pVertices = ((unsigned char *) pVertices) + (vertexCounts[i]*vertexSize);
	}
	return S_OK;
}