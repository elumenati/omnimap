
#include <math.h>
#pragma warning(disable: 4244) // disable warning 4244: double to float
#include "privatePrimitives.h"
#include "OM_DomeScreen.h"
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif
#include "OM_ErrorHandling.h"


#pragma warning (disable:4127) // warning C4127: conditional expression is constant
#pragma warning (disable:4244)



OmniMap_Screen *OM_DomeScreen::CreateOMDome()
{
	return (new OM_DomeScreen());
}



OM_DomeScreen::OM_DomeScreen()
{
	vertexBuffer = NULL;
	tesselation =100;
	SetUpPropertyAccess();
	Center=OmniVec3(0,0,0);
	ApexDirection =OmniVec3(0,1,0);
	Radius  = 1;
	needsrecompile = true;

  vertexBuffer = NULL;
  renderer = NULL;
}



OM_DomeScreen::~OM_DomeScreen()
{
  if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
  if (renderer) { delete renderer; renderer = NULL; } // this should be on ScreenGeometryBase destructor (must test if defining the destructor breaks API compatibility) 
}




void OM_DomeScreen::SetUpPropertyAccess()
{
  EH_DECLARE;
  EH_Log("derived OmniMap_Screen_Dome::SetUpPropertyAccess()");

	access.Register("Dome_Radius",&Radius);
	access.Register("Dome_tesselation",&tesselation);
	access.Register("Dome_Center.x",&Center.x);
	access.Register("Dome_Center.y",&Center.y);
	access.Register("Dome_Center.z",&Center.z);
	access.Register("Dome_ApexDirection.x",&ApexDirection.x);
	access.Register("Dome_ApexDirection.y",&ApexDirection.y);
	access.Register("Dome_ApexDirection.z",&ApexDirection.z);
	access.Register("needsrecompile",&needsrecompile);
}


#ifdef ELUMENATI_INTERNAL
const char *OM_DomeScreen::getScreenType()
{
	return("dome");
}
void OM_DomeScreen::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapeDome::new(\"dome\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializeDomeScreen(" << name << ")\n\n";

}
#endif


void *OM_DomeScreen::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
									 ScreenGeometryBase::ShapeType &shapeType, int &aNumShapes)
{
	if (needsrecompile)
	{
		CreateGeometry();
		renderer->ResetGeometry(vertexBuffer, ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT, ScreenGeometryBase::VBT_TRIANGLES,
			&vertexCount, 1);
		needsrecompile = false;
	}
	*numVertices = &vertexCount;
	vertType = vertexType;
	aNumShapes = 1;
	shapeType = ScreenGeometryBase::VBT_TRIANGLES;
	return vertexBuffer;
}



void OM_DomeScreen::Display(void *context)
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



void OM_DomeScreen::Display()
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



bool OM_DomeScreen::CreateGeometry()
{
	// Ok, so this isnt going to be exact, but it will create enough space for all of the vertices.
	int maxNumQuads = (tesselation-1)*(tesselation-1);
	//float inverse = (1.0/((float)tesselation-1.0));
	//int maxNumTris = (inverse*(tesselation*tesselation))+(.1*tesselation);
  EH_DECLARE;

	// Create enough memory for the vertices and indices
	EH_Ptr(vertexBuffer = new ScreenGeometryBase::SimpleVertex[(maxNumQuads * 2 * 3)]);

  createMeshGeometry(vertexBuffer);

  EH_OnError() {
    if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
    return false;
  }

  return true;
}

//#define Saturate(x) ((x)>=0?((x)>1?1:(x)):(0))

bool OM_DomeScreen::xyTo3D(float x, float y, float *xyz)
{
#ifdef notdef
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
#endif	
		OmniVec3 gl_Position;	
		gl_Position.x = x;
		gl_Position.y = y;
		//float x2 = x*x;
		//float y2 = y*y;
		//float xy2 = x2 - y2;
		//float xy2_1 = 1.0 - xy2;
		float xy2_2 = ((x*x)+(y*y))-1.0;
		//float xy2_3 = 1.0-(x*x)-(y*y);
		//gl_Position.z = sqrtf(xy2_1);
		gl_Position.z = -sqrtf(fabs(xy2_2));
		
		//gl_Position.w =1.0;
		//gl_Position.z =-D/1000.0;   // bug fix 2b fix for reverse z depth
		if (gl_Position.z > this->Radius)
			return false;
		xyz[0] = gl_Position.x;
		xyz[1] = gl_Position.y;
		xyz[2] = gl_Position.z;
		xyz[3] = 0.0;
		return(true);
}




OmniVec3 project2DPointUpYToUnitDome(OmniVec3 pointIn)
{
	pointIn.y = 1.0 - sqrtf(pointIn.x*pointIn.x+pointIn.z*pointIn.z);
	return pointIn;
}



bool IsAnyPartOfTriangleInsideCircle(OmniVec3 pointIn1,OmniVec3 pointIn2,OmniVec3 pointIn3)
{
	if(pointIn1.length() <= 1.0000001)
		return true;
	else if(pointIn2.length() <= 1.0000001)
		return true;
	else if(pointIn3.length() <= 1.0000001)
		return true;
	else return false;
}



OmniVec3 PullInsideCircle(OmniVec3 pointIn)
{
	if(pointIn.length() <= 1.0000001)
		return pointIn;
	else
	{
		return pointIn.returnNormalized();
	}
}



