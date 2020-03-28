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
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include "MediaDir.h"        // Header file for this *.cpp file.
#include MAIN_APP_TOOLS      // Header file for this *.cpp file.

// Globals
PTMediaDirDlg pMediaDirDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         GetSoundDirectory                                                             
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

BOOL MediaDir_GetSoundDirectory( HINSTANCE hInst, HWND hwnd, char *SoundDir, HWND hList, 
                                 PCSoundListAry pLst, PTStringList pSoundList )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pMediaDirDlg = new TMediaDirDlg();

    // Create the dialog box
    if( pMediaDirDlg )
        Result = pMediaDirDlg->CreateDialogWnd( hInst, hwnd, SoundDir, hList, pLst, pSoundList );

    return Result;

}   // MediaDir_GetSoundDirectory


/************************************************************************
*    FUNCTION NAME:         MediaDirDlgProc
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

UINT APIENTRY MediaDirDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pMediaDirDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_DESTROY,    pMediaDirDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_COMMAND,    pMediaDirDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pMediaDirDlg->OnNCDestroy );
        default:
            return FALSE;
    }

}   // MediaDirDlgProc


/************************************************************************
*    FUNCTION NAME:         SubClassProc                                                             
*
*    DESCRIPTION:           Subclassing windows controls
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

LRESULT CALLBACK SndSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pMediaDirDlg->HandleSubClassProc( hCtrl, Message, wParam, lParam );

}   // SubClassProc


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

TMediaDirDlg::TMediaDirDlg()
{
    activeIndex = -1;

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

TMediaDirDlg::~TMediaDirDlg()
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

BOOL TMediaDirDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, char *SoundDir, HWND hList, 
                                    PCSoundListAry pLst, PTStringList pSoundList )
{
    OPENFILENAME op;
    char FileName[MAX_PATH*2];
    int i, Length;

    // Save the instance for other processes
    hInst = hInstance;
    hFrameList = hList;
    pSoundLst = pLst;
    pSoundLstStr = pSoundList;
    pSoundDir = SoundDir;

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
    op.nMaxFile = sizeof(char)*(MAX_PATH*2);  
    op.lpstrInitialDir = SoundDir;
    op.lpstrTitle = "Frame Sound Select";
    op.Flags = OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_ENABLETEMPLATE|OFN_EXPLORER|OFN_ENABLEHOOK; 
    op.lpfnHook = MediaDirDlgProc; 
    op.lpTemplateName = "DLG_MEDIA_DIR";

    // Show the dialog box
    return GetOpenFileName( &op );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         MediaDir_OnInitDialog()                                                             
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

BOOL TMediaDirDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    int i;
    BOOL Result = FALSE;
    char fileName[MAX_PATH*2];

    // Get the handle to this dialog window
    hParentWnd = hwnd;

    // Change the buttons for our needs
    Button_SetText( GetDlgItem( GetParent( hwnd ), IDCANCEL ), "Done" );
    ShowWindow( GetDlgItem( GetParent( hwnd ), IDOK ), SW_HIDE );

    if( !MediaPlayer.Init( hwnd, hInst, USE_ERROR_DLG_BOXES ) )
    {
        PostMsg( "Error", "Error initilizing media player." );
        goto HANDLE_ERROR;
    }

    // Zero out the array
    memset( OldSubCtrlProc, 0, sizeof(OldSubCtrlProc) );

    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SND_SUBCLASS_CTRLS; ++i )
    {
        // Save the control handle for later use
        hSubCtrl[ i ] = GetDlgItem( hwnd, EDT_SOUND_VOLUME + i );

        // Subclass the windows control
        OldSubCtrlProc[ i ] = (WNDPROC)SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, 
                                                      LONG( SndSubClassProc ) );

        // We had an error
        if( !OldSubCtrlProc[ i ] )
            goto HANDLE_ERROR;
    }

    // Fill the list box with the names of the selected sound files
    for( i = 0; i < pSoundLst->Count(); ++i )
    {
        CropFileNameFromPath( pSoundLstStr->GetString( pSoundLst->GetID(i) ), fileName );
        ListBox_AddString( hSubCtrl[ LST_SND ], fileName );
    }

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // MediaDir_OnInitDialog


/************************************************************************
*    FUNCTION NAME:         HandleSubClassProc                                                             
*
*    DESCRIPTION:           Handles the subclassing of controls. Check the
*                           window handle to see which control is being trapped.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
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

LRESULT TMediaDirDlg::HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result;

    /////////////////////////////////////////////////////
    // Each subclassed control has it's own message handler.
    // That's to keep this function as general and simple
    // as possible. You dig?
    /////////////////////////////////////////////////////

    // Handle the base X edit controls
    if( hCtrl == hSubCtrl[ EDT_VOLUME ] )
        Result = OnEdit_VolumeCtrl( hParentWnd, EDT_VOLUME, Message, wParam, lParam );

    // Handle the base Y edit controls
    else if( hCtrl == hSubCtrl[ EDT_CHAN ] )
        Result = OnEdit_ChannelCtrl( hParentWnd, EDT_CHAN, Message, wParam, lParam );

    // Handle the base Z edit controls
    else if( hCtrl == hSubCtrl[ LST_SND ] )
        Result = OnLst_SoundCtrl( hParentWnd, LST_SND, Message, wParam, lParam );

    else
        Result = FALSE;

    return Result;

}   // HandleSubClassProc


/************************************************************************
*    FUNCTION NAME:         OnEdit_VolumeCtrl                                                             
*
*    DESCRIPTION:           Message handler for the volume edit control    
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

LRESULT TMediaDirDlg::OnEdit_VolumeCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    char StrValue[ 20 ];

    if( activeIndex > -1 && Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Save the entered channel
        pSoundLst->SetVolume( activeIndex, atoi( StrValue ) );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_VolumeCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_ChannelCtrl                                                             
*
*    DESCRIPTION:           Message handler for the channel edit control    
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

LRESULT TMediaDirDlg::OnEdit_ChannelCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    char StrValue[ 20 ];

    if( activeIndex > -1 && Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Save the entered channel
        pSoundLst->SetChannel( activeIndex, atoi( StrValue ) );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_ChannelCtrl


/************************************************************************
*    FUNCTION NAME:         OnLst_SoundCtrl                                                             
*
*    DESCRIPTION:           Message handler for the sound list control    
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

LRESULT TMediaDirDlg::OnLst_SoundCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int AnimIndex, offset=0, frameCount, i;
    char fileName[MAX_PATH*2];
    
    if( activeIndex > -1 && Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a selection to the animation
        AnimIndex = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
        if( AnimIndex == LB_ERR )
            return Result;

        // Ask if they want to delete the selected animation
        if( pSoundLst->Count() &&
            IDYES == AskQuestion( MB_YESNO, "Question?", "Do you want to delete the selected sound(s)?" ) )
        {
            // get the number of frames
            frameCount = pSoundLst->Count();

            // Go through the frame list and termination the selected ones
            for( i = 0; i < frameCount; ++i )
            {
                // Has this frame been selected?
                if( ListBox_GetSel( hSubCtrl[ Ctrl ], i ) > 0 )
                {
                    // delete this object's frames
                    pSoundLst->Delete( i + offset );

                    --offset;
                }
            }

            // Clear out the frame list box
            ListBox_ResetContent( hSubCtrl[ Ctrl ] );

            // Add animation name to create animation list box
            for( i = 0; i < pSoundLst->Count(); ++i )
            {
                CropFileNameFromPath( pSoundLstStr->GetString( pSoundLst->GetID(i) ), fileName );
                ListBox_AddString( hSubCtrl[ Ctrl ], fileName );
            }
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnLst_SoundCtrl


/************************************************************************
*    FUNCTION NAME:         MediaDir_OnInitDialog()                                                             
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

void TMediaDirDlg::OnDestroy( HWND hwnd )
{
    int i;
        
    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS; ++i )
    {
        // Give all the subclassed controls their procedure back 
        // before destroying the dialog box
        if( OldSubCtrlProc[ i ] )
            SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, LONG( OldSubCtrlProc[ i ] ) );
    } 

}   // MediaDir_OnInitDialog


/************************************************************************
*    FUNCTION NAME:         MediaDir_OnCommand()                                                             
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

void TMediaDirDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_PLAY_SOUND:
            BtnClk_PlaySound( hwnd );
        break;

        case BTN_ADD_SND:
            BtnClk_AddSound( hwnd );
        break;

        case LST_FRAME_SND:
            LstClk_SoundList( hwnd, hwndCtl, codeNotify );
        break;
    }

}   // HelpHintFrm_OnCommand


/************************************************************************
*    FUNCTION NAME:         LstClk_SoundList                                                             
*
*    DESCRIPTION:           Handle the list clicks
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

void TMediaDirDlg::LstClk_SoundList( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int index;

    if( codeNotify == CBN_SELCHANGE )
    {
        // Make sure we have a selection to the animation
        index = ListBox_GetCurSel( hwndCtl );
        if( index == LB_ERR )
            return;

        // Save the active index
        activeIndex = index;

        // Set the Volume control
        Edit_DisplayValue( hSubCtrl[ EDT_VOLUME ], pSoundLst->GetVolume( index ) );

        // Set the channel control
        Edit_DisplayValue( hSubCtrl[ EDT_CHAN ], pSoundLst->GetChannel( index ) );
    }

}   // LstClk_SoundList


/************************************************************************
*    FUNCTION NAME:         BtnClk_PlaySound                                                             
*
*    DESCRIPTION:           Play Sound
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

void TMediaDirDlg::BtnClk_PlaySound( HWND hwnd )
{
    char filePath[MAX_PATH*2];

    // Check first to see that a file was selected
    CommDlg_OpenSave_GetSpec( GetParent( hwnd ), filePath, sizeof(filePath) );

    // Get the file name to the currently selected sound file
    // Notice the handle is to our dialog box and not the common dialog box. To get
    // that handle w need to use the GetParent() function
    if( filePath[0] && CommDlg_OpenSave_GetFilePath( GetParent( hwnd ), filePath, sizeof(filePath) ) )
    {
        // If a file is playing, stop it by closing it.
        if( MediaPlayer.GetStatus() == PLAYER_PLAYING )
        {
            MediaPlayer.Close();
        }
        else
        {    
            MediaPlayer.Play( filePath );
        }
    }

}   // BtnClk_PlaySound


/************************************************************************
*    FUNCTION NAME:         BtnClk_AddSound                                                             
*
*    DESCRIPTION:           Add sound to list
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

void TMediaDirDlg::BtnClk_AddSound( HWND hwnd )
{
    char filePath[MAX_PATH*2], fileName[MAX_PATH*2];
    int i, soundIndex;
    BOOL soundFound = FALSE;

    // Check first to see that a file was selected
    CommDlg_OpenSave_GetSpec( GetParent( hwnd ), filePath, sizeof(filePath) );

    // Get the file name to the currently selected sound file
    // Notice the handle is to our dialog box and not the common dialog box. To get
    // that handle w need to use the GetParent() function
    if( filePath[0] && CommDlg_OpenSave_GetFilePath( GetParent( hwnd ), filePath, sizeof(filePath) ) )
    {
        // Crop the path from the file name
        CropOutPath( filePath, pSoundDir );

        // Make sure this file has not been already added
        // to the main list. The file path could have been added to the main list
        // but just no to this frame
        for( i = 0; i < pSoundLstStr->Count(); ++i )
        {
            if( strcmp( filePath, pSoundLstStr->GetString( i ) ) == 0 )
            {
                soundFound = TRUE;
                break;
            }
        }
        
        // If it's already part of the main list, don't add it
        if( !soundFound )
        {
            // Save the index so we know where the sound is in the list.
            soundIndex = pSoundLstStr->Count();

            // Add the string to the list
            pSoundLstStr->Add( filePath );
        }
        else
            // Save the index so we know where the sound is in the list.
            soundIndex = i;

        // Reset the sound flag
        soundFound = FALSE;
        
        // Go through the sound list to make sure this sound ID is not already in use
        for( i = 0; i < pSoundLst->Count(); ++i )
        {
            if( pSoundLst->GetID( i ) == soundIndex )
            {
                soundFound = TRUE;
                break;
            }
        }
        
        // If it's already there, don't add it
        if( !soundFound )
        {
            // Add a new entry to the list class
            pSoundLst->Add();

            // Add the sound Index
            pSoundLst->SetID( pSoundLst->Count()-1, soundIndex );

            // Clear out the frame list box
            ListBox_ResetContent( hSubCtrl[ LST_SND ] );

            // Add animation name to create animation list box
            for( i = 0; i < pSoundLst->Count(); ++i )
            {
                CropFileNameFromPath( pSoundLstStr->GetString( pSoundLst->GetID(i) ), fileName );
                ListBox_AddString( hSubCtrl[ LST_SND ], fileName );
            }
            
            // Select the list box item
            ListBox_SetSel( hSubCtrl[ LST_SND ], TRUE, pSoundLst->Count()-1 );

            // Pretend we click on it
            LstClk_SoundList( hwnd, hSubCtrl[ LST_SND ], CBN_SELCHANGE );
        }   
    }

}   // BtnClk_AddSound