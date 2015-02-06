#include <string>
#include <map>
#include <math.h>

#include "screenshapefactory.h"

class OmniMap_Screen_Quad;

typedef std::map<std::string,fpScreenCreatorFunction> String_fpScreenCreatorFunction_MAP;

	void OmniMap_Screen_Quad::SetTextureCoordinates(float bu,float bv, float w,float h)
	{
		RenderTexCoordinates = true;
		 blU = bu;
		 blV =bv;
		 widthU =w;
		 heightV =h;
	}

/*

///////////////////////////////////////////////////////////
////////OmniMap_Screen_Quad
////////////////////////////////////////////////////////////
class OMNIMAP_API   OmniMap_Screen_Quad : public OmniMap_Screen
{
private:  // 
	bool needsrecompile;
	int DisplayList;

	void OmniMap_Screen_Quad::BaseInit()
{
	LogSystem()->ReportMessage("new OmniMap_Screen:base");// not so accurate message?!?!!?
	needsrecompile = true;

	access.Register("needsrecompile",&needsrecompile);
	DisplayList = glGenLists(1);
}
void OmniMap_Screen_Quad::BaseDestroy()
{
	LogSystem()->ReportMessage("delete OmniMap_Screen:base");// not so accurate message?!?!!?
	glDeleteLists(DisplayList,1);
}




void OmniMap_Screen_Quad::Display()
{
	if(needsrecompile)
	{
		needsrecompile =false;
		glNewList(DisplayList,GL_COMPILE_AND_EXECUTE);
		Render();
		glEndList();
	}
	else
	{
		glCallList(DisplayList);
	}

}


private:
	OmniQuad *quad;
	int quad_tesselation;
	virtual	void OmniMap_Screen_Quad::Render()
	{
		RenderQuadTesselated();
	}	


	OmniVec3 MultiplyVector(OmniVec3 a, double x) // todo move this to omnivec as an operator?....
	{
		return OmniVec3(a.x*x,a.y*x,a.z*x);
	}
	virtual OmniVec3 OmniMap_Screen_Quad::FourPointInterpolation(OmniVec3 p00,OmniVec3 p01,OmniVec3 p11,OmniVec3 p10, double x, double y)
	{
		

		OmniVec3 top    = MultiplyVector(p01, (1.0f-(float)x));top+= MultiplyVector(p11,((float)x));
		OmniVec3 bottom = MultiplyVector(p00, (1.0f-(float)x));bottom+= MultiplyVector(p10,((float)x));
		OmniVec3 returnval = MultiplyVector(top,((float)y));returnval +=MultiplyVector(bottom,(1.0f-(float)y));
		return returnval;
	}

	virtual void OmniMap_Screen_Quad::RenderQuadTesselated()
	{

unsigned int tesselationX =quad_tesselation;
unsigned int tesselationZ =quad_tesselation;
		glBegin(GL_QUADS);
	
		for (unsigned int i = 0; i < tesselationX - 1; i++)
		{
			for (unsigned int j = 0; j < tesselationZ - 1; j++)
			{
				double x1=  i	/(double)(tesselationX - 1.0);
				double z1 = j	/(double)(tesselationZ - 1.0); 
				double x2= (i+1)/(double)(tesselationX - 1.0); 
				double z2 = j	/(double)(tesselationZ - 1.0); 
				double x3= (i+1)/(double)(tesselationX - 1.0);
				double z3 =(j+1)/(double)(tesselationZ - 1.0);
				double x4=  i	/(double)(tesselationX - 1.0);
				double z4 =(j+1)/(double)(tesselationZ - 1.0); 



				OmniVec3 vpoint1 = FourPointInterpolation(quad->bl,quad->tl,quad->tr,quad->br,x1 ,z1);
				OmniVec3 vpoint2 = FourPointInterpolation(quad->bl,quad->tl,quad->tr,quad->br,x4 ,z4);
				OmniVec3 vpoint3 = FourPointInterpolation(quad->bl,quad->tl,quad->tr,quad->br,x3 ,z3); 
				OmniVec3 vpoint4 = FourPointInterpolation(quad->bl,quad->tl,quad->tr,quad->br,x2 ,z2);

				
				
			glVertex3d(vpoint1.x,vpoint1.y,vpoint1.z);
			glVertex3d(vpoint2.x,vpoint2.y,vpoint2.z);
			glVertex3d(vpoint3.x,vpoint3.y,vpoint3.z);
			glVertex3d(vpoint4.x,vpoint4.y,vpoint4.z);
		


			}
		}
		glEnd();
	}


	void OmniMap_Screen_Quad::RenderQuadUV()
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0 ,0);
		glVertex3d(quad->bl.x,quad->bl.y,quad->bl.z);
		glTexCoord2f(0 ,1);
		glVertex3d(quad->tl.x,quad->tl.y,quad->tl.z);
		glTexCoord2f(1 ,1);
		glVertex3d(quad->tr.x,quad->tr.y,quad->tr.z);
		glTexCoord2f(1 ,0);
		glVertex3d(quad->br.x,quad->br.y,quad->br.z);
		glEnd();
	}

	virtual	void OmniMap_Screen_Quad::SetUpPropertyAccess()
	{
		//printf("derived SetUpPropertyAccess()\n");
		access.Register("quad_bl.x",&quad->bl.x);
		access.Register("quad_tl.x",&quad->tl.x);
		access.Register("quad_tr.x",&quad->tr.x);
		access.Register("quad_br.x",&quad->br.x);
		access.Register("quad_tesselation",&quad_tesselation);
	}

public:
	OmniMap_Screen_Quad::OmniMap_Screen_Quad()
	{
		BaseInit();
		quad_tesselation = OMNIMAP_DEFAULT_QUADLIST_TESSELATION;
		quad= new OmniQuad;
		SetUpPropertyAccess();
		LogSystem()->ReportMessage("new OmniMap_Screen:OmniMap_Screen_Quad");
	}

	OmniMap_Screen_Quad::~OmniMap_Screen_Quad()
	{
		delete quad;
		LogSystem()->ReportMessage("delete OmniMap_Screen:OmniMap_Screen_Quad");
	}

	////////// export ///////

};
*/


	void OmnimapScreenShapeFactory::addScreenShapeCreator(const char * type,fpScreenCreatorFunction functionPointer)
	{

		if( (*(String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP).find(type) !=
			(*(String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP).end())
		{
			LogSystem()->ReportError("constructor for type %s allready defined... overwriting it.",type);
		}
		(*(String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP) [type] = functionPointer;
		
	}


	OmniMap_Screen * OmnimapScreenShapeFactory::Create_ScreenShape(const char * type)
	{

		if((*(String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP) [type])
		{
			
				return (*(String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP) [type]();
		}
		else
		{
				LogSystem()->ReportMessage("Error Creating screenshape of type %s! returning a dome.",type);
				LogSystem()->ReportError("Error creating screenshape of type %s! returning a dome.",type);
				return(NULL);
		}

		return(NULL);

	}



	OmnimapScreenShapeFactory::OmnimapScreenShapeFactory()
	{
		LocalString_fpScreenCreatorFunction_MAP = (String_fpScreenCreatorFunction_MAP *) new String_fpScreenCreatorFunction_MAP();
		((String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP)->clear();	
	
	}
	OmnimapScreenShapeFactory::~OmnimapScreenShapeFactory()
	{
		((String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP)->clear();	
		delete 		((String_fpScreenCreatorFunction_MAP *)LocalString_fpScreenCreatorFunction_MAP);	
		LocalString_fpScreenCreatorFunction_MAP = 0;
	}

#ifdef ELUMENATI_INTERNAL
	int OmnimapScreenShapeFactory::getNumShapes() {
		String_fpScreenCreatorFunction_MAP *m = (String_fpScreenCreatorFunction_MAP *) LocalString_fpScreenCreatorFunction_MAP;
		return(m->size());
	}

	const char *OmnimapScreenShapeFactory::getScreenTypeName(int index) {

		String_fpScreenCreatorFunction_MAP *m = (String_fpScreenCreatorFunction_MAP *) LocalString_fpScreenCreatorFunction_MAP;
		int i = 0;
		for(String_fpScreenCreatorFunction_MAP::iterator itr = m->begin();	itr != m->end();itr++,i++) {
			if(i == index)
				return itr->first.c_str();
		}
	}
#endif




	 ///////////////////////////////////////////////////////////
	 ////////OmniMap_Screen_Quad
	 ////////////////////////////////////////////////////////////
#ifdef GL_DEPENDENT
	 void OmniMap_Screen_Quad::Render()
	 {
		 glColor3f(1,1,1);
		 glBegin(GL_QUADS);
		 glTexCoord2f(blU,blV+heightV);
		 glVertex3f(quad->bl.x,quad->bl.y,quad->bl.z);
		 glTexCoord2f(blU,blV);
		 glVertex3f(quad->tl.x,quad->tl.y,quad->tl.z);
		 glTexCoord2f(blU+widthU,blV);
		 glVertex3f(quad->tr.x,quad->tr.y,quad->tr.z);
		 glTexCoord2f(blU+widthU,blV+heightV);
		 glVertex3f(quad->br.x,quad->br.y,quad->br.z);
		 glEnd();
	 }	
	 void OmniMap_Screen_Quad::SetUpPropertyAccess()
	 {
		 //printf("derived SetUpPropertyAccess()\n");
		 access.Register("quad_bl.x",&quad->bl.x);
		 access.Register("quad_tl.x",&quad->tl.x);
		 access.Register("quad_tr.x",&quad->tr.x);
		 access.Register("quad_br.x",&quad->br.x);

		 access.Register("quad_bl.y",&quad->bl.y);
		 access.Register("quad_tl.y",&quad->tl.y);
		 access.Register("quad_tr.y",&quad->tr.y);
		 access.Register("quad_br.y",&quad->br.y);

		 access.Register("quad_bl.z",&quad->bl.z);
		 access.Register("quad_tl.z",&quad->tl.z);
		 access.Register("quad_tr.z",&quad->tr.z);
		 access.Register("quad_br.z",&quad->br.z);



		 access.Register("quad_tesselation",&quad_tesselation);

		 access.Register("RenderTexCoordinates",&RenderTexCoordinates);
		 access.Register("blU",&blU);
		 access.Register("blV",&blV);
		 access.Register("widthU",&widthU);
		 access.Register("heightV",&heightV);


	 }

	 OmniMap_Screen_Quad::OmniMap_Screen_Quad()
	 {
		 quad_tesselation = OMNIMAP_DEFAULT_QUADLIST_TESSELATION;
		 quad= new OmniQuad;
		 SetUpPropertyAccess();
		 LogSystem()->ReportMessage("new OmniMap_Screen:OmniMap_Screen_Quad");
	 }
	 OmniMap_Screen_Quad::~OmniMap_Screen_Quad()
	 {
		 delete quad;
		 LogSystem()->ReportMessage("delete OmniMap_Screen:OmniMap_Screen_Quad");
	 }


#endif


