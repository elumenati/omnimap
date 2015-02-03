#ifndef _UO_simMatrix_H_
#define _UO_simMatrix_H_

#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include "../omnimap.h"


class simpVec;
class simpVec3;
class HPoint;
class simMatrix;
class simMatrix_4x4;

extern simpVec3 UNIT_Y;
extern HPoint ZERO;
extern HPoint UNIT_Y_PT;

class unmatchedsimpVectorSizes{};
class unmatchedsimpVectorsimMatrixSizes{};
class unmatchedsimMatrixSizes{};
class nonSquaresimMatrix{};
class indexOutOfBounds{};

class simpVec {
	// allow class simMatrix access to private/protected members
	friend simMatrix;
	// not sure why these two are necessary, but MS VC++ gives me compiler errors...
	friend simpVec3;
	friend HPoint;

protected:
	int size;
	float *vals;

public:
	simpVec( int _size, const float* _vals=NULL );
    simpVec( const simpVec& v );
	virtual ~simpVec();

	inline int getSize() 
		const 
	{ 
		return size; 
	}
	inline float& getVal( int index ) const 
	{ 
		if(index>=size)
			LogSystem()->ReportError("error::getval()... index >= size\n"); 	
		return vals[index]; 
	}
	int max_abs_val() const;
    simpVec& zero();
	simpVec& operator= ( const simpVec& v );
	simpVec& operator+= ( const simpVec& v );
	simpVec& operator-= ( const simpVec& v );
	simpVec& operator*= ( float s );

    friend bool operator== ( const simpVec& v, const simpVec& w );
    friend bool operator!= ( const simpVec& v, const simpVec& w );
	friend simpVec operator+ ( const simpVec& v, const simpVec& w );
	friend simpVec operator- ( const simpVec& v, const simpVec& w );
	friend simpVec operator* ( const simpVec& v, float s );
	friend simpVec operator* ( float s, const simpVec& v );
	friend float dot( const simpVec& v, const simpVec& w );
	friend float operator| ( const simpVec& v, const simpVec& w ) { return dot(v,w); }
    friend float length( const simpVec& v ) { return sqrt( v | v ); }
	friend simpVec& normalize( simpVec& v );
	friend simpVec operator* ( const simMatrix& m, const simpVec& v );
    friend void print( const simpVec& v );
    friend void print( const simpVec& v, char* s );
};

class HPoint : public simpVec
{
public:
	HPoint( const float* _vals=NULL ) : simpVec(4,_vals) {}
    HPoint( const HPoint& p ) : simpVec(p) {}
    HPoint( float x, float y, float z ) : simpVec(4)
		{ vals[0]=x; vals[1]=y; vals[2]=z; vals[3]=1.0; }
    HPoint( float x, float y, float z, float h ) : simpVec(4)
		{ vals[0]=x; vals[1]=y; vals[2]=z; vals[3]=h; }
	HPoint( const simpVec3& v );
	HPoint( const simpVec& v );

    HPoint& init( float x, float y, float z );
    HPoint& init( float x, float y, float z, float h );

	// Return the distance between the two points.
    float distance( const HPoint& p ) { return length( simpVec(*this - p) ); }
	void homogenize();

    void print() const;
    void print( const char* s ) const;

	friend simpVec3 operator- ( HPoint& p, HPoint& q );
	friend HPoint operator- ( HPoint& p, const simpVec3& v );
	friend HPoint operator+ ( HPoint& p, const simpVec3& v );
	friend HPoint operator+ ( const simpVec3& v, HPoint& p );
	friend HPoint operator* ( const simMatrix_4x4& m, HPoint& p );
};

class simpVec3 : public simpVec
{
public:
	simpVec3() : simpVec(3) {}
	simpVec3( const float* _vals ) : simpVec(3,_vals) {}
    simpVec3( const simpVec3& v ) : simpVec(v) {}
	simpVec3( float x, float y, float z ) : simpVec(3) { vals[0]=x; vals[1]=y; vals[2]=z; }
	simpVec3( const HPoint& p ) : simpVec(3)
		{ vals[0]=p.vals[0]; vals[1]=p.vals[1]; vals[2]=p.vals[2]; }
	simpVec3( const simpVec& v );
	//~simpVec3();

