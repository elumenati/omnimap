#include <math.h>
#include <stdio.h>

#include "privatePrimitives.h"
#include "OmniMapBase.h"
#include "OM_ErrorHandling.h"

OmniVec3 Point3ToVec(Point3 p)
{
	return OmniVec3(p.x,p.y,p.z);
}


Point3::Point3() 
{
	x = y = z = 0.0;
}

Point3::Point3(float points[3])
{
	x = points[0];
	y = points[1];
	z = points[2];
}
	
Point3::Point3(float in_x, float in_y, float in_z)
{
	x = in_x; y = in_y; z = in_z;
}

Point3& 
Point3::operator=(Point3 inpoint){	
	this->x=inpoint.x;
	this->y=inpoint.y;
	this->z=inpoint.z;
	return *this;
}

Point3 
Point3::operator+(Point3 inpoint){	
	Point3 newpoint;
	newpoint.x = this->x+inpoint.x;
	newpoint.y = this->y+inpoint.y;
	newpoint.z = this->z+inpoint.z;
	return newpoint;
}

Point3 
Point3::operator-(Point3 inpoint){	
	Point3 newpoint;
	newpoint.x = this->x-inpoint.x;
	newpoint.y = this->y-inpoint.y;
	newpoint.z = this->z-inpoint.z;
	return newpoint;
}

Point3 
Point3::operator*(Point3 inpoint){	
	Point3 newpoint;
	newpoint.x = this->x*inpoint.x;
	newpoint.y = this->y*inpoint.y;
	newpoint.z = this->z*inpoint.z;
	return newpoint;
}

Point3 
Point3::operator*(float val){	
	Point3 newpoint;
	newpoint.x = this->x*val;
	newpoint.y = this->y*val;
	newpoint.z = this->z*val;
	return newpoint;
}

Point3 
Point3::operator/(Point3 inpoint){	
	Point3 newpoint;
	newpoint.x = this->x/inpoint.x;
	newpoint.y = this->y/inpoint.y;
	newpoint.z = this->z/inpoint.z;
	return newpoint;
}

Point3 
Point3::operator/(float val){	
	Point3 newpoint;
	newpoint.x = this->x/val;
	newpoint.y = this->y/val;
	newpoint.z = this->z/val;
	return newpoint;
}

Point3 operator*(Point3 one, float val)
{
	Point3 newpoint;
	newpoint.x = one.x*val;
	newpoint.y = one.y*val;
	newpoint.z = one.z*val;
	return newpoint;

}

Point3 operator*(float val, Point3 two)
{
	Point3 newpoint;
	newpoint.x = two.x*val;
	newpoint.y = two.y*val;
	newpoint.z = two.z*val;
	return newpoint;
}

Point3
Point3::lerp(float amount, Point3 pt2)
{
	float diffx = pt2.x - this->x;
	float diffy = pt2.y - this->y;
	float diffz = pt2.z - this->z;

	return Point3(this->x + amount*diffx, this->y + amount*diffy, this->z + amount*diffz);
}

float
Point3::distanceBetween(Point3 pt2)
{
	float x2 = this->x - pt2.x;
	float y2 = this->y - pt2.y;
	float z2 = this->z - pt2.z;

	return (float)sqrt(x2*x2 + y2*y2 + z2*z2);
}

void
Point3::set(Point3 point)
{
	this->x = point.x;
	this->y = point.y;
	this->z = point.z;
}

void Point3::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

bool
Point3::equals(Point3 point)
{
	float eps = (float)0.000001;

	if ( fabs(this->x - point.x) < eps &&
			fabs(this->y - point.y) < eps &&
			fabs(this->z - point.z) < eps )
		return true;
	else
		return false;
}

float
Point3::getCoord(int num)
{
	switch(num)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return x;
}

void
Point3::make4tuple(float v[])
{
	v[0] = this->x;
	v[1] = this->y;
	v[2] = this->z;
	v[3] = 1; // homogenous value = 1 so translates are used
}



void
Point3::make4tuple(double v[])
{
	v[0] = this->x;
	v[1] = this->y;
	v[2] = this->z;
	v[3] = 1; // homogenous value = 1 so translates are used
}


OmniVec3
Point3::toVec3()
{
	return OmniVec3(x,y,z);
}


void
Point3::print()
{
//	LogSystem()->ReportMessage("Point (%f %f %f)", x, y, z);
}

AffMatrix::AffMatrix()
{
	m = new float[16];

	for (int i = 0; i < 16; i++)
		m[i] = 0.0;
	m[0] = m[5] = m[10] = m[15] = 1.0;
}


//AffMatrix::AffMatrix(float values[16])
AffMatrix::AffMatrix(float* values)
{
	m = new float[16];
	for (int i = 0; i < 16; i++)
		m[i] = values[i];
}
AffMatrix::AffMatrix(double* values)
{
	m = new float[16];
	for (int i = 0; i < 16; i++)
		m[i] = (float)values[i];
}


void
AffMatrix::set(AffMatrix matrix)
{
	for (int i = 0; i < 16; i++)
	{
		this->m[i] = matrix.m[i];
	}

}

void
AffMatrix::set(float* vals)
{
	for (int i = 0; i < 16; i++)
	{
		this->m[i] = vals[i];
	}
}

