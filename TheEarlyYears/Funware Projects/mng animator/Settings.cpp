
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       SSSettings.cpp
*
*    DESCRIPTION:     Saves screen saver settings to the ini file 
*
*    IMPLEMENTATION:  I use ini files for all the programs I write and
*                     stay away from using the Registry. The Registry can
*                     get hosed and I would rather not be one of the
*                     suspects. Using ini files is fast and safe and if
*                     something were to go wrong because the users computer
*                     has low system resources, the worst that can happen
*                     is that the ini files get hit and my program
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

#include "Settings.h"        // Header file for this *.cpp file.
#include "gTools.h"          // A misc group of useful functions
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf



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

	ProjectPath[0] = 0;
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
    char IniFilePath[MAX_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    WindowX = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window X", "-1000" );
    WindowY = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window Y", "-1000" );

    lstrcpy( ProjectPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Path", "" ) );

    // Get the frame count
    AnimFrameCount = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Anim Frame Count", "20" );
    GostPrevFrame = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Gost Prev Frame", "0" );
    LoopAnimations = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Loop Animations", "1" );
	PlaySounds = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Play Sounds", "1" );

    // Get the sound directory
    lstrcpy( SoundDirectory, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Sound Dir", "" ) );

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
    char IniFilePath[MAX_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window X", WindowX );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window Y", WindowY );

    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Path", ProjectPath );

    // Save the speed
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Anim Frame Count", AnimFrameCount );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Gost Prev Frame", GostPrevFrame );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Loop Animations", LoopAnimations );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Play Sounds", PlaySounds );

    // Save the sound directory
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Sound Dir", SoundDirectory );

}   // WritePrivateString
