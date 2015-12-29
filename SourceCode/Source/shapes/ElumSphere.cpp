/*
 *  ElumSphere.cpp
 *  ElumSphere
 *
 *  Created by Jim Terhorst on 3/24/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif
#include "ElumSphere.h"
#ifdef __APPLE__
#include "OmniMapAPI/ScreenRendererOGL.h"
#include "OmniMapAPI/omnimap.h"
#else
#include "OmnimapBase.h"
#endif
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif
#include "OM_ErrorHandling.h"


#pragma warning (disable:4244)


#ifdef notdef
ElumSphere::ElumSphere()
{
	radius = 1.0;
	tesselation = 100;
	initialized = false;
}

void ElumSphere::initialize(float _radius, int _tesselation, OmniMapQuartz *om)
{
	if (initialized)
		return;
	initialized = true;
	radius = _radius;
	tesselation = _tesselation;
	
	sphereShape = new SphereShape();
	sphereShape->SetRenderer(new ScreenRendererOGL());
	sphereShape->access.SetValue("Dome_tesselation", tesselation);
	sphereShape->access.SetValue("Dome_Radius", radius);
	sphereShape->access.SetValue("Dome_ApexDirection.x", 0.0f);
	sphereShape->access.SetValue("Dome_ApexDirection.y", 0.0f);
	sphereShape->access.SetValue("Dome_ApexDirection.z", -1.0f);
	sphereShape->access.SetValue("needsrecompile", true);
	omnimap = om;

}


void ElumSphere::setRadius(float r)
{
	radius = r;
	sphereShape->access.SetValue("Dome_Radius", radius);
	sphereShape->access.SetValue("needsrecompile", true);
}

void ElumSphere::Display()
{

	glPushAttrib(GL_ALL_ATTRIB_BITS);
#ifdef notdef
	if (drawMode == WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glClientActiveTexture(GL_TEXTURE4);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0,0,350,350);
	
	glOrtho(-1.0, 1.0, -1.0, 1.0, .001, 100.00);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt (0.0,0.0,1.0,
				0.0,0.0,-1.0,
				0.0,1.0, 0.0);
	
	sphereShape->Display();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
#endif
	gluLookAt (0.0,0.0,1.0,
				0.0,0.0,-1.0,
				0.0,1.0, 0.0);
	glClientActiveTexture(GL_TEXTURE4);

	for (int i = 0; i < omnimap->GetNumChannels(); i++)
	{
	
		OmniMapChannelQuartz *channel = (OmniMapChannelQuartz *) omnimap->GetChannel(i);
		if (channel->channelType == CT_OMNIMAP)
		{
			channel->beginRenderToChannel();
			sphereShape->Display();
			channel->endRenderToChannel();
		}
	
	}
	glPopAttrib();

	
}
#endif

OmniMap_Screen *SphereShape::CreateElumSphere()
{
	return (new SphereShape());
}

SphereShape::SphereShape()
{
	hasTextureCoords = false;
  vertexBuffer = NULL;
  texVertexBuffer = NULL;
  renderer = NULL;
}


SphereShape::~SphereShape()
{
  if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
  if (texVertexBuffer) { delete[] texVertexBuffer; texVertexBuffer = NULL; }
  if (renderer) { delete renderer; renderer = NULL; } // this should be on ScreenGeometryBase destructor (must test if defining the destructor breaks API compatibility) 
}


#ifdef ELUMENATI_INTERNAL
const char *SphereShape::getScreenType()
{
	return("sphere");
}
void SphereShape::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapeDome:new(\"sphere\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializeDomeScreen(" << name << ")\n\n";

}
#endif


void SphereShape::Display(void *context)
{
	if(needsrecompile)
	{
		CreateGeometry();
		if (hasTextureCoords)
			renderer->ResetGeometry(texVertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP,
				vertexCounts, numShapes);
		else
			renderer->ResetGeometry(vertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP,
				vertexCounts, numShapes);
		needsrecompile = false;
	}
	renderer->Display(context);
}

void SphereShape::Display()
{
	if(needsrecompile)
	{
		CreateGeometry();
		if (hasTextureCoords)
			renderer->ResetGeometry(texVertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP,
				vertexCounts, numShapes);
		else
			renderer->ResetGeometry(vertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP,
				vertexCounts, numShapes);
		needsrecompile = false;
	}

	renderer->Display(NULL);

}


bool SphereShape::CreateGeometry()
{
	// Ok, so this isnt going to be exact, but it will create enough space for all of the vertices.
	//int maxNumQuads = (tesselation-1)*(tesselation-1);
	//float inverse = (1.0f/((float)tesselation-1.0f));
	// Create enough memory for the vertices and indices
	int numVerts = (tesselation+2) * ((tesselation/2.0)+2)+20;
  EH_DECLARE;

	if (hasTextureCoords)
	{
		EH_Ptr(texVertexBuffer = new ScreenGeometryBase::TextureCoordVertex[numVerts]);
		vertexType = ScreenGeometryBase::VBT_VERTEX_TEXCOORD_FLT;
		createMeshGeometry3(texVertexBuffer);
	} else
	{
		EH_Ptr(vertexBuffer = new ScreenGeometryBase::SimpleVertex[numVerts]);
		vertexType = ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT;
		createMeshGeometry3(vertexBuffer);
	}
   
  EH_OnError() {
    if (texVertexBuffer) { delete[] texVertexBuffer; texVertexBuffer = NULL; }
    if (vertexBuffer) { delete[] vertexBuffer; vertexBuffer = NULL; }
    return false;
  }

  return true;
}



void *SphereShape::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
										 ScreenGeometryBase::ShapeType &aShapeType, int &aNumShapes)
{
	if (needsrecompile)
	{
		CreateGeometry();
		if (hasTextureCoords)
			renderer->ResetGeometry(texVertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP,
				vertexCounts, numShapes);
		else
			renderer->ResetGeometry(vertexBuffer, vertexType, ScreenGeometryBase::VBT_TRIANGLE_STRIP,
				vertexCounts, numShapes);
		needsrecompile = false;
	}
	*numVertices = vertexCounts;
	vertType = vertexType;
	aNumShapes = numShapes;
	aShapeType = ScreenGeometryBase::VBT_TRIANGLE_STRIP;
	if (hasTextureCoords)
		return (texVertexBuffer);
	else
		return (vertexBuffer);
}

#ifdef notdef
#define Band_Power  6  // 2^Band_Power = Total Points in a band.
#define Band_Points 64.0 // 16 = 2^Band_Power
#define Band_Mask ((int)(Band_Points-2.0))
#define Sections_In_Band ((Band_Points/2.0)-1.0)
#define Total_Points (Sections_In_Band*Band_Points) 
// remember - for each section in a band, we have a band
#define Section_Arc (6.28/Sections_In_Band)
 // radius of 10
void SphereShape::createMeshGeometry2(ScreenGeometryBase::TextureCoordVertex *vertices) 
{
	int i;
	float x_angle;
	float y_angle;
	const float R = -Radius;
	vertexCount = 0;
	for (i=0;i<Total_Points;i++)
	{  
		// using last bit to alternate,+band number (which band)
		x_angle=(float)(i&1)+(i>>Band_Power);  
    
		// (i&Band_Mask)>>1 == Local Y value in the band
		// (i>>Band_Power)*((Band_Points/2)-1) == how many bands
		//  have we processed?
		// Remember - we go "right" one value for every 2 points.
		//  i>>bandpower - tells us our band number
		y_angle=(float)((i&Band_Mask)>>1)+((i>>Band_Power)*(Sections_In_Band));
 
		x_angle*=(float)Section_Arc/2.0f; // remember - 180° x rot not 360
		y_angle*=(float)Section_Arc*-1; 
		
		vertices[i].x = R*sin(x_angle)*sin(y_angle);
		vertices[i].z = -R*cos(x_angle);
		vertices[i].y = R*sin(x_angle)*cos(y_angle);
		vertices[i].u = (vertices[i].x*.5) + .5;
		vertices[i].v = (vertices[i].y*.5) + .5;
		vertexCount++;
    
	}
}
#endif
#define PI 3.14159265
void SphereShape::createMeshGeometry3(void *vertexBuffer)
{
	OmniVec3 NewJ = ApexDirection;
	OmniVec3 NewI = OmniVec3::MakePerpendicluarVector(NewJ);
	OmniVec3 NewK = NewI.cross(NewJ);
	ScreenGeometryBase::SimpleVertex *simpleVB = NULL;
	ScreenGeometryBase::TextureCoordVertex *texCoordVB = NULL;
	if (vertexType == ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT)
	{
		simpleVB = (ScreenGeometryBase::SimpleVertex *) vertexBuffer;
	} else {
		texCoordVB = (ScreenGeometryBase::TextureCoordVertex *) vertexBuffer;
	}

	double incr = 360.0/tesselation;
	//sdouble incr = 2.0;
	int vertNum = 0;
	Radius = 1.0;
	int numShapesPred = (90/(int) incr) + 1;
	vertexCounts = new int[numShapesPred];
	numShapes = 0;
	//double lastLat;
	for (double lat = 90.0; lat >= 0.0; lat -=incr)
	{
		vertexCounts[numShapes] = 0;
		double nextLat = lat - incr;
		if ((nextLat) < 0.0)
			nextLat = 0.0;
		for (double lon = -180.0; lon <= 180.0 + incr; lon += incr)
		{
			if (lon > 180.0)
				lon = 180.0;
			OmniVec3 v1(Radius * cos(lat *PI/180.0) * cos(lon*PI/180.0),
						Radius * sin(lat * PI/180.0),
						Radius * cos(lat * PI/180.0) * sin(lon * PI/180.0));
			v1 = OmniVec3(v1.dot(NewI),v1.dot(NewJ),v1.dot(NewK))+Center;
			
						
			OmniVec3 v2(Radius * cos((nextLat) *PI/180.0) * cos((lon)*PI/180.0),
				Radius * sin((nextLat) * PI/180.0),
				Radius * cos((nextLat) * PI/180.0) * sin((lon) * PI/180.0));
			v2 = OmniVec3(v2.dot(NewI),v2.dot(NewJ),v2.dot(NewK))+Center;
					
						
			if (hasTextureCoords)
			{
				texCoordVB[vertNum].x = (float) v2.x;
				texCoordVB[vertNum].y = (float) v2.y;
				texCoordVB[vertNum].z = (float) v2.z;
				float r2 = 1.0 - ((nextLat)/90.0);
				float u = (float) cos(lon*(PI/180.0)) * r2;
				float v = (float) sin(lon*(PI/180.0)) * r2;
				texCoordVB[vertNum].u = (u * .5) + .5;
				texCoordVB[vertNum].v = (v * .5) + .5;
			} else
			{
				simpleVB[vertNum].x = (float) v2.x;
				simpleVB[vertNum].y = (float) v2.y;
				simpleVB[vertNum].z = (float) v2.z;
			}
						
			vertNum++;
			vertexCounts[numShapes]++;
			
			if (hasTextureCoords)
			{
				texCoordVB[vertNum].x = (float) v1.x;
				texCoordVB[vertNum].y = (float) v1.y;
				texCoordVB[vertNum].z = (float) v1.z;
				float r = 1.0 - (lat/90.0);
				float u = (float) cos(lon*(PI/180.0)) * r;
				float v = (float) sin(lon*(PI/180.0)) * r;
				texCoordVB[vertNum].u = (u * .5) + .5;
				texCoordVB[vertNum].v = (v * .5) + .5;
			} else
			{
				simpleVB[vertNum].x = (float) v1.x;
				simpleVB[vertNum].y = (float) v1.y;
				simpleVB[vertNum].z = (float) v1.z;
			}

			vertNum++;
			vertexCounts[numShapes]++;
			if (lon == 180.0)
				lon = 180.0 + (incr*2.0);


			
		}
	
		numShapes++;
	}
//	int i = 10;
	return;
}

#ifdef notdef
#define Saturate(x) ((x)>=0?((x)>1?1:(x)):(0))
void SphereShape::createMeshGeometry(ScreenGeometryBase::TextureCoordVertex *vertices)
{
	// display a dome using opengl...
	// standard way of doing it....
	// have a basic unit dome, and have a matrix move and rotate it....

	// generate the 4x4 matrix for moving it...
	ScreenGeometryBase::TextureCoordVertex *vertP = vertices;
	vertexCount = 0;
	numTriangles = 0;
	OmniVec3 DefaultI(1,0,0);
	OmniVec3 DefaultJ(0,1,0);
	OmniVec3 DefaultK(0,0,1);

	OmniVec3 NewJ = ApexDirection;
	OmniVec3 NewI = OmniVec3::MakePerpendicluarVector(NewJ);
	OmniVec3 NewK = NewI.cross(NewJ);
	double domeradius= Radius;
	

	//Type = GL_POINTS;
	


	for ( int i = 0; i < tesselation - 1; i++)
	{
		for ( int j = 0; j < tesselation - 1; j++)
		{
			OmniVec3 bl= OmniVec3((float) (i/(double)(tesselation - 1.0)  *2.0 -1.0),
				(float)0.0,
				((float)(j/(double)(tesselation - 1.0)	)	*2.0-1.0));		 
			OmniVec3 tl= OmniVec3((float)((i+1)/(double)(tesselation - 1.0)  *2.0 -1.0),
				(float)0.0,
				((float)j/(double)(tesselation - 1.0)	)	*2.0-1.0);			     
			OmniVec3 tr= OmniVec3( (float) ((i+1)/(double)(tesselation - 1.0)  *2.0 -1.0),
				(float)0.0,
				((float)((j+1)/(double)(tesselation - 1.0))*2.0-1.0));
			OmniVec3 br= OmniVec3((float)(i/(double)(tesselation - 1.0)  *2.0 -1.0),
				(float)0.0,
				((float)(j+1)/(double)(tesselation - 1.0))	*2.0-1.0);				 


			bl.y =  sqrt(Saturate(domeradius*domeradius-bl.x*bl.x - bl.z*bl.z));
			tl.y =  sqrt(Saturate(domeradius*domeradius-tl.x*tl.x - tl.z*tl.z));
			tr.y = sqrt(Saturate(domeradius*domeradius-tr.x*tr.x - tr.z*tr.z));
			br.y =  sqrt(Saturate(domeradius*domeradius-br.x*br.x - br.z*br.z));

			double radius_forgiveness = .1; 
			double radius = 1.0+radius_forgiveness;
			if((bl.x*bl.x+bl.z*bl.z) > (radius ))
				goto docorner;
			if((tl.x*tl.x+tl.z*tl.z) > (radius ))
				goto docorner;
			if((tr.x*tr.x+tr.z*tr.z) > (radius))
				 goto docorner;
			if((br.x*br.x+br.z*br.z) > (radius))
					 goto docorner;
good:
			bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
			tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
			tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
			br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;

			vertP->u = (bl.x * .5f) + .5f;;
			vertP->v = (bl.y * .5f) + .5f;
			vertP->x = bl.x;
			vertP->y = bl.y;
			vertP++->z = bl.z;
			
			vertP->u = (tl.x * .5f) + .5f;;
			vertP->v = (tl.y * .5f) + .5f;
			vertP->x = tl.x;
			vertP->y = tl.y;
			vertP++->z = tl.z;
			
			vertP->u = (tr.x * .5f) + .5f;;
			vertP->v = (tr.y * .5f) + .5f;
			vertP->x = tr.x;
			vertP->y = tr.y;
			vertP++->z = tr.z;

			vertP->u = (tr.x * .5f) + .5f;;
			vertP->v = (tr.y * .5f) + .5f;
			vertP->x = tr.x;
			vertP->y = tr.y;
			vertP++->z = tr.z;
			
			vertP->u = (br.x * .5f) + .5f;;
			vertP->v = (br.y * .5f) + .5f;
			vertP->x = br.x;
			vertP->y = br.y;
			vertP++->z = br.z;
			
			vertP->u = (bl.x * .5f) + .5f;;
			vertP->v = (bl.y * .5f) + .5f;
			vertP->x = bl.x;
			vertP->y = bl.y;
			vertP++->z = bl.z;
			numTriangles += 2;
			vertexCount += 6;
			continue;

docorner:
			if(	((tl.x*tl.x+tl.z*tl.z) <= (radius))&&
				((tr.x*tr.x+tr.z*tr.z) <= (radius))&&
				((br.x*br.x+br.z*br.z) <= (radius)))
			{
	

				bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
				tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
				tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
				br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;
		
				vertP->u = (tl.x * .5) + .5;;
				vertP->v = (tl.y * .5) + .5;
				vertP->x = tl.x;
				vertP->y = tl.y;
				vertP++->z = tl.z;
				
				vertP->u = (tr.x * .5) + .5;;
				vertP->v = (tr.y * .5) + .5;
				vertP->x = tr.x;
				vertP->y = tr.y;
				vertP++->z = tr.z;
				
				vertP->u = (br.x * .5) + .5;;
				vertP->v = (br.y * .5) + .5;
				vertP->x = br.x;
				vertP->y = br.y;
				vertP++->z = br.z;
				numTriangles++;
				vertexCount += 3;

			}

			
			if(
				((bl.x*bl.x+bl.z*bl.z) <= (radius)) &&
				((tr.x*tr.x+tr.z*tr.z) <= (radius))&&
				((br.x*br.x+br.z*br.z) <= (radius)))
			{

				bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
				tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
				tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
				br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;

				vertP->u = (bl.x * .5) + .5;;
				vertP->v = (bl.y * .5) + .5;
				vertP->x = bl.x;
				vertP->y = bl.y;
				vertP++->z = bl.z;
				
				vertP->u = (tr.x * .5) + .5;;
				vertP->v = (tr.y * .5) + .5;
				vertP->x = tr.x;
				vertP->y = tr.y;
				vertP++->z = tr.z;
				
				vertP->u = (br.x * .5) + .5;;
				vertP->v = (br.y * .5) + .5;
				vertP->x = br.x;
				vertP->y = br.y;
				vertP++->z = br.z;
				numTriangles++;
				vertexCount += 3;
			}

			
			if(
				((bl.x*bl.x+bl.z*bl.z) <= (radius)) &&
				((tl.x*tl.x+tl.z*tl.z) <= (radius))&&
				((br.x*br.x+br.z*br.z) <= (radius)))
			{

				bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
				tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
				tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
				br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;

				vertP->u = (bl.x * .5) + .5;;
				vertP->v = (bl.y * .5) + .5;
				vertP->x = bl.x;
				vertP->y = bl.y;
				vertP++->z = bl.z;
				
				vertP->u = (tl.x * .5) + .5;;
				vertP->v = (tl.y * .5) + .5;
				vertP->x = tl.x;
				vertP->y = tl.y;
				vertP++->z = tl.z;
				
				vertP->u = (br.x * .5) + .5;;
				vertP->v = (br.y * .5) + .5;
				vertP->x = br.x;
				vertP->y = br.y;
				vertP++->z = br.z;
				numTriangles++;
				vertexCount += 3;
			}

			
			if(
				((bl.x*bl.x+bl.z*bl.z) <= (radius)) &&
				((tl.x*tl.x+tl.z*tl.z) <= (radius))&&
				((tr.x*tr.x+tr.z*tr.z) <= (radius)))
			{
				
				bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
				tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
				tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
				br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;
				
				vertP->u = (bl.x * .5) + .5;;
				vertP->v = (bl.y * .5) + .5;
				vertP->x = bl.x;
				vertP->y = bl.y;
				vertP++->z = bl.z;

				vertP->u = (tl.x * .5) + .5;;
				vertP->v = (tl.y * .5) + .5;
				vertP->x = tl.x;
				vertP->y = tl.y;
				vertP++->z = tl.z;

				vertP->u = (tr.x * .5) + .5;;
				vertP->v = (tr.y * .5) + .5;
				vertP->x = tr.x;
				vertP->y = tr.y;
				vertP++->z = tr.z;
				numTriangles++;
				vertexCount += 3;
			}
		}
	}
}
#endif




