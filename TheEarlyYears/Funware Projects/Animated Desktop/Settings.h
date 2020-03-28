
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
#define ORDER_FORM      "Order Form"


class TSettings : public TBaseSettings
{
protected:

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    int SnowAmount;
	BOOL Star, XMasLights, Smoke, Santa, People;


    /******************************************
    *  Standard screen saver settings
    *****************************************/
    char RegNum[100];
    char WinDir[MAX_PATH];
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
    char Name[MAX_OF_NAME+1];
    char Address1[MAX_OF_ADDRESS+1];
    char Address2[MAX_OF_ADDRESS+1];
    char City[MAX_OF_CITY+1];
    char Zip[MAX_OF_ZIP+1];
    char Email[MAX_OF_EMAIL+1];
    BOOL IncludeFloppy;
    BOOL IlTaxes;
    BOOL OrderFrmHelpWnd;

    // These are not saved to the ini file. They are for 
    // setting up the order form window
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

    void GetPrivateString();
    void WritePrivateString();

    void WriteHelpHintSettings();

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    int GetSnowAmount(){ return SnowAmount; };
    void SetSnowAmount( int Value ){ SnowAmount = Value; };
    BOOL GetStar(){ return Star; };
    void SetStar( BOOL Value ){ Star = Value; };
    BOOL GetXMasLights(){ return XMasLights; };
    void SetXMasLights( BOOL Value ){ XMasLights = Value; };
    BOOL GetSmoke(){ return Smoke; };
    void SetSmoke( BOOL Value ){ Smoke = Value; };
    BOOL GetSanta(){ return Santa; };
    void SetSanta( BOOL Value ){ Santa = Value; };
    BOOL GetPeople(){ return People; };
    void SetPeople( BOOL Value ){ People = Value; };


    /******************************************
    *  Standard screen saver settings
    *****************************************/
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
    char *GetRegNum(){ return RegNum; };
    void SetRegNum( char *text ){ lstrcpy( RegNum, text ); };

    /******************************************
    *  Orderform settings
    *****************************************/
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
    BOOL GetIlTaxes(){ return IlTaxes; };
    void SetIlTaxes( BOOL Taxes ){ IlTaxes = Taxes; };
    BOOL GetIntroHelpWnd(){ return IntroHelpWnd; };
    void SetIntroHelpWnd( BOOL Value ){ IntroHelpWnd = Value; };
    BOOL GetOrderFrmHelpWnd(){ return OrderFrmHelpWnd; };
    void SetOrderFrmHelpWnd( BOOL Value ){ OrderFrmHelpWnd = Value; };

    // These are not saved to the ini file. They are for 
    // setting up the order form window
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
};

typedef TSettings *PTSettings;


#endif  // __SSSETTINGS__