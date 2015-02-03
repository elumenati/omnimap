#include <stdio.h>
#include "simple_Matrix.h"
#pragma warning(disable: 4244) // disable warning 4244:

simpVec3 UNIT_Y( 0.0, 1.0, 0.0 );
HPoint ZERO( 0.0, 0.0, 0.0 );
HPoint UNIT_Y_PT( 0.0, 1.0, 0.0, 1.0 );

/******************************************************************************/
/******************************************************************************/
/**************************                          **************************/
/**************************   simpVec class members   **************************/
/**************************                          **************************/
/******************************************************************************/
/******************************************************************************/

/* constructor */
simpVec::simpVec( int _size, const float *_vals ) : size( _size )
{
	vals = new float[ size ];
	if ( _vals != NULL )
		for ( int i=0; i<size; i++ )
			vals[i] = _vals[i];
}

/* copy constructor */
simpVec::simpVec( const simpVec& v )
{
	vals = new float[ size = v.size ];
	for ( int i=0; i<size; i++ )
		vals[i] = v.vals[i];
}

/* destructor */
simpVec::~simpVec()
{
	if ( vals != NULL )
		delete [] vals;
}


/******************************************************************************/
/****************************    public members    ****************************/
/******************************************************************************/

// Return the index of the simpVector component with the largest absolute value.
int simpVec::max_abs_val() const
{
	float max = 0.0;
	int index = 0;
	for ( int i=0; i<size; i++ ) {
		if ( abs( vals[i] ) > max ) {
			max = abs( vals[i] );
			index = i;
		}
	}
	return index;
}

// Set all simpVector elements to zero.
simpVec& simpVec::zero()
{
	for ( int i=0; i<size; i++ )
		vals[i] = 0.0;
	return *this;
}

// Copy assignment.
simpVec& simpVec::operator=( const simpVec& v )
{
	if ( this != &v ) {
		delete [] vals;
		vals = new float[ size = v.size ];
		for ( int i=0; i<size; i++ )
			vals[i] = v.vals[i];
	}
	return *this;
}

simpVec& simpVec::operator+=( const simpVec& v )
{
	if ( size != v.size )
		throw unmatchedsimpVectorSizes();

	for ( int i=0; i<size; i++ )
		vals[i] += v.vals[i];
	return *this;
}

simpVec& simpVec::operator-=( const simpVec& v )
{
	if ( size != v.size )
		throw unmatchedsimpVectorSizes();

	for ( int i=0; i<size; i++ )
		vals[i] -= v.vals[i];
	return *this;
}

simpVec& simpVec::operator*=( float s )
{
	for ( int i=0; i<size; i++ )
		vals[i] *= s;
	return *this;
}

// Return true if the elements of the two simpVectors are equal, else false.
bool operator==( const simpVec& v, const simpVec& w )
{
	if ( v.size != w.size )
		throw unmatchedsimpVectorSizes();

	for ( int i=0; i<v.size; i++ )
		if ( v.vals[i] != w.vals[i] )
			return false;
	return true;
}

// Return true if the elements of the two simpVectors are not equal, else false.
bool operator!=( const simpVec& v, const simpVec& w )
{
	if ( v.size != w.size )
		throw unmatchedsimpVectorSizes();

	for ( int i=0; i<v.size; i++ )
		if ( v.vals[i] != w.vals[i] )
			return true;
	return false;
}

simpVec operator+( const simpVec& v, const simpVec& w )
{
	if ( v.size != w.size )
		throw unmatchedsimpVectorSizes();

	simpVec r = v;
	return r += w;
}

simpVec operator-( const simpVec& v, const simpVec& w )
{
	if ( v.size != w.size )
		throw unmatchedsimpVectorSizes();

	simpVec r = v;
	return r -= w;
}

simpVec operator*( const simpVec& v, float s )
{
	simpVec r = v;
	return r *= s;
}

simpVec operator*( float s, const simpVec& v )
{
	simpVec r = v;
	return r *= s;
}

// Return the dot product of two simpVectors.
float dot( const simpVec& v, const simpVec& w )
{
	if ( v.size != w.size )
		throw unmatchedsimpVectorSizes();

	float d = 0.0;

	for ( int i=0; i<v.size; i++ )
		d += v.vals[i] * w.vals[i];
	return d;
}

// Normalize the given simpVector.
simpVec& normalize( simpVec& v )
{
	float len = length( v );

	if ( len == 0.0 ) {
		LogSystem()->ReportError( "Attempt to divide by zero.\n" );
		exit( 0 );
	}

	for ( int i=0; i<v.size; i++ )
		v.vals[i] /= len;
	return v;
}

