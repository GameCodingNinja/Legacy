
/************************************************************************
*
*    PRODUCT:         eBook
*
*    FILE NAME:       eBook.cpp
*
*    DESCRIPTION:     The begining
*
*    IMPLEMENTATION: How it's done 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "GDefines.h"        // Header file with #defines
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include "WinMacros.h"       // My windows macros that should been included with windowsx.h
#include "YourInstDlg.h"     // Shows a dialog box that informs them on what file is theres to distribute

#include "gTools.h"          // A misc group of useful functions
#include "ebook.h"           // Header file for this *.cpp file.
//#include "ViewWnd.h"         // Window for blitting images to.
#include "TStartUp.h"        // Header file for the startup screen window
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer
#include "AboutDlg.h"        // About dialog box
#include "PrintCnl.h"        // Print Cancel dialog box
#include "TJPEG.h"           // JPEG Library
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource

#if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
#include "RegNum.h"          // Registration Number dialog box
#include "OrderFrm.h"        // Order form
#include "OrderOpt.h"        // Dialog the asks how you want to order
#endif

#ifdef EBOOK_TOOL
#include "Prefs.h"           // Preferences dialog box for tool
#include "status.h"          // Window that shows the progress bar
#include "TStrList.h"        // Header file for the string list class.
#include "MovePage.h"        // Dialog for moving pages around
#include "MediaDir.h"        // Metia file selector
#include "PictLoad.h"        // Dialog for loading pictures that shows a preview window

#ifdef SHARE_THE_MEMORIES
#include "Caption.h"         // Dialog for adding captions to pictures
#else
#include "InfoWnd.h"         // Info window setup dialog
#endif   // SHARE_THE_MEMORIES
#endif   // EBOOK_TOOL

// Globals
PTEbookWnd pEbookWnd;
FLOAT ZoomSizes[ZOOM_SIZE_COUNT] = { 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3 };
extern char States[NUM_US_STATES+1][3];



/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/

//make sure the check sum works!!!!!!


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
    pEbookWnd = new TEbookWnd();

    // Create the window
    if( pEbookWnd )
        hwnd = pEbookWnd->CreateParentWnd( hInst, nCmdShow );

    // Kick out of the program if there has been an error
    if( !hwnd )
        return FALSE;

    // Load the accelerators
    hAccel = LoadAccelerators( hInst, "eBookAccl" );
    if( !hAccel )
    {
        PostMsg( "Error", "Error loading accelerator table" );
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

}   // WINAPI   WinMain


/************************************************************************
*    FUNCTION NAME:         ParentWndProc                                                             
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

LRESULT CALLBACK ParentWndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_DESTROY,    pEbookWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND,    pEbookWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,     pEbookWnd->OnCreate );
        HANDLE_MSG( hwnd, WM_SIZE,       pEbookWnd->OnSize );
        HANDLE_MSG( hwnd, WM_CLOSE,      pEbookWnd->OnClose );
        HANDLE_MSG( hwnd, WM_SHOWWINDOW, pEbookWnd->OnShowWindow );
        HANDLE_MSG( hwnd, WM_MOVE,       pEbookWnd->OnMove );
        HANDLE_MSG( hwnd, WM_NOTIFY,     pEbookWnd->OnNotify );
        HANDLE_MSG( hwnd, WM_KEYDOWN,    pEbookWnd->OnKey );
        HANDLE_MSG( hwnd, WM_NCDESTROY,  pEbookWnd->OnNCDestroy );
        case WM_MOUSEWHEEL:
            pEbookWnd->OnMouseWheel( hwnd, (short) HIWORD(wParam) );
        break;
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

    return 0;

} // LRESULT CALLBACK ParentWndProc


/************************************************************************
*    FUNCTION NAME:         WndMidProc                                                             
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

LRESULT CALLBACK WndMidProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_HSCROLL,    pEbookWnd->MidWnd_OnHScroll );
        HANDLE_MSG( hwnd, WM_VSCROLL,    pEbookWnd->MidWnd_OnVScroll );
        #ifndef EBOOK_TOOL
        HANDLE_MSG( hwnd, WM_PAINT,      pEbookWnd->MidWnd_OnPaint );
        HANDLE_MSG( hwnd, WM_ERASEBKGND, pEbookWnd->MidWnd_OnEraseBkgnd );
        #endif
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

} // LRESULT CALLBACK WndMidProc


/************************************************************************
*    FUNCTION NAME:         ViewWndProc()                                                             
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
        HANDLE_MSG( hwnd, WM_PAINT,      pEbookWnd->ViewWnd_OnPaint );
        HANDLE_MSG( hwnd, WM_DESTROY,    pEbookWnd->ViewWnd_OnDestroy );
        HANDLE_MSG( hwnd, WM_CREATE,     pEbookWnd->ViewWnd_OnCreate );
        HANDLE_MSG( hwnd, WM_ERASEBKGND, pEbookWnd->ViewWnd_OnEraseBkgnd );
        #ifdef EBOOK_TOOL
        HANDLE_MSG( hwnd, WM_DROPFILES,  pEbookWnd->ViewWnd_OnDropFiles );
        #endif
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }
}   // ViewWndProc


/************************************************************************
*    FUNCTION NAME:         EdtSubclass_LoadPage                                                             
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

LRESULT CALLBACK EdtSubclass_LoadPage( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    // Kill the keyboard messages
    if( (Message == WM_KEYDOWN || Message == WM_KEYUP || Message == WM_CHAR) && wParam == RETURN_KEY )
    {
        if( Message == WM_KEYDOWN )
            pEbookWnd->LoadKeyEntryPage();

        return FALSE;
    }
    else
        return CallWindowProc( pEbookWnd->GetLoadPageSubProc(), hwnd, Message, wParam, lParam );

}   // EdtSubclass_LoadPage


/************************************************************************
*    FUNCTION NAME:         ShowOrderFormDlg                                                             
*
*    DESCRIPTION:           Displays the orderform dialog box.
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
#if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
void ShowOrderFormDlg()
{
    char *LetterTxt;
    TLoadRes LoadRes;
    
    // Load the text from the resource
    if( !LoadTheResource( pEbookWnd->GetInstance(), "orderFrmTxt", "TXT", &LoadRes ) )
        return;

    // Since this is touchy information, we want to checksum it to be
    // sure it has not been changed by someone. If we fail the checksum then
    // just display a weird message because we don't want to clue in the guilty
    // party that we are onto their game
    if( !CheckSumData( LoadRes.pData, LoadRes.Size ) )
    {
        // Display a meaning less error to confuse evil doers.
        PostMsg( "Error", "Error number 15432." );

        return;
    }

    // New up an array to hold the letter that will be printed out.
    // Make the array larger so that it can hold additional information to sprintf
    LetterTxt = new char[LoadRes.Size+LARGE_ARRAY_500];

    #ifdef EBOOK_TOOL

        // Take the thank you note and add in all the specific information
        sprintf( LetterTxt, (char *)LoadRes.pData, PRODUCT_NAME, TYPEOFPROGRAM, TYPEOFPROGRAM, AOL_ADDRESS,  
                 WEB_ADDRESS, EMAIL_ADDRESS, FIRST_TEMP_NUMBER );
    #else
        TLoadBuffer lb;

        // Zero out the structure
        memset( &lb, 0, sizeof( TLoadBuffer ) );

        // Take the thank you note and add in all the specific information
        // Are we using the second address line
        if( !pEbookWnd->GetPtrProjPrefs()->AddressLine2[0] )
        {
            sprintf( LetterTxt, (char *)LoadRes.pData, 
                     pEbookWnd->GetPtrProjPrefs()->NameOrCompany,
                     pEbookWnd->GetPtrProjPrefs()->AddressLine1,  
                     " ", " ", pEbookWnd->GetPtrProjPrefs()->City, 
                     (pEbookWnd->GetPtrProjPrefs()->StateIndex ? States[pEbookWnd->GetPtrProjPrefs()->StateIndex] : " "),
                     pEbookWnd->GetPtrProjPrefs()->Zip,
                     pEbookWnd->GetPtrProjPrefs()->WebAddress, 
                     pEbookWnd->GetPtrProjPrefs()->EmailAddress );
        }
        else
        {

            sprintf( LetterTxt, (char *)LoadRes.pData, 
                     pEbookWnd->GetPtrProjPrefs()->NameOrCompany, 
                     pEbookWnd->GetPtrProjPrefs()->AddressLine1,  
                     "\r", pEbookWnd->GetPtrProjPrefs()->AddressLine2, pEbookWnd->GetPtrProjPrefs()->City, 
                     (pEbookWnd->GetPtrProjPrefs()->StateIndex ? States[pEbookWnd->GetPtrProjPrefs()->StateIndex] : " "),
                     pEbookWnd->GetPtrProjPrefs()->Zip,
                     pEbookWnd->GetPtrProjPrefs()->WebAddress, pEbookWnd->GetPtrProjPrefs()->EmailAddress );
        }

        // Do they want to see the helpful hint window?
        if( pEbookWnd->GetPtrProjPrefs()->InfoOFWndTXTOffset )
        {
            // Load the Order Form Helpful Hint text box
            if( pEbookWnd->LoadFileDataFromEXEfile( pEbookWnd->GetPtrProjPrefs()->InfoOFWndTXTOffset, &lb ) )
                // Set the buffer
                pEbookWnd->GetPtrSettings()->SetOFInfoBufferPtr( (char *)lb.pData );
            else
                pEbookWnd->GetPtrSettings()->SetOFInfoBufferPtr( NULL );
        }
    #endif

    // Copy over the buffer pointer
    pEbookWnd->GetPtrSettings()->SetThankYouNoteBuffer( LetterTxt );

    // Show the Order Form dialog box
    DisplayOrderFrmDialogBox( pEbookWnd->GetInstance(), pEbookWnd->GetParentWnd(), 
                              pEbookWnd->GetPtrSettings(), CENTER_WND_TO_PARENT );

    if( LetterTxt )
        delete [] LetterTxt;

    #ifndef EBOOK_TOOL
    if( lb.pData )
        delete [] lb.pData;
    #endif

}   // ShowOrderFormDlg
#endif



/************************************************************************
*
*                  Dialog Object Related Functions
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

TEbookWnd::TEbookWnd()
{
    ViewWndActive = FALSE;
    BackGrndBrush = NULL;

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

TEbookWnd::~TEbookWnd()
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

HWND TEbookWnd::CreateParentWnd( HINSTANCE hInst, int nCmdShow )
{
    HWND hwnd = NULL;

    // First this to do is memset the AppData
    memset( &AppData, 0, sizeof( TApplicationData ) );

    // Init Windows 95 common controls
    InitCommonControls();

    // Get the command line and parse it to be sure
    // there is no junk with it
    ParsePathFromCmdLine( GetCommandLine(), AppData.Path );

    // We are in debug mode and are loading data from the default file name.
    // This causes the program to open the other file on the hard drive
    // instead of itself which allows us to step through it in the debugger
    #ifdef VIEW_DEBUG

    char FilePath[MAX_PATH];

    // We are asuming this file has bee generated by the tool
    CropOutPath( AppData.Path, FilePath );
    strcat( FilePath, "\\" );
    strcat( FilePath, PRO_EXAMPLE_EXE_NAME );
    strcpy( AppData.Path, FilePath );

    #endif    

    // Get the settings in the ini file
    Settings.GetPrivateString();

    #ifdef EBOOK_TOOL
		// Check to see if this program is registered
        Settings.GetSecretIniFile( INI_SECRET_REG_FILE, PRODUCT_NAME );

        // Init the registred mode
        AppData.RegistrationMode = FULL_REG_VERSION;

    #else

        // Reads in the file header, the preferences and all the TPage data
        if( !OpenEXEandReadHeader() )
            goto HANDLE_ERROR;

        // Checksum the program
        if( ProjPrefsData.DoChecksum )
            if( !CheckSumApp() )
                goto HANDLE_ERROR;

		#ifndef SHARE_THE_MEMORIES

	        // Set the registration number. This is so that there is only one
	        // variable to check no matter if tool or the view program is running.
	        strcpy( AppData.RegistrationNo, ProjPrefsData.RegNo );

	        // If the view program was created with the shareware mode version of eBook, 
	        // check if you can find the ebook 4 Sale ini file because they are not supposed
	        // to be able to run the program on another computer
	        if( ProjPrefsData.ProjRegMode == SHAREWARE_VERSION )
	        {
	            if( !AllowViewProgramToRun() )
	            {
	                DisplayAboutDlg( hInst, 0, OPPS_NEED_TO_REGISTER, SORRY_CANT_USE_EXE, 
	                                 "SmallStartup", "GIF", NULL );
	                goto HANDLE_ERROR;
	            }
	        }
	 
	        // Check to see if this program was registered.
	        // Init the registred mode
	        if( IsViewProgramRegistered() == FULL_REG_VERSION )
	            AppData.RegistrationMode = FULL_REG_VERSION;
	        else
	            AppData.RegistrationMode = SHAREWARE_VERSION;
		#else
		AppData.RegistrationMode = FULL_REG_VERSION;
		#endif
    #endif

    // Register the window
    if( !RegisterWnd( hInst ) )
        goto HANDLE_ERROR;

    // Create the window
    hwnd = CreateWnd( hInst, Settings.GetWindowState() );
    if( !hwnd )
        goto HANDLE_ERROR;

HANDLE_ERROR:;

    return hwnd;

}   // CreateParentWnd


/************************************************************************
*    FUNCTION NAME:         AllowViewProgramToRun                                                             
*
*    DESCRIPTION:           This function decides if the view program 
*                           should run. When the ebook 4 Sale tool is in
*                           shareware mode, the view program can't run
*                           on another computer other than the computer
*                           the ebook 4 Sale tool is on. 
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

#ifndef EBOOK_TOOL
BOOL TEbookWnd::AllowViewProgramToRun()
{
    BOOL Result = FALSE, VolInfoReturnValue;
    DWORD CheckSerialNo;

    // This will keep them from running the view program from their computers CD-ROM
    if( AppData.Path[0] == 'c' || AppData.Path[0] == 'C' )
    {
        // Check the serial number on the user's hard drive
        VolInfoReturnValue = GetVolumeInformation( "C:\\", NULL, 0, &CheckSerialNo, NULL, NULL, NULL, 0 );
        if( VolInfoReturnValue )
        {
            // See if the two values are the same
            if( ProjPrefsData.DriveSerialNo == CheckSerialNo )
                Result = TRUE;
        }

        // If the serial number from the drive didn't work, check for the ini file
        // This is a fail safe incase their hard drive is having problems
        if( !Result && (!VolInfoReturnValue || !CheckSerialNo) )
        {
            char WinDir[MAX_PATH];

            GetWindowsDirectory( WinDir, MAX_PATH * sizeof( char ) );

            // Search for the ebook 4 Sale ini file. If its not there, then this program should not run
            Result = BOOL( SearchPath( WinDir, INI_TOOL_FILE_CHECK, NULL, NULL, NULL, NULL ) );
        }
    }

    return Result;

}   // AllowViewProgramToRun
#endif



/************************************************************************
*    FUNCTION NAME:         IsViewProgramRegistered                                                             
*
*    DESCRIPTION:           Checks to see if the customer has registered
*                           the view program. The secret ini file is also
*                           used to hold specific settings like the
*                           helpful hint windows. 
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
#ifndef EBOOK_TOOL
BOOL TEbookWnd::IsViewProgramRegistered()
{
    char CompanyName[MAX_INFO+1];
    char TitleName[MAX_TITLE+1];
    char SectionName[MAX_INFO + MAX_TITLE + 20];
    BOOL Result;

    // Setup the program's name for the registration
    if( strlen( ProjPrefsData.NameOrCompany ) > 0 )
        sprintf( CompanyName, ProjPrefsData.NameOrCompany );
    else
        sprintf( CompanyName, UNKNOWN_COMANY_NAME );

    // Setup the program's title for the registration
    if( strlen( ProjPrefsData.Title ) > 0 )
        strcpy( TitleName, ProjPrefsData.Title );
    else
        strcpy( TitleName, UNKNOWN_PROGRAM_NAME );

    // Setup the section name for the secret ini file
    sprintf( SectionName, "%s-%s-%u", CompanyName, TitleName, (int)ProjPrefsData.ViewProgramRandNo );

    // Load the registration number from the secret ini file to
    // see if this program has been registered
    Settings.GetSecretIniFile( INI_SECRET_REG_FILE, SectionName );

    #ifndef SHARE_THE_MEMORIES
    // Check to see if a registration number was defined.
    // If they didn't define a registration number, then the view program is free.
    if( ProjPrefsData.UseRegNum )
    {
        // Has this program been registered?
        if( strcmp( Settings.GetRegNum(), AppData.RegistrationNo ) == 0 )
            Result = FULL_REG_VERSION;
    }
    else
        Result = FULL_REG_VERSION;
    #else
        Result = FULL_REG_VERSION;
    #endif

    return Result;

}   // IsViewProgramRegistered
#endif



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

void TEbookWnd::SaveSecretINIsettings()
{

#ifdef EBOOK_TOOL
    // They registered so save the registration number to the secret ini file
    Settings.WriteSecretIniFile( INI_SECRET_REG_FILE, PRODUCT_NAME );
#else

    #ifndef SHARE_THE_MEMORIES  // Share the Memories doesn't use the below information

    char CompanyName[MAX_INFO+1];
    char TitleName[MAX_TITLE+1];
    char SectionName[MAX_INFO + MAX_TITLE + 20];

    // Setup the program's name for the registration
    if( strlen( ProjPrefsData.NameOrCompany ) > 0 )
        sprintf( CompanyName, ProjPrefsData.NameOrCompany );
    else
        sprintf( CompanyName, UNKNOWN_COMANY_NAME );

    // Setup the program's title for the registration
    if( strlen( ProjPrefsData.Title ) > 0 )
        strcpy( TitleName, ProjPrefsData.Title );
    else
        strcpy( TitleName, UNKNOWN_PROGRAM_NAME );

    // Save the last selected page
    Settings.SetLastPage( PageList.GetCurrentPage() );

    // Save the zoom index
    Settings.SetZoomIndex( ProjPrefsData.ZoomIndex );

    // Setup the section name for the secret ini file
    sprintf( SectionName, "%s-%s-%u", CompanyName, TitleName, (int)ProjPrefsData.ViewProgramRandNo );

    // Save the customer's font choice
    Settings.SetLogFont( &ProjPrefsData.DefFont );
    Settings.SetFontColor( ProjPrefsData.DefaultTxtColor );
    Settings.SetBackgrndTextColor( ProjPrefsData.DefBckgrdTxtColor );

    Settings.WriteSecretIniFile( INI_SECRET_REG_FILE, SectionName );
    #endif

#endif

}   // SaveSecretINIsettings



/************************************************************************
*    FUNCTION NAME:         CheckSumApp                                                             
*
*    DESCRIPTION:           Checksums this application. There's no telling
*                           how big this file can be so we are going to take
*                           the safe route and read in each DWORD from the file. 
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
*    h rosenorn    8/18/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

#ifndef EBOOK_TOOL
BOOL TEbookWnd::CheckSumApp()
{
    BOOL Result = FALSE;

    // Is the result zero? Then it is ok
    if( CheckSumFile( AppData.Path ) != 0 )         
        goto HANDLE_ERROR;

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        PostMsg( "Program Check Failed", "This application has failed the program check which could mean that this program is damanaged or has been modified. Downloading files from the web or an online service can get cut off in the middle or corrupted durring the download. Try downloading this program again and if the problem continues, download again at a later time." ); 

    return Result;

}   // CheckSumApp
#endif


/************************************************************************
*    FUNCTION NAME:         OpenEXEandReadHeader                                                             
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

#ifndef EBOOK_TOOL
BOOL TEbookWnd::OpenEXEandReadHeader()
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
    hFile = CreateFile( AppData.Path, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open application file (%s) on disk to run program.", AppData.Path );
        goto FREE_MEMORY;
    }

    // Set the file pointer to the end of the file and then back it up to read the file header
    SetFilePointer( hFile, -sizeof(TFileHeader), 0, FILE_END );

    // Read the file header which is at the end of the file
    FileReadWriteResult = ReadFile( hFile, &FileHeader, sizeof(TFileHeader), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TFileHeader) )
    {
        PostMsg( "Error", "Error reading in application file header. (%s)", AppData.Path );
        goto FREE_MEMORY;
    }

    // Check that we have a proper formatID. This is just an exact number and if that
    // number is not the same, we are not looking at the data we should be.
    if( FileHeader.FormatID != FORMAT_ID )
    {
        PostMsg( "Error", "Format ID does not match. Application format out of sync. (%s)", AppData.Path );
        goto FREE_MEMORY;
    }

    // Set the file pointer to read in the applications preferences
    // This is where we use the data offset that is saved with the header
    SetFilePointer( hFile, FileHeader.ProjectDataOffset, 0, FILE_BEGIN );

    // Read contents of preferences data into structure
    FileReadWriteResult = ReadFile( hFile, &ProjPrefsData, sizeof(TProjectPrefs), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TProjectPrefs) )
    {
        PostMsg( "Error", "Error reading in application preferences. (%s)", AppData.Path );
        goto FREE_MEMORY;
    }

    // Unencrypt the data using the SHAREWARE_BUILD as the key. It's a long array of text that
    // a hacker may not think of using.
    LockItUp( (BYTE *)SHAREWARE_BUILD, (BYTE *)&ProjPrefsData, sizeof(TProjectPrefs), UNENCRYPT_DATA );

    // Check the preferences ID for vaild number
    if( ProjPrefsData.PrefsCheckID != PERFS_CHECK_ID )
    {
        PostMsg( "Error", "Preference ID does not match. Preference data out of sync." );
        goto FREE_MEMORY;
    }

    // Only read in the page data if it hasen't been read in before
    if( PageList.Count() == 0 )
    {
        // Jump the file pointer to the offset the page data is starting at
        SetFilePointer( hFile, ProjPrefsData.PageDataOffset, 0, FILE_BEGIN );

        // Open the pages that have been saved
        for( i = 0; i < ProjPrefsData.Total_Num_Of_Pages; ++i )
        {
            // Add a blank page
            if( !PageList.Add() )
            {
                PostMsg( "Error", "Error adding page." );
                goto FREE_MEMORY;
            }

            // Read contents of file into allocated memory
            if( !ReadFile( hFile, PageList.GetObj( i ), PageList.GetObjSize( i ), &bytesReadWrite, NULL ) )
            {
                PostMsg( "Error", "Error saving page data to disk." );
                goto FREE_MEMORY;
            }

            // Check the preferences ID for vaild number
            if( PageList.GetPageCheckID( i ) != PAGE_CHECK_ID )
            {
                PostMsg( "Error", "Page ID does not match. Page data out of sync." );
                goto FREE_MEMORY;
            }
        }
    }

    Result = TRUE;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // OpenEXEandReadHeader
#endif




/************************************************************************
*    FUNCTION NAME:         LoadFileDataFromEXEfile                                                             
*
*    DESCRIPTION:           Loads the imgage data, uncompress it from the exe 
*
*    FUNCTION PARAMETERS:
*    Input:    DWORD Offset - Offset to the compressed data
*              WORD *DataType - Pointer to a word to save the data type 
*
*    Output:   BYTE * - Pointer to filled data buffer
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    h rosenorn    12/10/00      Removed LZW Compression
************************************************************************/

