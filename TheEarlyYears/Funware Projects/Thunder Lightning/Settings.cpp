
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

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    // Get the Rain Amount
    RainAmount = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Rain Amount", "2" );
    // Get the Rain Type
    RainType = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Rain Type", "2" );
    // Get the Light Hits Type
    LightHits = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Light Hits", "2" );
    // Get the Group Hits Type
    GroupHits = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Group Hits", "2" );
    // Get the Black Outs Type
    BlackOuts = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Black Outs", "2" );

    // Do we want the sound on?
    SoundOn = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Sound On", "0" );

    /******************************************
    *  Standard screen saver settings
    *****************************************/
    // Is the sound on or off
    MusicOff = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Music Off", "1" );
    // Do we play the space song
    ThemeMusicOn = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Theme Music On", "0" );
    // Do we play our own songs
    LoadYourOwn = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Load Your Own", "0" );
    // Do we show the intro window
    IntroHelpWnd = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Intro Help", "1" );
    // Get the sound directory
    lstrcpy( SoundDirectory, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Sound Dir", "C:\\My Documents" ) );
    // Do we randomize the music
    RandomMusic = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Randomize Music", "1" );
    // Registration number
    lstrcpy( RegNum, GetSettingsChar( IniFilePath, PRODUCT_NAME, "FDMB", FAKE_VERSION_NUMBER ) );

    /******************************************
    *  Get Orderform settings
    *****************************************/
    lstrcpy( Name, GetSettingsChar( IniFilePath, ORDER_FORM, "Name", "" ) );
    lstrcpy( Address1, GetSettingsChar( IniFilePath, ORDER_FORM, "Address1", "" ) );
    lstrcpy( Address2, GetSettingsChar( IniFilePath, ORDER_FORM, "Address2", "" ) );
    lstrcpy( City, GetSettingsChar( IniFilePath, ORDER_FORM, "City", "" ) );
    StateIndex = GetSettingsInt( IniFilePath, ORDER_FORM, "State Index", "0" );
    lstrcpy( Zip, GetSettingsChar( IniFilePath, ORDER_FORM, "Zip", "" ) );
    lstrcpy( Email, GetSettingsChar( IniFilePath, ORDER_FORM, "Email", "" ) );
    IncludeFloppy = GetSettingsBool( IniFilePath, ORDER_FORM, "Floppy included", "0" );
    IlTaxes = GetSettingsBool( IniFilePath, ORDER_FORM, "Il Resident Taxes", "0" );
    OrderFrmHelpWnd = GetSettingsBool( IniFilePath, ORDER_FORM, "Order Form Help", "1" );    

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

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );


    /******************************************
    *  Screen saver specific settings
    *****************************************/
    // Save the Rain Amount
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Rain Amount", RainAmount );
    // Save Rain Type
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Rain Type", RainType );
    // Save Light Hits
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Light Hits", LightHits );
    // Save Group Hits
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Group Hits", GroupHits );
    // Save Black Outs
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Black Outs", BlackOuts );
    // Save the Sound On
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Sound On", SoundOn );

    /******************************************
    *  Standard screen saver settings
    *****************************************/
    // Save the sound on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Music Off", MusicOff );
    // Save if we play the space song
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Theme Music On", ThemeMusicOn );
    // Save if we play our own songs
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Load Your Own", LoadYourOwn );
    // Save the sound directory
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Sound Dir", SoundDirectory );
    // Save randomize the music
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Randomize Music", RandomMusic );
    // Save registration number
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "FDMB", RegNum );

    /******************************************
    *  Save Orderform settings
    *****************************************/
    SaveIniSetting( IniFilePath, ORDER_FORM, "Name", Name );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Address1", Address1 );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Address2", Address2 );
    SaveIniSetting( IniFilePath, ORDER_FORM, "City", City );
    SaveIniSetting( IniFilePath, ORDER_FORM, "State Index", StateIndex );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Zip", Zip );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Email", Email );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Floppy included", IncludeFloppy );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Il Resident Taxes", IlTaxes );

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
