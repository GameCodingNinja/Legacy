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

// Get the bit depth of the system from a windows handle
int GetSystemBitDepth( HWND hwnd );

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
        HANDLE_MSG( hDlg, WM_DESTROY,    pConfigDlg->OnDestroy );
        default:
            return FALSE;
    }
}   // ConfigDlgProc


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
    return pConfigDlg->HandleSubClassProc( hCtrl, Message, wParam, lParam );

}   // SubClassProc


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
    pFish = FALSE;
    pBackFish = FALSE;
    pBubble = FALSE;
    BufferReadyToBlit = FALSE;
    pOfficeProps = FALSE;
    fish = NULL;
    pFishPropXOffset = NULL;
    backFish = NULL;
    BackFishCount = 0;
    BubbleCount = 0;
    pBubble = NULL;

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
    if( fish )
        delete [] fish;

    if( pFishPropXOffset )
        delete [] pFishPropXOffset;

    if( backFish )
        delete [] backFish;

    if( pBubble )
        delete [] pBubble;

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
    int i;

    // init the parent window
    hParentWnd = hwnd;

    // Reseed the random number generator
    randomize();

    // Load all the ini file settings
    Settings.GetPrivateString();
    
    // Load the GIF graphic from resource
    dib.LoadFromBuffer( gif.LoadFromResource( "settings", "GIF", hInst ) );

    // Setup the Order Form info
    SetupOrderFormInfo();

    // Init the window title string
    strcpy( WindowTitle, PRODUCT_VERSION );

    // Set the new text to the window
    SetWindowText( hwnd, WindowTitle );

    // Assign a bitmap to the buttons
    SendMessage( GetDlgItem( hwnd, BTN_FONT_SELECT ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( hInst, "fonts" ) );

    // Assign a bitmap to the buttons
    SendMessage( GetDlgItem( hwnd, BTN_FONT_COLOR ), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 
                 (LPARAM) (HANDLE)LoadBitmap( hInst, "colors" ) );

    // Get the color mode
    SystemBitDepth = GetSystemBitDepth( hwnd );

    // Setup the number of fish scroll bar
    ScrollBar_SetRange( GetDlgItem( hwnd, SCRBAR_FISH_COUNT ), 0, 4, TRUE );
    ScrollBar_SetPos( GetDlgItem( hwnd, SCRBAR_FISH_COUNT ), Settings.GetFishCount(), TRUE );
    SetScrollBarLabels( hwnd, GetDlgItem( hwnd, SCRBAR_FISH_COUNT ), Settings.GetFishCount() );

    // Set a thicker font for the Registration number text
    // Remember, stock objects don't need to be freed
    SetWindowFont( GetDlgItem( hwnd, TXT_REG_NUM ), GetStockFont( SYSTEM_FONT ), FALSE );

    // Set the check boxes
    Button_SetCheck( GetDlgItem( hwnd, CHK_BUBBLES ), Settings.GetBubblesOnOff() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_BACK_FISH ), Settings.GetBackFishOnOff() );

    // Set the radio buttons
    Button_SetCheck( GetDlgItem( hwnd, RDO_MUSIC_OFF ), Settings.GetMusicOff() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_DEFAULT_MUSIC ), Settings.GetThemeMusicOn() );
    Button_SetCheck( GetDlgItem( hwnd, RDO_LOAD_YOUR_OWN ), Settings.GetLoadYourOwn() );
    Button_SetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ), Settings.GetRandomMusic() );

    // Enable the random music check box if the load your own option is checked
    if( Settings.GetLoadYourOwn() )
        Button_Enable( GetDlgItem( hwnd, CHK_RAND_MUSIC ), TRUE );

    // Enable the Company Name feature if they are registered
    Button_Enable( GetDlgItem( hwnd, BTN_FONT_SELECT ), TRUE );
    Button_Enable( GetDlgItem( hwnd, BTN_FONT_COLOR ), TRUE );
    Edit_Enable( GetDlgItem( hwnd, EDT_BUSINESS_NAME ), TRUE );
    Static_Enable( GetDlgItem( hwnd, STC_FONT_COLOR_TXT ), TRUE );
    Static_Enable( GetDlgItem( hwnd, STC_CUSTOM_TXT ), TRUE );

    // Set the limit of characters in the text box
    Edit_LimitText( GetDlgItem( hwnd, EDT_BUSINESS_NAME ), BUSINESS_NAME_MAX );

    // Set the text if they entered any
    Edit_SetText( GetDlgItem( hwnd, EDT_BUSINESS_NAME ), Settings.GetPtrBusinessName() );

    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS; ++i )
    {
        // Save the control handle for later use
        hSubCtrl[ i ] = GetDlgItem( hwnd, EDT_BUSINESS_NAME + i );

        // Subclass the windows control
        OldSubCtrlProc[ i ] = (WNDPROC)SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, 
                                                      LONG( SubClassProc ) );
    }

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

            // Load the graphics after the window has popped up
            LoadArtResources( hwnd );

            // Build the background
            BuildBackground( hwnd );

            // Show the helpful hint window
            if( Settings.GetIntroHelpWnd() )
            {
                BOOL ShowNextTime;
                DisplayHelpHintDialogBox( hInst, hwnd, "settings", "TXT", "Screen Saver Settings", "Helpful Hint", &ShowNextTime, DO_CHECK_SUM );
                Settings.SetIntroHelpWnd( ShowNextTime );
            }

            // Force a paint
            RepaintPreviewWnd( hwnd );
            
        break;

        case CHK_BUBBLES:
        case CHK_BACK_FISH:

            if( BufferReadyToBlit )
            {
                // Build the background
                BuildBackground( hwnd );

                // Force a paint
                RepaintPreviewWnd( hwnd );
            }

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

        case BTN_WEB_SITE:
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
        break;

        case BTN_FONT_SELECT:
            SetTextFont( hwnd );
        break;

        case BTN_FONT_COLOR:
            SetTextColor( hwnd );
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
*    FUNCTION NAME:         OnShowWindow                                                             
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
    if( hScrollBar == GetDlgItem( hwnd, SCRBAR_FISH_COUNT ) )
    {
        // Get the handle to the static text control
        hControl = GetDlgItem( hwnd, TXT_FISH_COUNT );

        switch( ScrollPos )
        {
            case 0: 
                Static_SetText( hControl, "Some Fish" );
            break;
            case 1: 
                Static_SetText( hControl, "More Fish" );
            break;
            case 2: 
                Static_SetText( hControl, "Even More Fish" );
            break;
            case 3: 
                Static_SetText( hControl, "Lots Of Fish" );
            break;
            case 4: 
                Static_SetText( hControl, "All The Fish" );
            break;
        }

        if( BufferReadyToBlit )
        {
            // Build the background
            BuildBackground( hwnd );

            // Force a paint
            RepaintPreviewWnd( hwnd );
        }
    }

}   // Config_SetScrollBarLabels



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
    // Save the position of the speed bar
    Settings.SetFishCount( ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_FISH_COUNT ) ) );

    // Save the check boxes
    Settings.SetBubblesOnOff( Button_GetCheck( GetDlgItem( hwnd, CHK_BUBBLES ) ) );
    Settings.SetBackFishOnOff( Button_GetCheck( GetDlgItem( hwnd, CHK_BACK_FISH ) ) );

    // Save the random music settings
    Settings.SetRandomMusic( Button_GetCheck( GetDlgItem( hwnd, CHK_RAND_MUSIC ) ) );

    // Get the business name they keyed in
    Edit_GetText( GetDlgItem( hwnd, EDT_BUSINESS_NAME ), Settings.GetPtrBusinessName(), sizeof(char)*(BUSINESS_NAME_MAX+1) );

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

}   // Config_GetSoundDirectory


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

