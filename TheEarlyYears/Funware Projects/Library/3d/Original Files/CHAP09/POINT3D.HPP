//
// File name: Point3D.HPP
//
// Description: The header file a 3D point class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef POINT3DHPP
#define POINT3DHPP

#include <Math.H>
#include <Stdio.H>

// The 3-dimensional point class:
class Point3D {
public:
  float Lx, Ly, Lz;  // The local X, Y and Z
  float Wx, Wy, Wz;  // The world X, Y and Z
  Point3D () { Lx = Ly = Lz = 0.0F; }
  // The equality operator:
  inline int operator == ( Point3D &V );
  // The inequality operator:
  inline int operator != ( Point3D &V );
  // The subtraction operator:
  inline Point3D operator - ( Point3D &V );
  // The addition operator:  
  inline Point3D operator + ( Point3D &V );
  // The multiplication operator:  
  inline Point3D operator * ( Point3D &V );
  // The division operator:  
  inline Point3D operator / ( Point3D &V );
  // The subtraction/assignment operator:  
  inline Point3D &operator -= ( Point3D &V );
  // The addition/assignment operator:  
  inline Point3D &operator += ( Point3D &V );
  // The multiplication/assignment operator:  
  inline Point3D &operator *= ( Point3D &V );
  // The division/assignment operator:  
  inline Point3D &operator /= ( Point3D &V );
  inline Point3D operator - ( double V );
  // The addition operator: 
  inline Point3D operator + ( double V );
  // The multiplication operator:  
  inline Point3D operator * ( double V );
  // The division operator:  
  inline Point3D operator / ( double V );
  // The subtraction/assignment operator:  
  inline Point3D &operator -= ( double V );
  // The addition/assignment operator:  
  inline Point3D &operator += ( double V );
  // The multiplication/assignment operator:  
  inline Point3D &operator *= ( double V );
  // The division/assignment operator:  
  inline Point3D &operator /= ( double V );
  inline float Mag (); // Returns the magnitute of the 3D point
  inline float DotUnit ( Point3D &V );
  inline float DotNonUnit ( Point3D &V );
  void Read ( FILE *File );
  void Write ( FILE *File );
};

int UniqueVert ( Point3D &V, Point3D *List, int Range );
unsigned int GetVertIndex ( Point3D &V, Point3D *List, unsigned int Range );

// Function section:
inline int Point3D::operator == ( Point3D &V )
  {
  int RValue = 0;
  if ( V.Lx == Lx )
     if ( V.Ly == Ly )
        if ( V.Lz == Lz )
           RValue = 1;
  return RValue;
  }

// The inequality operator:
inline int Point3D::operator != ( Point3D &V )
  {
  int RValue = 0;
  if ( ( V.Lx != Lx ) || ( V.Ly != Ly ) || ( V.Lz != Lz ) )
     RValue = 1;
  return RValue;
  }

// The subtraction operator:
inline Point3D Point3D::operator - ( Point3D &V )
  {
  Point3D Temp;
  Temp.Lx = Lx - V.Lx;
  Temp.Ly = Ly - V.Ly;
  Temp.Lz = Lz - V.Lz;
  return Temp;
  }

// The addition operator:  
inline Point3D Point3D::operator + ( Point3D &V )
  {
  Point3D Temp;
  Temp.Lx = Lx + V.Lx;
  Temp.Ly = Ly + V.Ly;
  Temp.Lz = Lz + V.Lz;
  return Temp;
  }

// The multiplication operator:  
inline Point3D Point3D::operator * ( Point3D &V )
  {
  Point3D Temp;
  Temp.Lx = Lx * V.Lx;
  Temp.Ly = Ly * V.Ly;
  Temp.Lz = Lz * V.Lz;
  return Temp;
  }

