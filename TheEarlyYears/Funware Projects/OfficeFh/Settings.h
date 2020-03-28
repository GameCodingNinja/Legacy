
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       Settings.h
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



#ifndef __SSSETTINGS_H__
#define __SSSETTINGS_H__

#include "BaseSet.h"    // Base class header file.
#define ORDER_FORM   "Order Form"

#define MAX_CUSTOM_COLORS  16
#define BUSINESS_NAME_MAX  256

class TSettings : public TBaseSettings
{
protected:

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    int FishCount;
    BOOL BubblesOnOff;
    BOOL BackFishOnOff;
    COLORREF TxtColor;
    COLORREF CustomColors[ MAX_CUSTOM_COLORS ];
    LOGFONT lf;
    char BusinessName[BUSINESS_NAME_MAX+1];

    /******************************************
    *  Standard screen saver settings
    *****************************************/
    BOOL IntroHelpWnd;
    int StateIndex;
    BOOL MusicOff;
    BOOL ThemeMusicOn;
    BOOL LoadYourOwn;
    BOOL RandomMusic;
    char SoundDirectory[MAX_PATH];

    /******************************************
    *  Orderform settings
    *****************************************/
    char Name[100];
    char Address1[150];
    char Address2[150];
    char City[MAX_OF_CITY+1];
    char Zip[MAX_OF_ZIP+1];
    char Email[100];
    BOOL IncludeFloppy;
    BOOL IlTaxes;
    BOOL OrderFrmHelpWnd;

    // Other
    char RegNum[100];
    char WinDir[MAX_PATH];

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

public:

    /******************************************
    *  Screen saver specific settings
    *****************************************/

    // Fish count index
    int GetFishCount(){ return FishCount; };
    void SetFishCount( int Value ){ FishCount = Value; };

    // Bubble on off
    BOOL GetBubblesOnOff(){ return BubblesOnOff; };
    void SetBubblesOnOff( BOOL Value ){ BubblesOnOff = Value; };

    // Background fish on off
    BOOL GetBackFishOnOff(){ return BackFishOnOff; };
    void SetBackFishOnOff( BOOL Value ){ BackFishOnOff = Value; };

    // Get pointer to business name
    char *GetPtrBusinessName(){ return BusinessName; };

    // Get the selected color
    COLORREF GetTxtColor(){ return TxtColor; };
    void SetTxtColor( COLORREF Value ){ TxtColor = Value; };

    // Get the font information
    LOGFONT * GetLogFont(){ return &lf; };
    void SetLogFont( LOGFONT * pLogFont ){ memcpy( &lf, pLogFont, sizeof(LOGFONT) ); };

    // Get the pointer to the custom colors
    COLORREF *GetPtrCustomColors(){ return CustomColors; };


    /******************************************
    *  Standard screen saver settings
    *****************************************/

    // Standard settings list
    BOOL GetMusicOff(){ return MusicOff; };
    void SetMusicOff( BOOL Value ){ MusicOff = Value; };
    BOOL GetThemeMusicOn(){ return ThemeMusicOn; };
    void SetThemeMusicOn( BOOL Value ){ ThemeMusicOn = Value; };
    BOOL GetLoadYourOwn(){ return LoadYourOwn; };
    void SetLoadYourOwn( BOOL Value ){ LoadYourOwn = Value; };
    char *GetSoundDir(){ return SoundDirectory; };
    void SetSoundDir( char *text ){ lstrcpy( SoundDirectory, text ); };
    BOOL GetRandomMusic(){ return RandomMusic; };
    void SetRandomMusic( BOOL Value ){ RandomMusic = Value; };

    // Order form access functions
    char *GetName(){ return Name; };
    void SetName( char *text ){ lstrcpy( Name, text ); };
    char *GetAddress1(){ return Address1; };
    void SetAddress1( char *text ){ lstrcpy( Address1, text ); };
    char *GetAddress2(){ return Address2; };
    void SetAddress2( char *text ){ lstrcpy( Address2, text ); };
    char *GetCity(){ return City; };
    void SetCity( char *text ){ lstrcpy( City, text ); };
    int GetStateIndex(){ return StateIndex; };
    void SetStateIndex( int Value ){ StateIndex = Value; };
    char *GetZip(){ return Zip; };
    void SetZip( char *text ){ lstrcpy( Zip, text ); };
    char *GetEmail(){ return Email; };
    void SetEmail( char *text ){ lstrcpy( Email, text ); };
    BOOL GetIncludeFloppy(){ return IncludeFloppy; };
    void SetIncludeFloppy( BOOL Value ){ IncludeFloppy = Value; };
    BOOL GetIlTaxes(){ return IlTaxes; };
    void SetIlTaxes( BOOL Value ){ IlTaxes = Value; };
    BOOL GetIntroHelpWnd(){ return IntroHelpWnd; };
    void SetIntroHelpWnd( BOOL Value ){ IntroHelpWnd = Value; };
    BOOL GetOrderFrmHelpWnd(){ return OrderFrmHelpWnd; };
    void SetOrderFrmHelpWnd( BOOL Value ){ OrderFrmHelpWnd = Value; };

    // Other
    char *GetRegNum(){ return RegNum; };
    void SetRegNum( char *text ){ lstrcpy( RegNum, text ); };

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

    TSettings();
    ~TSettings(){};

    // Read settings from hard drive
    void GetPrivateString();
    // Write settings from hard drive
    void WritePrivateString();
    // Helpful hint settings
    void WriteHelpHintSettings();
};

typedef TSettings *PTSettings;


#endif  // __SSSETTINGS__