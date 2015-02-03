#include "simpledome.h"
#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#pragma warning(disable: 4244) // disable warning 4244: double to float
#include "../privatePrimitives.h"

// Dome Needs to be recompiled after every tweak

displayHyperbola::~displayHyperbola()
{
	
}
displayHyperbola::displayHyperbola()
{
	 tesselation =100;

	 Center=OmniVec3(0,0,0);
	 ApexDirection =OmniVec3(0,1,0);
	 Radius  = 1;
	 
}



#define Saturate(x) ((x)>=0?((x)>1?1:(x)):(0))
void displayHyperbola::display()
{
	 OmniVec3 minbox(999,999,999);
	 OmniVec3 maxbox(-999,-999,-999);


	double domeradius= Radius;


	int Type = GL_QUADS;


	OmniVec3 DefaultDirection(0,1,0);
	OmniVec3 NewDirection = ApexDirection;
	float AngleBetween = NewDirection.angleBetween(DefaultDirection);
	OmniVec3 Axis =DefaultDirection.cross(NewDirection);
	OmniVec3 Apex = Center+ApexDirection*domeradius;

	//Type = GL_POINTS;
	
	for ( int i = 0; i < tesselation - 1; i++)
	{
		glBegin(Type);
		for ( int j = 0; j < tesselation - 1; j++)
		{
				double x1= i/(double)(tesselation - 1.0); x1*=2;x1-=1;
				double z1 =j/(double)(tesselation - 1.0); z1*=2;z1-=1;
				double y1 =1- sqrt(Saturate(1.0 - x1*x1 - z1*z1));
				OmniVec3 vpoint1 = OmniVec3(x1*domeradius,y1*domeradius,z1*domeradius);

				//	if(i == tesselationZ/2)
				//	printf("%f = sqrt(%f^2 %f^2)\n",y1,x1,z1);

				double x2= (i+1)/(double)(tesselation - 1.0); x2*=2;x2-=1;
				double z2 =j/(double)(tesselation - 1.0); z2*=2;z2-=1;
				double y2 =1- sqrt(Saturate(1.0 - x2*x2 - z2*z2));
				OmniVec3 vpoint2 = OmniVec3(x2*domeradius,y2*domeradius,z2*domeradius);

				double x3= (i+1)/(double)(tesselation - 1.0); x3*=2;x3-=1;
				double z3 =(j+1)/(double)(tesselation - 1.0); z3*=2;z3-=1;
				double y3 = 1-sqrt(Saturate(1.0 - x3*x3 - z3*z3));
				OmniVec3 vpoint3 = OmniVec3(x3*domeradius,y3*domeradius,z3*domeradius);

				double x4= i/(double)(tesselation - 1.0); x4*=2;x4-=1;
				double z4 =(j+1)/(double)(tesselation - 1.0); z4*=2;z4-=1;
				double y4 = 1-sqrt(Saturate(1.0 - x4*x4 - z4*z4));
				OmniVec3 vpoint4 = OmniVec3(x4*domeradius,y4*domeradius,z4*domeradius);


				vpoint1 = DefaultDirection - vpoint1 ;
				vpoint2 = DefaultDirection - vpoint2 ;
				vpoint3 = DefaultDirection - vpoint3 ;
				vpoint4 = DefaultDirection - vpoint4 ;

				if(AngleBetween>0)
				{
					vpoint1= vpoint1.rotateByVector(Axis,AngleBetween);
					vpoint2= vpoint2.rotateByVector(Axis,AngleBetween);
					vpoint3= vpoint3.rotateByVector(Axis,AngleBetween);
					vpoint4= vpoint4.rotateByVector(Axis,AngleBetween);

				}

				vpoint1 = Center + vpoint1 ;
				vpoint2 = Center + vpoint2 ;
				vpoint3 = Center + vpoint3 ;
				vpoint4 = Center + vpoint4 ;


				//	goto good;
				double radius_forgiveness = .1; 
				double radius = 1.0+radius_forgiveness;
				if((x1*x1+z1*z1) > (radius ))
					goto docorner;
				if((x2*x2+z2*z2) > (radius ))
					goto docorner;
				if((x3*x3+z3*z3) > (radius))
					 goto docorner;
				if((x4*x4+z4*z4) > (radius))
					 goto docorner;

//good:
				glVertex3d(vpoint1.x,vpoint1.y,vpoint1.z);
				glVertex3d(vpoint2.x,vpoint2.y,vpoint2.z);
				glVertex3d(vpoint3.x,vpoint3.y,vpoint3.z);
				glVertex3d(vpoint4.x,vpoint4.y,vpoint4.z);
				/*
				if(true)
				{
					minbox.x = min(vpoint1.x,minbox.x);
					minbox.y = min(vpoint1.y,minbox.y);
					minbox.z = min(vpoint1.z,minbox.z);

					maxbox.x = max(vpoint1.x,maxbox.x);
					maxbox.y = max(vpoint1.y,maxbox.y);
					maxbox.z = max(vpoint1.z,maxbox.z);
				}
				*/
				continue;

docorner:
			if(	((x2*x2+z2*z2) <= (radius))&&
				((x3*x3+z3*z3) <= (radius))&&
				((x4*x4+z4*z4) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

		
				glVertex3d(vpoint2.x,vpoint2.y,vpoint2.z);
				glVertex3d(vpoint3.x,vpoint3.y,vpoint3.z);
				glVertex3d(vpoint4.x,vpoint4.y,vpoint4.z);
				glEnd();
				glBegin(Type);
			}

			
			if(
				((x1*x1+z1*z1) <= (radius)) &&
				((x3*x3+z3*z3) <= (radius))&&
				((x4*x4+z4*z4) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

				glVertex3d(vpoint1.x,vpoint1.y,vpoint1.z);
		
				glVertex3d(vpoint3.x,vpoint3.y,vpoint3.z);
				glVertex3d(vpoint4.x,vpoint4.y,vpoint4.z);
				glEnd();
				glBegin(Type);
			}

			
			if(
				((x1*x1+z1*z1) <= (radius)) &&
				((x2*x2+z2*z2) <= (radius))&&
				((x4*x4+z4*z4) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

				glVertex3d(vpoint1.x,vpoint1.y,vpoint1.z);
				glVertex3d(vpoint2.x,vpoint2.y,vpoint2.z);
		
				glVertex3d(vpoint4.x,vpoint4.y,vpoint4.z);
				glEnd();
				glBegin(Type);
			}

			
			if(
				((x1*x1+z1*z1) <= (radius)) &&
				((x2*x2+z2*z2) <= (radius))&&
				((x3*x3+z3*z3) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

				glVertex3d(vpoint1.x,vpoint1.y,vpoint1.z);
				glVertex3d(vpoint2.x,vpoint2.y,vpoint2.z);
				glVertex3d(vpoint3.x,vpoint3.y,vpoint3.z);
		
				glEnd();
				glBegin(Type);
			}
		}
		glEnd();
	}

//	printf("\n");
//	printf("Dome aaBounding box Max : %f %f %f\n",maxbox.x,maxbox.y,maxbox.z);
	//printf("Dome aaBounding box Min : %f %f %f\n",minbox.x,minbox.y,minbox.z);


}