// Calculate simMatrix product:  return m * v.
simpVec operator*( const simMatrix& m, const simpVec& v )
{
	int i, j;
	simpVec r( m.rows );

	if ( v.size != m.cols )
		throw unmatchedsimpVectorsimMatrixSizes();

	for ( i=0; i<r.size; i++ ) {
		r.vals[i] = 0.0;
		for ( j=0; j<v.size; j++ )
			r.vals[i] += const_cast<simMatrix&>(m).getVal(i,j) * v.vals[j];
	}
	return r;
}

// print the simpVector.
void print( const simpVec& v )
{
    LogSystem()->ReportMessage( "simpVec: " );
	for ( int i=0; i<v.size; i++ )
		LogSystem()->ReportMessage( " %f", v.vals[i] );
	LogSystem()->ReportMessage( "\n" );
}

// print the simpVector along with the passed string.
void print( const simpVec& v, char *s )
{
    LogSystem()->ReportMessage( "%s", s );
	for ( int i=0; i<v.size; i++ )
		LogSystem()->ReportMessage( " %f", v.vals[i] );
	LogSystem()->ReportMessage( "\n" );
}



/******************************************************************************/
/******************************************************************************/
/*************************                            *************************/
/*************************   simpVec3 class members    *************************/
/*************************                            *************************/
/******************************************************************************/
/******************************************************************************/

// A simpVec3 represents a 3D simpVector with x, y, & z components.

// constructor
simpVec3::simpVec3( const simpVec& v ) : simpVec(3)
{
	for ( int i=0; i<3; i++ )
		vals[i] = (i < v.size) ? v.vals[i] : 0.0;
}

/******************************************************************************/
/****************************    public members    ****************************/
/******************************************************************************/

// Returns the cross product of two simpVec3s.
simpVec3 cross( const simpVec3& v, const simpVec3& w )
{
	simpVec3 r;

    r.vals[0] = v.vals[1]*w.vals[2] - v.vals[2]*w.vals[1];
    r.vals[1] = v.vals[2]*w.vals[0] - v.vals[0]*w.vals[2];
    r.vals[2] = v.vals[0]*w.vals[1] - v.vals[1]*w.vals[0];

    return r;
}

// Return determinant of 3x3 simMatrix formed by three row simpVec3s.
float determinant( const simpVec3& r1, const simpVec3& r2, const simpVec3& r3 )
{
    float t1, t2, t3;

    t1 = r1.vals[0] * (r2.vals[1]*r3.vals[2] - r2.vals[2]*r3.vals[1]);
    t2 = r1.vals[1] * (r2.vals[0]*r3.vals[2] - r2.vals[2]*r3.vals[0]);
    t3 = r1.vals[2] * (r2.vals[0]*r3.vals[1] - r2.vals[1]*r3.vals[0]);

    return t1 - t2 + t3;
}

/*
// Given the two basis simpVectors o->b1 and o->b2, this determines
// the coordinates of point p with respect to these basis
// simpVectors in the linear space formed by them.  ind1 and ind2
// represent the coordinates of the plane into which the
// simpVectors and points are projected, i.e. this is done in only
// two dimensions.  The basis coordinates are returned in
// a1 and a2.
void simpVec3::basisCoords( HPoint *o, HPoint *b1, HPoint *b2,
    HPoint *p, int ind1, int ind2, float *a1, float *a2 )
{
    simpVec3 v1, v2, v3;
    float denom;

    p-> diff( o, &v1 );
    b1->diff( o, &v2 );
    b2->diff( o, &v3 );

    denom = v2.v[ind1]*v3.v[ind2] - v3.v[ind1]*v2.v[ind2];
    *a1   = v1.v[ind1]*v3.v[ind2] - v3.v[ind1]*v1.v[ind2];
    *a2   = v2.v[ind1]*v1.v[ind2] - v1.v[ind1]*v2.v[ind2];
    *a1 /= denom;
    *a2 /= denom;
}
*/


/******************************************************************************/
/******************************************************************************/
/*************************                            *************************/
/*************************    HPoint class members    *************************/
/*************************                            *************************/
/******************************************************************************/
/******************************************************************************/

// An HPoint represents a 3D point with homogeneous coordinates, i.e. (x, y, z, h).
// It probably should be a separate class, rather than one derived from simpVec, but 
// there are some functions in simpVec that may be useful to the HPoint, and I haven't
// had time anyway.

