/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       PictLoad.cpp
*
*    DESCRIPTION:     Load picture dialog box
*
*    IMPLEMENTATION:  This is not a full dialog box but an extension to the
*                     common file open dialog box.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "PictLoad.h"        // Header file for this *.cpp file.
#include <commctrl.h>        // Header file for Windows 95 controls
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf

#ifndef NO_JPEG
#include "TJPEG.h"           // JPEG Library
#endif
#ifndef NO_GIF
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#endif

#include "gTools.h"        // Header file for this *.cpp file.


// Globals
PTPictLoadDlg pPictLoadDlg;



/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         PictLoad_GetPicture                                                             
*
*    DESCRIPTION:           Displays a modified common dialog box used
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *SoundDir - Returns the directory the sound file was selected
*              char *GetFileName - Returns the path to the sound file
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

BOOL PictLoad_GetPicture( HINSTANCE hInst, HWND hwnd, char *Caption, DWORD Flags, char *FileName, int fBufSize, char *FileMask, char *GetDir )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pPictLoadDlg = new TPictLoadDlg();

    // Create the dialog box
    if( pPictLoadDlg )
        Result = pPictLoadDlg->CreateDialogWnd( hInst, hwnd, Caption, Flags, FileName, fBufSize, FileMask, GetDir );

    return Result;

}   // PictLoad_GetPicture


