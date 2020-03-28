
/************************************************************************
*
*    FILE NAME:       CMatrix.cpp
*
*    DESCRIPTION:     4x4 Matrix math class
*
************************************************************************/

                       // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT         // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "CMatrix.h"   // Header file for this *.cpp file.
#include <math.h>
#include <memory.h>

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// Declare our static counter
int CMatrix::transCounter = 0;

/************************************************************************
*    DESCRIPTION:  Constructer                                                             
************************************************************************/
CMatrix::CMatrix()
{
    // Setup the double for convertion of degrees to radians
    c2r = (long double)PI / (long double)180;

    InitilizeMatrix();

}   // Constructor


/************************************************************************                                                             
*    DESCRIPTION:  Reset the matrix to the identity matrix
************************************************************************/
void CMatrix::InitilizeMatrix()
{
    InitIdentityMatrix( matrix );

}   // InitilizeMatrix


/************************************************************************
*    DESCRIPTION:  Initializes a specific matrix to the identity matrix
*
*    Input:    float mat[16] - Matrix array
************************************************************************/
void CMatrix::InitIdentityMatrix( float mat[16] )
{
    // Initializes a specific matrix to the identity matrix:
	mat[0]  = 1;   mat[1] = 0;   mat[2]  = 0;   mat[3] = 0;
    mat[4]  = 0;   mat[5] = 1;   mat[6]  = 0;   mat[7] = 0;
    mat[8]  = 0;   mat[9] = 0;   mat[10] = 1;  mat[11] = 0;
    mat[12] = 0;  mat[13] = 0;   mat[14] = 0;  mat[15] = 1;

}   // InitIdentityMatrix


/************************************************************************                                                            
*    DESCRIPTION:  Set the matrix to a specific value
*
*    Input:    float mat[16] - Matrix array
************************************************************************/
void CMatrix::SetMatrix( float mat[16] )
{
    memcpy( matrix, mat, sizeof(matrix) );

}   // SetMatrix


/************************************************************************
*    DESCRIPTION:  Merge matrix into master matrix
*
*    Input:    float newMatrix[16] - Matrix array
************************************************************************/  
void CMatrix::MergeMatrix( float newMatrix[16] )
{
    float temp[16];

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        { 
            temp[(i*4)+j] = (matrix[i*4] * newMatrix[j])
            + (matrix[(i*4)+1] * newMatrix[4+j])
            + (matrix[(i*4)+2] * newMatrix[8+j])
            + (matrix[(i*4)+3] * newMatrix[12+j]);
        }
    }

    // Copy temp to master Matrix
	memcpy( matrix, temp, sizeof(temp) );

}  // MergeMatrix


/************************************************************************
*    DESCRIPTION:  Set a 3x3 matrix into a 4x4 matrix
*
*    Input:    float newMatrix[9] - Matrix array
************************************************************************/
void CMatrix::SetMatrix3to4( float newMatrix[9] )
{
    InitilizeMatrix();

	matrix[0] = newMatrix[0];
	matrix[1] = newMatrix[1];
	matrix[2] = newMatrix[2];

    matrix[4] = newMatrix[3];
	matrix[5] = newMatrix[4];
	matrix[6] = newMatrix[5];

    matrix[8]  = newMatrix[6];
	matrix[9]  = newMatrix[7];
	matrix[10] = newMatrix[8];

}  // SetMatrix3to4


/************************************************************************
*    DESCRIPTION:  Merge source matrix into destination matrix.
*
*    Input:    float Dest[16] - Destination Matric
*              float Source[16] - Source Matrix
************************************************************************/
void CMatrix::MergeMatrices( float dest[16], float source[16] )
{
    float temp[16];

    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            temp[ (i*4)+j ] = ( source[ i*4 ] * dest[ j ] )
            + ( source[ (i*4)+1 ] * dest[ 4+j ] )
            + ( source[ (i*4)+2 ] * dest[ 8+j ] )
            + ( source[ (i*4)+3 ] * dest[ 12+j ] );
        }
    }

    // Copy Temp to Dest
	memcpy( dest, temp, sizeof(temp) );

}   // MergeMatrices