// constructor
HPoint::HPoint( const simpVec3& v ) : simpVec(4)
{
	for ( int i=0; i<3; i++ )
		vals[i] = v.vals[i];
	vals[3] = 1.0;
}

HPoint::HPoint( const simpVec& v ) : simpVec(4)
{
	for ( int i=0; i<4; i++ )
		vals[i] = (i < v.size) ? v.vals[i] : 0.0;
}

/******************************************************************************/
/****************************    public members    ****************************/
/******************************************************************************/

// Initialize to x, y, z.
HPoint& HPoint::init( float x, float y, float z )
{
    vals[0] = x;
    vals[1] = y;
    vals[2] = z;
    vals[3] = 1.0;

    return *this;
}

// Initialize to x, y, z, h.
HPoint& HPoint::init( float x, float y, float z, float h )
{
    vals[0] = x;
    vals[1] = y;
    vals[2] = z;
    vals[3] = h;

    return *this;
}

// print the HPoint.
void HPoint::print() const
{
    LogSystem()->ReportMessage( "HPoint:  %f, %f, %f, %f\n", vals[0], vals[1], vals[2], vals[3] );
}

// print the HPoint along with the passed string.
void HPoint::print( const char *s ) const
{
    LogSystem()->ReportMessage( "%s", s );
    LogSystem()->ReportMessage( "%f, %f, %f, %f\n", vals[0], vals[1], vals[2], vals[3] );
}

// Divide the HPoint by its homogeneous coordinate.
void HPoint::homogenize()
{
	if ( vals[3] != 1.0 && vals[3] != 0.0 )
		for ( int i=0; i<3; i++ )
			vals[i] /= vals[3];
}

simpVec3 operator- ( HPoint& p, HPoint& q )
{
	p.homogenize();
	q.homogenize();
	return simpVec3( p.getVal(0) - q.getVal(0),
				 p.getVal(1) - q.getVal(1),
				 p.getVal(2) - q.getVal(2) );
}

HPoint operator- ( HPoint& p, const simpVec3& v )
{
	p.homogenize();
	return HPoint( p.getVal(0) - v.getVal(0),
				   p.getVal(1) - v.getVal(1),
				   p.getVal(2) - v.getVal(2) );

}

HPoint operator+ ( HPoint& p, const simpVec3& v )
{
	p.homogenize();
	return HPoint( p.getVal(0) + v.getVal(0),
				   p.getVal(1) + v.getVal(1),
				   p.getVal(2) + v.getVal(2) );
}

HPoint operator+ ( const simpVec3& v, HPoint& p )
{
	return p + v;
}



/******************************************************************************/
/******************************************************************************/
/**************************                          **************************/
/**************************   simMatrix class members   **************************/
/**************************                          **************************/
/******************************************************************************/
/******************************************************************************/



/* constructor */
void simMatrix::simMatrix_PUT_OPEN_GL( int _rows, int _cols, const float *_vals )

{
 rows = _rows ;
 cols = _cols ;

	delete (vals);
	vals = new float[ rows * cols ];
	if ( _vals != NULL )
		for ( int i=0; i<rows; i++ )
			for ( int j=0; j<cols; j++ )
				vals[ index(i,j) ] = _vals[ index(i,j) ];
}
void simMatrix::simMatrix_GET_OPEN_GL( int &_rows, int &_cols,  float *_vals )

{

	 _rows = rows  ;
	 
	 _cols = cols ;
	//vals = new float[ rows * cols ];
	if ( _vals != NULL )
		for ( int i=0; i<rows; i++ )
			for ( int j=0; j<cols; j++ )
				_vals[ index(i,j) ] = (float) vals[ index(i,j) ] ;
}





/* constructor */
simMatrix::simMatrix( int _rows, int _cols, const float *_vals )
: rows( _rows ), cols( _cols )
{
	vals = new float[ rows * cols ];
	if ( _vals != NULL )
		for ( int i=0; i<rows; i++ )
			for ( int j=0; j<cols; j++ )
				vals[ index(i,j) ] = _vals[ index(i,j) ];
}

/* copy constructor */
simMatrix::simMatrix( const simMatrix& m ) : rows( m.rows ), cols( m.cols )
{
	vals = new float[ rows * cols ];
	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			vals[ index(i,j) ] = m.vals[ index(i,j) ];
}

simMatrix::simMatrix( float m[4][4] ) : rows( 4 ), cols( 4 )
{
	vals = new float[ 16 ];
	for ( int i=0; i<4; i++ )
		for ( int j=0; j<4; j++ )
			vals[ index(i,j) ] = m[i][j];
}