#ifndef EBOOK_TOOL
BOOL TEbookWnd::LoadFileDataFromEXEfile( DWORD Offset, PTLoadBuffer pLoadBuffer )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    HANDLE hFile;
    BOOL Result = FALSE;
    TDataHeader DataHeader;
    //BYTE *pCmpBuffer = NULL;
    BYTE *pUncBuffer;
    //TLZW lzw;
    DWORD FileSize;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    // Init the load data buffer
    pLoadBuffer->Type = 0;
    pLoadBuffer->Size = 0;
    pLoadBuffer->pData = NULL;

    // Open the file on the hard drive
    hFile = CreateFile( AppData.Path, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open application file (%s) on disk.", AppData.Path );
        goto FREE_MEMORY;
    }

    // Set the file pointer to the end of the file and then back it up to read the file header
    SetFilePointer( hFile, Offset, 0, FILE_BEGIN );

    // Read the data header that is at the beginning of all the data
    FileReadWriteResult = ReadFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
    {
        PostMsg( "Error", "Error reading in file data header." );
        goto FREE_MEMORY;
    }

    // Varify the data header is what we are looking for
    if( !(DataHeader.Type > START_FILE_TYPE && DataHeader.Type < MAX_FILE_TYPES) )
    {
        PostMsg( "Error", "Error reading in file data header. Header type out of sync." );
        goto FREE_MEMORY;
    }

    // Allocate a buffers to hold the compressed and uncompressed data
    // Give the compressed and the uncompressed buffers extra memory just in case a terminator is needed
    /*if( DataHeader.CompSize )
    { 
        pCmpBuffer = new BYTE[DataHeader.CompSize + 1];
        // Terminate just for the hect of it
        pCmpBuffer[ DataHeader.CompSize ] = 0;
    }*/

    pUncBuffer = new BYTE[DataHeader.UncompSize + 1];
    if( !pUncBuffer /*|| (DataHeader.CompSize ? !pCmpBuffer : FALSE)*/ )
    {
        PostMsg( "Error", "Memory allocation error while trying to load compressed file data." );

        // Normally we don't delete the uncompressed buffer because it ends up being owned by the
        // DIB class and get freed there when done but because there was an error, we free everything.
        delete [] pUncBuffer;
        goto FREE_MEMORY;
    }

    // Terminate the buffers
    pUncBuffer[ DataHeader.UncompSize ] = 0;

    FileSize = DataHeader.UncompSize;
    FileReadWriteResult = ReadFile( hFile, pUncBuffer, DataHeader.UncompSize, &bytesReadWrite, NULL );

    // The UncompSize and CompSize are the same for uncompressed files
    if( !FileReadWriteResult || bytesReadWrite != FileSize )
    {
        PostMsg( "Error", "Error reading in compressed data" );
        // Normally we don't delete the uncompressed buffer because it ends up being owned by the
        // DIB class and get freed there when done but because there was an error, we free everything.
        delete [] pUncBuffer;
        goto FREE_MEMORY;
    }

    // Close out the file now before we do anything else
    CloseHandle( hFile );
    hFile = NULL;

    // Save the pointer to the data
    Result = TRUE;

    // Fillout the load data structure so that we know what we loaded
    pLoadBuffer->Type = DataHeader.Type;
    pLoadBuffer->Size = DataHeader.UncompSize;
    pLoadBuffer->pData = pUncBuffer;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // LoadImageFromEXEfile
#endif



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

BOOL TEbookWnd::RegisterWnd( HINSTANCE hInst )
{
    WNDCLASSEX w;

    // Zero out the structure
    memset( &w, 0, sizeof(WNDCLASS) );

    // Non specific settings. Common to all
    w.cbSize        = sizeof( WNDCLASSEX );
    w.hIconSm       = NULL;
    w.hCursor       = LoadCursor( NULL, IDC_ARROW );
    w.style         = CS_HREDRAW|CS_VREDRAW;
    w.hInstance     = hInst;

    // Register the View window    
    w.lpfnWndProc   = ViewWndProc;    
    w.hbrBackground = (HBRUSH)GetStockBrush(WHITE_BRUSH);
    w.lpszClassName = "ViewWnd";

    if( !RegisterClassEx( &w ) )
        return FALSE;

    // Register the View window
    w.lpfnWndProc   = WndMidProc;
    w.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW+1);
    w.lpszClassName = "MidWnd";

    if( !RegisterClassEx( &w ) )
        return FALSE;

    // Register the parent window   
    w.lpfnWndProc   = ParentWndProc;
    w.hIcon         = LoadIcon( hInst, "AppIcon" );
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

HWND TEbookWnd::CreateWnd( HINSTANCE hInst, int nCmdShow )
{
    AppData.hInst = hInst;

    HWND hwnd = CreateWindow( WINDOW_NAME, PRODUCT_NAME, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL );

    AppData.hParentWnd = hwnd;

    // Do these good things if the handle is not NULL
    if( hwnd != NULL )
    {
        ShowWindow( hwnd, nCmdShow );
        UpdateWindow( hwnd );

        // Post a message to show the startup screen
        // This way it doesn't show up before the main window
        PostMessage( hwnd, ((UINT)WM_COMMAND), ((WPARAM)MAKELONG( POST_MSG_AFTER_WM_SHOW, 0 )), ((LPARAM)0) );
    }

    return hwnd;

}   // CreateWnd