// The division operator:  
inline Point3D Point3D::operator / ( Point3D &V )
  {
  Point3D Temp;
  Temp.Lx = Lx / V.Lx;
  Temp.Ly = Ly / V.Ly;
  Temp.Lz = Lz / V.Lz;
  return Temp;
  }

// The subtraction/assignment operator:  
inline Point3D &Point3D::operator -= ( Point3D &V )
  {
  Lx -= V.Lx;
  Ly -= V.Ly;
  Lz -= V.Lz;
  return *this;
  }

// The addition/assignment operator:  
inline Point3D &Point3D::operator += ( Point3D &V )
  {
  Lx += V.Lx;
  Ly += V.Ly;
  Lz += V.Lz;
  return *this;
  }
// The multiplication/assignment operator:  
inline Point3D &Point3D::operator *= ( Point3D &V )
  {
  Lx *= V.Lx;
  Ly *= V.Ly;
  Lz *= V.Lz;
  return *this;
  }

// The division/assignment operator:  
inline Point3D &Point3D::operator /= ( Point3D &V )
  {
  Lx /= V.Lx;
  Ly /= V.Ly;
  Lz /= V.Lz;
  return *this;
  }
// Subtraction operator:
inline Point3D Point3D::operator - ( double V )
  {
  Point3D Temp;
  Temp.Lx = Lx - V;
  Temp.Ly = Ly - V;
  Temp.Lz = Lz - V;
  return Temp;
  }

// The addition operator:  
inline Point3D Point3D::operator + ( double V )
  {
  Point3D Temp;
  Temp.Lx = Lx + V;
  Temp.Ly = Ly + V;
  Temp.Lz = Lz + V;
  return Temp;
  }

// The multiplication operator:  
inline Point3D Point3D::operator * ( double V )
  {
  Point3D Temp;
  Temp.Lx = Lx * V;
  Temp.Ly = Ly * V;
  Temp.Lz = Lz * V;
  return Temp;
  }

// The division operator:  
inline Point3D Point3D::operator / ( double V )
  {
  Point3D Temp;
  Temp.Lx = Lx / V;
  Temp.Ly = Ly / V;
  Temp.Lz = Lz / V;
  return Temp;
  }

// The subtraction/assignment operator:  
inline Point3D &Point3D::operator -= ( double V )
  {
  Lx -= V;
  Ly -= V;
  Lz -= V;
  return *this;
  }

// The addition/assignment operator:  
inline Point3D &Point3D::operator += ( double V )
  {
  Lx += V;
  Ly += V;
  Lz += V;
  return *this;
  }

// The multiplication/assignment operator:  
inline Point3D &Point3D::operator *= ( double V )
  {
  Lx *= V;
  Ly *= V;
  Lz *= V;
  return *this;
  }

// The division/assignment operator:  
inline Point3D &Point3D::operator /= ( double V )
  {
  Lx /= V;
  Ly /= V;
  Lz /= V;
  return *this;
  }

inline float Point3D::Mag ()
  {
  return sqrt ( Lx * Lx + Ly * Ly + Lz * Lz );
  }

inline float Point3D::DotUnit ( Point3D &V )
  {
  return ( Lx * V.Lx + Ly * V.Ly + Lz * V.Lz );
  }

inline float Point3D::DotNonUnit ( Point3D &V )
  {
  float Dot = ( Lx * V.Lx +
                Ly * V.Ly +
                Lz * V.Lz ) / ( Mag () * V.Mag () );
  return Dot;
  }

inline void Point3D::Read ( FILE *File )
   {
   fread ( &Lx, sizeof Lx, 1, File );
   fread ( &Ly, sizeof Ly, 1, File );
   fread ( &Lz, sizeof Lz, 1, File );
   }

inline void Point3D::Write ( FILE *File )
   {
   fwrite ( &Lx, sizeof Lx, 1, File );
   fwrite ( &Ly, sizeof Ly, 1, File );
   fwrite ( &Lz, sizeof Lz, 1, File );
   }

#endif
