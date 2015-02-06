#include "simpledome.h"

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

OmniVec3 MakePerpendicluarVector(OmniVec3 vecin)
{
	vecin.normalize();
	OmniVec3 TempOffset = vecin;
	if (fabs(TempOffset.x) < fabs(TempOffset.y))
	{
		if (fabs(TempOffset.x) < fabs(TempOffset.z))
		{
			TempOffset.x =1;
		}
		else
		{
			TempOffset.z =1;
		}
	}
	else
	{
		if (fabs(TempOffset.y) < fabs(TempOffset.z))
		{
			TempOffset.y = 1;
		}
		else
		{
			TempOffset.z = 1;
		}
	}

	TempOffset.normalize();

	OmniVec3 perp =  TempOffset.cross(vecin);

	perp.normalize();
	return perp;
}

void displayHyperbola::display()
{

	// display a dome using opengl...
	// standard way of doing it....
	// have a basic unit dome, and have a matrix move and rotate it....

	// generate the 4x4 matrix for moving it...

	OmniVec3 DefaultI(1,0,0);
	OmniVec3 DefaultJ(0,1,0);
	OmniVec3 DefaultK(0,0,1);

	OmniVec3 NewJ = ApexDirection;
	OmniVec3 NewI = MakePerpendicluarVector(NewJ);
	OmniVec3 NewK = NewI.cross(NewJ);

	






	double domeradius= Radius;
	int Type = GL_QUADS;

	//Type = GL_POINTS;
	

	glBegin(Type);
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
				(float)0.0	,
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

			glVertex3f(bl.x,bl.y,bl.z);
			glVertex3f(tl.x,tl.y,tl.z);
			glVertex3f(tr.x,tr.y,tr.z);
			glVertex3f(br.x,br.y,br.z);
			continue;

docorner:
			if(	((tl.x*tl.x+tl.z*tl.z) <= (radius))&&
				((tr.x*tr.x+tr.z*tr.z) <= (radius))&&
				((br.x*br.x+br.z*br.z) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

			bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
			tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
			tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
			br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;
		
				glVertex3f(tl.x,tl.y,tl.z);
				glVertex3f(tr.x,tr.y,tr.z);
				glVertex3f(br.x,br.y,br.z);
				glEnd();
				glBegin(Type);
			}

			
			if(
				((bl.x*bl.x+bl.z*bl.z) <= (radius)) &&
				((tr.x*tr.x+tr.z*tr.z) <= (radius))&&
				((br.x*br.x+br.z*br.z) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

			bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
			tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
			tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
			br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;


				glVertex3f(bl.x,bl.y,bl.z);		
				glVertex3f(tr.x,tr.y,tr.z);
				glVertex3f(br.x,br.y,br.z);
				glEnd();
				glBegin(Type);
			}

			
			if(
				((bl.x*bl.x+bl.z*bl.z) <= (radius)) &&
				((tl.x*tl.x+tl.z*tl.z) <= (radius))&&
				((br.x*br.x+br.z*br.z) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

			bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
			tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
			tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
			br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;

				glVertex3f(bl.x,bl.y,bl.z);
				glVertex3f(tl.x,tl.y,tl.z);
		
				glVertex3f(br.x,br.y,br.z);
				glEnd();
				glBegin(Type);
			}

			
			if(
				((bl.x*bl.x+bl.z*bl.z) <= (radius)) &&
				((tl.x*tl.x+tl.z*tl.z) <= (radius))&&
				((tr.x*tr.x+tr.z*tr.z) <= (radius)))
			{
				glEnd();
				glBegin(GL_TRIANGLES);

			bl = OmniVec3(bl.dot(NewI),bl.dot(NewJ),bl.dot(NewK))+Center;
			tl = OmniVec3(tl.dot(NewI),tl.dot(NewJ),tl.dot(NewK))+Center;
			tr = OmniVec3(tr.dot(NewI),tr.dot(NewJ),tr.dot(NewK))+Center;
			br = OmniVec3(br.dot(NewI),br.dot(NewJ),br.dot(NewK))+Center;
				glVertex3f(bl.x,bl.y,bl.z);
				glVertex3f(tl.x,tl.y,tl.z);
				glVertex3f(tr.x,tr.y,tr.z);
		
				glEnd();
				glBegin(Type);
			}



		}
	}
	glEnd();

}