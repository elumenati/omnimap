
#include "OM_PlaneScreen.h"
#include "math.h"
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif
#include "OM_ErrorHandling.h"

#pragma warning (disable:4244)

OM_PlaneScreen::OM_PlaneScreen(void)
{
  EH_DECLARE;

  needsrecompile =true;
	height = 2;
	width = 2;
	center.x = center.y = center.z = 0.0;
	forward.x = forward.y = 0.0;
	forward.z = 1.0;
	blU = 0.0;
	blV = 0.0;
	widthU = 1.0;
	heightV = 1.0;
	SetUpPropertyAccess();
	EH_Log("new OmniMap_Screen:OM_PlaneScreen");
	renderTexCoordinates = true;

  vertexBuffer = NULL;
  vertexCounts = NULL;
  renderer = NULL;
}

OM_PlaneScreen::~OM_PlaneScreen(void)
{
  if (vertexCounts) { delete[] vertexCounts; vertexCounts = NULL; }
  if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
  if (renderer) { delete renderer; renderer = NULL; } // this should be on ScreenGeometryBase destructor (must test if defining the destructor breaks API compatibility) 
}


OmniMap_Screen *OM_PlaneScreen::CreateOMPlane()
{
	return (new OM_PlaneScreen());
}

void OM_PlaneScreen::SetUpPropertyAccess()
{
  EH_DECLARE;
  EH_Log("OM_ConeScreen::SetUpPropertyAccess()");

	access.Register("needsrecompile",&needsrecompile);
	
	access.Register("PlaneHeight",  &height);
	access.Register("PlaneWidth",  &width);

	access.Register("Center.x",&center.x);
	access.Register("Center.y",&center.y);
	access.Register("Center.z",&center.z);
	access.Register("Foward.x",&forward.x);
	access.Register("Foward.y",&forward.y);
	access.Register("Foward.z",&forward.z);
		

	access.Register("RenderTexCoordinates",&renderTexCoordinates);
	access.Register("blU",&blU);
	access.Register("blV",&blV);
	access.Register("widthU",&widthU);
	access.Register("heightV",&heightV);
}



void *OM_PlaneScreen::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
										 ScreenGeometryBase::ShapeType &aShapeType, int &aNumShapes)
{
	*numVertices = vertexCounts;
	vertType = vertexType;
	aNumShapes = numShapes;
	aShapeType = ScreenGeometryBase::VBT_TRIANGLE_STRIP;
	return vertexBuffer;
}

#ifdef ELUMENATI_INTERNAL
const char *OM_PlaneScreen::getScreenType()
{
	return("plane");
}
void OM_ConeScreen::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapePlane:new(\"plane\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializePlaneScreen(" << name << ")\n\n";

}
#endif



bool OM_PlaneScreen::CreateGeometry()
{
  int vertexCount = 4;
  EH_DECLARE;
  
  EH_Ptr(vertexCounts = new int[1]);
	numShapes = 1;
	
	// Create enough memory for the vertices and indices
	if (renderTexCoordinates) {
		EH_Ptr(vertexBuffer = (void *) new ScreenGeometryBase::TextureCoordVertex[vertexCount]);
		vertexType = ScreenGeometryBase::VBT_VERTEX_TEXCOORD_FLT;
	} else {
		EH_Ptr(vertexBuffer = (void *) new ScreenGeometryBase::SimpleVertex[vertexCount]);
		vertexType = ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT;
	}
	
  createMeshGeometry((void *) vertexBuffer);

  EH_OnError() {
    if (vertexCounts) { delete[] vertexCounts; vertexCounts = NULL; }
    if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
    return false;
  }

  return true;
}



void OM_PlaneScreen::Display(void *context)
{
  if(needsrecompile)
  {
    needsrecompile =false;
    bool hr = CreateGeometry();
    hr;
  }
  renderer->ResetGeometry(vertexBuffer, vertexType, 
    ScreenGeometryBase::VBT_TRIANGLE_STRIP, vertexCounts, numShapes);
  renderer->Display(context);
}



void OM_PlaneScreen::Display()
{
  if(needsrecompile)
  {
    needsrecompile =false;
    bool hr = CreateGeometry();
    hr;
  }
  renderer->ResetGeometry(vertexBuffer, vertexType, 
    ScreenGeometryBase::VBT_TRIANGLE_STRIP, vertexCounts, numShapes);
  renderer->Display(NULL);
}



bool OM_PlaneScreen::xyTo3D(float x, float y, float *xyz)
{
  x;y;xyz;
	return false;
}



void OM_PlaneScreen::createMeshGeometry(void *vertexBuffer) {

	ScreenGeometryBase::SimpleVertex *simpleVB = NULL;
	ScreenGeometryBase::TextureCoordVertex *texCoordVB = NULL;
	if (vertexType == ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT)
	{
		simpleVB = (ScreenGeometryBase::SimpleVertex *) vertexBuffer;
	} else {
		texCoordVB = (ScreenGeometryBase::TextureCoordVertex *) vertexBuffer;
	}

	if(renderTexCoordinates){
		//Lower Left
		texCoordVB->x = center.x-width/2.0;
		texCoordVB->y = center.y;
		texCoordVB->z = center.z-height/2.0;
		texCoordVB->u = 0.0;
		texCoordVB->v = 0.0;
		texCoordVB++;

		// Lower Right
		texCoordVB->x = center.x+width/2.0;
		texCoordVB->y = center.y;
		texCoordVB->z = center.z-height/2.0;
		texCoordVB->u = 1.0;
		texCoordVB->v = 0.0;
		texCoordVB++;

		// Upper Right
		texCoordVB->x = center.x-width/2.0;
		texCoordVB->y = center.y;
		texCoordVB->z = center.z+height/2.0;
		texCoordVB->u = 0.0;
		texCoordVB->v = 1.0;
		texCoordVB++;

		// Upper Left
		texCoordVB->x = center.x+width/2.0;
		texCoordVB->y = center.y;
		texCoordVB->z = center.z+height/2.0;
		texCoordVB->u = 1.0;
		texCoordVB->v = 1.0;
		
	} else {
		simpleVB->x = center.x-width/2.0;
		simpleVB->y = center.y;
		simpleVB->z = -height/2.0;
		//LogSystem()->ReportError("%d:%f:%f %f %f\n", numVertsPerShape, sweepAngle, simpleVB->x, simpleVB->y, simpleVB->z);
		simpleVB++;

		simpleVB->x = center.x+width/2.0;
		simpleVB->y = center.y;
		simpleVB->z = -height/2.0;
		//LogSystem()->ReportError("%d:%f:%f %f %f\n", numVertsPerShape, RAD2DEG(sweepAngle), simpleVB->x, simpleVB->y, simpleVB->z);
		simpleVB++;

		simpleVB->x = center.x-width/2.0;
		simpleVB->y = center.y;
		simpleVB->z = height/2.0;
		//LogSystem()->ReportError("%d:%f:%f %f %f\n", numVertsPerShape, RAD2DEG(sweepAngle), simpleVB->x, simpleVB->y, simpleVB->z);
		simpleVB++;

		simpleVB->x = center.x+width/2.0;
		simpleVB->y = 0.0;
		simpleVB->z = height/2.0;	
	}
	vertexCounts[0] = 4;
}