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
#include "Settings.h"        // User settings for this screen saver
#include "dib.h"             // Dib and Bitmap class for displaying and animating bitmaps
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "SSBase.h"          // The command center for all screen savers
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "MediaDir.h"        // Header file for the common dialog box modification.
#include "OrderOpt.h"        // Dialog the asks how you want to order
#include "OrderFrm.h"        // Order form window
#include "TWave.h"           // Header file for low level loading and fast playing of wav files

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

}   // DisplayConfigDialogBox

                
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
    CWaveDevice testDevice;

    // Save the parent window handle
    hParentWnd = hwnd;

    // Load all the ini file settings
    Settings.GetPrivateString();
    
    // Load the GIF graphic from resource
    dib.LoadFromBuffer( gif.LoadFromResource( "settings", "GIF", hInst ) );

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

    // Setup the Rain Amount scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_RAIN_AMOUNT ), 0, MAX_RAIN_AMOUNT-1, TRUE );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_RAIN_AMOUNT ), Settings.GetRainAmount(), TRUE );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_RAIN_AMOUNT ), Settings.GetRainAmount() );

    // Setup the Lightning Hits Amount scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_LIGHT_HITS ), 0, MAX_LIGHT_SET-1, TRUE );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_LIGHT_HITS ), Settings.GetLightHits(), TRUE );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_LIGHT_HITS ), Settings.GetLightHits() );

    // Setup the Group Hits Amount scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_GROUP_HITS ), 0, MAX_LIGHT_SET-1, TRUE );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_GROUP_HITS ), Settings.GetGroupHits(), TRUE );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_GROUP_HITS ), Settings.GetGroupHits() );

    // Setup the Blackout Amount scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_BLACKOUTS ), 0, MAX_LIGHT_SET-1, TRUE );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_BLACKOUTS ), Settings.GetBlackOuts(), TRUE );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_BLACKOUTS ), Settings.GetBlackOuts() );

    // Set a thicker font for the Registration number text
    // Remember, stock objects don't need to be freed
    SetWindowFont( GetDlgItem( hwnd, TXT_REG_NUM ), GetStockFont( SYSTEM_FONT ), FALSE );    

    // Set the music related buttons
    Button_SetCheck( GetDlgItem( hwnd, RDO_MUSIC_OFF ), Settings.GetMusicOff() );
    //Button_SetCheck( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), Settings.GetThemeMusicOn() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), Settings.GetLoadYourOwn() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ), Settings.GetRandomMusic() );

    // Set the sound effects check box
    Button_SetCheck( GetDlgItem( hwnd, CHK_SND_EFX ), Settings.GetSoundOn() );

    // Do a test to see if we can have many sound devices
    ManySndDevices = testDevice.TestMultiDeviceOpen();

    // Disable the music controls if we can't open multiple devices
    if( Settings.GetSoundOn() && !ManySndDevices )
    {
        Button_Enable( GetDlgItem( hwnd, RDO_MUSIC_OFF ), FALSE );
        Button_Enable( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), FALSE );
        Button_Enable( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), FALSE );
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
    }
    // Enable the random music check box if the load your own option is checked
    else if( Settings.GetLoadYourOwn() )
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), TRUE );

    // Set the group box lable depending on if driver mixing is available
    if( ManySndDevices )
        Static_SetText( GetDlgItem( hwnd, GRPBOX_MUSIC ), "Sound Options:" );
    else
        Static_SetText( GetDlgItem( hwnd, GRPBOX_MUSIC ), "Sound Options (Limited):" );

    // Add the combo box entries
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_RAIN_OPT ), "Straight Rain" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_RAIN_OPT ), "Diagonal Rain" );
    ComboBox_AddString( GetDlgItem( hwnd, CBOBOX_RAIN_OPT ), "Randomise Rain Type" );
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBOBOX_RAIN_OPT ), Settings.GetRainType() );

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
            
        break;

        case RDO_MUSIC_OFF:
            Settings.SetMusicOff( TRUE );
            Settings.SetThemeMusicOn( FALSE );
            Settings.SetLoadYourOwn( FALSE );
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
        break;
