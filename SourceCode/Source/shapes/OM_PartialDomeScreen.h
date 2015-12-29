#ifndef __OM_PARTIAL_DOME_SCREEN__H
#define __OM_PARTIAL_DOME_SCREEN__H
#include <ScreenGeometryBase.h>

class OM_PartialDomeScreen : public OmniMap_Screen
{
public :
	OM_PartialDomeScreen();
	virtual ~OM_PartialDomeScreen();
	static OmniMap_Screen *CreateOMPartialDome();
	virtual void SetUpPropertyAccess();
	virtual bool CreateGeometry();
	virtual void *GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
		ScreenGeometryBase::ShapeType &shapeType, int &numShapes);
	virtual void Display(void *context);
	virtual void Display();
	virtual bool xyTo3D(float x, float y, float *xyz);
#ifdef ELUMENATI_INTERNAL
	virtual void WriteObject(void *filePtr, char *name);
	virtual const char * getScreenType();
#endif
protected:

	void createMeshGeometry(ScreenGeometryBase::SimpleVertex *vertices);
	ScreenGeometryBase::SimpleVertex *vertexBuffer;
	int numStacks;
	int numRings;
	int numTriangles;
	int tesselation;
	OmniVec3 Center;
	OmniVec3 ApexDirection;
	float Radius;
	bool needsrecompile;
	int vertexCount;
};


#endif