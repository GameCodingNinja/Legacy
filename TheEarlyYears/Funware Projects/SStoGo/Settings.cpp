
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
#include "mintools.h"        // A misc group of useful functions

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
    char StrTemp[20];
    int i;

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, "SSToGo_Builder.ini" );

    FWindowX = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window X", "-1" );
    FWindowY = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window Y", "-1" );
    showInstallInfo = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Show Installer Info", "1" );

	FOrderFrmHelpWnd = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Order Form Help", "1" );
    FIntroHelpWnd = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Intro Help", "1" );

    // Get the paths to the different projects
    lstrcpy( ProjectPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Path", "C:\\Program Files\\Screen Savers To Go\\Sample Screen Saver" ) );
	lstrcpy( ProjectArtPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Art Path", "" ) );
	lstrcpy( ProjectBKArtPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project BK Art Path", "C:\\Program Files\\Screen Savers To Go\\Background Files" ) );
	lstrcpy( ProjectSoundPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Sound Path", "" ) );

    // Get custom Colors
    for( i = 0; i < MAX_CUSTOM_COLORS; ++i )
    {
        sprintf( StrTemp, "%s%d", "Custom Color ", i );
        CustomColors[i] = GetSettingsInt( IniFilePath, PRODUCT_NAME, StrTemp, "16777215" );
    }

	// Get the font settings
    lf.lfHeight = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Font Height", "-56" ); 
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
    lstrcpy( lf.lfFaceName, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Font FaceName", "Arial" ) );
	DefTxtColor = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Def Text Color", "65535" );

	DefTxtJust = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Def Text Just", "0" );
	DelayInSec = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Delay In Sec", "7" );

}   // GetPrivateString


/************************************************************************
*    FUNCTION NAME:         WriteSecretIniFile                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - Pointer to file name that gets added to
*                               the Windows directory
*              char *Section  - The name of the section in the ini file
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

void TSettings::WriteSecretIniFile( char *FileName, char *Section )
{
    char IniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, FileName );

    // Save to the secret Ini file
    SaveIniSetting( IniFilePath, Section, "Reg No", RegNum );
    

}   // WriteSecretIniFile


/************************************************************************
*    FUNCTION NAME:         GetSecretIniFile                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - Pointer to file name that gets added to
*                               the Windows directory
*              char *Section  - The name of the section in the ini file 
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

void TSettings::GetSecretIniFile( char *FileName, char *Section )
{
    char IniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, FileName );

    // read from the secret ini file
    lstrcpy( RegNum, GetSettingsChar( IniFilePath, Section, "Reg No", FAKE_VERSION_NUMBER ) );

}   // GetSecretIniFile


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
    strcat( IniFilePath, "SSToGo_Builder.ini" );

    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window X", FWindowX );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window Y", FWindowY );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Show Installer Info", showInstallInfo );

	// Save if they want to see the orderform help window again
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Order Form Help", FOrderFrmHelpWnd );
    // Save if they want to see the intro window again
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Intro Help", FIntroHelpWnd );

    // Save the paths to the different projects
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Path", ProjectPath );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Art Path", ProjectArtPath );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project BK Art Path", ProjectBKArtPath );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Sound Path", ProjectSoundPath );

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
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Def Text Color", DefTxtColor );

	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Def Text Just", DefTxtJust );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Delay In Sec", DelayInSec );

}   // WritePrivateString
