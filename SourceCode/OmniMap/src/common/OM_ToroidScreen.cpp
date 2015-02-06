// This is the main DLL file.



#include "OM_ToroidScreen.h"

/// move to the c+++ file


#include <string>
#include <map>
#include <math.h>
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG2RAD(x) ((x) * (M_PI/180.0))
#define RAD2DEG(x) ((x) * (180.0/M_PI))

OmniMap_Screen *OM_ToroidScreen::CreateOMToroidScreen()
{
	return (new OM_ToroidScreen());
}

OM_ToroidScreen::OM_ToroidScreen()
{
	vertexBuffer = NULL;
	numRings = 100;
	numSides = 100;	
	Center=OmniVec3(0,0,0);
	ApexDirection =OmniVec3(0,1,0);
	ringRadius = 7.5;
	torusRadius = 6.0;
	ringStartAngle = 0.0;
	ringEndAngle = 240.0f;
	sideStartAngle = 0.0f;
	sideEndAngle = 70.0f;
	needsrecompile = true;
	renderer = NULL;
	SetUpPropertyAccess();
	numShapes = 0;
}

OM_ToroidScreen::~OM_ToroidScreen()
{
	if (renderer != NULL)
	{
		delete renderer;
		renderer = NULL;
	}
}

void OM_ToroidScreen::SetUpPropertyAccess()
{
	LogSystem()->ReportMessage("derived OM_ToroidScreen::SetUpPropertyAccess()");

	access.Register("RingRadius",&ringRadius);
	access.Register("TorusRadius",&torusRadius);
	access.Register("NumRings",&numRings);
	access.Register("NumSides",&numSides);
	access.Register("RingStartAngle", &ringStartAngle);
	access.Register("RingEndAngle", &ringEndAngle);
	access.Register("SideStartAngle", &sideStartAngle);
	access.Register("SideEndAngle", &sideEndAngle);


	access.Register("Dome_Center.x",&Center.x);
	access.Register("Dome_Center.y",&Center.y);
	access.Register("Dome_Center.z",&Center.z);
	access.Register("Dome_ApexDirection.x",&ApexDirection.x);
	access.Register("Dome_ApexDirection.y",&ApexDirection.y);
	access.Register("Dome_ApexDirection.z",&ApexDirection.z);
	access.Register("needsrecompile",&needsrecompile);
}

#ifdef ELUMENATI_INTERNAL
const char *OM_ToroidScreen::getScreenType()
{
	return("toroid");
}

void OM_ToroidScreen::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapeToroid::new(\"toroid\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializeToroidScreen(" << name << ")\n\n";

}
#endif


void *OM_ToroidScreen::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
									 ScreenGeometryBase::ShapeType &shapeType, int &aNumShapes)
{
	*numVertices = vertexCounts;
	vertType = vertexType;
	aNumShapes = numShapes;
	shapeType = ScreenGeometryBase::VBT_TRIANGLE_STRIP;
	return vertexBuffer;
}

void OM_ToroidScreen::Display(void *context)
{
	if(needsrecompile)
	{
		CreateGeometry();
		renderer->ResetGeometry(vertexBuffer, ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT, 
			ScreenGeometryBase::VBT_TRIANGLE_STRIP,
			vertexCounts, numShapes);
		needsrecompile = false;
	}
	renderer->Display(context);
}

void OM_ToroidScreen::Display()
{
	if(needsrecompile)
	{
		CreateGeometry();
		renderer->ResetGeometry(vertexBuffer, ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT, 
			ScreenGeometryBase::VBT_TRIANGLE_STRIP,
			vertexCounts, numShapes);
		needsrecompile = false;
	}
	renderer->Display(NULL);
}

bool OM_ToroidScreen::CreateGeometry()
{
	vertexCounts = new int[(numRings+1)];
	// Ok, so this isnt going to be exact, but it will create enough space for all of the vertices.
	int maxNumTris = (numSides+1)*(numRings) * 2;

	// Create enough memory for the vertices and indices
	vertexBuffer = new ScreenGeometryBase::SimpleVertex[(maxNumTris * 3)];
    if(!vertexBuffer)
        return false;
    createMeshGeometry(vertexBuffer);
	return true;

}

void OM_ToroidScreen::createMeshGeometry(ScreenGeometryBase::SimpleVertex *vertices)
{

	LogSystem()->ReportMessage("Create Toroid:%f %f %f %f %f %f\n", ringStartAngle, ringEndAngle, sideStartAngle, sideEndAngle,
		ringRadius, torusRadius);
	int i, j;
	float theta, phi, theta1;
	float cosTheta, sinTheta;
	float cosTheta1, sinTheta1;
	float ringDelta, sideDelta;
	numShapes = 0;

	ScreenGeometryBase::SimpleVertex *simpleVB;
	simpleVB = vertexBuffer;

	ringDelta = (float) DEG2RAD(ringEndAngle - ringStartAngle) / numRings;
	sideDelta = (float) DEG2RAD(sideEndAngle - sideStartAngle) / numSides;

	theta = (float) DEG2RAD(ringStartAngle);
	cosTheta = cos(theta);
	sinTheta = sin(theta);

	OmniVec3 DefaultI(1,0,0);
	OmniVec3 DefaultJ(0,1,0);
	OmniVec3 DefaultK(0,0,1);

	OmniVec3 NewJ = ApexDirection;
	OmniVec3 NewI = OmniVec3::MakePerpendicluarVector(NewJ);
	OmniVec3 NewK = NewI.cross(NewJ);
	//LogSystem()->ReportMessage("NEWI:%f %f %f\n", NewI.x, NewI.y, NewI.z);
	//LogSystem()->ReportMessage("NEWJ:%f %f %f\n", NewJ.x, NewJ.y, NewJ.z);
	//LogSystem()->ReportMessage("NEWK:%f %f %f\n", NewK.x, NewK.y, NewK.z);


	for (i = numRings - 1; i >= 0; i--) {
		vertexCounts[i] = 0; 
		numShapes++;
		theta1 = theta + ringDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);
		phi = (float) DEG2RAD(sideStartAngle);

		for (j = numSides; j >= 0; j--) {
			float cosPhi, sinPhi, dist;

			
			cosPhi = cos(phi);
			sinPhi = sin(phi);
      
			dist = torusRadius * cosPhi;

			OmniVec3 p1(cosTheta1*dist, sinPhi*ringRadius, sinTheta1*dist);
			simpleVB->x = p1.dot(NewI);
			simpleVB->y = p1.dot(NewJ);
			simpleVB->z = p1.dot(NewK);
			simpleVB++;
			vertexCounts[i]++;

			OmniVec3 p2(cosTheta*dist, sinPhi*ringRadius, sinTheta*dist);
			simpleVB->x = p2.dot(NewI);
			simpleVB->y = p2.dot(NewJ);
			simpleVB->z = p2.dot(NewK);
			simpleVB++;
			vertexCounts[i]++;
			//if (i == 0)
				//LogSystem()->ReportMessage("Side Angle:%f\n", RAD2DEG(phi));
			phi += sideDelta;
			
		}
		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}

