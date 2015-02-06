
#include <math.h>
#pragma warning(disable: 4244) // disable warning 4244: double to float
#pragma warning (disable:4305)
#include "../omnimap/privatePrimitives.h"
#include "OM_PartialDomeScreen.h"
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif

OmniMap_Screen *OM_PartialDomeScreen::CreateOMPartialDome()
{
	return (new OM_PartialDomeScreen());
}

OM_PartialDomeScreen::OM_PartialDomeScreen()
{
	vertexBuffer = NULL;
	tesselation =100;
	SetUpPropertyAccess();
	Center=OmniVec3(0,0,0);
	ApexDirection =OmniVec3(0,1,0);
	numStacks = 99;
	numRings = 99;
	Radius  = 1;
	needsrecompile = true;
	renderer = NULL;
}

OM_PartialDomeScreen::~OM_PartialDomeScreen()
{
	if (renderer != NULL)
	{
		delete renderer;
		renderer = NULL;
	}
}

void OM_PartialDomeScreen::SetUpPropertyAccess()
{
	LogSystem()->ReportMessage("derived OM_PartialDomeScreen::SetUpPropertyAccess()");

	access.Register("Dome_Radius",&Radius);
	access.Register("Dome_tesselation",&tesselation);
	access.Register("Dome_Center.x",&Center.x);
	access.Register("Dome_Center.y",&Center.y);
	access.Register("Dome_Center.z",&Center.z);
	access.Register("Dome_ApexDirection.x",&ApexDirection.x);
	access.Register("Dome_ApexDirection.y",&ApexDirection.y);
	access.Register("Dome_ApexDirection.z",&ApexDirection.z);
	access.Register("Dome_NumStacks",&numStacks);
	access.Register("Dome_NumRings",&numRings);
	access.Register("needsrecompile",&needsrecompile);
}

#ifdef ELUMENATI_INTERNAL
const char *OM_PartialDomeScreen::getScreenType()
{
	return("partial_dome");
}
void OM_PartialDomeScreen::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapePartialDome::new(\"partial_dome\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializePartialDomeScreen(" << name << ")\n\n";

}
#endif

void *OM_PartialDomeScreen::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
									 ScreenGeometryBase::ShapeType &shapeType, int &aNumShapes)
{
	*numVertices = &vertexCount;
	vertType = vertexType;
	aNumShapes = 1;
	shapeType = ScreenGeometryBase::VBT_TRIANGLES;
	return vertexBuffer;
}

void OM_PartialDomeScreen::Display(void *context)
{
	if(needsrecompile)
	{
		CreateGeometry();
		renderer->ResetGeometry(vertexBuffer, ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT, ScreenGeometryBase::VBT_TRIANGLES,
			&vertexCount, 1);
		needsrecompile = false;
	}
	renderer->Display(context);
}

void OM_PartialDomeScreen::Display()
{
	if(needsrecompile)
	{
		CreateGeometry();
		renderer->ResetGeometry(vertexBuffer, ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT, ScreenGeometryBase::VBT_TRIANGLES,
			&vertexCount, 1);
		needsrecompile = false;
	}
	renderer->Display(NULL);
}



bool OM_PartialDomeScreen::CreateGeometry()
{
	// Ok, so this isnt going to be exact, but it will create enough space for all of the vertices.
	int maxNumQuads = (tesselation-1)*(tesselation-1);
	float inverse = (1.0/((float)tesselation-1.0));
	int maxNumTris = (inverse*(tesselation*tesselation))+(.1*tesselation);

	// Create enough memory for the vertices and indices
	vertexBuffer = new ScreenGeometryBase::SimpleVertex[(maxNumQuads * 2 * 3)];
    if(!vertexBuffer)
        return false;
    createMeshGeometry(vertexBuffer);
	return true;

}

#define Saturate(x) ((x)>=0?((x)>1?1:(x)):(0))

bool OM_PartialDomeScreen::xyTo3D(float x, float y, float *xyz)
{
	OmniVec3 bl= OmniVec3(x,y,0.0);
	bl.z =  sqrt(Saturate(1.0-bl.x*bl.x - bl.y*bl.y));
	xyz[0] = bl.x;
	xyz[1] = bl.y;
	xyz[2] = bl.z;
	xyz[3] = 0.0;

	OmniVec3 bl2;
	bl2.x = bl.x;
	bl2.y = bl.y;

	OmniVec3 gl_Position;

		//vec4 VertexPosition_projectorSpace_Goal = ftransform();

		// F-Theta Warping Code
		float Z = bl.z;
		//float D = length(VertexPosition_projectorSpace_Goal.xyz);
		float D = bl.length();


		float MyPI = 3.14159265;  
		float R = (2.0/MyPI) * acos(Z/D);  // Ftheta.

		// if(usebangtheta)
		R =  0.2798*R *R *R *R *R *R  - 0.6875*R *R *R *R *R  + 0.7351*R *R *R *R  - 0.3472*R *R *R  + 0.0977*R *R  + 0.9221*R ;  
 
	
		float l =1.0/ (bl2.length()+.0000000000000000000000000000000000001);// bug fix  1
		float thetavec[2] = { bl.x*l, bl.y*l};
		
		gl_Position.x = thetavec[0] * R * 2.0;
		gl_Position.y = thetavec[1] * R * 2.0;
		
		//gl_Position.w =1.0;
		gl_Position.z =-D/1000.0;   // bug fix 2b fix for reverse z depth

		xyz[0] = gl_Position.x;
		xyz[1] = gl_Position.y;
		xyz[2] = bl.z;
		xyz[3] = 0.0;
		return(true);
}


