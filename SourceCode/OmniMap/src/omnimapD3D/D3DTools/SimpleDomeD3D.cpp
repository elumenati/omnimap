#include "SimpleDomeD3D.h"
#include <windows.h>
#include <math.h>
#include "../D3D/dxstdafx.h"
#pragma warning(disable: 4244) // disable warning 4244: double to float
#include "../../omnimap/privatePrimitives.h"


LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;

// Dome Needs to be recompiled after every tweak
HRESULT HyperbolaD3D::CreateShapeMesh(IDirect3DDevice9* pDev9, CUSTOMVERTEX* pVertices, 
										  UINT NumVertices)
{
	HRESULT hr = 0;
	if( FAILED( pDev9->CreateVertexBuffer( NumVertices*sizeof(CUSTOMVERTEX),
												  D3DUSAGE_WRITEONLY,
												  D3DFVF_CUSTOMVERTEX,
												  D3DPOOL_DEFAULT,
												  &g_pVertexBuffer,
												  NULL ) ) )

	{
		// TO DO: Respond to failure of CreateVertexBuffer
		return hr;
	}
	CUSTOMVERTEX *verts = NULL;

	if( FAILED( g_pVertexBuffer->Lock( 0, 0, (void**)&verts, 0 ) ) )
	{
		// TO DO: Respond to the failure of calling Lock on our Vertex Buffer
		return hr;
	}
	else
	{
		for( int i = 0; i < (int) NumVertices; ++i )
		{
            verts[i].diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 );
			verts[i].x = pVertices[i].x;
			verts[i].y = pVertices[i].y;
			verts[i].z = pVertices[i].z;
		}
		g_pVertexBuffer->Unlock();
	}
	return S_OK;
}

HRESULT HyperbolaD3D::CreateShapeMesh( IDirect3DDevice9* pDev9, ID3DXMesh** ppMesh, CUSTOMVERTEX* pVertices, 
										  UINT NumVertices, WORD* pIndices, UINT NumIndices )
{
    HRESULT hr = S_OK;

    // Create the mesh 
	hr = D3DXCreateMeshFVF( NumIndices/3, NumVertices, 0, D3DFVF_CUSTOMVERTEX, pDev9, ppMesh );
    if( FAILED(hr) )
        return(hr);
   
	ID3DXMesh *mesh = *ppMesh;
	LPDIRECT3DVERTEXBUFFER9 pTempVertexBuffer;
	
    if( SUCCEEDED( mesh->GetVertexBuffer( &pTempVertexBuffer ) ) )
    {
		g_pVertexBuffer = pTempVertexBuffer;
        int nNumVerts = mesh->GetNumVertices();
        CUSTOMVERTEX *verts = NULL;

        pTempVertexBuffer->Lock( 0, 0, (void**)&verts, 0 );
        {
            for( int i = 0; i < nNumVerts; ++i )
			{
                verts[i].diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 );//pVertices[i].diffuse;
				verts[i].x = pVertices[i].x;
				verts[i].y = pVertices[i].y;
				verts[i].z = pVertices[i].z;
			}
        }
        pTempVertexBuffer->Unlock();

        pTempVertexBuffer->Release();
    }

	// Retrieve the indices
    WORD* pIndex;
    hr = mesh->LockIndexBuffer(0, (PVOID*)&pIndex);
    if(FAILED(hr))
        return(hr);
	int numFaces = mesh->GetNumFaces();
    for( int i=0; i <(int)  mesh->GetNumFaces(); i++ )
    {
        *pIndex++ = (i*3) + 0;
        *pIndex++ = (i*3) + 1;
        *pIndex++ = (i*3) + 2;
    }

    mesh->UnlockIndexBuffer();
