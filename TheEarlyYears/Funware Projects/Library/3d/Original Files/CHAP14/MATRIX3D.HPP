//
// File name: Matrix3D.HPP
//
// Description: The header file a 3D "view" matrix
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef MATRIX3DHPP
#define MATRIX3DHPP

#include "Point3D.HPP"
#include "Vector3D.HPP"
#include "SinCos.HPP"

void InitMath ();

// The 3D view matrix class:
class Matrix3D {
protected:
  void InitMat ( float Mat [ 4 ] [ 4 ] );
  void MergeMatrix ( float NewMatrix [ 4 ] [ 4 ] );
  void MergeMatrices ( float Dest [ 4 ] [ 4 ], float Source [ 4 ] [ 4 ] );
  float Matrix [ 4 ] [ 4 ];
  float RMatrix [ 4 ] [ 4 ];
  int Xr, Zr, Yr;
  float XTrans, YTrans, ZTrans;
public:
  Matrix3D ()
     {
     InitMath ();
     Zr = Yr = Zr = 0;
     XTrans = YTrans = ZTrans = 0.0F;
     Initialize ();
     }
  void Rotate ( int Xa, int Ya, int Za );
  void Translate ( float Xt, float Yt, float Zt );
  void Scale ( float Xs, float Ys, float Zs );
  void Shear ( float Xs, float Ys );
  void Initialize ();
  Point3D &Transform ( Point3D &V );
  Point3D &Untransform ( Point3D &V );
  Vector inline &Transform ( Vector &V );
  float GetXt () { return XTrans; }
  float GetYt () { return YTrans; }
  float GetZt () { return ZTrans; }
};

#endif