OmniVec3 project2DPointUpYToUnitDome(OmniVec3 pointIn);
bool IsAnyPartOfTriangleInsideCircle(OmniVec3 pointIn1,OmniVec3 pointIn2,OmniVec3 pointIn3);
OmniVec3 PullInsideCircle(OmniVec3 pointIn);
ScreenGeometryBase::SimpleVertex OmniVec3ToSimpleVertex(OmniVec3 in);


float SlammedSquareSmoothstep(float x)
{
	float xp = x;
	x = 1.0-x;
	x = x*x;
	x = 1.0-x;
	return (xp + x)*.5;
}
OmniVec3 SlammedSmoothUnitVec(OmniVec3 in)
{
	float l = in.length();
	float newL = SlammedSquareSmoothstep(l);
	in = in * newL/l;
	return in;
}


void OM_PartialDomeScreen::createMeshGeometry(ScreenGeometryBase::SimpleVertex *vertices)
{
	// display a dome using opengl...
	// standard way of doing it....
	// have a basic unit dome, and have a matrix move and rotate it....

	// generate the 4x4 matrix for moving it...
	if (numRings > tesselation - 1)
		numRings = tesselation - 1;
	if (numStacks > tesselation - 1)
		numStacks = tesselation - 1;

	ScreenGeometryBase::SimpleVertex *vertP = vertices;
	vertexCount = 0;
	numTriangles = 0;
	OmniVec3 DefaultI(1,0,0);
	OmniVec3 DefaultJ(0,1,0);
	OmniVec3 DefaultK(0,0,1);

	OmniVec3 NewJ = ApexDirection;
	OmniVec3 NewI = OmniVec3::MakePerpendicluarVector(NewJ);
	OmniVec3 NewK = NewI.cross(NewJ);
	double domeradius= Radius;
	

	LogSystem()->ReportError("numRings:%d numStacks:%d Radius:%f\n", numRings, numStacks, Radius);

	//Type = GL_POINTS;
	


	for ( int i = 0; i < numRings; i++)
	{
		for ( int j = 0; j < numStacks ; j++)
		{
			float l = (j	/(double)(tesselation - 1.0)  );
			float r = ((j+1)/(double)(tesselation - 1.0)  );

			float cutoff = .65;//75;
			if(l>cutoff)
			{
				l -= cutoff;
				l /= 1.0 - cutoff;
				l = l /3.0;
				l *= 1.0 - cutoff;
				l += cutoff;
			}
			if(r>cutoff)
			{
				r -= cutoff;
				r /= 1.0 - cutoff;
				r =  r/3.0 ;
				r *= 1.0 - cutoff;
				r += cutoff;
			}

			l = l*2.0-1;
			r = r*2.0-1;
			
			float b = i	/(double)(tesselation - 1.0);  
			float t = (i+1)/(double)(tesselation - 1.0);

	

			t = t*2.0-1;
			b = b*2.0-1;



			OmniVec3 bl= OmniVec3(b,0.0f,l);
			OmniVec3 tl= OmniVec3(t,0.0f,l);			     
			OmniVec3 tr= OmniVec3(t,0.0f,r);
			OmniVec3 br= OmniVec3(b,0.0f,r);
			





			OmniVec3 blPrime = PullInsideCircle(bl);
			OmniVec3 tlPrime = PullInsideCircle(tl);
			OmniVec3 brPrime = PullInsideCircle(br);
			OmniVec3 trPrime = PullInsideCircle(tr);


			if(false)
			{
				blPrime = SlammedSmoothUnitVec(blPrime);/// SPECIAL TUNING FOR BETTER SLAMMED TESSEL
				tlPrime = SlammedSmoothUnitVec(tlPrime);
				brPrime = SlammedSmoothUnitVec(brPrime);
				trPrime = SlammedSmoothUnitVec(trPrime);
			}

/*
			blPrime.y = sqrt(1-Saturate(blPrime.x*blPrime.x + blPrime.z*blPrime.z)); // this does not look correct to clement
			tlPrime.y = sqrt(1-Saturate(tlPrime.x*tlPrime.x + tlPrime.z*tlPrime.z)); // the one minus should be on the outside of the sqrt
			trPrime.y = sqrt(1-Saturate(trPrime.x*trPrime.x + trPrime.z*trPrime.z)); // 
			brPrime.y = sqrt(1-Saturate(brPrime.x*brPrime.x + brPrime.z*brPrime.z)); // 
			*/
			blPrime = OmniVec3(blPrime.dot(NewI),blPrime.dot(NewJ),blPrime.dot(NewK))*Radius+Center;
			tlPrime = OmniVec3(tlPrime.dot(NewI),tlPrime.dot(NewJ),tlPrime.dot(NewK))*Radius+Center;
			trPrime = OmniVec3(trPrime.dot(NewI),trPrime.dot(NewJ),trPrime.dot(NewK))*Radius+Center;
			brPrime = OmniVec3(brPrime.dot(NewI),brPrime.dot(NewJ),brPrime.dot(NewK))*Radius+Center;

			//if(IsAnyPartOfTriangleInsideCircle(bl,tl,tr))
			{
				*(vertP++)=OmniVec3ToSimpleVertex(blPrime);
				*(vertP++)=OmniVec3ToSimpleVertex(tlPrime);
				*(vertP++)=OmniVec3ToSimpleVertex(trPrime);
				numTriangles += 1;
				vertexCount += 3;
			}

			//if(IsAnyPartOfTriangleInsideCircle(tr,br,bl))
			{
				*(vertP++)=OmniVec3ToSimpleVertex(trPrime);
				*(vertP++)=OmniVec3ToSimpleVertex(brPrime);
				*(vertP++)=OmniVec3ToSimpleVertex(blPrime);
				numTriangles += 1;
				vertexCount += 3;
			}
		}
	}
}
