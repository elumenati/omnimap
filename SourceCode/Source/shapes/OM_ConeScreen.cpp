
#include "OM_ConeScreen.h"
#include "math.h"
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif
#include "OM_ErrorHandling.h"

#pragma warning (disable:4244)

OM_ConeScreen::OM_ConeScreen(void)
{
  EH_DECLARE;

	needsrecompile =true;
	numStacks = 30;
	topRadius = .5;
	bottomRadius = 1;
	height = 2;
	sweepAngleStart = -90;
	sweepAngleEnd = 90;
	numStacks = 30;
	blU = 0.0;
	blV = 0.0;
	widthU = 1.0;
	heightV = 1.0;
	SetUpPropertyAccess();
	EH_Log("new OmniMap_Screen:OM_ConeScreen");
	renderTexCoordinates = false;
	
  vertexBuffer = NULL;
  vertexCounts = NULL;
  renderer = NULL;
}


OM_ConeScreen::~OM_ConeScreen(void)
{
  if (vertexCounts) { delete[] vertexCounts; vertexCounts = NULL; }
  if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
  if (renderer) { delete renderer; renderer = NULL; } // this should be on ScreenGeometryBase destructor (must test if defining the destructor breaks API compatibility) 
}



OmniMap_Screen *OM_ConeScreen::CreateOMCone()
{
	return (new OM_ConeScreen());
}

void OM_ConeScreen::SetUpPropertyAccess()
{
  EH_DECLARE;
  EH_Log("OM_ConeScreen::SetUpPropertyAccess()");

	access.Register("needsrecompile",&needsrecompile);
	access.Register("NumStacks",&numStacks);
	
	access.Register("Height",  &height);
	access.Register("TopRadius",  &topRadius);
	access.Register("SweepAngleStart", &sweepAngleStart);
	access.Register("SweepAngleEnd", &sweepAngleEnd);
	access.Register("BottomRadius",  &bottomRadius);
	//access.Register("OriginBottom.x",&originBottom.x);
	//access.Register("OriginBottom.y",&originBottom.y);
	//access.Register("OriginBottom.z",&originBottom.z);
	//access.Register("Up.x",&up.x);
	//access.Register("Up.y",&up.y);
	//access.Register("Up.z",&up.z);		
	//access.Register("Foward.x",&foward.x);
	//access.Register("Foward.y",&foward.y);
	//access.Register("Foward.z",&foward.z);		

	access.Register("RenderTexCoordinates",&renderTexCoordinates);
	access.Register("blU",&blU);
	access.Register("blV",&blV);
	access.Register("widthU",&widthU);
	access.Register("heightV",&heightV);
}

#ifdef ELUMENATI_INTERNAL
const char *OM_ConeScreen::getScreenType()
{
	return("cone");
}
void OM_ConeScreen::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapeCone:new(\"cone\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializeConeScreen(" << name << ")\n\n";

}
#endif


void *OM_ConeScreen::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
										 ScreenGeometryBase::ShapeType &aShapeType, int &aNumShapes)
{
	*numVertices = vertexCounts;
	vertType = vertexType;
	aNumShapes = numShapes;
	aShapeType = ScreenGeometryBase::VBT_TRIANGLE_STRIP;
	return vertexBuffer;
}



bool OM_ConeScreen::CreateGeometry()
{
  int vertexCount = (numStacks+1)*(numStacks+1)*2;
  EH_DECLARE;

  if (vertexCounts) { delete[] vertexCounts; vertexCounts = NULL; }
  if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }

  EH_Ptr(vertexCounts = new int[(numStacks+1)]);
  numShapes = 0;

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




void OM_ConeScreen::Display(void *context)
{
  if(needsrecompile)
  {
    needsrecompile =false;
    bool hr = CreateGeometry();
    hr;
  }
  renderer->ResetGeometry(vertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP, vertexCounts, numShapes);
  renderer->Display(context);
}



