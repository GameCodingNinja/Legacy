
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
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.


class TSettings : public TBaseSettings
{
protected:

    int WindowX, WindowY, ListSortMode;
    char ProjectPath[ MAX_PATH+1 ];
    char WinDir[ MAX_PATH+1 ];

    int AnimFrameCount;
	BOOL GostPrevFrame;
	BOOL LoopAnimations;
	BOOL PlaySounds;
	char SoundDirectory[MAX_PATH];

public:

    void GetPrivateString();
    void WritePrivateString();

    // Add your programr specific access functions here
    int GetWindowX(){ return WindowX; };
    void SetWindowX( int value ){ WindowX = value; };
    int GetWindowY(){ return WindowY; };
    void SetWindowY( int value ){ WindowY = value; };
    
    // Path to project file
    char *GetProjectPath(){ return ProjectPath; };

	// Get the path to the sound directory
	char *GetSoundDir(){ return SoundDirectory; };
    
    int GetAnimFrameCount(){ return AnimFrameCount; };
    void SetAnimFrameCount( int Value ){ AnimFrameCount = Value; };

    BOOL GetGostPrevFrame(){ return GostPrevFrame; };
    void SetGostPrevFrame( BOOL Value ){ GostPrevFrame = Value; };

    BOOL GetLoopAnimations(){ return LoopAnimations; };
    void SetLoopAnimations( BOOL Value ){ LoopAnimations = Value; };
    
    BOOL GetPlaySounds(){ return PlaySounds; };
    void SetPlaySounds( BOOL Value ){ PlaySounds = Value; };  

    TSettings();
    ~TSettings(){};
};

typedef TSettings *PTSettings;


#endif  // __SETTINGS__