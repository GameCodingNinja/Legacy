//
// File name: Matrix3D.CPP
//
// Description: The CPP file for the Matrix3D.HPP header file
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include "Matrix3D.HPP"

// Function designed to set matrix to identity matrix:
void Matrix3D::Initialize ()
   {
   Matrix[0][0] = 1;  Matrix[0][1] = 0;  Matrix[0][2] = 0;  Matrix[0][3] = 0;
   Matrix[1][0] = 0;  Matrix[1][1] = 1;  Matrix[1][2] = 0;  Matrix[1][3] = 0;
   Matrix[2][0] = 0;  Matrix[2][1] = 0;  Matrix[2][2] = 1;  Matrix[2][3] = 0;
   Matrix[3][0] = 0;  Matrix[3][1] = 0;  Matrix[3][2] = 0;  Matrix[3][3] = 1;
   }

void Matrix3D::InitMat ( float Mat [ 4 ] [ 4 ] )
   {
   // Initializes a specific matrix to the identity matrix:
   Mat [0][0] = 1;  Mat [0][1] = 0;  Mat [0][2] = 0;  Mat [0][3] = 0;
   Mat [1][0] = 0;  Mat [1][1] = 1;  Mat [1][2] = 0;  Mat [1][3] = 0;
   Mat [2][0] = 0;  Mat [2][1] = 0;  Mat [2][2] = 1;  Mat [2][3] = 0;
   Mat [3][0] = 0;  Mat [3][1] = 0;  Mat [3][2] = 0;  Mat [3][3] = 1;
   }
   
void Matrix3D::MergeMatrix ( float NewMatrix [ 4 ] [ 4 ] )
   {
   // Multiply NewMatirx by Matrix; store result in TempMatrix
   float TempMatrix [ 4 ] [ 4 ];
	for (short unsigned int i = 0; i < 4; i++)
		 for (short unsigned int j = 0; j < 4; j++) 
			  TempMatrix[i][j] = (Matrix[i][0] * NewMatrix[0][j])
                            + (Matrix[i][1] * NewMatrix[1][j])
                            + (Matrix[i][2] * NewMatrix[2][j])
                            + (Matrix[i][3] * NewMatrix[3][j]);
   // Copy TempMatrix to Matrix:
   for (int i = 0; i < 4; i++)
       {
       Matrix[i][0] = TempMatrix[i][0];
       Matrix[i][1] = TempMatrix[i][1];
       Matrix[i][2] = TempMatrix[i][2];
       Matrix[i][3] = TempMatrix[i][3];
       }
   }

void Matrix3D::MergeMatrices ( float Dest [ 4 ] [ 4 ], float Source [ 4 ] [ 4 ] )
   {
   // Multiply Source by Dest; store result in Temp:
   float Temp [ 4 ] [ 4 ];
	for ( short unsigned int i = 0; i < 4; i++ )
		 for ( short unsigned int j = 0; j < 4; j++ )
           {
			  Temp [ i ] [ j ] = ( Source [ i ] [ 0 ] * Dest [ 0 ] [ j ] )
                            + ( Source [ i ] [ 1 ] * Dest [ 1 ] [ j ] )
                            + ( Source [ i ] [ 2 ] * Dest [ 2 ] [ j ] )
                            + ( Source [ i ] [ 3 ] * Dest [ 3 ] [ j ] );
           }
   // Copy Temp to Dest:
   for (int i = 0; i < 4; i++)
       {
       Dest [ i ] [ 0 ] = Temp [ i ] [ 0 ];
       Dest [ i ] [ 1 ] = Temp [ i ] [ 1 ];
       Dest [ i ] [ 2 ] = Temp [ i ] [ 2 ];
       Dest [ i ] [ 3 ] = Temp [ i ] [ 3 ];
       }
   }
   
