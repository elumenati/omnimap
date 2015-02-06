#include "OM_CylinderScreen.h"



/// move to the c+++ file

#include <string>
#include <map>
#include <math.h>
#ifdef ELUMENATI_INTERNAL
#include <iostream>
#include <fstream>
#endif

OmniMap_Screen *OM_CylinderScreen::CreateOMCylinder()
{
	return (new OM_CylinderScreen());
}


OmniVec3 OM_CylinderScreen::CylindricalRadiansToCartisean(float r, float theta, float h)
{
	return OmniVec3 ( r*cos(theta),r*sin(theta), h);
}

OmniVec3 OM_CylinderScreen::CylindricalRadiansToCartisean(float r, float theta, float h,
									OmniVec3 Center,  
									OmniVec3 Up,  
									OmniVec3 Foward
									)
{
	OmniVec3 centered = CylindricalRadiansToCartisean(r,theta,h);
//	OmniVec3 normfoward = CylindricalRadiansToCartisean(1,0,0);

//	float angle = centered.angleBetween(normfoward);
//	centered.rotateByVector(OmniVec3(0,0,1),  angle);  // maybe negative?

	OmniVec3 X(-Foward.x,-Foward.y,-Foward.z);
	OmniVec3 Z = Up; //
	OmniVec3 Y =Up.cross(Foward); 

	OmniVec3 ret;
	ret.x = Z.dot(centered);
	ret.y = X.dot(centered);
	ret.z = Y.dot(centered);

	ret.add(Center);

	return ret;
}

void *OM_CylinderScreen::GetVertexBuffer(int **numVertices, ScreenGeometryBase::VertexType &vertType,
										 ScreenGeometryBase::ShapeType &aShapeType, int &aNumShapes)
{
	*numVertices = vertexCounts;
	vertType = vertexType;
	aNumShapes = numShapes;
	aShapeType = ScreenGeometryBase::VBT_TRIANGLE_STRIP;
	return vertexBuffer;
}


bool OM_CylinderScreen::CreateGeometry()
{
	
	vertexCounts = new int[(iTessel+1)];
	numShapes = 0;
	int vertexCount = (iTessel+1)*(iTessel+1)*2;
	
	// Create enough memory for the vertices and indices
	if (RenderTexCoordinates) {
		vertexBuffer = (void *) new ScreenGeometryBase::TextureCoordVertex[vertexCount];
		vertexType = ScreenGeometryBase::VBT_VERTEX_TEXCOORD_FLT;
	} else {
		vertexBuffer = (void *) new ScreenGeometryBase::SimpleVertex[vertexCount];
		vertexType = ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT;
	}
    if(!vertexBuffer)
        return false;
	
    createMeshGeometry((void *) vertexBuffer);

	return true;


}

bool OM_CylinderScreen::xyTo3D(float x, float y, float *xyz)
{
	return false;
}

////////////////////////////////////////////////////////////
void OM_CylinderScreen::createMeshGeometry(void *vertexBuffer)
{
	ScreenGeometryBase::SimpleVertex *simpleVB;
	ScreenGeometryBase::TextureCoordVertex *texCoordVB;
	if (vertexType == ScreenGeometryBase::VBT_SIMPLE_VERTEX_FLT)
	{
		simpleVB = (ScreenGeometryBase::SimpleVertex *) vertexBuffer;
	} else {
		texCoordVB = (ScreenGeometryBase::TextureCoordVertex *) vertexBuffer;
	}
	//vertexCount = 0;
	for(int iTheta =0; iTheta <= iTessel; iTheta++)
	{

		vertexCounts[iTheta] = (iTessel+1)*2; 
		numShapes++;
		for(int iH = 0; iH <= iTessel; iH++)
		{

			float localU0 = iTheta / (float)(iTessel+1);
			float localU1 = (iTheta +1 )/ (float)(iTessel+1);
			float localV = (float)iH / (float)(iTessel);

			float fTheta = localU0*ThetaMin + (1-localU0)*ThetaMax;
			float fTheta2 = localU1*ThetaMin + (1-localU1)*ThetaMax;

			float fH = Height* localV;


			OmniVec3 bl =CylindricalRadiansToCartisean(Radius,fTheta,fH,OriginBottom,Up,Foward);
			OmniVec3 br =CylindricalRadiansToCartisean(Radius,fTheta2,fH,OriginBottom,Up,Foward);


			float s1  = (float) blU+(1.0f-localU0)*widthU;
			float t1  = (float) blV+(1.0f-localV)*heightV;
			float s2  = (float) blU+(1.0f-localU1)*widthU;
			//vertexCount += 2;
			if(RenderTexCoordinates)
			{
				texCoordVB->x = bl.x;
				texCoordVB->y = bl.y;
				texCoordVB->z = bl.z;
				texCoordVB->u = s1;
				texCoordVB->v = t1;
				texCoordVB++;
				texCoordVB->x = br.x;
				texCoordVB->y = br.y;
				texCoordVB->z = br.z;
				texCoordVB->u = s2;
				texCoordVB->v = t1;
				texCoordVB++;

			} else
			{
				simpleVB->x = bl.x;
				simpleVB->y = bl.y;
				simpleVB->z = bl.z;
				simpleVB++;
				simpleVB->x = br.x;
				simpleVB->y = br.y;
				simpleVB->z = br.z;
				simpleVB++;
			}


				


			//				LogSystem()->ReportMessage("%f %f %f",br.x,br.y,br.z);
		}

	}


}