/*
        case RDO_DEFAULT_MUSIC:
            Settings.SetMusicOff( FALSE );
            Settings.SetThemeMusicOn( TRUE );
            Settings.SetLoadYourOwn( FALSE );
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
        break;
*/

        case RDO_LOAD_YOUR_OWN:
            GetSoundDirectory( hwnd );
        break;

        case BTN_WEB_SITE:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;

        case CHK_SND_EFX:
            // Toggle the music options if we don't support sound mixing
            if( !ManySndDevices )
            {
                if( Button_GetCheck( GetDlgItem( hwnd, CHK_SND_EFX ) ) )
                {
                    Edit_Enable( GetDlgItem( hwnd, RDO_MUSIC_OFF ), FALSE );
                    Edit_Enable( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), FALSE );
                    Edit_Enable( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), FALSE );
                    Edit_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), FALSE );
                }
                else
                {
                    Edit_Enable( GetDlgItem( hwnd, RDO_MUSIC_OFF ), TRUE );
                    Edit_Enable( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), TRUE );
                    Edit_Enable( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), TRUE );
                    Edit_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), TRUE );
                }
            }
        break;
    }
}   // OnCommand


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

    // Set the rain amount bar label
    if( hScrollBar == GetDlgItem( hwnd, SCRBAR_RAIN_AMOUNT ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, STC_RAIN_AMOUNT );

        switch( ScrollPos )
        {
            case RAIN_AMOUNT_SPRINKLE: 
                Static_SetText( hControl, "Rain Amount: Sprinkle" );
            break;
            case RAIN_AMOUNT_SHOWER: 
                Static_SetText( hControl, "Rain Amount: Shower" );
            break;
            case RAIN_AMOUNT_HEAVY: 
                Static_SetText( hControl, "Rain Amount: Heavy" );
            break;
            case RAIN_AMOUNT_DELUGE: 
                Static_SetText( hControl, "Rain Amount: Deluge" );
            break;
            case RAIN_AMOUNT_MONSOON: 
                Static_SetText( hControl, "Rain Amount: Monsoon" );
            break;
        }
    }
    // Set the lightning amount bar label
    else if( hScrollBar == GetDlgItem( hwnd, SCRBAR_LIGHT_HITS ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, STC_LIGHTING_HITS );

        switch( ScrollPos )
        {
            case LIGHTNING_RARELY: 
                Static_SetText( hControl, "Lightning Hits: Rarely" );
            break;
            case LIGHTNING_NORMAL: 
                Static_SetText( hControl, "Lightning Hits: Normal" );
            break;
            case LIGHTNING_OFTEN: 
                Static_SetText( hControl, "Lightning Hits: Often" );
            break;
            case LIGHTNING_LOTS: 
                Static_SetText( hControl, "Lightning Hits: Lots" );
            break;
        }
    }
    // Set the lightning group amount bar label
    else if( hScrollBar == GetDlgItem( hwnd, SCRBAR_GROUP_HITS ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, STC_GROUP_HITS );

        switch( ScrollPos )
        {
            case LIGHTNING_RARELY: 
                Static_SetText( hControl, "Group Hits: Rarely" );
            break;
            case LIGHTNING_NORMAL: 
                Static_SetText( hControl, "Group Hits: Normal" );
            break;
            case LIGHTNING_OFTEN: 
                Static_SetText( hControl, "Group Hits: Often" );
            break;
            case LIGHTNING_LOTS: 
                Static_SetText( hControl, "Group Hits: Lots" );
            break;
        }
    }
    // Set the blackout amount bar label
    else if( hScrollBar == GetDlgItem( hwnd, SCRBAR_BLACKOUTS ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, STC_BLACKOUTS );

        switch( ScrollPos )
        {
            case LIGHTNING_RARELY: 
                Static_SetText( hControl, "Blackout: Rarely" );
            break;
            case LIGHTNING_NORMAL: 
                Static_SetText( hControl, "Blackout: Normal" );
            break;
            case LIGHTNING_OFTEN: 
                Static_SetText( hControl, "Blackout: Often" );
            break;
            case LIGHTNING_LOTS: 
                Static_SetText( hControl, "Blackout: Lots" );
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
    PostMessage( hwnd, (UINT)WM_COMMAND, (WPARAM)MAKELONG( SHOW_HELPHINT_WND, 0 ), (LPARAM)0 );

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

    // Save the position of the rain amount bar
    Settings.SetRainAmount( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_RAIN_AMOUNT ) ) );

    // Save the position of the Light Hits bar
    Settings.SetLightHits( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_LIGHT_HITS ) ) );

    // Save the position of the GroupHits bar
    Settings.SetGroupHits( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_GROUP_HITS ) ) );

    // Save the position of the BlackOuts bar
    Settings.SetBlackOuts( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_BLACKOUTS ) ) );

    // Save the rain settings
    Settings.SetRainType( ComboBox_GetCurSel( GetDlgItem( hwnd, CBOBOX_RAIN_OPT ) ) );

    // Save the sound effects check box
    Settings.SetSoundOn( Button_GetCheck( GetDlgItem( hwnd, CHK_SND_EFX ) ) );

    // Save the random music settings
    Settings.SetRandomMusic( Button_GetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ) ) );

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

}   // FreeAllAlocations


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
