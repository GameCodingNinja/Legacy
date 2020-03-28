//
// File name: Collide.CPP
//
// Description: The main file for a collision detection program
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

// ------------------------------------------------------------
// | Global include files:                                    |
// ------------------------------------------------------------

#include <Dos.H>
#include <Math.H>
#include <Conio.H>
#include <Time.H>
#include <Stdio.H>
#include <Windows.H>
#include <Iostream.H>

extern const int SCREEN_TOTAL;

// ------------------------------------------------------------
// | Local include files:                                     |
// ------------------------------------------------------------

#include "32Bit.h"
#include "MouseLib.h"
//#include "Palette.h" not being used

#ifndef TEXTTYPEHPP
  #define TEXTTYPEHPP
  #include "TextType.h"
#endif

#ifndef _3DCLASSHPP
  #define _3DCLASSHPP
  #include "3Dclass.h"
#endif

#ifndef LINETYPEHPP
  #define LINETYPEHPP
  #include "LineType.h"
#endif

#ifndef PALSHADEHPP
  #define PALSHADEHPP
  #include "PalShade.h"
#endif

// ------------------------------------------------------------
// | Global variables/constants:                              |
// ------------------------------------------------------------

// Create an image of a cross for the mouse:
unsigned char Cursor [ ] = {
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     10,10,10,10,10,11,11,10,10,10,10,10,
                     10,10,10,10,10,11,11,10,10,10,10,10,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0,
                     };

// ------------------------------------------------------------
// | Local structs/classes:                                   |
// ------------------------------------------------------------

struct View {
int XRot, YRot, ZRot;
float XPos, YPos, ZPos;
View () { XRot = YRot = ZRot = 0;
          XPos = YPos = ZPos = 0.0F; }
void Clear () { XRot = YRot = ZRot = 0; 
                XPos = YPos = ZPos = 0.0F; }
};

// ------------------------------------------------------------
// | Function section:                                        |
// ------------------------------------------------------------

int InitZBuffer ()
  {
  // Allocate memory for Z-buffer:
  if ( ( ZBuffer = new long [ SCREEN_TOTAL ] ) == 0 )
     return 0;
  return 1;
  }

int DestZBuffer ()
  {
  // Deallocate Z-buffer's memory:
  delete [] ZBuffer;
  if ( ZBuffer )
     return 0;
  return 1;
  }

void ClearBuffer ()
   {
   // Clear the Z-buffer:
   long *ZPtr = ZBuffer;
   for( int Index = 0; Index < 6400; ++Index )
       {
       *ZPtr++ = 0;
       *ZPtr++ = 0;

       *ZPtr++ = 0;
       *ZPtr++ = 0;

       *ZPtr++ = 0;
       *ZPtr++ = 0;

       *ZPtr++ = 0;
       *ZPtr++ = 0;

       *ZPtr++ = 0;
       *ZPtr++ = 0;
       }
   }

// Create a virtual track-ball:
void UpdatePos ( PanelObject *World, MousePtr &Mouse, View &V )
   {
   int X, Y;
   static int Px = 0, Py = 0, Pressed = 0, Lx, Ly;

   // Get the mouse coordinates and map them to a suitable
   // range:
   X = ( Mouse.GetX () - 50 ) >> 2;
   Y = ( Mouse.GetY () - 50 ) >> 2;

   // If the left button is pressed:
   if ( ( Mouse.GetLb () ) && ( !Mouse.GetRb () ) )
      {
      // Do the translations and rotations:
      V.ZPos -= ( float ) Y * 3.0F;
      V.YRot += X;
      Pressed = 0;
      }

   // Else if the right button is pressed:   
   else if ( ( Mouse.GetRb () ) && ( ! Mouse.GetLb () ) )
           {
           V.XRot += Y;
           //V.ZRot -= X;
           Pressed = 0;
           }

   // Else if both buttons are pressed:
   else if ( Mouse.GetRb () && Mouse.GetLb () )
           {
           if ( Pressed == 0 )
              {
              Pressed = 1;
              // Record the actual screen coordinates of the 
              // mouse pointer:
              Px = Mouse.GetSx ();
              Py = Mouse.GetSy ();

              Lx = Mouse.GetSx ();
              Ly = Mouse.GetSy ();
              }
           else {
                World->MoveText ( Px, Py, 
                                  ( Lx - Mouse.GetSx () ) >> 2, 
                                  ( Ly - Mouse.GetSy () ) >> 2 );
                Lx = Mouse.GetSx ();
                Ly = Mouse.GetSy ();
                }
           }

   // Else no buttons are pressed:
   else Pressed = 0;
   }

void HandleKey ( PanelObject *World, MousePtr &Mouse, long Key )
   {
   switch ( Key )
          {
          case ('\t'):
               {
               World->SelNext ();
               break;
               }
          case ('t'):
          case ('T'):
               {
               World->NexText ();
               break;
               }
          case ('r'):
          case ('R'):
               {
               World->RotText ();
               break;
               }
          case ('i'):
          case ('I'):
               {
               World->NextInten ( Mouse.GetSx (), 
                                  Mouse.GetSy () );
               break;
               }
          default: break;
          }
   }

