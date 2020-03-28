
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
}


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
    strcat( IniFilePath, "SSToGo_Saver.ini" );

    // Is the sound on or off
    SoundOnOff = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Sound On or Off", "1" );
    // Is the transition on or off
    TransOnOff = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Trans On or Off", "1" );
    // Is the music to play in random order
    PlayMusicRandOrder = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Play Music Rand Order", "0" );
    // Get Size Image To Fit
    SizeImageToFit = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Size Image To Fit", "0" );
    // Get Screens Random Order
    ScreensRandomOrder = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Screens Random Order", "0" );
	// Get the backs in rendom order
	BackRandomOrder = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Backs Random Order", "0" );

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
    strcat( IniFilePath, "SSToGo_Saver.ini" );

    // Save the sound on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Sound On or Off", SoundOnOff );
    // Save the transition on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Trans On or Off", TransOnOff );
    // Is the music to play in random order
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Play Music Rand Order", PlayMusicRandOrder );
    // Save Size Image To Fit
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Size Image To Fit", SizeImageToFit );
    // Save Screens Random Order
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Screens Random Order", ScreensRandomOrder );
    // Save backs Random Order
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Backs Random Order", BackRandomOrder );

}   // WritePrivateString