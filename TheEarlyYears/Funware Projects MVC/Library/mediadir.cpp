/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       MediaDir.cpp
*
*    DESCRIPTION:     Screen saver Media directory dialog box
*
*    IMPLEMENTATION:  This is not a full dialog box but an extension to the
*                     common file open dialog box. The end result a string
*                     to the directory containing sound files.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "ssdefines.h"       // Header file with ss base specific #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include "MediaDir.h"        // Header file for this *.cpp file.
#include "minTools.h"        // Header file for this *.cpp file.

// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

// Globals
PCMediaDirDlg pMediaDirDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    DESCRIPTION:           Displays a modified common dialog box used
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *SoundDir - Returns the directory the sound file was selected
*              char *GetFileName - Returns the path to the sound file
*
*    Output:   returntype - none
************************************************************************/
bool MediaDir_GetSoundDirectory( HINSTANCE hInst, HWND hwnd, char *SoundDir, char *GetFileName )
{
    bool Result = false;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pMediaDirDlg = new CMediaDirDlg();

    // Create the dialog box
    if( pMediaDirDlg )
        Result = pMediaDirDlg->CreateDialogWnd( hInst, hwnd, SoundDir, GetFileName );

    return Result;

}   // MediaDir_GetSoundDirectory



/************************************************************************
*    DESCRIPTION:           Dialog hook message handler for common dialog boxes.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of our dialog and not the common dialog box
*              UINT uMsg      - Message being sent
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   returntype - Description
************************************************************************/
UINT APIENTRY MediaDirDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pMediaDirDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_DESTROY,    pMediaDirDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_COMMAND,    pMediaDirDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pMediaDirDlg->OnNCDestroy );
        default:
            return false;
    }

}   // MediaDirDlgProc


/************************************************************************
*
*                About Dialog Object Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
************************************************************************/
CMediaDirDlg::CMediaDirDlg()
{

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
************************************************************************/
CMediaDirDlg::~CMediaDirDlg()
{

}   // Destructer


/************************************************************************
*    DESCRIPTION:           Create the dialog Window
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaDirDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, char *SoundDir, char *GetFileName )
{
    OPENFILENAME op;
    char FileName[MAX_PATH];
    int i, Length;
    bool Result = false;

    // Save the instance for other processes
    hInst = hInstance;

    // Be sure to memset all windows structures because they
    // have the ability to grow for one version to the next.
    memset( &op, 0, sizeof(OPENFILENAME) );

    // Must terminate or it wont work
    FileName[0] = 0;
    op.lStructSize = sizeof(OPENFILENAME); 
    op.hwndOwner = hwnd; 
    op.hInstance = hInst; 
    op.lpstrFilter = "Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0Wave Files (*.wav)\0*.wav\0MP3 Files (*.mp3)\0*.mp3\0Midi Files (*.mid, *.rmi)\0*.mid;*.rmi\0\0"; 
    op.nFilterIndex = 1; 
    op.lpstrFile = FileName;
    op.nMaxFile = sizeof(char)*MAX_PATH;  
    op.lpstrInitialDir = SoundDir;
    op.lpstrTitle = "Select Sound Directory";
    op.Flags = OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_ENABLETEMPLATE|OFN_EXPLORER|OFN_ENABLEHOOK; 
    op.lpfnHook = MediaDirDlgProc; 
    op.lpTemplateName = "DLG_MEDIA_DIR";

    // Parameters that are currently not being used
    /*op.lpstrCustomFilter = 0; 
    op.nMaxCustFilter = 0;
    op.lpstrFileTitle = 0; 
    op.nMaxFileTitle = 0;
    op.nFileOffset = 0; 
    op.nFileExtension = 0; 
    op.lpstrDefExt = 0; 
    op.lCustData = 0;*/

    if( GetOpenFileName( &op ) )
    {
        // Save the selected file name
        if( GetFileName )
            strcpy( GetFileName, FileName );
        
        // Find the last \\ and null it
        Length = strlen(FileName);

        // Work you way backwards until you hit the
        for( i = Length; i > 0; --i )
        {
            // If we found the slash, null it and break out
            if( FileName[i] == '\\' )
            {
                FileName[i] = 0;
                break;  
            }
        }

        // Copy over the sound directory
        strcpy( SoundDir, FileName );

        Result = true;
    }

    return Result;

}   // CreateDialogWnd


/************************************************************************
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
*                           Here we are creating a window to paint our frames
*                           on to and a double buffer to compose our frames.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
************************************************************************/
bool CMediaDirDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    bool Result = false;

    // Change the button text for our needs
    Button_SetText( GetDlgItem( GetParent( hwnd ), IDOK ), "OK" );

    if( !MediaPlayer.Init( hwnd, hInst, USE_ERROR_DLG_BOXES ) )
    {
        PostMsg( "Error", "Error initilizing media player." );
        goto HANDLE_ERROR;
    }

    Result = true;

HANDLE_ERROR:;

    return Result;

}   // MediaDir_OnInitDialog


/************************************************************************
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message 
************************************************************************/
void CMediaDirDlg::OnDestroy( HWND hwnd )
{ 

}   // MediaDir_OnInitDialog


/************************************************************************
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
************************************************************************/
void CMediaDirDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    char filePath[MAX_PATH*2];

    switch( id )
    {
        case BTN_PLAY_SOUND:

            // Check first to see that a file was selected
            CommDlg_OpenSave_GetSpec( GetParent( hwnd ), filePath, sizeof(filePath) );

            // Get the file name to the currently selected sound file
            // Notice the handle is to our dialog box and not the common dialog box. To get
            // that handle w need to use the GetParent() function
            if( filePath[0] && CommDlg_OpenSave_GetFilePath( GetParent( hwnd ), filePath, sizeof(filePath) ) )
            {
                // If a file is playing, stop it by closing it.
                if( MediaPlayer.GetStatus() == PLAYER_PLAYING )
                    MediaPlayer.Close();
                else    
                    MediaPlayer.Play( filePath );
            }
        break;
    }

}   // HelpHintFrm_OnCommand
