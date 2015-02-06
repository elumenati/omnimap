#pragma once

#include "OmniMapBase.h"
#include <vector>

class Point3;
class Matrix;
class OmniVec3;
class Vec4;
class Ray;
class Quat;



class Point3 
{

private:
	

public:
	float x, y, z;
	Point3();
	Point3(float points[3]);
	Point3(float x, float y, float z);

	Point3& operator=(Point3 inpoint);
	Point3 operator+(Point3 inpoint);
	Point3 operator-(Point3 inpoint);
	Point3 operator*(Point3 inpoint);
	Point3 operator*(float val);
	Point3 operator/(Point3 inpoint);
	Point3 operator/(float val);

	//linearly interpolates between this point and incoming point, with amount between 0 and 1 (distance away from this point)
	Point3 lerp(float amount, Point3 pt2);

	//sets this point's x,y,z equal to incoming point's x,y,z
	void set(Point3 point);
	void set(float x, float y, float z);

	//grabs the x, y, z coordinates (0, 1, 2)
	float getCoord(int num);

	//returns float distance between two pts
	float distanceBetween(Point3 pt2);

	//returns vector3 of this point (same thing essentially)
	OmniVec3 toVec3();
	
	//returns true if this point is equal to the given point
	bool equals(Point3 point);

	void make4tuple(float v[]);
	void make4tuple(double v[]);

	void print();
};


class Point4 {
public:
	Point4(){
	};
	~Point4(){
	};
	float x, y, z,w;
};


Point3 operator*(Point3 one, float val);
Point3 operator*(float val, Point3 two);

//Affine matrix class
#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API AffMatrix {
#else
class AffMatrix {
#endif
	
private:
	

public:
	void inverse(AffMatrix *dest);
	void transformPoint(double v[]);
	void transformPoint(float v[]);
	void transformVector(double v[]);
	void transformVector(float v[]);
	AffMatrix();//constructs identity;

	AffMatrix(float* values);
	AffMatrix(double* values);

	AffMatrix(const AffMatrix& m);

	~AffMatrix()
	{ delete [] m; }

	//Sets this matrix equal to the provided
	AffMatrix& operator=(const AffMatrix& m);

	//Set this matrix to the values in the provided matrix
	void set(AffMatrix matrix);

	//Set the matrix with the given values (does an internal copy)
	void set(float* vals);

	//performs a post multiply with given matrix -> this * matrix
	AffMatrix postMultiply(AffMatrix matrix);
	
	//performs a pre multiply with given matrix -> matrix * this
	AffMatrix preMultiply(AffMatrix matrix);

	//Return the transpose of this matrix (doesn't alter stored values)
	AffMatrix transpose();

	//Set the matrix to its transpose
	void setTranspose();

	//Sets up the AffineMatrix to store a transformation given the three vectors
	//Identical to the gluLookAt(..) command (column major storage, so you can pass it directly to OpenGL)
	void setLookAtGL(OmniVec3 eye, OmniVec3 eye_focus, OmniVec3 up);

	//Sets up the AffineMatrix to store a transformation given the frustum values
	//Identical to the glFrustum(..) command (column major storage, so you can pass it directly to OpenGL)
	void setFrustumGL(float left, float right, float bottom, float top, float znear, float zfar);

	//Set a translate matrix with the given axis direction values (retains it's row major storage)
	void setTranslate(float x, float y, float z);

	void setScale(float x, float y, float z);

	//Set a rotation matrix with the given rotations about the x, y, and z axis (beware of gimbal lock!!)
	//Cumulative rotation is defined as Rx*Ry*Rz  (retains it's row major storage)
	void setRotation(float x_rad, float y_rad, float z_rad);

	//Clear the matrix values back to the identity matrix
	void setIdentity();

	//Applies the matrix transform to a vector v, overwriting its values
	//v[] in x,y,z,w
	void applyTransform(float v[]); 

	//returns a pointer to the data
	float* returnValues()
	{ return m; }

	float* returnTransposeValues();

	//Prints to stdout the values inside the matrix
	void printValues();

	AffMatrix returnInverse();

	float* m; //row Major order

};

//Matrix multiplication method
AffMatrix operator*(AffMatrix first, AffMatrix second);


//general Matrix Class - any arbitrary size
class Matrix {

private:
	
	
public:

	//Constructors
	Matrix();	//Creates a 10x10 matrix full of zeros
	Matrix(int rows, int cols); //Creates a rows x cols matrix full of zeros
	Matrix(int rows, int cols, double* values); //Creates a rows x cols matrix full of the given value data

	//Matrix(int rows, int cols, double values[]);
	Matrix(const Matrix& m);

	~Matrix();

	Matrix& operator=(const Matrix& m);

	//returns the transpose of the matrix
	Matrix transpose();

	void print(char* name = "Matrix");     

	//data members - public access
	int rows;
	int cols;
	int size;
	//matrix values - row major
	double *values;
};

//Matrix multiply operator
Matrix operator*(Matrix& first, Matrix& second);



#ifdef ELUMENATI_INTERNAL
OMNIMAP_API OmniVec3 operator+(OmniVec3,OmniVec3);
#else
OmniVec3 operator+(OmniVec3,OmniVec3);
#endif
OmniVec3 operator-(OmniVec3,OmniVec3);
OmniVec3 operator*(OmniVec3, float);
OmniVec3 operator*(float, OmniVec3);
OmniVec3 operator/(OmniVec3, float);
OmniVec3 operator/(float, OmniVec3);


class Vec4 : public OmniVec3
{
public:
	float w;

	Vec4();
	Vec4(float x, float y, float z, float w);
	Vec4(float incoord[4]);
	Vec4(OmniVec3 vec, float w);
	
