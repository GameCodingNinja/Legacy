/************************************************************************
*
*    PRODUCT:         Oder form dialog
*
*    FILE NAME:       OrderFrm.cpp
*
*    DESCRIPTION:     Order Form dialog box
*
*    IMPLEMENTATION:  Dialog box for users to order software
*                     After it is setup once, it shouldn't have to reworked
*                     unless making changes
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "minTools.h"        // A misc group of useful functions
#include "DecodGif.h"        // GIF decoder which can load GIFs from file or resource
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource
#include "Metafile.h"        // Class for loading and displaying Windows metafiles and enhanced metafiles
#include <stdio.h>           // Declares several commonly used routines such as atio, random, abs, malloc etc.

#include "OrderFrm.h"        // Header file for this *.cpp file.
#include "printcnl.h"        // Print Cancel dialog box
#include "dib.h"             // Dib and Bitmap class for displaying and animating bitmaps

#define TXT_LGNTH  150       // String length for the order form fields

// Globales
PTOrderFrmDlg pOrderFrmDlg;
char States[NUM_US_STATES+1][3] = { "NA",
                                  "AK","AL","AR","AZ","CA","CO","CT","DC","DE","FL",
                                  "GA","HI","IA","ID","IL","IN","KS","KY","LA","MA",
                                  "MD","ME","MI","MN","MO","MS","MT","NC","ND","NE",
                                  "NH","NJ","NM","NV","NY","OH","OK","0R","PA","RI",
                                  "SC","SD","TN","TX","UT","VT","VA","WA","WV","WI",
                                  "WY" };


// Turn off warnings for deprecated calls
#pragma warning(disable : 4996)

// Disable performance warning
#pragma warning(disable : 4800)

/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         DisplayOrderFrmDialogBox                                                             
*
*    DESCRIPTION:           Gets called by the base screen saver to
*                           display the dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*              HWND hwnd - Handle to parent window
*              PTSettings pSet - The setting object for recording user settings
*                                This is also used as a way to set other properties
*                                of the dialog box
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

BOOL DisplayOrderFrmDialogBox( HINSTANCE hInst, HWND hwnd, PTSettings pSet, BOOL ACenterToParent )
{
    BOOL Result = FALSE;

    // Create the object
    // It's best to not create the dialog box in the objects constructor
    pOrderFrmDlg = new TOrderFrmDlg();

    // Create the dialog box
    if( pOrderFrmDlg )
        Result = pOrderFrmDlg->CreateDialogWnd( hInst, hwnd, pSet, ACenterToParent );

    return Result;

}   // DisplayOrderFrmDialogBox


/************************************************************************
*    FUNCTION NAME:         OrderFrmDlgProc
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

LRESULT CALLBACK OrderFrmDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pOrderFrmDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND, pOrderFrmDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_PAINT, pOrderFrmDlg->OnPaint );
        HANDLE_MSG( hDlg, WM_SHOWWINDOW, pOrderFrmDlg->OnShowWindow );
        HANDLE_MSG( hDlg, WM_DESTROY, pOrderFrmDlg->OnDestroy );
        HANDLE_MSG( hDlg, WM_NCDESTROY, pOrderFrmDlg->OnNCDestroy );
        default:
            return FALSE;
    }

}   // OrderFrmDlgProc


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

TOrderFrmDlg::TOrderFrmDlg()
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

TOrderFrmDlg::~TOrderFrmDlg()
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

BOOL TOrderFrmDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd, PTSettings pSet, BOOL ACenterToParent )
{
    BOOL Result;

    // Get the pointer to the settings object
    // The order forms behavior is controlled though the settings
    pOrderSettings = pSet;
    hParentWnd = hwnd;
    CenterToParent = ACenterToParent;
    hInst = hInstance;

    // Create the dialog box
    Result = DialogBox( hInst, "DLG_ORDERFRM", hwnd, (DLGPROC)OrderFrmDlgProc );

    return Result;

}   // CreateDialogWnd



/************************************************************************
*    FUNCTION NAME:         OrderFrm_OnInitDialog()                                                             
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

BOOL TOrderFrmDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    char StrTmp[100];
    int i;
    RECT rect;
    // GIF decoder which can load GIFs from file or resource
    TGif gif;

    // Load the GIF graphic from resource
    OrderDib.LoadFromBuffer( gif.LoadFromResource( "orderForm", "GIF", hInst ) );

    // Center the Helpful Hint window to the parent window
    if( CenterToParent )
    {
        GetWindowRect( hwnd, &rect );
        CenterWindow( hwnd, rect.right - rect.left, rect.bottom - rect.top, hParentWnd );
    }

    // Set the window title
    Static_SetText( hwnd, pOrderSettings->GetTitle() );

    // Set the info title
    Static_SetText( GetDlgItem( hwnd, TXT_TITLE ), pOrderSettings->GetOrderInfo() );

    // Set the edit field limits
    Edit_LimitText( GetDlgItem( hwnd, EDT_NAME ), MAX_OF_NAME );
    Edit_LimitText( GetDlgItem( hwnd, EDT_ADDRESS_1 ), MAX_OF_ADDRESS );
    Edit_LimitText( GetDlgItem( hwnd, EDT_ADDRESS_2 ), MAX_OF_ADDRESS );
    Edit_LimitText( GetDlgItem( hwnd, EDT_CITY ), MAX_OF_CITY );
    Edit_LimitText( GetDlgItem( hwnd, EDT_EMAIL ), MAX_OF_EMAIL );

    // Init the edit fields with the information
    // Set the name
    Edit_SetText( GetDlgItem( hwnd, EDT_NAME ), pOrderSettings->GetName() );
    // Set the Address line 1
    Edit_SetText( GetDlgItem( hwnd, EDT_ADDRESS_1 ), pOrderSettings->GetAddress1() );
    // Set the Address line 2
    Edit_SetText( GetDlgItem( hwnd, EDT_ADDRESS_2 ), pOrderSettings->GetAddress2() );
    // Set the City   
    Edit_SetText( GetDlgItem( hwnd, EDT_CITY ), pOrderSettings->GetCity() );
    // Set the Zip   
    Edit_SetText( GetDlgItem( hwnd, EDT_ZIP ), pOrderSettings->GetZip() );
    // Set the email
    Edit_SetText( GetDlgItem( hwnd, EDT_EMAIL ), pOrderSettings->GetEmail() );
    // Set the floppy
    if( pOrderSettings->GetIncludeFloppy() )
        Button_SetCheck( GetDlgItem( hwnd, CHK_FLOPPY_DISK ), BST_CHECKED );
        
    // Set a thicker font for the information text
    SetWindowFont( GetDlgItem( hwnd, TXT_TITLE ), GetStockFont( SYSTEM_FONT ), FALSE );

    // Add the state strings to the combo box
    for( i = 0; i < NUM_US_STATES+1; ++i )
        ComboBox_AddString( GetDlgItem( hwnd, CBO_STATE ), States[i] );

    // Set the combo box position
    ComboBox_SetCurSel( GetDlgItem( hwnd, CBO_STATE ), pOrderSettings->GetStateIndex() );
    
    // Turn off the credit cards if they are not needed
    if( !pOrderSettings->GetVisa() )
        Button_Enable( GetDlgItem( hwnd, RDOBTN_VISA ), DISABLE );
        
    if( !pOrderSettings->GetMasterCard() )
        Button_Enable( GetDlgItem( hwnd, RDOBTN_MASTERCARD ), DISABLE );   

    if( !pOrderSettings->GetDiscover() )
        Button_Enable( GetDlgItem( hwnd, RDOBTN_DISCOVER ), DISABLE );

    if( !pOrderSettings->GetAmericanExpress() )
        Button_Enable( GetDlgItem( hwnd, RDOBTN_AMER_EXP ), DISABLE );

    // Disable the text that talks about the credit card if no credit cards are to be taken
    // as well as the edit field for entering in credit card numbers
    if( !pOrderSettings->GetVisa() &&
        !pOrderSettings->GetMasterCard() &&
        !pOrderSettings->GetDiscover() &&
        !pOrderSettings->GetAmericanExpress() )
    {
        Static_Enable( GetDlgItem( hwnd, TXT_CREDIT_CARD_EXP ), DISABLE );
        Static_Enable( GetDlgItem( hwnd, TXT_CREDIT_CARD_NO ), DISABLE );
        Static_Enable( GetDlgItem( hwnd, TXT_CREDIT_CARD_INFO ), DISABLE );
        Static_Enable( GetDlgItem( hwnd, EDT_CREDIT_CARD_NO ), DISABLE );
        Static_Enable( GetDlgItem( hwnd, EDT_EXP_DATE ), DISABLE );
    }

    // Set the Credit Card Info
    if( pOrderSettings->GetCreditCardInfo() )
    {
        Static_SetText( GetDlgItem( hwnd, TXT_CREDIT_CARD_INFO ), pOrderSettings->GetCreditCardInfo() );
        ShowWindow( GetDlgItem( hwnd, TXT_CREDIT_CARD_INFO ), SW_SHOWNORMAL );
    }

    // Set Floppy cost info
    if( pOrderSettings->GetOfferFloppy() && pOrderSettings->GetFloppyInfo() )
    {
        sprintf( StrTmp, pOrderSettings->GetFloppyInfo(), pOrderSettings->GetFloppyCost() );
        Button_SetText( GetDlgItem( hwnd, CHK_FLOPPY_DISK ), StrTmp );
        ShowWindow( GetDlgItem( hwnd, CHK_FLOPPY_DISK ), SW_SHOWNORMAL );
    }
    else
    {
        ShowWindow( GetDlgItem( hwnd, TXT_SHIP_HAND_STATIC ), SW_HIDE );
        ShowWindow( GetDlgItem( hwnd, TXT_SHIP_HAND_COST ), SW_HIDE );
    }

    // Do we want to allow the use of the "No Printer Available" button.
    // If not, hide the button
    if( !pOrderSettings->GetNoPrinterAvailable() )
        ShowWindow( GetDlgItem( hwnd, BTN_NO_PRINTER ), SW_HIDE );

    // Calculate the amount of money they owe
    FormatTotalCost( hwnd );

    return TRUE;

}   // OnInitDialog


/************************************************************************
*    FUNCTION NAME:         SaveUserSettings                                                             
*
*    DESCRIPTION:           Save the users settings
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle of the window receiving the message
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

void TOrderFrmDlg::SaveUserSettings( HWND hwnd )
{
    char tmpText[TXT_LGNTH];

    // Save the name
    Edit_GetText( GetDlgItem( hwnd, EDT_NAME ), tmpText, TXT_LGNTH );
    pOrderSettings->SetName( tmpText );

    // Save the Address line 1
    Edit_GetText( GetDlgItem( hwnd, EDT_ADDRESS_1 ), tmpText, TXT_LGNTH );
    pOrderSettings->SetAddress1( tmpText );

    // Save the Address lin 2
    Edit_GetText( GetDlgItem( hwnd, EDT_ADDRESS_2 ), tmpText, TXT_LGNTH );
    pOrderSettings->SetAddress2( tmpText );

    // Save the city
    Edit_GetText( GetDlgItem( hwnd, EDT_CITY ), tmpText, TXT_LGNTH );
    pOrderSettings->SetCity( tmpText );

    // Save the state index of the selected state
    pOrderSettings->SetStateIndex( ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_STATE ) ) );

    // Save the zip
    Edit_GetText( GetDlgItem( hwnd, EDT_ZIP ), tmpText, TXT_LGNTH );
    pOrderSettings->SetZip( tmpText );

    // Save the e-mail address
    Edit_GetText( GetDlgItem( hwnd, EDT_EMAIL ), tmpText, TXT_LGNTH );
    pOrderSettings->SetEmail( tmpText );

    // Save the floppy disk selection
    if( Button_GetCheck( GetDlgItem( hwnd, CHK_FLOPPY_DISK ) ) == BST_CHECKED )
        pOrderSettings->SetIncludeFloppy( TRUE );
    else
        pOrderSettings->SetIncludeFloppy( FALSE );

    // Write the settings to the hard disk
    pOrderSettings->WritePrivateString();

}   // OrderFrm_SaveUserSettings


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

void TOrderFrmDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch(id)
    {
        case BTN_PRINT_ORDERFORM:
            // print it
            PrintOrderForm( hwnd );  
        break;

        case BTN_DONE:
        case DLG_QUIT_BOX:
            // Save our settings
            SaveUserSettings( hwnd );

            // Kill this dialog
            EndDialog( hwnd, 0 );
        break;

        case BTN_NO_PRINTER:
            // Show the "No Printer" dialog box
            ShowNoPrinterDlg( hwnd );
        break;

        case CHK_FLOPPY_DISK:
        case CBO_STATE:
            FormatTotalCost( hwnd );
        break;

        case SHOW_HELPHINT_WND:
            // Show the helpful hint window
            if( pOrderSettings->GetOrderFrmHelpWnd() )
            {
                Sleep( 400 );
                BOOL ShowNextTime;
                DisplayHelpHintDialogBox( hInst, hwnd, NULL, NULL, pOrderSettings->GetOFInfoCaption(), 
                                          pOrderSettings->GetOFInfoTitle(), &ShowNextTime, pOrderSettings->GetOFInfoBufferCheckSum(), 
                                          pOrderSettings->GetOFInfoBufferPtr(), pOrderSettings->GetOFInfoBufferSize(), CenterToParent );
                pOrderSettings->SetOrderFrmHelpWnd( ShowNextTime );
            }
        break;
    }
}   // OnCommand



/************************************************************************
*    FUNCTION NAME:         ShowNoPrinterDlg)                                                             
*
*    DESCRIPTION:           Display the no printer dialog box    
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

void TOrderFrmDlg::ShowNoPrinterDlg( HWND hwnd )
{
    char *pStrBuffer, *pStrTmp;
    TLoadRes LoadRes;
    WORD StateIndex;

    // Load the text from the resource
    if( !LoadTheResource( hInst, "noprinter", "TXT", &LoadRes ) )
        return;

    // New up an array to hold the letter that will be printed out.
    // Make the array larger so that it can hold additional information to sprintf
    pStrBuffer = new char[LoadRes.Size+LARGE_ARRAY_500];

    // Get the businesses state index
    StateIndex = pOrderSettings->GetCheckStateIndex();

    // Take the thank you note and add in all the specific information
    // Are we using the second address line
    pStrTmp = pOrderSettings->GetCoAddressLine2();
    if( !pStrTmp[0] )
    {
        sprintf( pStrBuffer, (char *)LoadRes.pData, 
                 pOrderSettings->GetCompanyName(), pOrderSettings->GetCoAddressLine1(), " ", " ", 
                 pOrderSettings->GetCoCity(),
                 (StateIndex ? States[StateIndex] : " "),
                 pOrderSettings->GetCoZip(),
                 pOrderSettings->GetCoWebAddress(), pOrderSettings->GetCoEmailAddress() );
    }
    else
    {
        // Make a line return for the second address
        char cr[3];
        cr[0] = RETURN_KEY;
        cr[1] = LINEFEED_KEY;
        cr[2] = 0;

        sprintf( pStrBuffer, (char *)LoadRes.pData, 
                 pOrderSettings->GetCompanyName(), pOrderSettings->GetCoAddressLine1(), cr,
                 pOrderSettings->GetCoAddressLine2(), 
                 pOrderSettings->GetCoCity(),
                 (StateIndex ? States[StateIndex] : " "),
                 pOrderSettings->GetCoZip(),
                 pOrderSettings->GetCoWebAddress(), pOrderSettings->GetCoEmailAddress() );
    }

    // Show the "No Printer" Helpful Hint dialog box
    DisplayHelpHintDialogBox( hInst, hwnd, NULL, NULL, "Helpful Hint",
                              "No Printer Available", DONT_SHOW_CHECKBOX, DONT_DO_CHECK_SUM,
                              pStrBuffer, 0, CenterToParent );
}   // ShowNoPrinterDlg


/************************************************************************
*    FUNCTION NAME:         OnPaint                                                             
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

void TOrderFrmDlg::OnPaint( HWND hwnd )
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
    OrderDib.BlitToDC( PaintDC, r.right - OrderDib.width() - 15, 15 );

    // Must be called
    EndPaint( hwnd, &paintStruct );

}   // OnPaint




/************************************************************************
*    FUNCTION NAME:         PrintOrderForm                                                             
*
*    DESCRIPTION:           Print out the order form
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle of the window receiving the message
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

void TOrderFrmDlg::PrintOrderForm( HWND hwnd )
{   
    PTPrinter pPrinter;
    PTMetafile pMetafile;
    RECT rect;
    int TopMargin, StepIt;
    char DateStr[100];
    char CardNum[60], CardName[30], CardDate[15];
    char City[MAX_OF_CITY+1];
    char Zip[MAX_OF_ZIP+1], StrTmp[MED_ARRAY_300];
    TEXTMETRIC tm;
    HWND hPrintingWnd = NULL;

    // Allocate the printer and metafile object
    pPrinter = new TPrinter();
    pMetafile = new TMetafile();

    // Check all allocations
    if( !pPrinter || !pMetafile )
        goto FREE_MEMORY;

    // Create the printer DC and start art printing
    if( pPrinter->DisplayPrintDlg( 1, 1 ) && pPrinter->BeginDoc( "Print Orderform" ) )
    {
        // Show the printing... dialog window
        hPrintingWnd = DisplayPrintCancelDlg( hInst, hwnd, "Printing Order Form...", TRUE );

        pPrinter->SetFont( "Arial", 12 );
        pPrinter->BeginPage();

        // Print the logo
        // Push the logo to one side and make it a little less then 2.5 inches
        if( pOrderSettings->GetPrintLogo() )
        {
            // Load the metafile logo
            pMetafile->LoadFromResource( hInst, "funwareLogo", "WMF" );

            rect.top = pPrinter->GetpixelsPerInchY() / 4;
            rect.left = (int)(pPrinter->width()-(pPrinter->GetpixelsPerInchX() * 3.4));
            rect.bottom = (int)(rect.top + ( pPrinter->GetpixelsPerInchY() * 2.4 ));
            rect.right = (int)(rect.left + ( pPrinter->GetpixelsPerInchX() * 2.4 ));
            pPrinter->PrintMetafile( pMetafile, &rect );
        }

        // Format text to the page giving it a 1 inch margin all around 
        TopMargin = pPrinter->WrapText( pOrderSettings->GetThankYouNoteBuffer(), pPrinter->GetpixelsPerInchX(), 
                          pPrinter->GetpixelsPerInchY(), pPrinter->width()-(pPrinter->GetpixelsPerInchX()*2),
                          pPrinter->height()-(pPrinter->GetpixelsPerInchY()*2) );

        // Create a pen to draw a dotted line
        pPrinter->SetPen( PS_DASHDOT, 5, DEFAULT_BLACK );

        // Draw the dotted line
        pPrinter->Line( pPrinter->GetpixelsPerInchX(), TopMargin, 
                      pPrinter->width()-pPrinter->GetpixelsPerInchX(), TopMargin );

        // Setup step amount. This is what we'll increment each line to. Use GetTextMetrics
        // get information about the currently selected font.
        // If you need the leading to be a little more or less, use the Leading paramater
        if( GetTextMetrics( pPrinter->Handle(), &tm ) )
            StepIt = tm.tmHeight + (tm.tmExternalLeading * 5);
        else
            StepIt = 15; // Hard code a number just in case GetTextMetrics errors

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + StepIt + (tm.tmExternalLeading * 2), pPrinter );

        // Get the date in long form as a string to print on the order form
        GetDateFormat( LOCALE_USER_DEFAULT, DATE_SHORTDATE, NULL, NULL, DateStr, 100 );

        // Create the first line of text which is the name of the screen saver, the date and the version no
        sprintf( StrTmp, "%s      %s", pOrderSettings->GetTitle(), DateStr );

        // Print it Screen saver name and the date
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, StrTmp );

        // If floppy has been enabled, print the result
        if( pOrderSettings->GetOfferFloppy() )
        {
            // Move it down one
            // Check to see if we need to print out another page
            TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

            // Did they check if they wanted a floppy disk sent?
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_FLOPPY_DISK ) ) == BST_CHECKED )
                pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "3-1/2 Disk Requested." );
            else
                pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "Registration Number Through e-mail." );
        }

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + (StepIt * 2), pPrinter );

        // Print the name
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "Name:" );
        Edit_GetText( GetDlgItem( hwnd, EDT_NAME ), StrTmp, sizeof( char )*MED_ARRAY_300 );
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, StrTmp );

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

        // Print Address Line 1
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "Address:" );
        Edit_GetText( GetDlgItem( hwnd, EDT_ADDRESS_1 ), StrTmp, sizeof( char )*MED_ARRAY_300 );
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, StrTmp );

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

        // Print Address Line 2
        Edit_GetText( GetDlgItem( hwnd, EDT_ADDRESS_2 ), StrTmp, sizeof( char )*MED_ARRAY_300 );
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, StrTmp );

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

        // Print City State Zip
        City[0] = 0;
        Zip[0]  = 0;
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "City/State/Zip:" );
        if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_ZIP ) ) )
            Edit_GetText( GetDlgItem( hwnd, EDT_ZIP ), Zip, sizeof(char)*(MAX_OF_ZIP+1) );

        // Don't print any of this if they didn't enter in a city
        if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_CITY ) ) )
        {
            Edit_GetText( GetDlgItem( hwnd, EDT_CITY ), City, sizeof(char)*(MAX_OF_CITY+1) );
            sprintf( StrTmp, "%s, %s  %s", City, States[ ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_STATE ) ) ], Zip );        
            pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, StrTmp );
        }

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

        // Print E-mail
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "E-mail:" );
        Edit_GetText( GetDlgItem( hwnd, EDT_EMAIL ), StrTmp, sizeof( char )*MED_ARRAY_300 );
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, StrTmp );

        // Print Credit card info
        if( (Button_GetCheck( GetDlgItem( hwnd, RDOBTN_VISA ) ) ||
            Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MASTERCARD ) ) ||
            Button_GetCheck( GetDlgItem( hwnd, RDOBTN_DISCOVER ) ) ||
            Button_GetCheck( GetDlgItem( hwnd, RDOBTN_AMER_EXP ) )) &&
            Edit_GetTextLength( GetDlgItem( hwnd, EDT_CREDIT_CARD_NO ) ) &&
            Edit_GetTextLength( GetDlgItem( hwnd, EDT_EXP_DATE ) ) )
        {
            // Move it down one
            // Check to see if we need to print out another page
            TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

            // Get the name of the credit card chosen
            if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_VISA ) ) )
                Button_GetText( GetDlgItem( hwnd, RDOBTN_VISA ), CardName, sizeof(CardName) );
                    
            else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_MASTERCARD ) ) )
                Button_GetText( GetDlgItem( hwnd, RDOBTN_MASTERCARD ), CardName, sizeof(CardName) );

            else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_DISCOVER ) ) )
                Button_GetText( GetDlgItem( hwnd, RDOBTN_DISCOVER ), CardName, sizeof(CardName) );

            else if( Button_GetCheck( GetDlgItem( hwnd, RDOBTN_AMER_EXP ) ) )
                Button_GetText( GetDlgItem( hwnd, RDOBTN_AMER_EXP ), CardName, sizeof(CardName) );

            // Print the type of information
            pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "%s:", CardName );

            // Get the card number and exp date
            Edit_GetText( GetDlgItem( hwnd, EDT_CREDIT_CARD_NO ), CardNum, sizeof(CardNum) );
            Edit_GetText( GetDlgItem( hwnd, EDT_EXP_DATE ), CardDate, sizeof(CardDate) );

            // Print it all out
            pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, "%s       %s", CardNum, CardDate );

            // Move it down one
            // Check to see if we need to print out another page
            TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );
        }
        else
        {
            // Move it down two
            // Check to see if we need to print out another page
            TopMargin = CheckPrinterTopMargin( TopMargin + (StepIt * 2), pPrinter );
        }

        // Print Total money due
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "Total Cost:" );
        Static_GetText( GetDlgItem( hwnd, TXT_TOTAL_COST ), StrTmp, sizeof( char )*MED_ARRAY_300 );
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX()*3, TopMargin, StrTmp );

        // Move it down one
        // Check to see if we need to print out another page
        TopMargin = CheckPrinterTopMargin( TopMargin + StepIt, pPrinter );

        // Print Total money due
        pPrinter->PrintText( pPrinter->GetpixelsPerInchX(), TopMargin, "Please check to make sure all the information is correct." );

        pPrinter->StopPage();
        pPrinter->StopDoc();
    }

FREE_MEMORY:;

    if( pPrinter )
        delete pPrinter;

    if( pMetafile )
        delete pMetafile;

    // Destroy the printing... dialog window
    if( hPrintingWnd )
        DestroyWindow( hPrintingWnd );

}   // PrintOrderForm



/************************************************************************
*    FUNCTION NAME:         CheckPrinterTopMargin                                                             
*
*    DESCRIPTION:           A quick way to check each line to see if we
*                           need to print out another page
*
*    FUNCTION PARAMETERS:
*    Input:    int TopMargin     - The current top margin
*              PTPrinter printer - Pointer to the printer object
*
*    Output:   int - the new margin
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TOrderFrmDlg::CheckPrinterTopMargin( int TopMargin, PTPrinter printer )
{
    int Result = TopMargin;

    // If the top margin is past the height of the printer
    // print out the page and start a new one
    if( TopMargin >= printer->height() )
    {
        printer->StopPage();
        printer->BeginPage();
        Result = printer->GetpixelsPerInchY();
    }

    return Result;

}   // CheckPrinterTopMargin



/************************************************************************
*    FUNCTION NAME:         FormatTotalCost                                                             
*
*    DESCRIPTION:           Formats the total cost of the software
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd   - Handle of the window receiving the message
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

void TOrderFrmDlg::FormatTotalCost( HWND hwnd )
{
    char TotalCostStr[30], StrTmp[20];
    double SalesTax, Cost = pOrderSettings->GetProductCost();

    // Add in the cost of the floppy
    if( Button_GetCheck( GetDlgItem( hwnd, CHK_FLOPPY_DISK ) ) == BST_CHECKED )
    {
        Cost += pOrderSettings->GetFloppyCost();
        sprintf( StrTmp, "$%.2lf", pOrderSettings->GetFloppyCost() );
        Static_Enable( GetDlgItem( hwnd, TXT_SHIP_HAND_STATIC ), ENABLE );
        Static_Enable( GetDlgItem( hwnd, TXT_SHIP_HAND_COST ), ENABLE );
        Static_SetText( GetDlgItem( hwnd, TXT_SHIP_HAND_COST ), StrTmp );
    }
    else
    {
        Static_Enable( GetDlgItem( hwnd, TXT_SHIP_HAND_STATIC ), DISABLE );
        Static_Enable( GetDlgItem( hwnd, TXT_SHIP_HAND_COST ), DISABLE );
        Static_SetText( GetDlgItem( hwnd, TXT_SHIP_HAND_COST ), "$0.00" );
    }

    // Add in the cost of sales tax
    if( pOrderSettings->GetCheckStateIndex() == ComboBox_GetCurSel( GetDlgItem( hwnd, CBO_STATE ) ) )
    {
        // Calculate the sales tax
        SalesTax = Cost * pOrderSettings->GetSalesTax();
        // Calculate the total cost
        Cost += SalesTax;
        sprintf( StrTmp, "$%.2lf", SalesTax );
        Static_Enable( GetDlgItem( hwnd, TXT_IL_RESIDENT ), ENABLE );
        Static_Enable( GetDlgItem( hwnd, TXT_STATE_TAX ), ENABLE );
        Static_SetText( GetDlgItem( hwnd, TXT_IL_RESIDENT ), StrTmp );
    }
    else
    {
        Static_Enable( GetDlgItem( hwnd, TXT_IL_RESIDENT ), DISABLE );
        Static_SetText( GetDlgItem( hwnd, TXT_IL_RESIDENT ), "$0.00" );
        Static_Enable( GetDlgItem( hwnd, TXT_STATE_TAX ), DISABLE );
    }

    // Format the cost string
    sprintf( TotalCostStr, "$%.2lf", Cost );

    // Display the total
    Static_SetText( GetDlgItem( hwnd, TXT_TOTAL_COST ), TotalCostStr );

}   // FormatTotalCost


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

void TOrderFrmDlg::OnShowWindow( HWND hwnd, BOOL fShow, UINT status )
{
    // Post a message to show the Helpful hint window
    // This way it doesn't show up before the config window does
    if( pOrderSettings->GetOFInfoBufferPtr() )
        PostMessage( hwnd, ((UINT)WM_COMMAND), ((WPARAM)MAKELONG( SHOW_HELPHINT_WND, 0 )), ((LPARAM)0) );

}   // OnShowWindow



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

void TOrderFrmDlg::OnDestroy( HWND hwnd ) 
{
    OrderDib.Free();

} // OnDestroy