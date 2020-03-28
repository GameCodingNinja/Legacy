/************************************************************************
*
*    PRODUCT:         Art 4 Sale
*
*    FILE NAME:       TStartUpScrn.cpp
*
*    DESCRIPTION:     Start up screen
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "minTools.h"        // A misc group of useful functions
#include "TStartUp.h"        // Header file for this *.cpp file.
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


// Globals
PTStartUpDlg pStartUpDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayStartUpScreen                                                             
*
*    DESCRIPTION:           Shows a dialog box to display the startup screen
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to parent window
*              char *name - Name of graphic resource
*              char *type - Name of graphic type
*              BYTE *buffer - Pointer to a buffer containing bitmap data.
*                             If you can't store the image in a resource then
*                             you would pass the pointer to the bitmap data
*
*    Output:   returntype - none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

HWND DisplayStartUpScreen( HINSTANCE hInst, HWND hwnd, char *pResName, char *pResType, BYTE *buffer )
{
    HWND Result = NULL;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pStartUpDlg = new TStartUpDlg();

    // Create the dialog box
    if( pStartUpDlg )
        Result = pStartUpDlg->CreateDialogWnd( hInst, hwnd, pResName, pResType, buffer );

    return Result;

}   // DisplayStartUpScreen


/************************************************************************
*    FUNCTION NAME:         StartUpDlgProc
*
*    DESCRIPTION:           Standard Windows dialog message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of the window receiving the message
*              UINT uMsg      - Message being sent
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK StartUpDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pStartUpDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_PAINT,      pStartUpDlg->OnPaint );
        HANDLE_MSG( hDlg, WM_DESTROY,    pStartUpDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pStartUpDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // StartUpDlgProc


/************************************************************************
*
*                About Dialog Object Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TStartUpDlg::TStartUpDlg()
{
}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TStartUpDlg::~TStartUpDlg()
{
}   // Destructer



/************************************************************************
*    FUNCTION NAME:         CreateDialogWnd()                                                             
*
*    DESCRIPTION:           Create the dialog Window
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

HWND TStartUpDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *pResName, char *pResType, BYTE *buffer )
{
    hParentWnd = hwnd;
    BOOL Result = FALSE;
    TGif StartUpGIF;

    if( pResName && pResType )
    {
        // If this is a gif resource, decode and load the resource
        if( strcmp( pResType, "GIF" ) == 0 )
            Result = StartUpDib.LoadFromBuffer( StartUpGIF.LoadFromResource( pResName, pResType, hInst ) );

        // If this is a bitmap resource, load the resource
        else if( pResType == RT_BITMAP || (strcmp( pResType, "BMP" ) == 0) )
            Result = StartUpDib.LoadFromResource( hInst, pResName, pResType );
    }
    // If we are sending a pointer to the buffer, then load it
    else if( buffer )
        Result = StartUpDib.LoadFromBuffer( buffer );
        
    // Create the dialog box if we have a bitmap
    if( Result )
        return CreateDialog( hInst, "DLG_STARTUP_SCRN", hwnd, (DLGPROC)StartUpDlgProc );
    else
        return NULL;

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         StartUpDlg_OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
*                           Here we are creating a window to paint our frames
*                           on to and a double buffer to compose our frames.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStartUpDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    BOOL Result = FALSE;

    // Center the window using the size of the bitmap
    if( !StartUpDib.IsEmpty() )
    {
        CenterWindow( hwnd, StartUpDib.width(), StartUpDib.height(), hParentWnd );
        Result = TRUE;
    }

    return Result;

}   // StartUpDlg



/************************************************************************
*    FUNCTION NAME:         StartUpDlg_OnPaint                                                             
*
*    DESCRIPTION:           Standard Windows OnPaint message handler.    
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStartUpDlg::OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Blit the graphic to the screen
    // The class also takes care of the realizing the palette if in 256 colors
    StartUpDib.BlitToDC( PaintDC );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // StartUpDlg_OnPaint


/************************************************************************
*    FUNCTION NAME:         StartUpDlg_OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*                           You could free memory and do other clean up
*                           but it is best to keep it all in the OnClose
*                           message handler
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStartUpDlg::OnDestroy( HWND hwnd ) 
{
    StartUpDib.Free();

} // OnDestroy