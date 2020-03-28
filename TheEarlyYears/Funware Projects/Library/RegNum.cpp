
/************************************************************************
*
*    PRODUCT:         Registration Number
*
*    FILE NAME:       RegNumDlg.cpp
*
*    DESCRIPTION:     Registration Number Dialog Box
*
*    IMPLEMENTATION:  Dialog for retreving the registration number
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines, enums and structures
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "minTools.h"        // A misc group of useful functions
#include "RegNum.h"          // Header for this *.cpp file
#include "OrderOpt.h"        // Dialog the asks how you want to order

#include MAIN_APP_HEADER     // Header file for this project. This is needed for calling the main app's function ShowOrderFormDlg()


// Globals
PTRegNumDlg pRegNumDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        DisplayInfoTxtSetupDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to window you want centered
*              char *InfoWndCaption - Text entered for the window caption
*              char *InfoWndTitle   - Text entered for the window title  
*              char *InfoWndTxt - The body of text
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

BOOL DisplayRegNumDlg( HINSTANCE hInst, HWND hwnd, char *DlgCaption, char *ProgTitle,
                       char *ProgCost, char *TxtInfo, char *RegNum, char *CreditCardURL, BOOL ACenterToParent )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pRegNumDlg = new TRegNumDlg( hInst, hwnd );

    // Create the dialog box
    if( pRegNumDlg )
        Result = pRegNumDlg->CreateDialogWnd( DlgCaption, ProgTitle, ProgCost, TxtInfo, 
                                              RegNum, CreditCardURL, ACenterToParent );

    return Result;

}   // DisplayRegNumDlg


/************************************************************************
*    FUNCTION NAME:         RegNum_SetupDlgProc
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

LRESULT CALLBACK RegNumDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pRegNumDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pRegNumDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pRegNumDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY,  pRegNumDlg->OnNCDestroy );
        default:
            return FALSE;
    }
}   // RegNum_SetupDlgProc


/************************************************************************
*
*                About Dialog Object Functions
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

TRegNumDlg::TRegNumDlg( HINSTANCE hInstance, HWND hwnd )
{
    // Init member functions
    hInst = hInstance;
    hParentWnd = hwnd;
    TitleFont = NULL;

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

TRegNumDlg::~TRegNumDlg()
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

BOOL TRegNumDlg::CreateDialogWnd( char *DlgCaption, char *ProgTitle, char *ProgCost, char *TxtInfo, char *RegNum, char *CreditCardURL, BOOL ACenterToParent )
{
    // init these variables
    pDlgCaption = DlgCaption;
    pTxtInfo = TxtInfo;
    pRegNum = RegNum;
    pProgTitle = ProgTitle;
    pProgCost = ProgCost;
    CenterToParent = ACenterToParent;
    pCreditCardURL = CreditCardURL;

    return DialogBox( hInst, "DLG_REG_NUMBER", hParentWnd, (DLGPROC)RegNumDlg_Proc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
*                           Load the file the user has typed for their
*                           info window.
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

BOOL TRegNumDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    RECT rect;

    // Center the window to the parent window
    if( CenterToParent )
    {
        GetWindowRect( hwnd, &rect );
        CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );
    }

    // Set the text limitfor the registration edit box
    Edit_LimitText( GetDlgItem( hwnd, EDT_REG_NUMBER ), MAX_REG_NUM );

    // Set the caption of the registration window
    if( pDlgCaption )
        SetWindowText( hwnd, pDlgCaption );

    // Set the title of the registration window
    if( pProgTitle )
        Static_SetText( GetDlgItem( hwnd, TXT_TITLE ), pProgTitle );

    // Set the info of the registration window
    if( pProgCost )
        Static_SetText( GetDlgItem( hwnd, TXT_PROGRAM_COST ), pProgCost );

    // Set the cost of the registration window
    if( pTxtInfo )
        Static_SetText( GetDlgItem( hwnd, TXT_ORDERING_INFO ), pTxtInfo );

    // Set a thicker font for the Registration number text
    // Remember, stock objects don't need to be freed
    SetWindowFont( GetDlgItem( hwnd, TXT_REG_NUM ), GetStockFont( SYSTEM_FONT ), FALSE );

    // Make a special font for the title of the helpful hint window
    HDC hDC = GetDC( GetDlgItem( hwnd, TXT_TITLE ) );
    TitleFont = MakeFont( hDC, "MS Sans Serif", 18, FONT_BOLD, RGB( 0, 0, 0 ) );
    ReleaseDC( GetDlgItem( hwnd, TXT_TITLE ), hDC );

    // Set the font
    if( TitleFont )
        SetWindowFont( GetDlgItem( hwnd, TXT_TITLE ), TitleFont, FALSE );

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

void TRegNumDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    BOOL Result;

    switch( id )
    {
        case BTN_CANCEL:
        case DLG_QUIT_BOX:            
            // The FALSE is the return parameter        
            EndDialog( hwnd, FALSE );
        break;

        case BTN_OK:
            // Check the registration number
            Result = CheckRegNum( hwnd );
        
            // The TRUE is the return parameter
            EndDialog( hwnd, Result );        
        break;

        case BTN_ORDER_FORM:
            // The reg dlg stays open when they click on the order form button
            if( pCreditCardURL[0] )
            {
                switch( DisplayOrderOptionDlg( hInst, hwnd, CENTER_WND_TO_PARENT ) )
                {
                    case ONLINE_WEB_ORDER:
                        ShellExecute( NULL, "open", pCreditCardURL, NULL, NULL, SW_SHOWNORMAL );
                    break;

                    case STANDARD_US_MAIL_ORDER:
                        ShowOrderFormDlg();
                    break;
                }
            }
            else
                ShowOrderFormDlg();
        break;
    }

}   // OnCommand


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

void TRegNumDlg::OnDestroy( HWND hwnd ) 
{
    // Delete the font we created just for this window
    if( TitleFont )
        DeleteFont( TitleFont );

} // OnDestroy



/************************************************************************
*    FUNCTION NAME:         CheckRegNum                                                             
*
*    DESCRIPTION:           Check the registration number they keyed in
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

BOOL TRegNumDlg::CheckRegNum( HWND hwnd )
{
    char UserRegNum[MAX_REG_NUM+1];
    char StrTmp[LARGE_ARRAY_500];
    BOOL Result = FALSE;

    // Did the user enter in a registration number
    if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_REG_NUMBER ) ) > 0 &&
        strlen( pRegNum ) > 0 )
    {
        // get the entered registration number
        Edit_GetText( GetDlgItem( hwnd, EDT_REG_NUMBER ), UserRegNum, sizeof( char )*(MAX_REG_NUM+1) );

        // If it equils zero, we have a match
        if( strcmp( UserRegNum, pRegNum ) == 0 )
        {
            // Load the info string from the resource file
            LoadString( hInst, REG_NUM_THANK_YOU, StrTmp, sizeof(char)*LARGE_ARRAY_500 );
            PostMsg( "Registration Complete", StrTmp );

            Result = TRUE;
        }
        else
        {
            // Load the info string from the resource file
            LoadString( hInst, REG_NUM_ERROR, StrTmp, sizeof(char)*LARGE_ARRAY_500 );
            PostMsg( "Error", StrTmp );
        }
    }

    return Result;

}   // CheckRegNum