/************************************************************************
*    DESCRIPTION:  Generate 3D rotation matrix.
*
*    Input:    float x - X rotation
*              float y - Y rotation
*              float z - Z rotation
************************************************************************/  
void CMatrix::Rotate( float x, float y, float z )
{
    float temp[ 16 ];
    float rMatrix[ 16 ];

    InitIdentityMatrix( rMatrix );
    
    // Get the sin and cos for each axis
    float cosX( cos(x * c2r) );
    float sinX( sin(x * c2r) );

    float cosY( cos(y * c2r) );
    float sinY( sin(y * c2r) );

    float cosZ( cos(z * c2r) );
    float sinZ( sin(z * c2r) );

    // Initialize Z rotation matrix - Note: we perform Z
    // rotation first to align the 3D Z axis with the 2D Z axis.
    InitIdentityMatrix( temp );
    temp[0]=cosZ;
    temp[1]=sinZ;
    temp[4]=-sinZ;
    temp[5]=cosZ;

    // Merge matrix with rotation matrix
    MergeMatrices( rMatrix, temp );

    // Initialize X rotation matrix:
    InitIdentityMatrix( temp );
    temp[5]=cosX;
    temp[6]=sinX;
    temp[9]=-sinX;
    temp[10]=cosX;

    // Merge matrix with rotation matrix
    MergeMatrices ( rMatrix, temp );

    // Initialize Y rotation matrix:
    InitIdentityMatrix( temp );
    temp[0]=cosY;
    temp[2]=-sinY;
    temp[8]=sinY;
    temp[10]=cosY;

    // Merge matrix with rotation matrix
    MergeMatrices( rMatrix, temp );

    // Merg the rotation into the master matrix
    MergeMatrix( rMatrix );

}   // Rotate