	friend simpVec3 cross( const simpVec3& v, const simpVec3& w );
	friend simpVec3 operator^ ( const simpVec3& v, const simpVec3& w ) { return cross(v,w); }
    friend float determinant( const simpVec3& r1, const simpVec3& r2, const simpVec3& r3 );
	/*
    static void basisCoords( HPoint* o, HPoint* b1, HPoint* b2,
        HPoint* p, int ind1, int ind2, float* a1, float* a2 );
	*/
	friend simpVec3 operator- ( const HPoint& p, const HPoint& q );
	friend HPoint operator- ( const HPoint& p, const simpVec3& v );
	friend HPoint operator+ ( const HPoint& p, const simpVec3& v );
	friend HPoint operator+ ( const simpVec3& v, const HPoint& p );
};

class simMatrix {
protected:
	int rows, cols;
	float *vals;
	static float epsilon;

	int index( int row, int col ) const { return row * cols + col; }
	bool isZero( float f )  const { return (abs(f) < epsilon); }
	void appendsimMatrix( const simMatrix& m, simMatrix& dst );
	void scaleRow( int row, float s );
	void addRows( int src, int dst, float s );
	void swapRows( int row1, int row2 );
	void gaussJordan();
	int getSystemSolution( simpVec& dst );
	bool getInvertedsimMatrix( simMatrix& dst );

public:
	void appendsimpVec( const simpVec& v, simMatrix& dst ); // normally sould be protected
	void simMatrix::appendsimpVec( const simpVec& v);
	float& getVal ( int i, int j ) const { return vals[ index(i,j) ]; }// normally sould be protected

	static const int NO_SOLUTIONS;
	static const int ONE_SOLUTION;
	static const int INFINITE_SOLUTIONS;

	simMatrix( int _rows, int _cols, const float* _vals=NULL );
	simMatrix( const simMatrix& m );
	simMatrix( float m[4][4] );
	~simMatrix();
	simMatrix& makeZero();
	simMatrix& makeIdentity();
	int getRows() const { return rows; }
	int getCols() const { return cols; }
	bool inverse( simMatrix& dst );
	void transpose( simMatrix& dst );
	int solveSystem( const simpVec& b, simpVec& x );
	int solveLeastSquares( const simpVec& b, simpVec& x );
	void print() const;

	simMatrix& operator= ( const simMatrix& m );
	simMatrix operator+ ( const simMatrix& m );
	simMatrix operator- ( const simMatrix& m );
	simMatrix operator* ( float s ) const;
	friend simMatrix operator* ( float s, const simMatrix& m );
	simMatrix operator* ( const simMatrix& m );
	friend simpVec operator* ( const simMatrix& m, const simpVec& v );

	
	// _rows, int _cols must be 4 x 4
	void simMatrix::simMatrix_PUT_OPEN_GL( int _rows, int _cols, const float *_vals );

	void simMatrix::simMatrix_GET_OPEN_GL( int  &_rows, int  &_cols,  float *_vals );
	
};


enum MAT_AXIS {
    MAT_X_AXIS,
    MAT_Y_AXIS,
    MAT_Z_AXIS
};

class simMatrix_4x4 : public simMatrix
{
protected:
    float mult[16];

    void multiplyArr( const float *m1, const float *m2, float *dst );
    void makeIdentityArr( float *m );
    void makeZeroArr( float *m );
    void copysimMatrix_4x4( const float *src, float *dst );
	void transposeArr( const float *src, float *dst );

public:
    simMatrix_4x4( const float* _vals=NULL ) : simMatrix( 4, 4, _vals ) {}
    simMatrix_4x4( const simMatrix_4x4& m ) : simMatrix( m ) {}
	simMatrix_4x4( const simMatrix& m ) : simMatrix( m ) { if (rows!=4 || cols!=4) throw unmatchedsimMatrixSizes(); }
	simMatrix_4x4( float m[4][4] ) : simMatrix( m ) {}

    void timesTranslation( const simpVec3& v );
    void timesRotation( float angle, MAT_AXIS axis );
    void timesScale( const simpVec3& v );
    void timesView( HPoint& eye, HPoint& coi, const simpVec3& vup );
    void timesInvView( HPoint& eye, HPoint& coi, const simpVec3& vup );
    void timesPerspective( float d, float f, float w, float h );
    void transformPoint( HPoint& pnt, HPoint& dst ) const;
    void transformAndNormalizePoint( HPoint& pnt, HPoint& dst ) const;
	float* getOpenGLsimMatrix() const;

	simMatrix_4x4& operator= ( const simMatrix_4x4& m );
	simMatrix_4x4  operator* ( const simMatrix_4x4& m );
	friend HPoint operator* ( const simMatrix_4x4& m, HPoint& p );
};


#endif
