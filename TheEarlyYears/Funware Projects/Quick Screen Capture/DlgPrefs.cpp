/************************************************************************
*
*    PRODUCT:         Pref dialog box
*
*    FILE NAME:       DlgPrefs.cpp
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "DlgPrefs.h"        // Header for this *.cpp file
#include "GDefines.h"        // Header file with #defines
#include "shlobj.h"          // Allows for use of the directory selection window
#include <stdio.h>

// Globals
PCPrefsDlg pPrefsDlg;

extern int gGrabKey[21];
// File extension array
LPSTR fileType[MAX_FILE_FORMATS] = { "Windows Bitmap (*.bmp)", "PNG (*.png)", "TIF (*.tif)",  "TIF Adobe Compression (*.tif)",
                                     "JPEG (*.jpg) Quality Superb", "JPEG (*.jpg) Quality High", "JPEG (*.jpg) Quality Good", 
                                     "JPEG (*.jpg) Quality Normal", "JPEG (*.jpg) Quality Average", "JPEG (*.jpg) Quality Bad" };


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        Display_PrefsDlg                                                             
*
*    DESCRIPTION:           Create dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progam instance
*              HWND hwnd - Handle to the parent window
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

BOOL Display_PrefsDlg( HINSTANCE hInst, HWND hwnd, PTSettings pSet )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pPrefsDlg = new CPrefsDlg();

    // Create the dialog box
    if( pPrefsDlg )
        Result = pPrefsDlg->CreateDialogWnd( hInst, hwnd, pSet );

    return Result;

}   // Display_PrefsDlg


/************************************************************************
*    FUNCTION NAME:         PrefsDlg_Proc
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

LRESULT CALLBACK PrefsDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pPrefsDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pPrefsDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pPrefsDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pPrefsDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // PrefsDlg_Proc


/************************************************************************
*    FUNCTION NAME:         SubClassEdtProc                                                             
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

LRESULT CALLBACK SubClassEdtProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return pPrefsDlg->OnEdit_GetName( hCtrl, Message, wParam, lParam );

}   // SubClassEdtProc


/************************************************************************
*
*                        Dialog Object Functions
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

CPrefsDlg::CPrefsDlg()
{
    // Init class members

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

CPrefsDlg::~CPrefsDlg()
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

BOOL CPrefsDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, PTSettings pSet )
{
    // Init class variables
    hInst = hInstance;
    hParentWnd = hwnd;
    pSettings = pSet;

    return DialogBox( hInst, "DLG_PREFS", hParentWnd, (DLGPROC)PrefsDlg_Proc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
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

BOOL CPrefsDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    int i;

    // Get the handle to this dialog box
    hDlgWnd = hwnd;

    // Subclass the windows control
    OldSubEdtCtrlProc = (WNDPROC)SetWindowLong( GetDlgItem( hwnd, EDT_FILE_NAME ), GWL_WNDPROC, 
                                                LONG( SubClassEdtProc ) );

    // Set a text limit for the file name edit field
    Edit_LimitText( GetDlgItem( hwnd, EDT_FILE_NAME ), FILE_NAME_SIZE );

    // Fill the Save Format combo box
    for( i = 0; i < MAX_FILE_FORMATS; ++i )
	{
        ComboBox_AddString( GetDlgItem( hwnd, CBO_FILE_SAVE_OPT ), fileType[i] );
	}

    // Set the combo box selection
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_FILE_SAVE_OPT ), pSettings->GetSaveOption() );

    // Fill the Bit Depth combo box
    ComboBox_AddString( GetDlgItem( hwnd, CBO_BIT_DEPTH_OPT ), "System Default" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_BIT_DEPTH_OPT ), "8-Bit Color" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_BIT_DEPTH_OPT ), "16-Bit Color" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_BIT_DEPTH_OPT ), "24-Bit Color" );

    // Set the combo box selection
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_BIT_DEPTH_OPT ), pSettings->GetBitDepth() );

    // Fill the Grab Key combo box
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F1" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F2" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F3" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F4" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F5" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F6" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F7" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F8" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F9" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F10" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F11" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "F12" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Scroll Lock" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Pause" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Insert" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Home" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Delete" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "End" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Shift" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Ctrl" );
    ComboBox_AddString( GetDlgItem( hwnd, CBO_GRAB_KEY ), "Alt" );

    // Set the combo box selection
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_GRAB_KEY ), pSettings->GetGrabKey() );

    // Set the edit field with the path text
    Edit_SetText( GetDlgItem( hwnd, EDT_FILE_PATH ), pSettings->GetSavePath() );

    // Set the edit field with the default file name
    Edit_SetText( GetDlgItem( hwnd, EDT_FILE_NAME ), pSettings->GetFileName() );

	// Set the sound checkbox
    Button_SetCheck( GetDlgItem( hwnd, CHK_SOUND_EFFECT ), pSettings->GetSoundEffect() );

	// Setup the hour timer combo box options
	for( i = 0; i < MAX_TIMER_HOURS; ++i )
	{
		char valueStr[50];
		sprintf( valueStr, "%d", i );
		ComboBox_AddString( GetDlgItem( hwnd, CBO_TIMER_HOURS ), valueStr );
	}

	// Setup the minute timer combo box options
	for( i = 0; i < MAX_TIMER_MINUTES; ++i )
	{
		char valueStr[50];
		sprintf( valueStr, "%d", i );
		ComboBox_AddString( GetDlgItem( hwnd, CBO_TIMER_MINUTES ), valueStr );
	}

	// Setup the minute timer combo box options
	for( i = 0; i < MAX_TIMER_SECONDS; ++i )
	{
		char valueStr[50];
		sprintf( valueStr, "%d", i );
		ComboBox_AddString( GetDlgItem( hwnd, CBO_TIMER_SECONDS ), valueStr );
	}

	// Set the combo box selection
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_TIMER_HOURS ), pSettings->GetTimerHour() );
	ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_TIMER_MINUTES ), pSettings->GetTimerMinute() );
	ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_TIMER_SECONDS ), pSettings->GetTimerSecond() );

    return TRUE;

}   // OnInitDialog


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

void CPrefsDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case BTN_CANCEL:
            // The TRUE is the return parameter. This can be an int.
            EndDialog( hwnd, FALSE );        
        break;

        case BTN_OK:
            // Save any changed selections
            SaveUserSelections( hwnd );

            // The TRUE is the return parameter. This can be an int.
            EndDialog( hwnd, TRUE );        
        break;

        case BTN_SEL_DIR:
            BtnClk_SelectDirectory( hwnd );
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*                           You could free memory and do other clean up.
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

void CPrefsDlg::OnDestroy( HWND hwnd ) 
{
    // Give all the subclassed controls their procedure back 
    // before destroying the dialog box
    if( OldSubEdtCtrlProc )
        SetWindowLong( GetDlgItem( hwnd, EDT_FILE_NAME ), GWL_WNDPROC, LONG( OldSubEdtCtrlProc ) );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         BtnClk_SelectDirectory                                                             
*
*    DESCRIPTION:           Allows the user to select a directory
*                           
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

void CPrefsDlg::BtnClk_SelectDirectory( HWND hwnd )
{
    BROWSEINFO bi;
    char tmpDir[MAX_PATH+1];
    LPITEMIDLIST pidlBrowse;

    bi.hwndOwner = hwnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = tmpDir;  // Only returns the selected folder name, not the path
    bi.lpszTitle = "Select a folder you want your screen shots to be saved in.";
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
    
    // Allow the user to browse for a folder
    if( ( pidlBrowse = SHBrowseForFolder( &bi ) ))
    {
        // Use the returned item list to get the path to the folder
        SHGetPathFromIDList( pidlBrowse, tmpDir );

        // Set the edit field with the path text
        Edit_SetText( GetDlgItem( hwnd, EDT_FILE_PATH ), tmpDir );
    }

}   // BtnClk_SelectDirectory


/************************************************************************
*    FUNCTION NAME:         SaveUserSelections                                                             
*
*    DESCRIPTION:           Save the user selection
*                           
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

void CPrefsDlg::SaveUserSelections( HWND hwnd )
{
    char strTmp[MAX_PATH+1];

    // Get the path to the folder
    Edit_GetText( GetDlgItem( hwnd, EDT_FILE_PATH ), strTmp, sizeof(char) * (MAX_PATH) );
    
    if( strTmp[0] )
        strcpy( pSettings->GetSavePath(), strTmp );

    // Get the file name
    Edit_GetText( GetDlgItem( hwnd, EDT_FILE_NAME ), strTmp, sizeof(char) * (MAX_PATH) );
    
    if( strTmp[0] )
        strcpy( pSettings->GetFileName(), strTmp );

    // This clears out any previous key presses so that a screen capture is
    // not taken when they click OK.
    GetAsyncKeyState( gGrabKey[ ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_GRAB_KEY ) ) ] );

    pSettings->SetSaveOption( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_FILE_SAVE_OPT ) ) );
    pSettings->SetBitDepth( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_BIT_DEPTH_OPT ) ) );
    pSettings->SetGrabKey( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_GRAB_KEY ) ) );
    pSettings->SetSoundEffect( Button_GetCheck( GetDlgItem( hwnd, CHK_SOUND_EFFECT ) ) );

    pSettings->SetTimerHour( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_TIMER_HOURS ) ) );
	pSettings->SetTimerMinute( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_TIMER_MINUTES ) ) );
	pSettings->SetTimerSecond( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_TIMER_SECONDS ) ) );

}   // SaveUserSelections


/************************************************************************
*    FUNCTION NAME:         OnEdit_GetName                                                             
*
*    DESCRIPTION:           Message handler for the Create animation edit control    
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

LRESULT CPrefsDlg::OnEdit_GetName( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    // Keep illigal characters out of the edit field
    if( Message == WM_CHAR )
    {
        if( (wParam >= '!' &&  wParam <= ',') || (wParam >= ':' &&  wParam <= '@') ||
            (wParam >= '[' &&  wParam <= '^') || (wParam >= '{' &&  wParam <= '~') ||
            wParam == '.' || wParam == '/' || wParam == '`' )
            return FALSE;
    }

    return CallWindowProc( OldSubEdtCtrlProc, hCtrl, Message, wParam, lParam );

}   // OnEdit_GetName