/************************************************************************
*    FUNCTION NAME:         OnKey                                                             
*
*    DESCRIPTION:           Standard Windows WM_KEYDOWN message handler.
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

void TEbookWnd::OnKey( HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
    switch( vk )
    {
        case VK_UP:
            SendMessage( AppData.hMidWnd, WM_VSCROLL, SB_PAGELEFT, 0L );
        break;

        case VK_DOWN:
            SendMessage( AppData.hMidWnd, WM_VSCROLL, SB_PAGERIGHT, 0L );
        break;

        case VK_PRIOR:
        case VK_HOME:
            SendMessage( AppData.hMidWnd, WM_VSCROLL, SB_TOP, 0L );
        break;

        case VK_NEXT:
        case VK_END:
            SendMessage( AppData.hMidWnd, WM_VSCROLL, SB_BOTTOM, 0L );
        break;
    }

}   // OnKey


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

void TEbookWnd::MidWnd_OnHScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos )
{
    if( AppData.HScrollVisible )
    {
        GetScrollInfo( hwnd, SB_HORZ, &AppData.sh );
        GetScrollInfo( hwnd, SB_VERT, &AppData.sv );

        switch( code )
        {
            case SB_LINERIGHT:
            case SB_PAGERIGHT:
                if( AppData.sh.nPos >= AppData.sh.nMin && AppData.sh.nPos <= AppData.sh.nMax-AppData.sh.nPage )
                {
                    AppData.sh.nPos += AppData.sh.nPage;
                    SetScrollInfo( hwnd, SB_HORZ, &AppData.sh, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );
                }
            break;
            
            case SB_LINELEFT:
            case SB_PAGELEFT:
                if( AppData.sh.nPos > AppData.sh.nMin && AppData.sh.nPos <= AppData.sh.nMax )
                {
                    AppData.sh.nPos -= AppData.sh.nPage;
                    SetScrollInfo( hwnd, SB_HORZ, &AppData.sh, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );
                }
            break;

            case SB_THUMBTRACK:
                AppData.sh.nPos = pos;
                SetScrollInfo( hwnd, SB_HORZ, &AppData.sh, TRUE );

                // Scroll the view window
                MoveWindow( AppData.hViewWnd, 
                AppData.ViewWndRect.left - (AppData.sh.nTrackPos * SCROLL_BAR_PAGE_SIZE), 
                AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );

            break;
        }
    }

}   // OnHScroll



/************************************************************************
*    FUNCTION NAME:         MidWnd_OnVScroll                                                             
*
*    DESCRIPTION:           Standard Windows OnVScroll message handler.
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

void TEbookWnd::MidWnd_OnVScroll( HWND hwnd, HWND hwndCtl, UINT code, int pos )
{
    if( AppData.VScrollVisible )
    {
        GetScrollInfo( hwnd, SB_VERT, &AppData.sv );
        GetScrollInfo( hwnd, SB_HORZ, &AppData.sh );

        switch( code )
        {
            case SB_TOP:
                if( AppData.sv.nPos > AppData.sv.nMin )
                {
                    AppData.sv.nPos = AppData.sv.nMin;
                    SetScrollInfo( hwnd, SB_VERT, &AppData.sv, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );

                }
            break;

            case SB_BOTTOM:
                if( AppData.sv.nPos < AppData.sv.nMax-AppData.sv.nPage )
                {
                    // Set the top of the view window
                    AppData.sv.nPos = AppData.sv.nMax;
                    SetScrollInfo( hwnd, SB_VERT, &AppData.sv, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );

                }
            break;

            case SB_LINERIGHT:
            case SB_PAGERIGHT:
                if( AppData.sv.nPos >= AppData.sv.nMin && AppData.sv.nPos <= AppData.sv.nMax-AppData.sv.nPage )
                {
                    AppData.sv.nPos += AppData.sv.nPage;
                    SetScrollInfo( hwnd, SB_VERT, &AppData.sv, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );
                }
            break;

            case SB_LINELEFT:
            case SB_PAGELEFT:
                if( AppData.sv.nPos > AppData.sv.nMin && AppData.sv.nPos <= AppData.sv.nMax )
                {
                    AppData.sv.nPos -= AppData.sv.nPage;
                    SetScrollInfo( hwnd, SB_VERT, &AppData.sv, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (AppData.sv.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );
                }
            break;

            case SB_THUMBTRACK:
                    AppData.sv.nPos = pos;
                    SetScrollInfo( hwnd, SB_VERT, &AppData.sv, TRUE );

                    // Scroll the view window
                    MoveWindow( AppData.hViewWnd, 
                    AppData.ViewWndRect.left - (AppData.sh.nPos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.top - (pos * SCROLL_BAR_PAGE_SIZE), 
                    AppData.ViewWndRect.right, AppData.ViewWndRect.bottom, TRUE );
            break;
        }
    }

}   // OnVScroll



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

LRESULT TEbookWnd::OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr )
{
    #ifdef EBOOK_TOOL
    char ToolExtraInfo[SMALL_ARRAY_100];
    #endif

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
                #ifdef EBOOK_TOOL
                if( PageList.Count() )
                {
                    CropFileNameFromPath( PageList.GetMainDataPath( PageList.GetCurrentPage() ), ToolExtraInfo );
                    strcat( AppData.ToolTipStr, " - " );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                #endif
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_FLIP_RIGHT:
                strcpy( AppData.ToolTipStr, "Flip Ahead One Page" );
                #ifdef EBOOK_TOOL
                if( PageList.Count() )
                {
                    CropFileNameFromPath( PageList.GetMainDataPath( PageList.GetCurrentPage() ), ToolExtraInfo );
                    strcat( AppData.ToolTipStr, " - " );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                #endif
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_ZOOM_IN:
                if( AppData.pGBase && AppData.pGBase->GetObjectType() == TEXT_OBJECT )
                    sprintf( AppData.ToolTipStr, "Enlarge Font - %s", ProjPrefsData.DefFont.lfFaceName );
                else
                    sprintf( AppData.ToolTipStr, "Enlarge Page - %.0lf%", AppData.ZoomPercent * 100 );
                #ifdef EBOOK_TOOL
                if( !dib.IsEmpty() && AppData.pGBase && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
                {
                    sprintf( ToolExtraInfo, " - %u x %u", dib.width(), dib.height() );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                #endif
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_ZOOM_OUT:
                if( AppData.pGBase && AppData.pGBase->GetObjectType() == TEXT_OBJECT )
                    sprintf( AppData.ToolTipStr, "Reduce Font - %s", ProjPrefsData.DefFont.lfFaceName );
                else
                    sprintf( AppData.ToolTipStr, "Reduce Page - %.0lf%", AppData.ZoomPercent * 100 );
                #ifdef EBOOK_TOOL
                if( !dib.IsEmpty() && AppData.pGBase && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
                {
                    sprintf( ToolExtraInfo, " - %u x %u", dib.width(), dib.height() );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                #endif
                LPTOOLTIPTEXT( pnmhdr )->lpszText = AppData.ToolTipStr;
            break;

            case BTN_SOUND_PLAY:
                strcpy( AppData.ToolTipStr, "Play Sound" );
                #ifdef EBOOK_TOOL
                if( PageList.Count() && PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                {
                    CropFileNameFromPath( PageList.GetSoundDataPath( PageList.GetCurrentPage() ), ToolExtraInfo );
                    strcat( AppData.ToolTipStr, " - " );
                    strcat( AppData.ToolTipStr, ToolExtraInfo );
                }   
                #endif
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
*    FUNCTION NAME:         MidWnd_OnEraseBkgnd()                                                             
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
#ifndef EBOOK_TOOL
BOOL TEbookWnd::MidWnd_OnEraseBkgnd( HWND hwnd, HDC hdc )
{
    BOOL Result = FALSE;

    if( TileBackDIB.IsEmpty() )
    {
        HBRUSH brush;
        RECT DisplayRect;

        // Create a brush
        brush = CreateSolidBrush( GetSysColor( COLOR_BACKGROUND ) );

        // Get the rect of our view window
        GetClientRect( hwnd, &DisplayRect );

        FillRect( hdc, &DisplayRect, brush );
        DeleteObject( brush );

        Result = TRUE;
    }

    return Result;

}   // MidWnd_OnEraseBkgnd
#endif



/************************************************************************
*    FUNCTION NAME:         Parent_OnEraseBkgnd()                                                             
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

BOOL TEbookWnd::ParentWnd_OnEraseBkgnd( HWND hwnd, HDC hdc )
{
    return FALSE;

}   // Parent_OnEraseBkgnd



/************************************************************************
*    FUNCTION NAME:         OnMouseWheel()                                                             
*
*    DESCRIPTION:           Standard Windows OnMouseWheel message handler.
*                           This our way of forcing Mouse Wheel because Win 98
*                           to work with Mouse Wheel because it works on and
*                           off depending on the system.
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

void TEbookWnd::OnMouseWheel( HWND hwnd, int zDelta )
{
    if( zDelta == 120 )
        SendMessage( AppData.hMidWnd, WM_VSCROLL, SB_PAGELEFT, 0L );
    else
        SendMessage( AppData.hMidWnd, WM_VSCROLL, SB_PAGERIGHT, 0L );
        
}   // OnMouseWheel


/************************************************************************
*    FUNCTION NAME:         MidWnd_OnPaint()                                                             
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
#ifndef EBOOK_TOOL
void TEbookWnd::MidWnd_OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT DisplayRect;
    int i, j, Col, Row;

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    if( PaintDC && !TileBackDIB.IsEmpty() )
    {
        // Get the rect of our view window
        GetClientRect( hwnd, &DisplayRect );

        // Calculate the number of rows and colums
        if( DisplayRect.right % TileBackDIB.width() )
            Row = (DisplayRect.right / TileBackDIB.width()) + 1;
        else
            Row = DisplayRect.right / TileBackDIB.width();

        if( DisplayRect.bottom % TileBackDIB.height() )
            Col = (DisplayRect.bottom / TileBackDIB.height()) + 1;
        else
            Col = DisplayRect.bottom / TileBackDIB.height();

        // Tile the bitmat to the window
        for( j = 0; j < Col; ++j )
        {
            for( i = 0; i < Row; ++i )
            {
                TileBackDIB.Draw( PaintDC, TileBackDIB.width() * i, TileBackDIB.height() * j );
            }
        }
    }

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // MidWnd_OnPaint
#endif


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

#ifdef EBOOK_TOOL
void TEbookWnd::InitProject( HWND hwnd )
{
    int i;

    // Reset these objects
    PageList.Free();
    dib.Free();
    TextBlock.Free();

    // zero out all the structures
    memset( &ActProj, 0, sizeof(TActiveProjectData) );
    memset( &ProjPrefsData, 0, sizeof(TProjectPrefs) );
    memset( &FileHeader, 0, sizeof(TFileHeader) );
    memset( &AppData.ViewWndRect, 0, sizeof(RECT) );

    // Init to this value because there will never be a 4 bit windows color
    ProjPrefsData.DefBckgrdTxtColor = NO_COLOR_SET;

    // Initilize the zoom percent to 100
    AppData.ZoomPercent = ZoomSizes[ ZOOM_100_PERCENT_INDEX ];
    ProjPrefsData.ZoomIndex = ZOOM_100_PERCENT_INDEX;

    // Reset the button bar
    for( i = 0; i < NUM_TBBUTTONS; ++i )
        TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT + i, DISABLE );

    // Make sure init is done before updating the controls
    UpdateControls( hwnd, 0, 0 );

    // Init fileheader specific elements
    FileHeader.FormatID = FORMAT_ID;
    strcpy( FileHeader.VerNo, PRO_VER_NUM );

    // Init a few of the project preferences members
    ProjPrefsData.PrefsCheckID = PERFS_CHECK_ID; // Code number to check if data is valid
    strcpy( ProjPrefsData.Title, PRO_EXAMPLE_CAPTION );
    strcpy( ProjPrefsData.ExeName, PRO_EXAMPLE_EXE_NAME );
    strcpy( ProjPrefsData.RegNo, PRO_EXAMPLE_REG_NUM );

    #ifdef SHARE_THE_MEMORIES
    strcpy( ProjPrefsData.WebAddress, PRO_EXAMPLE_WEB );
    #endif

    ProjPrefsData.AskForRegAfterPage = PRO_EXAMPLE_ASK_4_REG_AFTER_PAGE;
    //ProjPrefsData.CompressImages = TRUE;
    ProjPrefsData.UseRegNum = TRUE;
    ProjPrefsData.DoChecksum = TRUE;
    ProjPrefsData.AllowFileExports = TRUE;
    ProjPrefsData.AllowPrintPage = TRUE;
    ProjPrefsData.PrintPageNumbers = TRUE;

    // Setup the default font properties
    ProjPrefsData.DefFont.lfHeight = -11;
    ProjPrefsData.DefFont.lfWeight = 400;
    ProjPrefsData.DefFont.lfOutPrecision = 1;
    ProjPrefsData.DefFont.lfClipPrecision = 2;
    ProjPrefsData.DefFont.lfQuality = 1;
    ProjPrefsData.DefFont.lfPitchAndFamily = 34;
    strcpy( ProjPrefsData.DefFont.lfFaceName, "Arial" );

    // Make the default font
    TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
                           0, 0, &ProjPrefsData.DefFont );

    // Disable menu items
    EnableMenuItems( hwnd, FALSE );
    EnableSpecificMenuItems( hwnd, FALSE );

    // Reset the window caption
    SetWindowCaption( hwnd );

}   // InitProject
#endif



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

BOOL TEbookWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    // Seed the random number generator
    randomize();

    // Create the medis player
    mediaPlayer.Init( hwnd, AppData.hInst, USE_ERROR_DLG_BOXES );

    // Create the tool bar
    CreateToolBar( hwnd );

    #ifdef EBOOK_TOOL // Tool Code

    // Init application structures
    InitProject( hwnd );

    #else  // Viewer Code

    //char TmpString[MAX_PATH];
    BOOL TextPages = FALSE;

    //Create the path to the temp sound file
    //GetTheWindowsTmpDir( TmpString );
    sprintf( AppData.SoundFileTmpPath, DEFAULT_MUSIC_FILE_NAME, "C:\\", rand() %  0xFFFFFFF );

    // Are we using the registration number
    if( !ProjPrefsData.UseRegNum )
    {
        // Delete the menu items that are not an option
        DeleteMenu( GetMenu(hwnd), MNU_ORDER, MF_BYCOMMAND );
    }

    // Add the web link menu item
    if( ProjPrefsData.WebAddress[0] )
        AppendMenu( GetMenu(hwnd), MF_ENABLED|MF_STRING, MNU_WEB_LINK, "go to Web Site..." );

    // Share the memories does not allow changing the font and color of text in it's view program
    #ifndef SHARE_THE_MEMORIES

    // Check to tee if there are any text pages 
    for( int i = 0; i < PageList.Count(); ++i )
    {
        if( PageList.GetMainDataType( i ) == FILE_TYPE_TXT )
        {
            TextPages = TRUE;
            break;
        }
    }
    #endif

    // If there are no text pages, remove the menu items for the text pages
    if( !TextPages )
    {
        DeleteMenu( GetMenu(hwnd), MNU_SET_DEF_FONT, MF_BYCOMMAND );
        DeleteMenu( GetMenu(hwnd), MNU_SET_BKG_COLOR, MF_BYCOMMAND );
        DeleteMenu( GetMenu(hwnd), MNU_RESET_FONT_DEF, MF_BYCOMMAND );
    }

    // Do we allow the user to print pages?
    // Delete the menu items that are not an option
    if( !ProjPrefsData.AllowPrintPage )
    {
        DeleteMenu( GetMenu(hwnd), MNU_PRINT, MF_BYCOMMAND );
        DeleteMenu( GetMenu(hwnd), MNU_PRINT_SETUP, MF_BYCOMMAND );
    }

    // For testing other ebooks created by other people, to be able to export
    // the files they added to debug the problem.
    #ifdef FORCE_FILE_EXPORT
    ProjPrefsData.AllowFileExports = TRUE;
    #endif

    // Do we allow the file exports?
    // Delete the menu items that are not an option
    if( !ProjPrefsData.AllowFileExports )
    {
        DeleteMenu( GetMenu(hwnd), MNU_EXPORT_PAGE_FILE, MF_BYCOMMAND );
        DeleteMenu( GetMenu(hwnd), MNU_EXPORT_SOUND_FILE, MF_BYCOMMAND );
    }

    // Add a seperator if one or the other is there
    if( ProjPrefsData.AllowPrintPage || TextPages || ProjPrefsData.AllowFileExports )     
        InsertMenu( GetMenu(hwnd), MNU_EXIT, MF_SEPARATOR, 0, NULL );

    // Seperate the print pages if any of the two are there
    if( ProjPrefsData.AllowPrintPage && (TextPages || ProjPrefsData.AllowFileExports) )     
        InsertMenu( GetMenu(hwnd), MNU_PRINT, MF_SEPARATOR, 0, NULL );

    // Seperate the last two if they are all there
    if( ProjPrefsData.AllowPrintPage && TextPages && ProjPrefsData.AllowFileExports )     
        InsertMenu( GetMenu(hwnd), MNU_SET_DEF_FONT, MF_SEPARATOR, 0, NULL );

    // Do we need to check the menu
    if( Settings.GetPlaySoundPageTurn() )
        CheckMenuItem( GetMenu( hwnd ), MNU_PLAY_ON_PAGE_TURN, MF_BYCOMMAND|MF_CHECKED );

    // Do we need to check the menu
    if( Settings.GetSizeWndToFit() )
        CheckMenuItem( GetMenu( hwnd ), MNU_AUTO_SIZE, MF_BYCOMMAND|MF_CHECKED );

    // Display the caption to show if this program is registered or shareware
    SetWindowCaption( hwnd );

    #endif   // #ifdef EBOOK_TOOL

    // Init Order Form specific data
    SetupOrderFormInfo();

    // set the window
    if( Settings.GetWindowX() > GetSystemMetrics( SM_CXSCREEN ) - GetSystemMetrics( SM_CYCAPTION ) ||
        Settings.GetWindowY() > GetSystemMetrics( SM_CYSCREEN ) - GetSystemMetrics( SM_CYCAPTION ) )
        ShowWindow( hwnd, SW_MAXIMIZE );
    else if( Settings.GetWindowW() && Settings.GetWindowH() && Settings.GetWindowState() == SW_SHOW )
        // Put a window at a given position and size
        MoveWindow( hwnd, Settings.GetWindowX(), Settings.GetWindowY(), Settings.GetWindowW(), Settings.GetWindowH(), TRUE );

    // Create the middle window
    AppData.hMidWnd = CreateWindow( "MidWnd", "Middle Window", WS_CHILD|WS_HSCROLL|WS_VSCROLL|WS_VISIBLE|WS_CLIPCHILDREN,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                              hwnd, NULL, AppData.hInst, NULL );

    // If this window fails to make then use the parent window
    if( !AppData.hMidWnd )
    {
        AppData.hMidWnd = hwnd;
        return FALSE;
    }

    // Init the variables holding the the scroll bar data
    AppData.sv.cbSize = sizeof(SCROLLINFO); 
    AppData.sv.fMask = SIF_ALL;
    AppData.sv.nPage = 1;  

    AppData.sh.cbSize = sizeof(SCROLLINFO); 
    AppData.sh.fMask = SIF_ALL;
    AppData.sh.nPage = 1;

    // Set the scroll bars
    SetScrollInfo( AppData.hMidWnd, SB_HORZ, &AppData.sh, FALSE );
    SetScrollInfo( AppData.hMidWnd, SB_VERT, &AppData.sv, FALSE );

    // Hide the scroll bars for the parent window
    ShowScrollBar( AppData.hMidWnd, SB_BOTH, FALSE );

    // Create the view window
    #ifndef EBOOK_TOOL // Viewer code


    // Load the background tile image if there is one
    LoadTiledBackground();

    // Initilize the zoom percent to 100
    AppData.ZoomPercent = ZoomSizes[ Settings.GetZoomIndex() ];
    ProjPrefsData.ZoomIndex = Settings.GetZoomIndex();

    // Font and background colors can't be changed in view program
    #ifndef SHARE_THE_MEMORIES
    // See if the default font has been changed and if so, set the LOGFONT
    // structure to the user defined font
    if( Settings.HasDefFontBeenChanged() )
    {
        // Copy over the font settings incase the customer has reset it.
        memcpy( &ProjPrefsData.DefFont, Settings.GetLogFont(), sizeof(LOGFONT));

        // Copy the font color incase that was changed
        ProjPrefsData.DefaultTxtColor = Settings.GetFontColor();
    }

    // See if the default font has been changed and if so use the new settings
    if( Settings.GetBackgrndTextColor() != NO_COLOR_SET )
        ProjPrefsData.DefBckgrdTxtColor = Settings.GetBackgrndTextColor();
    #endif


    // Enable the right tool bar button if there is more than one page
    if( PageList.Count() )
    {
        #ifndef SHARE_THE_MEMORIES
        // Set the current page to the last page viewed
        if( Settings.GetLastPage() < PageList.Count()  )
        {
            if( AppData.RegistrationMode == SHAREWARE_VERSION &&
                Settings.GetLastPage() < ProjPrefsData.AskForRegAfterPage )
                PageList.SetCurrentPage( Settings.GetLastPage() );
            else if( AppData.RegistrationMode == FULL_REG_VERSION )
                PageList.SetCurrentPage( Settings.GetLastPage() );
        }
        #endif  

        // Get the file type
        if( PageList.GetMainDataType( PageList.GetCurrentPage() ) == FILE_TYPE_TXT )
            AppData.pGBase = &TextBlock;
        else
            AppData.pGBase = &dib;

        // Try to load the first page
        if( LoadMainFile( hwnd, PageList.GetCurrentPage(), AppData.pGBase ) )
        {
            // Create Bitmaps for the bitmap objects only
            if( AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
            {
                // Create the GDI bitmap
                if( !dib.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_24 ) )
                    PostMsg( "Error", "Error creating view bitmap on startup." );

                // Delete the bitmap bits because we don't need them any more
                dib.DeleteBitmapBits();
            }

            // Enable the tool bar buttons
            if( PageList.Count() > 1 )
            {
                // Set the state of the flip buttons
                SetFlipButtons();
            }

            // Enable the flip buttons
            if( !Settings.GetSizeWndToFit() )
            {
                TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_IN, ENABLE );
                TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_OUT, ENABLE );
            }

            ViewWnd_CreateViewWnd( AppData.hInst, AppData.hMidWnd );
            UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );
        }
    }

    #else

    ViewWnd_CreateViewWnd( AppData.hInst, AppData.hMidWnd );

    #endif

    // Set the tool bar window to be on top
    BringWindowToTop( AppData.hToolBar );


    return TRUE;

}   // OnCreate



/************************************************************************
*    FUNCTION NAME:         CreateToolBar()                                                             
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

BOOL TEbookWnd::CreateToolBar( HWND hwnd )
{
    int i, TbButtonOffset;
    BOOL Result = FALSE;

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
        AppData.hEditCurPage = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, "edit", NULL, 
                                               WS_CHILD|WS_VISIBLE|WS_BORDER|ES_NUMBER|ES_RIGHT,
                                               TbButtonOffset, 2, 35, 22, AppData.hToolBar, NULL, 
                                               AppData.hInst, NULL );

        if( AppData.hEditCurPage )
            ApplyWin95Font( AppData.hEditCurPage );

         // Subclass Edit box control
        AppData.OldEditProc = (WNDPROC)SetWindowLong( AppData.hEditCurPage, GWL_WNDPROC, LONG( EdtSubclass_LoadPage ) );

        // Set the offset for the next item after the buttons
        TbButtonOffset += 42;

        // Add a static to the combo box
        AppData.hTBStatic = CreateWindow( "static", NULL, WS_CHILD|WS_VISIBLE|SS_LEFT,
                              TbButtonOffset, 6, 40, TBBUTTON_HEIGHT, AppData.hToolBar, NULL, AppData.hInst, NULL );

        if( AppData.hTBStatic )
            ApplyWin95Font( AppData.hTBStatic );

        // This feature is only for the share the wealth version
        #ifdef SHARE_THE_MEMORIES

        // Set the offset for the next item after the buttons
        TbButtonOffset += 42;

        AppData.hCaptionEdit = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, "edit", NULL, 
                                               WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE|WS_VSCROLL|ES_READONLY,
                                               TbButtonOffset, DEFAULT_CAPTION_Y, DEFAULT_CAPTION_WIDTH, DEFAULT_CAPTION_HEIGHT, AppData.hToolBar, NULL, 
                                               AppData.hInst, NULL );

        // Save the rect information for sizing of the caption window
        AppData.CaptionTxtRect.left = TbButtonOffset;
        AppData.CaptionTxtRect.top = DEFAULT_CAPTION_Y;
        AppData.CaptionTxtRect.right = DEFAULT_CAPTION_WIDTH;
        AppData.CaptionTxtRect.bottom = DEFAULT_CAPTION_HEIGHT;

        if( AppData.hCaptionEdit )
            ApplyWin95Font( AppData.hCaptionEdit );

        #endif

        Result = TRUE;
    }

    return Result;

}   // CreateToolBar



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

void TEbookWnd::SetWindowCaption( HWND hwnd )
{
    char StrTmp[MAX_TITLE + SMALL_ARRAY_50];

// Display the caption to show if this program is registered or shareware
#ifdef EBOOK_TOOL

    char FileNameOnly[MAX_PATH];

    // Init to a null string
    FileNameOnly[0] = 0;

    // Pull the file name from the path
    if( ActProj.FilePath[0] )
        CropFileNameFromPath( ActProj.FilePath, FileNameOnly );

    // Setup the window caption string
    sprintf( StrTmp, "%s  %s    %s", PRODUCT_NAME, PRODUCT_VERSION, FileNameOnly );

#else

    #ifndef SHARE_THE_MEMORIES
    // Are we using the registration number
    if( ProjPrefsData.UseRegNum )
    {
        if( AppData.RegistrationMode == FULL_REG_VERSION )
            sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, REGISTERED_STR );
        else
            sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, SHAREWARE_STR );
    }
    else
        strcpy( StrTmp, ProjPrefsData.Title );
    #else
        strcpy( StrTmp, ProjPrefsData.Title );
    #endif
#endif

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

void TEbookWnd::OnDestroy( HWND hwnd ) 
{
	mediaPlayer.Close();

    #ifndef EBOOK_TOOL

    // delete any temp files
    // Copy the path to add the extension
    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mp3" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".wav" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mid" );
    DeleteFile( AppData.SoundFileTmpPath );

    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".rmi" );
    DeleteFile( AppData.SoundFileTmpPath );

    #endif

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

void TEbookWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {   
        #ifdef EBOOK_TOOL
          
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
            if( DisplayPrefsDialogBox( AppData.hInst, hwnd, &ProjPrefsData, Settings.GetProjectArtPath() ) )
                ActProj.LiveProject = TRUE;
        break;

        case MNU_ADD_PAGE:
            mnuAddPage( hwnd );
        break;

        case MNU_DELETE_PAGE:
            DeletePage( hwnd );
        break;

        case MNU_MOVE_PAGE:
            MovePage( hwnd );
        break;

        case MNU_ADD_AVI:
            //mnuAddPage( hwnd );
        break;

        case MNU_ADD_SOUND:
            AddSound( hwnd );
        break;

        case MNU_REMOVE_SOUND:
            RemoveSound( hwnd );
        break;

        case MNU_BUILD_EXE:
            mnuBuild_EXE( hwnd );
        break;

        case MNU_HLP_CONTENTS:
            DisplayHelpFile( hwnd, HELP_CONTENTS, 0 );
        break;

        case MNU_HLP_INDEX:
            DisplayHelpFile( hwnd, HELP_PARTIALKEY, 0 );
        break;

        case MNU_REST_PROJECTS:
            ResetProjects( hwnd );
        break;

        case MNU_RUN_PROJECT:
            RunProject( hwnd );
        break;

        case MNU_FIND_PROJECT:
            FindProject( hwnd );
        break;

        #ifdef SHARE_THE_MEMORIES

        case MNU_ADD_CAPTION:
            if( DisplayCaptionDlg( AppData.hInst, hwnd, PageList.GetPtrToCaption( PageList.GetCurrentPage() ) ) )
                ActProj.LiveProject = TRUE;

            // Update the caption text window
            Edit_SetText( AppData.hCaptionEdit, PageList.GetPtrToCaption( PageList.GetCurrentPage() ) );
        break;

        #else

        case MNU_ADD_STARTUP_INFO:
            SetupInfoWnd( hwnd, "StartUp Info Window Setup", 
                                   ProjPrefsData.StartUpInfoCap, 
                                   ProjPrefsData.StartUpInfoTitle, STARTUP_INFO_TXT_FILE );
         break;

        case MNU_ADD_OF_INFO:
            SetupInfoWnd( hwnd, "Order Form Info Window Setup", 
                                   ProjPrefsData.OFInfoCap,
                                   ProjPrefsData.OFInfoTitle, ORDERFRM_INFO_TXT_FILE );
        break;

        #endif // SHARE_THE_MEMORIES

        #endif  // EBOOK_TOOL

        case MNU_PRINT_SETUP:
            mnuPrintSetup( hwnd );
        break;

        case MNU_PRINT:
            mnuPrint( hwnd );
        break;
         
        case MNU_EXIT:
            // Post the close message
            PostMessage( hwnd, WM_CLOSE, 0, 0 );
        break;
          
        case MNU_ABOUT:
            ShowAboutDlg( hwnd );
        break;
        
        case MNU_ORDER:
            mnuOrderForm( hwnd );
        break;

        case BTN_FLIP_LEFT:
            if( PageList.Count() && PageList.GetCurrentPage() > 0  )
                FlipPage( hwnd, PAGE_FLIP_LEFT );
        break;

        case BTN_FLIP_RIGHT:
            if( PageList.Count() && PageList.GetCurrentPage() < PageList.Count()-1 )
                FlipPage( hwnd, PAGE_FLIP_RIGHT );
        break;

        case BTN_ZOOM_IN:
            if( PageList.Count() )
                EnlargeReducePage( hwnd, ENLARGE_PAGE );
        break;

        case BTN_ZOOM_OUT:
            if( PageList.Count() )
                EnlargeReducePage( hwnd, REDUCE_PAGE );
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
            #ifdef EBOOK_TOOL
            ShellExecute( NULL, "open", WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
            #else
            ShellExecute( NULL, "open", ProjPrefsData.WebAddress, NULL, NULL, SW_SHOWNORMAL );
            #endif
        break;

        case POST_MSG_AFTER_WM_SHOW:
            HandlePostMeg( hwnd );
        break;

        case MNU_SET_DEF_FONT:
            ViewWnd_SetTextFont( hwnd );

            // set the scrool bars if needed
            SetScrollBars( AppData.hMidWnd );
        break;

        case MNU_SET_BKG_COLOR:
            ViewWnd_SetBackGrndColor( hwnd );
        break;

        case MNU_RESET_FONT_DEF:
            ViewWnd_ResetFontDefaults( hwnd );

            // set the scrool bars if needed
            SetScrollBars( AppData.hMidWnd );
        break;

        #ifndef EBOOK_TOOL

        case MNU_EXPORT_PAGE_FILE:
            ExportFile( hwnd, PageList.GetCurrentPage(), PAGE_TYPE_MAIN );
        break;

        case MNU_EXPORT_SOUND_FILE:
            ExportFile( hwnd, PageList.GetCurrentPage(), PAGE_TYPE_SOUND );
        break;

        case MNU_PLAY_ON_PAGE_TURN:
            if( IsMenuChecked( hwnd, MNU_PLAY_ON_PAGE_TURN ) )
            {
                CheckMenuItem( GetMenu( hwnd ), MNU_PLAY_ON_PAGE_TURN, MF_BYCOMMAND|MF_UNCHECKED );
                Settings.SetPlaySoundPageTurn( FALSE );
            }
            else
            {
                CheckMenuItem( GetMenu( hwnd ), MNU_PLAY_ON_PAGE_TURN, MF_BYCOMMAND|MF_CHECKED );
                Settings.SetPlaySoundPageTurn( TRUE );

                // Play the sound on this page if there is one
                if( PageList.Count() && !mediaPlayer.GetPlayError() && PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
                    PlaySoundFile( hwnd, PageList.GetCurrentPage() );
            }
        break;

        case MNU_AUTO_SIZE:

                if( IsMenuChecked( hwnd, MNU_AUTO_SIZE ) )
                {
                    CheckMenuItem( GetMenu( hwnd ), MNU_AUTO_SIZE, MF_BYCOMMAND|MF_UNCHECKED );
                    Settings.SetSizeWndToFit( FALSE );
                }
                else
                {
                    CheckMenuItem( GetMenu( hwnd ), MNU_AUTO_SIZE, MF_BYCOMMAND|MF_CHECKED );
                    Settings.SetSizeWndToFit( TRUE );
                }

                // No need to resize anything if a text block is showing
                if( AppData.pGBase && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
                {
                    // Size the view window
                    ViewWnd_ResetWnd( AppData.hMidWnd );

                    // Set the scrool bars if needed
                    SetScrollBars( AppData.hMidWnd );

                    // Update the controls
                    UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );
                }
        break;

        #endif
    }

}   // OnCommand



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

void TEbookWnd::HandlePostMeg( HWND hwnd )
{
    // Show the startup screen
    ShowStartUpScreen( hwnd );

    // Show the helpful hint window
    ShowHelpFulHint( hwnd );

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
    // Show the registration dialog the get the users registration number
    if( AppData.RegistrationMode == SHAREWARE_VERSION )
    {
        Sleep( 400 );
        // Returns true if they enter in the correct registration number
        // This is the same for the tool or the view program
        if( ShowRegDlg( hwnd ) )
        {
            // Set to full version mode
            AppData.RegistrationMode = FULL_REG_VERSION;

            // They are registered, Save the registration number to the secret ini file
            Settings.SetRegNum( AppData.RegistrationNo );

            // Display the caption to show if this program is registered or shareware
            SetWindowCaption( hwnd );
        }
    }
    #endif
}   // HandlePostMeg



/************************************************************************
*    FUNCTION NAME:         mnuOrderForm                                                             
*
*    DESCRIPTION:           Display the order form dialog box
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

void TEbookWnd::mnuOrderForm( HWND hwnd )
{
    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )

    // If they are a registered user, show the Order Form. Other wise
    // show the Registration Dialog box so they can enter in the registration number
    if( AppData.RegistrationMode == FULL_REG_VERSION )
    {
        // Only enable this option if the user has entered in a credit card url
        #ifndef EBOOK_TOOL
        if( ProjPrefsData.CreditWebAddress[0] )
        {
        #endif
            switch( DisplayOrderOptionDlg( GetInstance(), hwnd, CENTER_WND_TO_PARENT ) )
            {
                case ONLINE_WEB_ORDER:
                    #ifdef EBOOK_TOOL
                    ShellExecute( NULL, "open", ONLINE_ORDER_WEB_ADDRESS, NULL, NULL, SW_SHOWNORMAL );
                    #else
                    ShellExecute( NULL, "open", ProjPrefsData.CreditWebAddress, NULL, NULL, SW_SHOWNORMAL );
                    #endif
                break;

                case STANDARD_US_MAIL_ORDER:
                    ShowOrderFormDlg();
                break;
            }
        #ifndef EBOOK_TOOL
        }
        else
            ShowOrderFormDlg();
        #endif
    }                
    else
    {
        // Returns true if they enter in the correct registration number
        // This is the same for the tool or the view program
        if( ShowRegDlg( hwnd ) )
        {
            // Set to full version mode
            AppData.RegistrationMode = FULL_REG_VERSION;

            // They are registered, Save the registration number to the secret ini file
            Settings.SetRegNum( AppData.RegistrationNo );

            // Display the caption to show if this program is registered or shareware
            SetWindowCaption( hwnd );
        }
    }
    #endif
}   // mnuOrderForm



/************************************************************************
*    FUNCTION NAME:         mnuPrint                                                             
*
*    DESCRIPTION:           Display the print dialog box
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

void TEbookWnd::mnuPrint( HWND hwnd )
{
    #ifndef EBOOK_TOOL
    if( AppData.RegistrationMode == FULL_REG_VERSION )
    #endif
        Print( hwnd );
    #ifndef EBOOK_TOOL
    else
        PostMsg( "Printing", "Printing is only allowed when the ebook has been registered." );
    #endif
}   // mnuPrint


/************************************************************************
*    FUNCTION NAME:         mnuPrintSetup                                                             
*
*    DESCRIPTION:           Display the print setup dialog box
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

void TEbookWnd::mnuPrintSetup( HWND hwnd )
{
    #ifndef EBOOK_TOOL
    if( AppData.RegistrationMode == FULL_REG_VERSION )
    #endif
        Printer.DisplayPrinterSetup();
    #ifndef EBOOK_TOOL
    else
        PostMsg( "Printing", "Printing is only allowed when the ebook has been registered." );
    #endif

}   // mnuPrintSetup


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
#ifdef EBOOK_TOOL
void TEbookWnd::AddSound( HWND hwnd )
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

        // Enable the remove menu item
        EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_ENABLED );

        ActProj.LiveProject = TRUE;
    }
}   // AddSound
#endif


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
#ifdef EBOOK_TOOL
void TEbookWnd::DeletePage( HWND hwnd )
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
#endif



/************************************************************************
*    FUNCTION NAME:         RemoveSound                                                             
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
#ifdef EBOOK_TOOL
void TEbookWnd::RemoveSound( HWND hwnd )
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

        // Gray out the menu item
        EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_GRAYED|MF_DISABLED );

        ActProj.LiveProject = TRUE;
    }
}   // RemoveSound
#endif


/************************************************************************
*    FUNCTION NAME:         ExportPageFile                                                             
*
*    DESCRIPTION:           Saves the page data to a file
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
#ifndef EBOOK_TOOL
void TEbookWnd::ExportFile( HWND hwnd, int CurrentPage, int PageType )
{
    char TmpDirPath[ MAX_STR_PATH ], TmpFilePath[ MAX_STR_PATH ];
    char DialogCaption[ SMALL_ARRAY_50 ], Filter1[ SMALL_ARRAY_20 ];
    char DefExt[ 5 ];
    BYTE FileType;
    TLoadBuffer lb;
    BOOL FileLoadResult = FALSE;

    // Terminate the file path
    TmpFilePath[0] = 0;

    // Make a copy of the path to the exe file
    strcpy( TmpDirPath, AppData.Path );

    // Get the file type
    if( PageType == PAGE_TYPE_MAIN )
        FileType = PageList.GetMainDataType( CurrentPage );
    else if( PageType == PAGE_TYPE_SOUND )
        FileType = PageList.GetSoundDataType( CurrentPage );
    else if( PageType == PAGE_TYPE_AVI )
        FileType = PageList.GetAVIDataType( CurrentPage );

    // Setup according to file type
    switch( FileType )
    {
        case FILE_TYPE_BMP:
            strcpy( DialogCaption, "Export Windows BMP File" );
            strcpy( Filter1, "Windows BMP (*.bmp)\0*.bmp\0Bitmap & TXT Files (*.jpg,*.gif,*.bmp,*.txt)\0*.jpg;*.jpeg;*.gif;*.bmp;*.txt\0\0" );
            strcpy( DefExt, "bmp" );
        break;

        case FILE_TYPE_JPG:
            strcpy( DialogCaption, "Export JPEG File" );
            strcpy( Filter1, "JPEG Files (*.jpg)\0*.jpg\0Bitmap & TXT Files (*.jpg,*.gif,*.bmp,*.txt)\0*.jpg;*.jpeg;*.gif;*.bmp;*.txt\0\0" );
            strcpy( DefExt, "jpg" );
        break;

        case FILE_TYPE_GIF:
            strcpy( DialogCaption, "Export GIF File" );
            strcpy( Filter1, "GIF Files (*.gif)\0*.gif\0Bitmap & TXT Files (*.jpg,*.gif,*.bmp,*.txt)\0*.jpg;*.jpeg;*.gif;*.bmp;*.txt\0\0" );
            strcpy( DefExt, "gif" );
        break;

        case FILE_TYPE_TXT:
            strcpy( DialogCaption, "Export Text File" );
            strcpy( Filter1, "TEXT Files (*.txt)\0*.txt\0Bitmap & TXT Files (*.jpg,*.gif,*.bmp,*.txt)\0*.jpg;*.jpeg;*.gif;*.bmp;*.txt\0\0" );
            strcpy( DefExt, "txt" );
        break;

        case FILE_TYPE_WAV:
            strcpy( DialogCaption, "Export WAVE File" );
            strcpy( Filter1, "WAVE Files (*.wav)\0*.wav\0Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0\0" );
            strcpy( DefExt, "wav" );
        break;

        case FILE_TYPE_MP3:
            strcpy( DialogCaption, "Export MP3 File" );
            strcpy( Filter1, "MP3 Files (*.mp3)\0*.mp3\0Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0\0" );
            strcpy( DefExt, "mp3" );
        break;

        case FILE_TYPE_MID:
            strcpy( DialogCaption, "Export MIDI File" );
            strcpy( Filter1, "MIDI Files (*.mid)\0*.mid\0Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0\0" );
            strcpy( DefExt, "mid" );
        break;

        case FILE_TYPE_RMI:
            strcpy( DialogCaption, "Export MIDI RMI File" );
            strcpy( Filter1, "MIDI RMI Files (*.rmi)\0*.rmi\0Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0\0" );
            strcpy( DefExt, "rmi" );
        break;

        case FILE_TYPE_AVI:
            strcpy( DialogCaption, "Export AVI File" );
            strcpy( Filter1, "AVI Files (*.avi)\0*.avi\0Sound Files (*.mp3, *.wav, *.mid, *.rmi)\0*.mp3;*.wav;*.mid;*.rmi\0\0" );
            strcpy( DefExt, "avi" );
        break;
    }

    // Show the windows save file box
    if( GetFileName( hwnd, AppData.hInst, TmpFilePath, DialogCaption,
                     TmpDirPath, Filter1, DefExt, SAVE_FILENAME_DIALOG_BOX ) )
    {
        // Load in the data from the exe file
        if( PageType == PAGE_TYPE_MAIN )
            FileLoadResult = LoadFileDataFromEXEfile( PageList.GetMainDataOffset( CurrentPage ), &lb );
        else if( PageType == PAGE_TYPE_SOUND )
            FileLoadResult = LoadFileDataFromEXEfile( PageList.GetSoundDataOffset( CurrentPage ), &lb );
        else if( PageType == PAGE_TYPE_AVI )
            FileLoadResult = LoadFileDataFromEXEfile( PageList.GetAVIDataOffset( CurrentPage ), &lb );

        if( FileLoadResult )
        {
            // Text is encrypted so we need to unencrypt it
            if( FileType == FILE_TYPE_TXT )
            {
                char EncyKey[500];
                // Make an encryption key
                sprintf( EncyKey, "%st72m.a%si02sxlm%s4yk3aqvm%s49p1`,'t%sykw3nay", 
                         ProjPrefsData.Title, ProjPrefsData.RegNo, ProjPrefsData.NameOrCompany,
                         ProjPrefsData.WebAddress, ProjPrefsData.EmailAddress );

                // Unencrypt the text
                LockItUp( (BYTE *)EncyKey, lb.pData, lb.Size, UNENCRYPT_DATA );
            }

            // Write the data to the file
            WriteDataToFile( TmpFilePath, lb.pData, lb.Size, CREATE_ALWAYS );

            // Delete the buffer because we are done with it
            delete [] lb.pData;
        }
    }

}   // ExportPageFile
#endif


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

void TEbookWnd::PlaySoundFile( HWND hwnd, WORD PageToLoad )
{
    if( mediaPlayer.GetStatus() == PLAYER_PAUSED )
        mediaPlayer.Play();
    else
    {
        // No need to restart the same music playing
        if( AppData.CurrentPageSoundPlay == PageToLoad &&
            mediaPlayer.GetStatus() == PLAYER_PLAYING )
            return;
        else
        {
            // If the player is not closed, close it
            if( mediaPlayer.GetStatus() != PLAYER_CLOSED )
                mediaPlayer.Close();
        }

#ifdef EBOOK_TOOL

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

        // Play the sound
        mediaPlayer.Play( PageList.GetSoundDataPath( PageToLoad ) );

        // Record the page that is playing the sound
        AppData.CurrentPageSoundPlay = PageToLoad;
#else
        TLoadBuffer lb;

        // Load the data into the buffer
        if( LoadFileDataFromEXEfile( PageList.GetSoundDataOffset( PageToLoad ), &lb ) )
        {
            // Attach the correct file extension to the file name
            switch( lb.Type )
            {
                case FILE_TYPE_MP3:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mp3" );
                break;

                case FILE_TYPE_WAV:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".wav" );
                break;

                case FILE_TYPE_MID:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".mid" );
                break;

                case FILE_TYPE_RMI:
                    RelpaceFileExtension( NULL, AppData.SoundFileTmpPath, ".rmi" );
                break;                  
            }

            // Write the music data to a temp file
            if( WriteDataToFile( AppData.SoundFileTmpPath, lb.pData, lb.Size, CREATE_ALWAYS ) )
            {
                // Play the music
                mediaPlayer.Play( AppData.SoundFileTmpPath );

                // Record the page that is playing the sound
                AppData.CurrentPageSoundPlay = PageToLoad;
            }

            // Delete the buffer because we are done with it
            delete [] lb.pData;
        }
#endif
    }

}   // PlaySoundFile



/************************************************************************
*    FUNCTION NAME:         DisplayHelpFile                                                             
*
*    DESCRIPTION:           Resets all the projects to act as unregistered
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
#ifdef EBOOK_TOOL
void TEbookWnd::ResetProjects( HWND hwnd )
{
    char SecretIniFilePath[MAX_STR_PATH];

    if( MessageBox( hwnd, "This will reset all your projects to their default mode. This is so that you can experiment with the projects you created. Do you want to reset your projects to default mode?",
        "Reset All Projects", MB_YESNO ) == IDYES )
    {
        // Setup the path to the ini file
        GetTheWindowsDirectory( SecretIniFilePath );
        strcat( SecretIniFilePath, INI_SECRET_REG_FILE );

        // Delete the file
        DeleteFile( SecretIniFilePath );

        // Create a new secret registration file with out info in it
        Settings.WriteSecretIniFile( INI_SECRET_REG_FILE, PRODUCT_NAME );

        // Add in a little delay
        Sleep( 500 );

        // Inform them that the job is done
        PostMsg( "Completed", "All projects have been reset to shareware mode." );
    }   
}   // ResetProjects
#endif


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
#ifdef EBOOK_TOOL
void TEbookWnd::DisplayHelpFile( HWND hwnd, UINT Command, DWORD Index )
{
    char HelpFilePath[MAX_STR_PATH];

    // Build the path to the help file
    CropOutPath( AppData.Path, HelpFilePath );
    strcat( HelpFilePath, HELP_FILE_NAME );
    
    // show the help file
    WinHelp( hwnd, HelpFilePath, Command, Index );

}   // DisplayHelpFile
#endif


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

BOOL TEbookWnd::Print( HWND hwnd )
{
    LPPRINTDLG ppdlg;
    int i, j, DataType;
    RECT rect;
    double PrintableW, PrintableH, MultiW, MultiH, Multiplyer;
    double ImageW, ImageH, MarginX, MarginY;
    HWND hPrintingWnd;
    char PageNumber[20];
    BOOL Result = FALSE, LoadResult;
    TDIB PrintDib;
    TTextBlock PrintTextBlock;

    if( PageList.Count() )
    {
        ppdlg = Printer.DisplayPrintDlg( PageList.GetCurrentPage()+1, PageList.Count() );

        if( ppdlg )
        {
            if( Printer.BeginDoc( "Print page(s)" ) )
            {
                // Set the printer abort function
                SetAbortProc( Printer.Handle(), (ABORTPROC)PrintCancelDlg_AbortFunc );

                // Using CreateDialog causes the dialog to return rightaway.
                hPrintingWnd = DisplayPrintCancelDlg( GetInstance(), hwnd, "Printing Page...", TRUE );
            
                for( i = ppdlg->nFromPage; i < ppdlg->nToPage+1; ++i )
                {
                    // Load the bitmap or text file
                    DataType = PageList.GetMainDataType( i-1 );

                    if( DataType == FILE_TYPE_TXT )
                        LoadResult = LoadMainFile( hwnd, i-1, &PrintTextBlock );
                    else
                        LoadResult = LoadMainFile( hwnd, i-1, &PrintDib );

                    if( !LoadResult )
                    {
                        PostMsg( "Printing Error", "Error loading page to print." );
                        goto ERROR_JUMP;
                    }

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
                    if( DataType == FILE_TYPE_BMP || DataType == FILE_TYPE_JPG || DataType == FILE_TYPE_GIF )
                    {
                        ImageW = PrintDib.width();
                        ImageH = PrintDib.height();

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
                    }

                    // Make the font
                    Printer.SetFont( "Arial", 8 );

                    for( j = 0; j < ppdlg->nCopies; ++j )
                    {
                        // Jump out if the user has canceled the print job
                        if( !PrintCancelDlg_GetPrintCancel() )
                            goto ERROR_JUMP;

                        Printer.BeginPage();

                        // If we are using a custom font, we'll need to remake the other font as well
                        if( ProjPrefsData.DefFont.lfFaceName[0] )
                            Printer.SetFont( "Arial", 8 );

                        // Print out the page number on the bottom center of the screen
                        if( ProjPrefsData.PrintPageNumbers )
                        {
                            sprintf( PageNumber, "(%u)", i );
                            Printer.PrintText( (Printer.width()-Printer.TextWidth( PageNumber )) / 2, 
                                       Printer.height() - Printer.TextHeight( PageNumber ) - (MarginY / 2), PageNumber );
                        }

                        // Print out the copyright info on the top of the page
                        if( ProjPrefsData.CopyrightInfo[0] )
                            Printer.PrintText( MarginX / 2, MarginY / 2, ProjPrefsData.CopyrightInfo );

                        // Are we using a custom font
                        if( ProjPrefsData.DefFont.lfFaceName[0] )
                            Printer.SetFont( NULL, 0, 0, 0, 0, &ProjPrefsData.DefFont );

                        // This could be a text file or a graphic
                        if( DataType == FILE_TYPE_TXT )
                            Printer.WrapText( PrintTextBlock.GetPointerToData(), MarginX, MarginY, PrintableW, PrintableH, 0, LEFT_RIGHT_JUSTIFY );
                        else
                            Printer.PrintBitmap( &PrintDib, &rect );

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
    DestroyWindow( hPrintingWnd );

    // Free all printer objects
    Printer.Free();

    return Result;

}   // Print



/************************************************************************
*    FUNCTION NAME:         RunProject                                                             
*
*    DESCRIPTION:           Builds and Runs the project
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
#ifdef EBOOK_TOOL
void TEbookWnd::RunProject( HWND hwnd )
{
    char PathToEXE[MAX_STR_PATH];
    char PathToDir[MAX_PATH];
    BOOL BuildResult = TRUE;
    HWND ViewWindow;

    // See if they have a project exe running. If so, kill the program
    // because we can't save the EXE while the other EXE is running.
    #ifndef SHARE_THE_MEMORIES

    char StrTmp[MAX_INFO+20];

    // Are we using the registration number
    if( ProjPrefsData.UseRegNum )
    {
        // See if they are running the shareware version
        sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, SHAREWARE_STR );
        ViewWindow = FindWindow( NULL, StrTmp );

        // If not try the registered version
        if( !ViewWindow )
        {
            sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, REGISTERED_STR );
            ViewWindow = FindWindow( NULL, StrTmp );
        }
    }
    else
    {
        ViewWindow = FindWindow( NULL, ProjPrefsData.Title );
    }
    #else
        ViewWindow = FindWindow( NULL, ProjPrefsData.Title );
    #endif
 
    if( ViewWindow )
    {
        PostMessage( ViewWindow, WM_CLOSE, 0, 0 );
        Sleep( 500 );
    }

    // Do we want to build the EXE?
    if( ActProj.LiveProject )
        BuildResult = mnuBuild_EXE( hwnd );

    if( BuildResult && PageList.Count() )
    {
        // Put together the path to the new exe file
        AddFileToOtherPath( ProjPrefsData.ExeName, ActProj.FilePath, PathToEXE );

        // Pull the directory from the file path
        CropOutPath( ActProj.FilePath, PathToDir );

        // Check to see if the file is there. If not, build it.
        if( !FileExist( PathToEXE ) )
            if( !mnuBuild_EXE( hwnd ) )
                return;

        // If the function fails, the return value is an error value that is less than or equal to 32
        // The last aparam is a 0 to keep the DOS window from showing.
        if( ShellExecute( hwnd, "open", PathToEXE, NULL, PathToDir, SW_SHOWNORMAL ) <= (HINSTANCE)32 )
            PostMsg( "Opps","There has been an error running the project executable." );
    }

}   // RunProject  
#endif



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
#ifdef EBOOK_TOOL
void TEbookWnd::DeleteCurPage( HWND hwnd )
{
    WORD CurentPage;

    // Get the current page
    CurentPage = PageList.GetCurrentPage();

    // Hide the text window if this is text
    if( PageList.GetMainDataType( CurentPage ) == FILE_TYPE_TXT )
    {
        // Free the text
        TextBlock.FreeBlock();
    }

    // Delete the current page
    PageList.Delete( CurentPage );

    if( CurentPage < PageList.Count() )
        PageList.SetCurrentPage( CurentPage );
    else if( PageList.Count() )
        PageList.SetCurrentPage( CurentPage-1 );

    FlipPage( hwnd, 0, TRUE );

}   // DeleteCurPage
#endif



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
#ifdef EBOOK_TOOL
void TEbookWnd::MovePage( HWND hwnd )
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
#endif


/************************************************************************
*    FUNCTION NAME:         LoadTiledBackground                                                             
*
*    DESCRIPTION:           Loads the tiled background
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
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
#ifndef EBOOK_TOOL
void TEbookWnd::LoadTiledBackground()
{
    TLoadBuffer lb;
    TGif gif;
    TJPEG jpeg;

    if( ProjPrefsData.BackgroundBMPOffset )
    {
        // If there was an error, keep them from going to any other pages
        // Also, get the data type so we know what to do with the buffer
        
        if( LoadFileDataFromEXEfile( ProjPrefsData.BackgroundBMPOffset, &lb ) )
        {
            // Load the file
            // Give the pointer over to the dib class. It will delete it when it is done
            switch( lb.Type )
            {
                case FILE_TYPE_BMP:
                    // Normally we don't delete the uncompressed buffer because it ends up being owned by the
                    // DIB class and get freed there when done but because there was an error, we free everything.
                    if( !TileBackDIB.LoadFromBuffer( lb.pData ) )
                        delete [] lb.pData;
                    break;

                case FILE_TYPE_JPG:
                    TileBackDIB.LoadFromBuffer( jpeg.LoadFromBuffer( lb.pData, lb.Size ) );
                    break;

                case FILE_TYPE_GIF:
                    TileBackDIB.LoadFromBuffer( gif.LoadFromBuffer( lb.pData, lb.Size ) );
                    break;
            }

            // Create the GDI bitmap
            if( !TileBackDIB.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_24 ) )
                PostMsg( "Error", "Error creating bitmap on load tile." );

            // Delete the bitmap bits because we don't need them any more
            TileBackDIB.DeleteBitmapBits();
        }
    }

}   // LoadTiledBackground
#endif



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

void TEbookWnd::ShowStartUpScreen( HWND hwnd )
{
    // Display the startup screen
    #ifdef EBOOK_TOOL
    HWND hStartUpScrnWnd = DisplayStartUpScreen( AppData.hInst, hwnd, "startup", "GIF", NULL );
    #else

    HWND hStartUpScrnWnd = NULL;
    TLoadBuffer lb;
    BYTE *pData;
    TGif gif;
    TJPEG jpeg;

    // Use the below function to load the startup screen
    if( ProjPrefsData.StartUpScrnBMPOffset )
    {
        // Load the startup screen from within the file
        if( LoadFileDataFromEXEfile( ProjPrefsData.StartUpScrnBMPOffset, &lb ) )
        {
            // Load the file
            // The pointer passed from LoadFileDataFromEXEfile is deleted in the LoadFromBuffer
            switch( lb.Type )
            {
                case FILE_TYPE_BMP:
                    pData = lb.pData;
                    break;

                case FILE_TYPE_JPG:
                    pData = jpeg.LoadFromBuffer( lb.pData, lb.Size );
                    break;

                case FILE_TYPE_GIF:
                    pData = gif.LoadFromBuffer( lb.pData, lb.Size );
                    break;

                case FILE_TYPE_META:
                break;
            }

            // Give the pointer over to the dib class. It will delete it when it is done
            if( pData )
            {
                // Give a slight delay before showing the startup screen
                Sleep( 500 );

                // Show the startup screen
                hStartUpScrnWnd = DisplayStartUpScreen( AppData.hInst, hwnd, NULL, NULL, pData );
            }
        }
    }
    #endif

    // Did we get a window?
    if( hStartUpScrnWnd )
    {
        // Hold the program for a few seconds to display the startup screen
        Sleep( 3500 );
        
        // Destroy the startup screen
        DestroyWindow( hStartUpScrnWnd );
    }

    // Show the small startup screen for the view program
    #ifndef EBOOK_TOOL

    // Let the other screen disapear before showing the small ebook 4 sale
    Sleep( 500 );

    hStartUpScrnWnd = DisplayStartUpScreen( AppData.hInst, hwnd, "SmallStartup", "GIF", NULL );

    // Did we get a window?
    if( hStartUpScrnWnd )
    {
        // Hold the program for a few seconds to display the startup screen
        Sleep( 2500 );
        
        // Destroy the startup screen
        DestroyWindow( hStartUpScrnWnd );
    }

    #endif

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

void TEbookWnd::ShowAboutDlg( HWND hwnd )
{
    // Display the startup screen
    #ifdef EBOOK_TOOL
    DisplayAboutDlg( AppData.hInst, hwnd, ABOUT_CAPTION, ABOUT_MSG, "startup", "GIF", NULL );
    #else

    char  AboutCaption[MAX_TITLE+10];
    TLoadBuffer lb;
    BYTE *pData;
    TGif gif;
    TJPEG jpeg;

    // Setup the about caption using their product name
    sprintf( AboutCaption, "%s %s", "About", ProjPrefsData.Title );

    // Use the below function to load the startup screen
    if( ProjPrefsData.StartUpScrnBMPOffset )
    {
        // Load the startup screen from within the file
        if( LoadFileDataFromEXEfile( ProjPrefsData.StartUpScrnBMPOffset, &lb ) )
        {
            // Load the file
            // The pointer passed from LoadFileDataFromEXEfile is deleted in the LoadFromBuffer
            switch( lb.Type )
            {
                case FILE_TYPE_BMP:
                    pData = lb.pData;
                    break;

                case FILE_TYPE_JPG:
                    pData = jpeg.LoadFromBuffer( lb.pData, lb.Size );
                    break;

                case FILE_TYPE_GIF:
                    pData = gif.LoadFromBuffer( lb.pData, lb.Size );
                    break;

                case FILE_TYPE_META:
                break;
            }

            // Give the pointer over to the dib class. It will delete it when it is done
            if( pData )
                DisplayAboutDlg( AppData.hInst, hwnd, AboutCaption, ABOUT_MSG_VIEW, NULL, NULL, pData );
        }
    }
    else
        DisplayAboutDlg( AppData.hInst, hwnd, AboutCaption, ABOUT_MSG_VIEW, "SmallStartup", "GIF", NULL );
    #endif
     
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

void TEbookWnd::ShowHelpFulHint( HWND hwnd )
{
    // Display the helpful hint window
    #ifdef EBOOK_TOOL

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
    #else

    #ifndef SHARE_THE_MEMORIES

    BOOL ShowNextTime;
    TLoadBuffer lb;

    // Do they want to see the helpful hint window?
    if( ProjPrefsData.InfoWndTXTOffset && Settings.GetIntroHelpWnd() )
    {
        if( LoadFileDataFromEXEfile( ProjPrefsData.InfoWndTXTOffset, &lb ) )
        {
            Sleep( 400 );

            DisplayHelpHintDialogBox( AppData.hInst, hwnd, NULL, NULL, ProjPrefsData.StartUpInfoCap,
                                  ProjPrefsData.StartUpInfoTitle, &ShowNextTime, DONT_DO_CHECK_SUM, 
                                  (char *)lb.pData, 0, CENTER_WND_TO_PARENT );

            Settings.SetIntroHelpWnd( ShowNextTime );

            // The buffer needs to be deleted because we are done with it.
            delete [] lb.pData;
        }
        else
            PostMsg( "Error", "Error loading startup info text." );
    }
    #endif

    #endif

}   // ShowHelpFulHint



/************************************************************************
*    FUNCTION NAME:         ShowRegDlg                                                             
*
*    DESCRIPTION:           Shows the registration number dialog
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

#if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
BOOL TEbookWnd::ShowRegDlg( HWND hwnd )
{
    char CostStr[ MAX_REG_FEE ];
    char InfoString[ LARGE_ARRAY_500 ];
    BOOL Result = FALSE;

    #ifdef EBOOK_TOOL

        // Setup the registration cost
        sprintf( CostStr, "Registration Fee: $%.2lf", PRODUCT_COST );

        // Load the info string from the resource file
        LoadString( AppData.hInst, REG_NUM_INFO_TEXT_TOOL, InfoString, sizeof(char)*LARGE_ARRAY_500 );

        // Show the registration window which checks the registration against the number they keyed in
        if( DisplayRegNumDlg( AppData.hInst, hwnd, "Registration", "Shareware Mode",
                          CostStr, InfoString, AppData.RegistrationNo, ONLINE_ORDER_WEB_ADDRESS, CENTER_WND_TO_PARENT ) )
        {
            Result = TRUE;
        }

    #else

        // Setup the registration cost
        sprintf( CostStr, "Registration Fee: $%.2lf", ProjPrefsData.RegFee );

        // Load the info string from the resource file
        LoadString( AppData.hInst, REG_NUM_INFO_TEXT_VIEW_PROG, InfoString, sizeof(char)*LARGE_ARRAY_500 );

        // Show the registration window which checks the registration against the number they keyed in
        if( DisplayRegNumDlg( AppData.hInst, hwnd, "Registration", "Shareware Mode",
                          CostStr, InfoString, AppData.RegistrationNo, ProjPrefsData.CreditWebAddress, CENTER_WND_TO_PARENT ) )
        {
            Result = TRUE;
        }
    #endif

    return Result;

}   // ShowRegDlg
#endif


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

#ifdef EBOOK_TOOL
void TEbookWnd::mnuNew( HWND hwnd )
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

        // Copy over the file path because the SS2GO_InitProject clears it out
        strcpy( TmpFileName, ActProj.FilePath );

        // Init a new project
        InitProject( hwnd );

        // Copy back the file name
        strcpy( ActProj.FilePath, TmpFileName );

        // Save the file
        if( SaveToFile( hwnd, ActProj.FilePath ) )
        {
            // Set the scrool bars if needed
            SetScrollBars( AppData.hMidWnd );

            // Force a redraw to erase the save window
            ViewWnd_ResetWnd( AppData.hMidWnd );

            if( DisplayPrefsDialogBox( AppData.hInst, hwnd, &ProjPrefsData, Settings.GetProjectArtPath() ) )
            {
                // Give this project a random number it can use for checking
                // registration. Helps keep similar projects different
                ProjPrefsData.ViewProgramRandNo = rand() %  0xFFFFFFF;

                // Change the window caption to reflect the new loaded file
                SetWindowCaption( hwnd );

                // Enable all the menu items
                EnableMenuItems( hwnd, TRUE );

                ActProj.LiveProject = TRUE;

                // Size the view window
                ViewWnd_ResetWnd( AppData.hMidWnd );

                // Update the controls
                UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );
            }
        }
    }

}   // mnuNew
#endif



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

#ifdef EBOOK_TOOL
void TEbookWnd::mnuOpen( HWND hwnd )
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

        // Hide the window by moving it up and out of the way
        MoveWindow( AppData.hViewWnd,0, 0, 1, 1, TRUE );

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

                // Get the file type
                if( PageList.GetMainDataType( PageList.GetCurrentPage() ) == FILE_TYPE_TXT )
                    AppData.pGBase = &TextBlock;
                else
                    AppData.pGBase = &dib;

                // Load the first page
                if( LoadMainFile( hwnd, PageList.GetCurrentPage(), AppData.pGBase ) )
                {
                    // Create Bitmaps for the bitmap objects only
                    if( AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
                    {
                        // Create the GDI bitmap
                        if( !dib.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_24 ) )
                        {
                            PostMsg( "Error", "Error creating bitmap on open (%s).", 
                                     PageList.GetMainDataPath( PageList.GetCurrentPage() ) );
                            return;
                        }

                        // Delete the bitmap bits because we don't need them any more
                        dib.DeleteBitmapBits();
                    }
                }
                else
                    return; // we had an error loading an image
            }

            // The tool program doesn't have this information until a file is opened.
            // The view program has this information on startup.
            // If a new font has been selected, make the font
            // We can tell this if a font name has been saved
            if( ProjPrefsData.DefFont.lfFaceName[0] )
            {
                // Make the new font
                // Setting the color here does nothing for Windows controls. For
                // changing colors in controls, see ViewWnd_OnCtlColor.
                TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
                                       0, 0, &ProjPrefsData.DefFont );
            }

            // Enable the tool bar buttons if there is more than one page
            if( PageList.Count() > 1 )
                TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, ENABLE );

            if( PageList.Count() > 0 )
            {
                TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_IN, ENABLE );
                TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_OUT, ENABLE );
                EnableSpecificMenuItems( hwnd, TRUE );
            }

            // Update the interface
            UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

            // Enable all the menu items
            EnableMenuItems( hwnd, TRUE );

            // Change the window caption to reflect the new loaded file
            SetWindowCaption( hwnd );

            ActProj.OpenedFile = TRUE;

            // Set the zoom percent
            AppData.ZoomPercent = ZoomSizes[ ProjPrefsData.ZoomIndex ];

            // Size the view window
            ViewWnd_ResetWnd( AppData.hMidWnd );

            // set the scrool bars if needed
            SetScrollBars( AppData.hMidWnd );

            // Strip the file name off of the path          
            CropOutPath( ActProj.FilePath, TmpProjFile );

            // Save the file open path for next time
            Settings.SetProjectPath( TmpProjFile );
        }
    }

}   // mnuOpen
#endif


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

void TEbookWnd::OnShowWindow( HWND hwnd, BOOL fShow, UINT status )
{
    if( PageList.Count() )
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );
    else
        UpdateControls( hwnd, 0, 0 );

    #ifndef EBOOK_TOOL
    if( Settings.GetPlaySoundPageTurn() )
    {
        if( !mediaPlayer.GetPlayError() && PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
            PlaySoundFile( hwnd, PageList.GetCurrentPage() );
    }
    #endif

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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::mnuSave( HWND hwnd )
{
    BOOL Result = FALSE;
    HWND ViewWindow;

    // See if they have a project exe running. If so, kill the program
    // because we can't save the EXE while the other EXE is running.
    #ifndef SHARE_THE_MEMORIES

    char StrTmp[MAX_INFO+20];

    // Are we using the registration number
    if( ProjPrefsData.UseRegNum )
    {
        // See if they are running the shareware version
        sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, SHAREWARE_STR );
        ViewWindow = FindWindow( NULL, StrTmp );

        // If not try the registered version
        if( !ViewWindow )
        {
            sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, REGISTERED_STR );
            ViewWindow = FindWindow( NULL, StrTmp );
        }
    }
    else
    {
        ViewWindow = FindWindow( NULL, ProjPrefsData.Title );
    }
    #else
        ViewWindow = FindWindow( NULL, ProjPrefsData.Title );
    #endif

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
        ActProj.LiveProject = FALSE;

    return Result;
    
}   // mnuSave
#endif



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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::mnuSaveAs( HWND hwnd )
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
#endif



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

#ifdef EBOOK_TOOL
void TEbookWnd::mnuClose( HWND hwnd )
{
    // Disanable all the menu items
    if( CheckToSave( hwnd ) )
    {
        InitProject( hwnd );

        // Hide the window by moving it up and out of the way
        MoveWindow( AppData.hViewWnd,0, 0, 1, 1, TRUE );
    }

}   // mnuClose
#endif


/************************************************************************
*    FUNCTION NAME:         EnableMenuItems                                                             
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

#ifdef EBOOK_TOOL
void TEbookWnd::EnableMenuItems( HWND hwnd, BOOL Enable )
{
    EnableMenuItem( GetMenu(hwnd), MNU_SAVE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_SAVEAS, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_CLOSE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_PREFS, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_PAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

    EnableMenuItem( GetMenu(hwnd), MNU_SET_DEF_FONT, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_SET_BKG_COLOR, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_RESET_FONT_DEF, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

    // Menu items not available in Share the Memories version
    #ifndef SHARE_THE_MEMORIES
    EnableMenuItem( GetMenu(hwnd), MNU_BUILD_EXE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_STARTUP_INFO, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_OF_INFO, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_REST_PROJECTS, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    #endif

}   // EnableMenuItems
#endif



/************************************************************************
*    FUNCTION NAME:         EnableSpecificMenuItems                                                             
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

#ifdef EBOOK_TOOL
// Enable or disable menu items specific to having pages loaded
void TEbookWnd::EnableSpecificMenuItems( HWND hwnd, BOOL Enable )
{
    //EnableMenuItem( GetMenu(hwnd), MNU_ADD_AVI, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

    EnableMenuItem( GetMenu(hwnd), MNU_DELETE_PAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_MOVE_PAGE, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_PRINT, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_PRINT_SETUP, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_SOUND, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_RUN_PROJECT, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    EnableMenuItem( GetMenu(hwnd), MNU_FIND_PROJECT, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

    // Menu items only available in Share the Memories version
    #ifdef SHARE_THE_MEMORIES
    EnableMenuItem( GetMenu(hwnd), MNU_ADD_CAPTION, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );
    #endif

}   // EnableSpecificMenuItems
#endif



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

void TEbookWnd::FlipPage( HWND hwnd, int Direction, BOOL LoadSpecificPage )
{
    int i;

#if !defined( EBOOK_TOOL ) && !defined( SHARE_THE_MEMORIES )
    // Show the registration dialog the get the users registration number
    if( AppData.RegistrationMode == SHAREWARE_VERSION &&
        ProjPrefsData.AskForRegAfterPage != 0 &&
        PageList.GetCurrentPage() >= ProjPrefsData.AskForRegAfterPage-1 )
    {
        if( Direction == PAGE_FLIP_RIGHT )
        {
            // Disable the 
            TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, DISABLE );

            // Enable the Left flip button just incase
            if( PageList.GetCurrentPage() > 0 )
                TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT, ENABLE );

            // Show the registration dialog box
            // Returns true if they enter in the correct registration number
            // This is the same for the tool or the view program
            if( ShowRegDlg( hwnd ) )
            {
                // Set to full version mode
                AppData.RegistrationMode = FULL_REG_VERSION;

                // They are registered, Save the registration number to the secret ini file
                Settings.SetRegNum( AppData.RegistrationNo );

                // Enable the right flip button
                if( PageList.GetCurrentPage() < PageList.Count()-1 )
                    TBBtn_SetState( AppData.hToolBar, BTN_FLIP_RIGHT, ENABLE );

                // Display the caption to show if this program is registered or shareware
                SetWindowCaption( hwnd );
            }

            return;
        }
        else if( Direction == PAGE_FLIP_LEFT )
        {
            // Reset the current page
            PageList.SetCurrentPage( ProjPrefsData.AskForRegAfterPage-1 ); 
        }
    }
#endif

    // Are we just flipping pages or loading a specific page
    if( PageList.Count() && !LoadSpecificPage )
    {
        // Flip the page
        if( Direction == PAGE_FLIP_RIGHT )
            PageList.NextPage();
        else
            PageList.PreviousPage();
    }

    // Set the flip button state
    SetFlipButtons();

    // Get the file type
    if( PageList.GetMainDataType( PageList.GetCurrentPage() ) == FILE_TYPE_TXT )
        AppData.pGBase = &TextBlock;
    else
        AppData.pGBase = &dib;
             
    // Load the next page
    if( PageList.Count() )
    {
        if( LoadMainFile( hwnd, PageList.GetCurrentPage(), AppData.pGBase ) )
        {
            // Create Bitmaps for the bitmap objects only
            if( AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
            {
                // Create the GDI bitmap
                if( !dib.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_24 ) )
                    PostMsg( "Error", "Error creating bitmap on FlipPage (%s)." );

                // Delete the bitmap bits because we don't need them any more
                dib.DeleteBitmapBits();
            }
        }
        else
        {
            // If there was an error, disable all the buttons
            for( i = 0; i < NUM_TBBUTTONS; ++i )
                TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT + i, DISABLE );

            #ifdef EBOOK_TOOL
            EnableSpecificMenuItems( hwnd, FALSE );
            #endif

            return;
        }
    }

    // Disable the buttons if there are no pages to show
    #ifdef EBOOK_TOOL
    if( !PageList.Count() )
    {
        for( i = 0; i < NUM_TBBUTTONS; ++i )
            TBBtn_SetState( AppData.hToolBar, BTN_FLIP_LEFT + i, DISABLE );
        EnableSpecificMenuItems( hwnd, FALSE );
        UpdateControls( hwnd, 0, 0 );
        dib.Free();
    }
    else
    #endif
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );   

    // Play the sound associated with this page
    #ifndef EBOOK_TOOL
    if( IsMenuChecked( hwnd, MNU_PLAY_ON_PAGE_TURN ) && !mediaPlayer.GetPlayError() && 
        PageList.GetSoundDataType( PageList.GetCurrentPage() ) )
    {
        // Play the sound assocaited with this page
        PlaySoundFile( hwnd, PageList.GetCurrentPage() );
    }
    #endif

    // Size the view window
    ViewWnd_ResetWnd( AppData.hMidWnd );

    // set the scrool bars if needed
    SetScrollBars( AppData.hMidWnd );
            
}   // FlipPage



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

void TEbookWnd::SetFlipButtons()
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
*    FUNCTION NAME:         LoadMainFile                                                             
*
*    DESCRIPTION:           Load the page bitmap or text
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

int TEbookWnd::LoadMainFile( HWND hwnd, WORD PageToLoad, PTGBase pBase )
{
    BOOL Result = FALSE;
    int DataType;
    TGif gif;
    TJPEG jpeg;

    #ifdef EBOOK_TOOL

    // Get the data type
    DataType = PageList.GetMainDataType( PageToLoad );

    // Check to see that the file is still there. The could have moved it.
    if( !FileExist( PageList.GetMainDataPath( PageToLoad ) ) )
    {
        // Ask if they want to find the missing project file
        if( FindMissingProjectFile( hwnd, PageList.GetMainDataPath( PageToLoad ), 
            Settings.GetProjectArtPath(), PAGE_FILE_TYPES ) )
            ActProj.LiveProject = TRUE;
        else
            return FALSE;
    }

    // Load the file
    switch( DataType )
    {
        case FILE_TYPE_BMP:
        case FILE_TYPE_TXT:
            Result = pBase->LoadFromFile( PageList.GetMainDataPath( PageToLoad ) );
            break;

        case FILE_TYPE_JPG:
            Result = pBase->LoadFromBuffer( jpeg.LoadFromFile( PageList.GetMainDataPath( PageToLoad ) ) );
            break;

        case FILE_TYPE_GIF:
            Result = pBase->LoadFromBuffer( gif.LoadFromFile( PageList.GetMainDataPath( PageToLoad ) ) );
            break;
    }

    #else

    TLoadBuffer lb;

    // If there was an error, keep them from going to any other pages
    // Also, get the data type so we know what to do with the buffer
    
    if( LoadFileDataFromEXEfile( PageList.GetMainDataOffset( PageToLoad ), &lb ) )
    {
        // Get the data type
        DataType = lb.Type;

        // Load the file
        // Give the pointer over to the dib class. It will delete it when it is done
        switch( lb.Type )
        {
            case FILE_TYPE_TXT:

                char EncyKey[500];
                // Make an encryption key
                sprintf( EncyKey, "%st72m.a%si02sxlm%s4yk3aqvm%s49p1`,'t%sykw3nay", 
                         ProjPrefsData.Title, ProjPrefsData.RegNo, ProjPrefsData.NameOrCompany,
                         ProjPrefsData.WebAddress, ProjPrefsData.EmailAddress );

                // Unencrypt the text
                LockItUp( (BYTE *)EncyKey, lb.pData, lb.Size, UNENCRYPT_DATA );

            case FILE_TYPE_BMP:
                Result = pBase->LoadFromBuffer( lb.pData, lb.Size );
                // Normally we don't delete the uncompressed buffer because it ends up being owned by the
                // DIB class and get freed there when done but because there was an error, we free everything.
                if( !Result ) delete [] lb.pData;
                break;

            case FILE_TYPE_JPG:
                Result = pBase->LoadFromBuffer( jpeg.LoadFromBuffer( lb.pData, lb.Size ) );
                break;

            case FILE_TYPE_GIF:
                Result = pBase->LoadFromBuffer( gif.LoadFromBuffer( lb.pData, lb.Size ) );
                break;
        }
    }
    #endif

    // Return the data type if all went ok
    // or return Result which should be false
    if( Result )
        return DataType;
    else
        return Result;

}   // LoadMainFile



/************************************************************************
*    FUNCTION NAME:         EnlargeReduce                                                             
*
*    DESCRIPTION:           Flips the page left or right
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd     - Handle of the window receiving the message
*              int Direction - Flip the page left or flip the page right 
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

void TEbookWnd::EnlargeReducePage( HWND hwnd, int Direction )
{
    int i, FontSizeAry[16] = { -11, -12, -13, -15, -16, -19, -21, -24, -27, -29, -32, -35, -37, -48, -64, -96 };

    if( !dib.IsEmpty() && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
    {
        // Don't let it go any further if we have met our min or max
        if( Direction == ENLARGE_PAGE && ProjPrefsData.ZoomIndex == ZOOM_SIZE_COUNT )
            return;
        else if( Direction == REDUCE_PAGE && ProjPrefsData.ZoomIndex == 0 )
            return;

        // Zoom in Zoom out
        if( Direction == ENLARGE_PAGE )
            ++ProjPrefsData.ZoomIndex;
        else
            --ProjPrefsData.ZoomIndex;

        // Up the controls
        UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );
     
        // Get the enlargement/reduction persentage
        AppData.ZoomPercent = ZoomSizes[ ProjPrefsData.ZoomIndex ];

        // Size the view window
        ViewWnd_ResetWnd( AppData.hMidWnd );

        // set the scrool bars if needed
        SetScrollBars( AppData.hMidWnd );
    }
    else if( !TextBlock.IsEmpty() && AppData.pGBase->GetObjectType() == TEXT_OBJECT )
    {
        for( i = 0; i < 16; ++i )
        {
            if( Direction == ENLARGE_PAGE && FontSizeAry[i] < ProjPrefsData.DefFont.lfHeight )
            {
                // Set the font to a bigger size
                ProjPrefsData.DefFont.lfHeight = FontSizeAry[i];

                // Make the new font, delete the old
                TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
                                       ProjPrefsData.DefaultTxtColor, 
                                       0, &ProjPrefsData.DefFont );

                // Redraw the view window
                ViewWnd_ResetWnd( AppData.hMidWnd );

                // set the scrool bars if needed
                SetScrollBars( AppData.hMidWnd );

                break;
            }
            else if( Direction == REDUCE_PAGE && FontSizeAry[15-i] > ProjPrefsData.DefFont.lfHeight )
            {
                // Set the font to a bigger size
                ProjPrefsData.DefFont.lfHeight = FontSizeAry[15-i];

                // Make the new font, delete the old
                TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
                                       ProjPrefsData.DefaultTxtColor, 
                                       0, &ProjPrefsData.DefFont );

                // Redraw the view window
                ViewWnd_ResetWnd( AppData.hMidWnd );

                // set the scrool bars if needed
                SetScrollBars( AppData.hMidWnd );

                break;
            }

        }
    }
            
}   // EnlargeReduce



/************************************************************************
*    FUNCTION NAME:         OnSize                                                             
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

void TEbookWnd::OnSize( HWND hwnd, UINT state, int cx, int cy )
{
    RECT ParentRect, ToolBarRect, rect;
    static UINT LastState = state;

    if( state != SIZE_MINIMIZED && LastState != SIZE_MINIMIZED )
    {
        // Get the client rect of the tool bar
        GetClientRect( AppData.hToolBar, &ToolBarRect );

        // Get the client rect of parent window
        GetClientRect( hwnd, &ParentRect );
        
        // Move the tool bar to fill the window
        MoveWindow( AppData.hToolBar, 
                    0, 0, ParentRect.right, ToolBarRect.bottom, TRUE );

        // move the middle window to fill the window
        if( AppData.hMidWnd != hwnd )
            MoveWindow( AppData.hMidWnd, 0, ToolBarRect.bottom + (GetSystemMetrics(SM_CYEDGE)*2) + 1, 
                        ParentRect.right, ParentRect.bottom - (ToolBarRect.bottom + (GetSystemMetrics(SM_CYEDGE)*2)) - 1, TRUE ); 

        // Size the caption to fill the remaining space
        #ifdef SHARE_THE_MEMORIES

        if( AppData.hCaptionEdit )
        {
            // size to the new widow size
            AppData.CaptionTxtRect.right = ParentRect.right - AppData.CaptionTxtRect.left;

            // Make sure we don't get smaller then the default width
            if( AppData.CaptionTxtRect.right < DEFAULT_CAPTION_WIDTH )
                AppData.CaptionTxtRect.right = DEFAULT_CAPTION_WIDTH;

            // Move the caption window to take up the remaining space
            MoveWindow( AppData.hCaptionEdit, AppData.CaptionTxtRect.left, AppData.CaptionTxtRect.top,
                        AppData.CaptionTxtRect.right, AppData.CaptionTxtRect.bottom, TRUE );

        }
        #endif

        #ifdef EBOOK_TOOL
        if( ActProj.LiveProject || ActProj.OpenedFile )
            // Size the view window
            ViewWnd_ResetWnd( AppData.hMidWnd );
        #else
        // Size the view window
        ViewWnd_ResetWnd( AppData.hMidWnd );
        #endif

        // Get the windows rect
        GetWindowRect( hwnd, &rect );

        // Save the window state
        if( state == SIZE_MAXIMIZED )
            Settings.SetWindowState( SW_MAXIMIZE );
        // Save the window's mode, size and position
        else if( state == SIZE_RESTORED )
        {
            Settings.SetWindowState( SW_SHOW );
            Settings.SetWindowX( rect.left );
            Settings.SetWindowY( rect.top );
            Settings.SetWindowW( rect.right-rect.left );
            Settings.SetWindowH( rect.bottom-rect.top );
        }

        // set the scrool bars if needed
        SetScrollBars( AppData.hMidWnd );
    }

    // Record the last state
    LastState = state;

}   // OnSize



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

void TEbookWnd::OnMove( HWND hwnd, int x, int y )
{
    RECT rect;

    // Get the windows rect
    GetWindowRect( hwnd, &rect );

    // Make sure the window is normal when it is saved
    if( !IsZoomed( hwnd ) && !IsIconic( hwnd ) )
    {
        Settings.SetWindowState( SW_SHOW );
        Settings.SetWindowX( rect.left );
        Settings.SetWindowY( rect.top );
        Settings.SetWindowW( rect.right-rect.left );
        Settings.SetWindowH( rect.bottom-rect.top );
    }

}   // OnMove


/************************************************************************
*    FUNCTION NAME:         SetScrollBars                                                             
*
*    DESCRIPTION:           Turns on or off the scroll bars.
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
 
void TEbookWnd::SetScrollBars( HWND hwnd )
{
    RECT MidRect;
    int NumberOfPages=0, length;

    // Get the client rect of middle window
    GetClientRect( hwnd, &MidRect );

    // Hide or show the scroll bars for the parent window
    // Horizontal scroll bar
    if( AppData.ViewWndRect.left + AppData.ViewWndRect.right > MidRect.right )
    {
        length = ((AppData.ViewWndRect.left + AppData.ViewWndRect.right)-MidRect.right)+VIEW_WND_OFFSET_Y;

        // Make sure we have a length, otherwise Div by 0 error
        if( length )
            NumberOfPages = (length / SCROLL_BAR_PAGE_SIZE) + ( length % SCROLL_BAR_PAGE_SIZE ? 1 : 0 );

        AppData.sh.nPos = 0;
        AppData.sh.nMin = 0;
        AppData.sh.nMax = NumberOfPages;

        AppData.HScrollVisible = TRUE;
        SetScrollInfo( hwnd, SB_HORZ, &AppData.sh, TRUE );
        ShowScrollBar( hwnd, SB_HORZ, TRUE );
    }
    else
    {
        AppData.HScrollVisible = FALSE;
        ShowScrollBar( hwnd, SB_HORZ, FALSE );
        AppData.sh.nPos = 0;
    }

    // Vertical scroll bar
    if( AppData.ViewWndRect.top + AppData.ViewWndRect.bottom > MidRect.bottom )
    {
        length = ((AppData.ViewWndRect.top + AppData.ViewWndRect.bottom)-MidRect.bottom)+VIEW_WND_OFFSET_Y;

        // Make sure we have a length, otherwise Div by 0 error
        if( length )
            NumberOfPages = (length / SCROLL_BAR_PAGE_SIZE) + ( length % SCROLL_BAR_PAGE_SIZE ? 1 : 0 );

        AppData.sv.nPos = 0;
        AppData.sv.nMin = 0;
        AppData.sv.nMax = NumberOfPages;

        AppData.VScrollVisible = TRUE;
        SetScrollInfo( hwnd, SB_VERT, &AppData.sv, TRUE );
        ShowScrollBar( hwnd, SB_VERT, TRUE );
    }
    else
    {
        AppData.VScrollVisible = FALSE;
        ShowScrollBar( hwnd, SB_VERT, FALSE );
        AppData.sv.nPos = 0;
    }

}   // SetScrollBars



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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::SaveToFile( HWND hwnd, char *FileName )
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
#endif



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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::OpenFile( HWND hwnd )
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

    // Reset the project'e exe mode depending on if the development 
    // version has been registered. This flag is tested to see if
    // it can run on a nother machine
    if( AppData.RegistrationMode == SHAREWARE_VERSION )
        ProjPrefsData.ProjRegMode = SHAREWARE_VERSION;
    else if( AppData.RegistrationMode == FULL_REG_VERSION )
        ProjPrefsData.ProjRegMode = FULL_REG_VERSION;   

    Result = TRUE;


FREE_MEMORY:;

    // Close the file handle
    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle( hFile );

    return Result;

}   // OpenFile
#endif


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

BOOL TEbookWnd::CheckToSave( HWND hwnd )
{
    BOOL Result = TRUE;

    #ifdef EBOOK_TOOL
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
    #endif

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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::mnuAddPage( HWND hwnd )
{
    TStringList StringList;
    BOOL Result = FALSE;
    char FileDir[MAX_PATH];
    char FileName[MAX_STR_PATH*2];
    char StrTemp[MAX_STR_PATH];
    DWORD ExtraFlags;
    int i;

    FileName[0] = 0;

    // Setup the extra flags for this dialog box
    ExtraFlags = OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ALLOWMULTISELECT;

    if( PictLoad_GetPicture( AppData.hInst, hwnd, "Add Page File(s)",
        ExtraFlags, FileName, MAX_STR_PATH*2, PAGE_FILE_TYPES, Settings.GetProjectArtPath() ) )
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
                CropFileNameFromPath( StringList.GetString( i ), StrTemp );

                // Set the status bar text
                StatusWnd_SetStatusTxt( "Loading and Testing file (%s).", StrTemp );

                // Loads and tests each image and makes a page entry
                if( !AddMainFileToPageList( StringList.GetString(i) ) )
                    goto LOAD_ERROR;

                StatusWnd_MoveStatusBar();
            }
        }
        else // Single image load
        {
            if( !AddMainFileToPageList( FileName ) )
                goto LOAD_ERROR;
        }

        // Displays the new page or the last page of a group
        if( !DisplayLoadedPage( hwnd ) )
            goto LOAD_ERROR;
    }

    Result = TRUE;
    
LOAD_ERROR:;

    // Hide the status window
    StatusWnd_HideStatusWnd();

    return Result;

}   // mnuAddPage
#endif


/************************************************************************
*    FUNCTION NAME:         DisplayLoadedPage                                                             
*
*    DESCRIPTION:           Displays the new page or the last page of a group
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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::DisplayLoadedPage( HWND hwnd )
{
    BOOL Result = FALSE;

    // If the last page is a bitmap, build the GDI bitmap
    if( PageList.GetMainDataType( PageList.GetCurrentPage() ) == FILE_TYPE_TXT )
        AppData.pGBase = &TextBlock;
    else
    {
        // Create the GDI bitmap
        if( !dib.CreateBitmap( DIB_SECTION_BMP, BIT_COUNT_24 ) )
        {
            PostMsg( "Error", "Error creating bitmap on addpage (%s).", 
                     PageList.GetMainDataPath( PageList.GetCurrentPage() ) );

            goto DISPLAY_ERROR;
        }

        // Delete the bitmap bits because we don't need them any more
        dib.DeleteBitmapBits();

        // Set the graphic base class
        AppData.pGBase = &dib;
    }

    // Size the view window
    ViewWnd_ResetWnd( AppData.hMidWnd );

    // set the scrool bars if needed
    SetScrollBars( AppData.hMidWnd );

    // Update the controls depending on the conditions        
    if( PageList.Count() > 0 )
        EnableSpecificMenuItems( hwnd, TRUE );

    // Reset the Images Too Big flag
    AppData.FlagBigImages = FALSE;

    // Set the project live status
    ActProj.LiveProject = TRUE;

    // Update the static showing the number of pages and the current page
    UpdateControls( hwnd, PageList.GetCurrentPage(), PageList.Count() );

    // Set the flip button state
    SetFlipButtons();

    Result = TRUE;
    
DISPLAY_ERROR:;

    return Result;

}   // DisplayLoadedPage
#endif



/************************************************************************
*    FUNCTION NAME:         AddMainImageToPageList                                                             
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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::AddMainFileToPageList( char *FileName )
{
    WORD FileType;
    BOOL Result = FALSE;
    BOOL FileLoad = FALSE;
    TGif gif;
    TJPEG jpeg;

    // Get the file type - bmp, jpg, gif, and so on
    FileType = GetFileType( FileName );

    // Load the files to check for errors
    switch( FileType )
    {
        case FILE_TYPE_BMP:
            FileLoad = dib.LoadFromFile( FileName );
            break;

        case FILE_TYPE_JPG:
            FileLoad = dib.LoadFromBuffer( jpeg.LoadFromFile( FileName ) );
            break;

        case FILE_TYPE_GIF:
            FileLoad = dib.LoadFromBuffer( gif.LoadFromFile( FileName ) );
            break;

        case FILE_TYPE_TXT:
            FileLoad = TextBlock.LoadFromFile( FileName );
        break;
    }

    // Did the file load without an error? If so, make a 
    // new page and add it to the page list
    if( FileLoad )
    {
        // Check if the file size is a little too big
        //if( FileType == FILE_TYPE_JPG || FileType == FILE_TYPE_GIF || FileType == FILE_TYPE_BMP )
        //    WarnIfBMPisToBig();

        // Add a new blank page to the list
        PageList.Add();

        // Set the current page to the last page
        PageList.SetCurrentPage( (WORD)PageList.Count()-1 );

        // Save the file path to the main image
        strcpy( PageList.GetMainDataPath( PageList.GetCurrentPage() ), FileName );

        // Save the file type so that we always know what type of file we are using
        PageList.SetMainDataType( PageList.GetCurrentPage(), FileType );

        // Set the result
        Result = TRUE;
    }

    return Result;

}   // AddMainImageToPageList
#endif



/************************************************************************
*    FUNCTION NAME:         IsFileAnRTF                                                             
*
*    DESCRIPTION:           Returns true if it is an RTF file
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
#ifdef EBOOK_TOOL
BOOL TEbookWnd::IsFileAnRTF( char *FileName )
{
    unsigned long bytesRead;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    char buffer[6];
    BOOL Result = FALSE;


    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_WRITE,
                         &FileSec, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE ) 
        goto HANDLE_ERROR;

    // Zero out the buffer
    memset( buffer, 0, sizeof(buffer) );

    // Write your string
    ReadFile( hFile, buffer, sizeof(buffer)-1, &bytesRead, NULL );

    // See if the string matches
    if( strcmp( buffer, "{\rtf" ) == 0 )
        Result = TRUE;

    HANDLE_ERROR:;

    if( hFile )
        CloseHandle( hFile );

    return Result;

}   // IsFileAnRTF
#endif


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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::mnuBuild_EXE( HWND hwnd )
{
    unsigned long bytesReadWrite;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BOOL Result = FALSE;
    BOOL FileReadWriteResult;
    char StrTemp[MAX_STR_PATH];
    DWORD FileSize, CheckSum;
    BYTE QuadAlign[4] = {0,0,0,0};
    int i;
    DWORD *CheckSumFileHeader;
    // This varaible is incremented and marks the starting
    // offset the image files are added to the exe file
    DWORD FileDataOffset;
    double FileSizeDb, OneMeg;
    char FileSizStr[20];

    StrTemp[0] = 0;

    if( PageList.Count() )
    {
        // Check that all the project files are there
        if( !CheckAllFilesAreThere( hwnd ) )
            goto FREE_MEMORY;

        // Get the serial number of the hardrive being used
        GetVolumeInformation( "c:\\", NULL, 0, &ProjPrefsData.DriveSerialNo, NULL, NULL, NULL, 0 );

        // Set the project'e exe mode depending on if the development 
        // version has been registered. This flag is tested to see if
        // it can run on a nother machine
        if( AppData.RegistrationMode == SHAREWARE_VERSION )
        {
            ProjPrefsData.ProjRegMode = SHAREWARE_VERSION;
            PostMsg( "Shareware Version Build", SHAREWARE_BUILD );
        }
        else if( AppData.RegistrationMode == FULL_REG_VERSION )
            ProjPrefsData.ProjRegMode = FULL_REG_VERSION;

        // show the status window
        DisplayStatusWnd( AppData.hInst, hwnd );

        // Setup the status window
        StatusWnd_SetStatusBar( 1, PageList.Count()+1, 1 );

        // Set the status bar text
        StatusWnd_SetStatusTxt( "Building Program File..." );

        // Put together the path to the new exe file
        AddFileToOtherPath( ProjPrefsData.ExeName, ActProj.FilePath, StrTemp );

        // Update the number of pages being used in this project
        ProjPrefsData.Total_Num_Of_Pages = PageList.Count();

        // Write the exe from the resource to the hard drive
        // Init the FileDataOffset variable with the size of the exe file.
        // Because that is the starting point for all the project's data
        hFile = WriteEXEtoHDfromRes( StrTemp, &FileDataOffset );
        if( hFile == INVALID_HANDLE_VALUE )
            goto FREE_MEMORY;

        // Update the FileHeader.ProjectDataOffset to the size of the exe
        // file because that is the starting point for all the project's data
        FileHeader.ProjectDataOffset = FileDataOffset;  

        // Save the project preferences data to the exe file and incements the offset
        // This may also include the startup screen bitmap and info text blocks and anything else
        FileDataOffset = SaveProjectDataToFile( hFile, FileDataOffset );
        if( FileDataOffset == 0 )
        {
            PostMsg( "Error", "Error building file project data. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Write all the project files if any are in use, startup screen, info text...
        if( !SaveProjectFilesToEXE( hFile ) )
        {
            PostMsg( "Error", "Error building file with project files. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Write the page data to the file
        FileDataOffset = SavePageDataToFile( hFile, FileDataOffset );
        if( FileDataOffset == 0 )
        {
            PostMsg( "Error", "Error building file page data. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Write all the page files to the exe
        if( !SavePageFilesToEXE( hFile ) )
        {
            PostMsg( "Error", "Error building file with page files. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Get the file size
        FileSize = GetFileSize( hFile, NULL );

        // Quad align file before adding header
        if( FileSize % sizeof( DWORD ) != 0 )
        {
            FileReadWriteResult = WriteFile( hFile, QuadAlign, sizeof(DWORD)-(FileSize % sizeof( DWORD )), &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(DWORD)-(FileSize % sizeof( DWORD )) )
            {
                PostMsg( "Error", "Error building file alignment. (%s)", ProjPrefsData.ExeName );
                goto FREE_MEMORY;
            }
        }

        // Checksum the file
        CheckSum = CheckSumFile( hFile );

        // Make sure we didn't get a checksum error
        if( CheckSum == CHECK_SUM_ERROR )
        {
            PostMsg( "Error", "Error checksuming file. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // The check sum sets the file pointer to the begining of the file
        // so we need to set it back to the end of the file before writing the file header
        SetFilePointer( hFile, 0, 0, FILE_END );

        // Set the checksum result to zero because it could be
        // holding the value from the previous build
        FileHeader.CheckSumResult = 0;

        // Checksum the file header
        CheckSumFileHeader = (DWORD *)&FileHeader;
        for( i = 0; i < (sizeof(TFileHeader) / sizeof(DWORD)); ++i )
            CheckSum += CheckSumFileHeader[i];

        // Save the difference to the checksum
        FileHeader.CheckSumResult = 0x100000000 - CheckSum;

        // Write the projects preferences structure to the file
        FileReadWriteResult = WriteFile( hFile, &FileHeader, sizeof(TFileHeader), &bytesReadWrite, NULL );
        if( !FileReadWriteResult || bytesReadWrite != sizeof(TFileHeader) )
        {
            PostMsg( "Error", "Error building file with file header data. (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        // Get the final file size so they know what the size of their EXE is
        FileSize = GetFileSize( hFile, NULL );

        // One lase check to see if the file is quad aligned
        if( FileSize % sizeof( DWORD ) == 0 )
        {
            // Format the size string depending on the size of the file
            if( FileSize < 1024000 ) // Show in K size
            {
                sprintf( FileSizStr, "%uK", FileSize / 1024 );
            }
            else   // Show in MB size
            {
                OneMeg = 1024000;
                FileSizeDb = FileSize / OneMeg;
                sprintf( FileSizStr, "%.1lfMB", FileSizeDb );
            }

            // Let them know what that file size is
            //PostMsg( "Done", "Your ePhoto Book (%s) has been made. The file size of your ePhoto Book is %s.", 
                     //ProjPrefsData.ExeName, FileSizStr );
        }
        else
        {
            PostMsg( "Error", "Error building file. File size out of alignment (%s)", ProjPrefsData.ExeName );
            goto FREE_MEMORY;
        }

        Result = TRUE;

    FREE_MEMORY:;

        // Close the file handle
        if( hFile != INVALID_HANDLE_VALUE )
            CloseHandle( hFile );

        if( !Result && StrTemp[0] )
            DeleteFile( StrTemp );
        
        StatusWnd_HideStatusWnd();

    }   // if( PageList.Count() )
    else
    {
        PostMsg( "Opps", "Need to add a page before you can do a build." );
    }

    return Result;

}   // mnuBuild_EXE
#endif



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

#ifdef EBOOK_TOOL
DWORD TEbookWnd::SaveProjectDataToFile( HANDLE hFile, DWORD StartingOffset )
{
    DWORD PrefsStructSize, FileDataOffset;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    char TmpFilePath[MAX_STR_PATH];
    char ProjectPath[MAX_STR_PATH];
    DWORD ProjectImageSize, TextSize;
    LPTSTR ProjectBMPs[MAX_PROJECT_BMP];
    DWORD *ProjectOffsets[MAX_PROJECT_BMP];
    char *CheckIfInUse;
    int i;
    TProjectPrefs TmpProjPrefsData;

    // The projects preferences structure size minus the file paths and the 
    // future data array because the view program doesn't use or need them
    PrefsStructSize = sizeof(TProjectPrefs)-sizeof( TProjectPrefsSmallSize );

    // Init the data offset to the point where the first data file can be found at
    FileDataOffset = PrefsStructSize + StartingOffset;

    // Set to zero just incase it was added and then removed
    ProjPrefsData.StartUpScrnBMPOffset = 0;
    ProjPrefsData.BackgroundBMPOffset = 0;

    // init the project BMP string so that we can put it into a for loop
    ProjectBMPs[0] = ProjPrefsData.StartUpScreenPath;
    ProjectBMPs[1] = ProjPrefsData.BackgroundBMPPath;

    // Init the project offsets so that we can put it into a for loop
    ProjectOffsets[0] = &ProjPrefsData.StartUpScrnBMPOffset;
    ProjectOffsets[1] = &ProjPrefsData.BackgroundBMPOffset;

    // Setup the startup screen and the background bitmap
    for( i = 0; i < MAX_PROJECT_BMP; ++i )
    {
        CheckIfInUse = ProjectBMPs[i];

        // Increment the offset if we are using a bitmap for startup screen
        // or background bitmap
        if( CheckIfInUse[0] )
        {
            // Update the file size. Since this is the uncompressed file or a gif or a jpeg, 
            // we need to include the file header that's created on the fly.
            ProjectImageSize = GetTheFileSize( ProjectBMPs[i] ) + sizeof(TDataHeader);

            // Save the offset to this data
            *ProjectOffsets[i] = FileDataOffset;

            // Update file offset
            FileDataOffset += ProjectImageSize;
        }
    }
        
    // Strip out the path the project file is in.
    CropOutPath( ActProj.FilePath, ProjectPath );

    // Copy the path to the file buffer
    strcpy( TmpFilePath, ProjectPath );

    // Copy the file name to the file buffer
    strcat( TmpFilePath, STARTUP_INFO_TXT_FILE );

    // Increment the offset if we are using the startup info text
    if( FileExist( TmpFilePath ) )
    {
        // Get the size of the text file
        TextSize = GetTheFileSize( TmpFilePath ) + sizeof(TDataHeader);

        // Save the offset to this data
        ProjPrefsData.InfoWndTXTOffset = FileDataOffset;

        // Update the file data offset
        FileDataOffset += TextSize;
    }
    else
        // Set to zero just incase it was added and then removed
        ProjPrefsData.InfoWndTXTOffset = 0;

    // Copy the path to the file buffer
    strcpy( TmpFilePath, ProjectPath );

    // Copy the file name to the file buffer
    strcat( TmpFilePath, ORDERFRM_INFO_TXT_FILE );

    // Increment the offset if we are using the order form info text
    if( FileExist( TmpFilePath ) )
    {
        // Get the size of the text file
        TextSize = GetTheFileSize( TmpFilePath ) + sizeof(TDataHeader);

        // Save the offset to this data
        ProjPrefsData.InfoOFWndTXTOffset = FileDataOffset;

        // Update the file data offset
        FileDataOffset += TextSize;
    }
    else
        // Set to zero just incase it was added and then removed
        ProjPrefsData.InfoOFWndTXTOffset = 0;

    // Set the offset to the starting position of the TPage Data which is after the
    // TProjectPrefs data and any graphics needed by the TProjectPrefs
    ProjPrefsData.PageDataOffset = FileDataOffset;

    // Make a copy of the project prefs so that the data can be encrypted
    memcpy( &TmpProjPrefsData, &ProjPrefsData, PrefsStructSize );

    // Encrypt the data using the SHAREWARE_BUILD as the key. It's a long array of text that
    // a hacker may not think of using.
    LockItUp( (BYTE *)SHAREWARE_BUILD, (BYTE *)&TmpProjPrefsData, PrefsStructSize, ENCRYPT_DATA );

    // Write the projects preferences structure to the file
    FileReadWriteResult = WriteFile( hFile, &TmpProjPrefsData, PrefsStructSize, &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != PrefsStructSize )
    {
        PostMsg( "Error", "Error building file with prefs data. (%s)", ProjPrefsData.ExeName );
        FileDataOffset = 0;
    }

    return FileDataOffset;

}   // SaveProjectDataToFile
#endif



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

#ifdef EBOOK_TOOL
DWORD TEbookWnd::SavePageDataToFile( HANDLE hFile, DWORD StartingOffset )
{
    int i, j;
    DWORD FileDataOffset, MainFileSize=0, SoundFileSize=0;
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
        PageList.SetSoundDataDuplicate( i, FALSE );
        PageList.SetAVIDataDuplicate( i, FALSE );
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
        //  Check for dup Sound files
        /////////////////////////////////////

        // Add up the FileDataOffset if it is not a duplicate and a file has been used
        if( PageList.GetSoundDataType( i ) && !PageList.GetSoundDataDuplicate( i ) )
        {
            // Record the image data offset
            PageList.SetSoundDataOffset( i, FileDataOffset + MainFileSize );

            for( j = i+1; j < PageList.Count(); ++j )
            {
                // Check for page image duplicate
                if( !PageList.GetSoundDataDuplicate( j ) )
                {
                    // if it's the same path, then it is a duplicate
                    if( strcmp( PageList.GetSoundDataPath( i ), 
                        PageList.GetSoundDataPath( j ) ) == 0 )
                    {
                        // Set the duplicate flag
                        PageList.SetSoundDataDuplicate( j, TRUE );
                        
                        // Record the image data offset
                        PageList.SetSoundDataOffset( j, FileDataOffset + MainFileSize );   
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
        FileDataOffset += MainFileSize + SoundFileSize;
        MainFileSize = 0;
        SoundFileSize = 0;
    }

    return FileDataOffset;

}   // SavePageDataToFile
#endif



/************************************************************************
*    FUNCTION NAME:         SaveProjectFilesToEXE                                                             
*
*    DESCRIPTION:           Save the project files to the exe file
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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::SaveProjectFilesToEXE( HANDLE hFile )
{
    BOOL Result = FALSE;
    BYTE *pFileData;
    DWORD BufferSize;
    TDataHeader DataHeader;
    BOOL FileReadWriteResult;
    unsigned long bytesReadWrite;
    char TmpFilePath[MAX_STR_PATH], ProjectPath[MAX_STR_PATH];
    LPTSTR ProjectBMPs[MAX_PROJECT_BMP];
    char *CheckIfInUse;
    int i;

    // Init the buffer for making uncompressed file headers
    memset( &DataHeader, 0, sizeof(TDataHeader) );

    BufferSize = 1000000;
    pFileData = new BYTE[BufferSize];
    if( !pFileData )
    {
        PostMsg( "Error", "Memory allocation error during project file build." );
        goto FREE_MEMORY;
    }

    // init the project BMP string so that we can put it into a for loop
    ProjectBMPs[0] = ProjPrefsData.StartUpScreenPath;
    ProjectBMPs[1] = ProjPrefsData.BackgroundBMPPath;

    // Add the startup screen bitmap and background tile bitmap to the exe file
    for( i = 0; i < MAX_PROJECT_BMP; ++i )
    {
        CheckIfInUse = ProjectBMPs[i];
 
        if( CheckIfInUse[0] )
        {
            /*
            if( ProjPrefsData.CompressImages && GetFileType( ProjectBMPs[i] ) == FILE_TYPE_BMP )
                // Convert the file name to the compressed file name
                RelpaceFileExtension( ProjectBMPs[i], TmpFilePath, COMPRESSED_FILE_EXT );
            else
            {
            */
                // This is not the compressed file but use it as if it was
                strcpy( TmpFilePath, ProjectBMPs[i] );

                // Set the header. Since this is uncompressed, the CompSize is 0 
                DataHeader.Type = GetFileType( TmpFilePath );
                DataHeader.CompSize = 0;
                DataHeader.UncompSize = GetTheFileSize( TmpFilePath );
                if( DataHeader.UncompSize == 0 )
                {
                    PostMsg( "Error", "Error getting file size for file. (%s)", TmpFilePath );
                    goto FREE_MEMORY;
                }

                // Write the projects preferences structure to the file
                FileReadWriteResult = WriteFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
                if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
                {
                    PostMsg( "Error", "Error writing file header data. (%s)", TmpFilePath );
                    goto FREE_MEMORY;
                }
            //}

            // Copy the file to the view program
            if( !CopyFileToDest( TmpFilePath, hFile, pFileData, BufferSize ) )           
                goto FREE_MEMORY;
        }
    }


    // Strip out the path the project file is in.
    CropOutPath( ActProj.FilePath, ProjectPath );

    for( i = 0; i < MAX_PROJECT_INFO_TXT; ++i )
    {
        // Copy the path to the file buffer
        strcpy( TmpFilePath, ProjectPath );

        // Copy the file name to the file buffer
        if( i == 0 )
            strcat( TmpFilePath, STARTUP_INFO_TXT_FILE );
        else if( i == 1 )
            strcat( TmpFilePath, ORDERFRM_INFO_TXT_FILE );

        // Add the startup info text to the exe file 
        if( FileExist( TmpFilePath ) )
        {
            // Set the header. Since this is uncompressed, the CompSize is 0 
            DataHeader.Type = FILE_TYPE_TXT;
            DataHeader.CompSize = 0;
            DataHeader.UncompSize = GetTheFileSize( TmpFilePath );
            if( DataHeader.UncompSize == 0 )
            {
                PostMsg( "Error", "Error getting file size for file. (%s)", TmpFilePath );
                goto FREE_MEMORY;
            }
            
            // Write the text file header to the exe file before adding the text
            FileReadWriteResult = WriteFile( hFile, &DataHeader, sizeof(TDataHeader), &bytesReadWrite, NULL );
            if( !FileReadWriteResult || bytesReadWrite != sizeof(TDataHeader) )
            {
                PostMsg( "Error", "Error building file with Startup Info header data. (%s)", ProjPrefsData.ExeName );
                goto FREE_MEMORY;
            }

            // Copy the info text to the exe file
            if( !CopyFileToDest( TmpFilePath, hFile, pFileData, BufferSize ) )           
                goto FREE_MEMORY;
        }
    }

    Result = TRUE;

    FREE_MEMORY:;

    // delete the buffer
    if( pFileData )
        delete [] pFileData;

    return Result;
}   // SaveProjectFilesToEXE
#endif


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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::SavePageFilesToEXE( HANDLE hFile )
{
    int i;
    BOOL Result = FALSE;
    BYTE *pFileData;
    DWORD BufferSize;
    char EncyKey[500];
    TDataHeader DataHeader;
    BOOL FileReadWriteResult;
    unsigned long bytesReadWrite;

    // Make an encryption key
    sprintf( EncyKey, "%st72m.a%si02sxlm%s4yk3aqvm%s49p1`,'t%sykw3nay", 
             ProjPrefsData.Title, ProjPrefsData.RegNo, ProjPrefsData.NameOrCompany,
             ProjPrefsData.WebAddress, ProjPrefsData.EmailAddress );

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
        StatusWnd_SetStatusTxt( "Building Page (%u).", i );

        //////////////////////////////
        //  Save the Main Image File
        //////////////////////////////
        // Check if this is a duplicate, if so, skip this cycle
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

            // Encrypt the data if it's a text file
            if( DataHeader.Type == FILE_TYPE_TXT )
            {
                if( !CopyFileToDest( PageList.GetMainDataPath( i ), hFile, pFileData, BufferSize, ENCRYPT_DATA, (BYTE *)EncyKey ) )           
                    goto FREE_MEMORY;
            }
            else
            {
                if( !CopyFileToDest( PageList.GetMainDataPath( i ), hFile, pFileData, BufferSize ) )           
                    goto FREE_MEMORY;
            }
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
#endif



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

#ifdef EBOOK_TOOL
HANDLE TEbookWnd::WriteEXEtoHDfromRes( char *FilePath, DWORD *FileSize )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesReadWrite;
    BOOL FileReadWriteResult;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    TLoadRes LoadRes;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    if( !LoadTheResource( AppData.hInst, "eBookViewer", "EXE", &LoadRes ) )
    {
        PostMsg( "Error", "Error loading resource to build file. (%s)", ProjPrefsData.ExeName );
        goto HANDLE_ERROR;
    }

    // Get file size of the resource which will be the starting offset into the data
    *FileSize = LoadRes.Size;

    // Open the file on the hard drive
    hFile = CreateFile( FilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                           &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hFile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't create file. (%s)", FilePath );
        goto HANDLE_ERROR;
    }

    // Write exe to the hardrive
    FileReadWriteResult = WriteFile( hFile, LoadRes.pData, LoadRes.Size, &bytesReadWrite, NULL );
    if( !FileReadWriteResult || bytesReadWrite != LoadRes.Size )
    {
        PostMsg( "Error", "Can't write file. (%s)", FilePath );
        goto HANDLE_ERROR;
    }

HANDLE_ERROR:;

    return hFile;

}   // WriteEXEtoHdfromRes
#endif


