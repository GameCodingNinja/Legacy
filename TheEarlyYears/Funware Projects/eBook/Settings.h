
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       SSSettings.h
*
*    DESCRIPTION:     Saves screen saver settings to the ini file
*
*    IMPLEMENTATION:  This file is very screen saver specific. Add in the
*                     data types and access functions needed.
*
*                     I use ini files for all the programs I write and
*                     stay away from using the Registry. The Registry can
*                     get hosed and I would rather not be one of the
*                     suspects. Using ini files is fast and safe and if
*                     something were to go wrong because the users computer
*                     has low system resources, the worst that can happen
*                     is that the ini files get hit and the screen saver
*                     doesn't work where as the whole compter is hosed
*                     nothing works. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/



#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "GDefines.h"        // Header file with #defines
#include "BaseSet.h"         // Base class header file.
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.


#define MAX_CUSTOM_COLORS  16

class TSettings : public TBaseSettings
{
protected:

    // Add your variables here for screen saver specific programs
    char WinDir[MAX_PATH];
    int  FWindowState, FWindowX, FWindowY, FWindowW, FWindowH, ZoomIndex;
	BOOL showInstallInfo;

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
    // For the order form information
    char Name[MAX_OF_NAME+1];
    char Address1[MAX_OF_ADDRESS+1];
    char Address2[MAX_OF_ADDRESS+1];
    char City[MAX_OF_CITY+1];
    char Zip[MAX_OF_ZIP+1];
    char Email[MAX_OF_EMAIL+1];
    int StateIndex;
    BOOL IncludeFloppy;
    BOOL FOrderFrmHelpWnd;
    BOOL FIntroHelpWnd;
    
    // Order Form settings
    // These are not saved to the ini file
    char ProgramTitle[MAX_TITLE+1], CreditCardInfo[300];
    char CompanyName[MAX_INFO+1];
    char CoAddressLine1[MAX_INFO+1];
    char CoAddressLine2[MAX_INFO+1];
    char CoCity[MAX_INFO+1];
    char CoZip[SMALL_ARRAY_50+1];
    char CoWebAddress[MAX_INFO+1];
    char CoEmailAddress[MAX_INFO+1];
    char OrderInfo[200], FloppyInfo[100];
    BOOL OfferFloppy, NoPrinterAvailable;
    BOOL Visa, MasterCard, Discover, AmericanExpress;
    double ProductCost, SalesTax, FloppyCost;
    char OFInfoCap[MAX_INFO+1];       // Users text for the Order Form info caption
    char OFInfoTitle[MAX_INFO+1];     // Users text for the Order Form info Title
    char *OFInfoBufferPtr, *pThankYouNoteBuffer;
    DWORD OFInfoBudderSize, ThankYouNoteBufferSize;
    BOOL OFInfoCheckSum, PrintLogo;
    WORD CheckStateIndex;

    // Other
    char RegNum[100];
	COLORREF CustomColors[ MAX_CUSTOM_COLORS ];
    #endif


    #ifdef EBOOK_TOOL
    char ProjectPath[ MAX_STR_PATH  ];
    char ProjectArtPath[ MAX_PATH  ];
    char ProjectSoundPath[ MAX_PATH  ];
    #else
        #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
            // View program Font selection
            LOGFONT lf;
            COLORREF FontColor;
            COLORREF BackgrndTextColor;
            DWORD LastPage;
        #endif
        BOOL SizeWindowToFit;
        BOOL PlaySoundPageTurn;
    #endif

public:

    void GetPrivateString();
    void WritePrivateString();

    void WriteSecretIniFile( char *FileName, char *Section );
    void GetSecretIniFile( char *FileName, char *Section );

    // Add your programr specific access functions here
    int GetWindowState(){ return FWindowState; };
    void SetWindowState( int AWindowState ){ FWindowState = AWindowState; };
    int GetWindowX(){ return FWindowX; };
    void SetWindowX( int WindowX ){ FWindowX = WindowX; };
    int GetWindowY(){ return FWindowY; };
    void SetWindowY( int WindowY ){ FWindowY = WindowY; };
    int GetWindowW(){ return FWindowW; };
    void SetWindowW( int WindowW ){ FWindowW = WindowW; };
    int GetWindowH(){ return FWindowH; };
    void SetWindowH( int WindowH ){ FWindowH = WindowH; };

