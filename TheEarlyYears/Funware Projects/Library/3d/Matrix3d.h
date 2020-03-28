//
// File name: Matrix3D.h
//
// Description: The header file a 3D "view" matrix
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef MATRIX3DHPP
#define MATRIX3DHPP

#include "Panel3D.h"
#include "Point3D.h"
#include "Vector3D.h"

// Do we want to use a larger table for finer degree of rotation control?
#ifdef	REDEFINE_DEGREECOUNT
	#define DEGREECOUNT  REDEFINE_DEGREECOUNT
#else
	#define DEGREECOUNT  1024	// Default table size
#endif


// The 3D view matrix class:
class Matrix3D
{
protected:
    // Initializes a specific matrix to the identity matrix
    void InitIdentityMatrix( float Mat[16] );
    // Multiply NewMatirx by Matrix; store result in TempMatrix
    void MergeMatrices ( float Dest[16], float Source[16] );

public:
    float Matrix[16];

    // Constructer
    Matrix3D();
    // Reset the matrix
    void InitilizeMatrix();
    // Generate 3D rotation matrix
    void Rotate ( int Xa, int Ya, int Za );
    // Generate 3D rotation matrix
    void RotateAtDest ( int Xa, int Ya, int Za );
    // Rotates the object at the point coordinates
    void RotateFromPoint( int Xt, int Yt, int Zt, int Xr, int Yr, int Zr );
    // Create 3D translation matrix
    void Translate ( float Xt, float Yt, float Zt );
    // Function designed to merge scaling matrix with master matrix:
    void Scale ( float Xs, float Ys, float Zs );
    // Create 3D shearing matrix
    void Shear ( float Xs, float Ys );    
    // Multiply Source by Dest; store result in Temp
    void MergeMatrix( float NewMatrix[16] );
    // Get the point in space
    void GetPoint( int &X, int &Y, int &Z );
    // Initilize the global cos and sin tables
    void InitCosSinTables();
    // Get the individual point
    float GetX(){ return Matrix[ 0 ] + Matrix[ 4 ] + Matrix[ 8 ]  + Matrix[ 12 ]; };
    float GetY(){ return Matrix[ 1 ] + Matrix[ 5 ] + Matrix[ 9 ]  + Matrix[ 13 ]; };
    float GetZ(){ return Matrix[ 2 ] + Matrix[ 6 ] + Matrix[ 10 ] + Matrix[ 14 ]; };
    void Transform( Point3D *V, int count );
    void Transform( Panel3D *P, int count );
	void Transform( Vector &V );
	void Transform( Point3D &V );
};
typedef Matrix3D *PMatrix3D;

// External sin and cos table functions
float GetSin( int index );
float GetCos( int index );

#endif