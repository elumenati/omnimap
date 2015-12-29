#ifndef __OM_DOME_SCREEN__H
#define __OM_DOME_SCREEN__H
#ifdef __APPLE__
#include <OmniMapAPI/ScreenGeometryBase.h>
#else
#include <ScreenGeometryBase.h>
#endif

#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API OM_DomeScreen : public OmniMap_Screen
#else
class OM_DomeScreen : public OmniMap_Screen
#endif
{
public :
	OM_DomeScreen();
	virtual ~OM_DomeScreen();
	static OmniMap_Screen *CreateOMDome();
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
	int numTriangles;
	int tesselation;
	OmniVec3 Center;
	OmniVec3 ApexDirection;
	float Radius;
	bool needsrecompile;
	int vertexCount;
};


#endif