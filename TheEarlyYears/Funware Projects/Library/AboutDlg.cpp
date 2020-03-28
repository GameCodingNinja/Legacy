/************************************************************************
*
*    PRODUCT:         About Dialog
*
*    FILE NAME:       DlgAbout.cpp
*
*    DESCRIPTION:     About Dialog
*
*    IMPLEMENTATION:  About Dialog
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines, enums and structures
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "mintools.h"        // A misc group of useful functions
#include "AboutDlg.h"        // Header for this *.cpp file
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


// Globals
PTAboutDlg pAboutDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        DisplayAboutDlg                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *InfoWndCaption - Text entered for the window caption
*              char *InfoWndTitle   - Text entered for the window title  
*              char *InfoWndTxt - The body of text
*
*    Output:   returntype - none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/28/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL DisplayAboutDlg( HINSTANCE hInst, HWND hwnd, char *pCaption, char *pAbout, char *pResName, char *pResType, BYTE *buffer )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pAboutDlg = new TAboutDlg();

    // Create the dialog box
    if( pAboutDlg )
        Result = pAboutDlg->CreateDialogWnd( hInst, hwnd, pCaption, pAbout, pResName, pResType, buffer );

    return Result;

}   // DisplayAboutDlg



/************************************************************************
*    FUNCTION NAME:         AboutDlg_SetupDlgProc
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK AboutDlg_DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pAboutDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pAboutDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pAboutDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_PAINT,      pAboutDlg->OnPaint );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pAboutDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // About_SetupDlgProc


/************************************************************************
*
*                  Dialog Object Related Functions
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

TAboutDlg::TAboutDlg()
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

TAboutDlg::~TAboutDlg()
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

BOOL TAboutDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *pCaption, char *pAbout, char *pResName, char *pResType, BYTE *buffer )
{
    BOOL Result = FALSE;
    // GIF Decoder
    TGif AboutGIF;

    // Init class variables
    pDlgCaption = pCaption;
    pTxtAboutMsg = pAbout;
    hParentWnd = hwnd;

    // Load the graphic from a resource
    if( pResName && pResType )
    {
        // If this is a gif resource, decode and load the resource
        if( strcmp( pResType, "GIF" ) == 0 )
            Result = AboutDib.LoadFromBuffer( AboutGIF.LoadFromResource( pResName, pResType, hInst ) );

        // IF this is a bitmap resource, load the resource
        else if( pResType == RT_BITMAP || (strcmp( pResType, "BMP" ) == 0) )
            Result = AboutDib.LoadFromResource( hInst, pResName, pResType );
    }
    // If we are sending a pointer to the buffer, then load it
    else if( buffer )
        Result = AboutDib.LoadFromBuffer( buffer );

    if( Result )
        return DialogBox( hInst, "DLG_ABOUT", hwnd, (DLGPROC)AboutDlg_DlgProc );
    else
        return Result;

}   // CreateDialogWnd



/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
*                           Load the file the user has typed for their
*                           info window.
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

BOOL TAboutDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    BOOL Result = FALSE;
    RECT rect, GotoRect;
    int Offset, ImageWidth;

    // Set the caption
    if( pDlgCaption )
        SetWindowText( hwnd, pDlgCaption );

    // Set the message
    if( pTxtAboutMsg )
        SetWindowText( GetDlgItem( hwnd, TXT_ABOUT ), pTxtAboutMsg );

    // Center the window using the size of the bitmap
    if( !AboutDib.IsEmpty() )
    {
        // Check the image width
        ImageWidth = AboutDib.width();

        // If the image is too small, make the window a respectable size
        if( ImageWidth < ABOUT_DEFAULT_WIDTH )
            ImageWidth = ABOUT_DEFAULT_WIDTH;

        // Center the window with it's new size
        CenterWindow( hwnd, ImageWidth+(ABOUT_DIB_OFFSET * 2) + (GetSystemMetrics( SM_CXDLGFRAME ) * 2), 
                      AboutDib.height()+(ABOUT_DIB_OFFSET * 10) + (GetSystemMetrics( SM_CXDLGFRAME ) * 2) +
                      GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ), hParentWnd );

        // Get the rect to move the static text
        GetClientRect( GetDlgItem( hwnd, TXT_ABOUT ), &rect );

        // Move the static text
        MoveWindow( GetDlgItem( hwnd, TXT_ABOUT ), ABOUT_DIB_OFFSET, AboutDib.height() + (ABOUT_DIB_OFFSET * 2),
                    ImageWidth, rect.bottom, FALSE );

        // Save the offset to where the button should be
        Offset = AboutDib.height() + abs(rect.bottom - rect.top) + (ABOUT_DIB_OFFSET * 3);

        // Get the rects to move the buttons
        GetClientRect( GetDlgItem( hwnd, BTN_OK ), &rect );
        GetClientRect( GetDlgItem( hwnd, BTN_GOTO_WEB_SITE ), &GotoRect );

        // Move the OK button
        MoveWindow( GetDlgItem( hwnd, BTN_OK ), ((ImageWidth - rect.right) / 2) - ((rect.right + GotoRect.right) / 4) - 5, 
                    Offset, rect.right, rect.bottom, FALSE );

        // Move the OK button
        MoveWindow( GetDlgItem( hwnd, BTN_GOTO_WEB_SITE ), ((ImageWidth - GotoRect.right) / 2) + ((rect.right + GotoRect.right) / 4) + 5, 
                    Offset, GotoRect.right, GotoRect.bottom, FALSE );

        Result = TRUE;
    }

    return Result;

}   // OnInitDialog



/************************************************************************
*    FUNCTION NAME:         OnPaint                                                             
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

void TAboutDlg::OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT rect;

    // Get the rect of the about window
    GetClientRect( hwnd, &rect );

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Blit the graphic to the screen
    // The class also takes care of the realizing the palette if in 256 colors
    AboutDib.BlitToDC( PaintDC, (rect.right - AboutDib.width()) / 2, ABOUT_DIB_OFFSET );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // OnPaint



/************************************************************************
*    FUNCTION NAME:         OnCommand()                                                             
*
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
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

void TAboutDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_OK:
        case DLG_QUIT_BOX:
            // The TRUE is the return parameter
            EndDialog( hwnd, TRUE );        
        break;

        case BTN_GOTO_WEB_SITE:
            // goto the web site
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
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

void TAboutDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy