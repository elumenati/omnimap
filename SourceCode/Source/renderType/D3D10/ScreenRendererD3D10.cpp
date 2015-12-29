#include "OmniMapBase.h"
#include "OmniMapD3D10.h"
#include "ScreenRendererD3D10.h"
#include "OmniMapShaderD3D10.h"
#include "OM_ErrorHandling.h"


// should be declared on ScreenRendererD3D10 class, but for retrocompatibility is declared here as static.
// this works, because there is only one instance of ScreenRendererD3D10 thru omnimap.
static UINT *vertexBufferStrides = NULL;  
static UINT *vertexBufferOffsets = NULL;



ScreenRendererBase *ScreenRendererD3D10::CreateScreenRendererD3D10()
{
	return (new ScreenRendererD3D10());
}




static void clearVertexBuffer(ID3D10Buffer ***vBuffer, UINT **vertexBufferStrides, UINT **vertexBufferOffsets, int numShapes) {
  if ((*vBuffer) != NULL) {
    for (int i = 0; i < numShapes; i++) SAFE_RELEASE((*vBuffer)[i]);
    delete[] (*vBuffer);
    (*vBuffer) = NULL;
  }
  if ((*vertexBufferStrides) != NULL) { delete[] (*vertexBufferStrides); (*vertexBufferStrides) = NULL; }
  if ((*vertexBufferOffsets) != NULL) { delete[] (*vertexBufferOffsets); (*vertexBufferOffsets) = NULL; }
}




ScreenRendererD3D10::ScreenRendererD3D10()
{
  vertexBuffer = NULL;
  g_pVertexBuffers = NULL;
  d3d10Layout = NULL;
  vertexType = 0;
  vertexSize = 0;
  omShader = NULL;
  vertexBufferStrides = NULL;
  vertexBufferOffsets = NULL;
}



ScreenRendererD3D10::~ScreenRendererD3D10()
{
  clearVertexBuffer(&g_pVertexBuffers, &vertexBufferStrides, &vertexBufferOffsets, numShapes);
}



void ScreenRendererD3D10::ResetGeometry(void *aVertexBuffer, ScreenGeometryBase::VertexType aVertexType, 
									  ScreenGeometryBase::ShapeType aShapeType, int *aVertexCounts, int aNumShapes)
{
  clearVertexBuffer(&g_pVertexBuffers, &vertexBufferStrides, &vertexBufferOffsets, numShapes);

	vertexCounts = aVertexCounts;
	vertexBuffer = aVertexBuffer;
	numShapes = aNumShapes;
	vertexType = aVertexType;
	shapeType = aShapeType;
	
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
  D3D10_TECHNIQUE_DESC techDesc;
  ID3D10EffectTechnique* pTech = NULL;

  if (g_pVertexBuffers == NULL) CreateVertBuffers(d3dDevice, vertexBuffer, vertexCounts, numShapes);

  d3dDevice->IASetInputLayout( d3d10Layout );
  d3dDevice->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
  if (this->shapeType == ScreenGeometryBase::VBT_TRIANGLES)
    d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  else
    d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  pTech = omShader->getOMTechnique();
  pTech->GetDesc(&techDesc);

  for (int i = 0; i < numShapes; i++)
  {
    d3dDevice->IASetVertexBuffers( 0, 1, &(g_pVertexBuffers[i]), vertexBufferStrides, vertexBufferOffsets );

    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
      pTech->GetPassByIndex( p )->Apply(0);
      d3dDevice->Draw( vertexCounts[i], 0 );
    }
  }
}




void ScreenRendererD3D10::setOMShader(OmniMapShaderD3D10 *shader)
{
	omShader = shader;
}




HRESULT ScreenRendererD3D10::CreateVertBuffers(ID3D10Device* pDev10, void* pVertices, int *NumVertices, int numShapes)
{
  EH_DECLARE;

  EH_Ptr(g_pVertexBuffers = new ID3D10Buffer *[numShapes]);
  EH_Ptr(vertexBufferStrides = new UINT[numShapes]);
  EH_Ptr(vertexBufferOffsets = new UINT[numShapes]);

  for (int i = 0; i < numShapes; i++)
  {
    if (d3d10Layout == OmniMapShaderD3D10::PositionLayout)
    {
      D3D10_BUFFER_DESC vbdesc;
      vbdesc.Usage = D3D10_USAGE_IMMUTABLE;
      vbdesc.ByteWidth = vertexSize * NumVertices[i];
      vbdesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
      vbdesc.CPUAccessFlags = 0;
      vbdesc.MiscFlags = 0;
      D3D10_SUBRESOURCE_DATA InitData;
      InitData.pSysMem = pVertices;
      InitData.SysMemPitch = 0;
      InitData.SysMemSlicePitch = 0;
      EH_Test(pDev10->CreateBuffer( &vbdesc, &InitData, &g_pVertexBuffers[i] ), "(%d)", i);
    }
    pVertices = ((unsigned char *) pVertices) + (vertexCounts[i]*vertexSize);

    vertexBufferStrides[i] = vertexSize;
    vertexBufferOffsets[i] = 0;
  }

  EH_OnError() {
    clearVertexBuffer(&g_pVertexBuffers, &vertexBufferStrides, &vertexBufferOffsets, numShapes);
  }

  return EH_ERRORCODE;
}


