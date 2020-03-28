/************************************************************************
*
*    PRODUCT:         Art 4 Sale
*
*    FILE NAME:       Preferences.cpp
*
*    DESCRIPTION:     Preferences dialog box
*
*    IMPLEMENTATION:  Dialog box for users to fill in their project info
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
//#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "Prefs.h"           // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "gTools.h"          // A misc group of useful functions
#include "PictLoad.h"        // Dialog for loading pictures that shows a preview window

// Globals
PTPrefsDlg pPrefsDlg;
extern char States[NUM_US_STATES+1][3];


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayPrefsDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to parent window
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

BOOL DisplayPrefsDialogBox( HINSTANCE hInst, HWND hwnd, PTProjectPrefs pProjPrefs, char *pArtPath )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pPrefsDlg = new TPrefsDlg();

    // Create the dialog box
    if( pPrefsDlg )
        Result = pPrefsDlg->CreateDialogWnd( hInst, hwnd, pProjPrefs, pArtPath );

    return Result;
}


/************************************************************************
*    FUNCTION NAME:         PrefsDlgProc
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
*    h rosenorn    3/30/00       Function Created
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
}   // PrefsDlgDlgProc


/************************************************************************
*    FUNCTION NAME:         EdtSubClass_Fee                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep specific keystrokes from being
*                           keyed into the edit field. We only the user to
*                           be able to type numbers, delete keys and the
*                           period key.
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
#ifndef SHARE_THE_MEMORIES
LRESULT CALLBACK EdtSubClass_Fee( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    // Search for these keyboard messages and kill any messages that fall
    // into this range. 
    if( Message == WM_KEYDOWN || Message == WM_KEYUP || Message == WM_CHAR )
    {
        if( (wParam < NUM_0_KEY || wParam > NUM_9_KEY) &&
            !(wParam == BACKSPACE_KEY || wParam == DELETE_KEY || wParam == PERIOD_KEY ||
            wParam == LEFT_ARROW_KEY || wParam == RIGHT_ARROW_KEY ||
            wParam == UP_ARROW_KEY || wParam == DOWN_ARROW_KEY) )
        {
            return FALSE;
        }
    }

    return CallWindowProc( pPrefsDlg->GetEdtProc_Fee(), hwnd, Message, wParam, lParam );

}   // EdtSubClass_Fee
#endif


/************************************************************************
*    FUNCTION NAME:         EdtSubClass_Taxes                                                             
*
*    DESCRIPTION:           Subclassing the Edit field to trap keyboard
*                           messages to keep specific keystrokes from being
*                           keyed into the edit field. We only the user to
*                           be able to type numbers, delete keys and the
*                           period key.
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
#ifndef SHARE_THE_MEMORIES
LRESULT CALLBACK EdtSubClass_Taxes( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    // Search for these keyboard messages and kill any messages that fall
    // into this range. 
    if( Message == WM_KEYDOWN || Message == WM_KEYUP || Message == WM_CHAR )
    {
        if( (wParam < NUM_0_KEY || wParam > NUM_9_KEY) &&
            !(wParam == BACKSPACE_KEY || wParam == DELETE_KEY || wParam == PERIOD_KEY ||
            wParam == LEFT_ARROW_KEY || wParam == RIGHT_ARROW_KEY ||
            wParam == UP_ARROW_KEY || wParam == DOWN_ARROW_KEY) )
        {
            return FALSE;
        }
    }

    return CallWindowProc( pPrefsDlg->GetEdtProc_Taxes(), hwnd, Message, wParam, lParam );

}   // EdtSubClass_Taxes
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

TPrefsDlg::TPrefsDlg()
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

TPrefsDlg::~TPrefsDlg()
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

BOOL TPrefsDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, PTProjectPrefs pProjPrefs, char *pArtPath )
{
    // Init class members
    hInst = hInstance;
    pProjPrefsData = pProjPrefs;
    ArtPath = pArtPath;
	hParentWnd = hwnd;

    // Create the dialog box
    return DialogBox( hInst, "DLG_PROJECT_PREFS", hwnd, (DLGPROC)PrefsDlg_Proc );

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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TPrefsDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
	RECT rect;

    // Center the window to the parent window
    GetWindowRect( hwnd, &rect );
    CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );

    // Limit Window title
    Edit_LimitText( GetDlgItem( hwnd, EDT_APP_TITLE ), MAX_TITLE );
    // Limit Name of exe file
    Edit_LimitText( GetDlgItem( hwnd, EDT_EXE_NAME ), MAX_EXE_NAME );
    // Limit number for web address
    Edit_LimitText( GetDlgItem( hwnd, EDT_WEB_ADDRESS ), MAX_INFO );

    // Init the members of the preferences dialog box
    Edit_SetText( GetDlgItem( hwnd, EDT_APP_TITLE ), pProjPrefsData->Title );
    Edit_SetText( GetDlgItem( hwnd, EDT_EXE_NAME ), pProjPrefsData->ExeName );
    Button_SetCheck( GetDlgItem( hwnd, CHK_ALLOW_PRINTING ), pProjPrefsData->AllowPrintPage );
    Button_SetCheck( GetDlgItem( hwnd, CHK_PRINT_PAGE_NO ), pProjPrefsData->PrintPageNumbers );
    Button_SetCheck( GetDlgItem( hwnd, CHK_ALLOW_FILE_EXPORTS ), pProjPrefsData->AllowFileExports );
    Button_SetCheck( GetDlgItem( hwnd, CHK_CHECKSUM ), pProjPrefsData->DoChecksum );

    // No longer needed
    //Button_SetCheck( GetDlgItem( hwnd, CHK_COMPRESS ), pProjPrefsData->CompressImages );
    Edit_SetText( GetDlgItem( hwnd, EDT_WEB_ADDRESS ), pProjPrefsData->WebAddress );

    // Set the check box for the startup screen
    if( pProjPrefsData->StartUpScreenPath[0] )
        Button_SetCheck( GetDlgItem( hwnd, CHK_STARTUP_SCREEN ), ENABLE );

    // Set the check box for the background bitmap
    if( pProjPrefsData->BackgroundBMPPath[0] )
        Button_SetCheck( GetDlgItem( hwnd, CHK_USE_BK_BITMAP ), ENABLE );

    // Disable the print page no. and print copyright info if printing is not allowed
    if( !pProjPrefsData->AllowPrintPage )
        Button_Enable( GetDlgItem( hwnd, CHK_PRINT_PAGE_NO ), DISABLE );


    #ifndef SHARE_THE_MEMORIES

    char StrTemp[50];
    int i;

    // Subclass Edit box control
    OldEditProcFee = (WNDPROC)SetWindowLong( GetDlgItem( hwnd, EDT_REG_FEE ), 
                                          GWL_WNDPROC, LONG( EdtSubClass_Fee ) );

    OldEditProcTaxes = (WNDPROC)SetWindowLong( GetDlgItem( hwnd, EDT_SALES_TAX ), 
                                          GWL_WNDPROC, LONG( EdtSubClass_Taxes ) );

    // Limit registration number required for this software
    Edit_LimitText( GetDlgItem( hwnd, EDT_CUST_PASS ), MAX_REG_NUM );
    // Limit number indicating when to show registration notice
    Edit_LimitText( GetDlgItem( hwnd, EDT_USE_PASS_AFTER ), MAX_PAGES_BEFOR_REG_NUM );
    // Limit number for name
    Edit_LimitText( GetDlgItem( hwnd, EDT_INFO_NAME ), MAX_INFO );
    // Limit number for address1
    Edit_LimitText( GetDlgItem( hwnd, EDT_INFO_ADDRESS1 ), MAX_INFO );
    // Limit number for address2
    Edit_LimitText( GetDlgItem( hwnd, EDT_INFO_ADDRESS2 ), MAX_INFO );
    // Limit number for city
    Edit_LimitText( GetDlgItem( hwnd, EDT_CITY ), MAX_INFO );
    // Limit number for zip
    Edit_LimitText( GetDlgItem( hwnd, EDT_ZIP ), SMALL_ARRAY_50 );
    // Limit number for email address
    Edit_LimitText( GetDlgItem( hwnd, EDT_EMAIL_ADDRESS ), MAX_INFO );
    // Limit number for sales tax
    Edit_LimitText( GetDlgItem( hwnd, EDT_SALES_TAX ), MAX_SALES_TAX );
    // Limit number for reg fee
    Edit_LimitText( GetDlgItem( hwnd, EDT_REG_FEE ), MAX_REG_FEE );
    // Limit number for copyright info
    Edit_LimitText( GetDlgItem( hwnd, EDT_COPYRIGHT_INFO ), MAX_COPYRIGHT_INFO );
    // Limit credit card web address
    Edit_LimitText( GetDlgItem( hwnd, EDT_CREDIT_URL ), MAX_INFO );

    // Init the members of the preferences dialog box
    Edit_SetText( GetDlgItem( hwnd, EDT_CUST_PASS ), pProjPrefsData->RegNo );
    Edit_SetText( GetDlgItem( hwnd, EDT_COPYRIGHT_INFO ), pProjPrefsData->CopyrightInfo );
    sprintf( StrTemp, "%u", pProjPrefsData->AskForRegAfterPage );
    Edit_SetText( GetDlgItem( hwnd, EDT_USE_PASS_AFTER ), StrTemp );
    Button_SetCheck( GetDlgItem( hwnd, CHK_USE_REGNO ), pProjPrefsData->UseRegNum );
    Button_SetCheck( GetDlgItem( hwnd, CHK_VISA ), pProjPrefsData->AllowVisa );
    Button_SetCheck( GetDlgItem( hwnd, CHK_MASTERCARD ), pProjPrefsData->AllowMasterCard );
    Button_SetCheck( GetDlgItem( hwnd, CHK_DISCOVER ), pProjPrefsData->AllowDiscover );
    Button_SetCheck( GetDlgItem( hwnd, CHK_AMERICAN_EXP ), pProjPrefsData->AllowAmerExpress );

    Edit_SetText( GetDlgItem( hwnd, EDT_INFO_NAME ), pProjPrefsData->NameOrCompany );
    Edit_SetText( GetDlgItem( hwnd, EDT_INFO_ADDRESS1 ), pProjPrefsData->AddressLine1 );
    Edit_SetText( GetDlgItem( hwnd, EDT_INFO_ADDRESS2 ), pProjPrefsData->AddressLine2 );
    Edit_SetText( GetDlgItem( hwnd, EDT_CITY ), pProjPrefsData->City );
    Edit_SetText( GetDlgItem( hwnd, EDT_ZIP ), pProjPrefsData->Zip );
    Edit_SetText( GetDlgItem( hwnd, EDT_EMAIL_ADDRESS ), pProjPrefsData->EmailAddress );
    Edit_SetText( GetDlgItem( hwnd, EDT_CREDIT_URL ), pProjPrefsData->CreditWebAddress );

    if( pProjPrefsData->RegFee )
    {
        sprintf( StrTemp, "%.2lf", pProjPrefsData->RegFee );
        Edit_SetText( GetDlgItem( hwnd, EDT_REG_FEE ), StrTemp );
    }

    if( pProjPrefsData->SalesTax )
    {
        sprintf( StrTemp, "%.2lf", pProjPrefsData->SalesTax );
        Edit_SetText( GetDlgItem( hwnd, EDT_SALES_TAX ), StrTemp );
    }

    // Add the state strings to the combo box
    for( i = 0; i < NUM_US_STATES+1; ++i )
        ComboBox_AddString( GetDlgItem( hwnd, CBO_STATE ), States[i] );

    // Set the combo box position
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_STATE ), pProjPrefsData->StateIndex );

    #endif

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
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPrefsDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {
        case BTN_OK:
            // Save the select preferences
            SaveProjectPrefs( hwnd );

            // Kill this dialog
            EndDialog( hwnd, TRUE );  
        break;

        case BTN_CANCEL:
        case DLG_QUIT_BOX:
            // Kill this dialog
            EndDialog( hwnd, FALSE );
        break;

        case CHK_STARTUP_SCREEN:
            GetFilePath( hwnd, CHK_STARTUP_SCREEN, pProjPrefsData->StartUpScreenPath, 
                                  "Select Bitmap for Start Up Screen", BITMAP_FILE_TYPES );
        break;

        case CHK_USE_BK_BITMAP:
            GetFilePath( hwnd, CHK_USE_BK_BITMAP, pProjPrefsData->BackgroundBMPPath, 
                                  "Select Bitmap for Background Tiling", BITMAP_FILE_TYPES );
        break;

        case CHK_ALLOW_PRINTING:
            // Enable or Disable the print page no. and print copyright info  
            // depending on the state of the Allow printing button
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_ALLOW_PRINTING ) ) )
                Button_Enable( GetDlgItem( hwnd, CHK_PRINT_PAGE_NO ), ENABLE );
            else
                Button_Enable( GetDlgItem( hwnd, CHK_PRINT_PAGE_NO ), DISABLE );

        break;
    }
}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         GetFilePath                                                             
*
*    DESCRIPTION:           Get's the file path or clears out the path array
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

void TPrefsDlg::GetFilePath( HWND hwnd, int DlgItem, char *FileName, char *Title, char *FileMask )
{
    // Check to see if the button is checked
    if( Button_GetCheck( GetDlgItem( hwnd, DlgItem ) ) )
    {
        // Setup the extra flags for this dialog box
        DWORD ExtraFlags = OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_EXPLORER;

        if( !PictLoad_GetPicture( hInst, hwnd, Title,
            ExtraFlags, FileName, MAX_STR_PATH, FileMask, ArtPath ) )
        {
            // If they cancel out, uncheck the check box and zero out the path
            Button_SetCheck( GetDlgItem( hwnd, DlgItem ), FALSE );
            FileName[0] = 0;
        }
    }
    else
        // If they uncheck the check box, zero out the path
       FileName[0] = 0;

}   // GetFilePath


/************************************************************************
*    FUNCTION NAME:         SaveProjectPrefs                                                             
*
*    DESCRIPTION:           Save information recorded in the prefs dialog box
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

void TPrefsDlg::SaveProjectPrefs( HWND hwnd )
{
    char TempStr[50];
    int Length, i;


    // Window Title
    Edit_GetText( GetDlgItem( hwnd, EDT_APP_TITLE ), pProjPrefsData->Title, sizeof(char)*(MAX_TITLE+1) );

    // Name of exe file
    Edit_GetText( GetDlgItem( hwnd, EDT_EXE_NAME ), pProjPrefsData->ExeName, sizeof(char)*(MAX_EXE_NAME+1) );

    // Save Users web address
    Edit_GetText( GetDlgItem( hwnd, EDT_WEB_ADDRESS ), pProjPrefsData->WebAddress, sizeof(char)*(MAX_INFO+1) );

    // See if they included a proper *.EXE extension
    // Get the file extension
    CropExtensionFromPath( pProjPrefsData->ExeName, TempStr );

    // Upper case the file extension for compairison
    strupr( TempStr );

    // Do we have a proper extension?
    if( strcmp( TempStr, "EXE" ) != 0 )
    {
        // Get the length of the string
        Length = strlen( pProjPrefsData->ExeName );

        // See if we find the "."
        for( i = 0; i < Length; ++i )
        {
            // Terminate at the '.' if there is one
            // They could have entered more then one
            if( pProjPrefsData->ExeName[i] == '.' )
            {
                if( i > 0 )
                    pProjPrefsData->ExeName[i] = 0;
                else
                    // Just incase the first entry is a "." then the whole thing is wrong
                    strcpy( pProjPrefsData->ExeName, PRO_EXAMPLE_EXE_NAME );

                break;
            }   
        }

        // add in the approate extension
        strcat( pProjPrefsData->ExeName, ".exe" );
    }

    // Flag indicating if files are to be compressed
    // No longer needed
    //pProjPrefsData->CompressImages = Button_GetCheck( GetDlgItem( hwnd, CHK_COMPRESS ) );
    // Flag indicating if printing of pages is allowed      
    pProjPrefsData->AllowPrintPage = Button_GetCheck( GetDlgItem( hwnd, CHK_ALLOW_PRINTING ) );
    pProjPrefsData->PrintPageNumbers = Button_GetCheck( GetDlgItem( hwnd, CHK_PRINT_PAGE_NO ) );
    pProjPrefsData->AllowFileExports = Button_GetCheck( GetDlgItem( hwnd, CHK_ALLOW_FILE_EXPORTS ) );

    // Flag indicating if the program should do a checksum  
    pProjPrefsData->DoChecksum = Button_GetCheck( GetDlgItem( hwnd, CHK_CHECKSUM ) );

    #ifndef SHARE_THE_MEMORIES
    // Save Users credit card web address
    Edit_GetText( GetDlgItem( hwnd, EDT_CREDIT_URL ), pProjPrefsData->CreditWebAddress, sizeof(char)*(MAX_INFO+1) );
    // Registration number required for this software   
    Edit_GetText( GetDlgItem( hwnd, EDT_CUST_PASS ), pProjPrefsData->RegNo, sizeof(char)*(MAX_REG_NUM+1) );
    // Copyright information   
    Edit_GetText( GetDlgItem( hwnd, EDT_COPYRIGHT_INFO ), pProjPrefsData->CopyrightInfo, sizeof(char)*(MAX_COPYRIGHT_INFO+1) );

    // Flag indicating if a registration number is needed           
    pProjPrefsData->UseRegNum = Button_GetCheck( GetDlgItem( hwnd, CHK_USE_REGNO ) ); 

    // Show registration notice ofter how many pages
    if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_USE_PASS_AFTER ) ) )
    {
        Edit_GetText( GetDlgItem( hwnd, EDT_USE_PASS_AFTER ), TempStr, sizeof(char)*50 );           
        pProjPrefsData->AskForRegAfterPage = (WORD)atoi( TempStr );
    }
    else
        pProjPrefsData->AskForRegAfterPage = 0;

    // Flags to allow for credit card transactions  
    pProjPrefsData->AllowVisa = Button_GetCheck( GetDlgItem( hwnd, CHK_VISA ) );          
    pProjPrefsData->AllowMasterCard = Button_GetCheck( GetDlgItem( hwnd, CHK_MASTERCARD ) );
    pProjPrefsData->AllowDiscover = Button_GetCheck( GetDlgItem( hwnd, CHK_DISCOVER ) );
    pProjPrefsData->AllowAmerExpress = Button_GetCheck( GetDlgItem( hwnd, CHK_AMERICAN_EXP ) );

    // Save Users name
    Edit_GetText( GetDlgItem( hwnd, EDT_INFO_NAME ), pProjPrefsData->NameOrCompany, sizeof(char)*(MAX_INFO+1) );

    // Save Users address line 1
    Edit_GetText( GetDlgItem( hwnd, EDT_INFO_ADDRESS1 ), pProjPrefsData->AddressLine1, sizeof(char)*(MAX_INFO+1) );

    // Save Users address line 2
    Edit_GetText( GetDlgItem( hwnd, EDT_INFO_ADDRESS2 ), pProjPrefsData->AddressLine2, sizeof(char)*(MAX_INFO+1) );

    // Save Users city
    Edit_GetText( GetDlgItem( hwnd, EDT_CITY ), pProjPrefsData->City, sizeof(char)*(MAX_INFO+1) );

    // Save Users zip
    Edit_GetText( GetDlgItem( hwnd, EDT_ZIP ), pProjPrefsData->Zip, sizeof(char)*(SMALL_ARRAY_50+1) );

    // Save Users email address
    Edit_GetText( GetDlgItem( hwnd, EDT_EMAIL_ADDRESS ), pProjPrefsData->EmailAddress, sizeof(char)*(MAX_INFO+1) );

    // Save Users reg fee
    Edit_GetText( GetDlgItem( hwnd, EDT_REG_FEE ), TempStr, sizeof(char)*(MAX_REG_FEE+1) );
    pProjPrefsData->RegFee = atof( TempStr );

    // Save Users sales tax
    Edit_GetText( GetDlgItem( hwnd, EDT_SALES_TAX ), TempStr, sizeof(char)*(MAX_SALES_TAX+1) );
    pProjPrefsData->SalesTax = atof( TempStr );

    // Save the combo box position for their state
    pProjPrefsData->StateIndex = ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_STATE ) );

    #endif

}   // SaveProjectPrefs
             

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

void TPrefsDlg::OnDestroy( HWND hwnd ) 
{
#ifndef SHARE_THE_MEMORIES
    SetWindowLong( GetDlgItem( hwnd, EDT_REG_FEE ), GWL_WNDPROC, LONG( OldEditProcFee ) );
    SetWindowLong( GetDlgItem( hwnd, EDT_SALES_TAX ), GWL_WNDPROC, LONG( OldEditProcTaxes ) );
#endif

} // OnDestroy