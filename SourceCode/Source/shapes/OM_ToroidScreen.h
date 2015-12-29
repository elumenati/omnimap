#ifndef __OM_TOROID_SCREEN__
#define __OM_TOROID_SCREEN__
// ToroidScreen.h



#include <OmniMapBase.h>
#include <ScreenGeometryBase.h>




class OM_ToroidScreen : public OmniMap_Screen
{

	// TODO: Add your methods for this class here.
public:
	OM_ToroidScreen();
	~OM_ToroidScreen();

	static OmniMap_Screen *CreateOMToroidScreen();
	virtual void SetUpPropertyAccess();
	virtual bool CreateGeometry();
	virtual void *GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
		ScreenGeometryBase::ShapeType &shapeType, int &numShapes);
	virtual void Display(void *context);
	virtual void Display();
	virtual bool xyTo3D(float x, float y, float *xyz) { x; y; xyz; return true; }
#ifdef ELUMENATI_INTERNAL
	virtual void WriteObject(void *filePtr, char *name);
	virtual const char * getScreenType();
#endif


protected:

	void createMeshGeometry(ScreenGeometryBase::SimpleVertex *vertices);
	ScreenGeometryBase::SimpleVertex *vertexBuffer;
	int numTriangles;
	int numRings;
	int numSides;
	OmniVec3 Center;
	OmniVec3 ApexDirection;
	float ringRadius;
	float torusRadius;
	float ringStartAngle;
	float ringEndAngle;
	float sideStartAngle;
	float sideEndAngle;
	bool needsrecompile;
	int *vertexCounts;
	int numShapes;

};

#endif