/* destructor */
simMatrix::~simMatrix()
{
	if ( vals != NULL )
		delete [] vals;
}


/******************************************************************************/
/**************************    protected members    ***************************/
/******************************************************************************/

float simMatrix::epsilon = (float) 0.0000001;

// Append a simpVector to a simMatrix:  dst = this:v.
void simMatrix::appendsimpVec( const simpVec& v, simMatrix& dst )
{
	int i, j;

	if ( v.getSize() != rows || rows != dst.rows || cols+1 != dst.cols )
		throw unmatchedsimMatrixSizes();

	for ( i=0; i<rows; i++ )
		for ( j=0; j<cols; j++ )
			dst.getVal(i,j) = getVal(i,j);

	for ( i=0; i<rows; i++ )
		dst.getVal( i, dst.cols - 1 ) = v.vals[i];
}

void simMatrix::appendsimpVec( const simpVec& v)
{
	simMatrix newsimMatrix(rows,cols+1);
	appendsimpVec(v,newsimMatrix);
	*this = newsimMatrix;

}



// Append a simMatrix to a simMatrix:  dst = this:m.
void simMatrix::appendsimMatrix( const simMatrix& m, simMatrix& dst )
{
	int i, j;

	if ( rows != m.rows || rows != dst.rows ||
		 cols + m.cols != dst.cols )
		throw unmatchedsimMatrixSizes();

	for ( i=0; i<rows; i++ ) {
		for ( j=0; j<cols; j++ )
			dst.getVal(i,j) = getVal(i,j);
		for ( j=0; j<m.cols; j++ )
			dst.getVal(i,cols+j) = m.getVal(i,j);
	}
}

// Scale one row of a simMatrix.
void simMatrix::scaleRow( int row, float s )
{
	if ( row < 0 || row >= rows )
		throw indexOutOfBounds();

	for ( int i=0; i<cols; i++ )
		getVal(row,i) *= s;
}

// Add a scalar multiple of one row of a simMatrix to another.
void simMatrix::addRows( int src, int dst, float s )
{
	if ( src < 0 || src >= rows || dst < 0 || dst >= rows )
		throw indexOutOfBounds();

	for ( int i=0; i<cols; i++ )
		getVal(dst,i) += getVal(src,i) * s;
}

// Swap two rows in a simMatrix.
void simMatrix::swapRows( int row1, int row2 )
{
	if ( row1 < 0 || row1 >= rows || row2 < 0 || row2 >= rows )
		throw indexOutOfBounds();

	float tmp;

	for ( int i=0; i<cols; i++ ) {
		tmp = getVal(row1,i);
		getVal(row1,i) = getVal(row2,i);
		getVal(row2,i) = tmp;
	}
}

// Perform Gauss-Jordan elimination on the simMatrix.
// The given simMatrix may represent A:b in the system Ax = b.
// In this case, if there is exactly one solution, the resulting
// simMatrix represents B:x, where x is the simpVector solution.
// Call getSystemSolution() to extract x.
//
// Alternatively, the given simMatrix may represent A:I, where
// A is a square simMatrix and I is an identity simMatrix.  Then the
// resulting simMatrix represents either I:B, where B is the inverse
// of A, or J:X, where J is not an identity simMatrix, and therefore
// A does not have an inverse.  Call getInvertedsimMatrix() to check
// if an inverse exists, and to extract it (B) if it does.
void simMatrix::gaussJordan()
{
	int i, row_cursor=0, col_cursor, save_row;

	// Step 0: initialize the cursor.
	// initialize cursor to row 0 and the first column with a non-zero entry.
	for ( col_cursor=0; col_cursor < cols; col_cursor++ ) {
		for ( i=0; i<rows; i++ ) {
			// if entry is non-zero (approximately)
			if ( !isZero( getVal(i,col_cursor) ) ) {
				save_row = i;
				goto cursor_placed;
			}
		}
	}
	// cursor not placed - must be zero simMatrix.
	return;

cursor_placed:
	while ( true ) {
		// Step 1: Ensure cursor value is non-zero by swapping rows if necessary.
		if ( save_row != row_cursor )
			swapRows( row_cursor, save_row );

		// Step 2: Divide cursor row by cursor entry.
		scaleRow( row_cursor, float(1.0 / getVal(row_cursor,col_cursor)) );

		// Step 3: Zero out cursor column with addRows() method.
		for ( i=0; i<rows; i++ ) {
			if ( i == row_cursor || isZero(getVal(i,col_cursor)) )
				continue;

			addRows( row_cursor, i, float(-1.0 * getVal(i,col_cursor)) );
		}

		// Step 4: Relocate cursor.
		// move cursor down and to the right.
		row_cursor++;
		col_cursor++;

		// we're done if we move past last row or last column.
		if ( row_cursor >= rows || col_cursor >= cols )
			break;

		// set column cursor to the first column with a non-zero entry.
		for ( ; col_cursor<cols; col_cursor++ ) {
			for ( i=row_cursor; i<rows; i++ ) {
				// if entry is non-zero (approximately)
				if ( !isZero( getVal(i,col_cursor) ) ) {
					save_row = i;
					goto cursor_placed2;
				}
			}
		}
		// no more non-zero entries, we're done.
		break;

cursor_placed2:
		;
	}
}

