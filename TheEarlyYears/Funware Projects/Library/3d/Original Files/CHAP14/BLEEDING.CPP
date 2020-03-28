//
// File name: Bleeding.CPP
//
// Description: The main file for a simple 3D game
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
//#include <Time.H>
#include <Stdio.H>
#include <Windows.H>
#include <Iostream.H>

// ------------------------------------------------------------
// | Local include files:                                     |
// ------------------------------------------------------------

#include "TextType.HPP"
#include "3Dclass.HPP"
#include "LineType.HPP"
#include "PalShade.HPP"
#include "Monster.HPP"
#include "FastWin.HPP"

#include "Bleeding.RH"

// ------------------------------------------------------------
// | Global variables/constants:                              |
// ------------------------------------------------------------

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,
                          unsigned int wParam, LONG lParam );

HANDLE ThisInstance;
long RightButton = FALSE, LeftButton = FALSE, 
     Keys, Dir, Mag;
BOOL Running = TRUE;

const long SCREEN_WIDTH = 320;
const long SCREEN_HEIGHT = 200;

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


// ------------------------------------------------------------
// | Program entry:                                           |
// ------------------------------------------------------------

int WINAPI WinMain ( HANDLE hInstance, HANDLE hPrevInstance,
		    LPSTR lpszCmdParam, int nCmdShow )

  {
  hPrevInstance; lpszCmdParam;
  ThisInstance = hInstance;
  HWND hWnd;
  MSG Message;
  WNDCLASS WndClass;
  Matrix3D M, Copy; View V;
  unsigned char *VidBuf;
  Monster Mon ( 0.0, 400.0, 2000.0, "Mon.INI", Monster::ASCII );
  long MaxWait, BackC, FrameNum = 0, OldXRot = 0,
       Looking = FALSE, FrameCount = 0, NoSound = 0;
  ZTrans = 0;
  FastWin MyWin;

  // Randomize the timer for sound events:
  randomize ();

  // Allocate memory for a panel object:
  PanelObject *World = new PanelObject;

  // Allocate memory for Z-buffer:
  InitZBuffer ();

  // Read the panel data:
  World->ReadBIN ( "Test.BIN" );

  // Read -- if possible -- the texture data:
  if ( World->ReadText ( "Text.BT" ) == FALSE )
     {
     if ( TextDat.LoadINI ( "Text.INI" ) == FALSE )
        {
        delete World;
        return 0;
        }
     // Initialize default texture coords:
     World->InitDefText ();
     }
  // Read -- if possible -- the shade table:
  if ( TextShade.LoadTable ( "Shade.TAB" ) == FALSE )
     {
     // Generate a palette that fades to white:
     TextShade.GenTable ( 150.0F, 150.0F, 150.0F,
                          TextDat.TMap [ 0 ].Palette );
     TextShade.SaveTable ( "Shade.TAB" );
     }

  MyWin.Init ( 320, 200, TextDat.TMap [ 0 ].Palette );
  VidBuf = ( unsigned char * ) MyWin;

  // Raise the viewer and move him/her forward:
  M.Initialize ();
  M.Translate ( 0, -700, -500 );
  ClearBuffer ();
  World->Display ( M, VidBuf );
  MaxWait = ( long ) pow ( 2, 31 - ZSTEP_PREC  );

  // Calculate the background color (a shade of whitish-gray):
  BackC = GetColor ( 150, 150, 150, TextDat.TMap [ 0 ].Palette );

  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hbrBackground = GetStockObject ( GRAY_BRUSH );
  WndClass.hCursor =  LoadCursor ( NULL, IDC_ARROW );
  WndClass.hIcon = LoadIcon ( hInstance, NULL );
  WndClass.hInstance = hInstance;
  WndClass.lpfnWndProc = WndProc;
  WndClass.lpszClassName = "BLEEDING";
  WndClass.lpszMenuName = ( const char * ) MENU_1;
  WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

  if ( !RegisterClass ( &WndClass ) )
     return 0;           

  hWnd = CreateWindow ( "BLEEDING",             // class name
                        "The Bleeding Edge Of 3D Game Programming",  // Caption
                        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,// Style
                        CW_USEDEFAULT,           // x position
                        CW_USEDEFAULT,           // y position
                        320,                     // cx - size
                        240,                     // cy - size
                        NULL,                    // Parent window
                        NULL,                    // Menu
                        hInstance,               // Program Instance
                        NULL );                  // Parameters

  ShowWindow ( hWnd, nCmdShow );

  MyWin.Colors ();

  while ( Running )
        {
        // Process frame:
        if ( LeftButton == TRUE )
           {
           if ( Dir < 0 )
              {
              V.YRot = fabs ( Dir ) / 10.0;
              }
           else {
                V.YRot = -fabs ( Dir ) / 10.0;
                }
           if ( Mag < 0 )
              {
              V.ZPos -= fabs ( Mag ) / 5.0F;
              }
           else {
                V.ZPos += fabs ( Mag ) / 5.0F;
                }
           }
        if ( RightButton == TRUE )
           {
           V.ZRot = float ( Dir );
           V.XRot = -float ( Mag );
           }

        if ( V.XRot != 0 )
           {
           if ( !Looking )
              {
              Looking = TRUE;
              Copy = M;
              }
           M.Rotate ( OldXRot, 0, 0 );
           M.Rotate ( -V.XRot, 0, 0 );
           OldXRot = V.XRot;
           }
        else if ( RightButton == FALSE ) 
           {
           // Level the player by using matrix copy:
           if ( Looking == TRUE )
              {
              M = Copy;
              Looking = FALSE;
              }

           // Simulate gravity:
           V.YPos = -100.0F;

           // Perform collision detection:
           World->Collide ( V.XPos, V.YPos, V.ZPos, 100.0F );

           // Update the world:
           M.Translate ( -V.XPos, -V.YPos, -V.ZPos );
           M.Rotate ( 0, -V.YRot, 0 );

           OldXRot = 0;
           }

        V.Clear ();

        // Move the monster:
        Mon.MoveToward ( 0.0F, -150.0F, 0.0F, 100.0F, M );

        // Change (if necessary) to the next monster frame:
        if ( ( FrameCount % 4 ) == 0 )
           {
           ++FrameNum;
           if ( Mon.Dist () > 900.0F )
              {
              if ( FrameNum > 8 )
                 FrameNum = 0;
              }
           else {
                if ( Mon.Dist () < 500 )
                   {
                   if ( !NoSound )
                      {
                      if ( random ( 100 ) >= 50 )
                         sndPlaySound ( "Pain1.WAV", SND_ASYNC	);
                      else sndPlaySound ( "Pain2.WAV", SND_ASYNC );
                      NoSound = 20;
                      }
                   }
                if ( FrameNum > 11 )
                   FrameNum = 9;
                }
           if ( NoSound > 0 )
              --NoSound;
           Mon = FrameNum;
           }

        memset ( VidBuf, BackC, 64000 );

        // Clear the Z-buffer (if necessary):
        ZTrans += ( 1 << ZSTEP_PREC );
        if ( ( FrameCount % MaxWait ) == 0 )
           {
           ZTrans = 0;
           ClearBuffer ();
           }

        World->Display ( M, VidBuf );
        Mon.Show ( M, VidBuf );

        MyWin.Show ();

        if ( PeekMessage ( &Message, hWnd, 0, 0, PM_NOREMOVE ) == TRUE )
           {
           Running = GetMessage ( &Message, hWnd, 0, 0 );
           TranslateMessage ( &Message );
           DispatchMessage ( &Message );
           }

        ++FrameCount;
		Sleep(7);
        }

  // Destroy the Z buffer:
  DestZBuffer ();

  // Save the texture data:
  World->WriteText ( "Text.BT" );

  // Deallocate memory for panel object:
  delete World;

  return Message.wParam;
  }

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,
                          unsigned int wParam, LONG lParam )
   {
   float XPos, YPos;
   switch ( iMessage )
          {
          case ( WM_COMMAND ):
               {
               switch ( LOWORD ( wParam ) )
                      {
                      case ( CM_FILE_EXIT ):
                           {
                           Running = FALSE;
                           break;
                           }
                      }
               break;
               }
          case ( WM_LBUTTONUP ):
               {
               ReleaseCapture ();
               LeftButton = FALSE;
               break;
               }
          case ( WM_LBUTTONDOWN ):
               {
               SetCapture ( hWnd );
               LeftButton = TRUE; RightButton = FALSE;
               Keys = wParam; Keys;       // key flags                      
               XPos = float ( ( short int ) ( LOWORD ( lParam ) ) );
               YPos = float ( ( short int ) ( HIWORD ( lParam ) ) );
               Dir  = ( SCREEN_WIDTH / 2 )  - XPos;
               Mag  = ( SCREEN_HEIGHT / 2 ) - YPos;
               break;
               }
          case ( WM_RBUTTONUP ):
               {
               ReleaseCapture ();
               RightButton = FALSE;
               break;
               }
          case ( WM_RBUTTONDOWN ):
               {
               SetCapture ( hWnd );
               RightButton = TRUE; LeftButton = FALSE;
               Keys = wParam; Keys;
               XPos = float ( ( short int ) ( LOWORD ( lParam ) ) );
               YPos = float ( ( short int ) ( HIWORD ( lParam ) ) );
               Dir  = ( SCREEN_WIDTH / 2 )  - XPos;
               Mag  = ( SCREEN_HEIGHT / 2 ) - YPos;
               break;
               }
          case ( WM_MOUSEMOVE ):
               {
               if ( LeftButton == TRUE )
                  {
                  Keys = wParam; Keys;       // key flags                      
                  XPos = float ( ( short int ) ( LOWORD ( lParam ) ) );
                  YPos = float ( ( short int ) ( HIWORD ( lParam ) ) );
                  Dir  = ( SCREEN_WIDTH / 2 )  - XPos;
                  Mag  = ( SCREEN_HEIGHT / 2 ) - YPos;
                  }
               if ( RightButton == TRUE )
                  {
                  Keys = wParam; Keys;
                  XPos = float ( ( short int ) ( LOWORD ( lParam ) ) );
                  YPos = float ( ( short int ) ( HIWORD ( lParam ) ) );
                  Dir  = ( SCREEN_WIDTH / 2 )  - XPos;
                  Mag  = ( SCREEN_HEIGHT / 2 ) - YPos;
                  break;
                  }
               break;
               }
          case ( WM_DESTROY ):
               {
               Running = FALSE;
               break;
               }
          default:
                 return DefWindowProc ( hWnd, iMessage, wParam,
                                      lParam );
          }
   return 0;
   }
