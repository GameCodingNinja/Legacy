
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



#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "GDefines.h"        // Header file with #defines
#include "BaseSet.h"         // Base class header file.
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.


class TSettings : public TBaseSettings
{
protected:

    char SavePath[ MAX_PATH+1 ];
    char WinDir[ MAX_PATH+1 ];
	char FileName[ FILE_NAME_SIZE + 1 ];

	int SaveOption, BitDepth, GrabKey;
	BOOL ActiveWnd, soundEffect;
	int timerHour, timerMinute, timerSecond;

public:

    void GetPrivateString();
    void WritePrivateString();
    
    // Path to save image file
    char *GetSavePath(){ return SavePath; };
    // File name
    char *GetFileName(){ return FileName; };

    int GetSaveOption(){ return SaveOption; };
    void SetSaveOption( int value ){ SaveOption = value; };
    
    int GetBitDepth(){ return BitDepth; };
    void SetBitDepth( int value ){ BitDepth = value; };
    
    int GetGrabKey(){ return GrabKey; };
    void SetGrabKey( int value ){ GrabKey = value; };
    
    BOOL GetActiveWnd(){ return ActiveWnd; };
    void SetActiveWnd( BOOL value ){ ActiveWnd = value; };
    
    BOOL GetSoundEffect(){ return soundEffect; };
    void SetSoundEffect( BOOL value ){ soundEffect = value; };   
    
    int GetTimerHour(){ return timerHour; };
    void SetTimerHour( int value ){ timerHour = value; };
    
    int GetTimerMinute(){ return timerMinute; };
    void SetTimerMinute( int value ){ timerMinute = value; };
    
    int GetTimerSecond(){ return timerSecond; };
    void SetTimerSecond( int value ){ timerSecond = value; };  

    TSettings();
    ~TSettings(){};
};

typedef TSettings *PTSettings;


#endif  // __SETTINGS__