// Places the solution, if exactly one, of a solved system B:x, resulting 
// from Gauss-Jordan elimination, in dst.  Returns the number of solutions 
// to the system.
int simMatrix::getSystemSolution( simpVec& dst )
{
	int i, j;

	// The size of the (or a) solution must be the same as the number of
	// columns - 1 (i.e. the number of variables in the system) of the
	// given simMatrix.
	if ( cols-1 != dst.getSize() )
		throw unmatchedsimpVectorsimMatrixSizes();

	// check for number of solutions
	bool inconsistent_line;
	for ( i=0; i<rows; i++ ) {
		inconsistent_line = true;
		for ( j=0; j<cols-1; j++ ) {
			if ( !isZero( getVal(i,j) ) ) {
				inconsistent_line = false;
				break;
			}
		}

		// No solutions if we've found a line making the system inconsistent.
		if ( inconsistent_line && !isZero( getVal(i,cols-1) ) )
			return NO_SOLUTIONS;
	}

	// We have at least one solution. Check for infinite solutions.
	if ( cols-1 > rows )
		return INFINITE_SOLUTIONS;
	for ( i=0; i<cols-1; i++ )
		if ( isZero( getVal(i,i) ) )
			return INFINITE_SOLUTIONS;

	// We must have only one solution. Copy it to dst.
	for ( i=0; i<dst.getSize(); i++ )
		dst.vals[ i ] = getVal(i,cols-1);
	return ONE_SOLUTION;
}

bool simMatrix::getInvertedsimMatrix( simMatrix& dst )
{
	int i, j;

	// Make sure the matrices are the right size.
	if ( rows != dst.rows )
		throw unmatchedsimMatrixSizes();
	if ( dst.rows != dst.cols || rows * 2 != cols )
		throw nonSquaresimMatrix();

	// Check that the left half of the current simMatrix is the identity.
	for ( i=0; i<rows; i++ )
		for ( j=0; j<rows; j++ )
			if ( (i==j && ((fabs(getVal(i,j) - 1.0)) > 0.000001))||
				 (i!=j && ((fabs(getVal(i,j) - 0.0)) > 0.000001)) )
				 return false;

	// The inverse of the original simMatrix exists.  Return it in dst.
	for ( i=0; i<rows; i++ )
		for ( j=0; j<rows; j++ )
			dst.getVal(i,j) = getVal(i,j+rows);
	return true;
}


/******************************************************************************/
/****************************    public members    ****************************/
/******************************************************************************/

const int simMatrix::NO_SOLUTIONS = 0;
const int simMatrix::ONE_SOLUTION = 1;
const int simMatrix::INFINITE_SOLUTIONS = 2;


// Make a simMatrix the Zero simMatrix
simMatrix& simMatrix::makeZero()
{
	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			getVal(i,j) = 0.0;
	return *this;
}

// Make a simMatrix the Identity simMatrix.  Requires a square simMatrix.
simMatrix& simMatrix::makeIdentity()
{
	if ( rows != cols )
		throw nonSquaresimMatrix();
	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			getVal(i,j) = (i==j) ? 1.0 : 0.0;
	return *this;
}

// Copy assignment.
simMatrix& simMatrix::operator=( const simMatrix& m )
{
	if ( this != &m ) {
		delete [] vals;
		rows = m.rows;
		cols = m.cols;
		vals = new float[ rows * cols ];

		for ( int i=0; i<rows; i++ )
			for ( int j=0; j<cols; j++ )
				vals[ index(i,j) ] = m.vals[ index(i,j) ];
	}
	return *this;
}