/************************************************************************
*    DESCRIPTION:  Create 3D translation matrix
*
*    Input:    float x - X movement
*              float y - Y movement
*              float z - Z movement
************************************************************************/
void CMatrix::Translate( float x, float y, float z )
{
    float temp[ 16 ];

    // Initialize translation matrix
    InitIdentityMatrix( temp );

    temp[12]=x;
    temp[13]=y;
    temp[14]=z;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Translate


/************************************************************************
*    DESCRIPTION:  Function designed to transform a vertex using
*                  the master matrix
*
*    Input:    CPoint &pt3D - 3D point
************************************************************************/
void CMatrix::Transform( CPoint &pt3D )
{
    // Transform vertex by master matrix:
    pt3D.tx = ( pt3D.x * matrix[ 0 ] )
            + ( pt3D.y * matrix[ 4 ] )
            + ( pt3D.z * matrix[ 8 ] )
            + matrix[ 12 ];

    pt3D.ty = ( pt3D.x * matrix[ 1 ] )
            + ( pt3D.y * matrix[ 5 ] )
            + ( pt3D.z * matrix[ 9 ] )
            + matrix[ 13 ];

    pt3D.tz = ( pt3D.x * matrix[ 2 ] )
            + ( pt3D.y * matrix[ 6 ] )
            + ( pt3D.z * matrix[ 10 ] )
            + matrix[ 14 ];

    // Inc our static counter
    CMatrix::transCounter++;

}   // Transform


/************************************************************************
*    DESCRIPTION:  Function designed to transform a vertex using
*                  the master matrix
*
*    Input:    CPoint &pt3D - 3D point
************************************************************************/
void CMatrix::TransformVector( CPoint &pt3D )
{
    // Transform vertex by master matrix:
    pt3D.tx = ( pt3D.x * matrix[ 0 ])
            + ( pt3D.y * matrix[ 4 ])
            + ( pt3D.z * matrix[ 8 ]);

    pt3D.ty = ( pt3D.x * matrix[ 1 ])
            + ( pt3D.y * matrix[ 5 ])
            + ( pt3D.z * matrix[ 9 ]);

    pt3D.tz = ( pt3D.x * matrix[ 2 ])
            + ( pt3D.y * matrix[ 6 ])
            + ( pt3D.z * matrix[ 10 ]);

    // Inc our static counter
    CMatrix::transCounter++;

}   // Transform


/************************************************************************
*    DESCRIPTION:   Function designed to transform a vertex using
*                   the master matrix
*
*    Input:    CPoint &pt3D - 3D point
*              int count - number of points
************************************************************************/
void CMatrix::Transform( CPoint *pt3D, int count )
{
    for( int i = 0; i < count; ++i )
    {        
        // Transform vertex by master matrix:
        pt3D[i].tx = ( pt3D[i].x * matrix[ 0 ] )
                   + ( pt3D[i].y * matrix[ 4 ] )
                   + ( pt3D[i].z * matrix[ 8 ] )
                   + matrix[ 12 ];

        pt3D[i].ty = ( pt3D[i].x * matrix[ 1 ] )
                   + ( pt3D[i].y * matrix[ 5 ] )
                   + ( pt3D[i].z * matrix[ 9 ] )
                   + matrix[ 13 ];

        pt3D[i].tz = ( pt3D[i].x * matrix[ 2 ] )
                   + ( pt3D[i].y * matrix[ 6 ] )
                   + ( pt3D[i].z * matrix[ 10 ] )
                   + matrix[ 14 ];
    }

    // Inc our static counter
    CMatrix::transCounter += count;

}   // Transform


/************************************************************************
*    DESCRIPTION:   Function designed to transform a vertex using
*                   the master matrix
*
*    Input:    CPoint &pt3D - 3D point
*              int count - number of points
************************************************************************/
void CMatrix::TransformVector( CPoint *pt3D, int count )
{
    for( int i = 0; i < count; ++i )
    {        
        // Transform vertex by master matrix:
        pt3D[i].tx = ( pt3D[i].x * matrix[ 0 ])
                   + ( pt3D[i].y * matrix[ 4 ])
                   + ( pt3D[i].z * matrix[ 8 ]);

        pt3D[i].ty = ( pt3D[i].x * matrix[ 1 ])
                   + ( pt3D[i].y * matrix[ 5 ])
                   + ( pt3D[i].z * matrix[ 9 ]);

        pt3D[i].tz = ( pt3D[i].x * matrix[ 2 ])
                   + ( pt3D[i].y * matrix[ 6 ])
                   + ( pt3D[i].z * matrix[ 10 ]);
    }

    // Inc our static counter
    CMatrix::transCounter += count;

}   // Transform


/************************************************************************
*    DESCRIPTION:  Function designed to transform vertex normals
*
*    Input:    CUVRGB *pUVRGB - Normals for each vertex
************************************************************************/
void CMatrix::Transform( CUVRGB *pUVRGB, int count )
{
    for( int i = 0; i < count; ++i )
    {
        // Transform vertex by master matrix:
        pUVRGB[i].n.tx = ( pUVRGB[i].n.x * matrix[ 0 ])
                       + ( pUVRGB[i].n.y * matrix[ 4 ])
                       + ( pUVRGB[i].n.z * matrix[ 8 ]);

        pUVRGB[i].n.ty = ( pUVRGB[i].n.x * matrix[ 1 ])
                       + ( pUVRGB[i].n.y * matrix[ 5 ])
                       + ( pUVRGB[i].n.z * matrix[ 9 ]);

        pUVRGB[i].n.tz = ( pUVRGB[i].n.x * matrix[ 2 ])
                       + ( pUVRGB[i].n.y * matrix[ 6 ])
                       + ( pUVRGB[i].n.z * matrix[ 10 ]);
    }

	// Inc our static counter
    CMatrix::transCounter += count;

}   // Transform


/************************************************************************
*    DESCRIPTION:   Get matrix point in space
*
*    Input:    CPoint &pt3D - Object to be set
************************************************************************/
void CMatrix::GetMatrixPoint( CPoint &pt3D )
{
    // Copy the 3D point
    pt3D.x = matrix[ 0 ] + matrix[ 4 ] + matrix[ 8 ]  + matrix[ 12 ];
    pt3D.y = matrix[ 1 ] + matrix[ 5 ] + matrix[ 9 ]  + matrix[ 13 ];
    pt3D.z = matrix[ 2 ] + matrix[ 6 ] + matrix[ 10 ] + matrix[ 14 ];

}   // GetObjPoint


/************************************************************************
*    DESCRIPTION:   Function designed to merge scaling matrix 
*                   with master matrix
*
*    Input:    float x - X scale factor
*              float y - Y scale factor
*              float z - Z scale factor
************************************************************************/   
void CMatrix::Scale( float x, float y, float z )
{
    float temp[ 16 ];

    // Initialize translation matrix
    InitIdentityMatrix( temp );

    // Initialize scaling matrix:
    temp[0]=x;
    temp[5]=y;
    temp[10]=z;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Scale


/************************************************************************
*    DESCRIPTION:   Function designed to merge scaling matrix 
*                   with master matrix
*
*    Input:    CPoint &pt3D - scale point
************************************************************************/   
void CMatrix::Scale( CPoint &scale )
{
    float temp[ 16 ];

    // Initialize translation matrix
    InitIdentityMatrix( temp );

    // Initialize scaling matrix:
    temp[0]=scale.x;
    temp[5]=scale.y;
    temp[10]=scale.z;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Scale


/************************************************************************
*    DESCRIPTION:  Create 3D shearing matrix
*
*    Input:    float x - X shear factor
*              float y - Y shear factor
************************************************************************/
void CMatrix::Shear( float x, float y )
{
    float temp[ 16 ];

    // Initialize translation matrix
    InitIdentityMatrix( temp );

    // Initialize shearing matrix:
    temp[2]=x;
    temp[6]=y;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Shear