//
// File name: Monster.HPP
//
// Description: A monster object
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef MONSTERHPP
#define MONSTERHPP

#include "Point3D.HPP"
#include "Matrix3D.HPP"
#include "3DClass.HPP"
#include "BMP32.HPP"

class Monster {
protected:
  BMPImage *Frame;
  int FrameNum, FrameCount, Width, Height, Health;
  Point3D Pos, Dir;
public:
  enum { ASCII, Binary };
  Monster ( float Sx, float Sy, float Sz, char *IniFile,
            int Mode = ASCII );
  ~Monster () { delete [] Frame; }
  Monster &operator = ( int FN )
     {
     if ( ( FN >= 0 ) && ( FN < FrameCount ) )
        FrameNum = FN;
     return *this;
     }
  int GetHealth () { return Health; }
  int SetHealth () { return Health; }

  BOOL Show ( Matrix3D &M, BYTE *Dest );
  float GetX () { return Pos.Wx; }
  float GetY () { return Pos.Wy; }
  float GetZ () { return Pos.Wz; }
  float Dist ()
      {
      float Mag = sqrt ( Pos.Wx * Pos.Wx +
                         Pos.Wy * Pos.Wy +
                         Pos.Wz * Pos.Wz );
      return Mag;
      }
  void MoveToward ( float Tx, float Ty, float Tz, float Steps,
                    Matrix3D &M );
};

#endif