simMatrix simMatrix::operator+ ( const simMatrix& m )
{
	if ( rows != m.rows || cols != m.cols )
		throw unmatchedsimMatrixSizes();

	simMatrix r( *this );

	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			r.getVal(i,j) += m.getVal(i,j);
	return r;
}

simMatrix simMatrix::operator- ( const simMatrix& m )
{
	if ( rows != m.rows || cols != m.cols )
		throw unmatchedsimMatrixSizes();

	simMatrix r( *this );

	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			r.getVal(i,j) -= m.getVal(i,j);
	return r;
}

simMatrix simMatrix::operator* ( float s ) const
{
	simMatrix r( *this );

	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			r.getVal(i,j) *= s;
	return r;
}

simMatrix operator* ( float s, const simMatrix& m )
{
	return m * s;
}

simMatrix simMatrix::operator* ( const simMatrix& m )
{
	if ( cols != m.rows )
		throw unmatchedsimMatrixSizes();

	simMatrix r( rows, m.cols );

	for ( int i=0; i<rows; i++ ) {
		for ( int j=0; j<m.cols; j++ ) {
			r.getVal(i,j) = 0;
			for ( int k=0; k<cols; k++ )
				r.getVal(i,j) += getVal(i,k) * m.getVal(k,j);
		}
	}
	return r;
}

// Calculate the inverse of a simMatrix:  dst = Inverse( this )
// Both dst and this must be square matrices.  Returns true if an 
// identity simMatrix exists.
bool simMatrix::inverse( simMatrix& dst )
{
	if ( rows != cols || dst.rows != dst.cols )
		throw nonSquaresimMatrix();
	if ( rows != dst.rows )
		throw unmatchedsimMatrixSizes();

	simMatrix system( rows, cols * 2 );
	dst.makeIdentity();
	appendsimMatrix( dst, system );
	system.gaussJordan();
	return system.getInvertedsimMatrix( dst );
}

// Calculate the transpose of a simMatrix:  dst = Transpose( this ).
// Does not work if this and dst are the same simMatrix.
void simMatrix::transpose( simMatrix& dst )
{
	if ( rows != dst.cols || cols != dst.rows )
		throw unmatchedsimMatrixSizes();
	for ( int i=0; i<rows; i++ )
		for ( int j=0; j<cols; j++ )
			dst.getVal(j,i) = getVal(i,j);
}

// Solve the system Ax = b, where A = this. Return the number of solutions.
int simMatrix::solveSystem( const simpVec& b, simpVec& x )
{
	simMatrix system( rows, cols+1 );
	appendsimpVec( b, system );    // Create simMatrix A:b
	system.gaussJordan();         // Do Gauss-Jordan elimination
	int num_solutions = system.getSystemSolution( x );    // Get the solution

	
	return num_solutions;
}

// Find the least-squares solution of the system Ax = b, where A = this.
// Creates a new system:  AT*Ax = AT*b, where AT = A transpose.
// This system is always consistent, so it must have at least one solution.
int simMatrix::solveLeastSquares( const simpVec& b, simpVec& x )
{
	simMatrix AT( cols, rows );
	simMatrix ATA( cols, cols );
	simpVec ATb( cols );
	simMatrix system( cols, cols+1 );

	// Prepare the system.
	transpose( AT );		// transpose this and place in AT
	ATA = AT * (*this);
	ATb = AT * b;
	ATA.appendsimpVec( ATb, system );	// system = ATA:ATb

	// Solve the system.  Place solution in x.
	system.gaussJordan();
	int num_solutions = system.getSystemSolution( x );

	return num_solutions;
}













// Print out the values for this simMatrix.
void simMatrix::print() const
{
	for ( int i=0; i<rows; i++ ) {
		for ( int j=0; j<cols; j++ )
			LogSystem()->ReportMessage( "  %3.7f", getVal(i,j) );
		LogSystem()->ReportMessage( "\n" );
	}
	LogSystem()->ReportMessage( "\n" );
}


/******************************************************************************/
/******************************************************************************/
/************************                              ************************/
/************************   simMatrix_4x4 class members   ************************/
/************************                              ************************/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/**************************    protected members    ***************************/
/******************************************************************************/

// Multiply two matrices and place the result in dst.
void simMatrix_4x4::multiplyArr( const float *m1, const float *m2, float *dst )
{
    float res[16];

    for ( int i=0; i<4; i++ )
        for ( int j=0; j<4; j++ )
            res[i*4 + j] = m1[i*4 + 0] * m2[0*4 + j] +
                m1[i*4 + 1] * m2[1*4 + j] +
                m1[i*4 + 2] * m2[2*4 + j] +
                m1[i*4 + 3] * m2[3*4 + j];

    copysimMatrix_4x4( res, dst );
}

