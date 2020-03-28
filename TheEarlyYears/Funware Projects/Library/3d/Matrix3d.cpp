//
// File name: Matrix3D.CPP
//
// Description: The CPP file for the Matrix3D.h header file
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include "Matrix3D.h"


#define COS(a) ( CosTable [a & (DEGREECOUNT - 1)] )
#define SIN(a) ( SinTable [a & (DEGREECOUNT - 1)] )
#define PI   3.141592654

// ------------------------------------------------------------
// | Function section:                                        |
// ------------------------------------------------------------


// Global Cos and Sin Tables. Only one copy of this needs to be in memory
// because the data from these tables is only read and never written to after
// they have bee initilized. So this can be shared by all PolyObj's
float CosTable [ DEGREECOUNT ];
float SinTable [ DEGREECOUNT ];


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

Matrix3D::Matrix3D()
{
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

void Matrix3D::InitilizeMatrix()
{
    InitIdentityMatrix( Matrix );

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

void Matrix3D::InitIdentityMatrix( float Mat[16] )
{
    // Initializes a specific matrix to the identity matrix:
    Mat[0] = 1;   Mat[1] = 0;   Mat[2] = 0;   Mat[3] = 0;
    Mat[4] = 0;   Mat[5] = 1;   Mat[6] = 0;   Mat[7] = 0;
    Mat[8] = 0;   Mat[9] = 0;   Mat[10] = 1;  Mat[11] = 0;
    Mat[12] = 0;  Mat[13] = 0;  Mat[14] = 0;  Mat[15] = 1;

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
   
void Matrix3D::MergeMatrix( float NewMatrix[16] )
{
    float TempMatrix[16];
    int i, j;

    for( i = 0; i < 4; ++i )
    {
        for( j = 0; j < 4; ++j )
        { 
            TempMatrix[(i*4)+j] = (Matrix[i*4] * NewMatrix[j])
            + (Matrix[(i*4)+1] * NewMatrix[4+j])
            + (Matrix[(i*4)+2] * NewMatrix[8+j])
            + (Matrix[(i*4)+3] * NewMatrix[12+j]);
        }
    }

    // Copy TempMatrix to Matrix:
    for( i = 0; i < 4; ++i )
    {
        Matrix[(i*4)+0] = TempMatrix[(i*4)+0];
        Matrix[(i*4)+1] = TempMatrix[(i*4)+1];
        Matrix[(i*4)+2] = TempMatrix[(i*4)+2];
        Matrix[(i*4)+3] = TempMatrix[(i*4)+3];
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

void Matrix3D::MergeMatrices( float Dest[16], float Source[16] )
{
    float Temp[16];
    int i, j;

    for( i = 0; i < 4; ++i )
    {
        for( j = 0; j < 4; ++j )
        {
            Temp[ (i*4)+j ] = ( Source[ i*4 ] * Dest[ j ] )
            + ( Source[ (i*4)+1 ] * Dest[ 4+j ] )
            + ( Source[ (i*4)+2 ] * Dest[ 8+j ] )
            + ( Source[ (i*4)+3 ] * Dest[ 12+j ] );
        }
    }

    // Copy Temp to Dest
    for( i = 0; i < 4; ++i )
    {
        Dest[ (i*4)+0 ] = Temp[ (i*4)+0 ];
        Dest[ (i*4)+1 ] = Temp[ (i*4)+1 ];
        Dest[ (i*4)+2 ] = Temp[ (i*4)+2 ];
        Dest[ (i*4)+3 ] = Temp[ (i*4)+3 ];
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
   
void Matrix3D::Rotate( int Xa, int Ya, int Za )
{
    float temp[16];
	float rMatrix[16];

    InitIdentityMatrix( temp );
	InitIdentityMatrix( rMatrix );

    // Initialize Z rotation matrix - Note: we perform Z
    // rotation first to align the 3D Z axis with the 2D Z axis.
    temp[0]=COS(Za);  temp[1]=SIN(Za);
    temp[4]=-SIN(Za); temp[5]=COS(Za);

    // Merge matrix with master matrix:
    MergeMatrices ( rMatrix, temp );

    // Initialize X rotation matrix:
	InitIdentityMatrix( temp );
    temp[5]=COS(Xa);  temp[6]=SIN(Xa);
    temp[9]=-SIN(Xa); temp[10]=COS(Xa);

    // Merge matrix with master matrix:
    MergeMatrices ( rMatrix, temp );

    // Initialize Y rotation matrix:
	InitIdentityMatrix( temp );
    temp[0]=COS(Ya); temp[2]=-SIN(Ya);       
    temp[8]=SIN(Ya); temp[10]=COS(Ya);      

    // Merge matrix with master matrix:
    MergeMatrices( rMatrix, temp );

    MergeMatrix( rMatrix );

}   // Rotate


/************************************************************************
*    FUNCTION NAME:         RotateAtDest()                                                             
*
*    DESCRIPTION:           Generate 3D rotation matrix. Rotates the object
*                           at the destination coordinates.
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
   
void Matrix3D::RotateAtDest( int Xa, int Ya, int Za )
{
    float Rmat[16];

	InitIdentityMatrix( Rmat );

    // Initialize Z rotation matrix - Note: we perform Z
    // rotation first to align the 3D Z axis with the 2D Z axis.
    Rmat[0]=COS(Za);  Rmat[1]=SIN(Za);
    Rmat[4]=-SIN(Za); Rmat[5]=COS(Za);


    // Merge matrix with master matrix:
    MergeMatrices( Matrix, Rmat );

    // Initialize X rotation matrix:
	InitIdentityMatrix( Rmat );
    Rmat[5]=COS(Xa);  Rmat[6]=SIN(Xa); 
    Rmat[9]=-SIN(Xa); Rmat[10]=COS(Xa);      

    // Merge matrix with master matrix:
    MergeMatrices( Matrix, Rmat );

    // Initialize Y rotation matrix:
	InitIdentityMatrix( Rmat );
    Rmat[0]=COS(Ya); Rmat[2]=-SIN(Ya);
    Rmat[8]=SIN(Ya); Rmat[10]=COS(Ya);

    // Merge matrix with master matrix:
    MergeMatrices( Matrix, Rmat );

}   // RotateAtDest


/************************************************************************
*    FUNCTION NAME:         RotateFromPoint()                                                             
*
*    DESCRIPTION:           Generate 3D rotation matrix. Rotates the object
*                           at the point coordinates.
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
   
void Matrix3D::RotateFromPoint( int Xt, int Yt, int Zt, int Xr, int Yr, int Zr )
{
    float Rmat[16], Tmat[16], RMatrix[16];
    InitIdentityMatrix( RMatrix );
    InitIdentityMatrix( Tmat );
    
    if( Xt )
        Tmat[12]=-Xt;
    else if( Yt )
        Tmat[13]=-Yt;
    else if( Zt )
        Tmat[14]=-Zt;
    
    MergeMatrix( Tmat );
    
    if( Xt )
        Tmat[12]=Xt;
    else if( Yt )
        Tmat[13]=Yt;
    else if( Zt )
        Tmat[14]=Zt;
    
    // Merge matrix with temp matrix:
    MergeMatrices ( RMatrix, Tmat );

    // Initialize Z rotation matrix - Note: we perform Z
    // rotation first to align the 3D Z axis with the 2D Z axis.
    Rmat[0]=COS(Zr);  Rmat[1]=SIN(Zr);  Rmat[2]=0;    Rmat[3]=0;
    Rmat[4]=-SIN(Zr); Rmat[5]=COS(Zr);  Rmat[6]=0;    Rmat[7]=0;
    Rmat[8]=0;        Rmat[9]=0;        Rmat[10]=1;   Rmat[11]=0;
    Rmat[12]=0;       Rmat[13]=0;       Rmat[14]=0;   Rmat[15]=1;

    // Merge matrix with temp matrix:
    MergeMatrices ( RMatrix, Rmat );

    // Initialize X rotation matrix:
    Rmat[0]=1;  Rmat[1]=0;        Rmat[2]=0;        Rmat[3]=0;
    Rmat[4]=0;  Rmat[5]=COS(Xr);  Rmat[6]=SIN(Xr);  Rmat[7]=0;
    Rmat[8]=0;  Rmat[9]=-SIN(Xr); Rmat[10]=COS(Xr); Rmat[11]=0;
    Rmat[12]=0; Rmat[13]=0;       Rmat[14]=0;       Rmat[15]=1;

    // Merge matrix with temp matrix:
    MergeMatrices ( RMatrix, Rmat );

    // Initialize Y rotation matrix:
    Rmat[0]=COS(Yr); Rmat[1]=0;   Rmat[2]=-SIN(Yr); Rmat[3]=0;
    Rmat[4]=0;       Rmat[5]=1;   Rmat[6]=0;        Rmat[7]=0;
    Rmat[8]=SIN(Yr); Rmat[9]=0;   Rmat[10]=COS(Yr); Rmat[11]=0;
    Rmat[12]=0;      Rmat[13]=0;  Rmat[14]=0;       Rmat[15]=1;

    // Merge matrix with temp matrix:
    MergeMatrices( RMatrix, Rmat );

    MergeMatrix( RMatrix );

}   // RotateFromPoint


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
   
void Matrix3D::Translate( float Xt, float Yt, float Zt )
{
    // Declare translation matrix:
    float temp[16];

    // Initialize translation matrix:
	InitIdentityMatrix( temp );
    temp[12]=Xt; temp[13]=Yt; temp[14]=Zt;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Translate


/************************************************************************
*    FUNCTION NAME:         Scale()                                                             
*
*    DESCRIPTION:           Function designed to merge scaling matrix 
*                           with master matrix:
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

void Matrix3D::Scale( float Xs, float Ys, float Zs )
{
    // Create 3D scaling matrix:
    float temp[16];

    // Initialize scaling matrix:
	InitIdentityMatrix( temp );

    temp[0] = Xs; temp[5] = Ys;	temp[10] = Zs;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Scale


/************************************************************************
*    FUNCTION NAME:         Shear()                                                             
*
*    DESCRIPTION:           Create 3D shearing matrix
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

void Matrix3D::Shear( float Xs, float Ys )
{
    float temp[16];

    // Initialize shearing matrix:
	InitIdentityMatrix( temp );

    temp[2] = Xs;
    temp[6] = Ys;

    // Merge matrix with master matrix:
    MergeMatrix( temp );

}   // Shear


/************************************************************************
*    FUNCTION NAME:         GetPoint()                                                             
*
*    DESCRIPTION:           Get the point in space
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

void Matrix3D::GetPoint( int &X, int &Y, int &Z )
{

    // Get the point from the master matrix
    X = Matrix[ 0 ] + Matrix[ 4 ] + Matrix[ 8 ]  + Matrix[ 12 ];

    Y = Matrix[ 1 ] + Matrix[ 5 ] + Matrix[ 9 ]  + Matrix[ 13 ];

    Z = Matrix[ 2 ] + Matrix[ 6 ] + Matrix[ 10 ] + Matrix[ 14 ];

}   // GetPoint


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

void Matrix3D::Transform( Point3D &V )
{
    // Transform vertex by master matrix:
    V.Wx = ( ( V.Lx * Matrix[0]) )
         + ( ( V.Ly * Matrix[4]) )
         + ( ( V.Lz * Matrix[8]) )
         + Matrix[12];

    V.Wy = ( ( V.Lx * Matrix[1]) )
         + ( ( V.Ly * Matrix[5]) )
         + ( ( V.Lz * Matrix[9]) )
         + Matrix[13];

    V.Wz = ( ( V.Lx * Matrix[2]) )
         + ( ( V.Ly * Matrix[6]) )
         + ( ( V.Lz * Matrix[10]) )
         + Matrix[14];

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

void Matrix3D::Transform( Point3D *V, int count )
{
	for( int i = 0; i < count; ++i )
    {
        // Transform vertex by master matrix:
        V[i].Wx = ( ( V[i].Lx * Matrix[0]) )
                + ( ( V[i].Ly * Matrix[4]) )
                + ( ( V[i].Lz * Matrix[8]) )
                + Matrix[12];

        V[i].Wy = ( ( V[i].Lx * Matrix[1]) )
                + ( ( V[i].Ly * Matrix[5]) )
                + ( ( V[i].Lz * Matrix[9]) )
                + Matrix[13];

        V[i].Wz = ( ( V[i].Lx * Matrix[2]) )
                + ( ( V[i].Ly * Matrix[6]) )
                + ( ( V[i].Lz * Matrix[10]) )
                + Matrix[14];
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

void Matrix3D::Transform( Panel3D *P, int count )
{
	for( int i = 0; i < count; ++i )
    {
        // Transform vertex by master matrix:
        P[i].Normal.Tx =  ( ( P[i].Normal.X * Matrix[0]) )
                        + ( ( P[i].Normal.Y * Matrix[4]) )
                        + ( ( P[i].Normal.Z * Matrix[8]) )
                        + Matrix[12];

        P[i].Normal.Ty =  ( ( P[i].Normal.X * Matrix[1]) )
                        + ( ( P[i].Normal.Y * Matrix[5]) )
                        + ( ( P[i].Normal.Z * Matrix[9]) )
                        + Matrix[13];

        P[i].Normal.Tz =  ( ( P[i].Normal.X * Matrix[2]) )
                        + ( ( P[i].Normal.Y * Matrix[6]) )
                        + ( ( P[i].Normal.Z * Matrix[10]) )
                        + Matrix[14];
    }

}   // Transform


/************************************************************************
*    FUNCTION NAME:         Untransform()                                                             
*
*    DESCRIPTION:           Function designed to untransform
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
/*
Point3D &Matrix3D::Untransform( Point3D &V )
{
    // Initialize temporary variables:
    float Wx( V.Wx );
    float Wy( V.Wy );
    float Wz( V.Wz );

    float InvMatrix[ 4 ][ 4 ];
    double Pivot;
    int i, j, k;

    for( i = 0; i < 4; ++i )
    {
        InvMatrix[ i ][ 0 ] = Matrix[ i ][ 0 ];
        InvMatrix[ i ][ 1 ] = Matrix[ i ][ 1 ];
        InvMatrix[ i ][ 2 ] = Matrix[ i ][ 2 ];
        InvMatrix[ i ][ 3 ] = Matrix[ i ][ 3 ];
    }

    for( i = 0; i < 4; ++i )
    {
        Pivot = InvMatrix[ i ] [ i ];
        InvMatrix [ i ] [ i ] = 1.0F;

        for ( j = 0; j < 4; ++j)
            InvMatrix[ i ][ j ] /= Pivot;

        for( k = 0; k < 4; ++k )
        {
            if( k == i )
                continue;

            Pivot = InvMatrix[ k ][ i ];
            InvMatrix[ k ][ i ] = 0.0F;

            for( j = 0; j < 4; ++j )
                InvMatrix[ k ][ j ] -= Pivot * InvMatrix[ i ][ j ];
        }
    }

    // Transform vertex by inverse master matrix:
    V.Lx = ( ( Wx * InvMatrix[ 0 ][ 0 ]) )
         + ( ( Wy * InvMatrix[ 1 ][ 0 ]) )
         + ( ( Wz * InvMatrix[ 2 ][ 0 ]) )
         +          InvMatrix[ 3 ][ 0 ];

    V.Ly = ( ( Wx * InvMatrix[ 0 ][ 1 ]) )
         + ( ( Wy * InvMatrix[ 1 ][ 1 ]) )
         + ( ( Wz * InvMatrix[ 2 ][ 1 ]) )
         +          InvMatrix[ 3 ][ 1 ];

    V.Lz = ( ( Wx * InvMatrix[ 0 ][ 2 ]) )
         + ( ( Wy * InvMatrix[ 1 ][ 2 ]) )
         + ( ( Wz * InvMatrix[ 2 ][ 2 ]) )
         +          InvMatrix[ 3 ][ 2 ];

    return V;

}   // Untransform
*/

/************************************************************************
*    FUNCTION NAME:         Transform()                                                             
*
*    DESCRIPTION:           Function designed to transform a vector using 
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

void Matrix3D::Transform ( Vector &V )
{
    // Transform vertex by master matrix:
    V.Tx = ( ( V.X * Matrix[0] ) )
         + ( ( V.Y * Matrix[4] ) )
         + ( ( V.Z * Matrix[8] ) )
         +           Matrix[12];

    V.Ty = ( ( V.X * Matrix[1] ) )
         + ( ( V.Y * Matrix[5] ) )
         + ( ( V.Z * Matrix[9] ) )
         +           Matrix[13];

    V.Tz = ( ( V.X * Matrix[2] ) )
         + ( ( V.Y * Matrix[6] ) )
         + ( ( V.Z * Matrix[10] ) )
         +           Matrix[14];

}   // Transform


/************************************************************************
*    FUNCTION NAME:         InitCosSinTables()                                                             
*
*    DESCRIPTION:           Initiate math function - calculates trig tables
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

void Matrix3D::InitCosSinTables()
{
    long double Unit = ( long double ) ( PI * 2.0F ) /
                       ( long double ) DEGREECOUNT;

    // Loop through all DEGREECOUNT degrees:
    for( int N = 0; N < DEGREECOUNT; ++N )
    {
        long double Degree = ( long double ) N;
        CosTable [ N ] = float ( cos ( Unit * Degree ) );
        SinTable [ N ] = float ( sin ( Unit * Degree ) );
    }
}   // InitCosSinTables


/************************************************************************
*    FUNCTION NAME:         GetSin()                                                             
*
*    DESCRIPTION:           Get the sin table index
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

float GetSin( int index )
{
    return SIN( index );

}   // GetSin


/************************************************************************
*    FUNCTION NAME:         GetCos()                                                             
*
*    DESCRIPTION:           Get the cos table index
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

float GetCos( int index )
{
    return COS( index );

}   // GetSin