	BOOL GetShowInstallInfo(){ return showInstallInfo; };
    void SetShowInstallInfo( BOOL value ){ showInstallInfo = value; };

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
    // Order form save customer information to ini file
    char *GetName(){ return Name; };
    void SetName( char *text ){ lstrcpy( Name, text ); };
    char *GetAddress1(){ return Address1; };
    void SetAddress1( char *text ){ lstrcpy( Address1, text ); };
    char *GetAddress2(){ return Address2; };
    void SetAddress2( char *text ){ lstrcpy( Address2, text ); };
    char *GetCity(){ return City; };
    void SetCity( char *text ){ lstrcpy( City, text ); };
    int GetStateIndex(){ return StateIndex; };
    void SetStateIndex( int Index ){ StateIndex = Index; };
    char *GetZip(){ return Zip; };
    void SetZip( char *text ){ lstrcpy( Zip, text ); };
    char *GetEmail(){ return Email; };
    void SetEmail( char *text ){ lstrcpy( Email, text ); };
    BOOL GetIncludeFloppy(){ return IncludeFloppy; };
    void SetIncludeFloppy( BOOL Floppy ){ IncludeFloppy = Floppy; };
    BOOL GetIntroHelpWnd(){ return FIntroHelpWnd; };
    void SetIntroHelpWnd( BOOL IntroHelpWnd ){ FIntroHelpWnd = IntroHelpWnd; };
    BOOL GetOrderFrmHelpWnd(){ return FOrderFrmHelpWnd; };
    void SetOrderFrmHelpWnd( BOOL OrderFrmHelpWnd ){ FOrderFrmHelpWnd = OrderFrmHelpWnd; };

    // Other
    char *GetRegNum(){ return RegNum; };
    void SetRegNum( char *text ){ lstrcpy( RegNum, text ); };
    // Get the pointer to the custom colors
    COLORREF *GetPtrCustomColors(){ return CustomColors; };

    // Order Form settings
    // These are not saved to the ini file
    char *GetTitle(){ return ProgramTitle; };
    void SetTitle( char *text ){ lstrcpy( ProgramTitle, text ); };
    char *GetCompanyName(){ return CompanyName; };
    void SetCompanyName( char *text ){ lstrcpy( CompanyName, text ); };
    char *GetCoAddressLine1(){ return CoAddressLine1; };
    void SetCoAddressLine1( char *text ){ lstrcpy( CoAddressLine1, text ); };
    char *GetCoAddressLine2(){ return CoAddressLine2; };
    void SetCoAddressLine2( char *text ){ lstrcpy( CoAddressLine2, text ); };
    char *GetCoCity(){ return CoCity; };
    void SetCoCity( char *text ){ lstrcpy( CoCity, text ); };
    char *GetCoZip(){ return CoZip; };
    void SetCoZip( char *text ){ lstrcpy( CoZip, text ); };
    char *GetCoWebAddress(){ return CoWebAddress; };
    void SetCoWebAddress( char *text ){ lstrcpy( CoWebAddress, text ); };
    char *GetCoEmailAddress(){ return CoEmailAddress; };
    void SetCoEmailAddress( char *text ){ lstrcpy( CoEmailAddress, text ); };

    char *GetOrderInfo(){ return OrderInfo; };
    void SetOrderInfo( char *text ){ lstrcpy( OrderInfo, text ); };
    char *GetFloppyInfo(){ return FloppyInfo; };
    void SetFloppyInfo( char *text ){ lstrcpy( FloppyInfo, text ); };
    BOOL GetOfferFloppy(){ return OfferFloppy; };
    void SetOfferFloppy( BOOL Floppy ){ OfferFloppy = Floppy; };
    double GetFloppyCost(){ return FloppyCost; };
    void SetFloppyCost( double Cost ){ FloppyCost = Cost; };
    double GetProductCost(){ return ProductCost; };
    void SetProductCost( double Cost ){ ProductCost = Cost; };
    double GetSalesTax(){ return SalesTax; };
    void SetSalesTax( double Tax ){ SalesTax = Tax; };
    BOOL GetNoPrinterAvailable(){ return NoPrinterAvailable; };
    void SetNoPrinterAvailable( BOOL Allow ){ NoPrinterAvailable = Allow; };
    
