
/************************************************************************
*
*    PRODUCT:         MNG Animator
*
*    FILE NAME:       mngAnim.cpp
*
*    DESCRIPTION:     Program for outputing animation scripts for MNG files
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "GDefines.h"        // Header file with #defines
#include "mngAnim.h"         // Header file for this *.cpp file.
#include "gTools.h"          // A misc group of useful functions
#include "DlgGetName.h"      // Dialog box for getting the name of the script to be created
#include "shlobj.h"          // Allows for use of the directory selection window
#include "MovePage.h"        // Move data class
#include "PictLoad.h"        // Dialog for loading pictures that shows a preview window
#include "TJPEG.h"           // JPEG Library
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "MediaDir.h"        // Header file for the common dialog box modification.


// Globals
PCMNGanimWnd pMNGanimWnd;
LPSTR Asterick[MAX_ASTERICK] = { "", "*", "**", "***", "****", "*****", "******", "*******", "********", "**********", "**********" };


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         WinMain                                                             
*
*    DESCRIPTION:           The entry point for all good windows programs
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HINSTANCE hPrevInstance - Of no value to 32 bit windows programs
*              LPSTR lpszCmdParam - The command line is sent by Windows. The command
*                                   originally has the path to this program but
*                                   Windows parses it out before it hands it over.
*                                   To get the command line with the path in it you
*                                   need to call GetCommandLine() API call.
*              int nCmdShow - Specifies how the window is to be shown.
*                             Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED   
*
*    Output:   int - Usually it's the last message wParam.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                            LPSTR lpszCmdParam, int nCmdShow )
{
    MSG Msg;
    BOOL Result = FALSE;
    HACCEL hAccel;

    // Create the parent window object
    pMNGanimWnd = new CMNGanimWnd();

    // Create the parent window from a dialog
    if( pMNGanimWnd )
        Result = pMNGanimWnd->CreateParentWnd( hInst, nCmdShow );

    // Kick out of the program if there has been an error
    if( !Result )
        return Result;

    // Load the accelerators
    hAccel = LoadAccelerators( hInst, "DIALOG_ACC" );
    if( !hAccel )
    {
        MessageBox( pMNGanimWnd->GetParentWnd(), "Error loading accelerator table", "Error", MB_OK );
        return FALSE;
    }

    // Standard windows message loop
    while( GetMessage( &Msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( pMNGanimWnd->GetParentWnd(), hAccel, &Msg ) )
        {
            TranslateMessage( &Msg );
            DispatchMessage( &Msg );
        }
    }

    return Msg.wParam;

}   // WinMain


/************************************************************************
*    FUNCTION NAME:         WndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler.
*                           We are using this to call our object functions
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_INITDIALOG, pMNGanimWnd->OnInitDialog );
        HANDLE_MSG( hwnd, WM_COMMAND,    pMNGanimWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_DESTROY,    pMNGanimWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_NCDESTROY,  pMNGanimWnd->OnNCDestroy );
        HANDLE_MSG( hwnd, WM_ACTIVATE,   pMNGanimWnd->OnActivate );
        default:
            return FALSE;
    }

} // WndProc


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

LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pMNGanimWnd->HandleSubClassProc( hCtrl, Message, wParam, lParam );

}   // SubClassProc


/************************************************************************
*    FUNCTION NAME:         PlayAnimTimer                                                             
*
*    DESCRIPTION:           Callback timer function that plays the animation(s).
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

void CALLBACK PlayAnimTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime )
{
    // Play the animations
    pMNGanimWnd->PlayAnimations( hwnd );

}   // PlayAnimTimer


/************************************************************************
*
*                      Window Object Functions
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

CMNGanimWnd::CMNGanimWnd()
{
    MouseButtonDown = FALSE;
    activeScript = -1;
    BackgroundLoaded = FALSE;
    TimerReturnCode = 0;
    fLoopAnimations = TRUE;

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

CMNGanimWnd::~CMNGanimWnd()
{
}   // Destructer


/************************************************************************
*    FUNCTION NAME:         CreateParentWnd()                                                             
*
*    DESCRIPTION:           Create the parent Window
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

BOOL CMNGanimWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    BOOL Result = FALSE;

    // Save the program instance
    hInst = hInstance;

    // Create the parent window
    if( CreateWnd( hInst, nCmdShow ) )
        Result = TRUE;
    else
        // Let them know there was a problem
        MessageBox( 0, "Error creating parent window.", "Error", MB_OK );

    return Result;

}   // CreateParentWnd 


/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           Create the parent window from a dialog
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*              int nCmdShow    - Specifies how the window is to be shown.
*                                Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*
*    Output:   HWND - A handle to the newly created window
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CMNGanimWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window
    if( DialogBox( hInstance, "DLG_MNGANIM_WND", NULL, (DLGPROC)WndProc ) < 1 )
        return FALSE;

    return TRUE;

}   // CreateWnd


/************************************************************************
*    FUNCTION NAME:         OnSetFocus                                                             
*
*    DESCRIPTION:           Standard Windows SetFocus message handler.
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

void CMNGanimWnd::OnActivate( HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized )
{

}   // OnSetFocus


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
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

void CMNGanimWnd::OnDestroy( HWND hwnd )
{
    int i;

    if( TimerReturnCode )
        // Kill the timer if it is left running
        KillTimer( hSubCtrl[ STC_VIEW ], ANIM_TIMER_HANDLE );

    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS; ++i )
    {
        // Give all the subclassed controls their procedure back 
        // before destroying the dialog box
        if( OldSubCtrlProc[ i ] )
            SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, LONG( OldSubCtrlProc[ i ] ) );
    }

    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         QuitProgram                                                             
*
*    DESCRIPTION:           Quit the program by destroying the window
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void CMNGanimWnd::QuitProgram( HWND hwnd )
{
    RECT r;

    // Save all the active scripts
    MnuClk_SaveAllAciveScripts( hwnd );

    // Get the XY of the window
    GetWindowRect( hwnd, &r );

    // Save the position of the window
    Settings.SetWindowX( r.left );
    Settings.SetWindowY( r.top );

    // Save the frames per sec
    Settings.SetAnimFrameCount( Edit_GetValue( GetDlgItem( hwnd, EDT_FPS ) ) );

    // Save the settings
    Settings.WritePrivateString();
   
    // Destroy the window
    DestroyWindow( hwnd );

}   // QuitProgram


/************************************************************************
*    FUNCTION NAME:         OnCommand                                                             
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNGanimWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case PROGRAM_EXIT:
        case MNU_EXIT:
            QuitProgram( hwnd );        
        break;

        case MNU_OPEN_PROJ_PATH:
            MnuClk_OpenProjPath( hwnd );
        break;

        case LST_MNG_FILES:
            ListClk_MNGfileList( hwnd, hwndCtl, codeNotify );
        break;

        case LST_MNG_FRAMES:
            ListClk_MNGfileFrames( hwnd, hwndCtl, codeNotify );
        break;

        case BTN_CREATE_SCRIPT:
            BtnClk_CreateScript( hwnd );
        break;

        case LST_MNG_SCRIPT_FILES:
            ListClk_ScriptLst( hwnd, hwndCtl, codeNotify );
        break;

        case LST_ANIM_FRAMES:
            ListClk_AnimFrameLst( hwnd, hwndCtl, codeNotify );
        break;

        case CHK_GOST_PREV_FRAME:
            // update the view buffer
            UpdateViewBuffer( hwnd );

            // Invalidate the whole window so it all gets painted
            InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
        break;

        case BTN_ADD_FRAME:
            BtnClk_AddFrame( hwnd );
        break;

        case MNU_SAVE_ACTIVE_SCRIPTS:
            MnuClk_SaveAllAciveScripts( hwnd );
        break;

        case LST_AVAILABLE_MNG:
            ListClk_AvailableMNGFrames( hwnd, hwndCtl, codeNotify );
        break;

        case MNU_LOAD_BACKGROUND:
            MnuClk_LoadBackground( hwnd );
        break;

        case MNU_CLEAR_BACKGROUND:
            MnuClk_ClearBackground( hwnd );
        break;

        case BTN_ADD_SCRIPT:
            BtnClk_AddScriptToAnimate( hwnd );
        break;

        case BTN_PLAY_SCRIPTS:
            BtnClk_PlayScripts( hwnd );
        break;

        case MNU_SET_SOUND:
            MnuClk_SetSoundProperties( hwnd );
        break;

        case MNU_CLEAR_SOUND:
            MnuClk_ClearSoundProperties( hwnd );
        break;

        case MNU_PLAY_SOUNDS_ANIM:
            MnuClk_PlaySound( hwnd );
        break;

        case LST_SCRIPTS_TO_ANIM:
            ListClk_ScrptToAnimLst( hwnd, hwndCtl, codeNotify );
        break;

        case LST_PLAY_OPTIONS:
            ListClk_AnimTypeLst( hwnd, hwndCtl, codeNotify );
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         ListClk_ScrptToAnimLst                                                             
*
*    DESCRIPTION:           Click handler for the scripts to animate control
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_ScrptToAnimLst( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int index, i;

    if( codeNotify == CBN_SELCHANGE )
    {
        // Make sure we have a selection to the animation
        index = ListBox_GetCurSel( hwndCtl );
        if( index == LB_ERR )
            return;

        // Go through all the strings in the anim to run list box to 
        // find the index of the needed script
        for( i = 0; i < StoryLstAry.Count(); ++i )
        {
            if( strcmp( StoryLstAry.GetPtrToScriptName( i ), scriptAnimList.GetString( index ) ) == 0 )
            {
                // Select the list box item
                ListBox_SetCurSel( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), StoryLstAry.GetAnimType( i ) );
                break;
            }
        }       
    }
}   // ListClk_ScrptToAnimLst


/************************************************************************
*    FUNCTION NAME:         ListClk_AnimTypeLst                                                             
*
*    DESCRIPTION:           Click handler for the animation type control
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_AnimTypeLst( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int index, animType, i;

    if( codeNotify == CBN_SELCHANGE )
    {
        // Make sure we have stuff loaded
        if( scriptAnimList.Count() && StoryLstAry.Count() )
        {
            // Make sure we have a selection to the animation type
            animType = ListBox_GetCurSel( hwndCtl );
            if( animType == LB_ERR )
                return;

            // Make sure we have a selection to the animation
            index = ListBox_GetCurSel( hSubCtrl[ LST_SCR_TO_ANIM ] );
            if( index == LB_ERR )
                return;

            // Go through all the strings in the anim to run list box to 
            // find the index of the needed script
            for( i = 0; i < StoryLstAry.Count(); ++i )
            {
                if( strcmp( StoryLstAry.GetPtrToScriptName( i ), scriptAnimList.GetString( index ) ) == 0 )
                {
                    // Set the animation type
                    StoryLstAry.SetAnimType( i, animType );
                    break;
                }
            }
        }       
    }
}   // ListClk_AnimTypeLst


/************************************************************************
*    FUNCTION NAME:         MnuClk_PlaySound                                                             
*
*    DESCRIPTION:           Clear the check mark for playing sounds
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNGanimWnd::MnuClk_PlaySound( HWND hwnd )
{
    // Make sure we are not animating
    if( !TimerReturnCode )
    {
        // See if the menu is checked
        if( IsMenuChecked( hwnd, MNU_PLAY_SOUNDS_ANIM ) )
        {
            // We need to uncheck this menu
            Settings.SetPlaySounds( FALSE );
            CheckMenuItem( GetMenu( hwnd ), MNU_PLAY_SOUNDS_ANIM, MF_UNCHECKED );
        }
        else
        {
            // We need to uncheck this menu
            Settings.SetPlaySounds( TRUE );
            CheckMenuItem( GetMenu( hwnd ), MNU_PLAY_SOUNDS_ANIM, MF_CHECKED );
        }
    }

}   // MnuClk_PlaySound


/************************************************************************
*    FUNCTION NAME:         MnuClk_ClearSoundProperties                                                             
*
*    DESCRIPTION:           Clear the sound properties of the frame(s)
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNGanimWnd::MnuClk_ClearSoundProperties( HWND hwnd )
{
    int i;

    // Ask if they want to delete the selected animation
    if( activeScript > -1 && StoryLstAry.GetStoryList( activeScript )->Count() &&
        IDYES == AskQuestion( MB_YESNO, "Question?", "Do you want to delete all the sound properties from the selected frame(s)?" ) )
    {
        // Go through the frame list and termination the selected ones
        for( i = 0; i < StoryLstAry.GetStoryList( activeScript )->Count(); ++i )
        {
            // Has this frame been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_FRAMES ], i ) > 0 )
            {
                // delete this object's frames
                StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Free();
            }
        }
    }
}   // MnuClk_ClearSoundProperties


/************************************************************************
*    FUNCTION NAME:         Mnu_SetSoundProperties                                                             
*
*    DESCRIPTION:           Set the sound properties of the frame(s)
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CMNGanimWnd::MnuClk_SetSoundProperties( HWND hwnd )
{
    int FrameIndex, i, frame, count;
    char tmpStr[100];
    
    // Make sure we have some frames in this list
    if( !ListBox_GetCount( hSubCtrl[LST_FRAMES] ) )
        return; 

    // Make sure we have a selection
    FrameIndex = ListBox_GetCurSel( hSubCtrl[LST_FRAMES] );
    if( FrameIndex == LB_ERR )
        return;

    // Display the dialog box to get the users sound directory
    MediaDir_GetSoundDirectory( hInst, hwnd, Settings.GetSoundDir(), hSubCtrl[LST_FRAMES],
                                    StoryLstAry.GetStoryList( activeScript )->GetSndLst( FrameIndex ),
                                    StoryLstAry.GetSoundListStr( activeScript ) );

    // Clear out the frame list box
    ListBox_ResetContent( hSubCtrl[ LST_FRAMES ] );

    // Add animation name to create animation list box
    for( i = 0; i < StoryLstAry.GetStoryList( activeScript )->Count(); ++i )
    {
        // Get the frame
        frame = StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( i );
        
        // Do we have any sound properties attached to this frame
        if( (count = StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Count()) > MAX_ASTERICK-1 )
            count = MAX_ASTERICK-1;
                                
        // Show the number of sounds attached to this frame
        wsprintf( tmpStr, "MNG Frame Index %u   %s", frame, Asterick[count] );

        // Add the string
        ListBox_AddString( hSubCtrl[ LST_FRAMES ], tmpStr );
    }

}   // Mnu_GetSoundProperties


/************************************************************************
*    FUNCTION NAME:         MnuClk_SaveAllAciveScripts                                                             
*
*    DESCRIPTION:           Save all the loaded scripts
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void CMNGanimWnd::MnuClk_SaveAllAciveScripts( HWND hwnd )
{
    int i;

    for( i = 0; i < StoryLstAry.Count(); ++i )
    {        
        if( !StoryLstAry.SaveToFile( i ) )
        {
            PostMsg( "Save Error", "There was an error saving script %s", StoryLstAry.GetPtrToScriptName( i ) );
            break;
        }
    }

}   // MnuClk_SaveAllAciveScripts


/************************************************************************
*    FUNCTION NAME:         BtnClk_CreateScript                                                             
*
*    DESCRIPTION:           Create a script file
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

void CMNGanimWnd::BtnClk_CreateScript( HWND hwnd )
{
    int index, i;
    char tmpScriptName[ MAX_SCRIP_NAME_SIZE + 5 ];
    char filePath[MAX_PATH*2], MngFileName[MAX_PATH*2], PathToFile[MAX_PATH*2];

    // Make sure we have a selection to the MNG file
    index = ListBox_GetCurSel( GetDlgItem( hwnd, LST_MNG_FILES ) );
    if( index == LB_ERR )
    {
        PostMsg( "No Selection", "Oop, you need to select an MNG file from the MNG File List box to create a script for." );
        return;
    }

    // Strip out the path to the mng file
    CropOutPath( mngPathList.GetString( index ), PathToFile );

    // Get the name of the script to create
    if( Display_GetNameDlg( hInst, hwnd, tmpScriptName, PathToFile, &scriptPathList ) )
    {
        // Add the file extension to the script name
        strcat( tmpScriptName, SCRIPT_EXT );
        
        // Create the path to the file
        wsprintf( filePath, "%s\\%s", PathToFile, tmpScriptName );

        // Make sure the file doesn't allready exsist
        if( !FileExist( filePath ) )
        {
            // Get the file name of the selected MNG file
            ListBox_GetText( GetDlgItem( hwnd, LST_MNG_FILES ), index, MngFileName );

            // Add a script file
            StoryLstAry.Add();

            // Set the script members

            // Save the script name
            strcpy( StoryLstAry.GetPtrToScriptName( StoryLstAry.Count()-1 ), filePath );

            // Save the MNG file name
            strcpy( StoryLstAry.GetPtrToMngName( StoryLstAry.Count()-1 ), MngFileName );

            // Add the new script to the list
            scriptPathList.Add( filePath );

            // Sort the paths
            scriptPathList.SortByFileName();

            // Reset the list boxes
            ListBox_ResetContent( hSubCtrl[ LST_SCRIPT_FILES ] );

            // Display all the strings
            for( i = 0; i < scriptPathList.Count(); ++i )
            {
                CropFileNameFromPath( scriptPathList.GetString( i ), tmpScriptName );
                ListBox_AddString( hSubCtrl[ LST_SCRIPT_FILES ], tmpScriptName );
            }
        }
        else
            PostMsg( "Script Creation Error", "The script file you are trying to create already exists." );
    }

}   // BtnClk_CreateScript


/************************************************************************
*    FUNCTION NAME:         BtnClk_AddScriptToAnimate                                                             
*
*    DESCRIPTION:           Add an script to the list to animate
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

void CMNGanimWnd::BtnClk_AddScriptToAnimate( HWND hwnd )
{
    int AnimIndex, ListBoxCount, i;
    char PromptStr[ MAX_SCRIP_NAME_SIZE+5 ];

    if( activeScript > -1 )
    {
        // Make sure we have a selection to the animation
        AnimIndex = ListBox_GetCurSel( hSubCtrl[ LST_SCRIPT_FILES ] );
        if( AnimIndex == LB_ERR )
            return;

        // Make sure this animation has some storyboard in it
        if( !StoryLstAry.GetStoryList( activeScript )->Count() )
            goto HANDLE_ERROR;

        // Go through all the strings in the anim to run list box to 
        // be sure we are not adding duplicate animations
        for( i = 0; i < scriptAnimList.Count(); ++i )
        {
            if( strcmp( StoryLstAry.GetPtrToScriptName( activeScript ), scriptAnimList.GetString( i ) ) == 0 )
                goto HANDLE_ERROR;
        }

        // Get the script name from the list box
        ListBox_GetText( hSubCtrl[ LST_SCRIPT_FILES ], AnimIndex, PromptStr );

        // Add animation name to create animation list box
        ListBox_AddString( hSubCtrl[ LST_SCR_TO_ANIM ], PromptStr );

        // If the list box has no items in it then we just added one
        // so enable all the controls
        if( scriptAnimList.Count() == 0 )
        {
            Edit_Enable( GetDlgItem( hwnd, EDT_FPS ), TRUE );
            Button_Enable( GetDlgItem( hwnd, BTN_PLAY_SCRIPTS ), TRUE );
            Static_Enable( GetDlgItem( hwnd, STC_FRAMES_PER_SEC ), TRUE );
            ListBox_Enable( hSubCtrl[ LST_SCR_TO_ANIM ], TRUE );
            ListBox_Enable( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), TRUE );
        }

        // Save the script path for the list box
        scriptAnimList.Add( StoryLstAry.GetPtrToScriptName( activeScript ) );

        // Select the list box item
        ListBox_SetCurSel( hSubCtrl[ LST_SCR_TO_ANIM ], scriptAnimList.Count()-1 );

        // Select the newly added item
        ListClk_ScrptToAnimLst( hwnd, hSubCtrl[ LST_SCR_TO_ANIM ], CBN_SELCHANGE );
    }

HANDLE_ERROR:;

}   // BtnClk_AddScriptToAnimate


/************************************************************************
*    FUNCTION NAME:         BtnClk_PlayScripts                                                             
*
*    DESCRIPTION:           Play the selected animations
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

void CMNGanimWnd::BtnClk_PlayScripts( HWND hwnd )
{
    int AnimToRunCount, i, j, StoryBorardCount=0;
    int sty, frm, snd, sndID;
    char AnimSpeed[20], *pSoundFile;

    // Make sure there is a number specifing the frames per second
    if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_FPS ) ) )
    {
        if( !TimerReturnCode )
        {
            // Free all items from the int list
            zOrderList.Free();

            // Go through and find the indexes of these animations
            for( i = 0; i < scriptAnimList.Count(); ++i )
            {
                for( j = 0; j < StoryLstAry.Count(); ++j )
                {                    
                    // If we have a match, save the animation index to use later
                    if( strcmp( StoryLstAry.GetPtrToScriptName( j ), scriptAnimList.GetString( i ) ) == 0 )
                    {
                        // Get the index of the animation to run
                        zOrderList.Add( j );

                        // Take this moment to set the frame and disply counter
                        // Handle the frame index depending on how they want to animat it
                        switch( StoryLstAry.GetAnimType( j ) )
                        {
                            case PLAY_FORWARD_STAY:
                            case PLAY_FORWARD_HIDE:
                            case PLAY_FORWARD_LOOP:
                            case PLAY_PING_PONG:
                                StoryLstAry.SetFrameCounter( j, 0 );
                                StoryLstAry.SetDisplayTimer( j, 1 );
                            break;

                            case PLAY_REVERSE_STAY:
                            case PLAY_REVERSE_HIDE:
                            case PLAY_REVERSE_LOOP:
                                StoryLstAry.SetFrameCounter( j, StoryLstAry.GetStoryList( j )->Count()-1 );
                                StoryLstAry.SetDisplayTimer( j, 1 );
                            break;
                        }

                        // Set the flag
                        StoryLstAry.SetKillAnim( j, FALSE );
                        StoryLstAry.SetPingPongDir( j, 1 );

                        break;
                    }
                }
            }

            if( Settings.GetPlaySounds() )
            {
                // Load the sound files that need to play
                for( i = 0; i < zOrderList.Count(); ++i )
                {
                    // Get the story list index
                    sty = zOrderList.GetIndex( i );

                    for( frm = 0; frm < StoryLstAry.GetStoryList( sty )->Count(); ++frm )
                    {
                        for( snd = 0; snd < StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->Count(); ++snd )
                        {
                            // Init the MCI window
                            StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->GetMP( snd )->Init( hwnd, hInst, DONT_USE_ERROR_DLG_BOXES );

                            // Get the sound ID
                            sndID = StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->GetID( snd );
                            
                            // Get a pointer to the sound file
                            pSoundFile = StoryLstAry.GetSoundListStr( sty )->GetString( sndID );

                            // Load the selected sound file
                            StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->GetMP( snd )->Load( pSoundFile );
                        }
                    }
                }
            }

            // Get the frames per second
            Edit_GetText( GetDlgItem( hwnd, EDT_FPS ), AnimSpeed, sizeof(AnimSpeed) );

            // Do we have animations to run
            if( zOrderList.Count() )
            {
                // Start the timer
                TimerReturnCode = (int)SetTimer( hSubCtrl[ STC_VIEW ], ANIM_TIMER_HANDLE, 1000 / atoi( AnimSpeed ), (TIMERPROC)PlayAnimTimer );

                // Did we get a timer?
                if( !TimerReturnCode )
                    PostMsg( "Error", "Error getting timer handle to run animations. Windows may be out of timers. Try restarting Windows to see if that helps." );
                else
                    Button_SetText( GetDlgItem( hwnd, BTN_PLAY_SCRIPTS ), "Stop" );
            } 
        }
        else
        {
            if( Settings.GetPlaySounds() )
            {
                // Go through everything and unload all the MCI windows
                for( i = 0; i < zOrderList.Count(); ++i )
                {
                    // Get the story list index
                    sty = zOrderList.GetIndex( i );

                    for( frm = 0; frm < StoryLstAry.GetStoryList( sty )->Count(); ++frm )
                    {
                        for( snd = 0; snd < StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->Count(); ++snd )
                        {
                            // Delete the MCI window
                            StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->GetMP( snd )->Free();
                        }
                    }
                }
            }

            // Kill the timer
            KillTimer( hSubCtrl[ STC_VIEW ], ANIM_TIMER_HANDLE );

            // Reset the button label
            Button_SetText( GetDlgItem( hwnd, BTN_PLAY_SCRIPTS ), "Play" );
            TimerReturnCode = 0;
        }
    }

}   // BtnClk_PlayScripts


/************************************************************************
*    FUNCTION NAME:         BtnClk_AddFrame                                                             
*
*    DESCRIPTION:           Add MNG frame as animation frame
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

void CMNGanimWnd::BtnClk_AddFrame( HWND hwnd )
{
    int i, ListBoxCount, AnimIndex, FrameIndex, FrameCount, InsertCount=0, frame, count;
    char tmpStr[ 100 ];
    TDIB loadImage;
    BOOL InsertFrame=FALSE;

    // Need to know how many items in the list box
    ListBoxCount = ListBox_GetCount( GetDlgItem( hwnd, LST_AVAILABLE_MNG ) );

    // Make sure we have a selection to one of the MNG frames
    AnimIndex = ListBox_GetCurSel( GetDlgItem( hwnd, LST_AVAILABLE_MNG ) );
    if( AnimIndex == LB_ERR )
        return;

    // Get the index of the selected frame in case they want to insert
    FrameIndex = ListBox_GetCurSel( hSubCtrl[ LST_FRAMES ] );

    // See if they are holding down the Ctrl key
    // See if the high byte has been set
    if( HIBYTE( GetAsyncKeyState( VK_INSERT ) ) )
        InsertFrame = TRUE;

    // Go through the list and add the frames
    for( i = 0; i < ListBoxCount; ++i )
    {
        // Has this MNG frame been selected?
        if( ListBox_GetSel( GetDlgItem( hwnd, LST_AVAILABLE_MNG ), i ) > 0 )
        {
            // Add an entry to the storyboard list
            StoryLstAry.GetStoryList( activeScript )->Add();

            // Get the number of frames in this animation
            FrameCount = StoryLstAry.GetStoryList( activeScript )->Count();

            // Set the frame being used
            StoryLstAry.GetStoryList( activeScript )->SetFrameIndex( FrameCount-1, i );
            
            // Compose the text entry
            wsprintf( tmpStr, "MNG Frame Index %u", i );

            // Move the frame to the new position
            if( InsertFrame )
            {
                StoryLstAry.GetStoryList( activeScript )->MoveToNewPos( FrameCount-1, FrameIndex + InsertCount );
                ++InsertCount;
            }
            else
                // Add frame to the animation frame list box
                ListBox_AddString( hSubCtrl[ LST_FRAMES ], tmpStr );
        }
    }

    if( InsertFrame )
    {
        // Clear out the frame list box
        ListBox_ResetContent( hSubCtrl[ LST_FRAMES ] );

        // Load the list box with the remaining frames
        for( i = 0; i < FrameCount; ++i )
        {
            // Get the frame
            frame = StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( i );
            
            // Do we have any sound properties attached to this frame
            if( (count = StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Count()) > MAX_ASTERICK-1 )
                count = MAX_ASTERICK-1;
                                    
            // Show the number of sounds attached to this frame
            wsprintf( tmpStr, "MNG Frame Index %u   %s", frame, Asterick[count] );

            // Add the string
            ListBox_AddString( hSubCtrl[ LST_FRAMES ], tmpStr );
        }

        // Select the list box item
        ListBox_SetSel( hSubCtrl[ LST_FRAMES ], TRUE, FrameIndex );
    }

}   // BtnClk_AddFrame


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnCreate message handler.
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

BOOL CMNGanimWnd::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    BOOL Result = FALSE;
    RECT r;
    int i;

    // Set the dialog windows caption icon to the app icon
    SetClassLong( hwnd, GCL_HICON, (LONG) LoadIcon( hInst, "AppIcon" ) );

    // Save the handle to the parent window
    hParentWnd = hwnd;

    // initilize the MNG class
    if( !mng.Initialize( hwnd, RT_ALLOCATE_BITMAP_LIST, USE_BK_BUFFER ) )
        goto HANDLE_ERROR;

    // Get the settings in the ini file
    Settings.GetPrivateString();

    // Get the rect of the midia window
    GetWindowRect( hwnd, &r );
    r.right -= r.left;
    r.bottom -= r.top;

    // Center the window if it is way off the screen
    if( Settings.GetWindowX() < -r.right || Settings.GetWindowY() < -r.bottom ||
        Settings.GetWindowX() > GetSystemMetrics( SM_CXSCREEN ) || Settings.GetWindowY() > GetSystemMetrics( SM_CYSCREEN ) )
    {
        CenterWindow( hwnd, r.right, r.bottom );
    }
    else
        // Put a window at a given position and size
        MoveWindow( hwnd, Settings.GetWindowX(), Settings.GetWindowY(), r.right, r.bottom, TRUE );

    // Zero out the array
    memset( OldSubCtrlProc, 0, sizeof(OldSubCtrlProc) );

    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS; ++i )
    {
        // Save the control handle for later use
        hSubCtrl[ i ] = GetDlgItem( hwnd, EDT_XOFSET + i );

        // Subclass the windows control
        OldSubCtrlProc[ i ] = (WNDPROC)SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, 
                                                      LONG( SubClassProc ) );

        // We had an error
        if( !OldSubCtrlProc[ i ] )
            goto HANDLE_ERROR;
    }

    // Create the buffer
    if( !ViewBuffer.CreateStandardBuffer( SCREEN_W, SCREEN_H ) )
        goto HANDLE_ERROR;

    // Create the background
    if( !BackBuffer.CreateStandardBuffer( SCREEN_W, SCREEN_H ) )
        goto HANDLE_ERROR;

    // Set the rect
    SetRect( &r, 0, 0, SCREEN_W, SCREEN_H );

    // Color the buffers
    ViewBuffer.ColorRect( &r, WHITE_BRUSH );
    BackBuffer.ColorRect( &r, WHITE_BRUSH );

    // Get the rect of our view window
    GetClientRect( hSubCtrl[ STC_VIEW ], &ViewRect );

    // Get the rect of the preview window
    GetClientRect( GetDlgItem( hwnd, STC_PREVIEW_WND ), &PreviewRect );

    // Create the preview window buffer
    if( !PreviewBuffer.CreateStandardBuffer( PreviewRect.right, PreviewRect.bottom ) )
        goto HANDLE_ERROR;

    // Color the buffer
    PreviewBuffer.ColorRect( &PreviewRect, (COLORREF)GetSysColor( COLOR_BTNFACE ) );

    // Update the window caption
    SetWindowCaption( hwnd );

    // Set the frames per sec edit field
    Edit_DisplayValue( GetDlgItem( hwnd, EDT_FPS ), Settings.GetAnimFrameCount() );

    // Update the list boxes with the file names
    UpdateFileListBoxes( hwnd );

    // Do we check the Paly Sounds menu
    if( Settings.GetPlaySounds() )
        CheckMenuItem( GetMenu( hwnd ), MNU_PLAY_SOUNDS_ANIM, MF_CHECKED );

    // Add the items to the list box
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Play Forward - Last Frame Stay" );
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Play Forward - Last Frame Hide" );
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Play Forward - Loop" );
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Play Reverse - Last Frame Stay" );
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Play Reverse - Last Frame Hide" );
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Play Reverse - Loop" );
    ListBox_AddString( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), "Ping Pong" );

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        PostQuitMessage( 0 );

    return Result;

}   // Starter_OnCreate


/************************************************************************
*    FUNCTION NAME:         SetWindowCaption                                                             
*
*    DESCRIPTION:           Set the window caption
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

void CMNGanimWnd::SetWindowCaption( HWND hwnd )
{
    char Caption[MAX_PATH*2], *ProjFilePath;

    // Get the path to the project
    ProjFilePath = Settings.GetProjectPath();

    if( ProjFilePath[0] )
    {
        // Build a new caption
        wsprintf( Caption, "%s  -  Project Path:  %s", PRODUCT_CAPTION, ProjFilePath );
    } 
    else
        strcpy( Caption, PRODUCT_NAME );

    // Update the windo caption
    SetWindowText( hwnd, Caption );

}   // SetWindowCaption


/************************************************************************
*    FUNCTION NAME:         MnuClk_ClearBackground                                                             
*
*    DESCRIPTION:           Clear the background
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void CMNGanimWnd::MnuClk_ClearBackground( HWND hwnd )
{
    // Set the flag
    BackgroundLoaded = FALSE;

    // Color the background white
    BackBuffer.ColorRect( &ViewRect, WHITE_BRUSH );

    // Update the buffer
    UpdateViewBuffer( hwnd );

    // Invalidate the whole window so it all gets painted
    InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );

}   // MnuClk_ClearBackground


/************************************************************************
*    FUNCTION NAME:         MnuClk_LoadBackground                                                             
*
*    DESCRIPTION:           Load a file to use as the background
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void CMNGanimWnd::MnuClk_LoadBackground( HWND hwnd )
{
    char FileName[MAX_PATH*2];
    BYTE *pLoadPtr;
    TGif gif;
    TJPEG jpeg;
    TDIB dib;
    CMNG tmpMng;

    // Needs to be terminated for the common dialogs to work
    FileName[0] = 0;

    if( PictLoad_GetPicture( hInst, hwnd, "Select an image to load as the background", OFN_NOCHANGEDIR, 
                                  FileName, MAX_PATH * 2, PAGE_FILE_TYPES, Settings.GetProjectPath() ) )
    {
        // Get the type of file so we know how to load it
        switch( GetFileType( FileName ) )
        {
            case FILE_TYPE_BMP:
                dib.LoadFromFile( FileName );
            break;

            case FILE_TYPE_JPG:
                // First load and decode the image
                pLoadPtr = jpeg.LoadFromFile( FileName );

                // Then pass it along to the DIB class
                dib.LoadFromBuffer( pLoadPtr, jpeg.GetJPGDataSize() );
            break;

            case FILE_TYPE_GIF:
                // First load and decode the image
                pLoadPtr = gif.LoadFromFile( FileName );

                // Then pass it along to the DIB class
                dib.LoadFromBuffer( pLoadPtr, gif.GetGifDataSize() );
            break;

            case FILE_TYPE_PNG:

                // initilize the MNG class
                if( !tmpMng.Initialize( hwnd, RT_ALLOCATE_BITMAP_LIST, NO_BK_BUFFER ) )
                    return;

                // First load and decode the image
                if( !tmpMng.LoadFromFile( FileName ) )
                    return;

                // Then pass it along to the DIB class
                if( tmpMng.Count() )
                    dib.CopyDIB( tmpMng.GetDIB( 0 ) );
            break;
        }

        // Blit to the backgrounf buffer
        if( !dib.IsEmpty() )
        {
            // Set the flag
            BackgroundLoaded = TRUE;

            // Blit the image to the background
            dib.BlitToDC( BackBuffer.Handle() );

            // Update the buffer
            UpdateViewBuffer( hwnd );

            // Invalidate the whole window so it all gets painted
            InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
        }

    }
}   // MnuClk_LoadBackground


/************************************************************************
*    FUNCTION NAME:         MnuClk_OpenProjPath                                                             
*
*    DESCRIPTION:           Open path to project
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void CMNGanimWnd::MnuClk_OpenProjPath( HWND hwnd )
{
    char tmpDirPath[MAX_PATH*2];
    BROWSEINFO bi; 
    LPITEMIDLIST pidlBrowse;

    // Inti the browser structer
    bi.hwndOwner = hwnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = tmpDirPath; // Only returns the selected folder name, not the path
    bi.lpszTitle = "Select the folder that has your MNG and script files.";
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;

    // Show the get folder dialog box
    if( (pidlBrowse = SHBrowseForFolder( &bi )) )
    {
        // Save all the active scripts
        MnuClk_SaveAllAciveScripts( hwnd );
        
        // Free the class because we are changing to a different directory
        StoryLstAry.Free();

        // Init the active script
        activeScript = -1;
        
        // Update the buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
        
        // Color the buffer
        PreviewBuffer.ColorRect( &PreviewRect, (COLORREF)GetSysColor( COLOR_BTNFACE ) );
        
        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_PRE_VIEW ], &PreviewRect, TRUE );    
        
        // Use the returned item list to get the path to the folder
        SHGetPathFromIDList( pidlBrowse, tmpDirPath );
        
        // Save the file path
        strcpy( Settings.GetProjectPath(), tmpDirPath );

        // Update the list boxes with the file names
        UpdateFileListBoxes( hwnd );

        // Update the window caption
        SetWindowCaption( hwnd );
    }

}   // MnuClk_OpenProjPath


/************************************************************************
*    FUNCTION NAME:         GetAllFileNamesInDir                                                             
*
*    DESCRIPTION:           Gets all the file names via a suplied 
*                           extension, in a give directory
*
*    FUNCTION PARAMETERS:
*    Input:    PTStringList pStrList - Pointer to a string list object
*                                      to fill up with strings
*              char *Dir - Directory to look in
*              char *Ext - Extension to wild cards to use like *.* or *.wav 
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

void CMNGanimWnd::GetAllFileNamesInDir( char *Dir, char *Ext, HWND hListCtrl )
{
    WIN32_FIND_DATA FindData;
    char StrBuffer[MAX_PATH*2];
    HANDLE hSearch;

    // Make sure none of the string params are empty or NULL
    if( Dir && Dir[0] && Ext && Ext[0] )
    {
        // Init the search string
        strcpy( StrBuffer, Dir );
        strcat( StrBuffer, "\\" );
        strcat( StrBuffer, Ext );

        // Find the first file
        hSearch = FindFirstFile( StrBuffer, &FindData );

        if( hSearch != INVALID_HANDLE_VALUE )
        {
            do
            {
                // Just be sure not to copy any directory names
                if( !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                    // Add the file name to the list box
                    ListBox_AddString( hListCtrl, FindData.cFileName );
            }
            while( FindNextFile( hSearch, &FindData ) );

            FindClose( hSearch );
        }
    }

}   // GetAllFileNamesInDir


/************************************************************************
*    FUNCTION NAME:         UpdateFileListBoxes                                                             
*
*    DESCRIPTION:           Update the list boxes that show the file names
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void CMNGanimWnd::UpdateFileListBoxes( HWND hwnd )
{
    char *ProjFilePath, fileName[MAX_PATH*2];
    int i;
    HWND hListBox;
    TStringList dirList;

    // Get the path to the project
    ProjFilePath = Settings.GetProjectPath();

    if( ProjFilePath[0] )
    {
        // Reset the list boxes
        ListBox_ResetContent( GetDlgItem( hwnd, LST_MNG_FILES ) );
        ListBox_ResetContent( GetDlgItem( hwnd, LST_MNG_SCRIPT_FILES ) );
        ListBox_ResetContent( GetDlgItem( hwnd, LST_MNG_FRAMES ) );
        ListBox_ResetContent( GetDlgItem( hwnd, LST_AVAILABLE_MNG ) );
        ListBox_ResetContent( GetDlgItem( hwnd, LST_ANIM_FRAMES ) );
        ListBox_ResetContent( GetDlgItem( hwnd, LST_SCRIPTS_TO_ANIM ) );

        // Clear out the paths
        Static_SetText( GetDlgItem( hwnd, STC_MNG_PATH ), "\\.." );
        Static_SetText( GetDlgItem( hwnd, STC_SCRIPT_PATH ), "\\.." );

        // Clear out the lists
        mngPathList.Free();
        scriptPathList.Free();
        scriptAnimList.Free();

        // Find all the folders and sub folders
        dirList.GetAllFoldersInDir( Settings.GetProjectPath(), TRUE );

        // Get all the files in the top level directory
        mngPathList.GetAllFileNamesInDir( Settings.GetProjectPath(), "*.mng" );

        // Find any files in the underlying sub directories
        for( i = 0; i < dirList.Count(); ++i )
            mngPathList.GetAllFileNamesInDir( dirList.GetString( i ), "*.mng" );

        // Sort the paths
        mngPathList.SortByFileName();

        // Get the handle to the list box
        hListBox = GetDlgItem( hwnd, LST_MNG_FILES );

        // Display all the strings
        for( i = 0; i < mngPathList.Count(); ++i )
        {
            CropFileNameFromPath( mngPathList.GetString( i ), fileName );
            ListBox_AddString( hListBox, fileName );
        }

        // Get all the files in the top level directory
        scriptPathList.GetAllFileNamesInDir( Settings.GetProjectPath(), SCRIPT_EXT2 );

        // Find any files in the underlying sub directories
        for( i = 0; i < dirList.Count(); ++i )
            scriptPathList.GetAllFileNamesInDir( dirList.GetString( i ), SCRIPT_EXT2 );

        // Sort the paths
        scriptPathList.SortByFileName();

        // Display all the strings
        for( i = 0; i < scriptPathList.Count(); ++i )
        {
            CropFileNameFromPath( scriptPathList.GetString( i ), fileName );

            ListBox_AddString( hSubCtrl[ LST_SCRIPT_FILES ], fileName );
        }
    }

}   // UpdateFileListBoxes


/************************************************************************
*    FUNCTION NAME:         ListClk_MNGfileList                                                             
*
*    DESCRIPTION:           Click handler for the MNG file list
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_MNGfileList( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int index, i; 
    char PathToFile[MAX_PATH*2], frameName[50];

    if( codeNotify == CBN_SELCHANGE )
    {            
        // Make sure we have a selection to the animation
        index = ListBox_GetCurSel( hwndCtl );
        if( index == LB_ERR )
            return;
            
        // Clear out the list box
        ListBox_ResetContent( GetDlgItem( hwnd, LST_MNG_FRAMES ) );

        // Strip out the path to the mng file
        CropOutPath( mngPathList.GetString( index ), PathToFile );

        // Show the path
        if( strlen( PathToFile ) == strlen( Settings.GetProjectPath() ) )
            Static_SetText( GetDlgItem( hwnd, STC_MNG_PATH ), "\\.." );
        else
            Static_SetText( GetDlgItem( hwnd, STC_MNG_PATH ), PathToFile + strlen( Settings.GetProjectPath() ) );

        // Try to load the MNG file
        if( mng.LoadFromFile( mngPathList.GetString( index ) ) )
        {
            // If all is ok add frames to the list
            for( i = 0; i < mng.Count(); ++i )
            {
                // Pick a format depending on the number of frames
                if( i < 10 )
                    wsprintf( frameName, "Frame_00%u", i );

                else if( i < 100 )
                    wsprintf( frameName, "Frame_0%u", i );

                else
                    wsprintf( frameName, "Frame_%u", i );

                // Add the list to the list box
                ListBox_AddString( GetDlgItem( hwnd, LST_MNG_FRAMES ), frameName );
            }

            // Select the first frame in the list and draw it
            if( mng.Count() )
            {
                // Select the list box item
                ListBox_SetCurSel( GetDlgItem( hwnd, LST_MNG_FRAMES ), 0 );

                // Force a click message
                ListClk_MNGfileFrames( hwnd, GetDlgItem( hwnd, LST_MNG_FRAMES ), CBN_SELCHANGE );
            }
        }
        else  // There was an error
        {
            // Color the buffer
            PreviewBuffer.ColorRect( &PreviewRect, (COLORREF)GetSysColor( COLOR_BTNFACE ) );

            // Invalidate the whole window so it all gets painted
            InvalidateRect( hSubCtrl[ STC_PRE_VIEW ], &PreviewRect, TRUE );
        }
    }

}   // ListClk_MNGfileList


/************************************************************************
*    FUNCTION NAME:         ListClk_MNGfileFrames                                                             
*
*    DESCRIPTION:           Click handler for the MNG file frames list
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_MNGfileFrames( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int index; 
    char fileName[MAX_PATH*2], PathToFile[MAX_PATH*2], frameName[50];

    if( codeNotify == CBN_SELCHANGE )
    {            
        // Make sure we have a selection to the animation
        index = ListBox_GetCurSel( hwndCtl );
        if( index == LB_ERR )
            return;
            
        // Blit the image to the buffer
        BlitImageToBuffer( mng.GetDIB( index ), &PreviewBuffer, &PreviewRect, GetDlgItem( hwnd, STC_PREVIEW_INFO ), mng.Count() );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_PRE_VIEW ], &PreviewRect, TRUE );
    }

}   // ListClk_MNGfileFrames


/************************************************************************
*    FUNCTION NAME:         ListClk_ScriptLst                                                             
*
*    DESCRIPTION:           Click handler for the MNG file frames list
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_ScriptLst( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int index, i, frame, count; 
    char scriptPath[MAX_PATH*2], PathToFile[MAX_PATH*2];
    char tmpStr[ 100 ];
    BOOL scriptFound = FALSE, IndexNotAvailable = FALSE;

    if( codeNotify == CBN_SELCHANGE )
    {            
        // Make sure we have a selection to the animation
        index = ListBox_GetCurSel( hwndCtl );
        if( index == LB_ERR )
            return;

        // Clear out the other list boxes
        ListBox_ResetContent( GetDlgItem( hwnd, LST_AVAILABLE_MNG ) );
        ListBox_ResetContent( hSubCtrl[ LST_FRAMES ] );

        // Clear out the base offsets
        Edit_SetText( hSubCtrl[ EDT_BASE_X ], "" );
        Edit_SetText( hSubCtrl[ EDT_BASE_Y ], "" );

        // Build the path to the script file
        CropOutPath( scriptPathList.GetString( index ), scriptPath );

        // See if this script is already loaded
        for( i = 0; i < StoryLstAry.Count(); ++i )
        {
            // If we found the script, save the index and break out
            if( strcmp( StoryLstAry.GetPtrToScriptName( i ), scriptPathList.GetString( index ) ) == 0 )
            {
                activeScript = i;
                scriptFound = TRUE;
                break;  
            }
        }

        // Load the script if we didn't find it
        if( !scriptFound )
        {                    
            // Load the file
            if( !StoryLstAry.LoadFromFile( scriptPathList.GetString( index ) ) )
                // Return because we had an error
                return;

            // Set the active script index to the last index
            activeScript = StoryLstAry.Count() - 1;
        }

        // Do we need to load the MNG file?
        if( !StoryLstAry.GetPtrToMng( activeScript )->Count() )
        {
            wsprintf( PathToFile, "%s\\%s", scriptPath, StoryLstAry.GetPtrToMngName( activeScript ) );

            // Init the MNG class
            if( !StoryLstAry.GetPtrToMng( activeScript )->Initialize( hwnd, RT_ALLOCATE_BITMAP_LIST, NO_BK_BUFFER ) )
                // Return because we had an error
                return;

            // Load the mng file
            if( StoryLstAry.GetPtrToMng( activeScript )->LoadFromFile( PathToFile ) )
                // Free the mng data because we are using allocated bitmaps from this point
                StoryLstAry.GetPtrToMng( activeScript )->FreeMngLibary();
            else
                // Return because we had an error
                return;
        }

        // List out all the available mng frames
        for( i = 0; i < StoryLstAry.GetPtrToMng( activeScript )->Count(); ++i )
        {
            // Pick a format depending on the number of frames
            if( i < 10 )
                wsprintf( tmpStr, "Frame_00%u", i );

            else if( i < 100 )
                wsprintf( tmpStr, "Frame_0%u", i );

            else
                wsprintf( tmpStr, "Frame_%u", i );

            // Add the list to the list box
            ListBox_AddString( GetDlgItem( hwnd, LST_AVAILABLE_MNG ), tmpStr );
        }
        
        // List out all the animation frames
        for( i = 0; i < StoryLstAry.GetStoryList( activeScript )->Count(); ++i )
        {
            // Get the frame
            frame = StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( i );
            
            // Do we have any sound properties attached to this frame
            if( (count = StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Count()) > MAX_ASTERICK-1 )
                count = MAX_ASTERICK-1;
                                    
            // Show the number of sounds attached to this frame
            wsprintf( tmpStr, "MNG Frame Index %u   %s", frame, Asterick[count] );

            // Add the string
            ListBox_AddString( hSubCtrl[ LST_FRAMES ], tmpStr );

            // Check each frame to see that the index being used is in the MNG
            if( frame >= StoryLstAry.GetPtrToMng( activeScript )->Count() || frame < 0 )
                IndexNotAvailable = TRUE;
        }

        // If we have some frames, select the first one
        if( StoryLstAry.GetStoryList( activeScript )->Count() )
        {
            // Select the list box item
            ListBox_SetSel( hSubCtrl[ LST_FRAMES ], TRUE, 0 );

            // Display the first item in the list
            ListClk_AnimFrameLst( hParentWnd, hSubCtrl[ LST_FRAMES ], CBN_SELCHANGE );
        }
        else  // If we don't have any frames, just do an update
        {
            // Clear out all the edit controls
            Edit_SetText( hSubCtrl[ EDT_FRAME_X ], "" );
            Edit_SetText( hSubCtrl[ EDT_FRAME_Y ], "" );
            Edit_SetText( hSubCtrl[ EDT_DIS_COUNT ], "" );
            Edit_SetText( hSubCtrl[ EDT_Z_ORD ], "" );

            // update the view buffer
            UpdateViewBuffer( hwnd );

            // Invalidate the whole window so it all gets painted
            InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
        }

        // Get the base X offset
        wsprintf( tmpStr, "%d", StoryLstAry.GetBaseXOffset( activeScript ) );
        Edit_SetText( hSubCtrl[ EDT_BASE_X ], tmpStr );

        // Get the base Y offset
        wsprintf( tmpStr, "%d", StoryLstAry.GetBaseYOffset( activeScript ) );
        Edit_SetText( hSubCtrl[ EDT_BASE_Y ], tmpStr );

        // Get the base Z offset
        wsprintf( tmpStr, "%d", StoryLstAry.GetBaseZOffset( activeScript ) );
        Edit_SetText( hSubCtrl[ EDT_BASE_Z ], tmpStr );
        
        // Tell them they have indexs being used that are not available
        if( IndexNotAvailable )
            PostMsg( "Script Error", "The selected script uses frame indexes the MNG file does not have." );
            
        // Display the script path
        if( strlen( scriptPath ) == strlen( Settings.GetProjectPath() ) )
            Static_SetText( GetDlgItem( hwnd, STC_SCRIPT_PATH ), "\\.." );
        else
            Static_SetText( GetDlgItem( hwnd, STC_SCRIPT_PATH ), scriptPath + strlen( Settings.GetProjectPath() ) );                            
    }

}   // ListClk_MNGfileFrames


/************************************************************************
*    FUNCTION NAME:         ListClk_AvailableMNGFrames                                                             
*
*    DESCRIPTION:           Click handler for the available MNG file frames list
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_AvailableMNGFrames( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int FrameIndex;

    if( codeNotify == CBN_SELCHANGE )
    {
        // Make sure we have some frames in this list box
        if( ListBox_GetCount( hwndCtl   ) )
        {
            // Make sure we have a selection
            FrameIndex = ListBox_GetCurSel( hwndCtl );
            if( FrameIndex == LB_ERR )
                return;

            // Clear out the MNG frames list box
            ListBox_ResetContent( GetDlgItem( hwnd, LST_MNG_FRAMES ) );

            // Blit the image to the buffer
            BlitImageToBuffer( StoryLstAry.GetPtrToMng( activeScript )->GetDIB( FrameIndex ), &PreviewBuffer, 
                               &PreviewRect, GetDlgItem( hwnd, STC_PREVIEW_INFO ),
                               StoryLstAry.GetPtrToMng( activeScript )->Count() );

            // Invalidate the whole window so it all gets painted
            InvalidateRect( hSubCtrl[ STC_PRE_VIEW ], &PreviewRect, TRUE );
        }
    }
}   // ListClk_AvailableMNGFrames


/************************************************************************
*    FUNCTION NAME:         ListClk_AnimFrameLst                                                             
*
*    DESCRIPTION:           Click handler for the frame list
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place 
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

void CMNGanimWnd::ListClk_AnimFrameLst( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int FrameIndex, MoveIndex, i, frame, count;
    char tmpStr[100];

    if( codeNotify == CBN_SELCHANGE )
    {
        // Make sure we have a selection
        FrameIndex = ListBox_GetCurSel( hwndCtl );
        if( FrameIndex == LB_ERR )
            return;                

        // Get the X offset
        Edit_DisplayValue( hSubCtrl[ EDT_FRAME_X ], StoryLstAry.GetStoryList( activeScript )->GetXOffset( FrameIndex ) );

        // Get the Y offset
        Edit_DisplayValue( hSubCtrl[ EDT_FRAME_Y ], StoryLstAry.GetStoryList( activeScript )->GetYOffset( FrameIndex ) );

        // Get the count
        Edit_DisplayValue( hSubCtrl[ EDT_DIS_COUNT ], StoryLstAry.GetStoryList( activeScript )->GetFrameRepeat( FrameIndex ) );

        // Get the Z Order
        Edit_DisplayValue( hSubCtrl[ EDT_Z_ORD ], StoryLstAry.GetStoryList( activeScript )->GetZOrder( FrameIndex ) );

        // update the view buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );

        if( activeScript > -1 && StoryLstAry.GetStoryList( activeScript )->Count() > 1 && HIBYTE( GetAsyncKeyState( VK_INSERT ) ) )
        {
            // Do they want to move to another index
            MoveIndex = DisplayMovePageDlg( hInst, hwnd, hwndCtl, FrameIndex, 
                                            StoryLstAry.GetStoryList( activeScript )->Count() );

            if( MoveIndex > -1 )
            {
                // Move the selected item to the new index
                StoryLstAry.GetStoryList( activeScript )->MoveToNewPos( FrameIndex, MoveIndex );

                // Clear out the frame list box
                ListBox_ResetContent( hwndCtl );

                // Add animation name to create animation list box
                for( i = 0; i < StoryLstAry.GetStoryList( activeScript )->Count(); ++i )
                {
                    // Get the frame
                    frame = StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( i );
                    
                    // Do we have any sound properties attached to this frame
                    if( (count = StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Count()) > MAX_ASTERICK-1 )
                        count = MAX_ASTERICK-1;
                                            
                    // Show the number of sounds attached to this frame
                    wsprintf( tmpStr, "MNG Frame Index %u   %s", frame, Asterick[count] );

                    // Add the string
                    ListBox_AddString( hwndCtl, tmpStr );
                }

                // Select the list box item
                ListBox_SetCurSel( hwndCtl, MoveIndex );
            }
        }
    }
}   // ListClk_AnimFrameLst


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

LRESULT CMNGanimWnd::HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result;

    /////////////////////////////////////////////////////
    // Each subclassed control has it's own message handler.
    // That's to keep this function as general and simple
    // as possible. You dig?
    /////////////////////////////////////////////////////

    // Handle the base X edit controls
    if( hCtrl == hSubCtrl[ EDT_BASE_X ] )
        Result = OnEdit_BaseXCtrl( hParentWnd, EDT_BASE_X, Message, wParam, lParam );

    // Handle the base Y edit controls
    else if( hCtrl == hSubCtrl[ EDT_BASE_Y ] )
        Result = OnEdit_BaseYCtrl( hParentWnd, EDT_BASE_Y, Message, wParam, lParam );

    // Handle the base Z edit controls
    else if( hCtrl == hSubCtrl[ EDT_BASE_Z ] )
        Result = OnEdit_BaseZCtrl( hParentWnd, EDT_BASE_Z, Message, wParam, lParam );

    // Handle the mesages of the view window
    else if( hCtrl == hSubCtrl[ STC_PRE_VIEW ] )
        Result = OnPreviewWnd( hParentWnd, STC_PRE_VIEW, Message, wParam, lParam );

    // Handle the mesages of the animation frame X control
    else if( hCtrl == hSubCtrl[ EDT_FRAME_X ] )
        Result = OnEdit_FrameXCtrl( hParentWnd, EDT_FRAME_X, Message, wParam, lParam );

    // Handle the mesages of the animation frame Y control
    else if( hCtrl == hSubCtrl[ EDT_FRAME_Y ] )
        Result = OnEdit_FrameYCtrl( hParentWnd, EDT_FRAME_Y, Message, wParam, lParam );

    // Handle the mesages of the animation frame count control
    else if( hCtrl == hSubCtrl[ EDT_DIS_COUNT ] )
        Result = OnEdit_FrameCountCtrl( hParentWnd, EDT_DIS_COUNT, Message, wParam, lParam );

    // Handle the mesages of the animation Z Order control
    else if( hCtrl == hSubCtrl[ EDT_Z_ORD ] )
        Result = OnEdit_FrameZOrderCtrl( hParentWnd, EDT_Z_ORD, Message, wParam, lParam );

    // Handle the mesages of the view window
    else if( hCtrl == hSubCtrl[ STC_VIEW ] )
        Result = OnViewWnd( hParentWnd, STC_VIEW, Message, wParam, lParam );

    // Handle the mesages of the animation frames list control
    else if( hCtrl == hSubCtrl[ LST_FRAMES ] )
        Result = OnList_AnimFramesCtrl( hParentWnd, LST_FRAMES, Message, wParam, lParam );

    // Message handler for the script to animate list control
    else if( hCtrl == hSubCtrl[ LST_SCR_TO_ANIM ] )
        Result = OnList_ScrptToAnimCtrl( hParentWnd, LST_SCR_TO_ANIM, Message, wParam, lParam );

    // Message handler for the script to animate list control
    else if( hCtrl == hSubCtrl[ LST_SCRIPT_FILES ] )
        Result = OnList_ScriptFilesCtrl( hParentWnd, LST_SCRIPT_FILES, Message, wParam, lParam );

    else
        Result = FALSE;

    return Result;

}   // HandleSubClassProc


/************************************************************************
*    FUNCTION NAME:         OnPreviewWnd                                                             
*
*    DESCRIPTION:           Message handler for the preview view window    
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

LRESULT CMNGanimWnd::OnPreviewWnd( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    LRESULT Result = FALSE;

    // Handle the paint message
    if( Message == WM_PAINT )
    {
        // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
        // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
        // message handler. You CAN NOT paint any other way. Even if you have
        // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
        // and EndPaint API calls or bad things will happen,
        PaintDC = BeginPaint( hSubCtrl[ Ctrl ], &paintStruct );

        // Blit the buffer to the view window
        if( !PreviewBuffer.IsEmpty() )
            PreviewBuffer.Draw( PaintDC );

        // Must be called
        EndPaint( hSubCtrl[ Ctrl ], &paintStruct );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnPreviewWnd


/************************************************************************
*    FUNCTION NAME:         OnEdit_FrameXCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation frame X control    
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

LRESULT CMNGanimWnd::OnEdit_FrameXCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int FrameCount, i;

    if( Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Make sure we have some items in the list box
        if( !(FrameCount = ListBox_GetCount( hSubCtrl[ LST_FRAMES ] )) )
            return Result;

        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list and update the frames
        for( i = 0; i < FrameCount; ++i )
        {
            // Has this frame been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_FRAMES ], i ) > 0 )
                // Record the new offset value
                StoryLstAry.GetStoryList( activeScript )->SetXOffset( i, atoi( StrValue ) );
        }

        // update the view buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_FrameXCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_FrameXCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation frame Y control    
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

LRESULT CMNGanimWnd::OnEdit_FrameYCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int AnimIndex, FrameCount, i;

    if( Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Make sure we have some items in the list box
        if( !(FrameCount = ListBox_GetCount( hSubCtrl[ LST_FRAMES ] )) )
            return Result;

        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list and update the frames
        for( i = 0; i < FrameCount; ++i )
        {
            // Has this frame been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_FRAMES ], i ) > 0 )
                // Record the new offset value
                StoryLstAry.GetStoryList( activeScript )->SetYOffset( i, atoi( StrValue ) );
        }

        // update the view buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_FrameYCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_BaseYCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation base X control    
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

LRESULT CMNGanimWnd::OnEdit_BaseXCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int i, j;

    if( activeScript > -1 && Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list. Only the scripts that have been clicked on are loaded
        for( i = 0; i < scriptPathList.Count(); ++i )
        {
            // Has this script been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_SCRIPT_FILES ], i ) > 0 )
            {
                // Find the loaded script
                for( j = 0; j < StoryLstAry.Count(); ++j )
                {
                    // If we found the script, save the index and break out
                    if( strcmp( StoryLstAry.GetPtrToScriptName( j ), scriptPathList.GetString( i ) ) == 0 )
                    {
                        // Record the new offset value
                        StoryLstAry.SetBaseXOffset( j, atoi( StrValue ) );
                        break;  
                    }
                }
            }
        }

        // update the view buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_BaseXCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_BaseYCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation base Y control    
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

LRESULT CMNGanimWnd::OnEdit_BaseYCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int i, j;

    if( activeScript > -1 && Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list. Only the scripts that have been clicked on are loaded
        for( i = 0; i < scriptPathList.Count(); ++i )
        {
            // Has this script been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_SCRIPT_FILES ], i ) > 0 )
            {
                // Find the loaded script
                for( j = 0; j < StoryLstAry.Count(); ++j )
                {
                    // If we found the script, save the index and break out
                    if( strcmp( StoryLstAry.GetPtrToScriptName( j ), scriptPathList.GetString( i ) ) == 0 )
                    {
                        // Record the new offset value
                        StoryLstAry.SetBaseYOffset( j, atoi( StrValue ) );
                        break;  
                    }
                }
            }
        }

        // update the view buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_BaseYCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_BaseZCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation base Z control    
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

LRESULT CMNGanimWnd::OnEdit_BaseZCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int i, j;

    if( activeScript > -1 && Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list. Only the scripts that have been clicked on are loaded
        for( i = 0; i < scriptPathList.Count(); ++i )
        {
            // Has this script been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_SCRIPT_FILES ], i ) > 0 )
            {
                // Find the loaded script
                for( j = 0; j < StoryLstAry.Count(); ++j )
                {
                    // If we found the script, save the index and break out
                    if( strcmp( StoryLstAry.GetPtrToScriptName( j ), scriptPathList.GetString( i ) ) == 0 )
                    {
                        // Record the new offset value
                        StoryLstAry.SetBaseZOffset( j, atoi( StrValue ) );
                        break;  
                    }
                }
            }
        }

        // update the view buffer
        UpdateViewBuffer( hwnd );

        // Invalidate the whole window so it all gets painted
        InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_BaseYCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_FrameCountCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation frame count control    
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

LRESULT CMNGanimWnd::OnEdit_FrameCountCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int AnimIndex, FrameCount, i;

    if( Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Make sure we have some items in the list box
        if( !(FrameCount = ListBox_GetCount( hSubCtrl[ LST_FRAMES ] )) )
            return Result;

        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list and update the frames
        for( i = 0; i < FrameCount; ++i )
        {
            // Has this frame been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_FRAMES ], i ) > 0 )
                // Record the new offset value
                StoryLstAry.GetStoryList( activeScript )->SetFrameRepeat( i, atoi( StrValue ) );
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_FrameCountCtrl


/************************************************************************
*    FUNCTION NAME:         OnEdit_FrameZOrderCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation frame sound ID control    
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

LRESULT CMNGanimWnd::OnEdit_FrameZOrderCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    char StrValue[ 20 ];
    LRESULT Result = FALSE;
    int AnimIndex, FrameCount, i;

    if( Message == WM_KEYUP && Edit_GetTextLength( hSubCtrl[ Ctrl ] ) )
    {
        // Make sure we have some items in the list box
        if( !(FrameCount = ListBox_GetCount( hSubCtrl[ LST_FRAMES ] )) )
            return Result;

        // Get the text from the edit control
        Edit_GetText( hSubCtrl[ Ctrl ], StrValue, sizeof( StrValue ) );

        // Go through the list and update the frames
        for( i = 0; i < FrameCount; ++i )
        {
            // Has this frame been selected?
            if( ListBox_GetSel( hSubCtrl[ LST_FRAMES ], i ) > 0 )
                // Record the new offset value
                StoryLstAry.GetStoryList( activeScript )->SetZOrder( i, atoi( StrValue ) );
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_FrameZOrderCtrl


/************************************************************************
*    FUNCTION NAME:         OnList_ScrptToAnimCtrl                                                             
*
*    DESCRIPTION:           Message handler for the script to animate list control    
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

LRESULT CMNGanimWnd::OnList_ScrptToAnimCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int AnimIndex, ListBoxCount;

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a selection to the animation
        AnimIndex = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
        if( AnimIndex == LB_ERR )
            return Result;

        // Remove the string from the list
        ListBox_DeleteString( hSubCtrl[ Ctrl ], AnimIndex );

        if( scriptAnimList.Count() )
            scriptAnimList.Delete( AnimIndex );

        // Need to know how many items in the list box
        ListBoxCount = ListBox_GetCount( hSubCtrl[ Ctrl ] );

        // If the list box has no items in it then disable the controls
        if( ListBoxCount == 0 )
        {
            Edit_Enable( GetDlgItem( hwnd, EDT_FPS ), FALSE );
            Button_Enable( GetDlgItem( hwnd, BTN_PLAY_SCRIPTS ), FALSE );
            Static_Enable( GetDlgItem( hwnd, STC_FRAMES_PER_SEC ), FALSE );
            ListBox_Enable( hSubCtrl[ LST_SCR_TO_ANIM ], FALSE );
            ListBox_Enable( GetDlgItem( hwnd, LST_PLAY_OPTIONS ), FALSE );                
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_ScrptToAnimCtrl


/************************************************************************
*    FUNCTION NAME:         OnViewWnd                                                             
*
*    DESCRIPTION:           Message handler for the view window    
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

LRESULT CMNGanimWnd::OnViewWnd( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT rect;
    LRESULT Result = FALSE;
    int X, Y, CurPosX, CurPosY, FrameCount, i, SelctIndex;

    // Handle the paint message
    if( Message == WM_PAINT )
    {
        // Get the rect of the about window
        GetClientRect( hSubCtrl[ Ctrl ], &rect );

        // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
        // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
        // message handler. You CAN NOT paint any other way. Even if you have
        // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
        // and EndPaint API calls or bad things will happen,
        PaintDC = BeginPaint( hSubCtrl[ Ctrl ], &paintStruct );

        // Blit the buffer to the view window
        ViewBuffer.Draw( PaintDC );

        // Must be called
        EndPaint( hSubCtrl[ Ctrl ], &paintStruct );
    }
    else if( Message == WM_MOUSEMOVE )
    {
        if( MouseButtonDown )
        {
            // Get the X, Y offsets
            X = LOWORD( lParam );
            Y = HIWORD( lParam );

            // Get the frame count
            FrameCount = ListBox_GetCount( hSubCtrl[ LST_FRAMES ] );

            // Go through the list and get all the selections
            for( i = 0; i < FrameCount; ++i )
            {
                // Has this frame been selected?
                if( ListBox_GetSel( hSubCtrl[ LST_FRAMES ], i ) > 0 )
                {                    
                    // Get the current position
                    CurPosX = StoryLstAry.GetStoryList( activeScript )->GetXOffset( i );
                    CurPosY = StoryLstAry.GetStoryList( activeScript )->GetYOffset( i );

                    // Record the movement
                    if( MouseMoveX < X )
                        StoryLstAry.GetStoryList( activeScript )->SetXOffset( i, CurPosX + (X - MouseMoveX) );
                    else
                        StoryLstAry.GetStoryList( activeScript )->SetXOffset( i, CurPosX - (MouseMoveX - X) );

                    if( MouseMoveY < Y )
                        StoryLstAry.GetStoryList( activeScript )->SetYOffset( i, CurPosY + (Y - MouseMoveY) );
                    else
                        StoryLstAry.GetStoryList( activeScript )->SetYOffset( i, CurPosY - (MouseMoveY - Y) );

                    // Get the offsets of the selected image
                    CurPosX = StoryLstAry.GetStoryList( activeScript )->GetXOffset( i );
                    CurPosY = StoryLstAry.GetStoryList( activeScript )->GetYOffset( i );
                }
            }

            // Add in the base offsets
            CurPosX += StoryLstAry.GetBaseXOffset( activeScript );
            CurPosY += StoryLstAry.GetBaseYOffset( activeScript );

            // Init the rect
            // All the frames in an MNG are the same size
            SetRect( &rect, CurPosX, CurPosY, CurPosX + StoryLstAry.GetPtrToMng( activeScript )->Width(), 
                                              CurPosY + StoryLstAry.GetPtrToMng( activeScript )->Height() );

            // Display the moved image
            SelectAnimFrame( hwnd, &rect );

            // Save the new offsets
            MouseMoveX = X;
            MouseMoveY = Y;
        }
    }
    else if( Message == WM_LBUTTONDOWN || Message == WM_RBUTTONDOWN )
    {
        // Make sure we have some frames in the list box
        if( ListBox_GetCount( hSubCtrl[ LST_FRAMES ] ) )
        {
            // Did we select an animation frame?

            // Make sure we have a frame selection and that we have frames
            SelctIndex = ListBox_GetCurSel( hSubCtrl[ LST_FRAMES ] );

            if( SelctIndex != LB_ERR && StoryLstAry.GetStoryList( activeScript )->Count() )
            {
                // Get the offsets of the selected image
                X = StoryLstAry.GetStoryList( activeScript )->GetXOffset( SelctIndex ) + StoryLstAry.GetBaseXOffset( activeScript );
                Y = StoryLstAry.GetStoryList( activeScript )->GetYOffset( SelctIndex ) + StoryLstAry.GetBaseYOffset( activeScript );

                // Init the rect
                // All the frames in an MNG are the same size
                SetRect( &rect, X, Y, X + StoryLstAry.GetPtrToMng( activeScript )->Width(), 
                                      Y + StoryLstAry.GetPtrToMng( activeScript )->Height() );

                // Record the curser click offsets
                MouseMoveX = LOWORD( lParam );
                MouseMoveY = HIWORD( lParam );

                // Is the point inside the selected frame?
                if( PointInsideRect( MouseMoveX, MouseMoveY, &rect ) )
                {
                    // Init button down flag
                    MouseButtonDown = TRUE;

                    // Display the selection
                    SelectAnimFrame( hwnd, &rect );

                    // Return from here to keep the any other selections from being evaluated
                    return Result;
                }
            }

            // Even if a selection was missed, reset the window
            else
            {
                // Rebuild the view buffer
                UpdateViewBuffer( hwnd );

                // Invalidate the whole window so it all gets painted
                InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
            }
        }
    }
    else if( Message == WM_LBUTTONUP || Message == WM_RBUTTONUP )
    {
         MouseButtonDown = FALSE;
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnViewWnd


/************************************************************************
*    FUNCTION NAME:         SelectAnimFrame                                                             
*
*    DESCRIPTION:           Shows the selected image file
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

void CMNGanimWnd::SelectAnimFrame( HWND hwnd, LPRECT rect )
{
    char StrOffset[20];

    // Rebuild the view buffer
    UpdateViewBuffer( hwnd );

    // draw a line around the rect of the selected bitmap
    FrameRect( ViewBuffer.Handle(), rect, (HBRUSH)GetStockObject( WHITE_BRUSH ) );

    // Invalidate the whole window so it all gets painted
    InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );

    // Get the X offset value and set the edit control
    wsprintf( StrOffset, "%d", rect->left - StoryLstAry.GetBaseXOffset( activeScript ) );
    Edit_SetText( hSubCtrl[ EDT_FRAME_X ], StrOffset );

    // Get the Y offset value and set the edit control
    wsprintf( StrOffset, "%d", rect->top - StoryLstAry.GetBaseYOffset( activeScript ) );
    Edit_SetText( hSubCtrl[ EDT_FRAME_Y ], StrOffset );

}   // SelectAnimFrame


/************************************************************************
*    FUNCTION NAME:         OnList_ScriptFilesCtrl                                                             
*
*    DESCRIPTION:           Message handler for the script files list control    
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

LRESULT CMNGanimWnd::OnList_ScriptFilesCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int i, j, frameCount, offset=0;
    char fileName[MAX_PATH * 2];

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a valid selection
        if( ListBox_GetCurSel( hSubCtrl[ Ctrl ] ) == LB_ERR )
            return Result;

        // Ask if they want to delete the selected animation
        if( activeScript > -1 && scriptPathList.Count() &&
            IDYES == AskQuestion( MB_YESNO, "Question?", "Do you want to delete the selected script file(s)? This will delete the file from your hard drive." ) )
        {
            // get the number of scripts
            frameCount = scriptPathList.Count();

            // Go through the script list and delete
            for( i = 0; i < frameCount; ++i )
            {               
                // Has this frame been selected?
                if( ListBox_GetSel( hSubCtrl[ Ctrl ], i ) > 0 )
                {
                    // Delete the file. If a file does not exist, it just returns false
                    DeleteFile( scriptPathList.GetString( i + offset ) );

                    // Find the loaded script and delete that
                    for( j = 0; j < StoryLstAry.Count(); ++j )
                    {
                        // If we found the script, save the index and break out
                        if( strcmp( StoryLstAry.GetPtrToScriptName( j ), scriptPathList.GetString( i + offset ) ) == 0 )
                        {
                            // Record the new offset value
                            StoryLstAry.Delete( j );
                            break;  
                        }
                    }

                    // delete the path to the script
                    scriptPathList.Delete( i + offset );

                    --offset;
                }
            }

            // Clear out the frame list box
            ListBox_ResetContent( hSubCtrl[ Ctrl ] );
            ListBox_ResetContent( GetDlgItem( hwnd, LST_AVAILABLE_MNG ) );
            ListBox_ResetContent( hSubCtrl[ LST_FRAMES ] );

            // Display all the strings
            for( i = 0; i < scriptPathList.Count(); ++i )
            {
                CropFileNameFromPath( scriptPathList.GetString( i ), fileName );

                ListBox_AddString( hSubCtrl[ Ctrl ], fileName );
            }

            // If we have some scripts left, select the first one
            if( StoryLstAry.Count() )
            {
                // Select the list box item
                ListBox_SetSel( hSubCtrl[ Ctrl ], TRUE, 0 );

                // Display the first item in the list
                ListClk_ScriptLst( hParentWnd, hSubCtrl[ Ctrl ], CBN_SELCHANGE );
            }
            else  // If we don't have any frames, just do an update
            {
                // Clear out all the edit controls
                Edit_SetText( hSubCtrl[ EDT_FRAME_X ], "" );
                Edit_SetText( hSubCtrl[ EDT_FRAME_Y ], "" );
                Edit_SetText( hSubCtrl[ EDT_DIS_COUNT ], "" );
                Edit_SetText( hSubCtrl[ EDT_Z_ORD ], "" );

                // There are no active scripts so set the active index to reflect that
                activeScript = -1;

                // update the view buffer
                UpdateViewBuffer( hwnd );

                // Invalidate the whole window so it all gets painted
                InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
            }
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_ScriptFilesCtrl


/************************************************************************
*    FUNCTION NAME:         OnList_AnimFramesCtrl                                                             
*
*    DESCRIPTION:           Message handler for the animation frames list control    
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

LRESULT CMNGanimWnd::OnList_AnimFramesCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int AnimIndex, i, MoveIndex, frame, frameCount, offset=0, count;
    char tmpStr[100];

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a selection to the animation
        AnimIndex = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
        if( AnimIndex == LB_ERR )
            return Result;

        // Ask if they want to delete the selected animation
        if( activeScript > -1 && StoryLstAry.GetStoryList( activeScript )->Count() &&
            IDYES == AskQuestion( MB_YESNO, "Question?", "Do you want to delete the selected frame(s)?" ) )
        {
            // get the number of frames
            frameCount = StoryLstAry.GetStoryList( activeScript )->Count();

            // Go through the frame list and termination the selected ones
            for( i = 0; i < frameCount; ++i )
            {
                // Has this frame been selected?
                if( ListBox_GetSel( hSubCtrl[ Ctrl ], i ) > 0 )
                {
                    // delete this object's frames
                    StoryLstAry.GetStoryList( activeScript )->Delete( i + offset );

                    --offset;
                }
            }

            // Clear out the frame list box
            ListBox_ResetContent( hSubCtrl[ LST_FRAMES ] );

            // Add animation name to create animation list box
            for( i = 0; i < StoryLstAry.GetStoryList( activeScript )->Count(); ++i )
            {
                // Get the frame
                frame = StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( i );
                
                // Do we have any sound properties attached to this frame
                if( (count = StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Count()) > MAX_ASTERICK-1 )
                    count = MAX_ASTERICK-1;
                                        
                // Show the number of sounds attached to this frame
                wsprintf( tmpStr, "MNG Frame Index %u   %s", frame, Asterick[count] );

                // Add the string
                ListBox_AddString( hSubCtrl[ LST_FRAMES ], tmpStr );
            }

            // If we have some frames, select the first one
            if( StoryLstAry.GetStoryList( activeScript )->Count() )
            {
                // Select the list box item
                ListBox_SetSel( hSubCtrl[ LST_FRAMES ], TRUE, 0 );

                // Display the first item in the list
                ListClk_AnimFrameLst( hParentWnd, hSubCtrl[ LST_FRAMES ], CBN_SELCHANGE );
            }
            else  // If we don't have any frames, just do an update
            {
                // Clear out all the edit controls
                Edit_SetText( hSubCtrl[ EDT_FRAME_X ], "" );
                Edit_SetText( hSubCtrl[ EDT_FRAME_Y ], "" );
                Edit_SetText( hSubCtrl[ EDT_DIS_COUNT ], "" );
                Edit_SetText( hSubCtrl[ EDT_Z_ORD ], "" );

                // update the view buffer
                UpdateViewBuffer( hwnd );

                // Invalidate the whole window so it all gets painted
                InvalidateRect( hSubCtrl[ STC_VIEW ], &ViewRect, TRUE );
            }
        }
    }
    else if( Message == WM_RBUTTONUP )
    {
        if( activeScript > -1 && StoryLstAry.GetStoryList( activeScript )->Count() > 1 )
        {
            // Make sure we have a selection to the animation
            AnimIndex = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
            if( AnimIndex == LB_ERR )
                return Result;

            // Do they want to move to another index
            MoveIndex = DisplayMovePageDlg( hInst, hwnd, hSubCtrl[ Ctrl ], AnimIndex, 
                                            StoryLstAry.GetStoryList( activeScript )->Count() );

            if( MoveIndex > -1 )
            {
                // Move the selected item to the new index
                StoryLstAry.GetStoryList( activeScript )->MoveToNewPos( AnimIndex, MoveIndex );

                // Clear out the frame list box
                ListBox_ResetContent( hSubCtrl[ Ctrl ] );

                // Add animation name to create animation list box
                for( i = 0; i < StoryLstAry.GetStoryList( activeScript )->Count(); ++i )
                {
                    // Get the frame
                    frame = StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( i );
                    
                    // Do we have any sound properties attached to this frame
                    if( (count = StoryLstAry.GetStoryList( activeScript )->GetSndLst( i )->Count()) > MAX_ASTERICK-1 )
                        count = MAX_ASTERICK-1;
                                            
                    // Show the number of sounds attached to this frame
                    wsprintf( tmpStr, "MNG Frame Index %u   %s", frame, Asterick[count] );

                    // Add the string
                    ListBox_AddString( hSubCtrl[ Ctrl ], tmpStr );
                }

                // Select the list box item
                ListBox_SetCurSel( hSubCtrl[ Ctrl ], MoveIndex );
            }
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_AnimFramesCtrl


/************************************************************************
*    FUNCTION NAME:         BlitImageToBuffer                                                             
*
*    DESCRIPTION:           Blits an image to a buffer. This is written this
*                           way so that the function can be called by other
*                           classes for blitting.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void CMNGanimWnd::BlitImageToBuffer( PTDIB pDib, PTBitmap pBuffer, LPRECT pRect, HWND hStc_Ctrl, int count )
{
    TBitmap mask, image;
    RECT GraphicRect;
    char StrTemp[100];
    
    // Make sure it's not empty
    if( !pDib->IsEmpty() )
    {
        // Color the buffer
        pBuffer->ColorRect( &PreviewRect, (COLORREF)GetSysColor( COLOR_BTNFACE ) );

        // Do we center the image or size it to fit?
        if( pDib->width() <= pRect->right && pDib->height() <= pRect->bottom  )
            CenterRectInRect( &GraphicRect, pDib->width(), pDib->height(), pRect->right, pRect->bottom );

        else
            SizeToFitRect( &GraphicRect, pDib->width(), pDib->height(), pRect->right, pRect->bottom );
           
        // Is this image a transparent image?
        if( pDib->IsBitmapTransparent() )
        {           
            // copy the bitmap to the temp bitmap
            image.CopyDIB( pDib );

            // Copy the sprite over to the mask
            mask = image;
            
            // Convert to a mask
            mask.ConvertToMask();

            // Blit the mask            
            mask.BlitToDC( pBuffer->Handle(), &GraphicRect );

            // Convert the sprite to a transparent sprite
            image.ConvertToSprite();

            // This ensures the niceest looking stretch
            SetStretchBltMode( pBuffer->Handle(), HALFTONE );
                                                                
            // Blit the sprite
            image.BlitToDC( pBuffer->Handle(), &GraphicRect );
            
            // Resets every thing back after a stretch
            SetBrushOrgEx( pBuffer->Handle(), 0, 0, NULL );  
        }
        else
        {
            // This ensures the niceest looking stretch
            SetStretchBltMode( pBuffer->Handle(), HALFTONE );

            pDib->BlitToDC( pBuffer->Handle(), &GraphicRect );

            // Resets every thing back after a stretch
            SetBrushOrgEx( pBuffer->Handle(), 0, 0, NULL );
        }

        // Display related bitmap info
        if( hStc_Ctrl )
        {
            wsprintf( StrTemp, "Frame Info:  %u x %u,  %u frames", pDib->width(), pDib->height(), count );

            Static_SetText( hStc_Ctrl, StrTemp );
        }
    }

}   // BlitImageToBuffer


/************************************************************************
*    FUNCTION NAME:         BlitSprite                                                             
*
*    DESCRIPTION:           Blit the sprite to the view buffer
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

void CMNGanimWnd::BlitSprite( PTDIB pDib, PTStoryBoard pStoryBoard, int baseX, int baseY, BOOL PaintOnly )
{
    TBitmap mask, image;
    
    // Make sure it's not empty
    if( !pDib->IsEmpty() )
    {           
        // Is this image a transparent image?
        if( pDib->IsBitmapTransparent() || PaintOnly )
        {           
            // copy the bitmap to the temp bitmap
            image.CopyDIB( pDib );            
            
            if( !PaintOnly )
            {
                // Copy the sprite over to the mask
                mask = image;
                
                // Convert to a mask
                mask.ConvertToMask();

                // Blit the mask            
                mask.BlitToDC( ViewBuffer.Handle(), pStoryBoard->X + baseX, pStoryBoard->Y + baseY );

                // Convert the sprite to a transparent sprite
                image.ConvertToSprite();
            }
            else
            {
                // Convert the sprite to a transparent sprite
                image.ConvertToSprite();

                // We need a different paint mode for no backgrounds
                if( !BackgroundLoaded )
                    image.SetCopyMode( SRCINVERT );                            
            }
                                                                
            // Blit the sprite
            image.BlitToDC( ViewBuffer.Handle(), pStoryBoard->X + baseX, pStoryBoard->Y + baseY );
                          
        }
        else
        {
            pDib->BlitToDC( ViewBuffer.Handle(), pStoryBoard->X + baseX, pStoryBoard->Y + baseY );
        }
    }

}   // BlitSprite


/************************************************************************
*    FUNCTION NAME:         UpdateViewBuffer                                                             
*
*    DESCRIPTION:           Update the view buffer. This is what is displayed
*                           to the view window.
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

void CMNGanimWnd::UpdateViewBuffer( HWND hwnd )
{
    int index;
    PTStoryBoard pStoryBoard;
    PCMNG pMNG;

    // Update the view buffer with the background buffer if the buffer is being used
    // Usually the back buffer is just white unless an image has been loaded
    BackBuffer.Draw( ViewBuffer.Handle() );

    // Make sure we have a selection
    index = ListBox_GetCurSel( hSubCtrl[ LST_FRAMES ] );
    if( index == LB_ERR )
        return;

    // Make sure we have some scripts
    if( activeScript > -1 && StoryLstAry.GetStoryList( activeScript )->Count() )
    {
        // Get the storyboard
        pStoryBoard = StoryLstAry.GetStoryList( activeScript )->GetStoryBrd( index );

        // Get the MNG class
        pMNG = StoryLstAry.GetPtrToMng( activeScript );

        // Make sure we are using an index that is there
        if( pStoryBoard->FrameIndex >= 0 && pStoryBoard->FrameIndex < pMNG->Count() )
        {
            // Do we want to gost the previous frame?
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_GOST_PREV_FRAME ) ) == FALSE )
            {
                // Blit the sprite to the screen
                BlitSprite( pMNG->GetDIB( pStoryBoard->FrameIndex ), pStoryBoard, 
                            StoryLstAry.GetBaseXOffset( activeScript ), StoryLstAry.GetBaseYOffset( activeScript ), FALSE );
            }
            else
            {
                if( index == 0 )
                    // Blit the sprite to the screen
                    BlitSprite( pMNG->GetDIB( pStoryBoard->FrameIndex ), pStoryBoard, 
                                StoryLstAry.GetBaseXOffset( activeScript ), StoryLstAry.GetBaseYOffset( activeScript ), FALSE );
                else
                {
                    // Blit the previous frame as a gost
                    BlitSprite( pMNG->GetDIB( StoryLstAry.GetStoryList( activeScript )->GetFrameIndex( index - 1 ) ), 
                                StoryLstAry.GetStoryList( activeScript )->GetStoryBrd( index - 1 ), 
                                StoryLstAry.GetBaseXOffset( activeScript ), StoryLstAry.GetBaseYOffset( activeScript ), TRUE );

                    // Blit the current frame
                    BlitSprite( pMNG->GetDIB( pStoryBoard->FrameIndex ), pStoryBoard, 
                                StoryLstAry.GetBaseXOffset( activeScript ), StoryLstAry.GetBaseYOffset( activeScript ), FALSE );
                }
            }
        }
    }

}   // UpdateViewBuffer


/************************************************************************
*    FUNCTION NAME:         PlayAnimations                                                             
*
*    DESCRIPTION:           Play the selected animations. This function
*                           gets called from the windows timer.    
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

void CMNGanimWnd::PlayAnimations( HWND hwnd )
{
    int i, FrameCounter, DisplayCounter, zOrder;
    PTStoryBoard pStoryBoard;
    TStoryBoard StoryBoard;
    HDC DC;
    PTDIB pDib;
    int baseX, baseY;
    int sty, frm, snd;
    int stillAnimating=0;

    // Don't let it animate if we don't have a timer code
    if( TimerReturnCode == 0 )
        return;

    // Update the view buffer with the background buffer
    BackBuffer.Draw( ViewBuffer.Handle() );

    // Only bother sorting if we have more then one animation selected
    if( zOrderList.Count() > 1 )
    {
        // Set the Z Order Value
        for( i = 0; i < zOrderList.Count(); ++i )
        {
            // Get the story list index
            sty = zOrderList.GetIndex( i );

            // get the current frame for this animation
            FrameCounter = StoryLstAry.GetFrameCounter( sty );

            // Save off the Z order depending of the frame
            zOrderList.SetZorder( i, StoryLstAry.GetStoryList( sty )->GetZOrder( FrameCounter ) + StoryLstAry.GetBaseZOffset( sty ) );
        }

        // Sort it by the Z Order
        zOrderList.SortByZorder();
    }

    for( i = 0; i < zOrderList.Count(); ++i )
    {        
        // Get the story list index
        sty = zOrderList.GetIndex( i );
                
        // Get the counter values
        FrameCounter = StoryLstAry.GetFrameCounter( sty );
        DisplayCounter = StoryLstAry.GetDisplayTimer( sty );

        // Get the pointer to the storyboard
        pStoryBoard = StoryLstAry.GetStoryList( sty )->GetStoryBrd( FrameCounter );

        // Get the base offsets
        baseX = StoryLstAry.GetBaseXOffset( sty );
        baseY = StoryLstAry.GetBaseYOffset( sty );

        // Get the pointer to the BMP
        pDib = StoryLstAry.GetPtrToMng( zOrderList.GetIndex( i ) )->GetDIB( pStoryBoard->FrameIndex );

        // Make sure we want to here the sounds
        if( Settings.GetPlaySounds() )
        {
            // Play the loaded sound        
            for( snd = 0; snd < StoryLstAry.GetStoryList( sty )->GetSndLst( pStoryBoard->FrameIndex )->Count(); ++snd )
            {
                // Init the MCI window
                StoryLstAry.GetStoryList( sty )->GetSndLst( pStoryBoard->FrameIndex )->GetMP( snd )->Play();
            }
        }

        // Make sure we are done blitting this frame
        if( (DisplayCounter % (pStoryBoard->FrameRepeat+1)) == 0 )
        {
            // Reset the display counter
            DisplayCounter = 1;

            // Handle the frame index depending on how they want to animat it
            switch( StoryLstAry.GetAnimType( sty ) )
            {
                case PLAY_FORWARD_STAY:
                    // Blit the sprite to the buffer
                    BlitSprite( pDib, pStoryBoard, baseX, baseY );

                    if( FrameCounter < StoryLstAry.GetStoryList( sty )->Count()-1 )
                    {
                        FrameCounter = ( FrameCounter + 1 ) % StoryLstAry.GetStoryList( sty )->Count();
                        ++stillAnimating;
                    }
                break;

                case PLAY_FORWARD_HIDE:
                    if( FrameCounter < StoryLstAry.GetStoryList( sty )->Count() )
                    {
                        if( !StoryLstAry.GetKillAnim( sty ) )
                        {
                            // Blit the sprite to the buffer
                            BlitSprite( pDib, pStoryBoard, baseX, baseY );

                            ++stillAnimating;
                        }

                        if( FrameCounter < StoryLstAry.GetStoryList( sty )->Count()-1 )
                            FrameCounter = ( FrameCounter + 1 ) % StoryLstAry.GetStoryList( sty )->Count();
                        else
                            StoryLstAry.SetKillAnim( sty, TRUE );
                    }
                break;

                case PLAY_FORWARD_LOOP:
                    // Blit the sprite to the buffer
                    BlitSprite( pDib, pStoryBoard, baseX, baseY );

                    // Inc the frame counter
                    FrameCounter = ( FrameCounter + 1 ) % StoryLstAry.GetStoryList( sty )->Count();
                    ++stillAnimating;
                break;

                case PLAY_REVERSE_STAY:
                    // Blit the sprite to the buffer
                    BlitSprite( pDib, pStoryBoard, baseX, baseY );

                    if( FrameCounter > 0 )
                    {
                        --FrameCounter;

                        if( FrameCounter < 0 )
                            FrameCounter = StoryLstAry.GetStoryList( sty )->Count()-1;

                        ++stillAnimating;
                    }
                break;

                case PLAY_REVERSE_HIDE:

                    if( FrameCounter >= 0 )
                    {
                        if( !StoryLstAry.GetKillAnim( sty ) )
                        {
                            // Blit the sprite to the buffer
                            BlitSprite( pDib, pStoryBoard, baseX, baseY );

                            ++stillAnimating;
                        }

                        if( FrameCounter > 0 )
                        {
                            --FrameCounter;
                            if( FrameCounter < 0 )
                                FrameCounter = StoryLstAry.GetStoryList( sty )->Count()-1;
                        }
                        else
                            StoryLstAry.SetKillAnim( sty, TRUE );
                    }
                break;

                case PLAY_REVERSE_LOOP:
                    // Blit the sprite to the buffer
                    BlitSprite( pDib, pStoryBoard, baseX, baseY );

                    --FrameCounter;

                    if( FrameCounter < 0 )
                        FrameCounter = StoryLstAry.GetStoryList( sty )->Count()-1;

                    ++stillAnimating;
                break;

                case PLAY_PING_PONG:
                    // Blit the sprite to the buffer
                    BlitSprite( pDib, pStoryBoard, baseX, baseY );

                    // Inc the frame
                    FrameCounter += StoryLstAry.GetPingPongDir( sty );

                    if( FrameCounter == StoryLstAry.GetStoryList( sty )->Count()-1 )
                        StoryLstAry.SetPingPongDir( sty, -1 );
                    else if( FrameCounter == 0 )
                        StoryLstAry.SetPingPongDir( sty, 1 ); 

                    ++stillAnimating;
                break;
            }
        }
        else
        {
            ++DisplayCounter;
            ++stillAnimating;
        }

        // Save the counter values
        StoryLstAry.SetFrameCounter( sty, FrameCounter );
        StoryLstAry.SetDisplayTimer( sty, DisplayCounter );        
    }

    DC = GetDC( hwnd );
    ViewBuffer.Draw( DC );
    ReleaseDC( hwnd, DC );

    // Kill the timer if nothing is animating
    if( stillAnimating == 0 )
    {
        // See if we are plying sounds
        if( Settings.GetPlaySounds() )
        {
            // Go through everything and unload all the MCI windows
            for( i = 0; i < zOrderList.Count(); ++i )
            {
                // Get the story list index
                sty = zOrderList.GetIndex( i );

                for( frm = 0; frm < StoryLstAry.GetStoryList( sty )->Count(); ++frm )
                {
                    for( snd = 0; snd < StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->Count(); ++snd )
                    {
                        // Delete the MCI window
                        StoryLstAry.GetStoryList( sty )->GetSndLst( frm )->GetMP( snd )->Free();
                    }
                }
            }
        }

        // Kill the timer
        KillTimer( hSubCtrl[ STC_VIEW ], ANIM_TIMER_HANDLE );

        // Reset the button label
        Button_SetText( GetDlgItem( hParentWnd, BTN_PLAY_SCRIPTS ), "Play" );
        TimerReturnCode = 0;
    }

}   // PlayAnimations