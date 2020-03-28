
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       SSSettings.h
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

#ifndef __BASE_SETTINGS_H__
#define __BASE_SETTINGS_H__

class CBaseSettings
{
protected:
    char TempStr[MAX_PATH*2];

    int GetSettingsInt( char *FileName, char *SectName, char *KeyName, char *DefaultTxt );
    int GetSettingsInt( char *FileName, char *SectName, char *KeyName, int DefaultTxt );
    char *GetSettingsChar( char *FileName, char *SectName, char *KeyName, char *DefaultTxt );
    bool GetSettingsBool( char *FileName, char *SectName, char *KeyName, char *DefaultTxt );
    void SaveIniSetting( char *FileName, char *SectName, char *KeyName, char *Value );
    void SaveIniSetting( char *FileName, char *SectName, char *KeyName, int Value );
    float GetSettingsFloat( char *FileName, char *SectName, char *KeyName, char *DefaultTxt );

public:

    // The below code is NOT screen saver specific. Only make changes the are intended
    // to effect all projects.
    CBaseSettings(){};
    ~CBaseSettings(){};
};

#endif  // __BASE_SETTINGS__