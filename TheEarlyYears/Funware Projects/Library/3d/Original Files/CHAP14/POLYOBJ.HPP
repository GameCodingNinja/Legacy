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
  Point3D *TList;     // Temporary vertex list (used for loading)
  Point3D *MList;     // The morphing list

protected:            // Data is protected
  void CalcRadius (); // Calculates radius of object
  void Transform ( Matrix3D &M );      // Transforms vertex list
  Point3D LoadCoord ( FILE *InFile );  // Loads a coordinate
  DWORD CountPanels ( char *FileName );// Counts 3DFACEs
  WORD LoadVerts ( char *FileName );   // Loads vertex list
  WORD LoadPanelData ();               // Loads panel data
  WORD LoadPanel ( int &VIndex, int Index ); // Loads a panel
public:                                // Data is public
  Point3D  *VList; // The list of vertices
  Panel3D *PList;  // The list of panels
  DWORD   VCount,  // The vertex count
          PCount;  // The panel count
  WORD Visible,    // A visible flag
       Morph,      // A morphing flag
       StartMorph;
  long Select;
  float RadiusXY, RadiusXZ, Radius;    // The object's radius
  PanelObject () // Constructor
    {
    Morph = 0;
    StartMorph = 1;
    Select = -1;
    VCount = PCount = 0;
    }
  ~PanelObject () // Destructor
    {
    if ( VList )
       delete [] VList;
    if ( PList )
       delete [] PList;
    if ( Morph )
       delete [] MList;
    }

  void Display ( Matrix3D &M, unsigned char *Buffer ); // Displays object
  void LoadDXF ( char *FileName );  // Loads DXF files
  void WriteBIN ( char *FileName ); // Writes BIN files
  void ReadBIN ( char *FileName );  // Reads BIN files

  int ReadText ( char *FileName );  // Loads textures/data
  int WriteText ( char *FileName ); // Writes textures/data

  void InitDefText (); // Initializes default texture/shade 
                       // coords
  void MoveText ( int Ox, int Oy, int Tx, int Ty );
  void NexText () // Selects the next texture
     {
     if ( ( Select >= 0 ) && ( Select < PCount ) )
        PList [ Select ].NexText ();
     }
  void RotText () // Rotates the selected panel's texture
     {
     if ( ( Select >= 0 ) && ( Select < PCount ) )
        PList [ Select ].RotText ();
     }
  void NextInten ( int Sx, int Sy ); // Selects the next 
                                     // intensity
  int MorphTo ( char *FileName, float Steps );
  void MorphStop () { StartMorph = 0; }
  void MorphStart () { StartMorph = 1; }
  void MorphRev ();

  int Collide ( float &X, float &Y, float &Z, float Rad );
};

#endif