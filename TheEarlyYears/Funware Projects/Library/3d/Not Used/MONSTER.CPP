//
// File name: Monster.CPP
//
// Description: The monster implementation
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include <StdIO.H>

#include "Monster.h"
#include "PalShade.h"
//#include "3DClass.h"
#include "LineType.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

Monster::Monster ( float Sx, float Sy, float Sz, char *IniFile,
                   int Mode )
   {
   FILE *InFile;
   long Value, Count;
   // Initialize the position and direction vector:
   Pos.Lx = Sx; Pos.Ly = Sy; Pos.Lz = Sz;
   Dir.Lx = Sx; Dir.Ly = Sy; Dir.Lz = Sz + 10.0F; // Point down Z
   Width = Height = FrameNum = FrameCount = 0;
   Health = 100;
   Frame = NULL;
   char String [ 256 ];
   if ( Mode == ASCII )
      {
      if ( ( InFile = fopen ( IniFile, "rt" ) ) == 0 )
         return;
      Value = fscanf ( InFile, "%s", String );
      while ( ( Value != EOF ) && ( Value != 0 ) )
            {
            ++FrameCount;
            Value = fscanf ( InFile, "%s", String );
            }
      rewind ( InFile );
      Frame = new LoadTexture [ FrameCount ];
      for ( Count = 0; Count < FrameCount; ++Count )
          {
          fscanf ( InFile, "%s", String );
          Frame [ Count ].Load ( String );
          }
      Width  = Frame [ 0 ].Width;
      Height = Frame [ 0 ].Height;
      }
   if ( Mode == Binary )
      {
      }
   }

BOOL Monster::Show ( Matrix3D &M, BYTE *Dest )
   {
   float Px, Py, Pw, Ph, Ratio;
   long TexLeft = 0, TexTop = 0, Tx, Ty,
        Left, Top, Right, Bottom, TIndex,
        DIndex, Z;
   unsigned int Shade;
   BYTE *Texture = Frame [ FrameNum ].Image, Color;
   // Transform the position and direction vectors:
   M.Transform ( Pos );
   M.Transform ( Dir );
   if ( Pos.Wz > MAXZ )
      return FALSE;
   Shade = Pos.Wz / SHADE_DIV;
   if ( Shade > ( SHADE_COUNT - 1 ) )
      Shade = ( SHADE_COUNT - 1 );
   Shade = Shade << 8;
   if ( Pos.Wz > MINZ )
      {
      Z = ( 1.0F / float ( Pos.Wz ) ) * float ( 1 << ZSTEP_PREC );
      Z += ZTrans;
      Px = Pos.Wx * XSCALE / Pos.Wz + XCENTER;
      Py = Pos.Wy * YSCALE / Pos.Wz + YCENTER;
      Pw = ( float ) Width  * fabs ( XSCALE ) * 10.0F / Pos.Wz;
      Ph = ( float ) Height * fabs ( YSCALE ) * 10.0F / Pos.Wz;
      Left   = Px - ( Pw / 2.0F );
      Right  = Px + ( Pw / 2.0F );
      Top    = Py - ( Ph / 2.0F );
      Bottom = Py + ( Ph / 2.0F );
      // Take care of the easy clipping first:
      if ( Right > MAXX )
         Right = MAXX;
      if ( Bottom > MAXY )
         Bottom = MAXY;
      // Make sure the monster is on the screen:
      if ( Right < MINX )
         return FALSE;
      if ( Bottom < MINY )
         return FALSE;
      if ( Left >= MAXX )
         return FALSE;
      if ( Top >= MAXY )
         return FALSE;
      // Take care of the hard clipping next:
      if ( Left < MINX )
         {
         Ratio = ( float ) Width  / Pw;
         TexLeft += ( MINX - Left ) * Ratio;
         Left = MINX;
         }
      if ( Top < MINY )
         {
         Ratio = ( float ) Height / Ph;
         TexTop += ( MINY - Top ) * Ratio;
         Top = MINY;
         }
      long U, V, UStep, VStep;
      V = ( TexTop << 16 );
      UStep = ( Width  << 16 ) / Pw;
      VStep = ( Height << 16 ) / Ph;
      for ( Ty = Top; Ty < Bottom; Ty++ )
          {
          U = ( TexLeft << 16 );
          DIndex = Ty * SCREEN_WIDTH + Left;
          TIndex = ( V >> 16 ) * Width;
          for ( Tx = Left; Tx < Right; Tx++ )
              {
              if ( ZBuffer [ DIndex ] < Z )
                 {
                 Color  = Texture [ TIndex + ( U >> 16 ) ];
                 if ( Color )
                    {
                    Dest [ DIndex ] = TextShade.Shade [ Shade + Color ];
                    }
                 }
              U += UStep;
              ++DIndex;
              }
          V += VStep;
          }
      }
   return TRUE;
   }

void Monster::MoveToward ( float Tx, float Ty, float Tz,
                           float Steps, Matrix3D &M )
   {
   Tx; Ty; Tz; Steps;
   Point3D P;
   M.Transform ( Pos );
   P.Wx = Pos.Wx + ( Tx - Pos.Wx ) / Steps;
   P.Wy = Pos.Wy + ( Ty - Pos.Wy ) / Steps; // Change the monster's height
   P.Wz = Pos.Wz + ( Tz - Pos.Wz ) / Steps;
   M.Untransform ( P );
   Pos.Lx = P.Lx;
   Pos.Ly = P.Ly;
   Pos.Lz = P.Lz;
   }
