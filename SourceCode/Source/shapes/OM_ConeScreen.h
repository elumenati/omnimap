#pragma once
#include "OmniMapBase.h"

#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API OM_ConeScreen : public OmniMap_Screen
#else
class OM_ConeScreen : public OmniMap_Screen
#endif
{
public:
	static OmniMap_Screen *CreateOMCone();
	OM_ConeScreen(void);
	virtual ~OM_ConeScreen(void);

	virtual void SetUpPropertyAccess();
	virtual bool CreateGeometry();
	virtual void *GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
		ScreenGeometryBase::ShapeType &shapeType, int &numShapes);
	virtual void Display(void *context);
	virtual void Display();
	virtual bool xyTo3D(float x, float y, float *xyz);
#ifdef ELUMENATI_INTERNAL
	virtual const char * getScreenType();
	virtual void WriteObject(void *filePtr, char *name);
#endif

protected:

	void createMeshGeometry(void *vertices);
	int numTriangles;

	// The number of stacks to tesselate
	int numStacks;
	
	// The radius of the top and bottom of the cone
	float topRadius;
	float bottomRadius;
	float height;
	float sweepAngleStart;
	float sweepAngleEnd;
	bool needsrecompile;
	OmniVec3 originBottom;
	OmniVec3 up;
	OmniVec3 foward;

	bool renderTexCoordinates;
	float blU;
	float blV;
	float widthU;
	float heightV;



	void *vertexBuffer;
	int *vertexCounts;
	int numShapes;
};
