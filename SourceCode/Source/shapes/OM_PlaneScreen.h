#pragma once
#include "OmniMapBase.h"

#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API OM_PlaneScreen : public OmniMap_Screen
#else
class OM_PlaneScreen : public OmniMap_Screen
#endif
{
public:
	static OmniMap_Screen *CreateOMPlane();
	OM_PlaneScreen(void);
	virtual ~OM_PlaneScreen(void);

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

	float height;
	float width;
	bool needsrecompile;
	OmniVec3 center;
	OmniVec3 forward;

	bool renderTexCoordinates;
	float blU;
	float blV;
	float widthU;
	float heightV;

	void *vertexBuffer;
	int *vertexCounts;
	int numShapes;
};