/*
 *  ElumSphere.h
 *  ElumSphere
 *
 *  Created by Jim Terhorst on 3/24/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifdef __APPLE__
#include "OmniMapAPI/omnimap.h"
#include "OmniMapAPI/OM_DomeScreen.h"
#include "OmniMapAPI/privatePrimitives.h"
#else
#include "OmniMapBase.h"
#include "OM_DomeScreen.h"
#include "privatePrimitives.h"
#endif
class OmniMapQuartz;
class OmniMapChannelATOGL;

#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API SphereShape : public OM_DomeScreen {
#else
class SphereShape : public OM_DomeScreen {
#endif
public:
  SphereShape();
  virtual ~SphereShape();
	static OmniMap_Screen *CreateElumSphere();
	void *GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
						ScreenGeometryBase::ShapeType &aShapeType, int &aNumShapes);
	bool CreateGeometry();
	void Display();
	virtual void Display(void *context);
#ifdef ELUMENATI_INTERNAL
	virtual void WriteObject(void *filePtr, char *name);
	virtual const char * getScreenType();
#endif
	
	
	
private :
	bool hasTextureCoords;
	int *vertexCounts;
	int numShapes;
	ScreenGeometryBase::TextureCoordVertex *texVertexBuffer;
	void createMeshGeometry3(void *vertices);
	
	
};

#ifdef notdef
class ElumSphere 
{
public:
	ElumSphere();
	~ElumSphere();
	void initialize(float _radius, int _tesselation, OmniMapQuartz *om);
	void Display();
	void setRadius(float r);
	
	float radius;
	int tesselation;
	typedef enum {
		WIREFRAME,
		SOLID
	} DrawMode;

	void setDrawMode(bool drawWireframe)
	{
		if (drawWireframe)
			drawMode = WIREFRAME;
		else
			drawMode = SOLID;
	}
	DrawMode drawMode;
	SphereShape *sphereShape;
	bool initialized;
	OmniMapQuartz *omnimap;
};
#endif

