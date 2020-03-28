/************************************************************************
*
*    PRODUCT:         Program Installer
*
*    FILE NAME:       Install.cpp
*
*    DESCRIPTION:     Program for installing my programs
*
*    IMPLEMENTATION:  This version is currently setup to install screen
*                     savers but can be updated to install any application
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "Install.h"


// Globals
PTInstallS2G pInstallS2G;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         WndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler. I make good
*                           use of WindowsX macros here. It helps keep things
*                           simple and elegant instead of doing your own
*                           message cracking. The WindowsX macros add nothing
*                           to the size of your program.
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
*    HCR           10/23/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        HANDLE_MSG( hwnd, WM_DESTROY,   pInstallS2G->OnDestroy );
        HANDLE_MSG( hwnd, WM_CREATE,    pInstallS2G->OnCreate );
        HANDLE_MSG( hwnd, WM_PAINT,     pInstallS2G->OnPaint );
        HANDLE_MSG( hwnd, WM_COMMAND,   pInstallS2G->OnCommand );
        HANDLE_MSG( hwnd, WM_NCDESTROY, pInstallS2G->OnNCDestroy );
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

}  // WndProc


/************************************************************************
*    FUNCTION NAME:         InstallingDlg                                                             
*
*    DESCRIPTION:           Simply Dialog Box message handler.  The dialog
*                           box is very simple message and has no messages to
*                           really handle.
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
*    HCR           10/23/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CALLBACK InstallingDlg(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
    return FALSE;

}   // InstallingDlg


/************************************************************************
*    FUNCTION NAME:         WinMain()                                                             
*
*    DESCRIPTION:           Main procedure for starting a windows program
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - Instance of this program
*              HINSTANCE hPrevInstance - Worthless in 32 bit windows programs
*              LPSTR lpszCmdParam - The command line is sent by Windows. The command
*                                   originally has the path to this program but
*                                   Windows parses it out before it hands it over.
*                                   To get the command line with the path in it you
*                                   need to call GetCommandLine() API call.
*              int nCmdShow - Specifies how the window is to be shown.
*                             Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED   
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                            LPSTR lpszCmdParam, int nCmdShow )
{
    MSG Msg;
    BOOL Result = FALSE;

    // Create the parent window object
    pInstallS2G = new TInstallS2G();

    // Create the window
    if( pInstallS2G )
        Result = pInstallS2G->CreateParentWnd( hInst, nCmdShow );

    // Kick out of the program if there has been an error
    if( !Result )
        return Result;

    while( GetMessage( &Msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &Msg );
        DispatchMessage( &Msg );
    }

    return Msg.wParam;

}   // WinMain


/************************************************************************
*
*                      Object Related Functions
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

TInstallS2G::TInstallS2G()
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

TInstallS2G::~TInstallS2G()
{
}   // Destructer


/************************************************************************
*    FUNCTION NAME:         CreateParentWnd()                                                             
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

BOOL TInstallS2G::CreateParentWnd( HINSTANCE hInst, int nCmdShow )
{
    BOOL Result = FALSE;

    // Parse the path from the command line to get the path to the install program
    ParsePathFromCmdLine( GetCommandLine(), ExePath );

    // We are in debug mode and are loading data from the default file name.
    // This causes the program to open the other file on the hard drive
    // instead of itself which allows us to step through it in the debugger
    #ifdef VIEW_DEBUG

    char FilePath[MAX_PATH];

    // We are asuming this file has bee generated by the tool
    CropOutPath( ExePath, FilePath );
    strcat( FilePath, "\\" );
    strcat( FilePath, PRO_EXAMPLE_EXE_NAME );
    strcpy( ExePath, FilePath );

    #endif

    // Reads in the file header, the preferences and all the TPage data
    if( !OpenEXEandReadHeader( ExePath ) )
        goto HANDLE_ERROR;

    #ifndef VIEW_DEBUG

    // check sum this install file. The result should be zero. If not, the file
    // was damaged or tampered with
    if( CheckSumFile( ExePath ) != 0 )
    {
        MessageBox(0, "This install program is damaged. Downloading files from the web or an online service can get cut off in the middle or corrupted durring the download. Try downloading this program again and if the problem continues, download again at a later time.",
            "Install Error", MB_OK);
        goto HANDLE_ERROR;
    }
    #endif

    // Save the instance to a global variable for later use
    gInst = hInst;

    // Register the parent window with Windows
    if( !Register(hInst) )
        goto HANDLE_ERROR;

    // Create the parent window
    if( !Create(hInst, nCmdShow) )
        goto HANDLE_ERROR;

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // CreateParentWnd


/************************************************************************
*    FUNCTION NAME:         RegisterWnd                                                             
*
*    DESCRIPTION:           All custom windows created with CreateWindow
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TInstallS2G::Register( HINSTANCE hInst )
{
    WNDCLASS W;

    W.style = CS_HREDRAW | CS_VREDRAW;
    W.lpfnWndProc = WndProc;
    W.cbClsExtra = 0;
    W.cbWndExtra = 0;
    W.hInstance = hInst;
    W.hIcon = LoadIcon(hInst, "AppIcon");
    W.hCursor = LoadCursor(NULL, IDC_ARROW);
    W.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW+1);
    W.lpszMenuName = "Menu_1";
    W.lpszClassName = INSTCLASSNAME;

    return (RegisterClass(&W) != 0);

}   // Register


/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           If we are running the preview or a full screen
*                           window, the window is created here.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TInstallS2G::Create( HINSTANCE hInst, int nCmdShow )
{
    // Create the parent window
    HWND hwnd = CreateWindow( INSTCLASSNAME, "Installer", WS_CAPTION | WS_SYSMENU,
                              CW_USEDEFAULT,CW_USEDEFAULT,
                              CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInst,NULL);

    // Return false if we had a problem
    if( hwnd == NULL )
        return FALSE;

    // Show the parent window
    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );

    return TRUE;
}   // Create


/************************************************************************
*    FUNCTION NAME:         OnDestroy()                                                             
*
*    DESCRIPTION:           Handles the WM_DESTROY windows message. This
*                           message is generated when making the API call
*                           to DestroyWindow() or clicking on the little
*                           icon in the right corner of the window.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::OnDestroy( HWND hwnd )
{
    QuitProgram( hwnd );

}   // OnDestroy


/************************************************************************
*    FUNCTION NAME:         OnPaint()                                                             
*
*    DESCRIPTION:           Handles the WM_PAINT windows message. Bad things
*                           will happen if your program is handling WM_PAINT
*                           message and not doing any painting. If you don't
*                           need to paint anything, keep the WM_PAINT message 
*                           out of your WndProc.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::OnPaint( HWND hwnd )
{
    RECT rect;
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    HPALETTE hOldPalette = NULL;

    // Get the rect of our parent window
    GetClientRect(hwnd, &rect);

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Check to see that it's not empty
    if( !installDIB.IsEmpty() )
    {
        // You have to realize the palette each time you get a WM_PAINT message
        // or the bitmap will be drawn without it's colors. This does not apply
        // if the computer is in a higher color depth than 256. For this purpose
        // it wont hurt anything if the computer is in higher colors than 256.
        if( GetDeviceCaps( PaintDC, BITSPIXEL ) <= 8 )
        {
            hOldPalette = SelectPalette( PaintDC, installDIB.GetPalette(), FALSE ); 
            RealizePalette( PaintDC );
        }

        // API call that uses the BITMAPINFO structure and a pointer to the bitmaps
        // bits to blit the image to the window. This is not the fastest way to blit
        // images and wouldn't be my first choice for animation, but for this purpose,
        // it works fine. Even though this function is called StretchDIBits, we are not
        // doing any stretching.
        StretchDIBits( PaintDC,
        (rect.right - installDIB.width()) / 2, 11, installDIB.width(), installDIB.height(),
        0,0,installDIB.width(), installDIB.height(),
            installDIB.bits(), installDIB.dib(), DIB_RGB_COLORS,SRCCOPY );

        if( hOldPalette )
        {
            SelectPalette( PaintDC, hOldPalette, FALSE ); 
            RealizePalette( PaintDC );
        }
    }

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // OnPaint


/************************************************************************
*    FUNCTION NAME:         OnCommand()                                                             
*
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*                           We are handling our app specific messages for
*                           our buttons and what not.
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

void TInstallS2G::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {
        case ID_FILE_INSTALL:
            InstallProgram( hwnd );
        break;

        case ID_FILE_UNINSTALL:
            UninstallProgram( hwnd );
        break;

        case ID_MISC_ABOUT:
            ShowMessageBox( gInst, hwnd, ABOUT_WINDOW_INFO, ABOUT_WINDOW_CAPTION, MB_OK );
        break;

        case ID_FILE_EXIT:
            DestroyWindow( hwnd );
        break;

        case ID_BTN_EXIT:
            DestroyWindow( hwnd );
        break;

        case ID_BTN_INSTALL:
            InstallProgram( hwnd );
        break;

        case ID_BTN_UNINSTALL:
            UninstallProgram( hwnd );
        break;

        case MNU_WEB_LINK:
            ShellExecute( NULL, "open", WEBSITE, NULL, NULL, SW_SHOWNORMAL );
        break;
    }
}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         QuitProgram()                                                             
*
*    DESCRIPTION:           This function gives us one place to end the
*                           program if we need to do aditional cleanup.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::QuitProgram( HWND hwnd )
{
    PostQuitMessage( 0 );
}   // QuitProgram


/************************************************************************
*    FUNCTION NAME:         Install_OnCreate()                                                             
*
*    DESCRIPTION:           Standard Windows OnCreate message handler. Here
*                           we do our nessary initializations.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   BOOL - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TInstallS2G::OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct )
{
    char WndTitle[MAX_TITLE+10];

    // Load up our strings, screen bitmap and get the windows directory
    initStartUp( hwnd );

    // Setup all the buttons
    initParentWnd( hwnd );

    // Setup the title
    strcpy( WndTitle, ProjPrefsData.Title );
    strcat( WndTitle, " - Installer" );

    // Set the window caption to the name of the screen saver
    SetWindowText( hwnd, WndTitle );

    return TRUE;

}   // Install_OnCreate


/************************************************************************
*    FUNCTION NAME:         initParentWnd()                                                             
*
*    DESCRIPTION:           Loading strings from the resource file and
*                           creating the apps controls
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::initParentWnd( HWND hwnd )
{
    char strBuffer[50];
    const int margin = 10;
    int height, width;

    control btnInst = {"button", "", 0, 10, 20, 0, 0};
    control btnUninst = {"button", "", 0, 152, 20, 0, 0};
    control btnExit = {"button", "", 0, 318, 20, 0, 0};

    // Load Control caption text from resource
    LoadString( gInst, INSTALL_BUTTON, strBuffer, sizeof(strBuffer));
    strcpy(btnInst.caption, strBuffer);

    LoadString( gInst, UNINSTALL_BUTTON, strBuffer, sizeof(strBuffer));
    strcpy(btnUninst.caption, strBuffer);

    LoadString( gInst, EXIT_BUTTON, strBuffer, sizeof(strBuffer));
    strcpy(btnExit.caption, strBuffer);

    // Use control struct to create window controls
    sizeButtonWidthHeight(hwnd, btnInst);
    btnInst.hwnd = CreateWindow(btnInst.type, btnInst.caption,
             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
             btnInst.x, btnInst.y + installDIB.height(), btnInst.w, btnInst.h,
             hwnd, HMENU(ID_BTN_INSTALL), gInst, NULL);

    if( btnInst.hwnd )
        ApplyWin95Font(btnInst.hwnd); // Get the font for this button

    // Create Install button
    sizeButtonWidthHeight(hwnd, btnUninst);
    btnUninst.x = btnInst.x + btnInst.w + margin;
    btnUninst.hwnd = CreateWindow(btnUninst.type, btnUninst.caption,
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
             btnUninst.x, btnUninst.y + installDIB.height(), btnUninst.w, btnUninst.h,
             hwnd, HMENU(ID_BTN_UNINSTALL), gInst, NULL);

    if( btnUninst.hwnd )
        ApplyWin95Font(btnUninst.hwnd); // Get the font for this button


    // Create Exit button
    sizeButtonWidthHeight(hwnd, btnExit);
    btnExit.x = btnUninst.x + btnUninst.w + (margin * 2);
    btnExit.hwnd = CreateWindow(btnExit.type, btnExit.caption,
                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
             btnExit.x, btnExit.y + installDIB.height(), btnExit.w, btnExit.h,
             hwnd, HMENU(ID_BTN_EXIT), gInst, NULL);

    if( btnExit.hwnd )
        ApplyWin95Font(btnExit.hwnd); // Get the font for this button

    // Size and center window this window
    height = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYSIZEFRAME);
    width = btnExit.x + btnExit.w + GetSystemMetrics(SM_CXSIZEFRAME) + margin;
    CenterWindow(hwnd, width, height + btnExit.y + btnExit.h + margin  + installDIB.height());

}   // initParentWnd


/************************************************************************
*    FUNCTION NAME:         initStartUp()                                                             
*
*    DESCRIPTION:           Get the windows and system directories and load
*                           some strings from the resource file.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::initStartUp( HWND hwnd )
{
    // Get the Windows directory path
    if( GetWindowsDirectory( WinDir, sizeof(char)*(MAX_PATH+1) ) > 3 )
    {
        if( WinDir[ strlen(WinDir)-1 ] != '\\' )
            lstrcat( WinDir, "\\" ); // add back slash for end of directory
    }

    // Setup the name of the screen saver
    strcpy( ScrFileName, ProjPrefsData.Title );
    strcat( ScrFileName, ".scr" );

    // Setup the path to the screen saver file
    strcpy( ScrPath, WinDir );
    strcat( ScrPath, ScrFileName );

    // Load up the graphic for the window
    if( !(installDIB.loadFromResource( gInst, "installBMP", RT_BITMAP, USE_RESOURCE_POINTER )) )
    {
        ShowMessageBox( gInst, hwnd, PARENT_BITMAP_ERROR, RESOURCE_ERROR_TITLE,
                        MB_OK|MB_ICONEXCLAMATION );
        return;
    }

} // initStartUp


/************************************************************************
*    FUNCTION NAME:         InstallProgram()                                                             
*
*    DESCRIPTION:           Pull the file to be installed out of the
*                           resource and uncompress it.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::InstallProgram( HWND hwnd )
{
    HWND hInstallDlg;

    // Ask the user what they want to do because the program in already located
    // on their computer.
    if( SearchPath( WinDir, ScrFileName, NULL, NULL, NULL, NULL ) )
    {
        if( ShowMessageBox( gInst, hwnd, ALLREADY_INSTALLED, PROGRAM_INSTALLER,
                                MB_YESNO|MB_ICONEXCLAMATION) == IDYES )
            deleteProgram( hwnd, DONT_SHOW_MESSAGE );
        else
            return;
    }

    // Load Dialog Box that says "Installing"
    hInstallDlg = CreateDialog( gInst, "Installing", hwnd, (DLGPROC)InstallingDlg );

    // Install the program
    if( WriteSaverToHardDrive( hwnd ) )
    {
        // Depending on the size of the program the install can go too fast
        // so we need to slow it down
        Sleep( SLEEP_TIME );

        // Save the screensaver name as the default screen saver in windows
        SelectAsDefault( ScrPath );

        CheckChangeSaverDelay( "600" );

        // Installation is done so kill the "Install" dialog window
        if( hInstallDlg )
            DestroyWindow( hInstallDlg );

        // Show the Installation finished message window
        // Everythings good! Say thanks and be on our way
        ShowMessageBox( gInst, hwnd, SUCCESSFUL_INSTALL, INSTALL_FINISHED,
                     MB_OK|MB_ICONINFORMATION );

        // Install finished. Kill the program
        DestroyWindow( hwnd );

        // This runs the screen saver in a 
        ShellExecute( hwnd, "install", ScrPath, NULL, NULL, SW_SHOWNORMAL );
    }
    else
        MessageBox( GetActiveWindow(), "There has been an error installing Screen Savers.", "Install Error", MB_OK );

}   // InstallProgram


/************************************************************************
*    FUNCTION NAME:         CheckChangeRegistrySaverDelay()                                                             
*
*    DESCRIPTION:           If the delay on the screen saver is set to one
*                           or two minutes, it should be changed to atleast
*                           five or more because it upsets new users that
*                           the screen saver pops on all the time.
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
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::CheckChangeSaverDelay( char* DelayTime )
{
    int ScreenSaverDelay;

    // Get the time in seconds the screen saver delay is set to
    if( SystemParametersInfo( SPI_GETSCREENSAVETIMEOUT, 0, &ScreenSaverDelay, 0 ) )
    {
        // If it's less than 5 minutes, change it to ten minutes
        if( ScreenSaverDelay < 300 )
            SystemParametersInfo( SPI_SETSCREENSAVETIMEOUT, 600, NULL, 0 );
    }

}  // CheckChangeRegistrySaverDelay


/************************************************************************
*    FUNCTION NAME:         SelectAsDefault()                                                             
*
*    DESCRIPTION:           Set's the Windows System.ini file to default
*                           to our screen saver. This function would not
*                           be used if we were install something other
*                           than a screen saver.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::SelectAsDefault( char *scr )
{ 
    char sscr[MAX_PATH+1];

    // Generate a short path name for the INI file to use
    GetShortPathName( scr, sscr, sizeof(char)*(MAX_PATH+1) );

    // Write the new INI file settings
    WritePrivateProfileString( "boot", "SCRNSAVE.EXE", sscr, "system.ini" );

    // that sends a WM_WININICHANGE so that DisplayProperties dialog knows we've changed
    SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, TRUE, NULL, TRUE );
}


/************************************************************************
*    FUNCTION NAME:         deleteProgram()                                                             
*
*    DESCRIPTION:           This deletes the program on the users computer.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              BOOL inWinSys - Flag indicates the file is in the system dir
*              BOOL inWin - Flag indicates the file is in the windows dir
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::deleteProgram( HWND hwnd, BOOL fShowMsg )
{
    BOOL DeleteFlag = FALSE;

    // setup delete file string
    if( DeleteFile( ScrPath ) )
        DeleteFlag = TRUE;

    if( DeleteFlag && fShowMsg )
        ShowMessageBox( gInst, hwnd, SUCCESSFUL_UNINSTALL, UNINSTALL_FINISHED,
                         MB_OK|MB_ICONINFORMATION );
    else if( !DeleteFlag && fShowMsg )
            ShowMessageBox( gInst, hwnd, UNINSTALL_ERROR_MESSAGE, UNINSTALL_ERROR,
                         MB_OK|MB_ICONINFORMATION );

} // deleteProgram


/************************************************************************
*    FUNCTION NAME:         UninstallProgram()                                                             
*
*    DESCRIPTION:           Gives the user a chance to change their mind
*                           about uninstalling the program
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TInstallS2G::UninstallProgram( HWND hwnd )
{
    if( SearchPath( WinDir, ScrFileName, NULL, NULL, NULL, NULL ) )
    {
        // load messagebox string
        if ( ShowMessageBox( gInst, hwnd, QUESTION_UNINSTALL, PROGRAM_UNINSTALLER,
                             MB_YESNO|MB_ICONEXCLAMATION ) == IDYES)
            deleteProgram( hwnd, SHOW_MESSAGE );
    }
    else  // No files to delete
    {
        ShowMessageBox( gInst, hwnd, NO_PRODUCT_TO_UNSTALL, NOT_INSTALLED,
                        MB_OK|MB_ICONINFORMATION );
    }
}   // UninstallProgram


/************************************************************************
*    FUNCTION NAME:         Loader_OpenEXEandReadHeader                                                             
*
*    DESCRIPTION:           Open this applications exe file and read in
*                           the header information. 
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

BOOL TInstallS2G::OpenEXEandReadHeader( char *FileName )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    HANDLE hFile;
    BOOL FileReadWriteResult;
    BOOL Result = FALSE;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Open the file on the hard drive
    hFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                        &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        MessageBox( GetActiveWindow(), "Can't open application file on disk to get header info.", "Error", MB_OK );
        goto FREE_MEMORY;
    }

    // Set the file pointer to the end of the file and then back it up to read the file header
    SetFilePointer( hFile, -sizeof(TFileHeader), 0, FILE_END );

    // Read the file header which is at the end of the file
    FileReadWriteResult = ReadFile( hFile, &FileHeader, sizeof(TFileHeader), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TFileHeader) )
    {
        MessageBox( GetActiveWindow(), "Error reading in application file header.", "Error", MB_OK );
        goto FREE_MEMORY;
    }

    // Check that we have a proper formatID. This is just an exact number and if that
    // number is not the same, we are not looking at the data we should be.
    if( FileHeader.FormatID != FORMAT_ID )
    {
        MessageBox( GetActiveWindow(), "Header format ID does not match. Header format out of sync.", "Error", MB_OK );
        goto FREE_MEMORY;
    }

    // Set the file pointer to read in the applications preferences
    // This is where we use the data offset that is saved with the header
    SetFilePointer( hFile, -(FileHeader.FileSize - FileHeader.ProjectDataOffset), 0, FILE_END );

    // Read contents of preferences data into structure
    FileReadWriteResult = ReadFile( hFile, &ProjPrefsData, sizeof(TProjectPrefs), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TProjectPrefs) )
    {
        MessageBox( GetActiveWindow(), "Error reading in application preferences.", "Error", MB_OK );
        goto FREE_MEMORY;
    }

    // Check the preferences ID for vaild number
    if( ProjPrefsData.PrefsCheckID != PERFS_CHECK_ID )
    {
        MessageBox( GetActiveWindow(), "Preference ID does not match. Preference data out of sync.", "Error", MB_OK );
        goto FREE_MEMORY;
    }

    Result = TRUE;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // OpenEXEandReadHeader


/************************************************************************
*    FUNCTION NAME:         WriteSaverToHardDrive                                                             
*
*    DESCRIPTION:           Open this applications exe file and read in
*                           the header information. 
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

BOOL TInstallS2G::WriteSaverToHardDrive( HWND hwnd )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    HANDLE hSourceFile, hDestFile;
    BOOL FileReadWriteResult;
    BOOL Result = FALSE;
    DWORD FileReadSize;
    DWORD DataReadSize = 0;
    DWORD BufferSize;
    BYTE *pBuffer;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Check to make sure there is enough room on the hard drive for this program
    // If they don't have enough disk space then kill the install. File size + 50k
    if( !IsEnoughDiskSpace( ScrPath, FileHeader.FileSize ) )
    {
        ShowMessageBox( gInst, hwnd, NOT_ENOUGH_SPACE_ON_DRIVE, HARD_DRIVE_ERROR,
                            MB_OK|MB_ICONEXCLAMATION );
        goto HANDLE_ERROR;
    }

    // Open the install file on the hard drive
    hSourceFile = CreateFile( ExePath, GENERIC_READ, FILE_SHARE_READ,
                              &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Open the install file on the hard drive
    hDestFile = CreateFile( ScrPath, GENERIC_WRITE, FILE_SHARE_WRITE,
                            &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hSourceFile == INVALID_HANDLE_VALUE || hDestFile == INVALID_HANDLE_VALUE )
    {
        MessageBox( GetActiveWindow(), "Can't open application file to install screen saver.", "Error", MB_OK );
        goto HANDLE_ERROR;
    }

    // Set the file pointer to the end of the file and then back it up to read the screen saver file
    SetFilePointer( hSourceFile, -FileHeader.FileSize, 0, FILE_END );

    BufferSize = 524288; // half meg
    pBuffer = new BYTE[BufferSize];
    if( !pBuffer )
    {
        MessageBox( GetActiveWindow(), "Memory allocation error during install.", "Error", MB_OK );
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
        FileReadWriteResult = ReadFile( hSourceFile, pBuffer, FileReadSize, &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != FileReadSize )
        {
            MessageBox( GetActiveWindow(), "Error reading file for install.", "Error", MB_OK );
            goto HANDLE_ERROR;
        }

        // Write the amount from the buffer to the destination file
        FileReadWriteResult = WriteFile( hDestFile, pBuffer, FileReadSize, &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != FileReadSize )
        {
            MessageBox( GetActiveWindow(), "Error writing install file.", "Error", MB_OK );
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

    // Close the file handles
    if( hSourceFile != INVALID_HANDLE_VALUE )
        CloseHandle( hSourceFile );

    if( hDestFile != INVALID_HANDLE_VALUE )
        CloseHandle( hDestFile );

    return Result;

}   // WriteSaverToHardDrive