// Set a simMatrix to the identity simMatrix.
void simMatrix_4x4::makeIdentityArr( float *m )
{
    for ( int i=0; i<4; i++ ) {
        for ( int j=0; j<4; j++ ) {
            if ( i == j )
                m[i*4 + j] = 1;
            else
                m[i*4 + j] = 0;
        }
    }
}

// Set a simMatrix to the zero simMatrix.
void simMatrix_4x4::makeZeroArr( float *m )
{
    for ( int i=0; i<4; i++ )
        for ( int j=0; j<4; j++ )
            m[i*4 + j] = 0.0;
}

// Copy a simMatrix.
void simMatrix_4x4::copysimMatrix_4x4( const float *src, float *dst )
{
    for ( int i=0; i<4; i++ )
        for ( int j=0; j<4; j++ )
            dst[i*4 + j] = src[i*4 + j];
}

// dst = transpose( src )
// This works.  The tmp variable is used in case src and dst
// point to the same simMatrix.
void simMatrix_4x4::transposeArr( const float *src, float *dst )
{
    float tmp;

    for ( int i=0; i<4; i++ )
        dst[i*5] = src[i*5];

    tmp = src[1];
    dst[1] = src[4];
    dst[4] = tmp;

    tmp = src[2];
    dst[2] = src[8];
    dst[8] = tmp;

    tmp = src[3];
    dst[3] = src[12];
    dst[12] = tmp;

    tmp = src[6];
    dst[6] = src[9];
    dst[9] = tmp;

    tmp = src[7];
    dst[7] = src[13];
    dst[13] = tmp;

    tmp = src[11];
    dst[11] = src[14];
    dst[14] = tmp;
}


/******************************************************************************/
/****************************    public members    ****************************/
/******************************************************************************/

// Multiply the current simMatrix on the right by a translation simMatrix.
void simMatrix_4x4::timesTranslation( const simpVec3& v )
{
    makeIdentityArr( mult );
    mult[3*4 + 0] = v.getVal(0);
    mult[3*4 + 1] = v.getVal(1);
    mult[3*4 + 2] = v.getVal(2);
    multiplyArr( vals, mult, vals );
}

// Multiply the current simMatrix on the right by a rotation simMatrix.
// Angle must be in radians.
void simMatrix_4x4::timesRotation( float angle, MAT_AXIS axis )
{
    float cosAngle = (float) cos( (double) angle );
    float sinAngle = (float) sin( (double) angle );

    makeIdentityArr( mult );

    switch( axis ) {
    case MAT_X_AXIS:
        mult[1*4 + 1] =  cosAngle;
        mult[1*4 + 2] =  sinAngle;
        mult[2*4 + 1] = -sinAngle;
        mult[2*4 + 2] =  cosAngle;
        break;
    case MAT_Y_AXIS:
        mult[0*4 + 0] =  cosAngle;
        mult[0*4 + 2] = -sinAngle;
        mult[2*4 + 0] =  sinAngle;
        mult[2*4 + 2] =  cosAngle;
        break;
    case MAT_Z_AXIS:
        mult[0*4 + 0] =  cosAngle;
        mult[0*4 + 1] =  sinAngle;
        mult[1*4 + 0] = -sinAngle;
        mult[1*4 + 1] =  cosAngle;
        break;
    }

    multiplyArr( vals, mult, vals );
}

// Multiply the current simMatrix on the right by a scale simMatrix.
void simMatrix_4x4::timesScale( const simpVec3& v )
{
    makeIdentityArr( mult );
    mult[0*4 + 0] = v.getVal(0);
    mult[1*4 + 1] = v.getVal(1);
    mult[2*4 + 2] = v.getVal(2);
    multiplyArr( vals, mult, vals );
}

// Multiply the current simMatrix on the right by a view simMatrix.
// Create the view simMatrix using the given eye point, center of
// interest, and view up simpVector.
void simMatrix_4x4::timesView( HPoint& eye, HPoint& coi, const simpVec3& vup )
{
    int i;
    simpVec3 u, v, w;

    for ( i=0; i<3; i++ )
        v.getVal(i) = -eye.getVal(i);
    timesTranslation( v );

	w = coi - eye;
	normalize( w );
	v = vup - ((w | vup) * w);
	normalize( v );
	u = v ^ w;

    makeIdentityArr( mult );
    for ( i=0; i<3; i++ ) {
        mult[i*4 + 0] =  u.getVal(i);
        mult[i*4 + 1] =  v.getVal(i);
        mult[i*4 + 2] = -w.getVal(i);
    }
    multiplyArr( vals, mult, vals );

    makeIdentityArr( mult );
    mult[2*4 + 2] = -1;
    multiplyArr( vals, mult, vals );
}

