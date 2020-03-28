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
#include "ssdefines.h"       // Header file with #defines
#include "gdefines.h"        // Header file with #defines
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

// Globles
PCConfigDlg pConfigDlg;


// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:     HINSTANCE hInst - This progams instance
*               HWND hwnd - Handle to window you want centered
*
*    Output:   bool - True or False depending on if successful
************************************************************************/
bool DisplayConfigDialogBox( HINSTANCE hInst, HWND hwnd )
{
    bool Result = false;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pConfigDlg = new CConfigDlg();

    // Create the dialog box
    if( pConfigDlg )
        Result = pConfigDlg->CreateDialogWnd( hInst, hwnd );

    return Result;

}   // DisplayConfigDialogBox

                
/************************************************************************
*    DESCRIPTION:           Standard Windows dialog message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of the window receiving the message
*              UINT uMsg      - Message being sent
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   LRESULT CALLBACK - Description
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
            return false;
    }
}   // ConfigDlgProc


/************************************************************************
*
*                     Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
************************************************************************/
CConfigDlg::CConfigDlg()
{

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
************************************************************************/
CConfigDlg::~CConfigDlg()
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
int CConfigDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd )
{
    hInst = hInstance;

    return DialogBox( hInst, "DLG_CONFIG", hwnd, (DLGPROC)ConfigDlgProc );

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
*    Output:   bool - True or False depending on if successful
************************************************************************/
bool CConfigDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    CGif gif;   // GIF decoder which can load GIFs from file or resource
    char WindowTitle[200];

    // Save the parent window handle
    hParentWnd = hwnd;

    // Load all the ini file settings
    settings.GetPrivateString();
    
    // Load the GIF graphic from resource
    dib.LoadFromBuffer( gif.LoadFromResource( "settings", "GIF", hInst ) );

    // Init the window title string
    strcpy( WindowTitle, PRODUCT_VERSION );

    // Set the new text to the window
    SetWindowText( hwnd, WindowTitle );

    // Setup the Speed scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_SPEED ), 0, 5, true );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_SPEED ), settings.GetSpeed(), true );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_SPEED ), settings.GetSpeed() );    

    // Set the music related buttons
    Button_SetCheck( GetDlgItem( hwnd, RDO_MUSIC_OFF ), settings.GetMusicOff() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), settings.GetThemeMusicOn() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), settings.GetLoadYourOwn() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ), settings.GetRandomMusic() );

    // Enable the random music check box if the load your own option is checked
    if( settings.GetLoadYourOwn() )
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), true );

    return true;

}   // OnInitDialog


/************************************************************************
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
************************************************************************/
void CConfigDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {
        case BTN_OK:
            // Go through all the controls and record their settings
            RecordUserSettings( hwnd );

            // Saver all the ini file settings
            settings.WritePrivateString();

            // Free all the allocated objects
            FreeAllAlocations();

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case BTN_CANCEL:
            // Save off only the helful hint settings
            settings.WriteHelpHintSettings();

            // Free all the allocated objects
            FreeAllAlocations();

            // Kill the program
            EndDialog( hwnd, 0 );
        break;

        case SHOW_HELPHINT_WND:
            // Show the helpful hint window
            if( settings.GetIntroHelpWnd() )
            {
                Sleep( 400 );
                bool ShowNextTime;
                DisplayHelpHintDialogBox( hInst, hwnd, "settings", "TXT", "Screen Saver Settings", "Helpful Hint", &ShowNextTime );
                settings.SetIntroHelpWnd( ShowNextTime );
            }
            
        break;

        case RDO_MUSIC_OFF:
            settings.SetMusicOff( true );
            settings.SetThemeMusicOn( false );
            settings.SetLoadYourOwn( false );
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), false );
        break;

        case RDO_DEFAULT_MUSIC:
            settings.SetMusicOff( false );
            settings.SetThemeMusicOn( true );
            settings.SetLoadYourOwn( false );
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), false );
        break;

        case RDO_LOAD_YOUR_OWN:
            GetSoundDirectory( hwnd );
        break;

        case BTN_WEB_SITE:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;
    }
}   // OnCommand


