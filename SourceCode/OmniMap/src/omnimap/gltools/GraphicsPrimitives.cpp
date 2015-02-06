#pragma warning(disable: 4244) // disable warning 4244: double to float
#pragma warning(disable: 4305) // disable warning 4305: double to float
#ifdef WIN32
#include <windows.h>
#endif
//#include <GL/glew.h>
//#include <GL/gl.h>
#include <math.h>
#include "../OmniMapBase.h"
#include "../privatePrimitives.h"


OmniVec3::OmniVec3()
{
	x = y = z = 0.0;
}

OmniVec3::OmniVec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
/*
OmniVec3::OmniVec3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
*/


OmniVec3::OmniVec3(float incoord[3])
{
	this->x = incoord[0];
	this->y = incoord[1];
	this->z = incoord[2];
}

OmniVec3::OmniVec3(OmniVec3 pt1, OmniVec3 pt2)
{
	this->x = pt1.x - pt2.x;
	this->y = pt1.y - pt2.y;
	this->z = pt1.z - pt2.z;
}

OmniVec3 
OmniVec3::lerp(float amount, OmniVec3 endvec)
{
	float diffx = endvec.x - this->x;
	float diffy = endvec.y - this->y;
	float diffz = endvec.z - this->z;

	return OmniVec3(this->x + amount*diffx, this->y + amount*diffy, this->z + amount*diffz);

}

OmniVec3
OmniVec3::average(float amount, OmniVec3 endvec)
{
	float start_amount = (1 - amount) * 1000;
	float end_amount = amount * 1000;

	OmniVec3 temp(this->x, this->y, this->z);

	OmniVec3 av_vec = (temp * start_amount) + (endvec * end_amount);
	av_vec = av_vec / 1000;

	av_vec.normalize();
	return av_vec;

}

void
OmniVec3::set(OmniVec3 vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}

void OmniVec3::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/*inline float
OmniVec3::dot(OmniVec3 invec)
{
	return x*invec.x + y*invec.y + z*invec.z;
}*/

void
OmniVec3::add(OmniVec3 vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
}

float
OmniVec3::length()
{
	return (float)sqrt(x*x + y*y + z*z);
}

/*inline OmniVec3
OmniVec3::cross(OmniVec3 invec)
{
	return OmniVec3(y*invec.z - z*invec.y, z*invec.x - x*invec.z, x*invec.y - y*invec.x);
}*/

OmniVec3
OmniVec3::returnNormalized()
{
	float length = this->length();
	if (length == 0.0)
		return OmniVec3(0,0,0);
	else
		return OmniVec3(this->x / length, this->y / length, this->z / length);
}

void
OmniVec3::normalize()
{
	OmniVec3 newvec = returnNormalized();
	this->x = newvec.x;
	this->y = newvec.y;
	this->z = newvec.z;
}


OmniVec3 
OmniVec3::rotateByVector(OmniVec3 vector, float angle)
{
	Quat vec_quat(*this);
	Quat rot_quat(angle, vector);
	rot_quat.normalize();

	Quat end_quat = vec_quat.multiply(rot_quat);
	return end_quat.toVec3();
}

OmniVec3&
OmniVec3::operator+=(OmniVec3 invec){
	this->x += invec.x;
	this->y += invec.y;
	this->z += invec.z;
	return *this;
}

OmniVec3&
OmniVec3::operator-=(OmniVec3 invec){
	this->x -= invec.x;
	this->y -= invec.y;
	this->z -= invec.z;
	return *this;
}

OmniVec3& 
OmniVec3::operator=(OmniVec3 invec){
	
	this->x=invec.x;
	this->y=invec.y;
	this->z=invec.z;
	return *this;
}

void
OmniVec3::make4tuple(float v[])
{
	v[0] = this->x;
	v[1] = this->y;
	v[2] = this->z;
	v[3] = 0; // homogenous value = 0 so translates are ignored
}

float
OmniVec3::angleBetween(OmniVec3 invec)
{	

#define CLAMP_VALUE(x,mi,ma)( ((x)<(mi))?(mi):(((x)>(ma))?(ma):(x)))

	float d= this->dot(invec) / (this->length() * invec.length() );
	d =CLAMP_VALUE(d,-1,1);
	float te =acos( d);

	return te;
//return (float)acos( this->dot(invec) / (this->length() * invec.length() ) ); 


}


OmniVec3 OmniVec3::MakePerpendicluarVector(OmniVec3 vecin)
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