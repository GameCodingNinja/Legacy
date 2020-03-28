//
// File name: PolyObj.HPP
//
// Description: Header file for a panel object class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef POLYOBJHPP
#define POLYOBJHPP

#include "Matrix3D.HPP"
#include "Point3D.HPP"
#include "ATypes.HPP"
#include "Panel3D.HPP"

// An object made of panels:
class PanelObject {
  Point3D *TList;         // Temporary vertex list (used for loading)
protected:              // Data is protected
  void CalcRadius ();     // Calculates radius of object
  void Transform ( Matrix3D &M );       // Transforms vertex list
  Point3D LoadCoord ( FILE *InFile );   // Loads a coordinate
  DWORD CountPanels ( char *FileName ); // Counts 3DFACEs
  WORD LoadVerts ( char *FileName );    // Loads vertex list
  WORD LoadPanelData ();                // Loads panel data
  WORD LoadPanel ( int &VIndex, int Index ); // Loads a panel
public:                                 // Data is public
  PanelObject () { VCount = PCount = 0; } // Constructor
  ~PanelObject ()                         // Destructor
    {
    if ( VList )
       delete [] VList;
    if ( PList )
       delete [] PList;
    }
  Point3D  *VList;  // The list of vertices
  Panel3D *PList;   // The list of panels
  DWORD   VCount;   // The vertex count
  DWORD   PCount;   // The panel count
  WORD Visible;     // A visible flag
  double Radius;    // The object's radius
  void Display ( Matrix3D &M, unsigned char *Buffer ); // A display function
  void LoadDXF ( char *FileName );  // A function for loading DXF files
  void WriteBIN ( char *FileName ); // A function for writing BIN files
  void ReadBIN ( char *FileName );  // A function for reading BIN files
};

#endif