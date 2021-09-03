
#include "OmniMapBase.h"
#include "OmniMapD3D11.h"
#include "ScreenRendererD3D11.h"
#include "OmniMapShaderD3D11.h"
#include "OM_ErrorHandling.h"




ScreenRendererBase *ScreenRendererD3D11::CreateScreenRendererD3D11() {
	return (new ScreenRendererD3D11());
}




static void clearVertexBuffer(ID3D11Buffer ***vBuffer, UINT **vertexBufferStrides, UINT **vertexBufferOffsets, int numShapes) {
  if ((*vBuffer) != NULL) {
    for (int i = 0; i < numShapes; i++) SAFE_RELEASE((*vBuffer)[i]);
    delete[] (*vBuffer);
    (*vBuffer) = NULL;
  }
  if ((*vertexBufferStrides) != NULL) { delete[] (*vertexBufferStrides); (*vertexBufferStrides) = NULL; }
  if ((*vertexBufferOffsets) != NULL) { delete[] (*vertexBufferOffsets); (*vertexBufferOffsets) = NULL; }
}






ScreenRendererD3D11::ScreenRendererD3D11() {
	vertexBuffer = NULL;
	g_pVertexBuffers = NULL;
	d3d11Layout = NULL;
  vertexType = 0;
  vertexSize = 0;
  omShader = NULL;
  vertexBufferStrides = NULL;
  vertexBufferOffsets = NULL;
}


ScreenRendererD3D11::~ScreenRendererD3D11() {
  clearVertexBuffer(&g_pVertexBuffers, &vertexBufferStrides, &vertexBufferOffsets, numShapes);
}



void ScreenRendererD3D11::ResetGeometry(void *aVertexBuffer, ScreenGeometryBase::VertexType aVertexType, 
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
		d3d11Layout = OmniMapShaderD3D11::PositionLayout;
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
		break;
	default :
		d3d11Layout = OmniMapShaderD3D11::PositionLayout;
		vertexSize = sizeof(ScreenGeometryBase::SimpleVertex);
    break;
  }
}



void ScreenRendererD3D11::Display(void *p)
{
  ID3D11DeviceContext *d3dDeviceContext = (ID3D11DeviceContext *) p;
  D3DX11_TECHNIQUE_DESC techDesc;
  ID3DX11EffectTechnique* pTech = NULL;

	if (g_pVertexBuffers == NULL) CreateVertBuffers(d3dDeviceContext, vertexBuffer, vertexCounts, numShapes);
	
	d3dDeviceContext->IASetInputLayout( d3d11Layout );
  d3dDeviceContext->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
  if (this->shapeType == ScreenGeometryBase::VBT_TRIANGLES)
    d3dDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  else
    d3dDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  pTech = omShader->getOMTechnique();
  pTech->GetDesc(&techDesc);

	for (int i = 0; i < numShapes; i++)
	{
		d3dDeviceContext->IASetVertexBuffers( 0, 1, &(g_pVertexBuffers[i]), vertexBufferStrides, vertexBufferOffsets );
		
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			pTech->GetPassByIndex( p )->Apply(0, d3dDeviceContext);
			d3dDeviceContext->Draw( vertexCounts[i], 0 );
		}
	}
}



void ScreenRendererD3D11::setOMShader(OmniMapShaderD3D11 *shader)
{
	omShader = shader;
}



HRESULT ScreenRendererD3D11::CreateVertBuffers(ID3D11DeviceContext* pDevContext11, void* pVertices, int *NumVertices, int numShapes)
{
  ID3D11Device *d3dDevice = NULL;
  EH_DECLARE;

  pDevContext11->GetDevice(&d3dDevice);
  EH_Ptr(d3dDevice);
  EH_Ptr(g_pVertexBuffers = new ID3D11Buffer *[numShapes]);
  EH_Ptr(vertexBufferStrides = new UINT[numShapes]);
  EH_Ptr(vertexBufferOffsets = new UINT[numShapes]);

  memset(g_pVertexBuffers, 0, sizeof(ID3D11Buffer *) * numShapes);
  memset(vertexBufferStrides, 0, sizeof(UINT) * numShapes);
  memset(vertexBufferOffsets, 0, sizeof(UINT) * numShapes);
	
  for (int i = 0; i < numShapes; i++)
	{
		if (d3d11Layout == OmniMapShaderD3D11::PositionLayout)
		{
      D3D11_BUFFER_DESC vbdesc = {0};
      vbdesc.Usage = D3D11_USAGE_IMMUTABLE;
      vbdesc.ByteWidth = vertexSize * NumVertices[i];
      vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      vbdesc.CPUAccessFlags = 0;
      vbdesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData = {0};
			InitData.pSysMem = pVertices;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			EH_Test(d3dDevice->CreateBuffer( &vbdesc, &InitData, &g_pVertexBuffers[i] ), "(%d)", i);
		}
		pVertices = ((unsigned char *) pVertices) + (vertexCounts[i]*vertexSize);

    vertexBufferStrides[i] = vertexSize;
    vertexBufferOffsets[i] = 0;
  }

  EH_OnError() {
    clearVertexBuffer(&g_pVertexBuffers, &vertexBufferStrides, &vertexBufferOffsets, numShapes);
  }

  SAFE_RELEASE(d3dDevice);

  return EH_ERRORCODE;
}