/************************************************************************
*    FUNCTION NAME:         SetupInfoWnd                                                             
*
*    DESCRIPTION:           Display the setup info text dialog
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
*    h rosenorn    4/02/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
#if defined( EBOOK_TOOL ) && !defined( SHARE_THE_MEMORIES )
void TEbookWnd::SetupInfoWnd( HWND hwnd, char *pDlgCaption, char *WndCaption, 
                            char *WndTitle, char *FileName )
{
    char FilePathBuffer[MAX_STR_PATH], ProjectPath[MAX_STR_PATH];

    // Strip out the path from the path the project file is in.
    CropOutPath( ActProj.FilePath, ProjectPath );

    // Copy the path to the file buffer
    strcpy( FilePathBuffer, ProjectPath );

    // Copy the file name to the file buffer
    strcat( FilePathBuffer, FileName );

    // Loads the info text file and saves the new version to disk
    if( DisplayInfoTxtSetupDlg( AppData.hInst, hwnd, pDlgCaption, WndCaption, WndTitle, FilePathBuffer ) )
        ActProj.LiveProject = TRUE;

}   // SetupInfoWnd
#endif



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

void TEbookWnd::UpdateControls( HWND hwnd, int CurrentPage, int TotalPages )
{
    char StaticStr[20];
    int i;

    // Set the total number of pages
    sprintf( StaticStr, "of %u", TotalPages );
    Static_SetText( AppData.hTBStatic,  StaticStr );

    // Set the current page selected
    if( TotalPages )
        sprintf( StaticStr, "%u", CurrentPage+1 );
    else
        sprintf( StaticStr, "%u", CurrentPage );
    Edit_SetText( AppData.hEditCurPage, StaticStr );

    if( TotalPages )
    {
        // See if there is a sound associated with this page
        if( PageList.GetSoundDataType( CurrentPage ) )
        {
            for( i = 0; i < NUM_TBSOUNDBUTTONS; ++i )
                TBBtn_SetState( AppData.hToolBar, BTN_SOUND_PLAY+i, ENABLE );
        }
        else
        {
            if( mediaPlayer.GetStatus() != PLAYER_PLAYING )
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
        #ifdef EBOOK_TOOL
        if( PageList.GetSoundDataType( CurrentPage ) )
            EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_ENABLED );
        else
            EnableMenuItem( GetMenu(hwnd), MNU_REMOVE_SOUND, MF_GRAYED|MF_DISABLED );
        #endif

        // Enable or disable export menu options if a sound is exportable
        #ifndef EBOOK_TOOL
        if( ProjPrefsData.AllowFileExports )
        {
            if( PageList.GetSoundDataType( CurrentPage ) )
                EnableMenuItem( GetMenu(hwnd), MNU_EXPORT_SOUND_FILE, MF_ENABLED );
            else
                EnableMenuItem( GetMenu(hwnd), MNU_EXPORT_SOUND_FILE, MF_GRAYED|MF_DISABLED );
        }
        #endif

        #ifdef SHARE_THE_MEMORIES
        // Update the caption text window
        if( AppData.hCaptionEdit )
            Edit_SetText( AppData.hCaptionEdit, PageList.GetPtrToCaption( CurrentPage ) );
        #endif

        if( !dib.IsEmpty() && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
        {
            #ifndef EBOOK_TOOL
            if( IsMenuChecked( hwnd, MNU_AUTO_SIZE ) )
            {
                TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_IN, DISABLE );
                TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_OUT, DISABLE );
            }
            else
            {
            #endif
                if( ProjPrefsData.ZoomIndex == ZOOM_SIZE_COUNT-1 )
                    TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_IN, DISABLE );
                else
                    TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_IN, ENABLE );

                if( ProjPrefsData.ZoomIndex == 0 )
                    TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_OUT, DISABLE );
                else
                    TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_OUT, ENABLE );
            #ifndef EBOOK_TOOL
            }
            #endif
        }
        else if( !TextBlock.IsEmpty() && AppData.pGBase->GetObjectType() == TEXT_OBJECT )
        {
            TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_IN, ENABLE );
            TBBtn_SetState( AppData.hToolBar, BTN_ZOOM_OUT, ENABLE );
        }
    }
    else
    {
        #ifdef SHARE_THE_MEMORIES
        // Update the caption text window
        if( AppData.hCaptionEdit )
            Edit_SetText( AppData.hCaptionEdit, "" );
        #endif

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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::CheckAllFilesAreThere( HWND hwnd )
{
    int i;
    char StrTemp[MAX_STR_PATH];
    BOOL Result = FALSE;
    LPTSTR ProjectBMPs[MAX_PROJECT_BMP];

    // Check all the misc project files
    // init the project BMP string so that we can put it into a for loop
    ProjectBMPs[0] = ProjPrefsData.StartUpScreenPath;
    ProjectBMPs[1] = ProjPrefsData.BackgroundBMPPath;

    // Go through all the paths and check that all the files are there.
    // If the file is not located in it's original path, check to see if it is in the project directory.
    // If the file is in the project directory, save the new file paths. 
    for( i = 0; i < MAX_PROJECT_BMP; ++i )
    {
        // If the file is not there, see if it is located in the same directory the project file is in
        if( !FileExist( ProjectBMPs[ i ] ) )
        {
            // If we found the file in the project directory, update the path to reflect the change
            if( FileExist( ProjectBMPs[ i ], ActProj.FilePath ) )
            {
                // Combine the project path and the file name because the information has changed
                AddFileToOtherPath( ProjectBMPs[ i ], ActProj.FilePath, StrTemp );
                strcpy( ProjectBMPs[ i ], StrTemp );

                ActProj.LiveProject = TRUE;
            }
            else
            {
                // Ask if they want to find the missing project file
                if( FindMissingProjectFile( hwnd, ProjectBMPs[ i ], 
                    Settings.GetProjectArtPath(), PAGE_FILE_TYPES ) )
                    ActProj.LiveProject = TRUE;
                else
                    goto ERROR_JUMP;
            }
        }
    }

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
#endif



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

#ifdef EBOOK_TOOL
BOOL TEbookWnd::FindMissingProjectFile( HWND hwnd, char *MissingFile, char *StartSearchPath, 
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
#endif



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

BOOL TEbookWnd::OnClose( HWND hwnd )
{   
    BOOL Result = CheckToSave( hwnd );

    // Looks like the app is going to quit so doo all the quit stuff
    if( Result )
    {
        // Save the secret ini file settings
        SaveSecretINIsettings();

        // Save the settings
        Settings.WritePrivateString();

        // Give the edit field back its mesproc
        if( AppData.OldEditProc )
            SetWindowLong( AppData.hEditCurPage, GWL_WNDPROC, LONG( AppData.OldEditProc ) );

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

void TEbookWnd::LoadKeyEntryPage()
{
    char NumberStr[20];
    int PageNumber;

    if( Edit_GetTextLength( AppData.hEditCurPage ) )
    {
        Edit_GetText( AppData.hEditCurPage, NumberStr, sizeof(char)*20 );
        PageNumber = atoi( NumberStr ) - 1;

        if( PageNumber >= 0 && PageNumber < PageList.Count() )
        {
            PageList.SetCurrentPage( PageNumber );
            FlipPage( AppData.hParentWnd, PAGE_FLIP_RIGHT, TRUE );
        }
    }

}   // LoadKeyEntryPage


/************************************************************************
*    FUNCTION NAME:         FindProject                                                             
*
*    DESCRIPTION:           Find the project executable
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
#ifdef EBOOK_TOOL
void TEbookWnd::FindProject( HWND hwnd )
{
    char PathToEXE[MAX_STR_PATH];
    char PathToDir[MAX_PATH];
    BOOL BuildResult = TRUE;
    HWND ViewWindow;

    // See if they have a project exe running. If so, kill the program
    // because we can't save the EXE while the other EXE is running.
    #ifndef SHARE_THE_MEMORIES

    char StrTmp[MAX_INFO+20];

    // Are we using the registration number
    if( ProjPrefsData.UseRegNum )
    {
        // See if they are running the shareware version
        sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, SHAREWARE_STR );
        ViewWindow = FindWindow( NULL, StrTmp );

        // If not try the registered version
        if( !ViewWindow )
        {
            sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, REGISTERED_STR );
            ViewWindow = FindWindow( NULL, StrTmp );
        }
    }
    else
    {
        ViewWindow = FindWindow( NULL, ProjPrefsData.Title );
    }
    #else
        ViewWindow = FindWindow( NULL, ProjPrefsData.Title );
    #endif
 
    if( ViewWindow )
    {
        PostMessage( ViewWindow, WM_CLOSE, 0, 0 );
        Sleep( 500 );
    }

    // Do we want to build the EXE?
    if( ActProj.LiveProject )
        BuildResult = mnuBuild_EXE( hwnd );

    if( BuildResult && PageList.Count() )
    {
        // Put together the path to the new exe file
        AddFileToOtherPath( ProjPrefsData.ExeName, ActProj.FilePath, PathToEXE );

        // Pull the directory from the file path
        CropOutPath( ActProj.FilePath, PathToDir );

        // Check to see if the file is there. If not, build it.
        if( !FileExist( PathToEXE ) )
            if( !mnuBuild_EXE( hwnd ) )
                return;

		// Tell them what file they want
		if( Settings.GetShowInstallInfo() )
        	Settings.SetShowInstallInfo( Display_YourInstallerDlg( AppData.hInst, hwnd ) );

        // If the function fails, the return value is an error value that is less than or equal to 32
        // The last aparam is a 0 to keep the DOS window from showing.
        if( ShellExecute( NULL, "open", PathToDir, NULL, NULL, SW_SHOWNORMAL ) <= (HINSTANCE)32 )
            PostMsg( "Opps","There has been an error locating your project executable." );
    }

}   // FindProject  
#endif



/************************************************************************
*    FUNCTION NAME:         SetupOrderFormInfo                                                             
*
*    DESCRIPTION:           Init specific TSetting class members that
*                           is used for the Order Form
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

void TEbookWnd::SetupOrderFormInfo()
{
    #ifdef EBOOK_TOOL
        TLoadRes Res;
        char StrTmp[MED_ARRAY_300];

        // Set the tile
        sprintf( StrTmp, "%s  %s    %s", PRODUCT_NAME, PRODUCT_VERSION, MAIL_ORDER_ONLY );
        Settings.SetTitle( StrTmp );
        // Set Company Name
        Settings.SetCompanyName( CO_NAME );
        // Set Company Address1
        Settings.SetCoAddressLine1( CO_ADDRESS1 );
        // Set Company Address2
        Settings.SetCoAddressLine2( "" );
        // Set Company City
        Settings.SetCoCity( CO_CITY );
        // Set Company Zip
        Settings.SetCoZip( CO_ZIP );
        // Set Company web site
        Settings.SetCoWebAddress( WEB_ADDRESS );
        // Set Company email address
        Settings.SetCoEmailAddress( EMAIL_ADDRESS );
        // Set the ordering info
        sprintf( StrTmp, ORDER_FORM_INFO, PRODUCT_NAME, PRODUCT_COST );
        Settings.SetOrderInfo( StrTmp );
        // Allow floppy sales
        Settings.SetOfferFloppy( TRUE );
        // Cost of the floppy disk
        Settings.SetFloppyCost( FLOPPY_DISK_COST );
        // Set the floppy info
        Settings.SetFloppyInfo( FLOPPY_DISK_INFO );
        // Cost of the product
        Settings.SetProductCost( PRODUCT_COST );
        // Set the sales tax
        Settings.SetSalesTax( SALES_TAX );
        // Set the state index to check if taxes need to be added
        Settings.SetCheckStateIndex( STATE_ORDER_INDEX );
        // Show the "No Printer Available" button
        Settings.SetNoPrinterAvailable( TRUE );
        // Set the credit cards stuff
        Settings.SetCreditCardInfo( CREDIT_CARD_INFO );
        Settings.SetVisa( TRUE );
        Settings.SetMasterCard( TRUE );
        Settings.SetDiscover( TRUE );
        Settings.SetAmericanExpress( TRUE );
        // Set Helpful Hint Stuff
        Settings.SetOFInfoCaption( "Helpful Hint" );
        Settings.SetOFInfoTitle( "Two Ways To Purchase" );
        Settings.SetOFInfoBufferCheckSum( DO_CHECK_SUM );
        // Print the logo
        Settings.SetPrintLogo( TRUE );
        // Load the resource to our text file
        LoadTheResource( AppData.hInst, "orderFrmInfo", "TXT", &Res );
        if( Res.pData )
            Settings.SetOFInfoBufferPtr( (char *)Res.pData );

        Settings.SetOFInfoBufferSize( Res.Size );
    #else

    #ifndef SHARE_THE_MEMORIES

        char StrTmp[MED_ARRAY_300];

        // Set the tile
        sprintf( StrTmp, "%s    %s", ProjPrefsData.Title, MAIL_ORDER_ONLY );
        Settings.SetTitle( StrTmp );
        // Set Company Name
        Settings.SetCompanyName( ProjPrefsData.NameOrCompany );
        // Set Company Address1
        Settings.SetCoAddressLine1( ProjPrefsData.AddressLine1 );
        // Set Company Address2
        Settings.SetCoAddressLine2( ProjPrefsData.AddressLine2 );
        // Set Company City
        Settings.SetCoCity( ProjPrefsData.City );
        // Set Company Zip
        Settings.SetCoZip( ProjPrefsData.Zip );
        // Set Company web site
        Settings.SetCoWebAddress( ProjPrefsData.WebAddress );
        // Set Company email address
        Settings.SetCoEmailAddress( ProjPrefsData.EmailAddress );
        // Set the ordering info
        sprintf( StrTmp, ORDER_FORM_INFO, ProjPrefsData.Title, ProjPrefsData.RegFee );
        Settings.SetOrderInfo( StrTmp );
        // Allow floppy sales
        Settings.SetOfferFloppy( FALSE );
        // Cost of the floppy disk
        Settings.SetFloppyCost( 0 );
        // Set the floppy info
        Settings.SetFloppyInfo( NULL );
        // Cost of the product
        Settings.SetProductCost( ProjPrefsData.RegFee );
        // Set the sales tax
        Settings.SetSalesTax( ProjPrefsData.SalesTax );
        // Set the state index to check if taxes need to be added
        Settings.SetCheckStateIndex( ProjPrefsData.StateIndex );
        // Show the "No Printer Available" button
        Settings.SetNoPrinterAvailable( TRUE );
        // Don't Print the logo
        Settings.SetPrintLogo( FALSE );
        // Set the credit cards stuff
        Settings.SetCreditCardInfo( CREDIT_CARD_INFO_VIEW );
        Settings.SetVisa( ProjPrefsData.AllowVisa );
        Settings.SetMasterCard( ProjPrefsData.AllowMasterCard );
        Settings.SetDiscover( ProjPrefsData.AllowDiscover );
        Settings.SetAmericanExpress( ProjPrefsData.AllowAmerExpress );
        // Set Helpful Hint Stuff
        Settings.SetOFInfoCaption( ProjPrefsData.OFInfoCap );
        Settings.SetOFInfoTitle( ProjPrefsData.OFInfoTitle );
        Settings.SetOFInfoBufferCheckSum( DONT_DO_CHECK_SUM );
        Settings.SetOFInfoBufferPtr( NULL );
        Settings.SetOFInfoBufferSize( 0 );
    #endif
    #endif

}   // SetupOrderFormInfo


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

BOOL TEbookWnd::ViewWnd_CreateViewWnd( HINSTANCE hInst, HWND hParent )
{
    // Create the view window
    AppData.hViewWnd = CreateWindow( "ViewWnd", "View Window",
            WS_CHILD|WS_BORDER|WS_VISIBLE, 0, 0,   // |WS_CLIPSIBLINGS
            1, 1, hParent, HMENU(VIEW_WND), hInst, NULL );

    if( AppData.hViewWnd )
        ViewWndActive = TRUE;

    return ( AppData.hViewWnd != NULL );

}   // ViewWnd_CreateViewWnd


/************************************************************************
*    FUNCTION NAME:         eBook_OnDropFiles                                                             
*
*    DESCRIPTION:           Standard Windows WM_DROPFILES message handler.
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

#ifdef EBOOK_TOOL
void TEbookWnd::ViewWnd_OnDropFiles( HWND hwnd, HDROP hdrop )
{
    int NumOfFiles, i;
    char FilePath[ MAX_PATH+1 ];
    TStringList StringList;

    // Get the number of files dropped onto the window
    NumOfFiles = DragQueryFile( hdrop, GET_FILE_DROP_COUNT, NULL, 0 );

    // Make sure we can drag and drop at this point
    if( ActProj.OpenedFile || ActProj.LiveProject )
    {
        // Show the status window but only if we have files to show
        if( NumOfFiles > 1 )
        {
            // show the status window
            DisplayStatusWnd( AppData.hInst, hwnd );

            // Setup the status window
            StatusWnd_SetStatusBar( 0, NumOfFiles, 1 );
        }

        // Add each file to the string list program so that all the file
        // names can be sorted
        for( i = 0; i < NumOfFiles; ++i )
        {
            // Get the file name
            DragQueryFile( hdrop, i, FilePath, sizeof(char)*(MAX_PATH+1) );

            // Add all the paths the the string list class
            // for easy sorting
            StringList.Add( FilePath );
        }

        // If there is more than 1 path then sort it
        if( NumOfFiles > 1 )
            StringList.Sort();

        // Go through each file and check if it is a valid file
        for( i = 0; i < StringList.Count(); ++i )
        {
            // Set the status bar text
            StatusWnd_SetStatusTxt( "Loading and Testing file (%s).", StringList.GetString( i ) );

            // Loads and tests each image and makes a page entry
            if( !AddMainFileToPageList( StringList.GetString( i ) ) )
                goto LOAD_ERROR;

            // Inc the status bar
            StatusWnd_MoveStatusBar();
        }

        // Displays the new page or the last page of a group
        DisplayLoadedPage( AppData.hParentWnd );
    }

LOAD_ERROR:;

    // Hide the status window
    StatusWnd_HideStatusWnd();

    // Free the windows memory given to us for this drag and drop
    DragFinish( hdrop );
    
}   // eBook_OnDropFiles
#endif


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

void TEbookWnd::ViewWnd_OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;
    HDC PaintDC;
    RECT DisplayRect;
    int offset;

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    PaintDC = BeginPaint( hwnd, &paintStruct );

    // Get the rect of our view window
    GetClientRect( hwnd, &DisplayRect );

    // Blit the graphic to the screen
    // The class also takes care of the realizing the palette if in 256 colors
    // The image is blitted to the size of the view window so if the size of the view
    // window changes, so does the blit
    if( PaintDC && ViewWndActive )
    {
        // Blit the dib if there is one
        if( !dib.IsEmpty() && AppData.pGBase && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
        {
            #ifdef EBOOK_TOOL
            if( ProjPrefsData.ZoomIndex == ZOOM_100_PERCENT_INDEX )
            #else
            if( ProjPrefsData.ZoomIndex == ZOOM_100_PERCENT_INDEX && 
                !IsMenuChecked( AppData.hParentWnd, MNU_AUTO_SIZE ) )
            #endif
            {
                dib.Draw( PaintDC );
            }
            else
            {
                dib.StretchDraw( PaintDC, &DisplayRect );
            }
        }
        // Text if there is one
        else if( !TextBlock.IsEmpty() && AppData.pGBase && AppData.pGBase->GetObjectType() == TEXT_OBJECT )
        {
            if( ProjPrefsData.DefFont.lfFaceName[0] )
            {           
                SelectFont( PaintDC, TextBlock.GetFont() );
                SetTextColor( PaintDC, ProjPrefsData.DefaultTxtColor );
            }
            else
                SelectFont( PaintDC, GetStockFont( DEFAULT_GUI_FONT ) );

            if( ProjPrefsData.DefBckgrdTxtColor != NO_COLOR_SET )
                SetBkColor( PaintDC, ProjPrefsData.DefBckgrdTxtColor );
            else
                SetBkColor( PaintDC, GetSysColor( COLOR_WINDOW ) );

            // Calculate the offset depending on what the screen resolution is
            offset = (GetSystemMetrics(SM_CXSCREEN) * GetSystemMetrics(SM_CYSCREEN)) * TEXT_OFFSET;

            // The TextBlock only tries to blit if it contains text
            TextBlock.WrapText( PaintDC, offset, offset, AppData.ViewWndRect.right - (offset * 2) );
        }
    }

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // ViewWnd_OnPaint



/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnDestroy()                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*                           Here we are cleaning up what we have done.
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

void TEbookWnd::ViewWnd_OnDestroy( HWND hwnd )
{
    // Delete the text window brush
    if( BackGrndBrush )
    {
        DeleteObject( BackGrndBrush );
        BackGrndBrush = NULL;
    }
}   // ViewWnd_OnDestroy



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

BOOL TEbookWnd::ViewWnd_OnEraseBkgnd( HWND hwnd, HDC hdc )
{
    HBRUSH brush;
    RECT DisplayRect;

    if( ViewWndActive )
    {
        // Fill the view window with the system edit color
        if( dib.IsEmpty() || (AppData.pGBase && AppData.pGBase->GetObjectType() == TEXT_OBJECT) )
        {
            // Get the rect of our view window
            GetClientRect( hwnd, &DisplayRect );

            // Is the background color changed?
            if( ProjPrefsData.DefBckgrdTxtColor != NO_COLOR_SET )
                brush = CreateSolidBrush( ProjPrefsData.DefBckgrdTxtColor );
            else
                brush = CreateSolidBrush( GetSysColor( COLOR_WINDOW ) );

            FillRect( hdc, &DisplayRect, brush );
            DeleteObject( brush );
        }
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

BOOL TEbookWnd::ViewWnd_OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    #ifdef EBOOK_TOOL
        // Register the window to accept files
        DragAcceptFiles( hwnd, TRUE );
    #endif

    // The tool program doesn't have this information until a file is opened.
    // The view program has this information on startup.
    // If a new font has been selected, make the font
    // We can tell this if a font name has been saved
    if( ProjPrefsData.DefFont.lfFaceName[0] )
    {
        // Make the new font
        // Setting the color here does nothing for Windows controls. For
        // changing colors in controls, see ViewWnd_OnCtlColor.
        TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
        0, 0, &ProjPrefsData.DefFont );
    }

    return TRUE;

}   // ViewWnd_OnCreate



/************************************************************************
*    FUNCTION NAME:         ViewWnd_ResetWnd                                                             
*
*    DESCRIPTION:           Resize the window to the parent
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hParent  - Handle of the parent windowe 
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

void TEbookWnd::ViewWnd_ResetWnd( HWND hParent )
{
    RECT PRect, ResultRect, InvalidRect, DisplayRect;
    int offset;
    #ifndef EBOOK_TOOL
    double WindowW, WindowH, MultiW, MultiH, Multiplyer;
    double ImageW, ImageH;
    #endif

    if( AppData.hViewWnd )
    {
        // Get the client rect of parent window
        GetClientRect( hParent, &PRect );

        // Get the rect of our view window
        GetClientRect( AppData.hViewWnd, &DisplayRect );

        // Set the default window size
        // If there's no bitmat to show, put up a blank window
        ResultRect.left = VIEW_WND_OFFSET_X;
        ResultRect.top = VIEW_WND_OFFSET_Y;
        ResultRect.right = PRect.right - (VIEW_WND_OFFSET_X*2);
        ResultRect.bottom = PRect.bottom - (VIEW_WND_OFFSET_Y*2);

        if( !dib.IsEmpty() && AppData.pGBase && AppData.pGBase->GetObjectType() == BITMAP_OBJECT )
        {
            #ifndef EBOOK_TOOL
            if( !IsMenuChecked( AppData.hParentWnd, MNU_AUTO_SIZE ) )
            {
            #endif
                // Center it if the image window is smaller
                if( (dib.width() * AppData.ZoomPercent) > PRect.right - GetSystemMetrics(SM_CXHSCROLL) )
                    ResultRect.left = VIEW_WND_OFFSET_Y;
                else
                    ResultRect.left = (PRect.right - (dib.width() * AppData.ZoomPercent)) / 2;

                // Center it if the image window is smaller
                if( (dib.height() * AppData.ZoomPercent) > PRect.bottom - GetSystemMetrics(SM_CYHSCROLL) )
                    ResultRect.top = VIEW_WND_OFFSET_Y;
                else
                    ResultRect.top = (PRect.bottom - (dib.height() * AppData.ZoomPercent)) / 2;

                ResultRect.right = (dib.width() * AppData.ZoomPercent) - GetSystemMetrics(SM_CXBORDER);
                ResultRect.bottom = (dib.height() * AppData.ZoomPercent);
            #ifndef EBOOK_TOOL
            }
            else
            {
                ImageW = dib.width();
                ImageH = dib.height();
                WindowW = PRect.right - (VIEW_WND_OFFSET_Y * 2);
                WindowH = PRect.bottom - (VIEW_WND_OFFSET_Y * 2);

                // The width or the height of any bitmap can't be zero. There is a problem
                if( !ImageW || !ImageH || !WindowW || !WindowH )
                {
                    PostMsg( "Display Error", "Bitmap has no size" );
                    return;
                }

                // Who is bigger. The image or the window surface
                // Divide the largest number against the smaller number
                if( ImageW < WindowW )
                    MultiW = WindowW / ImageW;
                else
                    MultiW = ImageW / WindowW;

                if( ImageH < WindowH )
                    MultiH = WindowH / ImageH;
                else
                    MultiH = ImageH / WindowH;                       

                // If the image is smaller than the printable area
                // Multiply the multiplyer. If it is bigger than the
                // printable area then divide.
                if( ImageW < WindowW && ImageH < WindowH )  // Enlarge to fit
                {
                    // See if one side will enlarge past the window rect
                    // Add 1 to compensate for floating point math to the window sizes
                    if( ImageW * MultiW > WindowW+1 || ImageH * MultiW > WindowH+1 )
                        Multiplyer = MultiH;
                    else if( ImageW * MultiH > WindowW+1 || ImageH * MultiH > WindowH+1 )
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

                    ResultRect.right = ImageW * Multiplyer;
                    ResultRect.bottom = ImageH * Multiplyer;
                }
                else  // Reduce to fit
                {
                    // See if one side will enlarge past the window rect
                    // Add 1 to compensate for floating point math to the window sizes
                    if( ImageW / MultiW > WindowW+1 || ImageH / MultiW > WindowH+1 )
                        Multiplyer = MultiH;
                    else if( ImageW / MultiH > WindowW+1 || ImageH / MultiH > WindowH+1 )
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

                    ResultRect.right = ImageW / Multiplyer;
                    ResultRect.bottom = ImageH / Multiplyer;
                }

                // Center the image onto the page
                ResultRect.left = ((WindowW - ResultRect.right) / 2) + VIEW_WND_OFFSET_Y;
                ResultRect.top = ((WindowH - ResultRect.bottom) / 2) + VIEW_WND_OFFSET_Y;
            }
            #endif
        }
        else if( !TextBlock.IsEmpty() && AppData.pGBase && AppData.pGBase->GetObjectType() == TEXT_OBJECT )
        {
            // Size the text window to fit the view window
            // If the text block is not empty, we are to display text
            char SampleTextLgnth[] = { "aDwtWfbJ w,hrw Yter f,hrjha an a,n Whas H s,jsI isns h,hh Whh hhh, vjwtof gu,snc tohwm ,ashyeon ma,tnaiogn th" };
            SIZE TextSize;

            // Calculate the offset depending on what the screen resolution is
            offset = (GetSystemMetrics(SM_CXSCREEN) * GetSystemMetrics(SM_CYSCREEN)) * TEXT_OFFSET;

            // terminate it to the default string length
            SampleTextLgnth[61] = 0;
            
            // Get the block height in pixels so we know how high to make the window
            TextSize = TextBlock.BlockHeightWidthInPixels( AppData.hViewWnd, SampleTextLgnth );
            
            // Only use the new height if it is greater than the windows default height             
            if( TextSize.cy + (offset*2) > ResultRect.bottom )
                ResultRect.bottom = TextSize.cy + (offset*2);

            // Only center the window if it is smaller than the parent window
            if( TextSize.cx + (offset * 2) <= PRect.right )
                ResultRect.left = (PRect.right - (TextSize.cx + (offset * 2))) / 2;
            else
                ResultRect.left = VIEW_WND_OFFSET_Y;

            // Record the new width of the text window                    
            ResultRect.right = TextSize.cx + (offset*2);
        }

        // Set the invalid rect structure which is the new size of the window
        SetRect( &InvalidRect, 0, 0, ResultRect.right+2, ResultRect.bottom+2 );

        // Save the rect of the view window
        AppData.ViewWndRect = ResultRect;

        // Invalidate the whole window so it all gets painted
        InvalidateRect( AppData.hViewWnd, &InvalidRect, TRUE );

        // The MoveWindow will cause the window to be painted
        // Add two pixels to the width and height to compensate for the line drawn around the window
        MoveWindow( AppData.hViewWnd, 
                    ResultRect.left, ResultRect.top, ResultRect.right+2, ResultRect.bottom+2, TRUE );
    }

}   // ViewWnd_ResetWnd


/************************************************************************
*    FUNCTION NAME:         ViewWnd_SetTextFont                                                             
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

void TEbookWnd::ViewWnd_SetTextFont( HWND hwnd )
{
    CHOOSEFONT cf;
    LOGFONT    lf;

    // Memset the two structures
    memset( &cf, 0, sizeof( CHOOSEFONT ) );

    // Init with the current log font or init to zero
    if( ProjPrefsData.DefFont.lfFaceName[0] )
    {
        memcpy( &lf, &ProjPrefsData.DefFont, sizeof( LOGFONT ) );
        cf.rgbColors = ProjPrefsData.DefaultTxtColor;
    }
    else
        memset( &lf, 0, sizeof( LOGFONT ) );

    // Init the CHOOSEFONT structure
    cf.lStructSize = sizeof( CHOOSEFONT );
    cf.hwndOwner   = hwnd;
    cf.Flags       = CF_BOTH|CF_EFFECTS|CF_FORCEFONTEXIST|CF_INITTOLOGFONTSTRUCT;
    cf.lpLogFont   = &lf;

    // Save the font choices
    if( ChooseFont( &cf ) )
    {
        // Copy over the logfont structure
        memcpy( &ProjPrefsData.DefFont, &lf, sizeof( LOGFONT ) );

        // Save the color information
        ProjPrefsData.DefaultTxtColor = cf.rgbColors;

        // Make the new font, delete the old
        TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
                               ProjPrefsData.DefaultTxtColor, 
                               0, &ProjPrefsData.DefFont );

        // Redraw the view window
        ViewWnd_ResetWnd( AppData.hMidWnd );

        #ifdef EBOOK_TOOL
        // Set the project live status
        ActProj.LiveProject = TRUE;
        #endif
    }

}   // ViewWnd_SetTextFont



/************************************************************************
*    FUNCTION NAME:         ViewWnd_SetBackGrndColor                                                             
*
*    DESCRIPTION:           get the color for the background of the edit window
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

void TEbookWnd::ViewWnd_SetBackGrndColor( HWND hwnd )
{
    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )

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
        ProjPrefsData.DefBckgrdTxtColor = cc.rgbResult;

        // Redraw the view window
        ViewWnd_ResetWnd( AppData.hMidWnd );

        #ifdef EBOOK_TOOL
        // Set the project live status
        ActProj.LiveProject = TRUE;
        #endif
    }

    #endif

}   // ViewWnd_SetBackGrndColor



/************************************************************************
*    FUNCTION NAME:         ViewWnd_ResetFontDefaults()                                                             
*
*    DESCRIPTION:           Reset all the font and color choices
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

void TEbookWnd::ViewWnd_ResetFontDefaults( HWND hwnd )
{
    #ifdef EBOOK_TOOL
    // Free the font because we are not using it any more
    TextBlock.FreeFont();

    // Copy over the logfont structure
    memset( &ProjPrefsData.DefFont, 0, sizeof( LOGFONT ) );

    // Reset the text colors
    ProjPrefsData.DefaultTxtColor = 0;
    ProjPrefsData.DefBckgrdTxtColor = NO_COLOR_SET;
    
    // Set the project live status
    ActProj.LiveProject = TRUE;

    #else

    // Save the zoom index so that it can be restored
    BYTE TmpZoomIndex = ProjPrefsData.ZoomIndex;

    // Load in the project defaults
    OpenEXEandReadHeader();

    // Restore the zoom index
    ProjPrefsData.ZoomIndex = TmpZoomIndex;

    // Make the new font, delete the old
    TextBlock.MakeNewFont( AppData.hViewWnd, NULL, 0, 0, 
                           ProjPrefsData.DefaultTxtColor, 
                           0, &ProjPrefsData.DefFont );

    #endif

    // Redraw the view window
    ViewWnd_ResetWnd( AppData.hMidWnd );

}   // ViewWnd_ResetFontDefaults