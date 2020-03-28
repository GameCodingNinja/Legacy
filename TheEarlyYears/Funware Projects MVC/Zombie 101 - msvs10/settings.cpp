
/************************************************************************
*
*    FILE NAME:       settings.cpp
*
*    DESCRIPTION:     Saver settings to the ini file  
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "mintools.h"        // A misc group of useful functions
#include "Settings.h"        // Header file for this *.cpp file.

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)


/************************************************************************
*    DESCRIPTION:   Retrieve program settings
************************************************************************/
void CSettings::GetPrivateString( char *iniFileStr, char *sectionStr )
{
    invertMouse = GetSettingsBool( iniFileStr, sectionStr, "Invert Mouse", "0" );
    screenWidth = GetSettingsInt( iniFileStr, sectionStr, "Screen Width", "1024" );
    screenHeight = GetSettingsInt( iniFileStr, sectionStr, "Screen Height", "768" );
    renderDevice = (ERM_RENDER_DEVICE)GetSettingsInt( iniFileStr, sectionStr, "Render Device", "3" );
    displayMode = (EDM_DISPLAY_MODE)GetSettingsInt( iniFileStr, sectionStr, "Display Mode", "1" );
    disableTextures = GetSettingsBool( iniFileStr, sectionStr, "Disable Textures", "0" );    

}   // GetPrivateString


/************************************************************************
*    DESCRIPTION:   Save program settings
************************************************************************/
void CSettings::WritePrivateString( char *iniFileStr, char *sectionStr )
{
    SaveIniSetting( iniFileStr, sectionStr, "Invert Mouse", invertMouse );
	SaveIniSetting( iniFileStr, sectionStr, "Screen Width", screenWidth );
	SaveIniSetting( iniFileStr, sectionStr, "Screen Height", screenHeight );
	SaveIniSetting( iniFileStr, sectionStr, "Render Device", (int)renderDevice );
	SaveIniSetting( iniFileStr, sectionStr, "Display Mode", (int)displayMode );
	SaveIniSetting( iniFileStr, sectionStr, "Disable Textures", (int)disableTextures );

}   // WritePrivateString