AffMatrix
AffMatrix::postMultiply(AffMatrix matrix)
{
	float sum;
	float temp_m[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			sum = 0;
			for (int c = 0; c < 4; c++)
			{
				sum += matrix.m[j + (c * 4)] * m[(i * 4) + c];	
			}
			temp_m[(i*4) + j] = sum;
		}
	return AffMatrix(temp_m);
}

AffMatrix operator*(AffMatrix first, AffMatrix second)
{
	float sum;
	float temp_m[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			sum = 0;
			for (int c = 0; c < 4; c++)
			{
				sum += second.m[j + (c * 4)] * first.m[(i * 4) + c];	
			}
			temp_m[(i*4) + j] = sum;
		}
	return AffMatrix(temp_m);
}

AffMatrix
AffMatrix::preMultiply(AffMatrix matrix)
{
	float sum;
	float temp_m[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			sum = 0;
			for (int c = 0; c < 4; c++)
			{
				sum += m[j + (c * 4)] * matrix.m[(i * 4) + c];	
			}
			temp_m[(i*4) + j] = sum;
		}
	return AffMatrix(temp_m);
}
AffMatrix& 
AffMatrix::operator=(const AffMatrix& mat)
{
	for (int i = 0; i < 16; i++)
		this->m[i] = mat.m[i];

	return *this;
}

AffMatrix::AffMatrix(const AffMatrix& mat)
{
	m = new float[16];
	for (int i = 0; i < 16; i++)
		this->m[i] = mat.m[i];
}



AffMatrix
AffMatrix::transpose()
{
	float temp_m[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)									
		{
			temp_m[(i * 4) + j] = m [(j * 4) + i];
		}
	return AffMatrix(temp_m);
}

float*
AffMatrix::returnTransposeValues()
{
	float* temp_m = new float[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)									
		{
			temp_m[(i * 4) + j] = m [(j * 4) + i];
		}
	return temp_m;
}

void
AffMatrix::setTranspose()
{
	float temp_m[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)									
		{
			temp_m[(i * 4) + j] = m [(j * 4) + i];
		}
	
	for (int i = 0; i < 16; i++)
		m[i] = temp_m[i];
}

void
AffMatrix::setLookAtGL(OmniVec3 eye, OmniVec3 eye_focus, OmniVec3 up)
{
	OmniVec3 lookat = eye_focus - eye;
	lookat.normalize();
	up.normalize();

	OmniVec3 right = lookat.cross(up);
	right.normalize();

	OmniVec3 new_up = right.cross(lookat);
	new_up.normalize();

	//Now Column Major
	m[0] = right.x;
	m[1] = new_up.x;
	m[2] = -lookat.x;
	m[3] = 0.0f;

	m[4] = right.y;
	m[5] = new_up.y;
	m[6] = -lookat.y;
	m[7] = 0.0f;

	m[8] = right.z;
	m[9] = new_up.z;
	m[10] = -lookat.z;
	m[11] = 0.0f;

	m[12] = right.x   * -eye.x  +  right.y   * -eye.y  +  right.z   * -eye.z;
	m[13] = new_up.x  * -eye.x  +  new_up.y  * -eye.y  +  new_up.z  * -eye.z;
	m[14] = -lookat.x * -eye.x  +  -lookat.y * -eye.y  +  -lookat.z * -eye.z;
	m[15] = 1.0f;
}

void 
AffMatrix::setFrustumGL(float left, float right, float bottom, float top, float znear, float zfar)
{
	float A = (right + left) / (right - left);
	float B = (top + bottom) / (top - bottom);
	float C = (zfar + znear) / (zfar - znear);
	float D = (2 * zfar * znear) / (zfar - znear);

	//Now Column Major
	m[0]  = (2 * znear) / (right - left);
	m[1]  = 0.0f;
	m[2]  = 0.0f;
	m[3]  = 0.0f;

	m[4]  = 0.0f;
	m[5]  = (2 * znear) / (top - bottom);
	m[6]  = 0.0f;
	m[7]  = 0.0f;

	m[8]  = A;
	m[9]  = B;
	m[10] = C;
	m[11] = -1.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = D;
	m[15] = 0.0f;
}

void 
AffMatrix::setIdentity()
{
	m[0]  = 1.0f;
	m[1]  = 0.0f;
	m[2]  = 0.0f;
	m[3]  = 0.0f;

	m[4]  = 0.0f;
	m[5]  = 1.0f;
	m[6]  = 0.0f;
	m[7]  = 0.0f;

	m[8]  = 0.0f;
	m[9]  = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;

}