// Multiply the current simMatrix on the right by the inverse view
// simMatrix, i.e. the inverse of the simMatrix created by the
// timesView() method above.  This is done by reversing the
// order of multiplication of the three matrices above, and
// taking their inverses.
// Create the view simMatrix using the given eye point, center of
// interest, and view up simpVector.
void simMatrix_4x4::timesInvView( HPoint& eye, HPoint& coi, const simpVec3& vup )
{
    int i;
    simpVec3 u, v, w;

    // This simMatrix is its own inverse.
    makeIdentityArr( mult );
    mult[2*4 + 2] = -1;
    multiplyArr( vals, mult, vals );

    // This simMatrix is a rotation simMatrix, hence its inverse is
    // its transpose.
	w = coi - eye;
	normalize( w );
	v = vup - ((w | vup) * w);
	normalize( v );
	u = v ^ w;

    makeIdentityArr( mult );
    for ( i=0; i<3; i++ ) {
        mult[i*4 + 0] =  u.getVal(i);
        mult[i*4 + 1] =  v.getVal(i);
        mult[i*4 + 2] = -w.getVal(i);
    }
    // So we transpose the rotation simMatrix here.
    transposeArr( mult, mult );
    multiplyArr( vals, mult, vals );

    // The final simMatrix is inverted by using the positive value
    // of the eye simpVector, rather than the negative, for the
    // translation simMatrix.
    for ( i=0; i<3; i++ )
        v.getVal(i) = eye.getVal(i);
    timesTranslation( v );
}

// Multiply the current simMatrix on the right by a perspective simMatrix.
// Create the perspective projection simMatrix with the given parameters.
void simMatrix_4x4::timesPerspective( float d, float f, float w, float h )
{
    makeZeroArr( mult );
    mult[0*4 + 0] = 2.0/w;
    mult[1*4 + 1] = 2.0/h;
    mult[2*4 + 2] = 1.0/(d * (1.0 - d/f));
    mult[3*4 + 2] = -1.0/(1.0 - d/f);
    mult[2*4 + 3] = 1.0/d;
    multiplyArr( vals, mult, vals );
}

// Transform a point with the current simMatrix.
// This does the multiplication with a row simpVector on the left and
// the simMatrix on the right, rather than the usual way.
void simMatrix_4x4::transformPoint( HPoint& pnt, HPoint& dst ) const
{
    int i;
    HPoint temp;

    for ( i=0; i<4; i++ )
        temp.getVal(i) =
            pnt.getVal(0) * vals[0*4 + i] +
            pnt.getVal(1) * vals[1*4 + i] +
            pnt.getVal(2) * vals[2*4 + i] +
            pnt.getVal(3) * vals[3*4 + i];

    for ( i=0; i<4; i++ )
        dst.getVal(i) = temp.getVal(i);
}

// Transform and normalize a point with the current simMatrix.
void simMatrix_4x4::transformAndNormalizePoint( HPoint& pnt, HPoint& dst ) const
{
    transformPoint( pnt, dst );
    for ( int i=0; i<3; i++ )
        dst.getVal(i) /= dst.getVal(3);
    dst.getVal(3) = 1.0;
}

float* simMatrix_4x4::getOpenGLsimMatrix() const
{
	return vals;
}

// Copy assignment.
simMatrix_4x4& simMatrix_4x4::operator= ( const simMatrix_4x4& m )
{
	if ( this != &m ) {
		for ( int i=0; i<4; i++ )
			for ( int j=0; j<4; j++ )
				vals[ index(i,j) ] = m.vals[ index(i,j) ];
	}
	return *this;
}

simMatrix_4x4 simMatrix_4x4::operator* ( const simMatrix_4x4& m )
{
	simMatrix_4x4 r;

	for ( int i=0; i<4; i++ ) {
		for ( int j=0; j<4; j++ ) {
			r.getVal(i,j) = 0;
			for ( int k=0; k<4; k++ )
				r.getVal(i,j) += getVal(i,k) * m.getVal(k,j);
		}
	}
	return r;
}

HPoint operator* ( const simMatrix_4x4& m, HPoint& p )
{
	HPoint tmp;

	m.transformPoint( p, tmp );
	return tmp;
}
