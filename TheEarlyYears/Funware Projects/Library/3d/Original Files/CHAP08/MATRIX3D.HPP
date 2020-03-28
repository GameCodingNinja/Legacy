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
  void InitMat ( double Mat [ 4 ] [ 4 ] );
  void MergeMatrix ( double NewMatrix [ 4 ] [ 4 ] );
  void MergeMatrices ( double Dest [ 4 ] [ 4 ], double Source [ 4 ] [ 4 ] );
  double Matrix [ 4 ] [ 4 ];
  double RMatrix [ 4 ] [ 4 ];
  int Xr, Zr, Yr;
  double XTrans, YTrans, ZTrans;
public:
  Matrix3D ()
     {
     InitMath ();
     Zr = Yr = Zr = 0;
     XTrans = YTrans = ZTrans = 0.0F;
     }
  void Rotate ( int Xa, int Ya, int Za );
  void Translate ( double Xt, double Yt, double Zt );
  void Scale ( double Xs, double Ys, double Zs );
  void Shear ( double Xs, double Ys );
  void Initialize ();
  Point3D inline &Transform ( Point3D &V );
  Vector inline &Transform ( Vector &V );
  double GetXt () { return XTrans; }
  double GetYt () { return YTrans; }
  double GetZt () { return ZTrans; }
};

#endif