void
AffMatrix::setRotation(float x_rad, float y_rad, float z_rad)
{
	float xc = (float)cos(x_rad);//1
	float xs = (float)sin(x_rad);//0
	float yc = (float)cos(y_rad);//1
	float ys = (float)sin(y_rad);//0
	float zc = (float)cos(z_rad);//.707
	float zs = (float)sin(z_rad);//.707

	m[0]  = yc*zc;
	m[1]  = yc*zs;
	m[2]  = -ys;
	m[3]  = 0.0f;

	m[4]  = xs*ys*zc + xc*-zs;
	m[5]  = zs*xs*ys + xc*zc;
	m[6]  = xs*yc;
	m[7]  = 0.0f;

	m[8]  = xc*ys*zc + xs*zs;
	m[9]  = xc*ys*zs - xs*zc;
	m[10] = xc*yc;
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void 
AffMatrix::setTranslate(float x, float y, float z)
{
	//Row Major - standard Translate matrix - all values in last column
	m[0]  = 1.0f;
	m[1]  = 0.0f;
	m[2]  = 0.0f;
	m[3]  = x;

	m[4]  = 0.0f;
	m[5]  = 1.0f;
	m[6]  = 0.0f;
	m[7]  = y;

	m[8]  = 0.0f;
	m[9]  = 0.0f;
	m[10] = 1.0f;
	m[11] = z;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void 
AffMatrix::setScale(float x, float y, float z)
{
		//Row Major - standard Scale matrix - all values in last column
	m[0]  = x;
	m[1]  = 0.0f;
	m[2]  = 0.0f;
	m[3]  = 0.0f;

	m[4]  = 0.0f;
	m[5]  = y;
	m[6]  = 0.0f;
	m[7]  = 0.0f;

	m[8]  = 0.0f;
	m[9]  = 0.0f;
	m[10] = z;
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}


void
AffMatrix::applyTransform(float v[])
{
	float p[4];

	for (int i = 0; i < 4; i++)
	{
		p[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			p[i] += m[4 * i + j] * v[j];
		}
	}

	for (int i = 0; i < 4; i++)
		v[i] = p[i];
}

void
AffMatrix::printValues()
{
	/*LogSystem()->ReportMessage("Matrix Values ---------------");
	for (unsigned int i = 0; i < 4; i++)
	{
		
		for (unsigned int j = 0; j < 4; j++)
			LogSystem()->ReportMessage("%.3f ", m[i*4 + j]);
	}
	LogSystem()->ReportMessage("-----------------------------");
	*/
}


//Matrix Inversion
//by Richard Carling
//from "Graphics Gems", Academic Press, 1990
float det2x2(float a, float b, float c, float d)
{
    float ans;
    ans = a * d - b * c;
    return ans;
}

float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3 )
{
    float ans;

    ans = a1 * det2x2( b2, b3, c2, c3 )
        - b1 * det2x2( a2, a3, c2, c3 )
        + c1 * det2x2( a2, a3, b2, b3 );
    return ans;
}

float det4x4( AffMatrix* m )
{
    float ans;
    float a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

    ///* assign to individual variable names to aid selecting 
	///*  correct elements 

	a1 = m->m[0]; b1 = m->m[1]; 
	c1 = m->m[2]; d1 = m->m[3];

	a2 = m->m[4]; b2 = m->m[5]; 
	c2 = m->m[6]; d2 = m->m[7];

	a3 = m->m[8]; b3 = m->m[9]; 
	c3 = m->m[10]; d3 = m->m[11];

	a4 = m->m[12]; b4 = m->m[13]; 
	c4 = m->m[14]; d4 = m->m[15];

    ans = a1 * det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4)
        - b1 * det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4)
        + c1 * det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4)
        - d1 * det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
    return ans;
}

// *   adjoint( original_matrix, inverse_matrix )
// * 
// *     calculate the adjoint of a 4x4 matrix
// *     Let  a   denote the minor determinant of matrix A obtained by
// *           ij
// *
// *     deleting the ith row and jth column from A.
// *
// *                    i+j
// *     Let  b   = (-1)    a
// *          ij            ji
// *
// *    The matrix B = (b  ) is the adjoint of A
// *                     ij
void adjoint( AffMatrix* in, AffMatrix* out ) 
{
	float a1, a2, a3, a4, b1, b2, b3, b4;
    float c1, c2, c3, c4, d1, d2, d3, d4;

    //* assign to individual variable names to aid  */
    //* selecting correct values  */

	a1 = in->m[0]; b1 = in->m[1]; 
	c1 = in->m[2]; d1 = in->m[3];

	a2 = in->m[4]; b2 = in->m[5]; 
	c2 = in->m[6]; d2 = in->m[7];

	a3 = in->m[8]; b3 = in->m[9];
	c3 = in->m[10]; d3 = in->m[11];

	a4 = in->m[12]; b4 = in->m[13]; 
	c4 = in->m[14]; d4 = in->m[15];


    //* row column labeling reversed since we transpose rows & columns */
    out->m[0]  =   det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4);
    out->m[4]  = - det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4);
    out->m[8]  =   det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4);
    out->m[12]  = - det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
        
    out->m[1]  = - det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4);
    out->m[5]  =   det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4);
    out->m[9]  = - det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4);
    out->m[13]  =   det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4);

    out->m[2]  =   det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4);
    out->m[6]  = - det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4);
    out->m[10]  =   det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4);
    out->m[14]  = - det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4);
        
    out->m[3]  = - det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3);
    out->m[7]  =   det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3);
    out->m[11]  = - det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3);
    out->m[15]  =   det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3);
}

AffMatrix
AffMatrix::returnInverse()
{
    int i;
    float det;

	AffMatrix out;

    /* calculate the adjoint matrix */

    adjoint(this, &out);

    /*  calculate the 4x4 determinant
     *  if the determinant is zero, 
     *  then the inverse matrix is not unique.
     */

    det = det4x4(this);

    if ( fabs( det ) < 1.e-8) 
	{

		printf("Non-singular matrix, no inverse!  Adjust points on the plane!");
		system("pause");
//		LogSystem()->ReportError("Non-singular matrix, no inverse!  Adjust points on the plane!");
        exit(1);
    }

    /* scale the adjoint matrix to get the inverse */
    for (i=0; i<16; i++)
	    out.m[i] = out.m[i] / det;

	return out;
}

