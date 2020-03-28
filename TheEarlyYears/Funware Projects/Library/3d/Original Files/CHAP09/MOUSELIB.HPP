//
// File name: MouseLib.HPP
//
// Description: Class declaration for a mouse object
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef MOUSELIBHPP
#define MOUSELIBHPP

#include <Dos.H>
#include <String.H>

int ResetMouse ( void );
void ShowMouse ( void );
void HideMouse ( void );
void SetMousePos ( int X, int Y );
void GetMousePos ( int &X, int &Y );
void GetButtonPos ( int &Lb, int &Rb );

class MousePtr {
protected:
  int X, Y, ClipFlag, MapFlag, LeftB, RightB;
  int X1, Y1, X2, Y2, RangeX, RangeY, XCenter, YCenter;
  int CorrectIfMap, Width, Height;
  unsigned char *CImage;
  void CorrectClip ( int Left, int Top, int Right, int Bottom )
     {
     X1 = ( Left * 638 / RangeX );
     X2 = ( Right * 638 / RangeX );
     Y1 = ( Top * 199 / RangeY );
     Y2 = ( Bottom * 199 / RangeY );
     }
  void ClipPoint ( int &Sx, int &Sy )
     {
     if ( Sx < 0 )
        Sx = 0;
     if ( Sx > 319 )
        Sx = 319;
      
     if ( Sy < 0 )
        Sy = 0;
     if ( Sy > 199 )
        Sy = 199;
     }
public:
  MousePtr ()
     {
     CorrectIfMap = ClipFlag = 0; 
     MapFlag = LeftB = RightB = 0;
     Width = Height = X = Y = 0;
     XCenter = YCenter = 0;
     CImage = NULL;
     }
  int Init () { return ResetMouse (); }
  void Update ()
     {
     GetMousePos ( X, Y );
     if ( ClipFlag )
        {
        if ( X < X1 )
           X = X1;
        if ( X > X2 )
           X = X2;
        if ( Y < Y1 )
           Y = Y1;
        if ( Y > Y2 )
           Y = Y2;
        SetMousePos ( X, Y );   
        }
     GetButtonPos ( LeftB, RightB );
     }
  void MappingRange ( int Xrange, int Yrange )
     {
     MapFlag = 1;
     RangeX = Xrange;
     RangeY = Yrange;
     if ( CorrectIfMap )
        {
        CorrectIfMap = 0;
        CorrectClip ( X1, Y1, X2, Y2 );
        }
     }
  int GetX ()
    {
    Update ();
    if ( MapFlag )
       return X * RangeX / 638;
    return X;
    }
  int GetY ()
    {
    Update ();
    if ( MapFlag )
       return Y * RangeY / 199;
    return Y;
    }
  int GetLb ()
    {
    Update ();
    return LeftB;
    }
  int GetRb ()
    {
    Update ();
    return RightB;
    }
  void Show ()
     {
     ShowMouse ();
     }
  void Hide ()
     {
     HideMouse ();
     }   
  void SetXY ( int X, int Y )
     {
     SetMousePos ( X, Y );
     }
  void ChangeCursor ( unsigned char *Cursor, int CWidth, 
                       int CHeight, int XC = 0, int YC = 0 )
     {
     Width = CWidth;
     Height = CHeight;
     CImage = Cursor;
     XCenter = XC;
     YCenter = YC;
     }                  
  void Clip ( int Left, int Top, int Right, int Bottom )
     {
     ClipFlag = 1;
     if ( MapFlag )
        {
        CorrectClip ( Left, Top, Right, Bottom );
        }
     else {   
          CorrectIfMap = 1;
          X1 = Left; X2 = Right;
          Y1 = Top; Y2 = Bottom;
          }
     }
  void Display ( unsigned char *Buffer = NULL );
};

#endif