void OM_CylinderScreen::SetUpPropertyAccess()
{
	LogSystem()->ReportMessage("derived OM_CylinderScreen::SetUpPropertyAccess()");

	access.Register("needsrecompile",&needsrecompile);
	access.Register("iTessel",&iTessel);
	access.Register("ThetaMin",  &ThetaMin);
	access.Register("ThetaMax",  &ThetaMax);
	access.Register("Height",  &Height);
	access.Register("Radius",  &Radius);
	access.Register("OriginBottom.x",&OriginBottom.x);
	access.Register("OriginBottom.y",&OriginBottom.y);
	access.Register("OriginBottom.z",&OriginBottom.z);
	access.Register("Up.x",&Up.x);
	access.Register("Up.y",&Up.y);
	access.Register("Up.z",&Up.z);		
	access.Register("Foward.x",&Foward.x);
	access.Register("Foward.y",&Foward.y);
	access.Register("Foward.z",&Foward.z);		
	access.Register("needsrecompile",&needsrecompile);
//
	access.Register("ColorRGB.r",&ColorRGB[0]);
	access.Register("ColorRGB.g",&ColorRGB[1]);
	access.Register("ColorRGB.b",&ColorRGB[2]);

	access.Register("RenderTexCoordinates",&RenderTexCoordinates);
	access.Register("blU",&blU);
	access.Register("blV",&blV);
	access.Register("widthU",&widthU);
	access.Register("heightV",&heightV);


}

#ifdef ELUMENATI_INTERNAL
const char *OM_CylinderScreen::getScreenType()
{
	return("cylinder");
}
void OM_CylinderScreen::WriteObject(void *filePtr, char *name)
{
	std::ofstream *configFile = (std::ofstream *) filePtr;

	*configFile << name << " = ClassScreenShapeCylinder:new(\"cylinder\", \"" << name << "\")\n";
	access.writeVariables(filePtr, name);
	*configFile << "InitializeCylinderScreen(" << name << ")\n\n";

}
#endif

OM_CylinderScreen::OM_CylinderScreen()
{
	needsrecompile =true;
	iTessel = 100;
	SetUpPropertyAccess();
	LogSystem()->ReportMessage("new OmniMap_Screen:OM_CylinderScreen");
	RenderTexCoordinates = false;
	for(int i =0; i!=3;i++)
		ColorRGB[i]=1;
}

OM_CylinderScreen::~OM_CylinderScreen()
{
	
	LogSystem()->ReportMessage("delete OmniMap_Screen:OM_CylinderScreen");
	if (renderer != NULL)
	{
		delete renderer;
		renderer = NULL;
	}
}


void OM_CylinderScreen::Display(void *context)
{
	if(needsrecompile)
	{
		needsrecompile =false;
		bool hr = CreateGeometry();
		
	}
	renderer->ResetGeometry(vertexBuffer, vertexType, 
			ScreenGeometryBase::VBT_TRIANGLE_STRIP, vertexCounts, numShapes);
	renderer->Display(context);
}



OM_CylinderScreen::OM_CylinderScreen(const OM_CylinderScreen& p) 
{
	
	needsrecompile =true;
	//		int DisplayList;  created in baseinit

	for(int i =0; i!=3;i++)
		ColorRGB[i]=p.ColorRGB[i];

	OriginBottom = p.OriginBottom;
	Up = p.Up;
	Foward = p.Foward;

	Height  = p.Height;
	Radius  = p.Radius;
	iTessel  = p.iTessel ;
	ThetaMin  = p.ThetaMin ;
	ThetaMax  = p.ThetaMax;
	RenderTexCoordinates  = p.RenderTexCoordinates;
	blU  = p.blU;
	blV  = p.blV;
	widthU  = p.widthU;
	heightV  = p.heightV;


}

OM_CylinderScreen::OM_CylinderScreen(OM_CylinderScreen const * p) 
{


	for(int i =0; i!=3;i++)
			ColorRGB[i]=p->ColorRGB[i];

	needsrecompile =true;
	//		int DisplayList;  created in baseinit
	OriginBottom = p->OriginBottom;
	Up = p->Up;
	Foward = p->Foward;

	Height= p->Height;
	Radius= p->Radius;
	iTessel = p->iTessel ;
	ThetaMin = p->ThetaMin ;
	ThetaMax= p->ThetaMax;
	RenderTexCoordinates= p->RenderTexCoordinates;
	blU= p->blU;
	blV= p->blV;
	widthU= p->widthU;
	heightV= p->heightV;


}

void OM_CylinderScreen::SetTextureCoordinates(float bu,float bv, float w,float h)
{
	RenderTexCoordinates = true;
	blU =bu;
	blV =bv;
	widthU =w;
	heightV =h;
	needsrecompile=true;
}