Matrix::Matrix()
{	
	rows = 10;
	cols = 10;
	size = 100;

	values = new double[size];
	for (int i = 0; i < size; i++)
		values[i] = 0.0;
}

Matrix::Matrix(int _rows, int _cols)
{
	rows = _rows;
	cols = _cols;
	size = _rows * _cols;

	values = new double[size];
	for (int i = 0; i < size; i++)
		values[i] = 0.0;
}

Matrix::Matrix(int _rows, int _cols, double* _values)
{
	rows = _rows;
	cols = _cols;
	size = _rows * _cols;

	this->values = new double[size];
	for (int i = 0; i < size; i++)
		this->values[i] = _values[i];	
}

Matrix::~Matrix()
{
	if (values != 0)
		delete[] values;
}

Matrix::Matrix(const Matrix& m)
{
	this->rows = m.rows;
	this->cols = m.cols;
	this->size = m.size;
	
	this->values = new double[size];
	for (int i = 0; i < size; i++)
		this->values[i] = m.values[i];
}


Matrix& 
Matrix::operator=(const Matrix& m)
{
	this->cols = m.cols;
	this->size = m.size;
	
	delete[] values;

	this->values = new double[size];
	for (int i = 0; i < size; i++)
		this->values[i] = m.values[i];

	return *this;
}


/*Matrix*
Matrix::transpose()
{
	double * newvalues = new double[this->size];

	for (int r = 0; r < this->rows; r++)
	{
		for (int c = 0; c < this->cols; c++)
		{
			newvalues[c * rows + r] = this->values[r * cols + c];
		}
	}
	Matrix* newmatrix = new Matrix(this->cols, this->rows, newvalues);

	return newmatrix;
}*/

Matrix
Matrix::transpose()
{
	double * newvalues = new double[this->size];

	for (int r = 0; r < this->rows; r++)
	{
		for (int c = 0; c < this->cols; c++)
		{
			newvalues[c * rows + r] = this->values[r * cols + c];
		}
	}
	Matrix newmatrix(this->cols, this->rows, newvalues);

	return newmatrix;
}


/*Matrix*
Matrix::postMultiply(Matrix* matrix)
{
	if (this->cols != matrix->rows)
	{
		printf("\nCannot perform matrix multiplication, rows and cols do not match");
		return new Matrix();
	}
	
	double* newvalues = new double[this->rows * matrix->cols];

	double sum;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < matrix->cols; j++)
		{
			sum = 0;
			for (int c = 0; c < this->cols; c++)
			{
				sum +=  values[(i * this->cols) + c] * matrix->values[j + (c * matrix->cols)];	
			}
			newvalues[(i*matrix->cols) + j] = sum;
		}

	Matrix* newmatrix = new Matrix(this->rows, matrix->cols, newvalues);
	
	return newmatrix;
}*/

Matrix operator*(Matrix& first, Matrix& second)
{
	if (first.cols != second.rows)
	{
		
		printf("Cannot perform matrix multiplication, rows and cols do not match - returning Identity");
		system("pause");
//		LogSystem()->ReportError("Cannot perform matrix multiplication, rows and cols do not match - returning Identity");
		return Matrix();
	}
	
	double* newvalues = new double[first.rows * second.cols];

	double sum;
	for (int i = 0; i < first.rows; i++)
		for (int j = 0; j < second.cols; j++)
		{
			sum = 0;
			for (int c = 0; c < first.cols; c++)
			{
				sum +=  first.values[(i * first.cols) + c] * second.values[j + (c * second.cols)];	
			}
			newvalues[(i*second.cols) + j] = sum;
		}

	Matrix newmatrix(first.rows, second.cols, newvalues);
	
	return newmatrix;
}

/*Matrix*
Matrix::preMultiply(Matrix* matrix)
{
	if (matrix->cols != this->rows)
	{
		printf("\nCannot perform matrix multiplication, rows and cols do not match");
		return new Matrix();
	}
	
	double* newvalues = new double[matrix->rows * this->cols];

	double sum;
	for (int i = 0; i < matrix->rows; i++)
		for (int j = 0; j < this->cols; j++)
		{
			sum = 0;
			for (int c = 0; c < matrix->cols; c++)
			{
				sum +=  matrix->values[(i * matrix->cols) + c] * values[j + (c * this->cols)];	
			}
			newvalues[(i*this->cols) + j] = sum;
		}

	Matrix* newmatrix = new Matrix(matrix->rows, this->cols, newvalues);
	
	return newmatrix;
}*/



void
Matrix::print(char* name)
{
  EH_DECLARE;

	EH_Log("\nMatrix [%s]", name);
	for (int r = 0; r < rows; r++)
	{
		EH_Log("\nRow %d [", r);
		for (int c = 0; c < cols; c++)
		{
			EH_Log(" %f", values[r * cols + c]);
		}
		EH_Log(" ]");
	}
	
}

OmniVec3 operator+(OmniVec3 a, OmniVec3 b){
	OmniVec3 c = a;
	c.x += b.x;
	c.y += b.y;
	c.z += b.z;
	return c;
}

OmniVec3 operator-(OmniVec3 a, OmniVec3 b) {
	OmniVec3 c = a;
	c.x -= b.x;
	c.y -= b.y;
	c.z -= b.z;
	return c;
}

