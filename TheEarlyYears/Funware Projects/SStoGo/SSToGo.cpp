
/************************************************************************
*
*    PRODUCT:         Screen Savers To Go
*
*    FILE NAME:       SSToGo.cpp
*
*    DESCRIPTION:     Screen Savers To Go main code file 
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
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include "WinMacros.h"       // My windows macros that should been included with windowsx.h

#include "SSToGo.h"          // Header file for this *.cpp file.
#include "TStartUp.h"        // Header file for the startup screen window
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer
#include "AboutDlg.h"        // About dialog box
#include "PrintCnl.h"        // Print Cancel dialog box
#include "Prefs.h"           // Preferences dialog box for tool
#include "status.h"          // Window that shows the progress bar
#include "MovePage.h"        // Dialog for moving pages around
#include "MediaDir.h"        // Metia file selector
#include "PictLoad.h"        // Dialog for loading pictures that shows a preview window
#include "BldBuf.h"          // Functions for updating and refreshing the buffer
#include "YourInstDlg.h"     // Shows a dialog box that informs them on what file is theres to distribute

// Globals
PTSS2GO pSS2GO;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         WndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler.
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_DESTROY,       pSS2GO->OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND,       pSS2GO->OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,        pSS2GO->OnCreate );
        HANDLE_MSG( hwnd, WM_CLOSE,         pSS2GO->OnClose );
        HANDLE_MSG( hwnd, WM_SHOWWINDOW,    pSS2GO->OnShowWindow );
        HANDLE_MSG( hwnd, WM_MOVE,          pSS2GO->OnMove );
        HANDLE_MSG( hwnd, WM_NOTIFY,        pSS2GO->OnNotify );
        HANDLE_MSG( hwnd, WM_GETMINMAXINFO, pSS2GO->OnGetMinMaxInfo );
        HANDLE_MSG( hwnd, WM_NCDESTROY,     pSS2GO->OnNCDestroy );
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

} // LRESULT CALLBACK WndProc



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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                            LPSTR lpszCmdParam, int nCmdShow )
{
    MSG Msg;
    HWND hwnd = NULL;
    HACCEL hAccel;

    // Create the parent window object
    pSS2GO = new TSS2GO();

    // Create the window
    if( pSS2GO )
        hwnd = pSS2GO->CreateParentWnd( hInst, nCmdShow );

    // Kick out of the program if there has been an error
    if( !hwnd )
        return FALSE;

    // Load the accelerators
    hAccel = LoadAccelerators( hInst, "Art4SaleAccl" );
    if( !hAccel )
    {
        PostMsg( "Error", "Error loading accelerator table." );
        return FALSE;
    }

    // Standard windows message loop
    while( GetMessage( &Msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( hwnd, hAccel, &Msg) )
        {
            TranslateMessage( &Msg );
            DispatchMessage( &Msg );
        }
    }

    return Msg.wParam;

}   // WinMain


/************************************************************************
*    FUNCTION NAME:         EdtPageTo_Delay_Proc                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep the return key from beeping
*                           and to have it load the page they entered.
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

LRESULT CALLBACK EdtPageTo_Delay_Proc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pSS2GO->HandlePage_DelayProc( hwnd, Message, wParam, lParam );

}   // EdtPageTo_Delay_Proc


/************************************************************************
*    FUNCTION NAME:         EdtTopBotCaption_Proc                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep the return key from beeping
*                           and to have it load the page they entered.
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

LRESULT CALLBACK EdtTopBotCaption_Proc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pSS2GO->HandleTopBotCaption_Proc( hwnd, Message, wParam, lParam );

}   // EdtTopBotCaption_Proc


/************************************************************************
*    FUNCTION NAME:         PaintFrameWndProc()                                                             
*
*    DESCRIPTION:           Standard Windows message handler that makes use
*                           of the WindowsX macros.
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK ViewWndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_PAINT,      pSS2GO->ViewWnd_OnPaint );
        HANDLE_MSG( hwnd, WM_CREATE,     pSS2GO->ViewWnd_OnCreate );
        HANDLE_MSG( hwnd, WM_ERASEBKGND, pSS2GO->ViewWnd_OnEraseBkgnd );
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }
}   // ViewWndProc


/************************************************************************
*
*                      Dialog Object Functions
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

TSS2GO::TSS2GO()
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

TSS2GO::~TSS2GO()
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

HWND TSS2GO::CreateParentWnd( HINSTANCE hInst, int nCmdShow )
{
    HWND hwnd = NULL;

    // First this to do is memset the AppData
    memset( &AppData, 0, sizeof( TApplicationData ) );
    // Clear this out for the "DeleteTmpSaver"
    memset( &ProjPrefsData, 0, sizeof(TProjectPrefs) );

    // Init Windows 95 common controls
    InitCommonControls();

    // Get the command line and parse it to be sure
    // there is no junk with it
    ParsePathFromCmdLine( GetCommandLine(), AppData.Path );    

    // Get the settings in the ini file
    Settings.GetPrivateString();

    // Init the registred mode
    AppData.RegistrationMode = FULL_REG_VERSION;

    // Register the window
    if( !RegisterWnd( hInst ) )
        goto HANDLE_ERROR;

    // Create the window
    hwnd = CreateWnd( hInst, SW_SHOWNORMAL );
    if( !hwnd )
    	goto HANDLE_ERROR;

HANDLE_ERROR:;

    return hwnd;

}   // CreateParentWnd


/************************************************************************
*    FUNCTION NAME:         HandlePage_DelayProc                                                             
*
*    DESCRIPTION:           Handle the Jump to page and delay settings
*                           subclassed controls procedure 
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
*    h rosenorn    8/29/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT TSS2GO::HandlePage_DelayProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    int i, Delay;
    char NumberStr[10];

    // Process the Load key entry page
    if( AppData.hEditContrls[ EDT_CUR_PAGE ] == hwnd  )
    {
        // Trapping all these message keep the control from beeping when hitting the return key
        if( (Message == WM_KEYDOWN || Message == WM_KEYUP || Message == WM_CHAR) && wParam == RETURN_KEY )
        {
            // Load the keyed in page            
            LoadKeyEntryPage();

            return FALSE;
        }
    }

    // Save the Dealy In Seconds
    if( AppData.hEditContrls[ EDT_DELAY_IN_SEC ] == hwnd )
    {
        // save the delay in seconds
        if( Message == WM_KEYUP )
        {
            if( Edit_GetTextLength( AppData.hEditContrls[ EDT_DELAY_IN_SEC ] ) )
            {
                Edit_GetText( AppData.hEditContrls[ EDT_DELAY_IN_SEC ], NumberStr, sizeof(char)*10 );
                Delay = atoi( NumberStr );
                PageList.SetDelayInSec( PageList.GetCurrentPage(), Delay );
                Settings.SetDelayInSec( Delay );
            }
        }
    }

    // See who set the message and send it off to the default procedure
    for( i = 0; i < MAX_EDT_CONTRL; ++i )
        if( AppData.hEditContrls[i] == hwnd )
            return CallWindowProc( AppData.OldEditProc[i], hwnd, Message, wParam, lParam );

    return FALSE;

}   // HandlePage_DelayProc


/************************************************************************
*    FUNCTION NAME:         HandlePage_DelayProc                                                             
*
*    DESCRIPTION:           Handle the Jump to page and delay settings
*                           subclassed controls procedure 
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
*    h rosenorn    8/29/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT TSS2GO::HandleTopBotCaption_Proc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    int i;

    // Trap the keyboard messages
    if( Message == WM_KEYUP || wParam == RETURN_KEY )
    {
        char StrTmp[ MAX_PAGE_CAPTION+1 ];

        // Set the flag that indicates the project is live
        ActProj.LiveProject = TRUE;

        // Figure out which control is sending the message
        if( AppData.hEditMsg[ EDT_TEXT_TOP_MSG ] == hwnd )
        {
            // Get the text
            Edit_GetText( AppData.hEditMsg[ EDT_TEXT_TOP_MSG ], StrTmp, sizeof(char)*(MAX_PAGE_CAPTION+1) );

            // Copy the text to the page
            strcpy( PageList.GetPtrToTopCaption( PageList.GetCurrentPage() ), StrTmp );
        }
        else if( AppData.hEditMsg[ EDT_TEXT_BOTTOM_MSG ] == hwnd )
        {
            // Get the text
            Edit_GetText( AppData.hEditMsg[ EDT_TEXT_BOTTOM_MSG ], StrTmp, sizeof(char)*(MAX_PAGE_CAPTION+1) );

            // Copy the text to the page
            strcpy( PageList.GetPtrToBotCaption( PageList.GetCurrentPage() ), StrTmp );
        }

        // Show the text being displayed 
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();
    }

    // See who set the message and send it off to the default procedure
    for( i = 0; i < MAX_BTN_GROUPS; ++i )
        if( AppData.hEditMsg[i] == hwnd )
            return CallWindowProc( AppData.OldEditTxtProc[i], hwnd, Message, wParam, lParam );

    return FALSE;

}   // HandleTopBotCaption_Proc


/************************************************************************
*    FUNCTION NAME:         SaveSecretINIsettings                                                             
*
*    DESCRIPTION:           Save the secret ini file settings 
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
*    h rosenorn    8/29/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::SaveSecretINIsettings()
{
    // They registered so save the registration number to the secret ini file
    Settings.WriteSecretIniFile( INI_SECRET_REG_FILE, PRODUCT_NAME );

}   // SaveSecretINIsettings



/************************************************************************
*    FUNCTION NAME:         RegisterWnd                                                             
*
*    DESCRIPTION:           All windows created with CreateWindow
*                           must be registered first.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*    Output:   BOOL            - Returns TRUE or FALSE to indicate if
*                                the task was secessful.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TSS2GO::RegisterWnd( HINSTANCE hInst )
{
    WNDCLASSEX w;

    // Zero out the structure
    memset( &w, 0, sizeof(WNDCLASS) );

	w.cbSize = sizeof( WNDCLASSEX );
	w.hIconSm = NULL;

    // Register the View window
    w.hInstance     = hInst;
    w.lpfnWndProc   = ViewWndProc;
    w.hCursor       = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = (HBRUSH)(COLOR_BTNHIGHLIGHT+1);
    w.lpszClassName = "ViewWnd";

    if( !RegisterClassEx( &w ) )
        return FALSE;

    // Register the parent window
    w.style         = CS_HREDRAW|CS_VREDRAW;
    w.lpfnWndProc   = WndProc;
    w.hIcon         = LoadIcon( hInst, "AppIcon" );
    w.hCursor       = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    w.lpszMenuName  = "Menu_1";
    w.lpszClassName = WINDOW_NAME;

    return ( RegisterClassEx(&w) != 0 );

}   // RegisterWnd



/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           Create the window
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              int aSaverMode  - The screen saver mode which would be
*                                preview or full screen
*              HWND aPreview   - A handle to the preview window which you
*                                inturn create a child window of the same
*                                size. Full screen doesn't require a handle.
*              int nCmdShow    - Specifies how the window is to be shown.
*                                Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*
*    Output:   HWND - A handle to the newly created window
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

HWND TSS2GO::CreateWnd( HINSTANCE hInst, int nCmdShow )
{
    AppData.hInst = hInst;

    AppData.hParentWnd = CreateWindow( WINDOW_NAME, PRODUCT_NAME, WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL );

    // Do these good things if the handle is not NULL
    if( AppData.hParentWnd != NULL )
    {
        ShowWindow( AppData.hParentWnd, nCmdShow );
        UpdateWindow( AppData.hParentWnd );

        // Post a message to show the startup screen
        // This way it doesn't show up before the main window
        PostMessage( AppData.hParentWnd, ((UINT)WM_COMMAND), ((WPARAM)MAKELONG( POST_MSG_AFTER_WM_SHOW, 0 )), ((LPARAM)0) );
    }

    return AppData.hParentWnd;

}   // CreateWnd


/************************************************************************
*    FUNCTION NAME:         OnGetMinMaxInfo                                                             
*
*    DESCRIPTION:           Standard Windows OnGetMinMaxInfo message handler.
*                           The WM_GETMINMAXINFO message is sent to a window 
*                           when the size or position of the window is about 
*                           to change. An application can use this message to 
*                           override the window's default maximized size and 
*                           position, or its default minimum or maximum 
*                           tracking size.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd          - Handle of the window receiving the message
*              LPMINMAXINFO lpMinMaxInfo - Pointer to min max info
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

void TSS2GO::OnGetMinMaxInfo( HWND hwnd, LPMINMAXINFO lpMinMaxInfo )
{
    // Keep the width and height from being changed
    lpMinMaxInfo->ptMaxSize.x = SS2GO_WND_W;
    lpMinMaxInfo->ptMaxSize.y = SS2GO_WND_H;
    lpMinMaxInfo->ptMinTrackSize.x = SS2GO_WND_W;
    lpMinMaxInfo->ptMinTrackSize.y = SS2GO_WND_H;
    lpMinMaxInfo->ptMaxTrackSize.x = SS2GO_WND_W;
    lpMinMaxInfo->ptMaxTrackSize.y = SS2GO_WND_H;
        
}   // OnGetMinMaxInfo


/************************************************************************
*    FUNCTION NAME:         OnNotify                                                             
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

LRESULT TSS2GO::OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr )
{
    char ToolExtraInfo[MAX_PATH];

    if( pnmhdr->code == TTN_NEEDTEXT )
    {
        // init these other members
        LPTOOLTIPTEXT( pnmhdr )->szText[0] = 0;
        LPTOOLTIPTEXT( pnmhdr )->lpszText = 0;
        LPTOOLTIPTEXT( pnmhdr )->hinst = 0;

        switch( idFrom )
        {
            case BTN_FLIP_LEFT:
                strcpy( AppData.ToolTipStr, "Flip Back One Page" );
                if( PageList.Count() )
                {
                    CropFileNameFromPath( PageList.GetMainDataPath( PageList.GetCurrentPage() ), ToolExtraInfo );
                    strcat( AppData.ToolTipStr, " - " );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_FLIP_RIGHT:
                strcpy( AppData.ToolTipStr, "Flip Ahead One Page" );
                if( PageList.Count() )
                {
                    CropFileNameFromPath( PageList.GetMainDataPath( PageList.GetCurrentPage() ), ToolExtraInfo );
                    strcat( AppData.ToolTipStr, " - " );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_SOUND_PLAY:
                strcpy( AppData.ToolTipStr, "Play Sound" );
                if( PageList.Count() && PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                {
                    CropFileNameFromPath( PageList.GetSoundDataPath( PageList.GetCurrentPage() ), ToolExtraInfo );
                    strcat( AppData.ToolTipStr, " - " );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_SOUND_PAUSE:
                strcpy( AppData.ToolTipStr, "Pause Sound Play" );
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_SOUND_STOP:
                strcpy( AppData.ToolTipStr, "Stop Sound Play" );
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;
        }
    }
    return 1;

}   // OnNotify


/************************************************************************
*    FUNCTION NAME:         InitProject                                                             
*
*    DESCRIPTION:           Init application structures 
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

void TSS2GO::InitProject( HWND hwnd )
{
    int i;

    // First thing to do is to delete the temporary screen saver located
    // in the temp folder
    DeleteTmpSaver();

    // Reset these objects
    PageList.Free();
    MainDib.Free();
    BackDib.Free();

    // zero out all the structures
    memset( &ActProj, 0, sizeof(TActiveProjectData) );
    memset( &ProjPrefsData, 0, sizeof(TProjectPrefs) );
    memset( &FileHeader, 0, sizeof(TFileHeader) );
    memset( &AppData.ViewWndRect, 0, sizeof(RECT) );

    // Reset the button bar
    for( i = 0; i < NUM_TBBUTTONS; ++i )
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT + i, DISABLE );

    // Update the controls
    UpdateControls( hwnd, 0, 0 );

    // Init fileheader specific elements
    FileHeader.FormatID = FORMAT_ID;
    strcpy( FileHeader.VerNo, PRO_VER_NUM );

    // Init a few of the project preferences members
    ProjPrefsData.PrefsCheckID = PERFS_CHECK_ID; // Code number to check if data is valid
    strcpy( ProjPrefsData.Title, PRO_EXAMPLE_CAPTION );
    strcpy( ProjPrefsData.ExeName, PRO_EXAMPLE_EXE_NAME );

    // Init the file save flag
    ActProj.OpenedFile = FALSE;

    // Set the project live status
    ActProj.LiveProject = FALSE;

    // Disable menu items
    EnableControls( hwnd, FALSE );
    EnableSpecificControls( hwnd, FALSE );

    // Reset the window caption
    SetWindowCaption( hwnd );

}   // InitProject


/************************************************************************
*    FUNCTION NAME:         OnCreate()                                                             
*
*    DESCRIPTION:           Standard Windows OnCreate message handler. Here
*                           we are creating a windows call back function
*                           timer to reset mouse tick count variable.
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

BOOL TSS2GO::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    BOOL Result = FALSE;

    // Init the media player
    mediaPlayer.Init( hwnd, AppData.hInst, USE_ERROR_DLG_BOXES );

    // Create the buffer for blitting to the little preview window
    if( !buffer.CreateDIBSectionBuffer( GetScreenWidth(), GetScreenHeight(), BIT_COUNT_24 ) )
        goto INIT_ERROR;

    // Center the window
    if( Settings.GetWindowX() < 0 || Settings.GetWindowX() >= GetSystemMetrics( SM_CXSCREEN ) ||
        Settings.GetWindowY() < 0 || Settings.GetWindowY() >= GetSystemMetrics( SM_CYSCREEN ) )
        CenterWindow( hwnd, SS2GO_WND_W, SS2GO_WND_H );
    else
        MoveWindow( hwnd, Settings.GetWindowX(), Settings.GetWindowY(), SS2GO_WND_W, SS2GO_WND_H, TRUE );

    // Create the tool bar
    if( !CreateToolBar( hwnd ) )
        goto INIT_ERROR;

    // Create the view window
    if( !ViewWnd_CreateViewWnd( AppData.hInst, hwnd ) )
        goto INIT_ERROR;

    // Create the controls for the window
    if( !CreateControls( hwnd ) )
        goto INIT_ERROR;

    // Init application structures
    InitProject( hwnd );

    Result = TRUE;

INIT_ERROR:;

    return Result;

}   // OnCreate



/************************************************************************
*    FUNCTION NAME:         CreateToolBar                                                             
*
*    DESCRIPTION:           Create the tool bar 
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

BOOL TSS2GO::CreateToolBar( HWND hwnd )
{
    int i, TbButtonOffset;
    BOOL Result = TRUE;
    RECT ToolBarRect;

    // Init to zero the TBBUTTON array
    memset( AppData.ToolAry, 0, sizeof(TBBUTTON) * NUM_TBBUTTONS );

    // Init the tool bar array for the tool bar
    for( i = 0; i < NUM_TBBUTTONS; ++i )
    {
        AppData.ToolAry[i].iBitmap   = i;
        AppData.ToolAry[i].idCommand = (int)BTN_FLIP_LEFT + i;
        AppData.ToolAry[i].fsState   = TBSTATE_INDETERMINATE;
        if( (int)BTN_FLIP_LEFT + i == BTN_SEP1 )
            AppData.ToolAry[i].fsStyle   = TBSTYLE_SEP;
        else
            AppData.ToolAry[i].fsStyle   = TBSTYLE_BUTTON;
        AppData.ToolAry[i].dwData    = 0L;
        AppData.ToolAry[i].iString   = 0;
    }

    // Create the tool bar
    // The 486 running Windows 95 has trouble with a frame on the tool bar. It does not redraw itself properly. Is this the
    // 486 or Windows 95? It this time I don't know.
    AppData.hToolBar = CreateToolbarEx( hwnd, TBSTYLE_TOOLTIPS|WS_CHILD|WS_VISIBLE|WS_THICKFRAME|WS_CLIPCHILDREN,  // |WS_CLIPSIBLINGS
                                        ID_TOOL_BAR, NUM_TBBUTTONS, AppData.hInst, TOOLBARBMP, AppData.ToolAry,
                                        NUM_TBBUTTONS, TBBUTTON_WIDTH, TBBUTTON_HEIGHT, TBBUTTON_WIDTH, TBBUTTON_HEIGHT, sizeof(TBBUTTON) );

    // Add additional items to the tool bar
    if( AppData.hToolBar )
    {
        // Set the offset for the next item after the buttons
        // One of the buttons is a spacer but is much smaller than a regular button
        // The TBBUTTON_WIDTH is just the graphic but there is still meat around that graphice which
        // is the button. The TBBUTTON_ADDITION is that extra meat.
        TbButtonOffset = ((NUM_TBBUTTONS-1) * (TBBUTTON_WIDTH+TBBUTTON_ADDITION)) + TBBUTTON_SPACER;

        // Add an edit field to display the current page
        AppData.hEditContrls[ EDT_CUR_PAGE ] = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, "edit", NULL, 
                                               WS_CHILD|WS_VISIBLE|WS_BORDER|ES_NUMBER|ES_RIGHT|WS_DISABLED,
                                               TbButtonOffset, 2, 35, 22, AppData.hToolBar, NULL, 
                                               AppData.hInst, NULL );

        if( AppData.hEditContrls[ EDT_CUR_PAGE ] )
        {
            ApplyWin95Font( AppData.hEditContrls[ EDT_CUR_PAGE ] );

            // Subclass Edit box control
            AppData.OldEditProc[ EDT_CUR_PAGE ] = (WNDPROC)SetWindowLong( AppData.hEditContrls[ EDT_CUR_PAGE ], 
                                                                          GWL_WNDPROC, LONG( EdtPageTo_Delay_Proc ) );
        }

        // Set the offset for the next item after the buttons
        TbButtonOffset += 42;

        // Add a static to the combo box
        AppData.hTBStatic = CreateWindow( "static", NULL, WS_CHILD|WS_VISIBLE|SS_LEFT|WS_DISABLED,
                              TbButtonOffset, 6, 40, TBBUTTON_HEIGHT, AppData.hToolBar, NULL, AppData.hInst, NULL );

        // Get the client rect of the tool bar
        GetClientRect( AppData.hToolBar, &ToolBarRect );

        // Calculate the Y offset from the tool bar
        AppData.ChildOffsetY = ToolBarRect.bottom + GetSystemMetrics( SM_CYBORDER ) + (GetSystemMetrics( SM_CYEDGE )*2);

        if( AppData.hTBStatic )
            ApplyWin95Font( AppData.hTBStatic );
        else
            Result = FALSE;
    }
    else
        Result = FALSE;

    return Result;

}   // CreateToolBar



/************************************************************************
*    FUNCTION NAME:         CreateControls                                                             
*
*    DESCRIPTION:           Create the window controls 
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

BOOL TSS2GO::CreateControls( HWND hwnd )
{
    BOOL Result = TRUE;
    int OffsetY, i, j;
    LPSTR StringTxt[] = { "Message Across Top:", "Message Across Bottom:" };
    LPSTR JustRes[] = { "left", "center", "right" };
    LPSTR AttRes[] = { "fonts", "colors" };
    DWORD ButtonStyles;
    int StatusSect[MAX_STATUS_PANELS];
    RECT r;

    OffsetY = AppData.ChildOffsetY;

    // Create the "Size Bitmap To Fit" Checkbox
    AppData.hChkSizeBMP = CreateWindow( "button", "Size BMP To Fit", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_DISABLED,
                          VIEW_WND_W + (CTRL_OFFSET_X * 2), OffsetY + 10, 100, 20, 
                          hwnd, (HMENU)CHK_SIZE_TO_FIT, AppData.hInst, NULL );
    if( AppData.hChkSizeBMP )
        ApplyWin95Font( AppData.hChkSizeBMP );

    // Create the "Delay:" lable
    AppData.hStatic[0] = CreateWindow( "static", "Delay:", WS_CHILD|WS_VISIBLE|ES_LEFT|WS_DISABLED,
                          VIEW_WND_W + (CTRL_OFFSET_X * 2) + 200, OffsetY + 13, 40, 20, 
                          hwnd, NULL, AppData.hInst, NULL );
    if( AppData.hStatic[0] )
        ApplyWin95Font( AppData.hStatic[0] );

    // Create the delay edit field
    AppData.hEditContrls[ EDT_DELAY_IN_SEC ] = CreateWindowEx( WS_EX_CLIENTEDGE, "edit", NULL, 
                                               WS_CHILD|WS_VISIBLE|ES_LEFT|ES_NUMBER|WS_DISABLED,
                                               VIEW_WND_W + (CTRL_OFFSET_X * 2) + 240, OffsetY + 7, 30, 22, 
                                               hwnd, NULL, AppData.hInst, NULL );

    if( AppData.hEditContrls[ EDT_DELAY_IN_SEC ] )
    {
        ApplyWin95Font( AppData.hEditContrls[ EDT_DELAY_IN_SEC ] );

        // Limit the number of characters they can type in
        Edit_LimitText( AppData.hEditContrls[ EDT_DELAY_IN_SEC ], 2 );

        // Subclass Edit box control
        AppData.OldEditProc[ EDT_DELAY_IN_SEC ] = (WNDPROC)SetWindowLong( AppData.hEditContrls[ EDT_DELAY_IN_SEC ], 
                                                                          GWL_WNDPROC, LONG( EdtPageTo_Delay_Proc ) );
    }

    // Inc the offset
    OffsetY += 35;

    for( i = 0; i < MAX_BTN_GROUPS; ++i )
    {
        for( j = 0; j < MAX_JUST_BTNS; ++j )
        {
            // Use the WS_GROUP flag to specify the start of the group
            if( !j )
                ButtonStyles = WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_AUTORADIOBUTTON|WS_GROUP|WS_DISABLED;
            else
                ButtonStyles = WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_AUTORADIOBUTTON|WS_DISABLED;

            // Create the Left, Center and Right buttons
            AppData.hBtnJust[j+(i*MAX_JUST_BTNS)] = CreateWindow( "button", NULL, ButtonStyles,
                                               VIEW_WND_W + (CTRL_OFFSET_X * 2) + 135 + (j * 24), OffsetY + 10, 22, 16, 
                                               hwnd, (HMENU)(BTN_TXT_TOP_JUST_LEFT + j + (i*MAX_JUST_BTNS)), AppData.hInst, NULL );

            // Assign a bitmap to the buttons
            SendMessage( AppData.hBtnJust[j+(i*MAX_JUST_BTNS)], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                         (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, JustRes[j] ) );

        }

        for( j = 0; j < MAX_STYLE_BTNS; ++j )
        {
            // Create the Font and Color dialog buttons
            AppData.hFontDlg[j+(i*MAX_STYLE_BTNS)] = CreateWindow( "button", NULL, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_BITMAP|WS_DISABLED,
                                               VIEW_WND_W + (CTRL_OFFSET_X * 2) + 222 + (j * 24), OffsetY + 10, 22, 16, 
                                               hwnd, (HMENU)(BTN_TOP_FONT_SET + j + (i*MAX_STYLE_BTNS)), AppData.hInst, NULL );

            // Assign a bitmap to the buttons
            SendMessage( AppData.hFontDlg[j+(i*MAX_STYLE_BTNS)], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                         (LPARAM) (HANDLE)LoadBitmap( AppData.hInst, AttRes[j] ) );
        }

        // Create the static labels
        AppData.hStatic[1+i] = CreateWindow( "static", StringTxt[i], WS_CHILD|WS_VISIBLE|ES_LEFT|WS_DISABLED,
                              VIEW_WND_W + (CTRL_OFFSET_X * 2), OffsetY + 10, 130, 20, 
                              hwnd, NULL, AppData.hInst, NULL );
        if( AppData.hStatic[1+i] )
            ApplyWin95Font( AppData.hStatic[1+i] );

        // Add a message edit box
        AppData.hEditMsg[i] = CreateWindowEx( WS_EX_CLIENTEDGE, "edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|WS_DISABLED|ES_MULTILINE|WS_VSCROLL,
                              VIEW_WND_W + (CTRL_OFFSET_X * 2), OffsetY + 30, 271, 78, 
                              hwnd, NULL, AppData.hInst, NULL );

        if( AppData.hEditMsg[i] )
        {
            // Use the Windows 95 font
            ApplyWin95Font( AppData.hEditMsg[i] );

            // Limit the number of characters they can type in
            Edit_LimitText( AppData.hEditMsg[i], MAX_PAGE_CAPTION );

            // Subclass Edit box control
            AppData.OldEditTxtProc[i] = (WNDPROC)SetWindowLong( AppData.hEditMsg[i], GWL_WNDPROC, LONG( EdtTopBotCaption_Proc ) );
        }

        // Inc the offset
        OffsetY += 113;
    }

    // Create the status window
    AppData.hStatusWnd = CreateStatusWindow( WS_CHILD|WS_VISIBLE, NULL, hwnd, 0 );

    if( AppData.hStatusWnd )
    {
        // Use the windows 95 font
        ApplyWin95Font( AppData.hStatusWnd );

        // Get the rect of the statuse window
        GetClientRect( AppData.hStatusWnd, &r );

        // Divide the width of the status window into three sections
        for( i = 0; i < MAX_STATUS_PANELS; ++i )
            StatusSect[i] = r.right / MAX_STATUS_PANELS * (i+1);

        // Tell the window to create the panels
        SendMessage( AppData.hStatusWnd, SB_SETPARTS, (WPARAM)MAX_STATUS_PANELS, (LPARAM)StatusSect );
    }

    return Result;

}   // CreateControls



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
*    h rosenorn    8/29/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::SetWindowCaption( HWND hwnd )
{
    char StrTmp[MAX_TITLE + SMALL_ARRAY_50];
    char FileNameOnly[MAX_PATH+1];

    // Display the caption to show if this program is registered or shareware
    // Init to a null string
    FileNameOnly[0] = 0;

    // Pull the file name from the path
    if( ActProj.FilePath[0] )
        CropFileNameFromPath( ActProj.FilePath, FileNameOnly );

    // Setup the window caption string
	sprintf( StrTmp, "%s  %s    %s", PRODUCT_NAME, PRODUCT_VERSION, FileNameOnly );

    SetWindowText( hwnd, StrTmp );

}   // SetWindowCaption



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

void TSS2GO::OnDestroy( HWND hwnd ) 
{
    // Delete the temporary screen saver located in the temp folder
    DeleteTmpSaver();

    // Kill the program
    PostQuitMessage(0);

} // OnDestroy



/************************************************************************
*    FUNCTION NAME:         OnCommand                                                             
*
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*                           We are handling the click messages that are
*                           sent when an item is clicked in a listbox
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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {             
        case MNU_NEW:
            mnuNew( hwnd );
        break;
              
        case MNU_OPEN:
            mnuOpen( hwnd );
        break;
          
        case MNU_SAVE:
            mnuSave( hwnd );
        break;
          
        case MNU_SAVEAS:
            mnuSaveAs( hwnd );
        break;

        case MNU_CLOSE:
            mnuClose( hwnd );
        break;

        case MNU_PREFS:
            DisplayPrefsDialogBox( AppData.hInst, hwnd, &ProjPrefsData, &ActProj );
        break;

        case MNU_ADD_MAIN_IMAGE:
            mnuAddPage( hwnd, PAGE_TYPE_MAIN );
        break;

        case MNU_ADD_BK_IMAGE:
            mnuAddBackImage( hwnd );
        break;

        case MNU_DELETE_PAGE:
            DeletePage( hwnd );
        break;

        case MNU_MOVE_PAGE:
            MovePage( hwnd );
        break;

        case MNU_ADD_AVI:

        break;

        case MNU_ADD_SOUND:
            AddSound( hwnd );
        break;

        case MNU_REMOVE_SOUND:
            RemoveSound( hwnd );
        break;

        case MNU_HLP_CONTENTS:
            DisplayHelpFile( hwnd, HELP_CONTENTS, 0 );
        break;

        case MNU_HLP_INDEX:
            DisplayHelpFile( hwnd, HELP_PARTIALKEY, 0 );
        break;

        case MNU_RUN_SAVER:
            RunProject( hwnd, MNU_RUN_SAVER );
        break;

        case MNU_RUN_PREFS:
            RunProject( hwnd, MNU_RUN_PREFS );
        break;

        case MNU_RUN_INSTALLER:
            RunProject( hwnd, MNU_RUN_INSTALLER );
        break;

        case MNU_PRINT_SETUP:
        	Printer.DisplayPrinterSetup();
        break;

        case MNU_PRINT:
        	Print( hwnd );
        break;
         
        case MNU_EXIT:
            // Post the close message
            PostMessage( hwnd, WM_CLOSE, 0, 0 );
        break;
          
        case MNU_ABOUT:
            ShowAboutDlg( hwnd );
        break;

        case BTN_FLIP_LEFT:
            if( PageList.Count() && PageList.GetCurrentPage() > 0  )
                FlipPage( hwnd, PAGE_FLIP_LEFT );
        break;

        case BTN_FLIP_RIGHT:
            if( PageList.Count() && PageList.GetCurrentPage() < PageList.Count()-1 )
                FlipPage( hwnd, PAGE_FLIP_RIGHT );
        break;

        case BTN_SOUND_PLAY:
            if( PageList.Count() && !mediaPlayer.GetPlayError() && PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                PlaySoundFile( hwnd, PageList.GetCurrentPage() );
        break;

        case BTN_SOUND_PAUSE:
            if( PageList.Count() && !mediaPlayer.GetPlayError() )
            {
                // If a file is playing, pause it
                if( mediaPlayer.GetStatus() == PLAYER_PLAYING )    
                   mediaPlayer.Pause();

                // if the file is paused, play it
                else if( mediaPlayer.GetStatus() == PLAYER_PAUSED )
                    mediaPlayer.Play();
            }
        break;

        case BTN_SOUND_STOP:
            if( PageList.Count() && !mediaPlayer.GetPlayError() )
            {
                // If a file is playing, stop it by closing it.
                if( mediaPlayer.GetStatus() != PLAYER_CLOSED )
                {
                    mediaPlayer.Close();

                    // If this page doesn't have a sound file associated with it
                    // then disable the stop button
                    if( !PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                        TBBtn_SetState( AppData.hToolBar, BTN_SOUND_STOP, DISABLE );
                }
            }
        break;

        case MNU_WEB_LINK:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;

        case POST_MSG_AFTER_WM_SHOW:
            HandlePostMeg( hwnd );
        break;

        case CHK_SIZE_TO_FIT:
            // Save size to fit option
            PageList.SetSizeToFit( PageList.GetCurrentPage(), Button_GetCheck( AppData.hChkSizeBMP ) );

            // Set the view window
            BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );
            
            // Force the view window to repaint
            ViewWnd_Repaint();            
        break;

        case MNU_BACKGROUND_COLOR:
            SetColor( hwnd, MNU_BACKGROUND_COLOR );
        break;

        case MNU_CHANGE_MAIN_IMAGE:
            mnuAddPage( hwnd, PAGE_TYPE_RELOAD_MAIN );
        break;

        case BTN_TOP_FONT_SET:
            SetTextFont( hwnd, BTN_TOP_FONT_SET );
        break;

        case BTN_BOT_FONT_SET:
            SetTextFont( hwnd, BTN_BOT_FONT_SET );
        break;

        case BTN_TOP_FONT_CLR:
            SetColor( hwnd, BTN_TOP_FONT_CLR );
        break;

        case BTN_BOT_FONT_CLR:
            SetColor( hwnd, BTN_BOT_FONT_CLR );
        break;

        case BTN_TXT_TOP_JUST_LEFT:
        case BTN_TXT_TOP_JUST_CENT:
        case BTN_TXT_TOP_JUST_RIGH:
            // Save the justification
            PageList.SetTopTxtJust( PageList.GetCurrentPage(), id - BTN_TXT_TOP_JUST_LEFT );

            ActProj.LiveProject = TRUE;

            // Set the view window
            BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

            // Force the view window to repaint
            ViewWnd_Repaint();
        break;

        case BTN_TXT_BOT_JUST_LEFT:
        case BTN_TXT_BOT_JUST_CENT:
        case BTN_TXT_BOT_JUST_RIGH:
            // Save the justification
            PageList.SetBotTxtJust( PageList.GetCurrentPage(), id - BTN_TXT_BOT_JUST_LEFT );

            ActProj.LiveProject = TRUE;

            // Set the view window
            BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

            // Force the view window to repaint
            ViewWnd_Repaint();
        break;

        case MNU_FIND_SCREENSAVER:
            FindSaver( hwnd );  
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         SetTextFont                                                             
*
*    DESCRIPTION:           get the new font the user wants
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
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::SetTextFont( HWND hwnd, int FontDialogButton )
{
    CHOOSEFONT cf;
    LOGFONT    lf;

    // Memset the two structures
    memset( &cf, 0, sizeof( CHOOSEFONT ) );

    // Init with the current log font
    if( FontDialogButton == BTN_TOP_FONT_SET )
        memcpy( &lf, PageList.GetPtrToTopLogfont( PageList.GetCurrentPage() ), sizeof( LOGFONT ) );
    else if( FontDialogButton == BTN_BOT_FONT_SET )
        memcpy( &lf, PageList.GetPtrToBotLogfont( PageList.GetCurrentPage() ), sizeof( LOGFONT ) );

    // Init the CHOOSEFONT structure
    cf.lStructSize = sizeof( CHOOSEFONT );
    cf.hwndOwner   = hwnd;
    cf.Flags       = CF_BOTH|CF_FORCEFONTEXIST|CF_INITTOLOGFONTSTRUCT;
    cf.lpLogFont   = &lf;

    // Save the font choices
    if( ChooseFont( &cf ) )
    {
        // Copy over the logfont structure to save as default
        memcpy( Settings.GetLogFont(), &lf, sizeof( LOGFONT ) );

        // Save the font information to the top or bottom text fields
        if( FontDialogButton == BTN_TOP_FONT_SET )
            memcpy( PageList.GetPtrToTopLogfont( PageList.GetCurrentPage() ), &lf, sizeof( LOGFONT ) );
        else if( FontDialogButton == BTN_BOT_FONT_SET )
            memcpy( PageList.GetPtrToBotLogfont( PageList.GetCurrentPage() ), &lf, sizeof( LOGFONT ) );

        // Redraw the view window
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();

        // Set the project live status
        ActProj.LiveProject = TRUE;
    }

}   // SetTextFont


/************************************************************************
*    FUNCTION NAME:         SetColor                                                             
*
*    DESCRIPTION:           Set the background or text color
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::SetColor( HWND hwnd, int ColorDialog )
{
    CHOOSECOLOR cc;

    // Zero out the structure
    memset( &cc, 0, sizeof( CHOOSECOLOR ) );

    // fill out the structure
    cc.lStructSize = sizeof( CHOOSECOLOR ); 
    cc.hwndOwner = hwnd;   
    cc.Flags = CC_FULLOPEN;
    cc.lpCustColors = Settings.GetPtrCustomColors();
         
    // Display the color dialog box
    if( ChooseColor( &cc ) )
    {
        if( ColorDialog == MNU_BACKGROUND_COLOR )
        {
            // Set the chosen color
            PageList.SetBackColor( PageList.GetCurrentPage(), cc.rgbResult );

            // Set the data type
            PageList.SetBackDataType( PageList.GetCurrentPage(), FILE_TYPE_COLOR );
        }
        else if( ColorDialog == BTN_TOP_FONT_CLR )
        {
            // Set the chosen color
            PageList.SetTopFontColor( PageList.GetCurrentPage(), cc.rgbResult );
            Settings.SetDefTxtColor( cc.rgbResult );
        }
        else if( ColorDialog == BTN_BOT_FONT_CLR )
        {
            // Set the chosen color
            PageList.SetBotFontColor( PageList.GetCurrentPage(), cc.rgbResult );
            Settings.SetDefTxtColor( cc.rgbResult );
        }

        // Redraw the view window
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();

        // Update the interface
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

        // Set the project live status
        ActProj.LiveProject = TRUE;
    }

}   // SetColor


/************************************************************************
*    FUNCTION NAME:         HandlePostMeg                                                             
*
*    DESCRIPTION:           Handles the POST_MSG_AFTER_WM_SHOW message
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::HandlePostMeg( HWND hwnd )
{
    // Show the startup screen
    ShowStartUpScreen( hwnd );

    // Show the helpful hint window
    ShowHelpFulHint( hwnd );

}   // HandlePostMeg


/************************************************************************
*    FUNCTION NAME:         AddSound                                                             
*
*    DESCRIPTION:           Add sound file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::AddSound( HWND hwnd )
{
    int i;

    if( MediaDir_GetSoundDirectory( AppData.hInst, hwnd, Settings.GetProjectSoundPath(), 
                                PageList.GetSoundDataPath( PageList.GetCurrentPage() ) ) )
    {
        // record the sound type
        PageList.SetSoundDataType( PageList.GetCurrentPage(),
            GetFileType( PageList.GetSoundDataPath( PageList.GetCurrentPage() ) ) );

        for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
            TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PLAY+i, ENABLE );

        // Update the interface
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

        // Enable the remove menu item
        EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_ENABLED );

        ActProj.LiveProject = TRUE;
    }
}   // AddSound



/************************************************************************
*    FUNCTION NAME:         DeletePage                                                             
*
*    DESCRIPTION:           Delete the current page
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::DeletePage( HWND hwnd )
{
    if( PageList.Count() )
    {
        if( MessageBox( hwnd, "Do you want to delete this page from your project?",
            "Question", MB_YESNO ) == IDYES )
        {
            DeleteCurPage( hwnd );
            ActProj.LiveProject = TRUE;
        }
    }
}   // DeletePage


/************************************************************************
*    FUNCTION NAME:         DeletePage                                                             
*
*    DESCRIPTION:           Remove sound file from the current page
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::RemoveSound( HWND hwnd )
{
    int i;

    if( MessageBox( hwnd, "Do you want to remove the sound from this page?",
        "Question", MB_YESNO ) == IDYES )
    {
        // Clear out the sound path
        memset( PageList.GetSoundDataPath( PageList.GetCurrentPage() ), 0, sizeof(char)*MAX_STR_PATH );

        // Reset the sound type to no type
        PageList.SetSoundDataType( PageList.GetCurrentPage(), NO_FILE_TYPE );

        // Disable the sound buttons
        for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
            TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PLAY+i, DISABLE );

        // Update the interface
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

        // Gray out the menu item
        EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_GRAYED|MF_DISABLED );

        ActProj.LiveProject = TRUE;
    }
}


/************************************************************************
*    FUNCTION NAME:         PlaySoundFile                                                             
*
*    DESCRIPTION:           Loads and plays the sound file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::PlaySoundFile( HWND hwnd, WORD PageToLoad )
{
    if( mediaPlayer.GetStatus() == PLAYER_PAUSED )
        mediaPlayer.Play();
    else
    {
        // If the player is not closed, close it
        if( mediaPlayer.GetStatus() != PLAYER_CLOSED )
            mediaPlayer.Close();

        // Check to see that the file is still there. The could have moved it.
        if( !FileExist( PageList.GetSoundDataPath( PageToLoad ) ) )
        {
            // Ask if they want to find the missing project file
            if( FindMissingProjectFile( hwnd, PageList.GetSoundDataPath( PageToLoad ), 
                Settings.GetProjectSoundPath(), SOUND_FILE_TYPES ) )
                ActProj.LiveProject = TRUE;
            else
                return;
        }

        mediaPlayer.Play( PageList.GetSoundDataPath( PageToLoad ) );
    }

}   // PlaySoundFile


/************************************************************************
*    FUNCTION NAME:         DisplayHelpFile                                                             
*
*    DESCRIPTION:           Displays the help file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message
*              UINT Command - Type of action
*              DWORD Index  - Help file article depending on the command 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::DisplayHelpFile( HWND hwnd, UINT Command, DWORD Index )
{
    char HelpFilePath[MAX_STR_PATH];

    // Build the path to the help file
    CropOutPath( AppData.Path, HelpFilePath );
    strcat( HelpFilePath, HELP_FILE_NAME );
    
    // show the help file
    WinHelp( hwnd, HelpFilePath, Command, Index );

}   // DisplayHelpFile


/************************************************************************
*    FUNCTION NAME:         Print                                                             
*
*    DESCRIPTION:           Print a page to the printer
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
*    h rosenorn    8/23/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TSS2GO::Print( HWND hwnd )
{
    LPPRINTDLG ppdlg;
    int i, j;
    RECT rect;
    double PrintableW, PrintableH, MultiW, MultiH, Multiplyer;
    double ImageW, ImageH, MarginX, MarginY;
    HWND hPrintingWnd = NULL;
    char PageNumber[20];
    BOOL Result = FALSE;
    TBitmap PrintBuffer;

    if( PageList.Count() )
    {
        ppdlg = Printer.DisplayPrintDlg( PageList.GetCurrentPage()+1, PageList.Count() );

        if( ppdlg )
        {
            if( Printer.BeginDoc( "Print page(s)" ) )
            {
				// Init the creation of the printer buffer with the printer dc
				PrintBuffer.InitActiveDC( Printer.Handle() );

                // Make the printer buffer
                if( !PrintBuffer.CreateDIBSectionBuffer( GetScreenWidth(), GetScreenHeight(), BIT_COUNT_24 ) )
                {
                    PostMsg( "Printing Error", "Error allocating printer buffer." );
                    goto ERROR_JUMP;
                }

                // Using CreateDialog causes the dialog to return rightaway.
                hPrintingWnd = DisplayPrintCancelDlg( GetAppInstance(), hwnd, "Printing Page...", TRUE );
                if( !hPrintingWnd )
                    goto ERROR_JUMP;

                // Set the printer abort function
                SetAbortProc( Printer.Handle(), (ABORTPROC)PrintCancelDlg_AbortFunc );
            
                for( i = ppdlg->nFromPage; i < ppdlg->nToPage+1; ++i )
                {
                    // Load the main image file
                    if( !LoadImageFile( hwnd, PageList.GetMainDataPath( i-1 ), &MainDib ) )
                        goto ERROR_JUMP;

                    // The background can be a color
                    if( PageList.GetBackDataType( i-1 ) > NO_FILE_TYPE && 
                        PageList.GetBackDataType( i-1 ) < FILE_TYPE_META )
                    {
                        // Load the background image file
                        if( !LoadImageFile( hwnd, PageList.GetBackDataPath( i-1 ), &BackDib ) )
                            goto ERROR_JUMP;
                    }

                    // Set the view window
                    BldBuffer_ResetBuffer( &PrintBuffer, &MainDib, &BackDib, i-1, &PageList );

                    // Update the printing status
                    PrintCancelDlg_UpdateStatus( "Printing Page (%u)", i );
                    Sleep(100);

                    // Setup the margins
                    MarginX = Printer.GetpixelsPerInchX() / 2;
                    MarginY = Printer.GetpixelsPerInchY() / 2;

                    PrintableW = Printer.width();
                    PrintableH = Printer.height();

                    // The width or the height of the print area can't be zero
                    if( !PrintableW || !PrintableH ) 
                    {
                        PostMsg( "Printing Error", "Print area has no size" );
                        goto ERROR_JUMP;
                    }

                    // Determine the greatest size to blit into
                    PrintableW -= (MarginX * 2);
                    PrintableH -= ( MarginY * 2);

                    // Center the bitmap on the page
                    ImageW = PrintBuffer.width();
                    ImageH = PrintBuffer.height();

                    // The width or the height of any bitmap can't be zero. There is a problem
                    if( !ImageW || !ImageH )
                    {
                        PostMsg( "Printing Error", "Bitmap has no size" );
                        goto ERROR_JUMP;
                    }

                    // Who is bigger. The image or the printing surface
                    // Divide the largest number against the smaller number
                    if( ImageW < PrintableW )
                        MultiW = PrintableW / ImageW;
                    else
                        MultiW = ImageW / PrintableW;

                    if( ImageH < PrintableH )
                        MultiH = PrintableH / ImageH;
                    else
                        MultiH = ImageH / PrintableH;                       

                    // If the image is smaller than the printable area
                    // Multiply the multiplyer. If it is bigger than the
                    // printable area then divide.
                    if( ImageW < PrintableW && ImageH < PrintableH ) // Enlarge to fit
                    {
                        // See if one side will enlarge past the window rect
                        // Add 1 to compensate for floating point math to the window sizes
                        if( ImageW * MultiW > PrintableW+1 || ImageH * MultiW > PrintableH+1 )
                            Multiplyer = MultiH;
                        else if( ImageW * MultiH > PrintableW+1 || ImageH * MultiH > PrintableH+1 )
                            Multiplyer = MultiW;
                        else
                        {
                            // Since none of the windows extends past the window rect
                            // take the larger multiplyer
                            if( MultiW > MultiH )
                                Multiplyer = MultiW;
                            else
                                Multiplyer = MultiH;
                        }

                        rect.right = ImageW * Multiplyer;
                        rect.bottom = ImageH * Multiplyer;
                    }
                    else  // Reduce to fit
                    {
                        // See if one side will enlarge past the window rect
                        // Add 1 to compensate for floating point math to the window sizes
                        if( ImageW / MultiW > PrintableW+1 || ImageH / MultiW > PrintableH+1 )
                            Multiplyer = MultiH;
                        else if( ImageW / MultiH > PrintableW+1 || ImageH / MultiH > PrintableH+1 )
                            Multiplyer = MultiW;
                        else
                        {
                            // Since none of the windows extends past the window rect
                            // take the smaller multiplyer
                            if( MultiW < MultiH )
                                Multiplyer = MultiW;
                            else
                                Multiplyer = MultiH;
                        }

                        rect.right = ImageW / Multiplyer;
                        rect.bottom = ImageH / Multiplyer;
                    }

                    // Center the image onto the page
                    rect.left = ((PrintableW - rect.right) / 2) + MarginX;
                    rect.top = ((PrintableH - rect.bottom) / 2) + MarginY;

                    // Make the font
                    Printer.SetFont( "Arial", 8 );

                    for( j = 0; j < ppdlg->nCopies; ++j )
                    {
                        // Jump out if the user has canceled the print job
                        if( !PrintCancelDlg_GetPrintCancel() )
                            goto ERROR_JUMP;

                        Printer.BeginPage();

                        // If we are using a custom font, we'll need to remake the other font as well
                        Printer.SetFont( "Arial", 8 );

                        // Print out the page number on the bottom center of the screen
                        sprintf( PageNumber, "(%u)", i );
                        Printer.PrintText( (Printer.width()-Printer.TextWidth( PageNumber )) / 2, 
                                   Printer.height() - Printer.TextHeight( PageNumber ) - (MarginY / 2), PageNumber );

                        Printer.PrintBitmap( &PrintBuffer, &rect );

                        Printer.StopPage();
                    }
                }
            }
        }
    }

    // If we made it this far, the printing went ok
    Result = TRUE;

ERROR_JUMP:;

    // Were done. Stop everything
    Printer.StopDoc();

    if( hPrintingWnd )
        DestroyWindow( hPrintingWnd );      

    // Free all printer objects
    Printer.Free();

    return Result;

}   // Print



/************************************************************************
*    FUNCTION NAME:         DeleteTmpSaver                                                             
*
*    DESCRIPTION:           Delete the temporary screen saver in the temp folder 
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
*    h rosenorn    8/29/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::DeleteTmpSaver()
{
    char PathToEXE[MAX_PATH+1];

    // Make sure we have a file name to delete
    if( ProjPrefsData.ExeName[0] )
    {
        // Get the temporary directory that is usually in the windows
        // directory. Build the screen saver EXE there so that there isn't
        // two exe's in the project folder to confuse the user.
        //GetTheWindowsTmpDir( TempDir );

        // Put together the path to the new exe file        
        AddFileToOtherPath( ProjPrefsData.ExeName, "c:\\", PathToEXE );

        // Delete the temporary screen saver
        DeleteFile( PathToEXE );
    }

}   // DeleteTmpSaver


/************************************************************************
*    FUNCTION NAME:         RunProject                                                             
*
*    DESCRIPTION:           Builds and Runs the project control panel
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
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::RunProject( HWND hwnd, int RunType )
{
    char PathToEXE[MAX_STR_PATH];
    char TempDir[MAX_PATH+1]={"c:\\"};
    char Param[5];
    BOOL BuildResult = TRUE;
    HWND ViewWindow;

    // See if they have a project exe running. If so, kill the program
    // because we can't save the EXE while the other EXE is running.
    if( RunType == MNU_RUN_INSTALLER )
    {
        ViewWindow = FindWindow( INSTCLASSNAME, NULL );
     
        if( ViewWindow )
        {
            PostMessage( ViewWindow, WM_CLOSE, 0, 0 );
            Sleep( 500 );
        }
    }

    // Do we want to build the EXE?
    if( ActProj.LiveProject )
        BuildResult = mnuBuild_EXE( hwnd );

    if( BuildResult && PageList.Count() )
    {
        // Get the temporary directory that is usually in the windows
        // directory. Build the screen saver EXE there so that there isn't
        // two exe's in the project folder to confuse the user.
        //GetTheWindowsTmpDir( TempDir );

        // Put together the path to the new exe file        
        AddFileToOtherPath( ProjPrefsData.ExeName, TempDir, PathToEXE );

        // Check to see if the file is there. If not, build it.
        if( !FileExist( PathToEXE ) )
            if( !mnuBuild_EXE( hwnd ) )
                return;

        // Setup the parameters
        if( RunType == MNU_RUN_SAVER )
            strcpy( Param, "/S" );
        else if( RunType == MNU_RUN_PREFS )
            strcpy( Param, "/C" );
        else if( RunType == MNU_RUN_INSTALLER )
        {
            Param[0] = 0;
            // Put together the path to the new exe file        
            AddFileToOtherPath( ProjPrefsData.ExeName, ActProj.FilePath, PathToEXE );
            CropOutPath( ActProj.FilePath, TempDir );
        }  

        // If the function fails, the return value is an error value that is less than or equal to 32
        // The last aparam is a 0 to keep the DOS window from showing.
        if( ShellExecute( hwnd, "open", PathToEXE, Param, TempDir, SW_SHOWNORMAL ) <= (HINSTANCE)32 )
            PostMsg( "Opps","There has been a error running the project executable." );
    }

}   // RunProject


/************************************************************************
*    FUNCTION NAME:         FindSaver                                                             
*
*    DESCRIPTION:           Finds the screen saver file by bringing up the
*                           directory it is in.
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
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::FindSaver( HWND hwnd )
{
    char TempDir[MAX_PATH+1];
    BOOL BuildResult = TRUE;

    // Do we want to build the EXE?
    if( ActProj.LiveProject )
        BuildResult = mnuBuild_EXE( hwnd );

    if( BuildResult && PageList.Count() )
    {
        // Get the project directory.
        CropOutPath( ActProj.FilePath, TempDir );

        // Check to see if the file is there. If not, build it.
        if( !FileExist( ActProj.FilePath ) )
            if( !mnuBuild_EXE( hwnd ) )
                return;
                
        // Tell them what file they want
		if( Settings.GetShowInstallInfo() )
        	Settings.SetShowInstallInfo( Display_YourInstallerDlg( AppData.hInst, hwnd ) );  

        // If the function fails, the return value is an error value that is less than or equal to 32
        // The last aparam is a 0 to keep the DOS window from showing.
        if( ShellExecute( NULL, "open", TempDir, NULL, NULL, SW_SHOWNORMAL ) <= (HINSTANCE)32 )
            PostMsg( "Opps","There has been a error finding the project executable." );
    }
}   // FindSaver


/************************************************************************
*    FUNCTION NAME:         DeleteCurPage                                                             
*
*    DESCRIPTION:           Deletes the current page
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
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::DeleteCurPage( HWND hwnd )
{
    WORD CurentPage;

    // Get the current page
    CurentPage = PageList.GetCurrentPage();

    // Delete the current page
    PageList.Delete( CurentPage );

    if( CurentPage < PageList.Count() )
        PageList.SetCurrentPage( CurentPage );
    else if( PageList.Count() )
        PageList.SetCurrentPage( CurentPage-1 );

    FlipPage( hwnd, 0, TRUE );

}   // DeleteCurPage


/************************************************************************
*    FUNCTION NAME:         MovePage                                                             
*
*    DESCRIPTION:           Moves the current page
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
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::MovePage( HWND hwnd )
{
    int PageMovedTo;

    // Get the page they want to move the current page to
    PageMovedTo = DisplayMovePageDlg( AppData.hInst, hwnd, PageList.GetCurrentPage(), PageList.Count() );

    // Only if it is in range
    if( PageMovedTo >= 0 )
    {
        // Move this page to the new index
        PageList.MoveToNewPos( PageList.GetCurrentPage(), PageMovedTo );

        // Make the new index the current page
        PageList.SetCurrentPage( PageMovedTo );

        // Update the display
        FlipPage( hwnd, 0, TRUE );

        // Flag that this project needs to be saved
        ActProj.LiveProject = TRUE;
    }

}   // MovePage


/************************************************************************
*    FUNCTION NAME:         ShowStartUpScreen                                                             
*
*    DESCRIPTION:           Shows the startup screen be it for the tool
*                           or the viewer program
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

void TSS2GO::ShowStartUpScreen( HWND hwnd )
{
    // Give it a second before the startup screen it displayed
    Sleep( 200 );

    // Display the startup screen
    HWND hStartUpScrnWnd = DisplayStartUpScreen( AppData.hInst, hwnd, "startup", "GIF", NULL );

    // Did we get a window?
    if( hStartUpScrnWnd )
    {
        // Hold the program for a few seconds to display the startup screen
        Sleep( 4000 );
        
        // Destroy the dialog window
        DestroyWindow( hStartUpScrnWnd );
    }

}   // ShowStartUpScreen



/************************************************************************
*    FUNCTION NAME:         ShowAboutDlg                                                             
*
*    DESCRIPTION:           Shows the about dialog box
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

void TSS2GO::ShowAboutDlg( HWND hwnd )
{
    // Display the startup screen
    DisplayAboutDlg( AppData.hInst, hwnd, ABOUT_CAPTION, ABOUT_MSG, "startup", "GIF", NULL );
     
}   // ShowAboutDlg


/************************************************************************
*    FUNCTION NAME:         ShowHelpFulHint                                                             
*
*    DESCRIPTION:           Shows the helpful hint be it for the tool
*                           or the viewer program
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

void TSS2GO::ShowHelpFulHint( HWND hwnd )
{
    // Display the helpful hint window
    BOOL ShowNextTime;

    // Do they want to see the helpful hint window?
    if( Settings.GetIntroHelpWnd() )
    {
        Sleep( 400 );

        DisplayHelpHintDialogBox( AppData.hInst, hwnd, "IntroTxt", "TXT", 
                              "Helpful Hint", "Registration", &ShowNextTime, DO_CHECK_SUM, NULL, 
                              0, CENTER_WND_TO_PARENT );

        Settings.SetIntroHelpWnd( ShowNextTime );
    }

}   // ShowHelpFulHint


/************************************************************************
*    FUNCTION NAME:         mnuNew                                                             
*
*    DESCRIPTION:           New message handler
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

void TSS2GO::mnuNew( HWND hwnd )
{
    // Disanable all the menu items
    if( !CheckToSave( hwnd ) )
        return;

    // Force them to have to save a project file first before
    // displaying the preferences dialog box.
    if( GetFileName( hwnd, AppData.hInst, ActProj.FilePath, 
                     SAVE_FILE_CAPTION, Settings.GetProjectPath(), 
                     PROJECT_FILE_MASK, PROJECT_EXT, 
                     SAVE_FILENAME_DIALOG_BOX ) ) // Check to see if they canceled out
    {
        char TmpFileName[MAX_STR_PATH];

        // Copy over the file path because the InitProject clears it out
        strcpy( TmpFileName, ActProj.FilePath );

        // Init a new project
        InitProject( hwnd );

        // Copy back the file name
        strcpy( ActProj.FilePath, TmpFileName );

        // Size the view window
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, 0, &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();

        // Save the new project file
        if( SaveToFile( hwnd, ActProj.FilePath ) )
        {
            if( DisplayPrefsDialogBox( AppData.hInst, hwnd, &ProjPrefsData, &ActProj ) )
            {
                // Change the window caption to reflect the new loaded file
                SetWindowCaption( hwnd );

                // Enable all the menu items
                EnableControls( hwnd, TRUE );

                ActProj.LiveProject = TRUE;

                // Size the view window
                BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

                // Force the view window to repaint
                ViewWnd_Repaint();
            }
        }
    }

}   // mnuNew



/************************************************************************
*    FUNCTION NAME:         mnuOpen                                                             
*
*    DESCRIPTION:           Open message handler
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

void TSS2GO::mnuOpen( HWND hwnd )
{
    char TmpProjFile[MAX_STR_PATH];

    // Disanable all the menu items
    if( !CheckToSave( hwnd ) )
        return;

    // Terminate the temp string
    TmpProjFile[0] = 0;

    // Show the windows get file box
    if( GetFileName( hwnd, AppData.hInst, TmpProjFile, OPEN_FILE_CAPTION,
                     Settings.GetProjectPath(), PROJECT_FILE_MASK, PROJECT_EXT ) )
    {
        // Resets everything
        InitProject( hwnd );

        // Now copy the filename so that we have a file to open
        strcpy( ActProj.FilePath, TmpProjFile );

        if( OpenFile( hwnd ) )
        {
            if( PageList.Count() )
            {
                // Check that we have all the files. This also updates the file paths
                // if the project has changed directories.
                if( !CheckAllFilesAreThere( hwnd ) )
                    return;

                //////////////////////
                // Load the first page
                //////////////////////
                // Load the main image file
                if( !LoadImageFile( hwnd, PageList.GetMainDataPath( 0 ), &MainDib ) )
                    return;

                // The background can be a color
                if( PageList.GetBackDataType( 0 ) > NO_FILE_TYPE && 
                    PageList.GetBackDataType( 0 ) < FILE_TYPE_META )
                {
                    // Load the background image file
                    if( !LoadImageFile( hwnd, PageList.GetBackDataPath( 0 ), &BackDib ) )
                        return;
                }
            }

            // Enable the tool bar buttons if there is more than one page
            if( PageList.Count() > 1 )
                TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, ENABLE );

            if( PageList.Count() > 0 )
                EnableSpecificControls( hwnd, TRUE );

            // Update the interface
            UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

            // Enable all the menu items
            EnableControls( hwnd, TRUE );

            // Change the window caption to reflect the new loaded file
            SetWindowCaption( hwnd );

            ActProj.OpenedFile = TRUE;

            // Size the view window
            BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

            // Force the view window to repaint
            ViewWnd_Repaint();

            // Strip the file name off of the path          
            CropOutPath( ActProj.FilePath, TmpProjFile );

            // Save the file open path for next time
            Settings.SetProjectPath( TmpProjFile );
        }
    }

}   // TSS2GO::mnuOpen


/************************************************************************
*    FUNCTION NAME:         OnShowWindow                                                             
*
*    DESCRIPTION:           Standard Windows OnShowWindowe message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              BOOL fShow      - show/hide flag
*              UINT status     - status flag
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

void TSS2GO::OnShowWindow( HWND hwnd, BOOL fShow, UINT status )
{
    if( PageList.Count() )
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );
    else
        UpdateControls( hwnd, 0, 0 );

}   // OnShowWindow


/************************************************************************
*    FUNCTION NAME:         mnuSave                                                             
*
*    DESCRIPTION:           Save message handler
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

BOOL TSS2GO::mnuSave( HWND hwnd )
{
    BOOL Result = FALSE;
    HWND ViewWindow;

    // See if they have a project exe running. If so, kill the program
    // because we can't save the EXE while the other EXE is running.
    ViewWindow = FindWindow( NULL, ProjPrefsData.Title );

    if( ViewWindow )
    {
        PostMessage( ViewWindow, WM_CLOSE, 0, 0 );
        Sleep( 500 );
    }
     
    // Get the file name
    if( !ActProj.FilePath[0] && GetFileName( hwnd, AppData.hInst, ActProj.FilePath, 
                                             SAVE_FILE_CAPTION, Settings.GetProjectPath(), 
                                             PROJECT_FILE_MASK, PROJECT_EXT, 
                                             SAVE_FILENAME_DIALOG_BOX ) )
    {
        Result = SaveToFile( hwnd, ActProj.FilePath );
    }
    else if( ActProj.FilePath[0] )
        Result = SaveToFile( hwnd, ActProj.FilePath );

    if( Result )
    {
        // No longer a live project
        ActProj.LiveProject = FALSE;

        // Now that a save has been done, it's like an opened file
        ActProj.OpenedFile = TRUE;
    }

    return Result;
    
}   // mnuSave


/************************************************************************
*    FUNCTION NAME:         mnuSaveAs                                                             
*
*    DESCRIPTION:           Save As message handler
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

BOOL TSS2GO::mnuSaveAs( HWND hwnd )
{
    BOOL Result = FALSE;
    char TmpFilePath[MAX_STR_PATH];

    // Terminate file path
    TmpFilePath[0] = 0;
 
    // Get the file name
    if( GetFileName( hwnd, AppData.hInst, TmpFilePath, 
                     SAVEAS_FILE_CAPTION, Settings.GetProjectPath(), 
                     PROJECT_FILE_MASK, PROJECT_EXT, 
                     SAVE_FILENAME_DIALOG_BOX ) )
    {
        Result = SaveToFile( hwnd, TmpFilePath );
    }

    // If we had a good build, set these flags
    if( Result )
    {
        ActProj.LiveProject = FALSE;

        // Change the window caption to reflect the new loaded file
        SetWindowCaption( hwnd );

        ActProj.OpenedFile = TRUE;
    }

    return Result;
}   // mnuSaveAs


/************************************************************************
*    FUNCTION NAME:         mnuClose                                                             
*
*    DESCRIPTION:           Close message handler
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

void TSS2GO::mnuClose( HWND hwnd )
{
    // Disanable all the menu items
    if( CheckToSave( hwnd ) )
    {
        InitProject( hwnd );

        // Set the view window
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, 0, &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();
    }

}   // mnuClose



/************************************************************************
*    FUNCTION NAME:         EnableControls                                                             
*
*    DESCRIPTION:           Enable or disable menu items
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              BOOL Enable     - Enable or disable the menu 
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

void TSS2GO::EnableControls( HWND hwnd, BOOL Enable )
{
    // Enable/disable menu items
    EnableMenuItem( GetMenu(hwnd), MNU_SAVE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_SAVEAS, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_CLOSE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_PREFS, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_MAIN_IMAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

}   // EnableControls



/************************************************************************
*    FUNCTION NAME:         EnableSpecificControls                                                             
*
*    DESCRIPTION:           Enable or disable menu items specific to 
*                           having pages loaded
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              BOOL Enable     - Enable or disable the menu 
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

void TSS2GO::EnableSpecificControls( HWND hwnd, BOOL Enable )
{
    int i, j;

    EnableMenuItem( GetMenu(hwnd), MNU_DELETE_PAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_MOVE_PAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_PRINT, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_PRINT_SETUP, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_SOUND, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_BK_IMAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_BACKGROUND_COLOR, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_CHANGE_MAIN_IMAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_RUN_SAVER, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_RUN_PREFS, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_RUN_INSTALLER, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_FIND_SCREENSAVER, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

    // Enable/disable controls
    Button_Enable( AppData.hChkSizeBMP, Enable );
    Static_Enable( AppData.hTBStatic, Enable );
    
    for( i = 0; i < MAX_BTN_GROUPS; ++i )
    {
        // Enable/disable the justification buttons
        for( j = 0; j < MAX_JUST_BTNS; ++j )
            Button_Enable( AppData.hBtnJust[j+(i*MAX_JUST_BTNS)], Enable );

        // Enable/disable the style buttons
        for( j = 0; j < MAX_STYLE_BTNS; ++j )
            Button_Enable( AppData.hFontDlg[j+(i*MAX_STYLE_BTNS)], Enable );

        // Enable/disable the edit fields
        Edit_Enable( AppData.hEditMsg[i], Enable );
    }

    // Enable the static controls
    for( i = 0; i < MAX_STATIC_LABELS; ++i )
        Static_Enable( AppData.hStatic[i], Enable );

    // Enable/disable the edit fields
    for( i = 0; i < MAX_EDT_CONTRL; ++i )
        Edit_Enable( AppData.hEditContrls[i], Enable );

    // Clear out the data on some of these controls if we are disabling them
    if( Enable == DISABLE )
    {
        // Clear out all the edit controls
        for( i = 0; i < MAX_EDT_TEXT; ++i )
            Edit_SetText( AppData.hEditMsg[i], "" );

        Edit_SetText( AppData.hEditContrls[ EDT_DELAY_IN_SEC ], "" );

        // Clear out the check box
        Button_SetCheck( AppData.hChkSizeBMP, Enable );

        // Make sure all the justification buttons are pushed up
        for( i = 0; i < MAX_JUST_BTN; ++i )
            Button_SetCheck( AppData.hBtnJust[ i ], BST_UNCHECKED );
    }

}   // EnableSpecificControls


/************************************************************************
*    FUNCTION NAME:         FlipPage                                                             
*
*    DESCRIPTION:           Flips the page left or right
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              int Direction - Flip the page left or flip the page right
*              BOOL LoadSpecificPage - Flag that indicates the page to be
*                            shown is already set in the PageList and if
*                            this flag is TRUE, the function will not
*                            inc or dec the page in this function.
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

void TSS2GO::FlipPage( HWND hwnd, int Direction, BOOL LoadSpecificPage )
{
    int CurPage;
    BOOL Result = FALSE;

    // Are we just flipping pages or loading a specific page
    if( PageList.Count() && !LoadSpecificPage )
    {
        // Flip the page
        if( Direction == PAGE_FLIP_RIGHT )
            PageList.NextPage();
        else
            PageList.PreviousPage();
    }

    // Get the current page
    CurPage = PageList.GetCurrentPage();
             
    // Load the next page
    if( PageList.Count() )
    {
        // Load the main image file
        if( !LoadImageFile( hwnd, PageList.GetMainDataPath( CurPage ), &MainDib ) )
            goto LOAD_ERROR;

        // The background can be a color
        if( PageList.GetBackDataType( CurPage ) > NO_FILE_TYPE && 
            PageList.GetBackDataType( CurPage ) < FILE_TYPE_META )
        {
            // Load the background image file
            if( !LoadImageFile( hwnd, PageList.GetBackDataPath( CurPage ), &BackDib ) )
                goto LOAD_ERROR;
        }

        // Update the controls
        UpdateControls( hwnd, CurPage, PageList.Count() );
    }
    else  // Disable the buttons if there are no pages to show
        goto LOAD_ERROR;


    // Set the view window
    BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, CurPage, &PageList );

    // Force the view window to repaint
    ViewWnd_Repaint();

    Result = TRUE;

LOAD_ERROR:;

    // If there was an error, disable the program
    if( !Result )
        ErrorDisableControls( hwnd );
            
}   // FlipPage



/************************************************************************
*    FUNCTION NAME:         ErrorDisableControls                                                             
*
*    DESCRIPTION:           Disable all the controls on an error
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

void TSS2GO::ErrorDisableControls( HWND hwnd )
{
    int i;

    // If there was an error, disable all the buttons
    for( i = 0; i < NUM_TBBUTTONS; ++i )
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT + i, DISABLE );

    // If there was an error, disable menu items
    EnableSpecificControls( hwnd, FALSE );

}   // ErrorDisableControls



/************************************************************************
*    FUNCTION NAME:         SetFlipButtons                                                             
*
*    DESCRIPTION:           Set Flip Left and Flip Right buttons
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

void TSS2GO::SetFlipButtons()
{
    if( PageList.GetCurrentPage() == PageList.Count()-1 )
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, DISABLE );
    else
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, ENABLE );

    if( PageList.GetCurrentPage() == 0 )
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT, DISABLE );
    else
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT, ENABLE );

}   // SetFlipButtons



/************************************************************************
*    FUNCTION NAME:         LoadImageFile                                                             
*
*    DESCRIPTION:           Load the bitmap file
*
*    FUNCTION PARAMETERS:
*    Input:    PTGBase pBase - Pointer to the base graphic object
*              WORD PageToLoad - The page to load 
*
*    Output:   int - The datatype of the loaded file
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TSS2GO::LoadImageFile( HWND hwnd, char *FileName, PTBitmap pBitmap )
{
    int Result = FALSE;
    int FileType;
    TGif gif;
    TJPEG jpeg;

    // Check to see that the file is still there. The could have moved it.
    if( !FileExist( FileName ) )
    {
        // Ask if they want to find the missing project file
        if( FindMissingProjectFile( hwnd, FileName, 
            Settings.GetProjectArtPath(), PAGE_FILE_TYPES ) )
            ActProj.LiveProject = TRUE;
        else
            return FALSE;
    }

    // Load the file
    FileType = GetFileType( FileName );

    switch( FileType )
    {
        case FILE_TYPE_BMP:
            Result = pBitmap->LoadFromFile( FileName );
            break;

        case FILE_TYPE_JPG:
            Result = pBitmap->LoadFromBuffer( jpeg.LoadFromFile( FileName ) );
            break;

        case FILE_TYPE_GIF:
            Result = pBitmap->LoadFromBuffer( gif.LoadFromFile( FileName ) );
            break;
    }

    // Did all go well?
    if( Result )
        Result = FileType;
    else
    {
        PostMsg( "Error", "Error loading bitmap file (%s).", FileName );
        Result = NO_FILE_TYPE;
    }

    return Result;

}   // LoadImageFile


/************************************************************************
*    FUNCTION NAME:         OnMove                                                             
*
*    DESCRIPTION:           Standard Windows message handler
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

void TSS2GO::OnMove( HWND hwnd, int x, int y )
{
    RECT rect;

    // Get the windows rect
    GetWindowRect( hwnd, &rect );

    // Make sure the window is normal when it is saved
    if( !IsZoomed( hwnd ) && !IsIconic( hwnd ) )
    {
        Settings.SetWindowX( rect.left );
        Settings.SetWindowY( rect.top );
    }

}   // OnMove


/************************************************************************
*    FUNCTION NAME:         SaveToFile                                                             
*
*    DESCRIPTION:           Save project data to the hard drive
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
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

BOOL TSS2GO::SaveToFile( HWND hwnd, char *FileName )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    HANDLE hFile;
    WORD i;
    BOOL Result = FALSE;
    char TmpProjFile[MAX_STR_PATH];

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Open the file on the hard drive
    hFile = CreateFile( FileName, GENERIC_WRITE, FILE_SHARE_WRITE,
                           &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't save file (%s) to disk.", FileName );
        goto FREE_MEMORY;
    }

    // Update the number of pages being used
    ProjPrefsData.Total_Num_Of_Pages = (WORD)PageList.Count();

    // Write the projects preferences structure to the file
    FileReadWriteResult = WriteFile( hFile, &ProjPrefsData, sizeof(TProjectPrefs), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TProjectPrefs) )
    {
        PostMsg( "Error", "Error saving file (%s) to disk.", FileName );
        goto FREE_MEMORY;
    }

    // Write the pages that have been made
    for( i = 0; i < PageList.Count(); ++i )
    {
        // Write the TPage structure to the file
        FileReadWriteResult = WriteFile( hFile, PageList.GetPage( i ), sizeof(TPage), &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != sizeof(TPage) )
        {
            PostMsg( "Error", "Error saving file (%s) to disk.", FileName );
            goto FREE_MEMORY;
        }
    }

    // Strip the file name off of the path          
    CropOutPath( FileName, TmpProjFile );

    // Save the file open path for next time
    Settings.SetProjectPath( TmpProjFile );

    // Copy the file name
    strcpy( ActProj.FilePath, FileName );

    // Build the EXE file
    if( PageList.Count() && !mnuBuild_EXE( hwnd ) )
        goto FREE_MEMORY;

    Result = TRUE;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         OpenFile                                                             
*
*    DESCRIPTION:           Open project data that was saved to the hard drive
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
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

BOOL TSS2GO::OpenFile( HWND hwnd )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    HANDLE hFile;
    BOOL FileReadWriteResult;
    BOOL Result = FALSE;
    int i;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Open the file on the hard drive
    hFile = CreateFile( ActProj.FilePath, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open file (%s) on disk.", ActProj.FilePath );
        goto FREE_MEMORY;
    }

    // Read project preferences data
    FileReadWriteResult = ReadFile( hFile, &ProjPrefsData, sizeof(TProjectPrefs), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TProjectPrefs) )
    {
        PostMsg( "Error", "Error Opening file (%s) on disk.", ActProj.FilePath );
        goto FREE_MEMORY;
    }

    // Check the preferences ID for vaild number
    if( ProjPrefsData.PrefsCheckID != PERFS_CHECK_ID )
    {
        PostMsg( "Error", "Preference ID does not match. Preference data out of sync." );
        goto FREE_MEMORY;
    }

    // Open the pages that have been saved
    for( i = 0; i < ProjPrefsData.Total_Num_Of_Pages; ++i )
    {
		// Add a blank page
		if( !PageList.Add() )
        {
            PostMsg( "Error", "Error adding page." );
            goto FREE_MEMORY;
        }

        // Write contents of file into allocated memory
        FileReadWriteResult = ReadFile( hFile, PageList.GetObj( i ), PageList.GetObjSize( i ), &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != sizeof(TPage) )
        {
            PostMsg( "Error", "Error reading in page data (%s) from disk.", ActProj.FilePath );
            goto FREE_MEMORY;
        }

        // Check the preferences ID for vaild number
        if( PageList.GetPageCheckID( i ) != PAGE_CHECK_ID )
        {
            PostMsg( "Error", "Page ID does not match. Page data out of sync." );
            goto FREE_MEMORY;
        }
    }

    Result = TRUE;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // OpenFile


/************************************************************************
*    FUNCTION NAME:         CheckToSave                                                             
*
*    DESCRIPTION:           See if we need to save
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

BOOL TSS2GO::CheckToSave( HWND hwnd )
{
    BOOL Result = TRUE;

    int MsgResult;
    // Check to see if they need to save their changes
    if( ActProj.LiveProject )
    {
        MsgResult = MessageBox( hwnd, "Your project has not been saved. Do you want to save?", 
                             PRODUCT_NAME, MB_YESNOCANCEL );

        // Did they choose to save the project?
        if( MsgResult == IDYES )
        {
            if( !mnuSave( hwnd ) )
                Result = FALSE;
        }
        else if( MsgResult == IDCANCEL )
            Result = FALSE;
    }

    return Result;

}   // CheckToSave


/************************************************************************
*    FUNCTION NAME:         AddImageToPage                                                             
*
*    DESCRIPTION:           Adds an image to the current page
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

BOOL TSS2GO::mnuAddPage( HWND hwnd, int PageType )
{
    TStringList StringList;
    BOOL Result = FALSE;
    char FileDir[MAX_PATH+1];
    char FileName[MAX_STR_PATH*2];
    char StrTemp[MAX_STR_PATH];
    int i;
    DWORD ExtraFlags=0;

    // Needs to be terminated for the common dialogs to work
    FileName[0] = 0;
    
    // Allow for multi select
    if( PageType == PAGE_TYPE_MAIN )
        ExtraFlags = OFN_ALLOWMULTISELECT; 

    if( PictLoad_GetPicture( AppData.hInst, hwnd, "Add Main Image Bitmap(s)",
        ExtraFlags, FileName, sizeof(FileName), PAGE_FILE_TYPES, Settings.GetProjectArtPath() ) )
    {
        // Save the path to the art files
        CropOutPath( FileName, Settings.GetProjectArtPath() );

        // Check for multiple selections
        // The way this works is that if this is a multiple selection, you have string after string
        // seperated with a NULL and the whole thing is terminated with two NULLs. The first string
        // is the path to the file and after that, all the file names until you hit the double NULL.
        // That is why the first string won't have a "." extension because it is just the path.
        i = strlen(FileName);
        if( FileName[i-4] != '.' )
        {
            // Save the pointer to the file path
            // This is for readability
            strcpy( FileDir, FileName );
            if( FileDir[i-1] != BACK_SLASH )
                strcat( FileDir, "\\" );

            // Jump to the next string
            ++i; 
            while( *(FileName + i) )
            {
                // Piece together the file paths
                strcpy( StrTemp, FileDir );
                strcat( StrTemp, (FileName + i) );

                // Add all the paths the the string list class
                // for easy sorting
                StringList.Add( StrTemp );

                i += strlen( (FileName + i) );
                // Jump to the next string
                ++i;    
            }

            // Sort all the strings
            StringList.Sort();

            // show the status window
            DisplayStatusWnd( AppData.hInst, hwnd );

            // Setup the status window
            StatusWnd_SetStatusBar( 0, StringList.Count(), 1 );

            // Load each string up to check each bitmap
            for( i = 0; i < StringList.Count(); ++i )
            {
                // Get the file name from the path
                CropFileNameFromPath( StringList.GetString(i), StrTemp );

                // Set the status bar text
                StatusWnd_SetStatusTxt( "Loading and Testing file (%s).", StrTemp );

                // Loads and tests each image and makes a page entry
                if( !AddImageFileToPageList( hwnd, StringList.GetString(i), &MainDib, PageType ) )
                    goto LOAD_ERROR;

                StatusWnd_MoveStatusBar();
            }
        }
        else // Single image load
        {
            if( !AddImageFileToPageList( hwnd, FileName, &MainDib, PageType ) )
                goto LOAD_ERROR;
        }

        // Size the view window
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();
        
        if( PageList.Count() > 0 )
            EnableSpecificControls( hwnd, TRUE );

        // Set the project live status
        ActProj.LiveProject = TRUE;

        // Update the static showing the number of pages and the current page
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

        // Set the flip button state
        SetFlipButtons();
    }

    Result = TRUE;
    
LOAD_ERROR:;

    StatusWnd_HideStatusWnd();

    return Result;

}   // mnuAddPage



/************************************************************************
*    FUNCTION NAME:         mnuAddBackImage                                                             
*
*    DESCRIPTION:           Adds an image to the current page
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

BOOL TSS2GO::mnuAddBackImage( HWND hwnd )
{
    BOOL Result = FALSE;
    char FileName[MAX_STR_PATH*2];

    // Needs to be terminated for the common dialogs to work
    FileName[0] = 0;

    if( PictLoad_GetPicture( AppData.hInst, hwnd, "Add Background Image Bitmap",
        0, FileName, sizeof(FileName), PAGE_FILE_TYPES, Settings.GetProjectBKArtPath() ) )
    {
        // Save the path to the art files
        CropOutPath( FileName, Settings.GetProjectBKArtPath() );

        // Single image load
        if( !AddImageFileToPageList( hwnd, FileName, &BackDib, PAGE_TYPE_BACK ) )
            goto LOAD_ERROR;

        // Size the view window
        BldBuffer_ResetBuffer( &buffer, &MainDib, &BackDib, PageList.GetCurrentPage(), &PageList );

        // Force the view window to repaint
        ViewWnd_Repaint();

        // Set the project live status
        ActProj.LiveProject = TRUE;

        // Update the static showing the number of pages and the current page
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

        // Set the flip button state
        SetFlipButtons();
    }

    Result = TRUE;
    
LOAD_ERROR:;

    return Result;

}   // mnuAddBackImage



/************************************************************************
*    FUNCTION NAME:         AddMainFileToPageList                                                             
*
*    DESCRIPTION:           Takes the file path and sets up the page list object
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

BOOL TSS2GO::AddImageFileToPageList( HWND hwnd, char *FileName, PTBitmap Image, int PageType )
{
    WORD FileType;
    BOOL Result = FALSE;

    // Get the file type - bmp, jpg, gif, and so on
    FileType = LoadImageFile( hwnd, FileName, Image );

    // Did the file load without an error? If so, make a 
    // new page and add it to the page list
    if( FileType )
    {
        // Add a new blank page to the list
        if( PageType == PAGE_TYPE_MAIN )
        {
            // Add a new page
            PageList.Add();

            // Set the current page to the last page
            PageList.SetCurrentPage( (WORD)PageList.Count()-1 );

            // Set the default font for every new page added
            memcpy( PageList.GetPtrToTopLogfont( PageList.GetCurrentPage() ), Settings.GetLogFont(), sizeof( LOGFONT ) );
            memcpy( PageList.GetPtrToBotLogfont( PageList.GetCurrentPage() ), Settings.GetLogFont(), sizeof( LOGFONT ) );

            // Set the default page settings
            PageList.SetTopTxtJust( PageList.GetCurrentPage(), Settings.GetDefTxtJust() );
            PageList.SetBotTxtJust( PageList.GetCurrentPage(), Settings.GetDefTxtJust() );
            PageList.SetTopFontColor( PageList.GetCurrentPage(), Settings.GetDefTxtColor() );
            PageList.SetBotFontColor( PageList.GetCurrentPage(), Settings.GetDefTxtColor() );
            PageList.SetDelayInSec( PageList.GetCurrentPage(), Settings.GetDelayInSec() );
        }

        if( PageType == PAGE_TYPE_MAIN || PageType == PAGE_TYPE_RELOAD_MAIN )
        {
            // Save the file path to the main image
            strcpy( PageList.GetMainDataPath( PageList.GetCurrentPage() ), FileName );

            // Save the file type so that we always know what type of file we are using
            PageList.SetMainDataType( PageList.GetCurrentPage(), FileType );
        }
        else if( PageType == PAGE_TYPE_BACK )
        {
            // Save the file path to the main image
            strcpy( PageList.GetBackDataPath( PageList.GetCurrentPage() ), FileName );

            // Save the file type so that we always know what type of file we are using
            PageList.SetBackDataType( PageList.GetCurrentPage(), FileType );
        }

        // Set the result
        Result = TRUE;
    }

    return Result;

}   // AddMainFileToPageList


/************************************************************************
*    FUNCTION NAME:         mnuBuild_EXE                                                             
*
*    DESCRIPTION:           Builds the final executible
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

BOOL TSS2GO::mnuBuild_EXE( HWND hwnd )
{
    unsigned long bytesReadWrite;
    HANDLE hScrFile = INVALID_HANDLE_VALUE;
    HANDLE hInstFile = INVALID_HANDLE_VALUE;
    BOOL Result = FALSE;
    BOOL FileReadWriteResult;
    char PathToSSBuild[MAX_PATH+1];
    char PathToInstallBuild[MAX_PATH+1];
    char TempDir[MAX_PATH+1];
    DWORD FileSize, CheckSum;
    BYTE QuadAlign[4] = {0,0,0,0};
    int i;
    DWORD *CheckSumFileHeader;
    // This varaible is incremented and marks the starting
    // offset the image files are added to the exe file
    DWORD FileDataOffset;
    double FileSizeDb, OneMeg;
    char FileSizStr[20];

    if( PageList.Count() )
    {
        // Check that all the project files are there
        if( !CheckAllFilesAreThere( hwnd ) )
            goto FREE_MEMORY;

        // show the status window
        DisplayStatusWnd( AppData.hInst, hwnd );

        // Setup the status window
        StatusWnd_SetStatusBar( 1, PageList.Count()+1, 1 );

        // Set the status bar text
        StatusWnd_SetStatusTxt( "Building Program File..." );

        // Get the temporary directory that is usually in the windows
        // directory. Build the screen saver EXE there so that there isn't
        // two exe's in the project folder to confuse the user.
        //GetTheWindowsTmpDir( TempDir );

        // Update the number of pages being used in this project
        ProjPrefsData.Total_Num_Of_Pages = PageList.Count();

        // Put together the path to the new exe file        
        AddFileToOtherPath( ProjPrefsData.ExeName, "C:\\", PathToSSBuild );

        // Write the screen saver from the resource to the hard drive
        // Init the FileDataOffset variable with the size of the exe file.
        // Because that is the starting point for all the project's data
        hScrFile = WriteEXEtoHDfromRes( "ScreenSaver", PathToSSBuild, &FileDataOffset );
        if( hScrFile == INVALID_HANDLE_VALUE )
            goto FREE_MEMORY;

        // Update the FileHeader.ProjectDataOffset to the size of the exe
        // file because that is the starting point for all the project's data
        FileHeader.ProjectDataOffset = FileDataOffset;  

        // Save the project preferences data to the exe file and incements the offset
        // This may also include the startup screen bitmap and info text blocks and anything else
        FileDataOffset = SaveProjectDataToFile( hScrFile, FileDataOffset );
        if( FileDataOffset == 0 )
        {
            PostMsg( "Error", "Error building file project data. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Write the page data to the file
        FileDataOffset = SavePageDataToFile( hScrFile, FileDataOffset );
        if( FileDataOffset == 0 )
        {
            PostMsg( "Error", "Error building file page data. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Write all the page files to the exe
        if( !SavePageFilesToEXE( hScrFile ) )
        {
            PostMsg( "Error", "Error building file with page files. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Get the file size for quad alignment check
        FileSize = GetFileSize( hScrFile, NULL );

        // Quad align file before adding header
        if( FileSize % sizeof( DWORD ) != 0 )
        {
            FileReadWriteResult = WriteFile( hScrFile, QuadAlign, sizeof(DWORD)-(FileSize % sizeof( DWORD )), &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(DWORD)-(FileSize % sizeof( DWORD )) )
            {
                PostMsg( "Error", "Error building file alignment. (%s)", ProjPrefsData.ExeName );
                goto FREE_MEMORY;
            }
        }

        // Get the file size again for the file header to save the full file size
        FileSize = GetFileSize( hScrFile, NULL );

        // Checksum the file
        CheckSum = CheckSumFile( hScrFile );

        // Make sure we didn't get a checksum error
        if( CheckSum == CHECK_SUM_ERROR )
        {
            PostMsg( "Error", "Error checksuming file. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // The check sum sets the file pointer to the begining of the file
        // so we need to set it back to the end of the file before writing the file header
        SetFilePointer( hScrFile, 0, 0, FILE_END );

        // Set the checksum result to zero because it could be
        // holding the value from the previous build
        FileHeader.CheckSumResult = 0;

        // Save the file size of the completed file
        FileHeader.FileSize = FileSize + sizeof(TFileHeader);

        // Checksum the file header
        CheckSumFileHeader = (DWORD *)&FileHeader;
        for( i = 0; i < (sizeof(TFileHeader) / sizeof(DWORD)); ++i )
            CheckSum += CheckSumFileHeader[i];

        // Save the difference to the checksum
        FileHeader.CheckSumResult = 0x100000000 - CheckSum;

        // Write the projects file header structure to the file
        FileReadWriteResult = WriteFile( hScrFile, &FileHeader, sizeof(TFileHeader), &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != sizeof(TFileHeader) )
        {
            PostMsg( "Error", "Error building file with file header data. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Get the final file size so they know what the size of their EXE is
        FileSize = GetFileSize( hScrFile, NULL );

        // Double check that our file sizes match.
        if( FileSize != FileHeader.FileSize )
        {
            PostMsg( "Error", "Error building file with file header data. File size miss match. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // One lase check to see if the file is quad aligned
        if( FileSize % sizeof( DWORD ) == 0 )
        {
            // Format the size string depending on the size of the file
            if( FileSize < 1024000 ) // Show in K size
            {
                sprintf( FileSizStr, "%uK", FileSize / 1024 );
            }
            else    // Show in MB size
            {
                OneMeg = 1024000;
                FileSizeDb = FileSize / OneMeg;
                sprintf( FileSizStr, "%.1lfMB", FileSizeDb );
            }

            // Let them know what that file size is
            //PostMsg( "Done", "Your screen saver (%s) has been made. The file size of your screen saver is %s.", 
                     //ProjPrefsData.ExeName, FileSizStr );
        }
        else
        {
            PostMsg( "Error", "Error building file. File size out of alignment (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Put together the path to the new installer exe file
        AddFileToOtherPath( ProjPrefsData.ExeName, ActProj.FilePath, PathToInstallBuild );

        // Write the installer from the resource to the hard drive
        hInstFile = WriteEXEtoHDfromRes( "Installer", PathToInstallBuild );
        if( hInstFile == INVALID_HANDLE_VALUE )
            goto FREE_MEMORY;

        // Write the screen saver to the back of the installer
        if( !WriteScrToInstaller( hScrFile, hInstFile ) )
            goto FREE_MEMORY;

        Result = TRUE;

    FREE_MEMORY:;

        // Close the file handles
        if( hScrFile != INVALID_HANDLE_VALUE )
            CloseHandle( hScrFile );

        if( hInstFile != INVALID_HANDLE_VALUE )
            CloseHandle( hInstFile );

        if( !Result && PathToSSBuild[0] )
            DeleteFile( PathToSSBuild );
        
        StatusWnd_HideStatusWnd();

    }   // if( PageList.Count() )
    else
    {
        PostMsg( "Opps", "Need to add a page before you can do a build." );
    }

    return Result;

}   // mnuBuild_EXE


/************************************************************************
*    FUNCTION NAME:         SaveProjectDataToFile                                                             
*
*    DESCRIPTION:           Save the project info to the new EXE file.
*                           The project data is encrypted.
*                           The startup screen and the info text for
*                           the startup info screen and the order form
*                           screen offsets are calculated.
*
*    FUNCTION PARAMETERS:
*    Input:    HANDLE hFile - Handle to opened file the data is written to
*              DWORD StartingOffset - Number indicating the starting position
*                                     to init this functions internal counter 
*
*    Output:   DWORD - New value of FileDataOffset counter
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

DWORD TSS2GO::SaveProjectDataToFile( HANDLE hFile, DWORD StartingOffset )
{
    DWORD PrefsStructSize, FileDataOffset;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;

    // The projects preferences structure size minus the file paths and the 
    // future data array because the view program doesn't use or need them
    PrefsStructSize = sizeof(TProjectPrefs)-sizeof( TProjectPrefsSmallSize );

    // Init the data offset to the point where the first data file can be found at
    FileDataOffset = PrefsStructSize + StartingOffset;

    // Set the offset to the starting position of the TPage Data which is after the
    // TProjectPrefs data and any graphics needed by the TProjectPrefs
    ProjPrefsData.PageDataOffset = FileDataOffset;

    // Write the projects preferences structure to the file
    FileReadWriteResult = WriteFile( hFile, &ProjPrefsData, PrefsStructSize, &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != PrefsStructSize )
    {
        PostMsg( "Error", "Error building file with prefs data. (%s)", ProjPrefsData.ExeName );
        FileDataOffset = 0;
    }

    return FileDataOffset;

}   // SaveProjectDataToFile



/************************************************************************
*    FUNCTION NAME:         SavePageDataToFile                                                             
*
*    DESCRIPTION:           Save the page info to the new EXE file
*
*    FUNCTION PARAMETERS:
*    Input:    HANDLE hFile - Handle to opened file the data is written to
*              DWORD StartingOffset - Number indicating the starting position
*                                     to init this functions internal counter 
*
*    Output:   DWORD - New value of FileDataOffset counter
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

DWORD TSS2GO::SavePageDataToFile( HANDLE hFile, DWORD StartingOffset )
{
    int i, j;
    DWORD FileDataOffset, MainFileSize=0, SoundFileSize=0, BackFileSize=0;
    BOOL FileReadWriteResult;
    unsigned long bytesReadWrite;

    // The TPageView structure is the same as the TPage structure minus
    // all the tool program varaibles. The TPageView structure is never
    // used for anything other than calculating the size of the TPage
    // structure minus the tool program varaibles.

    // Set the starting point for the page data
    FileDataOffset = StartingOffset + (PageList.Count() * sizeof(TPageView));

    // Init all the dup data to zero because we check the value of this
    // before setting it. If it is already set, that means it is duplicate data.
    for( i = 0; i < PageList.Count(); ++i )
    {
        PageList.SetMainDataDuplicate( i, FALSE );
        PageList.SetBackDataDuplicate( i, FALSE );
        PageList.SetSoundDataDuplicate( i, FALSE );
    }

    // Write the pages that have been made
    for( i = 0; i < PageList.Count(); ++i )
    {

        /////////////////////////////////////
        //  Check for dup Main image files
        /////////////////////////////////////

        // Check for duplicates
        if( !PageList.GetMainDataDuplicate( i ) )
        {
            // Record the image data offset
            PageList.SetMainDataOffset( i, FileDataOffset );

            for( j = i+1; j < PageList.Count(); ++j )
            {
                // Check for page image duplicate
                if( !PageList.GetMainDataDuplicate( j ) )
                {
                    // if it's the same path, then it is a duplicate
                    if( strcmp( PageList.GetMainDataPath( i ), 
                        PageList.GetMainDataPath( j ) ) == 0 )
                    {
                        // Set the duplicate flag
                        PageList.SetMainDataDuplicate( j, TRUE );
                        
                        // Record the image data offset
                        PageList.SetMainDataOffset( j, FileDataOffset );   
                    }
                }
            }
        }

        // Add up the FileDataOffset if it is not a duplicate
        if( !PageList.GetMainDataDuplicate( i ) )
        {
            // Update the file size. Since this is the uncompressed file, we need to include the
            // file header that's created on the fly.
            MainFileSize = GetTheFileSize( PageList.GetMainDataPath( i ) ) + sizeof(TDataHeader);
        }


        /////////////////////////////////////
        //  Check for dup Back image files
        /////////////////////////////////////

        // Add up the FileDataOffset if it is not a duplicate and a file has been used
        if( !PageList.GetBackDataDuplicate( i ) && PageList.GetBackDataType( i ) > NO_FILE_TYPE && 
            PageList.GetBackDataType( i ) < FILE_TYPE_META )
        {
            // Record the image data offset
            PageList.SetBackDataOffset( i, FileDataOffset + MainFileSize );

            for( j = i+1; j < PageList.Count(); ++j )
            {
                // Check for page image duplicate
                if( !PageList.GetBackDataDuplicate( j ) )
                {
                    // if it's the same path, then it is a duplicate
                    if( strcmp( PageList.GetBackDataPath( i ), 
                        PageList.GetBackDataPath( j ) ) == 0 )
                    {
                        // Set the duplicate flag
                        PageList.SetBackDataDuplicate( j, TRUE );
                        
                        // Record the image data offset
                        PageList.SetBackDataOffset( j, FileDataOffset + MainFileSize );   
                    }
                }
            }

            // Update the file size. Since this is the uncompressed file, we need to include the
            // file header that's created on the fly.
            BackFileSize = GetTheFileSize( PageList.GetBackDataPath( i ) ) + sizeof(TDataHeader);
        }


        /////////////////////////////////////
        //  Check for dup Sound files
        /////////////////////////////////////

        // Add up the FileDataOffset if it is not a duplicate and a file has been used
        if( PageList.GetSoundDataType( i ) && !PageList.GetSoundDataDuplicate( i ) )
        {
            // Record the sound data offset
            PageList.SetSoundDataOffset( i, FileDataOffset + MainFileSize + BackFileSize );

            for( j = i+1; j < PageList.Count(); ++j )
            {
                // Check for page sound duplicate
                if( !PageList.GetSoundDataDuplicate( j ) )
                {
                    // if it's the same path, then it is a duplicate
                    if( strcmp( PageList.GetSoundDataPath( i ), 
                        PageList.GetSoundDataPath( j ) ) == 0 )
                    {
                        // Set the duplicate flag
                        PageList.SetSoundDataDuplicate( j, TRUE );
                        
                        // Record the image data offset
                        PageList.SetSoundDataOffset( j, FileDataOffset + MainFileSize + BackFileSize );   
                    }
                }
            }

            // Update the file size. Since this is the uncompressed file, we need to include the
            // file header that's created on the fly.
            SoundFileSize = GetTheFileSize( PageList.GetSoundDataPath( i ) ) + sizeof(TDataHeader);
        }

        // Write the TPage structure to the file. We are not writing the file path to the file
        FileReadWriteResult = WriteFile( hFile, PageList.GetPage( i ), sizeof(TPageView), &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != sizeof(TPageView) )
        {
            PostMsg( "Error", "Error building file with page data. (%s)", ProjPrefsData.ExeName );
            FileDataOffset = 0;
            break;
        }

        // Update the offset
        FileDataOffset += MainFileSize + BackFileSize + SoundFileSize;
        MainFileSize = 0;
        SoundFileSize = 0;
        BackFileSize = 0;
    }

    return FileDataOffset;

}   // SavePageDataToFile


/************************************************************************
*    FUNCTION NAME:         SavePageFilesToEXE                                                             
*
*    DESCRIPTION:           Save the page files to the exe file which can
*                           consist of bitmaps, sound files and avis.
*
*    FUNCTION PARAMETERS:
*    Input:    HANDLE hFile - Handle to opened file the data is written to
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

BOOL TSS2GO::SavePageFilesToEXE( HANDLE hFile )
{
    int i;
    BOOL Result = FALSE;
    BYTE *pFileData;
    DWORD BufferSize;
    TDataHeader DataHeader;
    BOOL FileReadWriteResult;
    unsigned long bytesReadWrite;

    // Init the buffer for making uncompressed file headers
    memset( &DataHeader, 0, sizeof(TDataHeader) );

    BufferSize = 1000000;
    pFileData = new BYTE[BufferSize];
    if( !pFileData )
    {
        PostMsg( "Error", "Memory allocation error during page file build." );
        goto FREE_MEMORY;
    }

    for( i = 0; i < PageList.Count(); ++i )
    {
        // Tell them what page we are building
        StatusWnd_SetStatusTxt( "Building Screen (%u).", i );


        //////////////////////////////
        //  Save the Main Image File
        //////////////////////////////

        // Check if this is a duplicate, if so, don't write it
        if( !PageList.GetMainDataDuplicate( i ) )
        {
            // Set the header. Since this is uncompressed, the CompSize is 0 
            DataHeader.Type = PageList.GetMainDataType( i );
            DataHeader.CompSize = 0;
            DataHeader.UncompSize = GetTheFileSize( PageList.GetMainDataPath( i ) );
            if( DataHeader.UncompSize == 0 )
            {
                PostMsg( "Error", "Error getting file size for file. (%s)", PageList.GetMainDataPath( i ) );
                goto FREE_MEMORY;
            }

            // Write the projects preferences structure to the file
            FileReadWriteResult = WriteFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
            {
                PostMsg( "Error", "Error building file with uncompressed header data. (%s)", PageList.GetMainDataPath( i ) );
                goto FREE_MEMORY;
            }

            if( !CopyFileToDest( PageList.GetMainDataPath( i ), hFile, pFileData, BufferSize ) )           
                    goto FREE_MEMORY;
        }


        //////////////////////////////
        //  Save the Background File
        //////////////////////////////

        // See if we added a background file
        if( !PageList.GetBackDataDuplicate( i ) && PageList.GetBackDataType( i ) > NO_FILE_TYPE && 
            PageList.GetBackDataType( i ) < FILE_TYPE_META )
        {
            // Set the header. Since this is uncompressed, the CompSize is 0 
            DataHeader.Type = PageList.GetBackDataType( i );
            DataHeader.CompSize = 0;
            DataHeader.UncompSize = GetTheFileSize( PageList.GetBackDataPath( i ) );
            if( DataHeader.UncompSize == 0 )
            {
                PostMsg( "Error", "Error getting file size for file. (%s)", PageList.GetBackDataPath( i ) );
                goto FREE_MEMORY;
            }

            // Write the projects preferences structure to the file
            FileReadWriteResult = WriteFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
            {
                PostMsg( "Error", "Error building file with uncompressed header data. (%s)", PageList.GetBackDataPath( i ) );
                goto FREE_MEMORY;
            }

            if( !CopyFileToDest( PageList.GetBackDataPath( i ), hFile, pFileData, BufferSize ) )           
                goto FREE_MEMORY;
        }


        //////////////////////////////
        //  Save the Sound File
        //////////////////////////////

        // See if we added a sound file
        if( !PageList.GetSoundDataDuplicate( i ) && PageList.GetSoundDataType( i ) )
        {
            // Set the header. Since this is uncompressed, the CompSize is 0 
            DataHeader.Type = PageList.GetSoundDataType( i );
            DataHeader.CompSize = 0;
            DataHeader.UncompSize = GetTheFileSize( PageList.GetSoundDataPath( i ) );
            if( DataHeader.UncompSize == 0 )
            {
                PostMsg( "Error", "Error getting file size for file. (%s)", PageList.GetSoundDataPath( i ) );
                goto FREE_MEMORY;
            }

            // Write the projects preferences structure to the file
            FileReadWriteResult = WriteFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
            {
                PostMsg( "Error", "Error building file with uncompressed header data. (%s)", PageList.GetSoundDataPath( i ) );
                goto FREE_MEMORY;
            }

            if( !CopyFileToDest( PageList.GetSoundDataPath( i ), hFile, pFileData, BufferSize ) )           
                goto FREE_MEMORY;
        }

        // Move the status window
        StatusWnd_MoveStatusBar();
    }


    Result = TRUE;


    FREE_MEMORY:;

    // delete the buffer
    if( pFileData )
        delete [] pFileData;

    return Result;

}   // SavePageFilesToEXE


/************************************************************************
*    FUNCTION NAME:         WriteEXEtoHdfromRes                                                             
*
*    DESCRIPTION:           Write the runtime viewer to the hard drive
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

HANDLE TSS2GO::WriteEXEtoHDfromRes( char *ResID, char *FilePath, DWORD *FileSize )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    HANDLE hFile=INVALID_HANDLE_VALUE;
    TLoadRes LoadRes;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    if( !LoadTheResource( AppData.hInst, ResID, "EXE", &LoadRes ) )
    {
        PostMsg( "Error", "Error loading resource to build file. (%s)", ResID );
        goto HANDLE_ERROR;
    }

    // Get file size of the resource which will be the starting offset into the data
    if( FileSize )
        *FileSize = LoadRes.Size;

    // Open the file on the hard drive
    hFile = CreateFile( FilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                           &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't create file.", ResID );
        goto HANDLE_ERROR;
    }

    // Write exe to the hardrive
    FileReadWriteResult = WriteFile( hFile, LoadRes.pData, LoadRes.Size, &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != LoadRes.Size )
    {
        PostMsg( "Error", "Can't write file.", ResID );
        goto HANDLE_ERROR;
    }

HANDLE_ERROR:;

    return hFile;

}   // WriteEXEtoHdfromRes


/************************************************************************
*    FUNCTION NAME:         WriteScrToInstaller                                                             
*
*    DESCRIPTION:           Write the screen saver to the back of the installer
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

BOOL TSS2GO::WriteScrToInstaller( HANDLE hScr, HANDLE hInst )
{
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    BOOL Result = FALSE;
    DWORD FileReadSize;
    DWORD DataReadSize = 0;
    DWORD BufferSize;
    BYTE *pBuffer;

    // Set the file pointer to the end of the installer file
    SetFilePointer( hInst, 0, 0, FILE_END );

    // Set the file pointer to the beginning of the screen saver file
    SetFilePointer( hScr, 0, 0, FILE_BEGIN   );

    BufferSize = 524288; // half meg
    pBuffer = new BYTE[BufferSize];
    if( !pBuffer )
    {
        MessageBox( GetActiveWindow(), "Memory allocation error during build.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }
       
    // Setup the amount of memory will be using to copy over the file
    if( FileHeader.FileSize > BufferSize )
        FileReadSize = BufferSize;
    else
        FileReadSize = FileHeader.FileSize;

    // If the file is bigger than the buffer, then keep writting
    // chuncks until the file is finished
    do
    {
        // Read the amount specified into the buffer
        FileReadWriteResult = ReadFile( hScr, pBuffer, FileReadSize, &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != FileReadSize )
        {
            MessageBox( GetActiveWindow(), "Error reading file for install build.", "Error", MB_OK );
            goto HANDLE_ERROR;
        }

        // Write the amount from the buffer to the destination file
        FileReadWriteResult = WriteFile( hInst, pBuffer, FileReadSize, &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != FileReadSize )
        {
            MessageBox( GetActiveWindow(), "Error writing install file build.", "Error", MB_OK );
            goto HANDLE_ERROR;
        }
            
        // Update the data read size to the amount read
        DataReadSize += FileReadSize;

        // Update how much we need to read in for that last piece of memory
        // that is less then the file read size otherwise keep reading in the
        // same chunk size.
        if( DataReadSize < FileHeader.FileSize && DataReadSize + FileReadSize > FileHeader.FileSize )
            FileReadSize = FileHeader.FileSize - DataReadSize;
    }
    while( DataReadSize < FileHeader.FileSize );

    Result = TRUE;


HANDLE_ERROR:;

    // Close the file handle
    if( pBuffer )
        delete [] pBuffer;

    return Result;

}   // WriteScrToInstaller


/************************************************************************
*    FUNCTION NAME:         UpdateControls                                                             
*
*    DESCRIPTION:           Updates the controls to reflect the  
*                           changing conditions from page to page
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

void TSS2GO::UpdateControls( HWND hwnd, int CurrentPage, int TotalPages )
{
   char StaticStr[20], StrTemp[MAX_PATH], FileName[MAX_PATH];
    int i;

    // Set the total number of pages
    sprintf( StaticStr, "of %u", TotalPages );
    Static_SetText( AppData.hTBStatic,  StaticStr );

    // Set the current page selected
    if( TotalPages )
        sprintf( StaticStr, "%u", CurrentPage+1 );
    else
        sprintf( StaticStr, "%u", CurrentPage );

    Edit_SetText( AppData.hEditContrls[ EDT_CUR_PAGE ], StaticStr );

        // Set the size to fit check box
        Button_SetCheck( AppData.hChkSizeBMP, PageList.GetSizeToFit( CurrentPage ) );

    if( TotalPages )
    {
        if( CurrentPage == TotalPages-1 )
            TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, DISABLE );
        else
            TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, ENABLE );

        if( CurrentPage == 0 )
            TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT, DISABLE );
        else
            TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT, ENABLE );

        // See if there is a sound associated with this page
        if( PageList.GetSoundDataType( CurrentPage ) )
        {
            for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
                TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PLAY+i, ENABLE );
        }
        else
        {
            if( !mediaPlayer.GetPlayError() && mediaPlayer.GetStatus() != PLAYER_PLAYING )
            {
                for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
                    TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PLAY+i, DISABLE );
            }
            // If they go to a new page and the media player is playing, don't disable the stop button
            else
            {
                TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PLAY, DISABLE );
                TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PAUSE, DISABLE );
                TBBtn_SetState( AppData.hToolBar, BTN_SOUND_STOP, ENABLE );
            }
        }

        // Enable or disable Remove sound menu options
        if( PageList.GetSoundDataType( CurrentPage ) )
            EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_ENABLED );
        else
            EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_GRAYED|MF_DISABLED );

        // Set the text for the two edit controls
        Edit_SetText( AppData.hEditMsg[ EDT_TEXT_TOP_MSG ], PageList.GetPtrToTopCaption( CurrentPage ) );
        Edit_SetText( AppData.hEditMsg[ EDT_TEXT_BOTTOM_MSG ], PageList.GetPtrToBotCaption( CurrentPage ) );

        // Set the state of the top justification buttons
        if( PageList.GetTopTxtJust( CurrentPage ) == LEFT_JUSTIFY )
        {
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_LEFT ], BST_CHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_CENT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_RIGH ], BST_UNCHECKED );
        }
        else if( PageList.GetTopTxtJust( CurrentPage ) == CENTER_JUSTIFY )
        {
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_LEFT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_CENT ], BST_CHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_RIGH ], BST_UNCHECKED );
        }
        else if( PageList.GetTopTxtJust( CurrentPage ) == RIGHT_JUSTIFY )
        {
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_LEFT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_CENT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_TOP_RIGH ], BST_CHECKED );
        }

        // Set the state of the bottom justification buttons
        if( PageList.GetBotTxtJust( CurrentPage ) == LEFT_JUSTIFY )
        {
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_LEFT ], BST_CHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_CENT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_RIGH ], BST_UNCHECKED );
        }
        else if( PageList.GetBotTxtJust( CurrentPage ) == CENTER_JUSTIFY )
        {
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_LEFT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_CENT ], BST_CHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_RIGH ], BST_UNCHECKED );
        }
        else if( PageList.GetBotTxtJust( CurrentPage ) == RIGHT_JUSTIFY )
        {
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_LEFT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_CENT ], BST_UNCHECKED );
            Button_SetCheck( AppData.hBtnJust[ BTN_JUST_BOT_RIGH ], BST_CHECKED );
        }

        // Set the delay in seconds edit control
        sprintf( StaticStr, "%u", PageList.GetDelayInSec( CurrentPage ) );
        Edit_SetText( AppData.hEditContrls[ EDT_DELAY_IN_SEC ], StaticStr );

        // Give the name of the image files being used
        CropFileNameFromPath( PageList.GetMainDataPath( CurrentPage ), FileName );
        sprintf( StrTemp, " Main Image:  %s", FileName );
        SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)0, (LPARAM)StrTemp );

        // Is there an image selected for the background?
        if( PageList.GetBackDataType( CurrentPage ) > NO_FILE_TYPE && 
            PageList.GetBackDataType( CurrentPage ) < FILE_TYPE_META )
        {
            CropFileNameFromPath( PageList.GetBackDataPath( CurrentPage ), FileName );
            sprintf( StrTemp, " Back Image:  %s", FileName );
            SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)1, (LPARAM)StrTemp );
        }
        else if( PageList.GetBackDataType( CurrentPage ) == FILE_TYPE_COLOR )
        {
            sprintf( StrTemp, " Red: %u,  Green: %u,  Blue: %u", 
            GetRValue( PageList.GetBackColor( CurrentPage ) ),
            GetGValue( PageList.GetBackColor( CurrentPage ) ),
            GetBValue( PageList.GetBackColor( CurrentPage ) )  );
            SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)1, (LPARAM)StrTemp );
        }
        else
            SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)1, (LPARAM)"" );

        // Is there a sound selected for this screen
        if( PageList.GetSoundDataType( CurrentPage ) )
        {
            CropFileNameFromPath( PageList.GetSoundDataPath( CurrentPage ), FileName );
            sprintf( StrTemp, " Sound File:  %s", FileName );
            SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)2, (LPARAM)StrTemp );
        }
        else
            SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)2, (LPARAM)"" );
    }
    else
    {
        // Clear out the status bar panels
        SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)0, (LPARAM)"" );
        SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)1, (LPARAM)"" );
        SendMessage( AppData.hStatusWnd, SB_SETTEXT, (WPARAM)2, (LPARAM)"" );
    }

}   // UpdateControls



/************************************************************************
*    FUNCTION NAME:         CheckThatAllFilesAreThere                                                             
*
*    DESCRIPTION:           Check that we have all our files. This also 
*                           updates the file paths if the project has 
*                           changed directories.
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

BOOL TSS2GO::CheckAllFilesAreThere( HWND hwnd )
{
    int i;
    char StrTemp[MAX_STR_PATH];
    BOOL Result = FALSE;

    // Go through all the pages and check that all the files are there.
    // If the file is not located in it's original path, check to see if it is in the project directory.
    // If the file is in the project directory, save the new file paths. 
    for( i = 0; i < PageList.Count(); ++i )
    {
        // Check for the main image file
        // If the file is not there, see if it is located in the same directory the project file is in
        if( !FileExist( PageList.GetMainDataPath( i ) ) )
        {
            // If we found the file in the project directory, update the path to reflect the change
            if( FileExist( PageList.GetMainDataPath( i ), ActProj.FilePath ) )
            {
                // Combine the project path and the file name because the information has changed
                AddFileToOtherPath( PageList.GetMainDataPath( i ), ActProj.FilePath, StrTemp );
                strcpy( PageList.GetMainDataPath( i ), StrTemp );

                ActProj.LiveProject = TRUE;
            }
            else
            {
                // Ask if they want to find the missing project file
                if( FindMissingProjectFile( hwnd, PageList.GetMainDataPath( i ), 
                    Settings.GetProjectArtPath(), PAGE_FILE_TYPES ) )
                    ActProj.LiveProject = TRUE;
                else
                    goto ERROR_JUMP;
            }
        }

        // Check for the sound file. Make sure we are using one first.
        // If the file is not there, see if it is located in the same directory the project file is in
        if( !FileExist( PageList.GetSoundDataPath( i ) ) )
        {
            // If we found the file in the project directory, update the path to reflect the change
            if( FileExist( PageList.GetSoundDataPath( i ), ActProj.FilePath ) )
            {
                // Combine the project path and the file name because the information has changed
                AddFileToOtherPath( PageList.GetSoundDataPath( i ), ActProj.FilePath, StrTemp );
                strcpy( PageList.GetSoundDataPath( i ), StrTemp );

                ActProj.LiveProject = TRUE;
            }
            else
            {
                // Ask if they want to find the missing project file
                if( FindMissingProjectFile( hwnd, PageList.GetSoundDataPath( i ), 
                    Settings.GetProjectSoundPath(), SOUND_FILE_TYPES ) )
                    ActProj.LiveProject = TRUE;
                else
                    goto ERROR_JUMP;
            }
        }
    }

    Result = TRUE;

ERROR_JUMP:;

    return Result;

}   // CheckAllFilesAreThere


/************************************************************************
*    FUNCTION NAME:         FindMissingProjectFile                                                             
*
*    DESCRIPTION:           Check that we have all our files. This also 
*                           updates the file paths if the project has 
*                           changed directories.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd - Handle to window
*              char *MissingFile - Path of the missing file
*              char *StartSearchPath - Def search directory
*              char *FileMask - File masks
*              char *ExtMask - extension mask 
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

BOOL TSS2GO::FindMissingProjectFile( HWND hwnd, char *MissingFile, char *StartSearchPath, 
                                   char *FileMask )
{
    char FoundFilePath[MAX_STR_PATH];
    char SearchFile[100], FoundFile[100];
    char DialogBoxCaption[150];
    BOOL Result = FALSE;

    // First ask if they want to search for the missing file
    if( AskQuestion( MB_YESNO, "Error", "Unable to find project file. (%s) Do you want to try to find it?", MissingFile ) ==
        IDYES )
    {
        // Crop out the file name we are looking for
        CropFileNameFromPath( MissingFile, SearchFile );

        // Put together a caption that has the file name in it
        sprintf( DialogBoxCaption, "Find Missing Project File - %s", SearchFile );

        // Dialog boxes don't like unterminated strings
        FoundFilePath[0] = 0;

        // Show the open file dialog box to have the user find the missing file
        if( GetFileName( hwnd, AppData.hInst, FoundFilePath, DialogBoxCaption, 
            StartSearchPath, FileMask, NULL, OPEN_FILENAME_DIALOG_BOX ) )
        {
            // Crop out the file name we have found
            CropFileNameFromPath( FoundFilePath, FoundFile );

            // Did we find the same file that is missing?
            if( strcmp( FoundFile, SearchFile ) == 0 )
            {
                strcpy( MissingFile, FoundFilePath );
                Result = TRUE;
            }
            else
            {
                // The file they found is different than the file that was missing. Ask if they still want to use the file they selected.
                if( AskQuestion( MB_YESNO, "Error", 
                    "The file you found (%s) is different than the file that is missing (%s). Do you still want to use the file you found?", 
                    FoundFile, SearchFile ) == IDYES )
                {
                    strcpy( MissingFile, FoundFilePath );
                    Result = TRUE;
                }
            }
        }
    }

    return Result;

}   // FindMissingProjectFile


/************************************************************************
*    FUNCTION NAME:         OnClose                                                             
*
*    DESCRIPTION:           Besure we want to end this session
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

BOOL TSS2GO::OnClose( HWND hwnd )
{   
    BOOL Result = CheckToSave( hwnd );
    int i;

    // Looks like the app is going to quit so do all the quit stuff
    if( Result )
    {
        // Save the secret ini file settings
        SaveSecretINIsettings();

        // Save the settings
        Settings.WritePrivateString();

        // Give the edit field back its mesproc
        for( i = 0; i < MAX_EDT_CONTRL; ++i )
            if( AppData.OldEditProc[i] )
                SetWindowLong( AppData.hEditContrls[i], GWL_WNDPROC, LONG( AppData.OldEditProc[i] ) );

        // Same here too
        for( i = 0; i < MAX_BTN_GROUPS; ++i )
            if( AppData.OldEditTxtProc[i] )
                SetWindowLong( AppData.hEditMsg[i], GWL_WNDPROC, LONG( AppData.OldEditTxtProc[i] ) );

        // Destroy the window
        DestroyWindow( hwnd );
    }

    return Result;

}   // OnClose


/************************************************************************
*    FUNCTION NAME:         LoadKeyEntryPage                                                             
*
*    DESCRIPTION:           Load the page the user keyed in.
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

void TSS2GO::LoadKeyEntryPage()
{
    char NumberStr[20];
    int PageNumber;

    if( Edit_GetTextLength( AppData.hEditContrls[ EDT_CUR_PAGE ] ) )
    {
        Edit_GetText( AppData.hEditContrls[ EDT_CUR_PAGE ], NumberStr, sizeof(char)*20 );
        PageNumber = atoi( NumberStr ) - 1;

        if( PageNumber >= 0 && PageNumber < PageList.Count() )
        {
            PageList.SetCurrentPage( PageNumber );
            FlipPage( AppData.hParentWnd, PAGE_FLIP_RIGHT, TRUE );
        }
    }

}   // LoadKeyEntryPage


/************************************************************************
*
*                      View window Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         ViewWnd_CreateViewWnd                                                             
*
*    DESCRIPTION:           Create the window to view the loaded images
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hParent - Handle to window you want centered
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

BOOL TSS2GO::ViewWnd_CreateViewWnd( HINSTANCE hInst, HWND hParent )
{
    RECT ToolBarRect;

    // Get the client rect of the tool bar
    GetClientRect( AppData.hToolBar, &ToolBarRect );

    // Create the view window
    AppData.hViewWnd = CreateWindowEx( WS_EX_CLIENTEDGE, "ViewWnd", "View Window",
            WS_CHILD|WS_BORDER|WS_VISIBLE, VIEW_WND_X,
            VIEW_WND_Y + AppData.ChildOffsetY,
            VIEW_WND_W+VIEW_WND_FAT, VIEW_WND_H+VIEW_WND_FAT, hParent, HMENU(VIEW_WND), hInst, NULL );

    return ( AppData.hViewWnd != NULL );

}   // ViewWnd_CreateViewWnd


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

void TSS2GO::ViewWnd_OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT DisplayRect;
    HPALETTE hOldPalette = NULL;

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Make sure we are ready to blit
    if( !buffer.IsEmpty() && (ActProj.LiveProject || ActProj.OpenedFile) )
    {
        // Setup the rect
        SetRect( &DisplayRect, 0, 0, VIEW_WND_W, VIEW_WND_H );

        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256.
        if( GetDeviceCaps( PaintDC, BITSPIXEL ) <= 8 )
        {
            // Save the old palette so that we can select it back in before
            // deleting the palette we created. DeleteObject doesn't seem to
            // work if your trying to delete a palette that is still in the DC
            // We only use the main dib's pallete
            if( !MainDib.IsEmpty() )
            {
                hOldPalette = SelectPalette( PaintDC, MainDib.GetPalette(), FALSE );                 
                RealizePalette( PaintDC );
            }
        }

        // Blit the buffer to the screen
        buffer.StretchDraw( PaintDC, &DisplayRect );

       // Reselect the old palette back into the DC so that the created palette can be deleted
        if( hOldPalette )
            SelectPalette( PaintDC, hOldPalette, FALSE );
    }

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // ViewWnd_OnPaint


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnEraseBkgnd()                                                             
*
*    DESCRIPTION:           Standard Windows OnEraseBkgnd message handler.
*                           We control if we want the background to 
*                           erase itself.
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

BOOL TSS2GO::ViewWnd_OnEraseBkgnd( HWND hwnd, HDC hdc )
{
    HBRUSH brush;
    RECT DisplayRect;

    // Fill the view window with the system edit color
    if( !ActProj.LiveProject && !ActProj.OpenedFile )
    {
        // Get the rect of our view window
        GetClientRect( hwnd, &DisplayRect );

        brush = CreateSolidBrush( GetSysColor( COLOR_WINDOW ) );

        FillRect( hdc, &DisplayRect, brush );
        DeleteObject( brush );
    }

    return TRUE;

}   // ViewWnd_OnEraseBkgnd


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnCreate()                                                             
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

BOOL TSS2GO::ViewWnd_OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    return TRUE;

}   // ViewWnd_OnCreate


/************************************************************************
*    FUNCTION NAME:         ViewWnd_Repaint                                                             
*
*    DESCRIPTION:           Force the window to repaint itself
*
*    FUNCTION PARAMETERS:
*    Input:    var2type Var2Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TSS2GO::ViewWnd_Repaint()
{
    RECT DisplayRect;

    // Get the rect of our view window
    GetClientRect( AppData.hViewWnd, &DisplayRect );

    // Invalidate the whole window so it all gets painted
    InvalidateRect( AppData.hViewWnd, &DisplayRect, TRUE );

}   // ViewWnd_Repaint()