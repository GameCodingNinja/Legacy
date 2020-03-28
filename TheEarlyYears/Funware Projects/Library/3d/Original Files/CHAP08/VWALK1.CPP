//
// File name: Vwalk1.CPP
//
// Description: The main file for a virtual walk-through using 
//              solid colored polygons
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
#include <Iostream.H>

// ------------------------------------------------------------
// | Local include files:                                     |
// ------------------------------------------------------------

#include "32Bit.HPP"
#include "MouseLib.HPP"
#include "Palette.HPP"

#include "3Dclass.HPP"
#include "LineType.HPP"

// ------------------------------------------------------------
// | Global variables/constants:                              |
// ------------------------------------------------------------

// Create an image of a cross for the mouse:
unsigned char Cursor [ ] = {
                     0, 0, 0, 0, 0,17,17, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,18,18, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,19,19, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,20,20, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,21,21, 0, 0, 0, 0, 0,
                     17,18,19,20,21,22,22,21,20,19,18,17,
                     17,18,19,20,21,22,22,21,20,19,18,17,
                     0, 0, 0, 0, 0,21,21, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,20,20, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,19,19, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,18,18, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0,17,17, 0, 0, 0, 0, 0,
                     };

// ------------------------------------------------------------
// | Local structs/classes:                                   |
// ------------------------------------------------------------

struct View {
int XRot, YRot, ZRot;
double ZPos;
View () { XRot = YRot = ZRot = 0; ZPos = 0.0F; }
void Clear () { XRot = YRot = ZRot = 0; ZPos = 0.0F; }
};

// ------------------------------------------------------------
// | Function section:                                        |
// ------------------------------------------------------------

int InitZBuffer ()
  {
  // Allocate memory for Z-buffer:
  if ( ( ZBuffer = new long [ 64000 ] ) == 0 )
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
   for ( unsigned int Index = 0; Index < 6400; Index++ )
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
void UpdatePos ( MousePtr &Mouse, View &V )
   {
   // Get the mouse coordinates and map them to a suitable
   // range:
   int X = ( Mouse.GetX () - 50 ) >> 2;
   int Y = ( Mouse.GetY () - 50 ) >> 2;

   // If the left button is pressed:
   if ( ( Mouse.GetLb () ) && ( !Mouse.GetRb () ) )
      {
      // Do the translations and rotations:
      V.ZPos -= ( double ) Y * 3.0F;
      V.YRot += X;
      }

   // Else if the right button is pressed:   
   else if ( ( Mouse.GetRb () ) && ( ! Mouse.GetLb () ) )
           {
           V.XRot += Y;
           V.ZRot -= X;
           }
   }

double RunWorld ( unsigned char *VidMem, unsigned char *VidBuf,
                  PanelObject *World )
   {
   // Enter the main loop, updating view as necessary:

   VidMem; VidBuf; World;
   MousePtr Mouse;
   Matrix3D M; View V;
   long QuitFlag = 0, StartTime, EndTime, FrameCount = 0, MaxWait;
   double FramesPerSecond;
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
   M.Translate ( 0, -600, 0 );
   ClearBuffer ();
   World->Display ( M, VidBuf );
   MaxWait = ( long ) pow ( 2, 31 - ZSTEP_PREC  );

   StartTime = clock ();

   // Loop until ESC pressed:
   while ( !QuitFlag )
         {
         UpdatePos ( Mouse, V );

         M.Translate ( 0, 0, -V.ZPos );
         M.Rotate ( -V.XRot, -V.YRot, -V.ZRot );
         V.Clear ();

         setmem ( VidBuf, 64000, 1 );

         // Clear the Z-buffer (if necessary):
         ZTrans += ( 1 << ZSTEP_PREC );
         if ( ( FrameCount % MaxWait ) == 0 )
            {
            ZTrans = 0;
            ClearBuffer ();
            }

         World->Display ( M, VidBuf );
         Mouse.Display ( VidBuf );

         memmove ( VidMem, VidBuf, 64000 );
         if ( kbhit () )
            {
            if ( getch () == 27 )
               QuitFlag = 1;
            }
         ++FrameCount;
         }
   EndTime = clock ();

   DestZBuffer ();

   // Calculate the frames per second:
   FramesPerSecond = ( double ) FrameCount * 
                     ( double )  CLK_TCK /
                     ( double ) ( EndTime - StartTime ); 
   return FramesPerSecond;
   }

void SetPalette ()
   {
   short int Scale;
   for ( short int N = 0; N < 256; N++ )
       {
       Scale = N >> 2;
       SetPalReg ( N, Scale, Scale, Scale );
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
   double FramesPerSecond;

   // Allocate buffer memory - abort if error:
   if ( ( VidBuf = new unsigned char [ 64000 ] ) == NULL )
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
        World->ReadBIN ( "Test.BIN" );

        // Set video mode:
        SetVideo ( 0x13 );

        // Set the palette to shades of gray:
        SetPalette ();

        //CommitStack ();

        // Enter main loop:
        FramesPerSecond = RunWorld ( VidMem, VidBuf, World );

        // Set video mode:
        SetVideo ( 0x03 );

        // Display FPS:
        cout << "\nFrames per second: " << FramesPerSecond;
        }

   // Deallocate memory/prepare to shut down:
   cout << "\nDeallocating memory...";
   delete [] VidBuf;
      
   // Deallocate memory for panel object:
   delete World;

   cout << "\nReturning control to OS...";

   // Exit program:
   return;
   }