ScreenGeometryBase::SimpleVertex OmniVec3ToSimpleVertex(OmniVec3 in)
{
	ScreenGeometryBase::SimpleVertex o;
	o.x = in.x;
	o.y = in.y;
	o.z = in.z;
	return o;

}



float SquareSmoothstep(float x)
{
	float xp = x;
	x = 1.0-x;
	x = x*x;
	x = 1.0-x;
	return (xp + x)*.5;
}


OmniVec3 SmoothUnitVec(OmniVec3 in)
{
	float l = in.length();
	float newL = SquareSmoothstep(l);
	in = in * newL/l;
	return in;
}


void OM_DomeScreen::createMeshGeometry(ScreenGeometryBase::SimpleVertex *vertices)
{
	// display a dome using opengl...
	// standard way of doing it....
	// have a basic unit dome, and have a matrix move and rotate it....

	// generate the 4x4 matrix for moving it...
	ScreenGeometryBase::SimpleVertex *vertP = vertices;
	vertexCount = 0;
	numTriangles = 0;
	OmniVec3 DefaultI(1,0,0);
	OmniVec3 DefaultJ(0,1,0);
	OmniVec3 DefaultK(0,0,1);

	OmniVec3 NewJ = ApexDirection;
	OmniVec3 NewI = OmniVec3::MakePerpendicluarVector(NewJ);
	OmniVec3 NewK = NewI.cross(NewJ);

	//double domeradius= Radius;
	

	//Type = GL_POINTS;
	


	for ( int i = 0; i < tesselation - 1; i++)
	{
		for ( int j = 0; j < tesselation - 1; j++)
		{
			float b = (i	/(double)(tesselation - 1.0)  *2.0 -1.0);
			float t = ((i+1)/(double)(tesselation - 1.0)  *2.0 -1.0);
			float l = (j	/(double)(tesselation - 1.0)  *2.0 -1.0);
			float r = ((j+1)/(double)(tesselation - 1.0)  *2.0 -1.0);


			OmniVec3 bl= OmniVec3(b,0.0f,l);
			OmniVec3 tl= OmniVec3(t,0.0f,l);			     
			OmniVec3 tr= OmniVec3(t,0.0f,r);
			OmniVec3 br= OmniVec3(b,0.0f,r);
			
	


			if(IsAnyPartOfTriangleInsideCircle(bl,tl,tr))
			{
				OmniVec3 blPrime = (PullInsideCircle(bl));
				OmniVec3 tlPrime = (PullInsideCircle(tl));
				OmniVec3 trPrime = (PullInsideCircle(tr));

				if(true)
				{
					blPrime = SmoothUnitVec(blPrime);
					tlPrime = SmoothUnitVec(tlPrime);
					trPrime = SmoothUnitVec(trPrime);
				}	
				blPrime = project2DPointUpYToUnitDome(blPrime);
				tlPrime = project2DPointUpYToUnitDome(tlPrime);
				trPrime = project2DPointUpYToUnitDome(trPrime);


				blPrime  = OmniVec3(blPrime .dot(NewI),blPrime .dot(NewJ),blPrime .dot(NewK))*Radius+Center;
				tlPrime  = OmniVec3(tlPrime .dot(NewI),tlPrime .dot(NewJ),tlPrime .dot(NewK))*Radius+Center;
				trPrime  = OmniVec3(trPrime .dot(NewI),trPrime .dot(NewJ),trPrime .dot(NewK))*Radius+Center;

				*(vertP++) = OmniVec3ToSimpleVertex(blPrime);
				*(vertP++) = OmniVec3ToSimpleVertex(tlPrime);
				*(vertP++) = OmniVec3ToSimpleVertex(trPrime);
				numTriangles += 1;vertexCount += 3;
			}
			if(IsAnyPartOfTriangleInsideCircle(tr,br,bl))
			{
				OmniVec3 blPrime = (PullInsideCircle(bl));
				OmniVec3 trPrime = (PullInsideCircle(tr));
				OmniVec3 brPrime = (PullInsideCircle(br));

				if(true)
				{
					blPrime = SmoothUnitVec(blPrime);
					trPrime = SmoothUnitVec(trPrime);
					brPrime = SmoothUnitVec(brPrime);
				}	
				blPrime = project2DPointUpYToUnitDome(blPrime);
				trPrime = project2DPointUpYToUnitDome(trPrime);
				brPrime = project2DPointUpYToUnitDome(brPrime);


				blPrime  = OmniVec3(blPrime.dot(NewI),blPrime.dot(NewJ),blPrime.dot(NewK))*Radius+Center;
				trPrime  = OmniVec3(trPrime.dot(NewI),trPrime.dot(NewJ),trPrime.dot(NewK))*Radius+Center;
				brPrime  = OmniVec3(brPrime.dot(NewI),brPrime.dot(NewJ),brPrime.dot(NewK))*Radius+Center;
				*(vertP++) = OmniVec3ToSimpleVertex(trPrime);
				*(vertP++) = OmniVec3ToSimpleVertex(brPrime);
				*(vertP++) = OmniVec3ToSimpleVertex(blPrime);
				numTriangles += 1;vertexCount += 3;
			}
		}
	}
}