void TConfigDlg::SetTextFont( HWND hwnd )
{
    CHOOSEFONT cf;
    LOGFONT    lf;

    // Memset the two structures
    memset( &cf, 0, sizeof( CHOOSEFONT ) );

    // Init with the current logfont
    memcpy( &lf, Settings.GetLogFont(), sizeof( LOGFONT ) );

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

        // Seset the font
        canvas.SetFont( NULL, 0, 0, Settings.GetTxtColor(), 0, Settings.GetLogFont() );

        if( BufferReadyToBlit )
        {
            // Build the background
            BuildBackground( hwnd );

            // Force a paint
            RepaintPreviewWnd( hwnd );
        }
    }

}   // SetTextFont


/************************************************************************
*    FUNCTION NAME:         SetTextColor                                                             
*
*    DESCRIPTION:           Set the text color
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

void TConfigDlg::SetTextColor( HWND hwnd )
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
        // Save the selected color
        Settings.SetTxtColor( cc.rgbResult );

        // Set the color
        canvas.SetTextColorToDC( cc.rgbResult );

        if( BufferReadyToBlit )
        {
            // Build the background
            BuildBackground( hwnd );

            // Force a paint
            RepaintPreviewWnd( hwnd );
        }
    }

}   // SetTextColor


/************************************************************************
*    FUNCTION NAME:         BuildBackground                                                             
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

void TConfigDlg::BuildBackground( HWND hwnd )
{
    RECT sourceSRCAND, sourceSRCPAINT, source, rect;
    int Row, i, XOffset, FishCount, HalfBubbles;
    POINT point;

    // Build the back buffer
    if( SystemBitDepth > BIT_COUNT_8 )
    {
        if( !metafile.IsEmpty() )
            metafile.Draw( buffer.Handle(), &BufferRect );
        else
            BackSolidColor();   
    }
    else
        BackSolidColor();

    // Get the  offset
    XOffset = buffer.width() / pOfficeProps->count;

    // Blit the props randomly across the bottom
    // Only blit the odd props
    for( i = 0; i < pOfficeProps->count; ++i )
    {
        if( i % 2 )
        {
            point.x = (abs(XOffset - pOfficeProps->Art[i].Right) / 2) + (XOffset * i);
            point.y = buffer.height()- pOfficeProps->Art[i].Bottom - SAND_BAR_LOW_H;
            
            SetRect( &source, pOfficeProps->Art[i].Left, pOfficeProps->Art[i].Top + pOfficeProps->Art[i].Bottom, 
                              pOfficeProps->Art[i].Right, pOfficeProps->Art[i].Bottom );
            FishArt.SetCopyMode( SRCAND );
            FishArt.Draw( &buffer, &point, &source );
            FishArt.SetCopyMode( SRCPAINT );
            source.top = pOfficeProps->Art[i].Top;
            FishArt.Draw( &buffer, &point, &source );
        }
    }

    // Set the source rect
    SetRect( &sourceSRCAND, SAND_BAR_X, SAND_BAR_Y+SAND_BAR_H, SAND_BAR_W, SAND_BAR_H );
    SetRect( &sourceSRCPAINT, SAND_BAR_X, SAND_BAR_Y, SAND_BAR_W, SAND_BAR_H );

    // Set the Y offset
    point.y = buffer.height() - SAND_BAR_H;

    // Calculate the number of rows
    if( buffer.width() % SAND_BAR_W )
        Row = (buffer.width() / SAND_BAR_W) + 1;
    else
        Row = buffer.width() / SAND_BAR_W;

    // Blit the sand bar across the bottom
    for( i = 0; i < Row; ++i )
    {
        point.x = i * SAND_BAR_W;
        FishArt.SetCopyMode( SRCAND );
        FishArt.Draw( &buffer, &point, &sourceSRCAND );
        FishArt.SetCopyMode( SRCPAINT );
        FishArt.Draw( &buffer, &point, &sourceSRCPAINT );
    }

    // Blit the props randomly across the bottom
    // Only blit the even props
    for( i = 0; i < pOfficeProps->count; ++i )
    {
        if( i % 2 == 0 )
        {
            point.x = (abs(XOffset - pOfficeProps->Art[i].Right) / 2) + (XOffset * i);
            point.y = buffer.height()- pOfficeProps->Art[i].Bottom - (SAND_BAR_H / 2);
            
            SetRect( &source, pOfficeProps->Art[i].Left, pOfficeProps->Art[i].Top + pOfficeProps->Art[i].Bottom, 
                              pOfficeProps->Art[i].Right, pOfficeProps->Art[i].Bottom );
            FishArt.SetCopyMode( SRCAND );
            FishArt.Draw( &buffer, &point, &source );
            FishArt.SetCopyMode( SRCPAINT );
            source.top = pOfficeProps->Art[i].Top;
            FishArt.Draw( &buffer, &point, &source );
        }
    }

    // Blit the props randomly across the bottom
    for( i = 0; i < pFishProps->count; ++i )
    {
        point.x = pFishPropXOffset[i];
        point.y = buffer.height() - SAND_BAR_H - (pFishProps->Art[i].Bottom / 2);
        SetRect( &source, pFishProps->Art[i].Left, pFishProps->Art[i].Top + pFishProps->Art[i].Bottom, 
                          pFishProps->Art[i].Right, pFishProps->Art[i].Bottom );
        FishArt.SetCopyMode( SRCAND );
        FishArt.Draw( &buffer, &point, &source );
        FishArt.SetCopyMode( SRCPAINT );
        source.top = pFishProps->Art[i].Top;
        FishArt.Draw( &buffer, &point, &source );
    }

    // Blit the background fish
    if( Button_GetCheck( GetDlgItem( hwnd, CHK_BACK_FISH ) ) )
    {
        for( i = 0; i < BackFishCount; ++i )
        {
            FishArt.SetCopyMode( SRCAND );
            rect = backFish[i].Source;
            rect.top += rect.bottom;
            source = backFish[i].Source;
            source.left += source.right * backFish[i].FrameCounter;
            rect.left += rect.right * backFish[i].FrameCounter;
            FishArt.Draw( &buffer, &backFish[i].Dest, &rect );
            FishArt.SetCopyMode( SRCPAINT );
            FishArt.Draw( &buffer, &backFish[i].Dest, &source );        
        }
    }

    // Get half of the bubbles
    HalfBubbles = BubbleCount / 2;

    // Blit the first half of the bubblesbubbles
    if( Button_GetCheck( GetDlgItem( hwnd, CHK_BUBBLES ) ) )
    {
        for( i = 0; i < HalfBubbles; ++i )      
            FishArt.Draw( &buffer, &pBubble[i].Dest, &pBubble[i].Art );
    }

    // Output the text to the buffer
    canvas.WrapText( Settings.GetPtrBusinessName(), 0, buffer.height()/6, buffer.width(), 
                     buffer.height() - (buffer.height()/6), 0, CENTER_JUSTIFY );

    // Get the number of fish needed
    FishCount = (ScrollBar_GetPos( GetDlgItem( hwnd, SCRBAR_FISH_COUNT ) )+1) * MIN_REQUIRED_FISH;

    // Blit the fish
    for( i = 0; i < FishCount; ++i )
    {
        FishArt.SetCopyMode( SRCAND );
        rect = fish[i].Source;
        rect.top += rect.bottom;
        source = fish[i].Source;
        source.left += source.right * fish[i].FrameCounter;
        rect.left += rect.right * fish[i].FrameCounter;
        FishArt.Draw( &buffer, &fish[i].Dest, &rect );
        FishArt.SetCopyMode( SRCPAINT );
        FishArt.Draw( &buffer, &fish[i].Dest, &source );
    }

    // Blit the last half of the bubbles
    if( Button_GetCheck( GetDlgItem( hwnd, CHK_BUBBLES ) ) )
    {
        for( i = HalfBubbles; i < BubbleCount; ++i )
            FishArt.Draw( &buffer, &pBubble[i].Dest, &pBubble[i].Art );
    }

    // Set the flag that indicates blitting is ready
    BufferReadyToBlit = TRUE;

}   // BuildBackground


/************************************************************************
*    FUNCTION NAME:         LoadResources                                                             
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

void TConfigDlg::LoadArtResources( HWND hwnd )
{
    TLoadRes LoadRes;
    int i, count=0;
    BOOL Result = FALSE;
    TGif gif;           // GIF decoder which can load GIFs from file or resource

    /**********************************
    *   Load Artwork Offset Data
    ***********************************/

    // Load the background fish art offsets from the resource
    if( !LoadTheResource( Instance(), "BackFishData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish data
    pBackFish = (PTFishDataArry)LoadRes.pData;

    // Load the fish art offsets from the resource
    if( !LoadTheResource( Instance(), "FishData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish data
    pFish = (PTFishDataArry)LoadRes.pData;

    // Load the bubble art offsets from the resource
    if( !LoadTheResource( Instance(), "BubbleData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish data
    bubble = (PTRectArry)LoadRes.pData;

    // Load the office props art offsets from the resource
    if( !LoadTheResource( Instance(), "OfficePropsData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the office props data
    pOfficeProps = (PTRectArry)LoadRes.pData;

    // Load the fish props art offsets from the resource
    if( !LoadTheResource( Instance(), "FishPropsData", RT_RCDATA, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get a pointer to the fish props data
    pFishProps = (PTRectArry)LoadRes.pData;



    /**********************************
    *   Load Artwork files
    ***********************************/

    // Load the graduated background no matter what color depth the system is in
    if( !metafile.LoadFromResource( Instance(), "gradback", "WMF" ) )
        goto HANDLE_ERROR;

    // For our needs, just load the 256 color artwork
    if( !FishArt.LoadFromBuffer( gif.LoadFromResource( "fish256", "GIF", Instance() ) ) ) 
        goto HANDLE_ERROR;

    if( !FishArt.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_24 ) )
        goto HANDLE_ERROR;

    // Force it to generate a palette if in 256 color mode before we delete
    // the bitmap bits
    if( SystemBitDepth <= 8 )
        FishArt.GetPalette();

    // Delete the bitmap bits because we don't need them any more
    FishArt.DeleteBitmapBits();

    // Set the buffer rect
    SetRect( &BufferRect, 0, 0, GetScreenWidth(), GetScreenHeight() );

    // Create the buffer
    if( !buffer.CreateDIBSectionBuffer( BufferRect.right, BufferRect.bottom, BIT_COUNT_24 ) )
        goto HANDLE_ERROR;

    // Give the canvas class the active DC
    canvas.InitActiveDC( buffer.Handle() );
    // Create the font
    canvas.SetFont( NULL, 0, 0, Settings.GetTxtColor(), 0, Settings.GetLogFont() );
    // Set the background to transparent
    canvas.SetBackgroundMode( TRANSPARENT );


    /**********************************
    *   Setup the fish data
    ***********************************/

    // Allocate the fish data
    fish = new TFish[ pFish->count ];

    if( !fish )
        goto HANDLE_ERROR;

    // init the structure to all zeros
    memset( fish, 0, sizeof(TFish)*pFish->count );

    // Set all the fish
    for( i = 0; i < pFish->count; ++i )
    {
        // Copy over the art offsets. The resource data is a short
        fish[i].Art.left   = pFish->FishData[i].Art.Left;
        fish[i].Art.top    = pFish->FishData[i].Art.Top;
        fish[i].Art.right  = pFish->FishData[i].Art.Right;
        fish[i].Art.bottom = pFish->FishData[i].Art.Bottom;

        // Copy the art settings to the sourec and dest
        fish[i].Source = fish[i].Art;
        fish[i].Dest = fish[i].Art;

        // Get the frame count
        fish[i].NumFrames = pFish->FishData[i].FrameCount;

        // Get the fish is on the bottom only
        fish[i].BottomOnly = pFish->FishData[i].BottomOnly;

        // Set the destination offset
        fish[i].Dest.left = rand() % ( buffer.width() - fish[i].Art.right );

        if( fish[i].BottomOnly )
            fish[i].Dest.top  = buffer.height() - fish[i].Art.bottom - ((rand() % MAX_BOTTOM_EDGE)+MIN_BOTTOM_EDGE);
        else
            fish[i].Dest.top  = (rand() % ( buffer.height() - fish[i].Art.bottom - SAND_BAR_H - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;
        
        // Update the source rect so they can move left
        if( rand() % MAX_NUM_OF_MOVEMENTS == MOVE_LEFT )
            // The reverse art is to the left
            fish[i].Source.left = fish[i].Art.left + (fish[i].Art.right * fish[i].NumFrames);
    }


    /**********************************
    *   set the fish prop offsets
    ***********************************/

    // Record the random fish props
    pFishPropXOffset = new int[ pFishProps->count ];

    if( !pFishPropXOffset )
        goto HANDLE_ERROR;

    for( i = 0; i < pFishProps->count; ++i )
        pFishPropXOffset[i] = rand() % (buffer.width() - pFishProps->Art[i].Right);



    /**********************************
    *   Setup the background fish data
    ***********************************/

    // Get the number of background fish
    BackFishCount = (buffer.width() / 128) * pBackFish->count;

    // Allocate the back fish array
    backFish = new TFish[ BackFishCount ];

    if( !backFish )
        goto HANDLE_ERROR;

    // init the structure to all zeros
    memset( backFish, 0, sizeof(TFish)*BackFishCount );

    for( i = 0; i < BackFishCount; ++i )
    {
        // Copy over the art offsets. The resource data is a short
        backFish[i].Art.left   = pBackFish->FishData[ count ].Art.Left;
        backFish[i].Art.top    = pBackFish->FishData[ count ].Art.Top;
        backFish[i].Art.right  = pBackFish->FishData[ count ].Art.Right;
        backFish[i].Art.bottom = pBackFish->FishData[ count ].Art.Bottom;

        // Copy the art settings to the sourec and dest
        backFish[i].Source = backFish[i].Art;
        backFish[i].Dest = backFish[i].Art;

        // Get the frame count
        backFish[i].NumFrames = pBackFish->FishData[ count ].FrameCount;

        // Randomize the X position
        backFish[i].Dest.left = rand() % ( buffer.width() - backFish[i].Art.right );

        // Randomize the Y position
        backFish[i].Dest.top  = (rand() % ( buffer.height() - backFish[i].Art.bottom - BOTTOM_PROP_OFFSET - (MAX_UP_DOWN_OFFSET*2)))+MAX_UP_DOWN_OFFSET;

        // If the fish is on the bottom half of the screen use the darker fish
        if( backFish[i].Dest.top > buffer.height() / 2 )
            backFish[i].Source.top += DARK_FISH_OFFSET;

        // Update the source rect so they can move left
        if( rand() % MAX_NUM_OF_MOVEMENTS == MOVE_LEFT )
            // The reverse art is to the left
            backFish[i].Source.left = backFish[i].Art.left + (backFish[i].Art.right * backFish[i].NumFrames);

        // Update the count
        count = ( count + 1 ) % pBackFish->count;
    }


    /**********************************
    *   Setup the bubble data
    ***********************************/

    count = 0;

    // How many bubbles do we need?
    BubbleCount = (buffer.width() / 128) * bubble->count;

    // Allocate the bubble array
    pBubble = new TBubble[ BubbleCount ];

    if( !pBubble )
        goto HANDLE_ERROR;

    for( i = 0; i < BubbleCount; ++i )
    {
        // Copy over the art offsets. The resource data is a short
        pBubble[i].Art.left   = bubble->Art[count].Left;
        pBubble[i].Art.top    = bubble->Art[count].Top;
        pBubble[i].Art.right  = bubble->Art[count].Right;
        pBubble[i].Art.bottom = bubble->Art[count].Bottom;

        // Copy the art settings to the sourec and dest
        pBubble[i].Dest = pBubble[i].Art;

        // Set the Y offset
        pBubble[i].Dest.top = rand() % ( buffer.height() - SAND_BAR_H ); // pBubble[i].Art.bottom

        // Set the X offset
        pBubble[i].Dest.left  = (rand() % ( buffer.width() - pBubble[i].Art.right - (MAX_LEFT_RIGHT_OFFSET*2)))+MAX_LEFT_RIGHT_OFFSET;

        // Inc the counter
        count = (count + 1) % bubble->count;
    }


    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        PostMsg( "Error", "Error loading graphics" );

}   // BuildBackground


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

LRESULT TConfigDlg::HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;

    /////////////////////////////////////////////////////
    // Each subclassed control has it's own message handler.
    // That's to keep this function as general and simple
    // as possible. You dig?
    /////////////////////////////////////////////////////

    if( hCtrl == hSubCtrl[ EDT_BUS_NAME ] )
        Result = OnEdit_BusinessName( hParentWnd, EDT_BUS_NAME, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ STC_PREVIEW_WND ] )
        Result = OnPreviewWnd( hParentWnd, STC_PREVIEW_WND, Message, wParam, lParam );

    return Result;

}   // HandleSubClassProc


/************************************************************************
*    FUNCTION NAME:         OnEdit_BusinessName                                                             
*
*    DESCRIPTION:           Subclassed edit control
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

LRESULT TConfigDlg::OnEdit_BusinessName( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;

    // Kill the keyboard messages
    if( Message == WM_KEYUP || wParam == RETURN_KEY )
    {
        // Get the text
        Edit_GetText( hSubCtrl[ Ctrl ], Settings.GetPtrBusinessName(), sizeof(char)*(BUSINESS_NAME_MAX+1) );

        if( BufferReadyToBlit )
        {
            // Build the background
            BuildBackground( hwnd );

            // Force a paint
            RepaintPreviewWnd( hwnd );
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_BusinessName


/************************************************************************
*    FUNCTION NAME:         OnPreviewWnd                                                             
*
*    DESCRIPTION:           Subclassed static control
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

LRESULT TConfigDlg::OnPreviewWnd( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT rect;
    LRESULT Result = FALSE;
    HPALETTE hOldPalette = NULL;

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

        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256.
        if( SystemBitDepth <= 8 && BufferReadyToBlit )
        {
            // Save the old palette so that we can select it back in before
            // deleting the palette we created. DeleteObject doesn't seem to
            // work if your trying to delete a palette that is still in the DC
            hOldPalette = SelectPalette( PaintDC, FishArt.GetPalette(), FALSE );
            RealizePalette( PaintDC );
        }

        // Blit the buffer to the view window
        if( BufferReadyToBlit )
            buffer.StretchDraw( PaintDC, &rect );

        // Reselect the old palette back into the DC so that the palette can be deleted
        if( hOldPalette )
            SelectPalette( PaintDC, hOldPalette, FALSE );

        // Must be called
        EndPaint( hSubCtrl[ Ctrl ], &paintStruct );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnPreviewWnd


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

void TConfigDlg::OnDestroy( HWND hwnd ) 
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

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         RepaintPreviewWnd                                                             
*
*    DESCRIPTION:           Force a repaint onto the preview window
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

void TConfigDlg::RepaintPreviewWnd( HWND hwnd )
{
    RECT DisplayRect;

    // Get the rect of our view window
    GetClientRect( hSubCtrl[ STC_PREVIEW_WND ], &DisplayRect );

    // Invalidate the whole window so it all gets painted
    InvalidateRect( hSubCtrl[ STC_PREVIEW_WND ], &DisplayRect, TRUE );

}   // RepaintViewWnd


/************************************************************************
*    FUNCTION NAME:         BackSolidColor                                                             
*
*    DESCRIPTION:           Setup the background with a solid color.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TConfigDlg::BackSolidColor()
{
    int Row, Col, i, j;
    RECT BrushRect;
    POINT point;

    // Calculate the number of rows and colums
    if( buffer.width() % BACK_COLOR_SIZE )
        Row = (buffer.width() / BACK_COLOR_SIZE) + 1;
    else
        Row = buffer.width() / BACK_COLOR_SIZE;

    if( buffer.height() % BACK_COLOR_SIZE )
        Col = (buffer.height() / BACK_COLOR_SIZE) + 1;
    else
        Col = buffer.height() / BACK_COLOR_SIZE;

    // Set the rect to blit the solid color background
    SetRect( &BrushRect, BACK_COLOR_X, BACK_COLOR_Y, BACK_COLOR_SIZE, BACK_COLOR_SIZE );

    // Set the copy mode
    FishArt.SetCopyMode( SRCCOPY );

    // Tile the bitmat to the buffer
    for( j = 0; j < Col; ++j )
    {
        for( i = 0; i < Row; ++i )
        {
            // Init the point
            point.x = BACK_COLOR_SIZE * i;
            point.y = BACK_COLOR_SIZE * j;

            // Blit the back color
            FishArt.Draw( &buffer, &point, &BrushRect );
        }
    }

}   // BackSolidColor


/************************************************************************
*    FUNCTION NAME:         GetSystemBitDepth                                                             
*
*    DESCRIPTION:           Get the bit depth of the system from a windows handle
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int GetSystemBitDepth( HWND hwnd )
{
    int SystemBitDepth;
    HDC hDC;

    // Get the screen DC
    hDC = GetDC( hwnd );

    // Get the bit depth of the system, be it 8, 16, 24, 32
    SystemBitDepth = GetDeviceCaps( hDC, BITSPIXEL );

    // Now release the DC
    ReleaseDC( hwnd, hDC );

    return SystemBitDepth;

}   // GetSystemBitDepth