OmniVec3 operator*(OmniVec3 a, float b){
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

OmniVec3 operator*(float b, OmniVec3 a){
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

OmniVec3 operator/(OmniVec3 a, float b){
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return a;
}

OmniVec3 operator/(float b, OmniVec3 a){
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return a;
}


Vec4::Vec4() : OmniVec3()
{
	w = 0;
}

Vec4::Vec4(float x, float y, float z, float w) : OmniVec3(x,y,z)
{
	this->w = w;
}

Vec4::Vec4(float incoord[4])
{
	x = incoord[0];
	y = incoord[1];
	z = incoord[2];
	w = incoord[3];
}

Vec4::Vec4(OmniVec3 vec, float w)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = w;
}

	
void 
Vec4::make4tuple(double v[])
{
	v[0] = x; v[1] = y; v[2] = z; v[3] = w;
}


	
void 
Vec4::make4tuple(float v[])
{
	v[0] = x; v[1] = y; v[2] = z; v[3] = w;
}


void
Vec4::set(Vec4 vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void
Vec4::set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void
Vec4::set(OmniVec3 vec, float w)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = w;
}




Ray::Ray(Point3 in_start, OmniVec3 in_direction)
{
	start = in_start;
	direction = in_direction;
}



bool
Ray::intersect(Ray in_ray, Point3& point) 
{

	OmniVec3 r = this->direction;
	OmniVec3 s = in_ray.direction;
	OmniVec3 r0 = Point3ToVec(this->start);
	OmniVec3 s0 = Point3ToVec(in_ray.start);

	//The vector formed between the starting points of the two rays
	OmniVec3 a(r0, s0);

	//This solving technique comes from a webpage 
	//http://astronomy.swin.edu.au/~pbourke/geometry/lineline3d/
	float denom = ( r.dot(r) * s.dot(s) - s.dot(r) * s.dot(r) );			
	if (fabs(denom)    < 0.000001)
		return false;
	float tr =  ( a.dot(s) * s.dot(r) - a.dot(r) * s.dot(s) ) / denom;			
	float ts =  ( a.dot(s) + tr * s.dot(r) ) / ( s.dot(s) );


	//Now we have the times for each ray, so lets get its position at that time
	Point3 rb = this->pointAt(tr);
	Point3 sb = in_ray.pointAt(ts);

	//printf("\nR point = (%f %f %f)", rb.x, rb.y, rb.z);
	//printf("\nS point = (%f %f %f)", sb.x, sb.y, sb.z);
	//returns the midpoint between rb and sb
	Point3 midpoint = rb.lerp(0.5, sb);
	//printf("\nIntersection Pt = (%f %f %f)", midpoint.x, midpoint.y, midpoint.z);

	//set the passed in points values to this midpoints values
	point.x = midpoint.x; point.y = midpoint.y; point.z = midpoint.z;

	return true;
}

bool 
Ray::intersectsTriangle(Point3* triangle, Point3& P)
{
	//first compute plane of the triangle
	//P * N + d = 0
	OmniVec3 v1(
		Point3ToVec(triangle[1]), Point3ToVec(triangle[0]));
	OmniVec3 v2(Point3ToVec(triangle[2]), Point3ToVec(triangle[0]));
	OmniVec3 n = v1.cross(v2);
	n.normalize();
	float d = triangle[0].toVec3().dot(n);

	//Compute intersection time with plane
	float t = - (this->start.toVec3().dot(n) - d) / (this->direction.dot(n));

	//The intersection point P on the plane
	OmniVec3 vec_P = this->start.toVec3() + t * this->direction;
	P = Point3(vec_P.x, vec_P.y, vec_P.z);

	
	OmniVec3 p_tri(Point3ToVec(P), Point3ToVec(triangle[0]));

	OmniVec3 p_unit = p_tri.returnNormalized();

	//float alpha = p_tri.dot(v1);
	//if (alpha < 0 || alpha > 1)
	//	return false;

	//float beta = p_tri.dot(v2);
	//if (beta < 0 || beta > 1)
	//	return false;
	
	/*OmniVec3 b = -p_tri.dot(v1) * v1 + p_tri;

	//float beta = length(p_tri) / (v1.dot(p_unit));
	float beta = b.length() / (v2.returnNormalized().dot(b.returnNormalized()));

	OmniVec3 tb = beta * v2.returnNormalized();
	float alpha = (tb + -1 * b).dot(v1);

	if (alpha < 0 || alpha > 1)
		return false;

	if (beta < 0 || beta > 1)
		return false;

	return true;*/

	OmniVec3 p0(Point3ToVec(P), Point3ToVec(triangle[0]));
	OmniVec3 p1(Point3ToVec(P), Point3ToVec(triangle[1]));
	OmniVec3 p2(Point3ToVec(P), Point3ToVec(triangle[2]));

	float area1 = (p0.cross(p1)).length() / 2.0f;
	float area2 = (p1.cross(p2)).length() / 2.0f;
	float area3 = (p2.cross(p0)).length() / 2.0f;

	if (area1 + area2 + area3 <= (v1.cross(v2)).length() / 2.0f + 0.0000001)
		return true;
	else 
		return false;

}

Point3 
Ray::pointAt(float t)
{
	OmniVec3 dir = this->direction;
	dir = dir * t;

	Point3 newpoint = this->start;

	newpoint.x += dir.x;
	newpoint.y += dir.y;
	newpoint.z += dir.z;

	return newpoint;
}

void
Ray::print()
{
//	LogSystem()->ReportMessage("Ray (%f %f %f) -> (%f %f %f)", start.x, start.y, start.z, direction.x, direction.y, direction.z);

}


CR_Spline::CR_Spline(Point3 _p0, Point3 _p1, Point3 _p2, Point3 _p3)
{
	this->p0 = _p0;
	this->p1 = _p1;
	this->p2 = _p2;
	this->p3 = _p3;
}

Point3 
CR_Spline::getPointAt(float time)
{
	float q[3] = {0, 0, 0};

	for (int i = 0; i < 3; i++)
	{
		q[i] = (float)0.5 * (  ((-p0.getCoord(i) + 3 * p1.getCoord(i) - 3 * p2.getCoord(i) + p3.getCoord(i)) * time * time * time)
						+ ((2 * p0.getCoord(i) - 5 * p1.getCoord(i) + 4 * p2.getCoord(i) - p3.getCoord(i)) * time * time)
						+ ((-p0.getCoord(i) + p2.getCoord(i)) * time)
						+ 2 * p1.getCoord(i) );
	}
	return Point3(q[0], q[1], q[2]);
}

bool
CR_Spline::containsX(float x, float& t)
{
	if (!(p1.x <= x && x <= p2.x))
		return false;
	else
	{
		float range = p2.x - p1.x;
		float dist = x - p1.x;
		float time = (float)fabs(dist/range);
		t = time;
		return true;
	}

}

CR_Spline& 
CR_Spline::operator=(CR_Spline spline)
{
	this->p0 = spline.p0;
	this->p1 = spline.p1;
	this->p2 = spline.p2;
	this->p3 = spline.p3;

	return *this;
}

PieceWise_CR_Spline::PieceWise_CR_Spline(Point3* _pts, int _num_pts)
{
	this->pts = _pts;
	this->num_pts = _num_pts;
	segments = num_pts - 3;
}

Point3
PieceWise_CR_Spline::getPointAt(float in_time)
{
	int current_segment = (int) (in_time * (float)segments );
	//if (current_segment > segments - 1)
	//	current_segment = segments - 1;
	if (in_time >= 1.0)
		current_segment = segments - 1;

	float seg_start_time = (float)current_segment / (float)segments;
	float seg_time = 1.0f / (float)segments;

	float time = (in_time - seg_start_time) / seg_time;

	int point_offset = current_segment;

	Point3 p0 = pts[point_offset + 0];
	Point3 p1 = pts[point_offset + 1];
	Point3 p2 = pts[point_offset + 2];
	Point3 p3 = pts[point_offset + 3];

	float q[3] = {0, 0, 0};

	for (int i = 0; i < 3; i++)
	{
		q[i] = (float)0.5 * (  ((-p0.getCoord(i) + 3 * p1.getCoord(i) - 3 * p2.getCoord(i) + p3.getCoord(i)) * time * time * time)
						+ ((2 * p0.getCoord(i) - 5 * p1.getCoord(i) + 4 * p2.getCoord(i) - p3.getCoord(i)) * time * time)
						+ ((-p0.getCoord(i) + p2.getCoord(i)) * time)
						+ 2 * p1.getCoord(i) );
	}

	return Point3(q[0], q[1], q[2]);
}

PieceWise_CR_Spline::~PieceWise_CR_Spline()
{
	if (pts != 0)
		delete[] pts;
}

PieceWise_CR_Spline& 
PieceWise_CR_Spline::operator=(PieceWise_CR_Spline spline)
{
	if (pts != 0)
		delete[] pts;	

	pts = new Point3[spline.num_pts];
	this->num_pts = spline.num_pts;
	this->segments = spline.segments;

	for (int i = 0; i < spline.num_pts; i++)
	{
		this->pts[i] = spline.pts[i];
	}

	return *this;
}

PieceWise_CR_Spline::PieceWise_CR_Spline(const PieceWise_CR_Spline& spline)
{
	this->num_pts = spline.num_pts;
	this->segments = spline.segments;
	this->pts = new Point3[spline.num_pts];

	for (int i = 0; i < spline.num_pts; i++)
	{
		this->pts[i] = spline.pts[i];
	}
}





Quat::Quat(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Quat::Quat(OmniVec3 vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = 0;
}


Quat::Quat(float theta, OmniVec3 u)
{	
	x = u.x * (float)sin(theta/2);
	y = u.y * (float)sin(theta/2);
	z = u.z * (float)sin(theta/2);
	w = (float)cos(theta/2);
}

float
Quat::length()
{
	return (float)sqrt(x*x + y*y + z*z + w*w);
}

void 
Quat::set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void
Quat::normalize()
{
	float L = length();
	if (L != 0.0)
	{
		x = x / L;
		y = y / L;
		z = z / L;
		w = w / L;
	}
	else 
		x = y = z = w = 0.0;
}

Quat
Quat::conjugate()
{
	float _x = -x;
	float _y = -y;
	float _z = -z;
	float _w = w;

	Quat quat(_x, _y, _z, _w);
	return quat;
}

Quat
Quat::multiply(Quat B)
{
	Quat conjB = B.conjugate();

	Quat temp = this->multiplyQuat(conjB);

	return B.multiplyQuat(temp);
}

Quat
Quat::multiplyQuat(Quat B)
{
	float _x = w*B.x + x*B.w + y*B.z - z*B.y;
	float _y = w*B.y - x*B.z + y*B.w + z*B.x;
	float _z = w*B.z + x*B.y - y*B.x + z*B.w;
	float _w = w*B.w - x*B.x - y*B.y - z*B.z;

	return Quat(_x, _y, _z, _w);
}

OmniVec3
Quat::toVec3()
{
	//float RADIANS = .0174533f;
	float cos_angle  = w;
  //  float angle      = (float)acos( cos_angle ) * 2 * RADIANS;
    float sin_angle  = (float)sqrt( 1.0 - cos_angle * cos_angle );

    if (  sin_angle  < 0.0005  &&  sin_angle > -0.0005 )
      sin_angle = 1;

	float vx = x / sin_angle;
    float vy = y / sin_angle;
    float vz = z / sin_angle;
	return OmniVec3(vx, vy, vz);
}

Quat&
Quat::operator=(Quat inquat){
	this->x = inquat.x;
	this->y = inquat.y;
	this->z = inquat.z;
	this->w = inquat.w;
	return *this;
}

Quat operator+(Quat a, Quat b){
	Quat c = a;
	c.x += b.x;
	c.y += b.y;
	c.z += b.z;
	c.w += b.w;
	return c;
}


void
AffMatrix::inverse(AffMatrix *dest)
{
	dest->m[0] = m[0];
	dest->m[1] = m[4];
	dest->m[2] = m[8];
	dest->m[3] = m[3];

	dest->m[4] = m[1];
	dest->m[5] = m[5];
	dest->m[6] = m[9];
	dest->m[7] = m[7];

	dest->m[8] = m[2];
	dest->m[9] = m[6];
	dest->m[10] = m[10];
	dest->m[11] = m[11];

	dest->m[12] = 0.0f;
	dest->m[13] = 0.0f;
	dest->m[14] = 0.0f;
	dest->m[15] = 1.0f;

	double pos[4];
	pos[0] = m[12]; pos[1] = m[13]; pos[2] = m[14]; pos[3] = 1.0f;
	
	dest->transformVector(pos);
	dest->m[12] =(float) -pos[0];
	dest->m[13] =(float) -pos[1];
	dest->m[14] =(float) -pos[2];
	dest->m[15] =(float) m[15];
}




void AffMatrix::transformPoint(double v[])
{
	double p1, p2, p3;

	p1 = this->m[0] * v[0] + this->m[4] * v[1] + this->m[8] * v[2] + this->m[12] * v[3];
	p2 = this->m[1] * v[0] + this->m[5] * v[1] + this->m[9] * v[2] + this->m[13] * v[3];
	p3 = this->m[2] * v[0] + this->m[6] * v[1] + this->m[10] * v[2] + this->m[14] * v[3];

	v[0] = p1;
	v[1] = p2;
	v[2] = p3;
	v[3] = 1.0f;
}

void AffMatrix::transformPoint(float v[])
{
	double p1, p2, p3, p4;

	p1 = this->m[0] * v[0] + this->m[4] * v[1] + this->m[8] * v[2] + this->m[12] * v[3];
	p2 = this->m[1] * v[0] + this->m[5] * v[1] + this->m[9] * v[2] + this->m[13] * v[3];
	p3 = this->m[2] * v[0] + this->m[6] * v[1] + this->m[10] * v[2] + this->m[14] * v[3];
	p4 = this->m[3] * v[0] + this->m[7] * v[1] + this->m[11] * v[2] + this->m[15] * v[3];

	v[0] = (float)p1;
	v[1] = (float)p2;
	v[2] = (float)p3;
	v[3] = (float)p4;
}


void AffMatrix::transformVector(double v[])
{
	double v1, v2, v3;

	v1 = this->m[0] * v[0] + this->m[4] * v[1] + this->m[8] * v[2];
	v2 = this->m[1] * v[0] + this->m[5] * v[1] + this->m[9] * v[2];
	v3 = this->m[2] * v[0] + this->m[6] * v[1] + this->m[10] * v[2];

	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
}

void AffMatrix::transformVector(float v[])
{
	double v1, v2, v3;

	v1 = this->m[0] * v[0] + this->m[4] * v[1] + this->m[8] * v[2];
	v2 = this->m[1] * v[0] + this->m[5] * v[1] + this->m[9] * v[2];
	v3 = this->m[2] * v[0] + this->m[6] * v[1] + this->m[10] * v[2];

	v[0] = (float)v1;
	v[1] = (float)v2;
	v[2] = (float)v3;
}

bool
Plane::isPointInFront(Vec4 p)
{
	return equation.dot(p) > 0;
}

bool
Plane::isPointInFront(OmniVec3 p)
{
	return equation.dot(Vec4(p, 1)) > 0;
}

bool
Plane::pointsCulled(std::vector<Vec4> points)
{
	bool culled = true;
	unsigned int i = 0;
	
	while(culled && i < points.size())
	{
		culled = equation.dot(points[i]) < 0;
		i++;
	}

	return culled;
}

bool
Plane::pointsCulled(std::vector<Plane> planes, std::vector<Vec4> points)
{
	unsigned int planes_size = (unsigned int)planes.size();
	unsigned int points_size = (unsigned int)points.size();
	
	for(unsigned int p = 0; p < planes_size; p++)
	{
		bool culled = true;
		unsigned int i = 0;
		
		while(culled && i < points_size)
		{
			culled = planes[p].equation.dot(points[i]) < 0;
			i++;
		}

		if(culled)
		{
			return true;
		}
	}

	return false;
}

float
Plane::distanceToPoint(Vec4 p)
{
	return equation.dot(p);
}




#ifdef notdef
/////////////////////////////////////////
void OmniQuad::QuadToEquations(float UPlaneEquation[4] ,float VPlaneEquation[4],float PlaneEquation[4])
{
	/////////////// to do, use projective texturing matricies to do the projective texturing rather then uv plane equations.....

/////// in the pixel shader we have 
/////// the audience 
	//  the projector
	// the quad corners
	// the current xyz of the screenshape
	// from the audience position, we shoot a ray through the xyz of the screen shape
	// and penetrate into the quad to get the s and t of the quad to do projective texturing...


//////////	// we are going to construct a system of linear equations and solve them to get the matrix that translates plane hit positions to UV coordinates
		QuadToPlaneEquation(PlaneEquation );

		
		
		OmniVec3 nonplanar = (tl-bl).cross(br-bl);


		float QuadMatrix[4][4]=
		{		
			{bl.x,bl.y,bl.z,1},
			{tl.x,tl.y,tl.z,1},
			{br.x,br.y,br.z,1},
			{nonplanar.x,nonplanar.y,nonplanar.z,1}
		};
		simMatrix_4x4 MatrixQuad2(QuadMatrix);
		simMatrix_4x4 MatrixQuad;
		MatrixQuad2.transpose(MatrixQuad);

		float _Uvalues[4] = {0,0,1,0};
		float _Vvalues[4] = {0,1,0,0};
		simpVec Uvalues(4,_Uvalues);
		simpVec Vvalues(4,_Vvalues);
		
		simpVec UEquation(4);
		simpVec VEquation(4);
		MatrixQuad2.solveSystem(Uvalues,UEquation);//solveLeastSquares
		MatrixQuad2.solveSystem(Vvalues,VEquation);//solveLeastSquares

		for(int i=0; i!=4; i++)
		{
		UPlaneEquation[i]= UEquation.getVal(i);
		VPlaneEquation[i] = VEquation.getVal(i);
		}

//////////////// error checking to make sure the solve worked.................

		float predictedP00_u = UPlaneEquation[0] * bl.x+	UPlaneEquation[1] * bl.y+UPlaneEquation[2] *bl.z+UPlaneEquation[3];
		float predictedP00_v = VPlaneEquation[0] * bl.x+	VPlaneEquation[1] * bl.y+VPlaneEquation[2] *bl.z+VPlaneEquation[3];

		float predictedP01_u = UPlaneEquation[0] * tl.x+	UPlaneEquation[1] * tl.y+UPlaneEquation[2] *tl.z+UPlaneEquation[3];
		float predictedP01_v = VPlaneEquation[0] * tl.x+	VPlaneEquation[1] * tl.y+VPlaneEquation[2] *tl.z+VPlaneEquation[3];

		float predictedP10_u = UPlaneEquation[0] * br.x+	UPlaneEquation[1] * br.y+UPlaneEquation[2] *br.z+UPlaneEquation[3];
		float predictedP10_v = VPlaneEquation[0] * br.x+	VPlaneEquation[1] * br.y+VPlaneEquation[2] *br.z+VPlaneEquation[3];

		float predictedP11_u = UPlaneEquation[0] * tr.x+	UPlaneEquation[1] * tr.y+UPlaneEquation[2] *tr.z+UPlaneEquation[3];
		float predictedP11_v = VPlaneEquation[0] * tr.x+	VPlaneEquation[1] * tr.y+VPlaneEquation[2] *tr.z+VPlaneEquation[3];


		float error =
		(predictedP00_u -0)*(predictedP00_u -0)+
		 (predictedP00_v -0)*(predictedP00_v -0)+

		 (predictedP01_u -0)*(predictedP01_u -0)+
		 (predictedP01_v -1)*(predictedP01_v -1)+

		 (predictedP10_u -1)*(predictedP10_u -1)+
		 (predictedP10_v -0)*(predictedP10_v -0)+

		 (predictedP11_u -1)*(predictedP11_u -1)+
		 (predictedP11_v -1)*(predictedP11_v -1);
		
		if(error>.1)
			LogSystem()->ReportError("Warning RMS Error in fitting ,matrix to quad is > .1\n");
}



void OmniQuad::QuadToPlaneEquation(float PlaneEquation[4])
	{
		PlaneEquation[0] =	bl.y*(tl.z-tr.z) + tl.y*(tr.z-bl.z) + tr.y*(bl.z-tl.z);
		PlaneEquation[1] =	bl.z*(tl.x-tr.x) + tl.z*(tr.x-bl.x) + tr.z*(bl.x-tl.x);
		PlaneEquation[2]=	bl.x*(tl.y-tr.y) + tl.x*(tr.y-bl.y) + tr.x*(bl.y-tl.y);
		PlaneEquation[3]=	-bl.x*(tl.y*tr.z - tr.y*tl.z) - tl.x*(tr.y*bl.z - bl.y*tr.z) - tr.x*(bl.y*tl.z - tl.y*bl.z);
	}
#endif