void OM_ConeScreen::Display()
{
	if(needsrecompile)
	{
		needsrecompile =false;
		bool hr = CreateGeometry();
    hr;
	}
	renderer->ResetGeometry(vertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP, vertexCounts, numShapes);
	renderer->Display(NULL);
}


bool OM_ConeScreen::xyTo3D(float x, float y, float *xyz)
{
  x;y;xyz;
	return false;
}


#define M_PI 3.14159265
#define DEG2RAD(x) ((x) * (M_PI/180.0))
#define RAD2DEG(x) ((x) * (180.0/M_PI))

void OM_ConeScreen::createMeshGeometry(void *vertexBuffer) {

	ScreenGeometryBase::SimpleVertex *simpleVB = NULL;
	ScreenGeometryBase::TextureCoordVertex *texCoordVB = NULL;
	if (vertexType == ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT)
	{
		simpleVB = (ScreenGeometryBase::SimpleVertex *) vertexBuffer;
	} else {
		texCoordVB = (ScreenGeometryBase::TextureCoordVertex *) vertexBuffer;
	}
	
	float zStep = height/numStacks;
	float rStep = (topRadius - bottomRadius) / numStacks;
	float z0 = 0.0;
	float r0 = bottomRadius;
	float z1 = z0 + zStep;
	float r1 = bottomRadius + rStep;
	float bottomZ = z0;

	double sweepAngle = DEG2RAD(sweepAngleStart);
	double sa1 = sweepAngle;
	double sa2 = DEG2RAD(sweepAngleEnd);
	double angleStep = (sa2 - sa1) / (double) numStacks;

	for( int i=0; i<numStacks-1; i++ )
	{
		sweepAngle = DEG2RAD(sweepAngleStart);
		int numVertsPerShape = 0;
		for(int j=0; j<=numStacks; j++)
		{
			
			if(renderTexCoordinates)
			{
				texCoordVB->x = cosf((float) sweepAngle)*r0;
				texCoordVB->z = sinf((float) sweepAngle)*r0;
				texCoordVB->y = z0;
				texCoordVB->u = 1.0-((sweepAngle - sa1)/(sa2-sa1));
				texCoordVB->v = 1.0-((z0 - bottomZ)/height);
				texCoordVB++;
				numVertsPerShape++;
				texCoordVB->x = cosf((float) sweepAngle)*r1;
				texCoordVB->z = sinf((float) sweepAngle)*r1;
				texCoordVB->y = z1;
				texCoordVB->u = 1.0-((sweepAngle - sa1)/(sa2-sa1));
				texCoordVB->v = 1.0-((z1 - bottomZ)/height);
				texCoordVB++;
				numVertsPerShape++;
				
			} else
			{
				simpleVB->x = cosf(sweepAngle)*r0;
				simpleVB->z = sinf(sweepAngle)*r0;
				simpleVB->y = z0;
				//LogSystem()->ReportError("%d:%f:%f %f %f\n", numVertsPerShape, sweepAngle, simpleVB->x, simpleVB->y, simpleVB->z);
				simpleVB++;
				numVertsPerShape++;
				simpleVB->x = cosf(sweepAngle)*r1;
				simpleVB->z = sinf(sweepAngle)*r1;
				simpleVB->y = z1;
				//LogSystem()->ReportError("%d:%f:%f %f %f\n", numVertsPerShape, RAD2DEG(sweepAngle), simpleVB->x, simpleVB->y, simpleVB->z);
				simpleVB++;
				numVertsPerShape++;
				
			}
			sweepAngle += angleStep;
			
			
		}
		//LogSystem()->ReportError("SHAPE:%d : %d\n", numShapes, numVertsPerShape);
		vertexCounts[numShapes] = numVertsPerShape;
		z0 = z1; z1 += zStep;
		r0 = r1; r1 += rStep;
		numShapes++;


   }



}