	inline float dot(Vec4 invec)
	{ return x*invec.x + y*invec.y + z*invec.z + w*invec.w; }

	void make4tuple(float v[]);
	void make4tuple(double v[]);
	void set(Vec4 vec);
	void set(float x, float y, float z, float w);
	void set(OmniVec3 vec, float w);

	inline OmniVec3 toVec3()
	{ return OmniVec3(x, y, z); }
};





class Ray {

public:

	Ray(Point3 start, OmniVec3 direction);

	//Returns a Point3 of their intersection
	//Since rays will almost never intersect, this method finds the minimum distance between the two rays
	//Which is a line between them.  It will then return the point3 which is the lines midpoint.
	bool intersect(Ray in_ray, Point3& point);

	//Given the time t, it returns the point3 position the ray is at
	Point3 pointAt(float t);

	void print();

	Point3 start;
	OmniVec3 direction;

	//Returns true if it intersects this triangle
	//Triangle defined as a list of 3 Point3's
	bool intersectsTriangle(Point3* triangle, Point3& P);

private:


};

class CR_Spline
{

private:
	Point3 p0;
	Point3 p1;
	Point3 p2;
	Point3 p3;

public:

	CR_Spline(){;}

	CR_Spline(Point3 p0, Point3 p1, Point3 p2, Point3 p3);

	Point3 getPointAt(float time);

	bool containsX(float x, float& t);

	CR_Spline& operator=(CR_Spline spline);
};

class PieceWise_CR_Spline
{

private:

	Point3* pts;
	int num_pts;
	int segments;

public:

	PieceWise_CR_Spline(){ pts = 0; num_pts = 0; }
	PieceWise_CR_Spline(Point3* pts, int num_pts);

	~PieceWise_CR_Spline();

	Point3 getPointAt(float time);

	PieceWise_CR_Spline(const PieceWise_CR_Spline& spline);
	PieceWise_CR_Spline& operator=(PieceWise_CR_Spline spline);

	Point3* getPointPtr(int index)
	{ return &pts[index]; }

	
};

class Quat {

public:

	//quaternion values x,y,z part of vector, w is the scalar
	float x, y, z, w;

	//Creates a quaternion given the four values
	Quat(float _x = 0, float _y = 0, float _z = 0, float _w = 0);

	//Creates a quaternion for a single vector
	Quat(OmniVec3 vector);
	
	//Creates a quaternion for a rotation theta about an axis u (theta in radians)
	Quat(float theta, OmniVec3 u);

	//returns the length of the quaternion
	float length();

	void set(float x, float y, float z, float w);

	//multiplies this quaterion(a) by quaternion b  in the form b * a * b.conjugate
	//b is quaternion that represents the rotation
	Quat multiply(Quat b);

	Quat multiplyQuat(Quat b);
	
	//normalizes this quaternion
	void normalize();

	//returns the conjugate of this quaternion
	Quat conjugate();

	//returns OmniVec3 of the quaternion
	OmniVec3 toVec3();

	Quat& operator=(Quat inquat);
};

Quat operator+(Quat,Quat);





class Plane
{
public:
	//		ax + by + cz + d = 0
	// <=>	normal.dot(x,y,z) + d = 0
	// equation = Vec4(a,b,c,d)
	Plane()
	{
		equation.set(0,0,1,0);
	}

	Plane(OmniVec3 n, float d)
	{
		equation.set(n.returnNormalized(), d);
	}

	Plane(OmniVec3 n, OmniVec3 point)
	{
		equation.set(n, -n.dot(point));
	}

	Plane(OmniVec3 a, OmniVec3 b, OmniVec3 c)
	{
		OmniVec3 v1 = (b - a).returnNormalized();
		OmniVec3 v2 = (c - a).returnNormalized();

		equation.set(v1.cross(v2), 1);
		equation.w = -equation.toVec3().dot(a);
	}

	void set(OmniVec3 n, float d)
	{
		equation.set(n.returnNormalized(), d);
	}

	void set(OmniVec3 n, OmniVec3 point)
	{
		equation.set(n, -n.dot(point));
	}

	void set(OmniVec3 a, OmniVec3 b, OmniVec3 c)
	{
		OmniVec3 v1 = (b - a).returnNormalized();
		OmniVec3 v2 = (c - a).returnNormalized();

		equation.set(v1.cross(v2), 1);
		equation.w = -equation.toVec3().dot(a);
	}

	void set(Plane p)
	{
		equation.set(p.equation);
	}
		
	void flip()
	{
		equation.set(-1.0 * equation.toVec3(), -equation.w);
	}

	bool isPointInFront(OmniVec3 p);
	bool isPointInFront(Vec4 p);

	// returns true when all points are behind plane
	bool pointsCulled(std::vector<Vec4> points);

	// returns true when all points are behind at least one plane in the set
	static bool pointsCulled(std::vector<Plane> planes, std::vector<Vec4> points);

	float distanceToPoint(Vec4 p);

	inline OmniVec3 getNormal()
	{ return equation.toVec3(); }

	inline float getDistance()
	{ return equation.w; }

	Vec4 equation;
};



////////////////////////////
//void QuadToEquations(Quad myquad, Point4 &UPlaneEquation ,Point4 &VPlaneEquation ,Point4 &PlaneEquation );
OmniVec3 Point3ToVec(Point3 p);
/*
void QuadToEquations(OmniQuad q, float UPlaneEquation[4],float VPlaneEquation[4],float PlaneEquation[4]);
void	QuadToPlaneEquation(OmniQuad q,float PlaneEquation[4]);
*/