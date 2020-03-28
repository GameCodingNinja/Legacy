
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
#include "gdefines.h"        // Header file with #defines
#include "ssdefines.h"       // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "mintools.h"        // A misc group of useful functions
#include "Settings.h"        // Header file for this *.cpp file.

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
************************************************************************/
CSettings::CSettings()
{
    // Get the windows directory
    GetTheWindowsDirectory( winDir );

}   // Constructer


/************************************************************************
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
************************************************************************/
void CSettings::GetPrivateString()
{
    char iniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( iniFilePath, winDir );
    strcat( iniFilePath, INI_FILE_NAME );

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    // Get the speed
    speed = GetSettingsInt( iniFilePath, PRODUCT_NAME, "Speed", "2" );


    /******************************************
    *  Standard screen saver settings
    *****************************************/
    // Is the sound on or off
    musicOff = GetSettingsBool( iniFilePath, PRODUCT_NAME, "Music Off", "1" );
    // Do we play the space song
    themeMusicOn = GetSettingsBool( iniFilePath, PRODUCT_NAME, "Theme Music On", "0" );
    // Do we play our own songs
    loadYourOwn = GetSettingsBool( iniFilePath, PRODUCT_NAME, "Load Your Own", "0" );
    // Do we show the intro window
    introHelpWnd = GetSettingsBool( iniFilePath, PRODUCT_NAME, "Intro Help", "1" );
    // Get the sound directory
    lstrcpy( soundDirectory, GetSettingsChar( iniFilePath, PRODUCT_NAME, "Sound Dir", "C:\\My Documents" ) );
    // Do we randomize the music
    randomMusic = GetSettingsBool( iniFilePath, PRODUCT_NAME, "Randomize Music", "1" );    

}   // GetPrivateString


/************************************************************************
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
************************************************************************/
void CSettings::WritePrivateString()
{
    char iniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( iniFilePath, winDir );
    strcat( iniFilePath, INI_FILE_NAME );


    /******************************************
    *  Screen saver specific settings
    *****************************************/
    // Save the speed
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Speed", speed );


    /******************************************
    *  Standard screen saver settings
    *****************************************/
    // Save the sound on or off
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Music Off", musicOff );
    // Save if we play the space song
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Theme Music On", themeMusicOn );
    // Save if we play our own songs
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Load Your Own", loadYourOwn );
    // Save the sound directory
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Sound Dir", soundDirectory );
    // Save randomize the music
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Randomize Music", randomMusic );

	WriteHelpHintSettings();

}   // WritePrivateString


/************************************************************************
*    DESCRIPTION:           The helpful hint settings need to be saved
*                           even if the user cancels out of the config
*                           dialog box. Put all the Helpful hint settings
*                           to be saved here so that it can be saved seperately.
************************************************************************/
void CSettings::WriteHelpHintSettings()
{
    char iniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( iniFilePath, winDir );
    strcat( iniFilePath, INI_FILE_NAME );

	// Save if they want to see the intro window again
    SaveIniSetting( iniFilePath, PRODUCT_NAME, "Intro Help", introHelpWnd );

}   // WriteHelpHintSettings