    void SetCreditCardInfo( char *text ){ lstrcpy( CreditCardInfo, text ); };
    char *GetCreditCardInfo(){ return CreditCardInfo; };
    void SetVisa( BOOL Card ){ Visa = Card; };
    BOOL GetVisa(){ return Visa; };
    void SetMasterCard( BOOL Card ){ MasterCard = Card; };
    BOOL GetMasterCard(){ return MasterCard; };
    void SetDiscover( BOOL Card ){ Discover = Card; };
    BOOL GetDiscover(){ return Discover; };
    void SetAmericanExpress( BOOL Card ){ AmericanExpress = Card; };
    BOOL GetAmericanExpress(){ return AmericanExpress; };

    char *GetOFInfoCaption(){ return OFInfoCap; };
    void SetOFInfoCaption( char *text ){ lstrcpy( OFInfoCap, text ); };
    char *GetOFInfoTitle(){ return OFInfoTitle; };
    void SetOFInfoTitle( char *text ){ lstrcpy( OFInfoTitle, text ); };
    char *GetOFInfoBufferPtr(){ return OFInfoBufferPtr; };
    void SetOFInfoBufferPtr( char *text ){ OFInfoBufferPtr = text; };
    DWORD GetOFInfoBufferSize(){ return OFInfoBudderSize; };
    void SetOFInfoBufferSize( DWORD Size ){ OFInfoBudderSize = Size; }; 
    BOOL GetOFInfoBufferCheckSum(){ return OFInfoCheckSum; };
    void SetOFInfoBufferCheckSum( BOOL CheckSum ){ OFInfoCheckSum = CheckSum; };
    WORD GetCheckStateIndex(){ return CheckStateIndex; };
    void SetCheckStateIndex( WORD Index ){ CheckStateIndex = Index; };
    void SetPrintLogo( BOOL Print ){ PrintLogo = Print; };
    BOOL GetPrintLogo(){ return PrintLogo; };
    void SetThankYouNoteBuffer( char *text ){ pThankYouNoteBuffer = text; };
    char *GetThankYouNoteBuffer(){ return pThankYouNoteBuffer; };
    DWORD GetThankYouNoteBufferSize(){ return ThankYouNoteBufferSize; };
    void SetThankYouNoteBufferSize( DWORD Size ){ ThankYouNoteBufferSize = Size; };
    
    #endif  

    TSettings();
    ~TSettings(){};

    #ifdef EBOOK_TOOL

    // Path to project file
    char *GetProjectPath(){ return ProjectPath; };
    void SetProjectPath( char *text ){ lstrcpy( ProjectPath, text ); };

    // Path to project art files
    char *GetProjectArtPath(){ return ProjectArtPath; };

    // Path to project sound files
    char *GetProjectSoundPath(){ return ProjectSoundPath; };

    #else

        #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
            // View program Font selection
            char HasDefFontBeenChanged(){ return lf.lfFaceName[0]; };
            LOGFONT * GetLogFont(){ return &lf; };
            void SetLogFont( LOGFONT * pLogFont ){ memcpy( &lf, pLogFont, sizeof(LOGFONT) ); };

            // Font color set and get
            COLORREF GetFontColor(){ return FontColor; };
            void SetFontColor( COLORREF Color ){ FontColor = Color; };

            // Background color set and get
            COLORREF GetBackgrndTextColor(){ return BackgrndTextColor; };
            void SetBackgrndTextColor( COLORREF Value ){ BackgrndTextColor = Value; };

            // Set the last page viewed
            void SetLastPage( DWORD Value ){ LastPage = Value; };
            DWORD GetLastPage(){ return LastPage; };
        #endif

    // ShareTheMemories doesn't save the zoom index
    int GetZoomIndex(){ return ZoomIndex; };
    void SetZoomIndex( int Value ){ ZoomIndex = Value; };

    BOOL GetPlaySoundPageTurn(){ return PlaySoundPageTurn; };
    void SetPlaySoundPageTurn( BOOL Value ){ PlaySoundPageTurn = Value; };

    BOOL GetSizeWndToFit(){ return SizeWindowToFit; };
    void SetSizeWndToFit( BOOL Value ){ SizeWindowToFit = Value; };

    #endif

};

typedef TSettings *PTSettings;


#endif  // __SETTINGS__