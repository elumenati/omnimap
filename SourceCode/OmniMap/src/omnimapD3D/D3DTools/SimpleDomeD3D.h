#pragma once
#include "../omnimapD3D.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// Dome Needs to be recompiled after every tweak
class HyperbolaD3D
{
private:
	// The custom vertex type
	struct CUSTOMVERTEX {
		FLOAT x, y, z;
		DWORD diffuse; // Diffuse color of vertex
	};
	// d3dDevice contains the address of an IDirect3DDevice9 interface
	//LPDIRECT3DVERTEXBUFFER9 sphereBuffer;
	ID3DXMesh *sphereMesh;
	int numTriangles;
	int numVertices;
	void createMeshGeometry(CUSTOMVERTEX *vertices);
	HRESULT CreateShapeMesh( IDirect3DDevice9* pDev9, ID3DXMesh** ppMesh, 
		CUSTOMVERTEX* pVertices, UINT NumVertices, WORD* pIndices, UINT NumIndices );
	HRESULT HyperbolaD3D::CreateShapeMesh(IDirect3DDevice9* pDev9, CUSTOMVERTEX* pVertices, 
										  UINT NumVertices);

	





public:
	HyperbolaD3D::HyperbolaD3D();
	HyperbolaD3D::~HyperbolaD3D();
	
	void display(IDirect3DDevice9* d3dDevice);
	bool createDisplayList(IDirect3DDevice9* d3dDevice);
	OmniVec3 Center;
	OmniVec3 ApexDirection;
	int tesselation;
	float Radius;


};