#ifdef notdef
    // Set attributes
    DWORD dwNumAttr = 1;
    D3DX10_ATTRIBUTE_RANGE* pAttr = new D3DX10_ATTRIBUTE_RANGE[dwNumAttr];
    if(!pAttr)
        return E_OUTOFMEMORY;

    pAttr[0].AttribId = 0;
    pAttr[0].FaceStart = 0;
    pAttr[0].FaceCount = NumIndices/3;
    pAttr[0].VertexStart = 0;
    pAttr[0].VertexCount = NumVertices;
    (*ppMesh)->SetAttributeTable( pAttr, dwNumAttr );
    SAFE_DELETE_ARRAY( pAttr );
    
    // Create the internal mesh VBs and IBs
    (*ppMesh)->CommitToDevice();
#endif
    return hr;
}


HyperbolaD3D::~HyperbolaD3D()
{
	SAFE_RELEASE( sphereMesh );
}

HyperbolaD3D::HyperbolaD3D()
{
	 tesselation = 20;

	 Center=OmniVec3(0,0,0);
	 ApexDirection =OmniVec3(0,1,0);
	 sphereMesh = NULL;
	 Radius  = 1;
	 
}

// Create a clipping-capable vertex buffer. Allocate enough memory 
// in the default memory pool to hold three CUSTOMVERTEX 
// structures
bool HyperbolaD3D::createDisplayList(IDirect3DDevice9* d3dDevice)
{

	CUSTOMVERTEX *vertices;
	
	// Ok, so this isnt going to be exact, but it will create enough space for all of the vertices.
	int maxNumQuads = (tesselation-1)*(tesselation-1);
	float inverse = (1.0/((float)tesselation-1.0));
	int maxNumTris = (inverse*(tesselation*tesselation))+(.1*tesselation);

	// Create enough memory for the vertices and indices
    vertices = new CUSTOMVERTEX[ (maxNumQuads * 2 * 3) + (maxNumTris*3)];
    if(!vertices)
        return FALSE;
    createMeshGeometry(vertices);
	WORD* indices = new WORD[numVertices];
	//CreateShapeMesh(d3dDevice, &sphereMesh, vertices, numVertices, indices, numVertices);
	CreateShapeMesh(d3dDevice, vertices, numVertices);

	return(true);

	
}

void HyperbolaD3D::display(IDirect3DDevice9* d3dDevice)
{
#ifdef works
	HRESULT hr;
	sphereMesh->DrawSubset(0);
#endif

	d3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );

	d3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, numTriangles );
}