void  Matrix3D::Rotate ( int Xa, int Ya, int Za )
   {
   // Generate 3D rotation matrix:
   Xr = Xa; Yr = Ya; Zr = Za;
   float Rmat [ 4 ] [ 4 ];
   InitMat ( RMatrix );

   // Initialize Z rotation matrix - Note: we perform Z
   // rotation first to align the 3D Z axis with the 2D Z axis.
   Rmat[0][0]=COS(Za);  Rmat[0][1]=SIN(Za);  Rmat[0][2]=0;    Rmat[0][3]=0;
   Rmat[1][0]=-SIN(Za); Rmat[1][1]=COS(Za);  Rmat[1][2]=0;    Rmat[1][3]=0;
   Rmat[2][0]=0;        Rmat[2][1]=0;        Rmat[2][2]=1;    Rmat[2][3]=0;
   Rmat[3][0]=0;        Rmat[3][1]=0;        Rmat[3][2]=0;    Rmat[3][3]=1;

   // Merge matrix with master matrix:
   MergeMatrices ( RMatrix, Rmat );

   // Initialize X rotation matrix:
   Rmat[0][0]=1;  Rmat[0][1]=0;        Rmat[0][2]=0;       Rmat[0][3]=0;
   Rmat[1][0]=0;  Rmat[1][1]=COS(Xa);  Rmat[1][2]=SIN(Xa); Rmat[1][3]=0;
   Rmat[2][0]=0;  Rmat[2][1]=-SIN(Xa); Rmat[2][2]=COS(Xa); Rmat[2][3]=0;
   Rmat[3][0]=0;  Rmat[3][1]=0;        Rmat[3][2]=0;       Rmat[3][3]=1;

   // Merge matrix with master matrix:
   MergeMatrices ( RMatrix, Rmat );

   // Initialize Y rotation matrix:
   Rmat[0][0]=COS(Ya); Rmat[0][1]=0;   Rmat[0][2]=-SIN(Ya); Rmat[0][3]=0;
   Rmat[1][0]=0;       Rmat[1][1]=1;   Rmat[1][2]=0;        Rmat[1][3]=0;
   Rmat[2][0]=SIN(Ya); Rmat[2][1]=0;   Rmat[2][2]=COS(Ya);  Rmat[2][3]=0;
   Rmat[3][0]=0;       Rmat[3][1]=0;   Rmat[3][2]=0;        Rmat[3][3]=1;

   // Merge matrix with master matrix:
   MergeMatrices ( RMatrix, Rmat );

   MergeMatrix ( RMatrix );
   }
   
void Matrix3D::Translate ( float Xt, float Yt, float Zt )
   {
   // Create 3D translation matrix:

   // Declare translation matrix:
   float Tmat [ 4 ] [ 4 ];
   
   // Save translation values:
   XTrans = Xt; YTrans = Yt; ZTrans = Zt;
   
   // Initialize translation matrix:
   Tmat[0][0]=1;  Tmat[0][1]=0;  Tmat[0][2]=0;  Tmat[0][3]=0;
   Tmat[1][0]=0;  Tmat[1][1]=1;  Tmat[1][2]=0;  Tmat[1][3]=0;
   Tmat[2][0]=0;  Tmat[2][1]=0;  Tmat[2][2]=1;  Tmat[2][3]=0;
   Tmat[3][0]=Xt; Tmat[3][1]=Yt; Tmat[3][2]=Zt; Tmat[3][3]=1;

   // Merge matrix with master matrix:
   MergeMatrix ( Tmat );
   }
   
// Function designed to merge scaling matrix with master
// matrix:
void  Matrix3D::Scale ( float Xs, float Ys, float Zs )
   {
   // Create 3D scaling matrix:
   float Smat [ 4 ] [ 4 ];

   // Initialize scaling matrix:
   Smat[0][0] = Xs; Smat[0][1] = 0;  Smat[0][2] = 0;  Smat[0][3] = 0;
   Smat[1][0] = 0;  Smat[1][1] = Ys; Smat[1][2] = 0;  Smat[1][3] = 0;
   Smat[2][0] = 0;  Smat[2][1] = 0;  Smat[2][2] = Zs; Smat[2][3] = 0;
   Smat[3][0] = 0;  Smat[3][1] = 0;  Smat[3][2] = 0;  Smat[3][3] = 1;

   // Merge matrix with master matrix:
   MergeMatrix ( Smat );
   }

void  Matrix3D::Shear ( float Xs, float Ys )
   {
   // Create 3D shearing matrix:

   float Smat [ 4 ] [ 4 ];

   // Initialize shearing matrix:
   Smat[0][0] = 1;  Smat[0][1] = 0;  Smat[0][2] = Xs;  Smat[0][3] = 0;
   Smat[1][0] = 0;  Smat[1][1] = 1;  Smat[1][2] = Ys;  Smat[1][3] = 0;
   Smat[2][0] = 0;  Smat[2][1] = 0;  Smat[2][2] = 1;   Smat[2][3] = 0;
   Smat[3][0] = 0;  Smat[3][1] = 0;  Smat[3][2] = 0;   Smat[3][3] = 1;

   // Merge matrix with master matrix:
   MergeMatrix ( Smat );
   }

