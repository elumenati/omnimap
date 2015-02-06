#ifndef __OMNIMAP_SCREEN_CYLINDER__H
#define __OMNIMAP_SCREEN_CYLINDER__H
#include "omnimapbase.h"

class    OM_CylinderScreen : public OmniMap_Screen
{
protected:
	void createMeshGeometry(void *_vertexBuffer);
	void SetUpPropertyAccess();

	void *vertexBuffer;
	int *vertexCounts;
	int numShapes;


	bool needsrecompile;
	int DisplayList;

	OmniVec3 OriginBottom;
	OmniVec3 Up;
	OmniVec3 Foward;

	float Height;
	float Radius;

	int iTessel ;
	float ThetaMin ;
	float ThetaMax;


	bool RenderTexCoordinates;
	float blU;
	float blV;
	float widthU;
	float heightV;


	float ColorRGB[3];


public:
	OM_CylinderScreen(const OM_CylinderScreen& p) ;
	OM_CylinderScreen(OM_CylinderScreen const * p) ;
	virtual void SetTextureCoordinates(float bu,float bv, float w,float h);
	OM_CylinderScreen();
	virtual ~OM_CylinderScreen();
	static OmniMap_Screen *CreateOMCylinder();

	void Display(void *context);
	virtual void *GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
		ScreenGeometryBase::ShapeType &shapeType, int &aNumShapes);
	bool CreateGeometry();

	OmniVec3 CylindricalRadiansToCartisean(float r, float theta, float h);
	OmniVec3 CylindricalRadiansToCartisean(float r, float theta, float h,
									OmniVec3 Center,  
									OmniVec3 Up,  
									OmniVec3 Foward
									);
	virtual bool xyTo3D(float x, float y, float *xyz);
#ifdef ELUMENATI_INTERNAL
	virtual void WriteObject(void *filePtr, char *name);
	virtual const char * getScreenType();
#endif
};
#endif