/************************************************************************
*    DESCRIPTION:           Standard Windows OnPaint message handler.    
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
************************************************************************/
void CConfigDlg::OnPaint( HWND hwnd )
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
*    DESCRIPTION:           Standard Windows OnHScroll message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT code       - The action taking place
*              int pos         - Position of the thumb
************************************************************************/
void CConfigDlg::OnHScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos )
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
                ScrollBar_SetPos( hwndCtl, position + 1, true );
        break;

        case SB_LINELEFT:
        case SB_PAGELEFT:
            if( position <= ScrollMax )
                ScrollBar_SetPos( hwndCtl, position - 1, true );
        break;

        case SB_THUMBTRACK:
            ScrollBar_SetPos( hwndCtl, pos, true );
        break;
    }

    // Set the label for the scroll bar
    if( code == SB_ENDSCROLL || code == SB_THUMBTRACK )
            SetScrollBarLabels( hwnd, hwndCtl, position );

}   // OnHScroll


/************************************************************************
*    DESCRIPTION:           Set the labels for the scroll bars
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwndCtl     - Handle of the scroll bar control
*              int ScrollPos    - scroll bar position
************************************************************************/
void CConfigDlg::SetScrollBarLabels( HWND hwnd, HWND hScrollBar, int ScrollPos )
{
    HWND hControl;

    // Set the Speed bar label
    if( hScrollBar == GetDlgItem( hwnd, SCRBAR_SPEED ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, TXT_SPEED );

        switch( ScrollPos )
        {
            case 0: 
                Static_SetText( hControl, "Speed: Very Slow" );
            break;
            case 1: 
                Static_SetText( hControl, "Speed: Slower" );
            break;
            case 2: 
                Static_SetText( hControl, "Speed: Slow" );
            break;
            case 3: 
                Static_SetText( hControl, "Speed: Fast" );
            break;
            case 4: 
                Static_SetText( hControl, "Speed: Very Fast" );
            break;
            case 5: 
                Static_SetText( hControl, "Speed: Speedy" );
            break;
        }
    }
}   // SetScrollBarLabels


/************************************************************************
*    DESCRIPTION:           Standard Windows OnShowWindowe message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              bool fShow      - show/hide flag
*              UINT status     - status flag
************************************************************************/
void CConfigDlg::OnShowWindow( HWND hwnd, bool fShow, UINT status )
{
    // Post a message to show the Helpful hint window
    // This way it doesn't show up before the config window does
    PostMessage( hwnd, (UINT)WM_COMMAND, (WPARAM)MAKELONG( SHOW_HELPHINT_WND, 0 ), (LPARAM)0 );

}   // OnShowWindow


/************************************************************************
*    DESCRIPTION:           Go through all the controls and record the
*                           settings they hold
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
************************************************************************/
void CConfigDlg::RecordUserSettings( HWND hwnd )
{
    // Save the position of the speed bar
    settings.SetSpeed( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_SPEED ) ) );

    // Save the random music settings
    settings.SetRandomMusic( Button_GetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ) ) );

}   // RecordUserSettings


/************************************************************************
*    DESCRIPTION:           Free all the allocations
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
************************************************************************/
void CConfigDlg::FreeAllAlocations()
{

}   // FreeAllAlocations


/************************************************************************
*    DESCRIPTION:           Get the users sound directory
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message
************************************************************************/
void CConfigDlg::GetSoundDirectory( HWND hwnd )
{
    // Display the dialog box to get the users sound directory
    if( MediaDir_GetSoundDirectory( hInst, hwnd, settings.GetSoundDir() ) )
    {
        // Set the radio buttons settings
        settings.SetMusicOff( false );
        settings.SetThemeMusicOn( false );
        settings.SetLoadYourOwn( true );
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), true );
    }
    else // They canceled so set it back to what it was before the cancel
    {
        Button_SetCheck( GetDlgItem( hwnd, RDO_MUSIC_OFF ), settings.GetMusicOff() );
        Button_SetCheck( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), settings.GetThemeMusicOn() );
        Button_SetCheck( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), settings.GetLoadYourOwn() );
        if( !settings.GetLoadYourOwn() )
            Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), false );
    }

}   // GetSoundDirectory
