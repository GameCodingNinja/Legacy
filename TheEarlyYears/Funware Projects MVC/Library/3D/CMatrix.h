
/************************************************************************
*
*    FILE NAME:       CMatrix.h
*
*    DESCRIPTION:     4x4 Matrix math class
*
************************************************************************/

#ifndef __CMATRIX_H__
#define __CMATRIX_H__

#include "CPoint.h"

#define PI   3.1415926435898F


// The 3D view matrix class:
class CMatrix
{
protected:
    // Precomputed for cos and sin use.
    // Convert to radians
    long double c2r;
	// Static transform counter
	static int transCounter;

    // Initializes a specific matrix to the identity matrix
    void InitIdentityMatrix( float mat[16] );
    // Merge source matrix into destination matrix
    void MergeMatrices ( float dest[16], float source[16] );

public:
    // Master matrix
    // a single dimension array (array[]) is much faster
    // then a multi-dimension array (array[][])
    float matrix[16];

    // Constructer
    CMatrix();
    // Reset the matrix to the identity matrix
    void InitilizeMatrix();
    // Generate 3D rotation matrix
    void Rotate( float x, float y, float z );
    // Create 3D translation matrix
    void Translate( float x, float y, float z );
    // Merge matrix into master matrix
    void MergeMatrix( float newMatrix[16] );
	void SetMatrix3to4( float newMatrix[9] );
    // Get the matrix point in space
    void GetMatrixPoint( CPoint &pt3D );
    // Function designed to merge scaling matrix with master matrix
    void Scale( float x, float y, float z );
	void Scale( CPoint &scale );
    // Create 3D shearing matrix
    void Shear( float x, float y );
    // Set the matrix to a specific value
    void SetMatrix( float mat[16] );
	// Reset the transform value
	void SetX( float x ){ matrix[12] = x; };
	void SetY( float y ){ matrix[13] = y; };
	void SetZ( float z ){ matrix[14] = z; };
	// Init the counter
	static void InitTransCounter(){ transCounter = 0; };
	// Get the trans count
	static int GetTransCount(){ return transCounter; };

    // Functions designed to transform using the master matrix
    void Transform( CPoint &pt3D );
    void Transform( CPoint *pt3D, int count );
    void Transform( CUVRGB *pUVI, int count );
	void TransformVector( CPoint *pt3D, int count );
	void TransformVector( CPoint &pt3D );
};

typedef CMatrix *PCMatrix;

#endif  // __CMATRIX_H__