/************************************************************************
*    FUNCTION NAME:         LoadPictDlgProc
*
*    DESCRIPTION:           Dialog hook message handler for common dialog boxes.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of our dialog and not the common dialog box
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

UINT APIENTRY LoadPictDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pPictLoadDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_DESTROY, pPictLoadDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_COMMAND, pPictLoadDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_NOTIFY, pPictLoadDlg->OnNotify );
        HANDLE_MSG( hDlg, WM_NCDESTROY, pPictLoadDlg->OnNCDestroy );
        default:
            return FALSE;
    }

}   // LoadPictDlgProc


/************************************************************************
*    FUNCTION NAME:         NewEditProc                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep specific keystrokes from being
*                           keyed into the edit field. We only the user to
*                           be able to type numbers, delete keys and the
*                           period key.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*              UINT Message   - Message being sent 
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
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

LRESULT CALLBACK PictLoadStaticProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_PAINT, pPictLoadDlg->OnPaint );
        default:
            return CallWindowProc( pPictLoadDlg->GetStaticProc(), hwnd, Message, wParam, lParam );;
    }
}   // NewStaticProc


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

TPictLoadDlg::TPictLoadDlg()
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

TPictLoadDlg::~TPictLoadDlg()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         CreateDialogWnd                                                             
*
*    DESCRIPTION:           Create the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *SoundDir - Returns the directory the sound file was selected
*              char *GetFileName - Returns the path to the sound file
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

BOOL TPictLoadDlg::CreateDialogWnd( HINSTANCE hInst, HWND hwnd, char *Caption, DWORD Flags, char *FileName, int fBufSize, char *FileMask, char *GetDir )
{
    OPENFILENAME op;
    BOOL Result = FALSE;

    // Be sure to memset all windows structures because they
    // have the ability to grow for one version to the next.
    memset( &op, 0, sizeof(OPENFILENAME) );

    op.lStructSize = sizeof(OPENFILENAME); 
    op.hwndOwner = hwnd; 
    op.hInstance = hInst;  
    op.lpstrFilter = FileMask; 
    op.nFilterIndex = 1; 
    op.lpstrFile = FileName;
    op.nMaxFile = fBufSize;  
    op.lpstrInitialDir = GetDir;
    op.lpstrTitle = Caption;
    op.Flags = OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_ENABLETEMPLATE|OFN_EXPLORER|OFN_ENABLEHOOK|Flags; 
    op.lpfnHook = LoadPictDlgProc; 
    op.lpTemplateName = "DLG_LOAD_PICT";

    if( GetOpenFileName( &op ) )
        Result = TRUE;

    return Result;

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         PictLoad_OnInitDialog()                                                             
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TPictLoadDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT DisplayRect;

    // Get the rect of our view window
    GetClientRect( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), &DisplayRect );

    // Create the preview buffer
    PrevBuffer.CreateStandardBuffer( DisplayRect.right, DisplayRect.bottom );

    // Subclass static control
    OldStaticWnd = (WNDPROC)SetWindowLong( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), 
                                          GWL_WNDPROC, LONG( PictLoadStaticProc ) );

    return TRUE;

}   // PictLoad_OnInitDialog


/************************************************************************
*    FUNCTION NAME:         SS2GO_OnNotify                                                             
*
*    DESCRIPTION:           Standard Windows OnNotify message handler.
*                           This is for using tool tips
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd          - Handle of the window receiving the message
*              int idFrom         - Menu ID of the of the control.
*              NMHDR FAR * pnmhdr - LPTOOLTIPTEXT structure
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

LRESULT TPictLoadDlg::OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr )
{
    char filePath[MAX_PATH*2];
    RECT DisplayRect;
    #if !defined( NO_GIF ) || !defined( NO_JPEG )
    BYTE *pLoadPtr;
    #endif
    #ifndef NO_GIF
    // Gif Decoder
    TGif gif;
    #endif
    #ifndef NO_JPEG
    // Jpeg Decoder
    TJPEG jpeg;
    #endif

    // If a new folder was selected, erase the displayed image
    if( pnmhdr->code == CDN_FOLDERCHANGE )
    {
        // Free the image
        PreviewImage.Free();

        // Get the rect of our view window
        GetClientRect( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), &DisplayRect );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), &DisplayRect, TRUE );
    }

    // See if the user made a selection
    if( pnmhdr->code == CDN_SELCHANGE )
    {
        // Check first to see that a file was selected
        CommDlg_OpenSave_GetSpec( GetParent( hwnd ), filePath, sizeof(filePath) );

        // Get the file name to the currently selected file or files
        // Notice the handle is to our dialog box and not the common dialog box. To get
        // that handle we need to use the GetParent() function.
        // The '"' indicates a multiselection and we can't display that.
        if( filePath[0] && filePath[0] != '"' && CommDlg_OpenSave_GetFilePath( GetParent( hwnd ), filePath, sizeof(filePath) ) )
        {
            // Make sure the file exists because they could have changed
            // directories and a file from the previous directory will merge
            // with the current directory
            if( filePath[0] && FileExist( filePath ) )
            {
                // Get the type of file so we know how to load it
                switch( GetFileType( filePath ) )
                {
                    case FILE_TYPE_BMP:
                        PreviewImage.LoadFromFile( filePath );
                    break;

                    #ifndef NO_JPEG
                    case FILE_TYPE_JPG:
                        // First load and decode the image
                        pLoadPtr = jpeg.LoadFromFile( filePath );

                        // Then pass it along to the DIB class
                        PreviewImage.LoadFromBuffer( pLoadPtr, jpeg.GetJPGDataSize() );
                    break;
                    #endif

                    #ifndef NO_GIF
                    case FILE_TYPE_GIF:
                        // First load and decode the image
                        pLoadPtr = gif.LoadFromFile( filePath );

                        // Then pass it along to the DIB class
                        PreviewImage.LoadFromBuffer( pLoadPtr, gif.GetGifDataSize() );
                    break;
                    #endif

                    #ifndef NO_TXT
                    case FILE_TYPE_TXT:
                        // Free the image
                        PreviewImage.Free();
                    break;
                    #endif
                }

                // Get the rect of our view window
                GetClientRect( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), &DisplayRect );

                // Invalidate the whole window so it all gets painted
                InvalidateRect( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), &DisplayRect, TRUE );
            }
        }
    }

    return 1;

}   // PictLoad_OnNotify


/************************************************************************
*    FUNCTION NAME:         PictLoad_OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message 
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

void TPictLoadDlg::OnDestroy( HWND hwnd )
{
    // Give the static control back its window procedure
    if( OldStaticWnd )
        SetWindowLong( GetDlgItem( hwnd, STC_PICTLOAD_IMAGE_WND ), GWL_WNDPROC, LONG( OldStaticWnd ) );

}   // PictLoad_OnInitDialog


/************************************************************************
*    FUNCTION NAME:         PictLoad_OnCommand()                                                             
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

void TPictLoadDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{

}   // HelpHintFrm_OnCommand


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnPaint()                                                             
*
*    DESCRIPTION:           Standard Windows OnPaint message handler.
*                           The OnPaint is not being used at this time.
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

void TPictLoadDlg::OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT DisplayRect, ControlRect;
    BOOL SizeToFit = FALSE;
    HBRUSH brush;
    char ImageInfoStr[100];

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Get the rect
    GetClientRect( hwnd, &ControlRect );

    // Create a brush to erase the buffer
    brush = CreateSolidBrush( GetSysColor( COLOR_BTNFACE ) );

    // Clear out the buffer by coloring the buffer with the default color
    PrevBuffer.ColorRect( &ControlRect, brush );

    // Delete the brush
    DeleteObject( brush );

    // Make sure we are ready to blit
    if( !PreviewImage.IsEmpty() )
    {
        // Setup the image info string
        sprintf( ImageInfoStr, "%u x %u - %u Bit Color Image - File Size: %u Bytes", 
            PreviewImage.width(), PreviewImage.height(), PreviewImage.GetBitCount(), PreviewImage.GetObjectSizeInBytes() );
        
        // Set the info window
        Static_SetText( GetDlgItem( GetParent( hwnd ), STC_PICTLOAD_INFO ), ImageInfoStr );

        // If the image is bigger than the static control,
        // size the bitmap to fit
        if( PreviewImage.width() > ControlRect.right ||
            PreviewImage.height() > ControlRect.bottom )
        {
            SizeToFit = TRUE;
        }

        // Size the main image to fit the static control
        if( SizeToFit )
        {
            // Size the main image to fit into the static control's size
            if( SizeToFitRect( &DisplayRect, PreviewImage.width(), PreviewImage.height(), 
                       ControlRect.right, ControlRect.bottom ) )
            {
                // This ensures the niceest looking stretch
                SetStretchBltMode( PrevBuffer.Handle(), HALFTONE );

                // Blit the main image to the static control
                PreviewImage.BlitToDC( PrevBuffer.Handle(), &DisplayRect );

                // Resets every thing back after a stretch
                SetBrushOrgEx( PrevBuffer.Handle(), 0, 0, NULL );
            }
        }
        else  // Center the main image in the static control
        {
            // Blit the main image to the static control
            PreviewImage.BlitToDC( PrevBuffer.Handle(), abs(ControlRect.right - PreviewImage.width()) / 2,
                                   abs(ControlRect.bottom - PreviewImage.height()) / 2 );
        }
    }
    else
        // Erase the info window
        Static_SetText( GetDlgItem( GetParent( hwnd ), STC_PICTLOAD_INFO ), "" );

    // Blit the buffer to the screen
    PrevBuffer.Draw( PaintDC );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // ViewWnd_OnPaint
