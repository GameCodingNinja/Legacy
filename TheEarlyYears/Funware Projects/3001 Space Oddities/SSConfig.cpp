/************************************************************************
*
*    PRODUCT:         Screen saver
*
*    FILE NAME:       SSConfig.cpp
*
*    DESCRIPTION:     Screen saver config dialog box
*
*    IMPLEMENTATION:  Dialog box for users to configure the screen saver
*                     Every project will need to have it's own different
*                     dialog box. This *.cpp and *.h are project specific
*                     and a new one for each screen saver project is required.
*
*    Copyright(c) 2000 by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "SSConfig.h"        // Header file for this *.cpp file.

#include "mintools.h"        // A misc group of useful functions
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "SSBase.h"          // The command center for all screen savers
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "MediaDir.h"        // Header file for the common dialog box modification.
#include "OrderOpt.h"        // Dialog the asks how you want to order
#include "OrderFrm.h"        // Order form window

// Globles
PTConfigDlg pConfigDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayConfigDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:     HINSTANCE hInst - This progams instance
*               HWND hwnd - Handle to window you want centered
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

BOOL DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pConfigDlg = new TConfigDlg();

    // Create the dialog box
    if( pConfigDlg )
        Result = pConfigDlg->CreateDialogWnd( hInst, hwnd );

    return Result;
}


                
/************************************************************************
*    FUNCTION NAME:         ConfigDlgProc
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

LRESULT CALLBACK ConfigDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pConfigDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pConfigDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_PAINT,      pConfigDlg->OnPaint );
        HANDLE_MSG( hDlg, WM_HSCROLL,    pConfigDlg->OnHScroll );
        HANDLE_MSG( hDlg, WM_SHOWWINDOW, pConfigDlg->OnShowWindow );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pConfigDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // ConfigDlgProc


/************************************************************************
*
*                     Object Related Functions
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

TConfigDlg::TConfigDlg()
{
    fRegisteredUser = FALSE;

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

TConfigDlg::~TConfigDlg()
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

int TConfigDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd )
{
    hInst = hInstance;

    return DialogBox( hInst, "DLG_CONFIG", hwnd, (DLGPROC)ConfigDlgProc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
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

BOOL TConfigDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    TGif gif;   // GIF decoder which can load GIFs from file or resource
    char WindowTitle[200];
    HDC dc;

    // Init the parent window handle
    hParentWnd = hwnd;
    
    // Load the GIF graphic from resource
    dib.LoadFromBuffer( gif.LoadFromResource( "settings", "GIF", hInst ) );

    // Load all the ini file settings
    Settings.GetPrivateString();

    // Setup the Order Form info
    SetupOrderFormInfo();

    // Init the window title string
    strcpy( WindowTitle, PRODUCT_VERSION );

    // Check to see if this is the registered version
    if( strcmp( FULL_VERSION_NUMBER, Settings.GetRegNum() ) == 0 )
    {
        // Add the tect to the file
        //strcat( WindowTitle, "    Registered" );

        // Disable the edit box and the static text label
        Edit_Enable( GetDlgItem( hwnd, EDT_REG_NUMBER ), FALSE );
        Static_Enable( GetDlgItem( hwnd, TXT_REG_NUM ), FALSE );

        // Remind them they are registered. You'd think they'd know.
        Edit_SetText( GetDlgItem( hwnd, EDT_REG_NUMBER ), " You Are Registered" );

        fRegisteredUser = TRUE;
    }
    else
    {
        // Add the tect to the file
        //strcat( WindowTitle, "    Shareware" );

        fRegisteredUser = FALSE;
    }

    // Set the new text to the window
    SetWindowText( hwnd, WindowTitle );

    // Setup the Star count scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_AMOUNT ), 0, 5, TRUE );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_AMOUNT ), Settings.GetStarCount(), TRUE );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_AMOUNT ), Settings.GetStarCount() );

    // Set a thicker font for the Registration number text
    // Remember, stock objects don't need to be freed
    SetWindowFont( GetDlgItem( hwnd, TXT_REG_NUM ), GetStockFont( SYSTEM_FONT ), FALSE );

    // Set the radio movement button to its last selection
    switch( Settings.GetStarMovement() )
    {
        case MOVE_LEFT:
            Button_SetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_LEFT ), TRUE );
        break;

        case MOVE_RIGHT:
            Button_SetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_RIGHT ), TRUE );
        break;

        case MOVE_DOWN:
            Button_SetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_DOWN ), TRUE );
        break;

        case MOVE_UP:
            Button_SetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_UP ), TRUE );
        break;

        case MOVE_RANDOM:
            Button_SetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_RANDOM ), TRUE );
        break;
    }

    // Set the check boxes
    Button_SetCheck( GetDlgItem( hwnd, CHK_PLANETS ), Settings.GetPlanetsOnOff() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_SPACE_SHIPS ), Settings.GetSpaceShipsOnOff() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ), Settings.GetRandomMusic() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_FRONT_OF_STARS ), Settings.GetPlanetsInfrontOfStars() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_CLOUDS ), Settings.GetCloudsOnOff() );

    // Set the radio buttons
    Button_SetCheck( GetDlgItem( hwnd, RDO_MUSIC_OFF ), Settings.GetMusicOff() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), Settings.GetThemeMusicOn() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), Settings.GetLoadYourOwn() );

    // Enable the random music check box if the load your own option is checked
    if( Settings.GetLoadYourOwn() )
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), TRUE );

    // Disable the "Planets in front of stars" check box and the "how many planets to
    // run at any given time" if the planets are turned off
    if( !Settings.GetPlanetsOnOff() )
    {
        Button_Enable( GetDlgItem( hwnd, CHK_FRONT_OF_STARS ), FALSE );
        ComboBox_Enable( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), FALSE );
    }

    // Add the combo box options for the number of planets
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "One planet at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Two planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Three planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Four planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Five planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Six planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Seven planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Eight planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Nine planets at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), "Ten planets at any time" );

    // Select the number of planets combobox index
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), Settings.GetNumPlanetsAtOneTime() );

    // Disable the "how many ships to run at any given time" if the ships are turned off
    if( !Settings.GetSpaceShipsOnOff() )
        ComboBox_Enable( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), FALSE );

    // Add the combo box options for the number of ships
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Three ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Four ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Five ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Six ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Seven ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Eight ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Nine ships at any time" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), "Ten ships at any time" );

    // Select the number of shipss combobox index
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), Settings.GetNumShipsAtOneTime() );

    // Get the DC so that we can check the color setting the system is at
    dc = GetDC( hwnd );

    // Disable specific features if we are in 256 color or less mode
    if( GetDeviceCaps( dc, BITSPIXEL ) <= 8 )
    {
        Button_Enable( GetDlgItem( hwnd, CHK_PLANETS ), FALSE );
        Button_Enable( GetDlgItem( hwnd, CHK_FRONT_OF_STARS ), FALSE );
        Button_Enable( GetDlgItem( hwnd, CHK_CLOUDS ), FALSE );
    }

    ReleaseDC( hwnd, dc );

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

void TConfigDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    HDC dc;

    switch(id)
    {
        case BTN_OK:
            // Go through all the controls and record their settings
            RecordUserSettings( hwnd );

            // Saver all the ini file settings
            Settings.WritePrivateString();

            // Free all the allocated objects
            FreeAllAlocations();

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case BTN_CANCEL:
            // Save off only the helful hint settings
            Settings.WriteHelpHintSettings();

            // Free all the allocated objects
            FreeAllAlocations();

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case BTN_ORDER_FORM:
            switch( DisplayOrderOptionDlg( hInst, hwnd ) )
            {
                case ONLINE_WEB_ORDER:
                    ShellExecute( NULL, "open", ONLINE_ORDER_WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
                break;

                case STANDARD_US_MAIL_ORDER:
                    ShowOrderFormDlg( hwnd );
                break;
            }
        break;

        case SHOW_HELPHINT_WND:
            // Show the helpful hint window
            if( Settings.GetIntroHelpWnd() )
            {
                Sleep( 400 );
                BOOL ShowNextTime;
                DisplayHelpHintDialogBox( hInst, hwnd, "settings", "TXT", "Screen Saver Settings", "Helpful Hint", &ShowNextTime, DO_CHECK_SUM );
                Settings.SetIntroHelpWnd( ShowNextTime );
            }

            // Get the DC so that we can check the color setting the system is at
            dc = GetDC( hwnd );

            // Disable specific features if we are in 256 color or less mode
            if( GetDeviceCaps( dc, BITSPIXEL ) <= 8 )
                PostMsg( "System Color", "Some features of this screen saver are not supported in 256 or less colors. You'll need to increase your system colors to enable these features." );

            ReleaseDC( hwnd, dc );
            
        break;

        case RDO_MUSIC_OFF:
            Settings.SetMusicOff( TRUE );
            Settings.SetThemeMusicOn( FALSE );
            Settings.SetLoadYourOwn( FALSE );
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
        break;

        case RDO_DEFAULT_MUSIC:
            Settings.SetMusicOff( FALSE );
            Settings.SetThemeMusicOn( TRUE );
            Settings.SetLoadYourOwn( FALSE );
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
        break;

        case RDO_LOAD_YOUR_OWN:
            GetSoundDirectory( hwnd );
        break;

        case CHK_PLANETS:
            // Disable or enable the "Planets in front of stars" check box
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_PLANETS ) ) )
            {
                Button_Enable( GetDlgItem( hwnd, CHK_FRONT_OF_STARS ), TRUE );
                ComboBox_Enable( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), TRUE );
            }
            else
            {
                Button_Enable( GetDlgItem( hwnd, CHK_FRONT_OF_STARS ), FALSE );
                ComboBox_Enable( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ), FALSE );
            }
        break;

        case CHK_SPACE_SHIPS:
            // Disable or enable the "number of ships on screen" combo box
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_SPACE_SHIPS ) ) )
                ComboBox_Enable( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), TRUE );
            else
                ComboBox_Enable( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ), FALSE );
        break;

        case BTN_WEB_SITE:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;
    }
}


/************************************************************************
*    FUNCTION NAME:         OnPaint()                                                             
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

void TConfigDlg::OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT r;

    // Get the rect of our parent window
    GetClientRect( hwnd, &r );

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Blit the graphic to the screen
    // The class also takes care of the realizing the palette if in 256 colors
    dib.BlitToDC( PaintDC, abs((r.right - dib.width()) / 2), 15 );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // OnPaint



/************************************************************************
*    FUNCTION NAME:         OnHScroll                                                             
*
*    DESCRIPTION:           Standard Windows OnHScroll message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT code       - The action taking place
*              int pos         - Position of the thumb
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

void TConfigDlg::OnHScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos )
{
    int position, ScrollMin, ScrollMax;

    // Get the scroll bars current position
    position = ScrollBar_GetPos( hwndCtl );

    // Get the scroll bars minimum and max settings
    ScrollBar_GetRange( hwndCtl, &ScrollMin, &ScrollMax );

    switch( code )
    {
        case SB_LINERIGHT:
        case SB_PAGERIGHT:
            if( position >= ScrollMin )
                ScrollBar_SetPos( hwndCtl, position + 1, TRUE );
        break;

        case SB_LINELEFT:
        case SB_PAGELEFT:
            if( position <= ScrollMax )
                ScrollBar_SetPos( hwndCtl, position - 1, TRUE );
        break;

        case SB_THUMBTRACK:
            ScrollBar_SetPos( hwndCtl, pos, TRUE );
        break;
    }

    // Set the label for the scroll bar
    if( code == SB_ENDSCROLL || code == SB_THUMBTRACK )
            SetScrollBarLabels( hwnd, hwndCtl, position );

}   // OnHScroll



/************************************************************************
*    FUNCTION NAME:         SetScrollBarLabels                                                             
*
*    DESCRIPTION:           Set the labels for the scroll bars
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwndCtl     - Handle of the scroll bar control
*              int ScrollPos    - scroll bar position
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

void TConfigDlg::SetScrollBarLabels( HWND hwnd, HWND hScrollBar, int ScrollPos )
{
    HWND hControl;

    // Set the Star amount bar label
    if( hScrollBar == GetDlgItem( hwnd, SCRBAR_AMOUNT ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, TXT_COUNT );

        switch( ScrollPos )
        {
            case 0: 
                Static_SetText( hControl, "Some Stars" );
            break;
            case 1: 
                Static_SetText( hControl, "More Stars" );
            break;
            case 2: 
                Static_SetText( hControl, "Even More Stars" );
            break;
            case 3: 
                Static_SetText( hControl, "Lots of Stars" );
            break;
            case 4: 
                Static_SetText( hControl, "Lots and Lots of Stars" );
            break;
            case 5: 
                Static_SetText( hControl, "Tons of Stars" );
            break;
        }
    }

}   // SetScrollBarLabels


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

void TConfigDlg::OnShowWindow( HWND hwnd, BOOL fShow, UINT status )
{
    // Post a message to show the Helpful hint window
    // This way it doesn't show up before the config window does
    PostMessage( (HWND)hwnd, (UINT)WM_COMMAND, (WPARAM)MAKELONG( SHOW_HELPHINT_WND, 0 ), (LPARAM)0 );

}   // OnShowWindow



/************************************************************************
*    FUNCTION NAME:         RecordUserSettings                                                             
*
*    DESCRIPTION:           Go through all the controls and record the
*                           settings they hold
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TConfigDlg::RecordUserSettings( HWND hwnd )
{
    char RegNum[100];

    // Check to see if they entered in a registration number
    // Check the length of the text in the edit box to see if anything was entered
    if( !fRegisteredUser && Edit_GetTextLength( GetDlgItem( hwnd, EDT_REG_NUMBER ) ) )
    {
        // Get text the user entered in as a registration number
        Edit_GetText( GetDlgItem( hwnd, EDT_REG_NUMBER ), RegNum, sizeof(RegNum) );

        // Check to see if the number is a valid registration number
        CheckRegNumber( RegNum );
    }

    // Save the position of the number of stars
    Settings.SetStarCount( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_AMOUNT ) ) );

    // Save the direction of the stars and planets

    if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_LEFT ) ) )
        Settings.SetStarMovement( MOVE_LEFT );
        
    else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_RIGHT ) ) )
        Settings.SetStarMovement( MOVE_RIGHT );

    else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_UP ) ) )
        Settings.SetStarMovement( MOVE_UP );

    else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_DOWN ) ) )
        Settings.SetStarMovement( MOVE_DOWN );

    else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MOVE_RANDOM ) ) )
        Settings.SetStarMovement( MOVE_RANDOM );

    // Get the check boxes settings
    Settings.SetPlanetsOnOff( Button_GetCheck( GetDlgItem( hwnd, CHK_PLANETS ) ) );
    Settings.SetSpaceShipsOnOff( Button_GetCheck( GetDlgItem( hwnd, CHK_SPACE_SHIPS ) ) );
    Settings.SetPlanetsInfrontOfStars( Button_GetCheck( GetDlgItem( hwnd, CHK_FRONT_OF_STARS ) ) );
    Settings.SetCloudsOnOff( Button_GetCheck( GetDlgItem( hwnd, CHK_CLOUDS ) ) );

    // Save the random music settings
    Settings.SetRandomMusic( Button_GetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ) ) );

    // Get the combobox for settings for the number of planets on the screen 
    Settings.SetNumPlanetsAtOneTime( ComboBox_GetCurSel( GetDlgItem( hwnd, CBOBOX_NUM_PLANETS ) ) );

    // Get the combobox for settings for the number of ships on the screen 
    Settings.SetNumShipsAtOneTime( ComboBox_GetCurSel( GetDlgItem( hwnd, CBOBOX_NUM_SHIPS ) ) );

}   // RecordUserSettings



/************************************************************************
*    FUNCTION NAME:         FreeAllAlocations                                                             
*
*    DESCRIPTION:           Free all the allocations
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

void TConfigDlg::FreeAllAlocations()
{

}   // Config_FreeAllAlocations


/************************************************************************
*    FUNCTION NAME:         GetSoundDirectory                                                             
*
*    DESCRIPTION:           Get the users sound directory
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

void TConfigDlg::GetSoundDirectory( HWND hwnd )
{
    // Display the dialog box to get the users sound directory
    if( MediaDir_GetSoundDirectory( hInst, hwnd, Settings.GetSoundDir() ) )
    {
        // Set the radio buttons settings
        Settings.SetMusicOff( FALSE );
        Settings.SetThemeMusicOn( FALSE );
        Settings.SetLoadYourOwn( TRUE );
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), TRUE );
    }
    else // They canceled so set it back to what it was before the cancel
    {
        Button_SetCheck( GetDlgItem( hwnd, RDO_MUSIC_OFF ), Settings.GetMusicOff() );
        Button_SetCheck( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), Settings.GetThemeMusicOn() );
        Button_SetCheck( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), Settings.GetLoadYourOwn() );
        if( !Settings.GetLoadYourOwn() )
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
    }

}   // GetSoundDirectory
