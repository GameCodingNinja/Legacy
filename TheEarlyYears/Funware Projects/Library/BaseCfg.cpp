/************************************************************************
*
*    PRODUCT:         Screen saver config dialog box
*
*    FILE NAME:       BaseCfg.cpp
*
*    DESCRIPTION:     Base class for the config dialog box
*
*    IMPLEMENTATION:  Functions and features of all config dialog boxes
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

#include "BaseCfg.h"         // The *.cpp's header file
#include "minTools.h"        // A misc group of useful functions
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "HelpHint.h"        // Helpful hint window. The text is loaded from resource or a buffer
#include "OrderOpt.h"        // Dialog the asks how you want to order
#include "OrderFrm.h"        // Order form window

// Turn off warnings for deprecated calls
#pragma warning(disable : 4996)

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

TBaseConfigDlg::TBaseConfigDlg()
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

TBaseConfigDlg::~TBaseConfigDlg()
{
}   // Destructer


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

void TBaseConfigDlg::SetupOrderFormInfo()
{
    char StrTmp[MED_ARRAY_300];

    TLoadRes Res;

    // Set the tile
    sprintf( StrTmp, "%s    %s", PRODUCT_VERSION, MAIL_ORDER_ONLY );
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
    LoadTheResource( hInst, "orderFrmInfo", "TXT", &Res );
    if( Res.pData )
        Settings.SetOFInfoBufferPtr( (char *)Res.pData );

    Settings.SetOFInfoBufferSize( Res.Size );

}   // SetupOrderFormInfo


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

void TBaseConfigDlg::ShowOrderFormDlg( HWND hwnd )
{
    char *LetterTxt;
    TLoadRes LoadRes;

    // Load the text from the resource
    if( !LoadTheResource( hInst, "orderFrmTxt", "TXT", &LoadRes ) )
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

    // Take the thank you note and add in all the specific information
    sprintf( LetterTxt, (char *)LoadRes.pData, PROGRAM, TYPEOFPROGRAM2, 
             AOL_ADDRESS, WEB_ADDRESS, EMAIL_ADDRESS, FIRST_TEMP_NUMBER );

    // Copy over the buffer pointer
    Settings.SetThankYouNoteBuffer( LetterTxt );

    // Show the Order Form dialog box
    DisplayOrderFrmDialogBox( hInst, hwnd, &Settings, CENTER_WND_TO_PARENT );

    if( LetterTxt )
        delete [] LetterTxt;

}   // ShowOrderFormDlg


/************************************************************************
*    FUNCTION NAME:         CheckRegNumber                                                             
*
*    DESCRIPTION:           Check the registration number entered
*
*    FUNCTION PARAMETERS:
*    Input:    char *RegNum - Number entered in from the user
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

void TBaseConfigDlg::CheckRegNumber( char *RegNum )
{
    char FilePath[MAX_PATH];
    unsigned long bytesWritten, Count;
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    char MsgStr[500], *pTmpRegNo = RegNum;
    int TmpRegNumType = REG_NUM_ERROR, strLength, i;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Get the length of the string
    strLength = strlen( RegNum );

    // Convert the string to all upper case
    CharUpperBuff( RegNum, strLength );

    // Remove any front spaces
    for( i = 0; i < strLength; ++i )
    {
        if( RegNum[i] < '-' || RegNum[i] > 'Z' )
            ++pTmpRegNo;
        else
            break;
    }

    // Get the length of the string incase
    // we removed any characters
    strLength = strlen( pTmpRegNo );

    // Remove any back spaces
    for( i = strLength-1; i > 0; --i )
    {
        if( pTmpRegNo[i] < '-' || pTmpRegNo[i] > 'Z' )
            pTmpRegNo[i] = 0;
        else
            break;
    }

    // Check to see if this is the full registration number
    if( strcmp( FULL_VERSION_NUMBER, pTmpRegNo ) == 0 )
    {
        // Record the full version number
        Settings.SetRegNum( FULL_VERSION_NUMBER );

        // Thank them for registering
        if( LoadString( hInst, THANK_YOU_FOR_REG, MsgStr, sizeof( MsgStr ) ) )
            PostMsg( "Registration", MsgStr );

        // Get the windows directory and add the file name to it
        GetTheWindowsDirectory( FilePath );
        strcat( FilePath, FIRST_CHECK_FILE );

        // Delete any of the dummy files that may be in use
        DeleteFile( FilePath );

        // Get the windows directory and add the file name to it
        GetTheWindowsDirectory( FilePath );
        strcat( FilePath, SECOND_CHECK_FILE );

        // Delete the dummy files that may be in use
        DeleteFile( FilePath );

        return;
    }

    // Check to see if this is the first or the second temp registration number
    if( strcmp( FIRST_TEMP_NUMBER, pTmpRegNo ) == 0 )
        TmpRegNumType = FIRST_TMP_REG_NUM;
    else if( strcmp( SECOND_TEMP_NUMBER, pTmpRegNo ) == 0 )
        TmpRegNumType = SECOND_TMP_REG_NUM;

    // If they have entered in the temp number for the first time
    // create the secret hidden dummy file that counts down how many times it can run
    if( TmpRegNumType )
    {
        // Get the windows directory
        GetTheWindowsDirectory( FilePath );

        // Add the file name to the path
        if( TmpRegNumType == FIRST_TMP_REG_NUM )
            strcat( FilePath, FIRST_CHECK_FILE );
        else
            strcat( FilePath, SECOND_CHECK_FILE );

        // Create the file
        // The CREATE_NEW flag returns an INVALID_HANDLE_VALUE when the file allready exists
        hFile = CreateFile( FilePath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ,
                             &FileSec, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0 );

        if( hFile == INVALID_HANDLE_VALUE )
        {
            // Let them know they can only enter in the temp number once
            if( LoadString( hInst, TMP_NUM_ONLY_ONCE, MsgStr, sizeof( MsgStr ) ) )
                PostMsg( "Error", MsgStr );
        }
        else
        {
            // Write the starting value to the file
            Count = SHAREWAEW_TMP_RUN_MAX;
            WriteFile( hFile, &Count, sizeof( Count ), &bytesWritten, NULL );
            CloseHandle( hFile );

            if( TmpRegNumType == FIRST_TMP_REG_NUM )
            {
                // Let them know they entered in a temporary registration number
                if( LoadString( hInst, FIRST_TMP_NUM_ACT, MsgStr, sizeof( MsgStr ) ) )
                    PostMsg( "Temporary Registration", MsgStr );
            }
            else
            {
                // Let them know they entered in the last temporary registration number
                if( LoadString( hInst, SECOND_TMP_NUM_ACT, MsgStr, sizeof( MsgStr ) ) )
                    PostMsg( "Temporary Registration", MsgStr );
            }
        }
    }

    // Let the user know that the number entered wasn't recognized
    else
    {
        // Let them know they entered in an invalid registration number
        if( LoadString( hInst, REG_NUM_ERROR_MSG, MsgStr, sizeof( MsgStr ) ) )
            PostMsg( "Error", MsgStr );
    }

}   // CheckRegNumber