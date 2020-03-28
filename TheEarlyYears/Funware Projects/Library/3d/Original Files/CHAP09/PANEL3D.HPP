//
// File name: Panel3D.HPP
//
// Description: Header file for a 3D panel class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef PANEL3DHPP
#define PANEL3DHPP

#include "Point3D.HPP"
#include "Point2D.HPP"
#include "Vector3D.HPP"
#include "ATypes.HPP"
#include "PolyEdge.HPP"
#include "Matrix3D.HPP"

#include <Dos.H>

// Forward declaration:
class PanelObject;

// The panel class:
class Panel3D {
protected:                 // Data is protected
  // Functions:
  void CalcRadius ();      // Calculates panel radius
  long CalcCenterZ ();     // Calculates center Z  
  int inline CalcBFace (); // Determines if panel is backface
  void CalcNormal ();      // Calculates the panel normal
  int CheckExtents ();     // Determines if panel lies within
                           // the Z extents
  int CalcVisible3D ();    // Determines panel's visibility 
                           // in 3D
  int CalcVisible2D ();    // Determines panel's visibility
                           // in 2D
  // Data:
  Point3D *VPoint [ 4 ];   // 4 pointers to a vertex list
  Point2D SPoint [ 5 ];    // Maximum of 5 on-screen points
  WORD Inten [ 4 ];
  Vector Normal;           // The panel normal
  WORD SPCount, Invis;     // Screen point count & invisible flag
  BYTE Color, Padding;     // The panel color
  double Radius;           // The panel's radius
public:                    // Data is public
  Panel3D () { Invis = 0; Color = 0; }  // Constructor:
  void Update ( Matrix3D &M );          // Update normal and misc. information
  void Display ( unsigned char *Dest ); // Rasterize panel
  void Project ();                      // Project panel onto screen
  void InitPosition () { CalcNormal (); CalcRadius (); } // Performs initialization
  void CalcInten ();                    // Calculates panel's intensity
  int Invisible () { return Invis; }    // Determine invisibility
  int IsVisible2D () { return CalcVisible2D (); } // Determine visibility in 3D
  int IsVisible3D () { return CalcVisible3D (); } // Determine visibility in 2D
  
  friend class PanelObject;   // Declare a friend class

  int HasVert ( Point3D &P )
    {
    if ( ( *VPoint [ 0 ] ) == P )
       return 1;
    if ( ( *VPoint [ 1 ] ) == P )
       return 1;
    if ( ( *VPoint [ 2 ] ) == P )
       return 1;
    if ( ( *VPoint [ 3 ] ) == P )
       return 1;
    return 0;
    }
};

#endif
