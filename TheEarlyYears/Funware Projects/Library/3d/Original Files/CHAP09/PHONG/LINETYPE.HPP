//
// File name: LineType.HPP
//
// Description: A fixed-point implementation of a polygon edge 
//             algorithm
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#define XSTEP_PREC 10
#define ZSTEP_PREC 28
#define ISTEP_PREC 18

#include <Dos.H>
#include <Math.H>

struct Point3D {
double Wx, Wy, Wz, Nx, Ny, Nz;
};

struct Point2D {
long X, Y;
};

struct LightSource {
double X, Y, Z;
};

extern LightSource Light;

// Ceiling step constant:
const long CEIL_FRACT = ( ( 1 << XSTEP_PREC ) - 1 );

// A class that steps on the ceiling of integers - always steps 
// on Y, rendering it useless for anything but polygon 
// rasterization: 
class CeilLine {
protected:
  long X1, X2, Y1, Y2;
  long X, StepX;
  double Nx, Ny, Nz, StepNx, StepNy, StepNz;
  long EdgeHeight, Y;
  void inline Step ();
  void inline Step ( long Amount );
public:
  CeilLine () { EdgeHeight = 0; }  // The constructor

  // The initialize function
  void inline Init ( Point2D P1, Point3D N1, Point2D P2, Point3D N2); 

  void operator ++ () { Step (); }        // Steps polygon edge
  void operator ++ ( int ) { Step (); }
  long GetX () { return ( X >> XSTEP_PREC ); } // Returns the current X coordinate
  long GetY () { return Y; } // Returns the current Y coordinate
  double GetNx () { return Nx; } // Returns the x component of the normal
  double GetNy () { return Ny; } // Returns the y component of the normal
  double GetNz () { return Nz; } // Returns the z component of the normal
  long Height () { return EdgeHeight; } // Returns the current poly height
};

void inline CeilLine::Init ( Point2D P1, Point3D N1, 
                             Point2D P2, Point3D N2 )
   {
   // Calculate initial values for polygon edge:
   long FWidth;
   X1 = P1.X; X2 = P2.X;
   Y1 = P1.Y; Y2 = P2.Y;

   EdgeHeight = ( Y2 - Y1 );
   FWidth = ( X2 - X1 ) << XSTEP_PREC;
   X = ( X1 << XSTEP_PREC ) + CEIL_FRACT;
   Y = Y1;
   Nx = N1.Nx;
   Ny = N1.Ny;
   Nz = N1.Nz;

   if ( EdgeHeight )
      {
      StepX = FWidth / EdgeHeight;
      StepNx = ( N2.Nx - N1.Nx ) / ( double ) EdgeHeight;
      StepNy = ( N2.Ny - N1.Ny ) / ( double ) EdgeHeight;
      StepNz = ( N2.Nz - N1.Nz ) / ( double ) EdgeHeight;
      }
   else {
        StepX = 0;
        }
   }

void inline CeilLine::Step ()
   {
   // Step the edge:
   X += StepX;
   ++Y; Nx += StepNx;
   Ny += StepNy;
   Nz += StepNz;
   --EdgeHeight;
   }
