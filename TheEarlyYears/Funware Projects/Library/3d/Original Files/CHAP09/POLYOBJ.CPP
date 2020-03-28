//
//
// File name: PolyObj.CPP
//
// Description: The support file for the PolyObj.HPP header
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include <Math.H>
#include <CType.H>
#include <Stdio.H>
#include <String.H>

// Temp include:
#include <ConIO.H>
#include <IOStream.H>

#include "PolyObj.HPP"
#include "Matrix3D.HPP"
#include "Point3D.HPP"
#include "LightP.HPP"

// Logical replacement for "strcmp" - returns true
// if strings match:
int inline Match ( char *S1, char *S2 )
  {
  return !strcmp ( S1, S2 );
  }


int GetLine ( FILE *InFile, char *String )
   {
   // Reads a line of text from a text file:
   int NextByte = fgetc ( InFile );
   int Index = 0;
   
   // Loop until we reach a new-line character:
   while ( NextByte != '\n' )
         {
         // Check for end of file:
         if ( NextByte == EOF )
            {
            // If found, close off string
            // and return EOF:
            String [ Index ] = '\0';
            return EOF;
            }

         // If the next byte is not a space....
         if ( !isspace ( NextByte ) )
            {
            // ....record it:
            String [ Index++ ] = ( char ) NextByte;
            }
            
         // Get the next character:
         NextByte = fgetc ( InFile );
         }
   // Close off the string and return success (true):
   String [ Index ] = '\0';//*/
   return 1;
   }
   
void PanelObject::CalcRadius ()
   {
   // Calculates the radius of the object:
   }
   
void PanelObject::Transform ( Matrix3D &M )
   {
   // Translates/rotates entire vertex list:
   unsigned int Count;

   // Transform the vertex list:
   for ( Count = 0; Count < VCount; Count++ )
       {
       M.Transform ( VList [ Count ] );
       }

   // Update the normal and other panel-specific data:
   for ( Count = 0; Count < PCount; Count++ )
       {
       PList [ Count ].Update ( M );
       }
   }

void PanelObject::Display ( Matrix3D &M, unsigned char *Buffer )
   {
   // Displays a world on screen buffer "Buffer"
   const False = 0;
   // Declare variables:
   unsigned int Count;

   // Transform the vertex points:
   Transform ( M );
   
   // Display selected panels in the off-screen buffer:
   for ( Count = 0; Count < PCount; Count++ )
       {
       // Determine if polygon is invisible:
       if ( PList [ Count ].Invisible () == False )
          {
          // If the panel is visible in 3D....
          if ( PList [ Count ].IsVisible3D () )
             {
             // ....project it:
             PList [ Count ].Project ();

             // If panel is visible in 2D....
             if ( PList [ Count ].IsVisible2D () )
                {
                // ....display it:
                PList [ Count ].Display ( Buffer );
                }

             }
          }
       }
   } 

