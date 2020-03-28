
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
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "BaseSet.h"         // Header file for this *.cpp file.

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf


// This quiets the compiler warnings about a bunch of functions that Microsoft has
// decided to deprecate, even though the C++ standardization committee has not done so.
#pragma warning(disable : 4996)

/************************************************************************
*    DESCRIPTION:           Get the ini settings if it is a BOOL
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
*
*    Output:   BOOL - The setting type
************************************************************************/
bool CBaseSettings::GetSettingsBool( char *FileName, char *SectName, char *KeyName, char *DefaultTxt )
{
    GetPrivateProfileString( SectName, KeyName, DefaultTxt, TempStr, sizeof(TempStr), FileName );

    return (bool)atoi( TempStr );

}   // GetSettingsBool


/************************************************************************
*    DESCRIPTION:           Get the ini settings if it is a int
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
*
*    Output:   int - The setting type
************************************************************************/
int CBaseSettings::GetSettingsInt( char *FileName, char *SectName, char *KeyName, char *DefaultTxt )
{
    GetPrivateProfileString( SectName, KeyName, DefaultTxt, TempStr, sizeof(TempStr), FileName );

    return atoi( TempStr );

}   // GetSettingsInt


/************************************************************************
*    DESCRIPTION:           Get the ini settings if it is a int
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
*
*    Output:   int - The setting type
************************************************************************/
float CBaseSettings::GetSettingsFloat( char *FileName, char *SectName, char *KeyName, char *DefaultTxt )
{
    GetPrivateProfileString( SectName, KeyName, DefaultTxt, TempStr, sizeof(TempStr), FileName );

    return (float)atof( TempStr );

}   // GetSettingsInt


/************************************************************************
*    DESCRIPTION:           Get the ini settings if it is a int
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
*
*    Output:   int - The setting type
************************************************************************/
int CBaseSettings::GetSettingsInt( char *FileName, char *SectName, char *KeyName, int DefaultTxt )
{
    char DefSetting[20];

    sprintf( DefSetting, "%u", DefaultTxt );

    GetPrivateProfileString( SectName, KeyName, DefSetting, TempStr, sizeof(TempStr), FileName );

    return atoi( TempStr );

}   // GetSettingsInt


/************************************************************************
*    DESCRIPTION:           Get the ini settings if it is a char
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
*
*    Output:   char - The setting type
************************************************************************/
char *CBaseSettings::GetSettingsChar( char *FileName, char *SectName, char *KeyName, char *DefaultTxt )
{
    GetPrivateProfileString( SectName, KeyName, DefaultTxt, TempStr, sizeof(TempStr), FileName );

    return TempStr;

}   // GetSettingsChar


/************************************************************************
*    DESCRIPTION:           Save the value to the ini file
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
************************************************************************/
void CBaseSettings::SaveIniSetting( char *FileName, char *SectName, char *KeyName, char *Value )
{
    WritePrivateProfileString( SectName, KeyName, Value, FileName );

}   // SaveIniSetting


/************************************************************************
*    DESCRIPTION:           Save the value to the ini file
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName   - Name of the INI file
*              char *SectName   - The application this group applies to
*              char *KeyName    - The name assocated with this setting
*              char *DefaultTxt - The default value of this setting 
************************************************************************/

void CBaseSettings::SaveIniSetting( char *FileName, char *SectName, char *KeyName, int Value )
{
    sprintf( TempStr, "%d", Value );
    WritePrivateProfileString( SectName, KeyName, TempStr, FileName );

}   // SaveIniSetting