// Function designed to transform a vertex using the master
// matrix:   
Point3D &Matrix3D::Transform ( Point3D &V )
   {
   // Initialize temporary variables:
   float Lx = V.Lx;
   float Ly = V.Ly;
   float Lz = V.Lz;

   // Transform vertex by master matrix:
   V.Wx = ( (   Lx * Matrix [ 0 ][ 0 ]) )
          + ( ( Ly * Matrix [ 1 ][ 0 ]) )
          + ( ( Lz * Matrix [ 2 ][ 0 ]) )
          + Matrix [ 3 ][ 0 ];

   V.Wy = (   ( Lx * Matrix [ 0 ][ 1 ]) )
          + ( ( Ly * Matrix [ 1 ][ 1 ]) )
          + ( ( Lz * Matrix [ 2 ][ 1 ]) )
          + Matrix [ 3 ][ 1 ];

   V.Wz = (   ( Lx * Matrix [ 0 ][ 2 ]) )
          + ( ( Ly * Matrix [ 1 ][ 2 ]) )
          + ( ( Lz * Matrix [ 2 ][ 2 ]) )
          + Matrix [ 3 ][ 2 ];
   return V;
   }

Point3D &Matrix3D::Untransform ( Point3D &V )
   {
   // Initialize temporary variables:
   float Wx = V.Wx;
   float Wy = V.Wy;
   float Wz = V.Wz;
   float InvMatrix [ 4 ] [ 4 ];
   double Pivot;
	int i, j, k;

   for ( i = 0; i < 4; i++ )
       {
       InvMatrix [ i ] [ 0 ] = Matrix [ i ] [ 0 ];
       InvMatrix [ i ] [ 1 ] = Matrix [ i ] [ 1 ];
       InvMatrix [ i ] [ 2 ] = Matrix [ i ] [ 2 ];
       InvMatrix [ i ] [ 3 ] = Matrix [ i ] [ 3 ];
       }

   for ( i = 0; i < 4; i++ )
       {
       Pivot = InvMatrix [ i ] [ i ];
       InvMatrix [ i ] [ i ] = 1.0F;
       for ( j = 0; j < 4; j++)
           InvMatrix [ i ] [ j ] /= Pivot;
       for ( k = 0; k < 4; k++)
           {
           if ( k == i )
              continue;
           Pivot = InvMatrix [ k ] [ i ];
           InvMatrix [ k ] [ i ] = 0.0F;
           for ( j = 0; j < 4; j++ )
               InvMatrix [ k ] [ j ] -= Pivot * InvMatrix [ i ] [ j ];
           }
       }

   // Transform vertex by inverse master matrix:
   V.Lx = ( (   Wx * InvMatrix [ 0 ][ 0 ]) )
          + ( ( Wy * InvMatrix [ 1 ][ 0 ]) )
          + ( ( Wz * InvMatrix [ 2 ][ 0 ]) )
          +          InvMatrix [ 3 ][ 0 ];

   V.Ly = (   ( Wx * InvMatrix [ 0 ][ 1 ]) )
          + ( ( Wy * InvMatrix [ 1 ][ 1 ]) )
          + ( ( Wz * InvMatrix [ 2 ][ 1 ]) )
          +          InvMatrix [ 3 ][ 1 ];

   V.Lz = (   ( Wx * InvMatrix [ 0 ][ 2 ]) )
          + ( ( Wy * InvMatrix [ 1 ][ 2 ]) )
          + ( ( Wz * InvMatrix [ 2 ][ 2 ]) )
          +          InvMatrix [ 3 ][ 2 ];
   return V;
   }

// Function designed to transform a vector using the master
// matrix:
Vector &Matrix3D::Transform ( Vector &V )
   {
   // Initialize temporary variables:
   float OldX = V.X;
   float OldY = V.Y;
   float OldZ = V.Z;

   // Transform vertex by master matrix:
   V.Tx = ( ( OldX * Matrix [ 0 ] [ 0 ] ) )
        + ( ( OldY * Matrix [ 1 ] [ 0 ] ) )
        + ( ( OldZ * Matrix [ 2 ] [ 0 ] ) )
        +            Matrix [ 3 ] [ 0 ];

   V.Ty = ( ( OldX * Matrix [ 0 ] [ 1 ] ) )
        + ( ( OldY * Matrix [ 1 ] [ 1 ] ) )
        + ( ( OldZ * Matrix [ 2 ] [ 1 ] ) )
        +            Matrix [ 3 ] [ 1 ];

   V.Tz = ( ( OldX * Matrix [ 0 ] [ 2 ] ) )
        + ( ( OldY * Matrix [ 1 ] [ 2 ] ) )
        + ( ( OldZ * Matrix [ 2 ] [ 2 ] ) )
        +            Matrix [ 3 ] [ 2 ];
   return V;
   }