Point3D PanelObject::LoadCoord ( FILE *InFile )
   {
   // Loads a single coordinate from a text file:
   int PCount = 0;
   char String [ 100 ] = "";
   Point3D Coord;
   while ( PCount < 3 )
         {
         if ( GetLine ( InFile, String ) == EOF )
            {
            Coord.Lx = 0;
            Coord.Ly = 0;
            Coord.Lz = 0;
            break;
            }

         // Check for the first X coordinate:
         if ( Match ( String, "10" ) )
            {
            GetLine ( InFile, String );
            Coord.Lx = atof ( String );
            ++PCount;
            }
         // Check for the second X coordinate:
         else if ( Match ( String, "11" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lx = atof ( String );
                 ++PCount;
                 }
         // Check for the third X coordinate:
         else if ( Match ( String, "12" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lx = atof ( String );
                 ++PCount;
                 }
         // Check for the fourth X coordinate:
         else if ( Match ( String, "13" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lx = atof ( String );
                 ++PCount;
                 }
         // Check for the first Y coordinate:
         else if ( Match ( String, "20" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Ly = atof ( String );
                 ++PCount;
                 }
         // Check for the second Y coordinate:
         else if ( Match ( String, "21" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Ly = atof ( String );
                 ++PCount;
                 }
         // Check for the third Y coordinate:
         else if ( Match ( String, "22" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Ly = atof ( String );
                 ++PCount;
                 }
         // Check for the fourth Y coordinate:
         else if ( Match ( String, "23" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Ly = atof ( String );
                 ++PCount;
                 }
         // Check for the first Z coordinate:
         else if ( Match ( String, "30" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lz = atof ( String );
                 ++PCount;
                 }
         // Check for the second Z coordinate:
         else if ( Match ( String, "31" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lz = atof ( String );
                 ++PCount;
                 }
         // Check for the third Z coordinate:
         else if ( Match ( String, "32" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lz = atof ( String );
                 ++PCount;
                 }
         // Check for the fourth Z coordinate:
         else if ( Match ( String, "33" ) )
                 {
                 GetLine ( InFile, String );
                 Coord.Lz = atof ( String );
                 ++PCount;
                 }
         }

   // Return a copy of the loaded vertex:
   return Coord;
   }

DWORD PanelObject::CountPanels ( char *FileName )
  {
  // Count the number of panels in a text file:
  char String [ 100 ] = "";
  DWORD PanelCount = 0;
  FILE *InFile;

  // Open the file:
  if ( ( InFile = fopen ( FileName, "rt" ) ) == 0 )
     return 0;

  // Loop until we come to the end of the file:
  for ( ;; )
      {
      // Get a line of text:
      if ( GetLine ( InFile, String ) == EOF )
         break;

      // Search for a panel (3DFACE):
      if ( Match ( String, "3DFACE" ) )
         ++PanelCount;
      }

  // Close the file:
  fclose ( InFile );

  // Return number of vertices:
  return PanelCount;
  }
   
WORD PanelObject::LoadVerts ( char *FileName )
  {
  // Load all vertices from a DXF text file:

  // Declare/initialize variables:

  PCount = CountPanels ( FileName );

  // Allocate memory for temporary list:
  if ( ( TList = new Point3D [ PCount * 4 ] ) == 0 )
     return 0;

  // The vertex index in the temporary list:
  unsigned int VIndex = 0, Count;

  // A file structure:
  FILE *InFile;
  
  // Open file - abort if error:
  if ( ( InFile = fopen ( FileName, "rt" ) ) == 0 )
     return 0;

  // An all-purpose string:
  char String [ 100 ] = "";
  
  // Loop until we come to the end of the file:
  for ( ;; )
      {
      // Get a line of text:
      if ( GetLine ( InFile, String ) == EOF )
         break;

      // If a 3DFACE entity is encountered....
      if ( Match ( "3DFACE", String ) )
         {
         // Load four vertices:
         for ( Count = 0; Count < 4; Count++ )
             {
             // Load the next coordinate:
             Point3D TPoint3D = LoadCoord ( InFile );
             TList [ VIndex++ ] = TPoint3D;   
             }
         }
      }

  // Close the file:
  fclose ( InFile );

  // Set the vertex count at zero:
  VCount = 0;

  // Allocate memory for vertex list - abort if error:
  if ( ( VList = new Point3D [ VIndex ] ) == 0 )
     return 0;

  // Copy TList to VList:
  for ( Count = 0; Count < VIndex; Count++ )
      {
      if ( UniqueVert ( TList [ Count ], VList, VCount ) )
         {
         VList [ VCount ] = TList [ Count ];

         // Increment the vertex count/index:
         ++VCount;
         }
      }
  
  // Return true (success):
  return 1;
  }

WORD PanelObject::LoadPanelData ()
   {
   // Read the panel data from a DXF file:
   // Note: Assumes all vertices have been loaded via member
   //       function "LoadVerts"

   // Allocate memory for files:
   if ( ( PList = new Panel3D [ PCount ] ) == 0 )
      return 0;

   int VIndex = 0;

   // Loop until we come to the end of the file:
   for ( unsigned int PIndex = 0; PIndex < PCount; PIndex++ )
       {
       // Load an entire panel:
       LoadPanel ( VIndex, PIndex );
       }

   // Deallocate temporary list data
   delete [] TList;

   // Return true (success):
   return 1;
   }

WORD PanelObject::LoadPanel ( int &VIndex, int Index )
   {
   // Load a panel from a DXF text file:

   // 3DFACE has been found - load four coordinates:
   for ( unsigned int Count = 0; Count < 4; Count++ )
       {
       // Assign a vertex to the panel:
       PList [ Index ].VPoint [ Count ] = 
             &VList [ GetVertIndex (  TList [ VIndex ], 
             VList, VCount ) ];

       // Increment the vertex index:
       ++VIndex;
       }

   // Calculate panel normal:
   PList [ Index ].InitPosition ();

   // Calculate panel's intensity:
   PList [ Index ].CalcInten ();

   // Return true (success):
   return 1;
   }

void PanelObject::LoadDXF ( char *FileName )
   {
   // Load an object from a DXF text file:
   LoadVerts ( FileName );
   LoadPanelData ();
   }

void PanelObject::ReadBIN ( char *FileName )
   {
   // Load an object from a BIN binary file:
   unsigned int VertCount, PanelCount, Count;
   unsigned int SharedIndex [ 100 ], SharedIndexCount, N, SharedColor;
   unsigned int short PIndex, VIndex;
   double IPer;
   Point3D TVert;
   FILE *InFile;
   if ( ( InFile = fopen ( FileName, "rb" ) ) == NULL )
      return;
   fread ( &VertCount, sizeof VertCount, 1, InFile );
   fread ( &PanelCount, sizeof PanelCount, 1, InFile );
   // Allocate memory:
   VList = new Point3D [ VertCount ];
   PList = new Panel3D [ PanelCount ];

   // Load vertices:
   for ( Count = 0; Count < VertCount; Count++ )
       {
       VList [ Count ].Read ( InFile );
       }
   VCount = VertCount;
   PCount = PanelCount;

   cout << "\nReading Polygon Dataset:\n";
   // Load panel data:
   for ( Count = 0; Count < PCount; Count++ )
       {
       IPer = ( ( double ) Count / ( double ) PCount ) * 101.0F;
       cout << ( long ) IPer << "%\r";
       for ( VIndex = 0; VIndex < 4; VIndex++ )
           {
           fread ( &PIndex, sizeof PIndex, 1, InFile );
           PList [ Count ].VPoint [ VIndex ] = &VList [ PIndex ];
           }
       // Calculate the panel normal:
       PList [ Count ].InitPosition ();

       // Calculate the panel's intensity:
       PList [ Count ].CalcInten ();
       }
   fclose ( InFile );
   cout << "\nCalculating Gouraud Information:\n";
   // Calculate Gouraud information for all polygons:
   for ( Count = 0; Count < PCount; Count++ )
       {
       IPer = ( ( double ) Count / ( double ) PCount ) * 101.0F;
       cout << ( long ) IPer << "%\r";
       // Calculate an intensity value for each of the vertices:
       for ( VIndex = 0; VIndex < 4; VIndex++ )
           {
           SharedIndexCount = 0;
           SharedColor = 0;
           TVert = *PList [ Count ].VPoint [ VIndex ];
           // Search through the entire list of panels looking 
           // for shared vertices:
           for ( N = 0; N < PCount; N++ )
               {
               if ( PList [ N ].HasVert ( TVert ) )
                  {
                  SharedIndex [ SharedIndexCount++ ] = N;
                  }
               }
           for ( N = 0; N < SharedIndexCount; N++ )
               {
               SharedColor += PList [ SharedIndex [ N ] ].Color;
               }
           if ( !SharedIndexCount )
              SharedIndexCount = 1;
           SharedColor /= SharedIndexCount;
           PList [ Count ].Inten [ VIndex ] = ( WORD ) SharedColor;
           }
       }
   }

void PanelObject::WriteBIN ( char *FileName )
   {
   // Write a BIN binary file:
   unsigned int VertCount = VCount, PanelCount = PCount, Count;
   unsigned int short PIndex, VIndex;
   Point3D *PVert;
   FILE *OutFile;
   if ( ( OutFile = fopen ( FileName, "wb" ) ) == NULL )
      return;
   fwrite ( &VertCount, sizeof VertCount, 1, OutFile );
   fwrite ( &PanelCount, sizeof PanelCount, 1, OutFile );

   // Write vertex data:
   for ( Count = 0; Count < VCount; Count++ )
       {
       VList [ Count ].Write ( OutFile );
       }

   // Write panel data:
   for ( Count = 0; Count < PCount; Count++ )
       {
       for ( VIndex = 0; VIndex < 4; VIndex++ )
           {
           PVert = PList [ Count ].VPoint [ VIndex ];
           PIndex = ( unsigned short ) GetVertIndex ( *PVert, VList, VCount );
           fwrite ( &PIndex, sizeof PIndex, 1, OutFile );
           }
       }
   fclose ( OutFile );
   }
