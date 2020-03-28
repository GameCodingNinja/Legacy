
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

void TSettings::GetPrivateString( char *IniFile, char *Section )
{
    WindowX = GetSettingsInt( IniFile, Section, "Window X", "-1000" );
    WindowY = GetSettingsInt( IniFile, Section, "Window Y", "-1000" );
    CameraSize = GetSettingsInt( IniFile, Section, "Camera Size", "400" );
    MaxZDistance = GetSettingsInt( IniFile, Section, "Max Z Distance", "500000" );
	MaxLightDistance = GetSettingsInt( IniFile, Section, "Max Light Distance", "100000" );
    lstrcpy( ProjectPath, GetSettingsChar( IniFile, Section, "Project Path", "" ) );

	XYTransAmount = GetSettingsInt( IniFile, Section, "XYTrans Amount", "1" );
	ZTransAmount = GetSettingsInt( IniFile, Section, "ZTrans Amount", "20" );

	MoveCamera = GetSettingsBool( IniFile, Section, "Move Camera", "0" );
	TextureMode = GetSettingsBool( IniFile, Section, "Animation Mode", "1" );
	CameraZeroPoint = GetSettingsBool( IniFile, Section, "Camera Zero Point", "1" );
	FramesPerSec = GetSettingsInt( IniFile, Section, "Frames Per Sec", "20" );
	CompressedList = GetSettingsBool( IniFile, Section, "Compressed List", "0" );

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

void TSettings::WritePrivateString( char *IniFile, char *Section )
{
    SaveIniSetting( IniFile, Section, "Window X", WindowX );
    SaveIniSetting( IniFile, Section, "Window Y", WindowY );
    SaveIniSetting( IniFile, Section, "Camera Size", CameraSize );
    SaveIniSetting( IniFile, Section, "Max Z Distance", MaxZDistance );
	SaveIniSetting( IniFile, Section, "Max Light Distance", MaxLightDistance );
    SaveIniSetting( IniFile, Section, "Project Path", ProjectPath );

	SaveIniSetting( IniFile, Section, "XYTrans Amount", XYTransAmount );
	SaveIniSetting( IniFile, Section, "ZTrans Amount", ZTransAmount );

	SaveIniSetting( IniFile, Section, "Move Camera", MoveCamera );
	SaveIniSetting( IniFile, Section, "Animation Mode", TextureMode );
	SaveIniSetting( IniFile, Section, "Camera Zero Point", CameraZeroPoint );
	SaveIniSetting( IniFile, Section, "Frames Per Sec", FramesPerSec );
	SaveIniSetting( IniFile, Section, "Compressed List", CompressedList );

}   // WritePrivateString
