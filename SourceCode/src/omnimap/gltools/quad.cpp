#include "../privatePrimitives.h"
#include "../GraphicsPrimitives.h"

#include <GL/glew.h>
#include <windows.h>
#include <GL/gl.h>



	OmniQuad::OmniQuad()
	{
	};
	OmniQuad::OmniQuad(
		float blx,float bly,float blz,
		float tlx,float tly,float tlz,
		float trx,float tr_y,float trz,
		float brx,float bry,float brz
		)
	{
		tl.x =tlx;
		tl.y =tly;
		tl.z =tlz;

		bl.x =blx;
		bl.y =bly;
		bl.z =blz;

		tr.x =trx;
		tr.y =tr_y;
		tr.z =trz;

		br.x =brx;
		br.y =bry;
		br.z =brz;

	};
	void OmniQuad::set(
		float blx,float bly,float blz,
		float tlx,float tly,float tlz,
		float trx,float tr_y,float trz,
		float brx,float bry,float brz
		)
	{
		tl.x =tlx;
		tl.y =tly;
		tl.z =tlz;

		bl.x =blx;
		bl.y =bly;
		bl.z =blz;

		tr.x =trx;
		tr.y =tr_y;
		tr.z =trz;

		br.x =brx;
		br.y =bry;
		br.z =brz;


	};
	OmniQuad::~OmniQuad(){};

/*
Point4	QuadToPlaneEquation()
{

}*/
	void QuadToMatrix()
	{

	}

	OmniVec3 OmniQuad::FourPointInterpolation(OmniVec3 p00,OmniVec3 p01,OmniVec3 p11,OmniVec3 p10, double x, double y)
	{
		OmniVec3 top    = p01* (1.0f-(float)x)+ p11*((float)x);
		OmniVec3 bottom = p00* (1.0f-(float)x)+ p10*((float)x);
		return top*((float)y)+bottom*(1.0f-(float)y);
	}
/*
	void OmniQuad::RenderQuadTesselated(unsigned int tesselationX,unsigned int tesselationZ)
	{
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



				OmniVec3 vpoint1 = FourPointInterpolation(bl,tl,tr,br,x1 ,z1);
				OmniVec3 vpoint2 = FourPointInterpolation(bl,tl,tr,br,x4 ,z4);
				OmniVec3 vpoint3 = FourPointInterpolation(bl,tl,tr,br,x3 ,z3); 
				OmniVec3 vpoint4 = FourPointInterpolation(bl,tl,tr,br,x2 ,z2);

				
				
			glVertex3d(vpoint1.x,vpoint1.y,vpoint1.z);
			glVertex3d(vpoint2.x,vpoint2.y,vpoint2.z);
			glVertex3d(vpoint3.x,vpoint3.y,vpoint3.z);
			glVertex3d(vpoint4.x,vpoint4.y,vpoint4.z);
		


			}
		}
		glEnd();
	}


	void OmniQuad::RenderQuadUV()
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0 ,0);
		glVertex3d(bl.x,bl.y,bl.z);
		glTexCoord2f(0 ,1);
		glVertex3d(tl.x,tl.y,tl.z);
		glTexCoord2f(1 ,1);
		glVertex3d(tr.x,tr.y,tr.z);
		glTexCoord2f(1 ,0);
		glVertex3d(br.x,br.y,br.z);
		glEnd();
	}
*/

