
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMatrix3D.cpp
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "CMatrix.h"         // Header file for this *.cpp file.
#include <math.h>


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMatrix::CMatrix()
{
    // Setup the double for convertion of degrees to radians
    c2r = (long double)PI / (long double)180;

    InitilizeMatrix();
}


/************************************************************************
*    FUNCTION NAME:         InitilizeMatrix()                                                             
*
*    DESCRIPTION:           Reset the matrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::InitilizeMatrix()
{
    InitIdentityMatrix( matrix );

}   // InitilizeMatrix


/************************************************************************
*    FUNCTION NAME:         InitIdentityMatrix()                                                             
*
*    DESCRIPTION:           Initializes a specific matrix to the identity matrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::InitIdentityMatrix( float mat[16] )
{
    // Initializes a specific matrix to the identity matrix:
    mat[0] = 1;   mat[1] = 0;   mat[2] = 0;   mat[3] = 0;
    mat[4] = 0;   mat[5] = 1;   mat[6] = 0;   mat[7] = 0;
    mat[8] = 0;   mat[9] = 0;   mat[10] = 1;  mat[11] = 0;
    mat[12] = 0;  mat[13] = 0;  mat[14] = 0;  mat[15] = 1;

}   // InitIdentityMatrix


/************************************************************************
*    FUNCTION NAME:         MergeMatrix()                                                             
*
*    DESCRIPTION:           Multiply NewMatirx by Matrix; store result in 
*                           TempMatrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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

    // Copy TempMatrix to Matrix:
    for( int i = 0; i < 4; ++i )
    {
        matrix[(i*4)+0] = temp[(i*4)+0];
        matrix[(i*4)+1] = temp[(i*4)+1];
        matrix[(i*4)+2] = temp[(i*4)+2];
        matrix[(i*4)+3] = temp[(i*4)+3];
    }

}  // MergeMatrix


/************************************************************************
*    FUNCTION NAME:         MergeMatrices()                                                             
*
*    DESCRIPTION:           Multiply Source by Dest; store result in Temp
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
    for( int i = 0; i < 4; ++i )
    {
        dest[ (i*4)+0 ] = temp[ (i*4)+0 ];
        dest[ (i*4)+1 ] = temp[ (i*4)+1 ];
        dest[ (i*4)+2 ] = temp[ (i*4)+2 ];
        dest[ (i*4)+3 ] = temp[ (i*4)+3 ];
    }

}   // MergeMatrices


/************************************************************************
*    FUNCTION NAME:         Rotate()                                                             
*
*    DESCRIPTION:           Generate 3D rotation matrix. Rotates the object
*                           at the starting world coordinates.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*    FUNCTION NAME:         Translate()                                                             
*
*    DESCRIPTION:           Create 3D translation matrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*    FUNCTION NAME:         Transform()                                                             
*
*    DESCRIPTION:           Function designed to transform a vertex using
*                           the master matrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::Transform( CPoint3D &pt3D )
{
    // Transform vertex by master matrix:
    pt3D.tx = ( ( pt3D.x * matrix[ 0 ]) )
            + ( ( pt3D.y * matrix[ 4 ]) )
            + ( ( pt3D.z * matrix[ 8 ]) )
            + matrix[ 12 ];

    pt3D.ty = ( ( pt3D.x * matrix[ 1 ]) )
            + ( ( pt3D.y * matrix[ 5 ]) )
            + ( ( pt3D.z * matrix[ 9 ]) )
            + matrix[ 13 ];

    pt3D.tz = ( ( pt3D.x * matrix[ 2 ]) )
            + ( ( pt3D.y * matrix[ 6 ]) )
            + ( ( pt3D.z * matrix[ 10 ]) )
            + matrix[ 14 ];

}   // Transform


/************************************************************************
*    FUNCTION NAME:         Transform()                                                             
*
*    DESCRIPTION:           Function designed to transform a vertex using
*                           the master matrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::Transform( CPoint3D *pt3D, int count )
{
    for( int i = 0; i < count; ++i )
    {
        // Transform vertex by master matrix:
        pt3D[i].tx = ( ( pt3D[i].x * matrix[ 0 ]) )
                   + ( ( pt3D[i].y * matrix[ 4 ]) )
                   + ( ( pt3D[i].z * matrix[ 8 ]) )
                   + matrix[ 12 ];

        pt3D[i].ty = ( ( pt3D[i].x * matrix[ 1 ]) )
                   + ( ( pt3D[i].y * matrix[ 5 ]) )
                   + ( ( pt3D[i].z * matrix[ 9 ]) )
                   + matrix[ 13 ];

        pt3D[i].tz = ( ( pt3D[i].x * matrix[ 2 ]) )
                   + ( ( pt3D[i].y * matrix[ 6 ]) )
                   + ( ( pt3D[i].z * matrix[ 10 ]) )
                   + matrix[ 14 ];
    }

}   // Transform


/************************************************************************
*    FUNCTION NAME:         Transform()                                                             
*
*    DESCRIPTION:           Function designed to transform a vertex using
*                           the master matrix
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::Transform( CPoint *pt, int count )
{
    for( int i = 0; i < count; ++i )
    {
        // Transform vertex by master matrix:
        pt[i].tx = ( ( pt[i].x * matrix[ 0 ]) )
                 + ( ( pt[i].y * matrix[ 4 ]) )
                 + ( ( pt[i].z * matrix[ 8 ]) )
                 + matrix[ 12 ];

        pt[i].ty = ( ( pt[i].x * matrix[ 1 ]) )
                 + ( ( pt[i].y * matrix[ 5 ]) )
                 + ( ( pt[i].z * matrix[ 9 ]) )
                 + matrix[ 13 ];

        pt[i].tz = ( ( pt[i].x * matrix[ 2 ]) )
                 + ( ( pt[i].y * matrix[ 6 ]) )
                 + ( ( pt[i].z * matrix[ 10 ]) )
                 + matrix[ 14 ];
    }

}   // Transform


/************************************************************************
*    FUNCTION NAME:         Transform()                                                             
*
*    DESCRIPTION:           Function designed to transform UV normals
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::Transform( CUVI *pUVI )
{
    for( int i = 0; i < 3; ++i )
    {
        // Transform vertex by master matrix:
        pUVI[i].n.tx = ( ( pUVI[i].n.x * matrix[ 0 ]) )
                     + ( ( pUVI[i].n.y * matrix[ 4 ]) )
                     + ( ( pUVI[i].n.z * matrix[ 8 ]) )
                     + matrix[ 12 ];

        pUVI[i].n.ty = ( ( pUVI[i].n.x * matrix[ 1 ]) )
                     + ( ( pUVI[i].n.y * matrix[ 5 ]) )
                     + ( ( pUVI[i].n.z * matrix[ 9 ]) )
                     + matrix[ 13 ];

        pUVI[i].n.tz = ( ( pUVI[i].n.x * matrix[ 2 ]) )
                     + ( ( pUVI[i].n.y * matrix[ 6 ]) )
                     + ( ( pUVI[i].n.z * matrix[ 10 ]) )
                     + matrix[ 14 ];
    }

}   // Transform


/************************************************************************
*    FUNCTION NAME:         GetObjPoint()                                                             
*
*    DESCRIPTION:           Get object point in space
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMatrix::GetObjPoint( CPoint3D &pt3D )
{
    // Copy the 3D point
    pt3D.x = matrix[ 0 ] + matrix[ 4 ] + matrix[ 8 ]  + matrix[ 12 ];
    pt3D.y = matrix[ 1 ] + matrix[ 5 ] + matrix[ 9 ]  + matrix[ 13 ];
    pt3D.z = matrix[ 2 ] + matrix[ 6 ] + matrix[ 10 ] + matrix[ 14 ];

}   // GetObjPoint