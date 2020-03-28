
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       Settings.h
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



#ifndef __SSSETTINGS_H__
#define __SSSETTINGS_H__

#include "BaseSet.h"    // Base class header file.


class TSettings : public TBaseSettings
{
protected:

    // Add your variables here for screen saver specific programs
    BOOL SoundOnOff, TransOnOff, PlayMusicRandOrder;
    BOOL SizeImageToFit, ScreensRandomOrder, BackRandomOrder;

	// Other
	char WinDir[MAX_PATH];

public:

    void GetPrivateString();
    void WritePrivateString();

    // Add your screen saver specific access functions here
    BOOL GetSoundOnOff(){ return SoundOnOff; };
    void SetSoundOnOff( BOOL OnOff ){ SoundOnOff = OnOff; };
    BOOL GetTransOnOff(){ return TransOnOff; };
    void SetTransOnOff( BOOL OnOff ){ TransOnOff = OnOff; };
    BOOL GetPlayMusicRandOrder(){ return PlayMusicRandOrder; };
    void SetPlayMusicRandOrder( BOOL Value ){ PlayMusicRandOrder = Value; };
    BOOL GetSizeImageToFit(){ return SizeImageToFit; };
    void SetSizeImageToFit( BOOL Value ){ SizeImageToFit = Value; };
    BOOL GetScreensRandomOrder(){ return ScreensRandomOrder; };
    void SetScreensRandomOrder( BOOL Value ){ ScreensRandomOrder = Value; };
    BOOL GetBackRandOrder(){ return BackRandomOrder; };
    void SetBackRandOrder( BOOL Value ){ BackRandomOrder = Value; };

    TSettings();
    ~TSettings(){};
};

typedef TSettings *PTSettings;


#endif  // __SSSETTINGS__