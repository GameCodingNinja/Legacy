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
#include "TextType.HPP"
#include "PalShade.HPP"

extern float GlobalStep;

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
   // Translates/rotates/morphs entire vertex list:
   unsigned int Count;

   // Transform the vertex list:
   if ( ( Morph ) && ( StartMorph ) )
      {
      for ( Count = 0; Count < VCount; Count++ )
          {
          M.Transform ( VList [ Count ] );
          // Perform the morph:
          VList [ Count ] += MList [ Count ];
          }
      }

   else {
        for ( Count = 0; Count < VCount; Count++ )
            {
            M.Transform ( VList [ Count ] );
            }
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


void PanelObject::MoveText ( int Ox, int Oy, int Tx, int Ty )
   {
   float Dist [ 5 ], X, Y;
   long Count, ScreenCount, MinDist;
   // Make sure selection is in allowed range:
   if ( ( Select >= 0 ) && ( Select < PCount ) )
      {
      PList [ Select ].Project ();
      ScreenCount = PList [ Select ].SPCount;
      if ( ScreenCount > 4 )
         ScreenCount = 4;
      // Calculate the distance from each vertex to (Ox, Oy):
      for ( Count = 0; Count < ScreenCount; Count++ )
          {
          X = PList [ Select ].SPoint [ Count ].X;
          Y = PList [ Select ].SPoint [ Count ].Y;
          Dist [ Count ] = sqrt ( ( X - Ox ) * ( X - Ox ) +
                                  ( Y - Oy ) * ( Y - Oy ) );
          }
      // Select minimum distance:
      MinDist = 0;
      for ( Count = 1; Count < ScreenCount; Count++ )
          {
          if ( Dist [ Count ] < Dist [ MinDist ] )
             MinDist = Count;
          }
      // Translate texture vertex by specified amount:
      PList [ Select ].Attrib [ MinDist ].U += Tx;
      PList [ Select ].Attrib [ MinDist ].V += Ty;
      // Clip selected translation:
      if ( PList [ Select ].Attrib [ MinDist ].U < 0 )
         PList [ Select ].Attrib [ MinDist ].U = 0;

      if ( PList [ Select ].Attrib [ MinDist ].V < 0 )
         PList [ Select ].Attrib [ MinDist ].V = 0;

      if ( PList [ Select ].Attrib [ MinDist ].U >=
           TextDat.TMap [ PList [ Select ].TextHan ].Width )
         {
         PList [ Select ].Attrib [ MinDist ].U = 
             ( TextDat.TMap [ PList [ Select ].TextHan ].Width 
               - 1 );
         }

      if ( PList [ Select ].Attrib [ MinDist ].V >=
           TextDat.TMap [ PList [ Select ].TextHan ].Height )
         {
         PList [ Select ].Attrib [ MinDist ].V = 
             ( TextDat.TMap [ PList [ Select ].TextHan ].Height 
               - 1 );
         }
      }
   }

void PanelObject::NextInten ( int Sx, int Sy )
   {
   float Dist [ 5 ], X, Y;
   long Count, ScreenCount, MinDist;
   // Make sure selection is in allowed range:
   if ( ( Select >= 0 ) && ( Select < PCount ) )
      {
      PList [ Select ].Project ();
      ScreenCount = PList [ Select ].SPCount;
      if ( ScreenCount > 4 )
         ScreenCount = 4;
      // Calculate the distance from each vertex to (Sx, Sy):
      for ( Count = 0; Count < ScreenCount; Count++ )
          {
          X = PList [ Select ].SPoint [ Count ].X;
          Y = PList [ Select ].SPoint [ Count ].Y;
          Dist [ Count ] = sqrt ( ( X - Sx ) * ( X - Sx ) +
                                  ( Y - Sy ) * ( Y - Sy ) );
          }
      // Select minimum distance:
      MinDist = 0;
      for ( Count = 1; Count < ScreenCount; Count++ )
          {
          if ( Dist [ Count ] < Dist [ MinDist ] )
             MinDist = Count;
          }
      // Select next intensity:
      ++PList [ Select ].Attrib [ MinDist ].I;
      if ( PList [ Select ].Attrib [ MinDist ].I > ( SHADE_COUNT - 1 ) )
         PList [ Select ].Attrib [ MinDist ].I = 0;
      }
   }

void PanelObject::InitDefText ()
   {
   float IPer;
   long Count, Odd, Tri;
   cout << "\nInitializing texture-shade data:\n";
   for ( Count = 0; Count < PCount; Count++ )
       {
       IPer = ( ( float ) Count / ( float ) PCount ) * 101.0F;
       cout << ( long ) IPer << "%\r";
       Odd = Count % 2;
       PList [ Count ].TextHan = 0;
       Tri = 0;
       if ( ( *PList [ Count ].VPoint [ 0 ] ) == 
            ( *PList [ Count ].VPoint [ 1 ] ) )
          Tri = 1;
       else if ( ( *PList [ Count ].VPoint [ 0 ] ) == 
                 ( *PList [ Count ].VPoint [ 2 ] ) )
               Tri = 1;
       else if ( ( *PList [ Count ].VPoint [ 0 ] ) == 
                 ( *PList [ Count ].VPoint [ 3 ] ) )
               Tri = 1;

       else if ( ( *PList [ Count ].VPoint [ 1 ] ) == 
                 ( *PList [ Count ].VPoint [ 2 ] ) )
               Tri = 1;
       else if ( ( *PList [ Count ].VPoint [ 1 ] ) == 
                 ( *PList [ Count ].VPoint [ 3 ] ) )
               Tri = 1;

       else if ( ( *PList [ Count ].VPoint [ 2 ] ) == 
                 ( *PList [ Count ].VPoint [ 3 ] ) )
               Tri = 1;

       if ( Tri )
          {
          if ( !Odd )
             {
             PList [ Count ].Attrib [ 3 ].U = 0;
             PList [ Count ].Attrib [ 3 ].V = 0;
             PList [ Count ].Attrib [ 3 ].I = PList [ Count ].Color;

             PList [ Count ].Attrib [ 2 ].U = TextDat.TMap [ 0 ].Width - 1;
             PList [ Count ].Attrib [ 2 ].V = 0;
             PList [ Count ].Attrib [ 2 ].I = PList [ Count ].Color;

             PList [ Count ].Attrib [ 1 ].U = TextDat.TMap [ 0 ].Width - 1;
             PList [ Count ].Attrib [ 1 ].V = TextDat.TMap [ 0 ].Height - 1;
             PList [ Count ].Attrib [ 1 ].I = PList [ Count ].Color;

             PList [ Count ].Attrib [ 0 ].U = TextDat.TMap [ 0 ].Width - 1;
             PList [ Count ].Attrib [ 0 ].V = TextDat.TMap [ 0 ].Height - 1;
             PList [ Count ].Attrib [ 0 ].I = PList [ Count ].Color;
             }
          if ( Odd )
             {
             PList [ Count ].Attrib [ 3 ].U = 0;
             PList [ Count ].Attrib [ 3 ].V = 0;
             PList [ Count ].Attrib [ 3 ].I = PList [ Count ].Color;

             PList [ Count ].Attrib [ 2 ].U = TextDat.TMap [ 0 ].Width - 1;
             PList [ Count ].Attrib [ 2 ].V = TextDat.TMap [ 0 ].Height - 1;
             PList [ Count ].Attrib [ 2 ].I = PList [ Count ].Color;

             PList [ Count ].Attrib [ 1 ].U = 0;
             PList [ Count ].Attrib [ 1 ].V = TextDat.TMap [ 0 ].Height - 1;
             PList [ Count ].Attrib [ 1 ].I = PList [ Count ].Color;

             PList [ Count ].Attrib [ 0 ].U = 0;
             PList [ Count ].Attrib [ 0 ].V = TextDat.TMap [ 0 ].Height - 1;
             PList [ Count ].Attrib [ 0 ].I = PList [ Count ].Color;
             }
           }
       else {
            PList [ Count ].Attrib [ 0 ].U = 0;
            PList [ Count ].Attrib [ 0 ].V = 0;
            PList [ Count ].Attrib [ 0 ].I = PList [ Count ].Color;

            PList [ Count ].Attrib [ 1 ].U = TextDat.TMap [ 0 ].Width - 1;
            PList [ Count ].Attrib [ 1 ].V = 0;
            PList [ Count ].Attrib [ 1 ].I = PList [ Count ].Color;

            PList [ Count ].Attrib [ 2 ].U = TextDat.TMap [ 0 ].Width - 1;
            PList [ Count ].Attrib [ 2 ].V = TextDat.TMap [ 0 ].Height - 1;
            PList [ Count ].Attrib [ 2 ].I = PList [ Count ].Color;

            PList [ Count ].Attrib [ 3 ].U = 0;
            PList [ Count ].Attrib [ 3 ].V = TextDat.TMap [ 0 ].Height - 1;
            PList [ Count ].Attrib [ 3 ].I = PList [ Count ].Color;
            }
       }
   }

int PanelObject::MorphTo ( char *FileName, float Steps )
  {
  // Note: FileName must be a BIN file
  // Load an object from a BIN binary file:
   unsigned int VertCount, PanelCount, Count;
   Point3D TVert;
   FILE *InFile;

   StartMorph = 1;

   if ( ( InFile = fopen ( FileName, "rb" ) ) == NULL )
      return 0;
   fread ( &VertCount, sizeof VertCount, 1, InFile );
   fread ( &PanelCount, sizeof PanelCount, 1, InFile );

   if ( VertCount != VCount )
      {
      fclose ( InFile );
      return 0;
      }

   // Allocate memory:
   TList = new Point3D [ VertCount ];

   if ( !Morph )
      {
      Morph = 1;
      MList = new Point3D [ VertCount ];
      }

   // Load vertices:
   for ( Count = 0; Count < VertCount; Count++ )
       {
       TList [ Count ].Read ( InFile );
       }

  // Generate morphing data to morph VList to TList:
  for ( Count = 0; Count < VertCount; Count++ )
      {
      MList [ Count ] = ( TList [ Count ] - VList [ Count ] ) /
                        Steps;
      }

  // Delete memory:
  delete [] TList;

  // Close file:
  fclose ( InFile );

  // Return success:
  return 1;
  }

void PanelObject::MorphRev ()
   {
   // Reverse the direction of the morph:
   DWORD Count;
   for ( Count = 0; Count < VCount; Count++ )
       {
       MList [ Count ] = MList [ Count ] * -1.0F;
       }
   }

void WRITE ( char *Message, float Value )
   {
   static int Opened = 0;
   FILE *File;
   if ( !Opened )
      {
      File = fopen ( "Debug.DAT", "wt" );
      Opened = 1;
      }
   else File = fopen ( "Debug.DAT", "at" );
   fprintf ( File, Message, Value );
   fclose ( File );
   }

int PanelObject::Collide ( float &X, float &Y, float &Z,
                           float Rad )
  {
  long Count;
  float Dist, X2, Y2, Z2, A, B, C, D, Sign, MinDist, NormY;
  const float NOSE = 300.0F, VIEW_HEIGHT = 700.0F;
  Y -= VIEW_HEIGHT;
  for ( Count = 0; Count < PCount; Count++ )
      {
      X2 = ( PList [ Count ].VPoint [ 0 ]->Wx +
             PList [ Count ].VPoint [ 1 ]->Wx +
             PList [ Count ].VPoint [ 2 ]->Wx +
             PList [ Count ].VPoint [ 3 ]->Wx ) / 4.0F;

      Y2 = ( PList [ Count ].VPoint [ 0 ]->Wy +
             PList [ Count ].VPoint [ 1 ]->Wy +
             PList [ Count ].VPoint [ 2 ]->Wy +
             PList [ Count ].VPoint [ 3 ]->Wy ) / 4.0F;

      Z2 = ( PList [ Count ].VPoint [ 0 ]->Wz +
             PList [ Count ].VPoint [ 1 ]->Wz +
             PList [ Count ].VPoint [ 2 ]->Wz +
             PList [ Count ].VPoint [ 3 ]->Wz ) / 4.0F;

      // Calculate distance to panel:
      Dist = sqrt ( ( X2 - X ) * ( X2 - X ) +
                    ( Y2 - Y ) * ( Y2 - Y ) +
                    ( Z2 - Z ) * ( Z2 - Z ) );
      // Check for possible collision by using bounding sphere:
      MinDist = Rad + PList [ Count ].Radius;
      if ( Dist <= MinDist )
         {
         // Confirm/reject collision warning by calculating
         // which side of the plane the viewer will be on:
         A = ( PList [ Count ].Normal.Tx -
               PList [ Count ].VPoint [ 0 ]->Wx );
         B = ( PList [ Count ].Normal.Ty -
               PList [ Count ].VPoint [ 0 ]->Wy );
         C = ( PList [ Count ].Normal.Tz -
               PList [ Count ].VPoint [ 0 ]->Wz );
         D = -( A * PList [ Count ].VPoint [ 0 ]->Wx +
                B * PList [ Count ].VPoint [ 0 ]->Wy +
                C * PList [ Count ].VPoint [ 0 ]->Wz );
         Sign = A * X + B * Y + C * ( Z + NOSE ) + D;
         if ( Sign <= 0.0F )
            {
            // Collision is definite:

            // Store the Y component of the panel normal:
            NormY = B;

            // If panel is a wall....
            if ( fabs ( NormY ) < sqrt ( 0.5F ) )
               {
               if ( A )
                  {
                  Z = ( A * X + B * Y + D ) / ( -C ) + 1.0F - NOSE;
                  X = ( B * Y + C * ( Z + NOSE ) + D ) / ( -A );
                  }
               else X = Z = 0.0F;
               }
            // Else panel is a floor or ceiling....
            else {
                 // Determine if panel is a floor:
                 if ( NormY > 0.0F )
                    {
                    Y = ( A * X + C * Z + D ) / ( - B );
                    }
                 }
            }
         }
      }
  Y += VIEW_HEIGHT;
  return 0;
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
   unsigned int short PIndex, VIndex;
   float IPer;
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
       IPer = ( ( float ) Count / ( float ) PCount ) * 101.0F;
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

int PanelObject::ReadText ( char *FileName )
  {
  long Count;
  FILE *InFile;
  if ( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
     return 0;
  // Load the textures:
  TextDat.LoadBT ( InFile );

  // Load the texture coordinates and handles:
  for ( Count = 0; Count < PCount; Count++ )
      {
      fread ( &PList [ Count ].TextHan, 
              sizeof PList [ Count ].TextHan, 1, InFile );
      fread ( PList [ Count ].Attrib, sizeof ( Detail3D ), 4, 
              InFile );
      }
  fclose ( InFile );

  return 1;
  }

int PanelObject::WriteText ( char *FileName )
  {
  long Count;
  FILE *OutFile;
  if ( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
     return 0;
  // Save the textures:
  TextDat.SaveBT ( OutFile );

  // Save the texture coordinates and handles:
  for ( Count = 0; Count < PCount; Count++ )
      {
      fwrite ( &PList [ Count ].TextHan, 
               sizeof PList [ Count ].TextHan, 1, OutFile );
      fwrite ( PList [ Count ].Attrib, sizeof ( Detail3D ), 4, 
               OutFile );
      }
  fclose ( OutFile );

  return 1;
  }