float RunWorld ( unsigned char *VidMem, unsigned char *VidBuf,
                  PanelObject *World )
   {
   // Enter the main loop, updating view as necessary:

   VidMem; VidBuf; World;
   MousePtr Mouse;
   Matrix3D M; View V;
   long QuitFlag = 0, StartTime, EndTime, FrameCount = 0, 
        MaxWait, Input, BackC;
   float FramesPerSecond;
   ZTrans = 0;

   // Initialize the mouse driver:
   Mouse.Init ();

   // Hide the pointer:   
   Mouse.Hide ();

   // Re-map the cursor's coordinates:
   Mouse.MappingRange ( 100, 100 );

   // Clip the cursor to a rectangular region:
   Mouse.Clip ( 5, 5, 95, 95 );

   // Give the cursor a face-lift:
   Mouse.ChangeCursor ( Cursor, 12, 12 );

   // Allocate memory for Z-buffer:
   InitZBuffer ();

   // Raise the viewer:
   M.Initialize ();
   M.Translate ( 0, -700, 0 );
   ClearBuffer ();
   World->Display ( M, VidBuf );
   MaxWait = ( long ) pow ( 2, 31 - ZSTEP_PREC  );

   // Record the time:
   StartTime = clock ();

   // Calculate the background color (a shade of whitish-gray):
   BackC = GetColor ( 150, 150, 150, TextDat.TMap [ 0 ].Palette );

   // Loop until ESC pressed:
   while ( !QuitFlag )
         {
         UpdatePos ( World, Mouse, V );

         // Simulate gravity:
         V.YPos = -100.0F;

         // Perform collision detection:
         World->Collide ( V.XPos, V.YPos, V.ZPos, 100.0F );

         M.Translate ( -V.XPos, -V.YPos, -V.ZPos );
         M.Rotate ( -V.XRot, -V.YRot, -V.ZRot );
         V.YPos = 0.0F;
         V.Clear ();

         setmem ( VidBuf, SCREEN_TOTAL, BackC );

         // Clear the Z-buffer (if necessary):
         ZTrans += ( 1 << ZSTEP_PREC );
         if ( ( FrameCount % MaxWait ) == 0 )
            {
            ZTrans = 0;
            ClearBuffer ();
            }

         World->Display ( M, VidBuf );
         Mouse.Display ( VidBuf );

         memmove ( VidMem, VidBuf, SCREEN_TOTAL );
         if ( kbhit () )
            {
            Input = getch ();
            if ( Input == 27 )
               QuitFlag = 1;
            else HandleKey ( World, Mouse, Input );
            }
         ++FrameCount;
         }
   EndTime = clock ();

   DestZBuffer ();

   // Calculate the frames per second:
   FramesPerSecond = ( float ) FrameCount * 
                     ( float )  CLK_TCK /
                     ( float ) ( EndTime - StartTime ); 
   return FramesPerSecond;
   }

void SetPalette ( RGBQUAD *Palette )
   {
   int Index, Red, Green, Blue;
   for ( Index = 0; Index < 256; ++Index )
       {
       Red    = ( int ) ( Palette [ Index ].rgbRed   >> 2 );
       Green  = ( short ) ( Palette [ Index ].rgbGreen >> 2 );
       Blue   = ( short ) ( Palette [ Index ].rgbBlue  >> 2 );
       setpalreg ( Index, Green, Blue, Red );
       }
   }


// ------------------------------------------------------------
// | Program entry:                                           |
// ------------------------------------------------------------

void main ( int ArgCount, char *Arg[] )
   {
   // Declare/initialize variables and allocate memory:
   unsigned char *VidMem, *VidBuf;
   VidMem = VideoAddress ();
   float FramesPerSecond;

   // Allocate buffer memory - abort if error:
   if ( ( VidBuf = new unsigned char [ SCREEN_TOTAL ] ) == NULL )
      return;

   // Allocate memory for a panel object:
   PanelObject *World = new PanelObject;

   if ( ( ArgCount > 1 ) && 
        ( !strcmp ( strupr ( Arg [ 1 ] ), "WRITEBINARY" ) ) )
      {
      World->LoadDXF ( "Test.DXF" );
      World->WriteBIN ( "Test.BIN" );
      }

   else {
        // Read the panel data:
        World->ReadBIN ( "Test.BIN" );

        // Read -- if possible -- the texture data:
        if ( World->ReadText ( "Text.BT" ) == 0 )
           {
           if ( TextDat.LoadINI ( "Text.INI" ) == 0 )
              {

              delete [] VidBuf;
              delete World;
              return;
              }
           // Initialize default texture coords:
           World->InitDefText ();
           }
        // Read -- if possible -- the shade table:
        if ( TextShade.LoadTable ( "Shade.TAB" ) == 0 )
           {
           // Generate a palette that fades to white:
           TextShade.GenTable ( 150.0F, 150.0F, 150.0F, 
                                TextDat.TMap [ 0 ].Palette );
           TextShade.SaveTable ( "Shade.TAB" );
           }

        // Set video mode:
        SetVideo ( 0x13 );

        // Set the palette:
        SetPalette ( TextDat.TMap [ 0 ].Palette );

        // Enter main loop:
        FramesPerSecond = RunWorld ( VidMem, VidBuf, World );

        // Set video mode:
        SetVideo ( 0x03 );

        // Save the texture data:
        World->WriteText ( "Text.BT" );
        }

   // Deallocate memory for buffer:
   delete [] VidBuf;
      
   // Deallocate memory for panel object:
   delete World;

   // Exit program:
   return;
   }
