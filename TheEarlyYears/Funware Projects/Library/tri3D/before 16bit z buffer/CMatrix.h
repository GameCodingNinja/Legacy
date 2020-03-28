
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMatrix3D.h
*
*    DESCRIPTION:     Matrix math class
*
*    IMPLEMENTATION:  For these classes, we are using a class more so
*                     as a container and not so much for following good
*                     object design. In 3D, speed is everything. We need
*                     fast access to all data which is why everything
*                     is made public.
*
*    All the code in this class was originally written by John De Goes.
*    Author of the excellent book "Cutting Edge 3D Game Programming".
*    I'm a math idiot so there's no way possible I would be able to
*    write a class such as this. I made a few changes, nothing major.
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

    // Initializes a specific matrix to the identity matrix
    void InitIdentityMatrix( float mat[16] );
    // Multiply NewMatirx by Matrix; store result in TempMatrix
    void MergeMatrices ( float dest[16], float source[16] );

public:
    float matrix[16];

    // Constructer
    CMatrix();
    // Reset the matrix
    void InitilizeMatrix();
    // Generate 3D rotation matrix
    void Rotate( float x, float y, float z );
    // Create 3D translation matrix
    void Translate( float x, float y, float z );
    // Multiply Source by Dest; store result in Temp
    void MergeMatrix( float newMatrix[16] );
	// Get the object point in space
	void GetObjPoint( CPoint3D &pt3D );

    // Functions designed to transform a vertex using the master matrix
    void Transform( CPoint3D &pt3D );
    void Transform( CPoint3D *pt3D, int count );
	void Transform( CPoint *pt, int count );
	void Transform( CUVI *pUVI );
};
typedef CMatrix *PCMatrix;

#endif