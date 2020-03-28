
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       Settings.cpp
*
*    DESCRIPTION:     Saves screen saver settings to the ini file 
*
*    IMPLEMENTATION:  I use ini files for all the programs I write and
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

#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "mintools.h"        // A misc group of useful functions
#include "Settings.h"        // Header file for this *.cpp file.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


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

TSettings::TSettings()
{
    // Get the windows directory
    GetTheWindowsDirectory( WinDir );

}   // Constructer


/************************************************************************
*    FUNCTION NAME:         GetPrivateString                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    none 
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

void TSettings::GetPrivateString()
{
    char IniFilePath[MAX_STR_PATH];
    char StrTemp[20];
    int i;

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    // Add your screen saver specific access functions here
    // Fish count index
    FishCount = GetSettingsInt( IniFilePath, PRODUCT_NAME, "FishCount", "1" );

    // Bubbles on or off
    BubblesOnOff = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Bubbles On or Off", "1" );

    // Background fish on or off
    BackFishOnOff = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Backfish On or Off", "1" );

    // Get the business name
    strcpy( BusinessName, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Business Name", "" ) );

    // Get custom Colors
    for( i = 0; i < MAX_CUSTOM_COLORS; ++i )
    {
        sprintf( StrTemp, "%s%d", "Custom Color ", i );
        CustomColors[i] = GetSettingsInt( IniFilePath, PRODUCT_NAME, StrTemp, "16777215" );
    }

    // Get the font settings
    lf.lfHeight = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Height", "-96" ); 
    lf.lfWidth = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Width", "0" ); 
    lf.lfEscapement = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Escapement", "0" ); 
    lf.lfOrientation = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Orientation", "0" ); 
    lf.lfWeight = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Weight", "400" ); 
    lf.lfItalic = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Italic", "0" ); 
    lf.lfUnderline = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Underline", "0" ); 
    lf.lfStrikeOut = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font StrikeOut", "0" ); 
    lf.lfCharSet = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font CharSet", "0" ); 
    lf.lfOutPrecision = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font OutPrecision", "3" ); 
    lf.lfClipPrecision = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font ClipPrecision", "2" ); 
    lf.lfQuality = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Quality", "1" ); 
    lf.lfPitchAndFamily = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font PitchAndFamily", "34" );
    strcpy( lf.lfFaceName, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Font FaceName", "Arial" ) );

    // Text Color
    TxtColor = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Text Color", "65535" );

    // Standard screen saver settings
    // Is the sound on or off
    MusicOff = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Music Off", "1" );
    // Do we play the theme song
    ThemeMusicOn = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Theme Music On", "0" );
    // Do we play our own songs
    LoadYourOwn = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Load Your Own", "0" );
    // Do we show the intro window
    IntroHelpWnd = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Intro Help", "1" );
    // Get the sound directory
    strcpy( SoundDirectory, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Sound Dir", "C:\\My Documents" ) );
    // Do we randomize the music
    RandomMusic = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Randomize Music", "1" );

    // Get Orderform settings
    strcpy( Name, GetSettingsChar( IniFilePath, ORDER_FORM, "Name", "" ) );
    strcpy( Address1, GetSettingsChar( IniFilePath, ORDER_FORM, "Address1", "" ) );
    strcpy( Address2, GetSettingsChar( IniFilePath, ORDER_FORM, "Address2", "" ) );
    strcpy( City, GetSettingsChar( IniFilePath, ORDER_FORM, "City", "" ) );
    StateIndex = GetSettingsInt( IniFilePath, ORDER_FORM, "State Index", "0" );
    strcpy( Zip, GetSettingsChar( IniFilePath, ORDER_FORM, "Zip", "" ) );
    strcpy( Email, GetSettingsChar( IniFilePath, ORDER_FORM, "Email", "" ) );
    IncludeFloppy = GetSettingsBool( IniFilePath, ORDER_FORM, "Floppy included", "0" );
    IlTaxes = GetSettingsBool( IniFilePath, ORDER_FORM, "Il Resident Taxes", "0" );
    OrderFrmHelpWnd = GetSettingsBool( IniFilePath, ORDER_FORM, "Order Form Help", "1" );

    // Other
    strcpy( RegNum, GetSettingsChar( IniFilePath, PRODUCT_NAME, "FDMB", FAKE_VERSION_NUMBER ) );

}   // GetPrivateString


/************************************************************************
*    FUNCTION NAME:         WritePrivateString                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    none 
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

void TSettings::WritePrivateString()
{
    char IniFilePath[MAX_STR_PATH];
    char StrTemp[20];
    int i;

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    // Fish count index
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "FishCount", FishCount );

    // Save bubbles on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Bubbles On or Off", BubblesOnOff );

    // Save background fish on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Backfish On or Off", BackFishOnOff );

    // Save the business name
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Business Name", BusinessName );

    // Save custom Colors
    for( i = 0; i < MAX_CUSTOM_COLORS; ++i )
    {
        sprintf( StrTemp, "%s%d", "Custom Color ", i );
        SaveIniSetting( IniFilePath, PRODUCT_NAME, StrTemp, CustomColors[i] );
    }

    // Write the font settings
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Height", lf.lfHeight );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Width", lf.lfWidth );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Escapement", lf.lfEscapement );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Orientation", lf.lfOrientation );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Weight", lf.lfWeight );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Italic", lf.lfItalic );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Underline", lf.lfUnderline );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font StrikeOut", lf.lfStrikeOut );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font CharSet", lf.lfCharSet );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font OutPrecision", lf.lfOutPrecision );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font ClipPrecision", lf.lfClipPrecision );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font Quality", lf.lfQuality );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font PitchAndFamily", lf.lfPitchAndFamily );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Font FaceName", lf.lfFaceName );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Text Color", TxtColor );

    // Standard screen saver settings
    // Save the sound on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Music Off", MusicOff );
    // Save if we play the space song
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Theme Music On", ThemeMusicOn );
    // Save if we play our own songs
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Load your own", LoadYourOwn );
    // Save the sound directory
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Sound Dir", SoundDirectory );
    // Save randomize the music
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Randomize Music", RandomMusic );

    // Save Orderform settings
    SaveIniSetting( IniFilePath, ORDER_FORM, "Name", Name );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Address1", Address1 );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Address2", Address2 );
    SaveIniSetting( IniFilePath, ORDER_FORM, "City", City );
    SaveIniSetting( IniFilePath, ORDER_FORM, "State Index", StateIndex );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Zip", Zip );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Email", Email );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Floppy included", IncludeFloppy );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Il Resident Taxes", IlTaxes );

    // Other
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "FDMB", RegNum );

    WriteHelpHintSettings();

}   // WritePrivateString


/************************************************************************
*    FUNCTION NAME:         WriteHelpHintSettings                                                             
*
*    DESCRIPTION:           The helpful hint settings need to be saved
*                           even if the user cancels out of the config
*                           dialog box. Put all the Helpful hint settings
*                           to be saved here so that it can be saved seperately.
*
*    FUNCTION PARAMETERS:
*    Input:    none 
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

void TSettings::WriteHelpHintSettings()
{
    // Save if they want to see the orderform help window again
    SaveIniSetting( INI_FILE_NAME, ORDER_FORM, "Order Form Help", OrderFrmHelpWnd );
    // Save if they want to see the intro window again
    SaveIniSetting( INI_FILE_NAME, PRODUCT_NAME, "Intro Help", IntroHelpWnd );

}   // WriteHelpHintSettings