#define Saturate(x) ((x)>=0?((x)>1?1:(x)):(0))
void HyperbolaD3D::createMeshGeometry(CUSTOMVERTEX *vertices)
{
	 OmniVec3 minbox(999,999,999);
	 OmniVec3 maxbox(-999,-999,-999);

	CUSTOMVERTEX *vertP = vertices;
	double domeradius= Radius;
	numTriangles = 0;
	numVertices = 0;


	OmniVec3 DefaultDirection(0,1,0);
	OmniVec3 NewDirection = ApexDirection;
	float AngleBetween = NewDirection.angleBetween(DefaultDirection);
	OmniVec3 Axis =DefaultDirection.cross(NewDirection);
	OmniVec3 Apex = Center+ApexDirection*domeradius;

	//Type = GL_POINTS;
	
	for ( int i = 0; i < tesselation - 1; i++)
	{

		for ( int j = 0; j < tesselation - 1; j++)
		{
				double x1= i/(double)(tesselation - 1.0); x1*=2;x1-=1;
				double z1 =j/(double)(tesselation - 1.0); z1*=2;z1-=1;
				double y1 =1- sqrt(Saturate(1.0 - x1*x1 - z1*z1));
				OmniVec3 vpoint1 = OmniVec3(x1*domeradius,y1*domeradius,z1*domeradius);

				//	if(i == tesselationZ/2)
				//	printf("%f = sqrt(%f^2 %f^2)\n",y1,x1,z1);

				double x2= (i+1)/(double)(tesselation - 1.0); x2*=2;x2-=1;
				double z2 =j/(double)(tesselation - 1.0); z2*=2;z2-=1;
				double y2 =1- sqrt(Saturate(1.0 - x2*x2 - z2*z2));
				OmniVec3 vpoint2 = OmniVec3(x2*domeradius,y2*domeradius,z2*domeradius);

				double x3= (i+1)/(double)(tesselation - 1.0); x3*=2;x3-=1;
				double z3 =(j+1)/(double)(tesselation - 1.0); z3*=2;z3-=1;
				double y3 = 1-sqrt(Saturate(1.0 - x3*x3 - z3*z3));
				OmniVec3 vpoint3 = OmniVec3(x3*domeradius,y3*domeradius,z3*domeradius);

				double x4= i/(double)(tesselation - 1.0); x4*=2;x4-=1;
				double z4 =(j+1)/(double)(tesselation - 1.0); z4*=2;z4-=1;
				double y4 = 1-sqrt(Saturate(1.0 - x4*x4 - z4*z4));
				OmniVec3 vpoint4 = OmniVec3(x4*domeradius,y4*domeradius,z4*domeradius);


				vpoint1 = DefaultDirection - vpoint1 ;
				vpoint2 = DefaultDirection - vpoint2 ;
				vpoint3 = DefaultDirection - vpoint3 ;
				vpoint4 = DefaultDirection - vpoint4 ;

				if(AngleBetween>0)
				{
					vpoint1= vpoint1.rotateByVector(Axis,AngleBetween);
					vpoint2= vpoint2.rotateByVector(Axis,AngleBetween);
					vpoint3= vpoint3.rotateByVector(Axis,AngleBetween);
					vpoint4= vpoint4.rotateByVector(Axis,AngleBetween);

				}

				vpoint1 = Center + vpoint1 ;
				vpoint2 = Center + vpoint2 ;
				vpoint3 = Center + vpoint3 ;
				vpoint4 = Center + vpoint4 ;


				//	goto good;
				double radius_forgiveness = .1; 
				double radius = 1.0+radius_forgiveness;
				if((x1*x1+z1*z1) > (radius ))
					goto docorner;
				if((x2*x2+z2*z2) > (radius ))
					goto docorner;
				if((x3*x3+z3*z3) > (radius))
					 goto docorner;
				if((x4*x4+z4*z4) > (radius))
					 goto docorner;

//good:
#ifdef DEBUG
				LogSystem()->ReportError("Quad:");
				LogSystem()->ReportError("\t%f %f %f",vpoint1.x,vpoint1.y,vpoint1.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint2.x,vpoint2.y,vpoint2.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint3.x,vpoint3.y,vpoint3.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint4.x,vpoint4.y,vpoint4.z);
#endif
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 );
				vertP->x = vpoint1.x;
				vertP->y = vpoint1.y;
				vertP++->z = vpoint1.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 );
				vertP->x = vpoint2.x;
				vertP->y = vpoint2.y;
				vertP++->z = vpoint2.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 );
				vertP->x = vpoint3.x;
				vertP->y = vpoint3.y;
				vertP++->z = vpoint3.z;

				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 );
				vertP->x = vpoint3.x;
				vertP->y = vpoint3.y;
				vertP++->z = vpoint3.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 );
				vertP->x = vpoint4.x;
				vertP->y = vpoint4.y;
				vertP++->z = vpoint4.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 );
				vertP->x = vpoint1.x;
				vertP->y = vpoint1.y;
				vertP++->z = vpoint1.z;
				numTriangles += 2;
				numVertices += 6;

				/*
				if(true)
				{
					minbox.x = min(vpoint1.x,minbox.x);
					minbox.y = min(vpoint1.y,minbox.y);
					minbox.z = min(vpoint1.z,minbox.z);

					maxbox.x = max(vpoint1.x,maxbox.x);
					maxbox.y = max(vpoint1.y,maxbox.y);
					maxbox.z = max(vpoint1.z,maxbox.z);
				}
				*/
				continue;

