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
#include "Detail3D.HPP"
#include "ATypes.HPP"
//#include "PolyEdge.HPP"
#include "Matrix3D.HPP"
#include "TextType.HPP"

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
  void DisplaySel ( unsigned char *Dest );
  void DisplayNorm ( unsigned char *Dest );
  // Data:
  Point3D *VPoint [ 4 ];   // 4 pointers to a vertex list
  Detail3D Attrib [ 4 ];   // The panel's detail attributes
  Vector Normal;           // The panel normal
  Point2D SPoint [ 5 ];    // Maximum of 5 on-screen points
  WORD SPCount, Invis;     // Screen point count & invisible flag
  BYTE Color, TextHan;     // The panel color * texture handle
  float RadiusXZ;         // The panel's X/Z extent
  float RadiusY;          // The panel's Y extent
  float Radius;
public:                   // Data is public
  Panel3D () { Color = Invis = 0; } // Constructor:
  void Update ( Matrix3D &M );          // Update normal and misc. information
  void Display ( unsigned char *Dest )
     {
     DisplayNorm ( Dest );
     }
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

  void NexText ()
     {
     TextHan = ( BYTE ) ( ( TextHan + 1 ) % TextDat.TCount );
     }
  void RotText ()
     {
     Detail3D Temp;
     Temp = Attrib [ 0 ];
     Attrib [ 0 ] = Attrib [ 1 ];
     Attrib [ 1 ] = Attrib [ 2 ];
     Attrib [ 2 ] = Attrib [ 3 ];
     Attrib [ 3 ] = Temp;
     }
};

#endif