docorner:
			if(	((x2*x2+z2*z2) <= (radius))&&
				((x3*x3+z3*z3) <= (radius))&&
				((x4*x4+z4*z4) <= (radius)))
			{
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 );
				vertP->x = vpoint2.x;
				vertP->y = vpoint2.y;
				vertP++->z = vpoint2.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 );
				vertP->x = vpoint3.x;
				vertP->y = vpoint3.y;
				vertP++->z = vpoint3.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 1.0, 0.0, 0.0, 1.0 );
				vertP->x = vpoint4.x;
				vertP->y = vpoint4.y;
				vertP++->z = vpoint4.z;
				numTriangles++;
				numVertices += 3;
#ifdef DEBUG
				LogSystem()->ReportError("TRI:");
				LogSystem()->ReportError("\t%f %f %f",vpoint2.x,vpoint2.y,vpoint2.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint3.x,vpoint3.y,vpoint3.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint4.x,vpoint4.y,vpoint4.z);
#endif

			}

			
			if(
				((x1*x1+z1*z1) <= (radius)) &&
				((x3*x3+z3*z3) <= (radius))&&
				((x4*x4+z4*z4) <= (radius)))
			{
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 );
				vertP->x = vpoint1.x;
				vertP->y = vpoint1.y;
				vertP++->z = vpoint1.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 );
				vertP->x = vpoint3.x;
				vertP->y = vpoint3.y;
				vertP++->z = vpoint3.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 );
				vertP->x = vpoint4.x;
				vertP->y = vpoint4.y;
				vertP++->z = vpoint4.z;
				numTriangles++;
				numVertices += 3;
#ifdef DEBUG
				LogSystem()->ReportError("TRI:");
				LogSystem()->ReportError("\t%f %f %f",vpoint1.x,vpoint1.y,vpoint1.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint3.x,vpoint3.y,vpoint3.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint4.x,vpoint4.y,vpoint4.z);
#endif

			}

			
			if(
				((x1*x1+z1*z1) <= (radius)) &&
				((x2*x2+z2*z2) <= (radius))&&
				((x4*x4+z4*z4) <= (radius)))
			{
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 );
				vertP->x = vpoint1.x;
				vertP->y = vpoint1.y;
				vertP++->z = vpoint1.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 );
				vertP->x = vpoint2.x;
				vertP->y = vpoint2.y;
				vertP++->z = vpoint2.z;
				//vertP->diffuse = D3DCOLOR_COLORVALUE( 0.0, 0.0, 1.0, 1.0 );
				vertP->x = vpoint4.x;
				vertP->y = vpoint4.y;
				vertP++->z = vpoint4.z;
				numTriangles++;
				numVertices += 3;
#ifdef DEBUG
				LogSystem()->ReportError("TRI:");
				LogSystem()->ReportError("\t%f %f %f",vpoint1.x,vpoint1.y,vpoint1.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint2.x,vpoint2.y,vpoint2.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint4.x,vpoint4.y,vpoint4.z);
#endif

			}

			
			if(
				((x1*x1+z1*z1) <= (radius)) &&
				((x2*x2+z2*z2) <= (radius))&&
				((x3*x3+z3*z3) <= (radius)))
			{
				vertP->x = vpoint1.x;
				vertP->y = vpoint1.y;
				vertP++->z = vpoint1.z;
				vertP->x = vpoint2.x;
				vertP->y = vpoint2.y;
				vertP++->z = vpoint2.z;
				vertP->x = vpoint3.x;
				vertP->y = vpoint3.y;
				vertP++->z = vpoint3.z;
				numTriangles++;
				numVertices += 3;
#ifdef DEBUG
				LogSystem()->ReportError("TRI:");
				LogSystem()->ReportError("\t%f %f %f",vpoint1.x,vpoint1.y,vpoint1.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint2.x,vpoint2.y,vpoint2.z);
				LogSystem()->ReportError("\t%f %f %f",vpoint3.x,vpoint3.y,vpoint3.z);
#endif

			}
		}

	}

//	printf("\n");
//	printf("Dome aaBounding box Max : %f %f %f\n",maxbox.x,maxbox.y,maxbox.z);
	//printf("Dome aaBounding box Min : %f %f %f\n",minbox.x,minbox.y,minbox.z);


}