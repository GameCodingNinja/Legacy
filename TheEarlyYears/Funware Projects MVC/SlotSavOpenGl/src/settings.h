
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


class CSettings : public CBaseSettings
{
protected:

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    int speed;


    /******************************************
    *  Standard screen saver settings
    *****************************************/
    char winDir[MAX_PATH];
    bool introHelpWnd;
    bool musicOff;
    bool themeMusicOn;
    bool loadYourOwn;
    bool randomMusic;
    char soundDirectory[MAX_PATH];

public:

    void GetPrivateString();
    void WritePrivateString();
	void WriteHelpHintSettings();

    /******************************************
    *  Screen saver specific settings
    *****************************************/
    int GetSpeed(){ return speed; };
    void SetSpeed( int Value ){ speed = Value; };


    /******************************************
    *  Standard screen saver settings
    *****************************************/
    bool GetMusicOff(){ return musicOff; };
    void SetMusicOff( bool value ){ musicOff = value; };
    bool GetThemeMusicOn(){ return themeMusicOn; };
    void SetThemeMusicOn( bool value ){ themeMusicOn = value; };
    bool GetLoadYourOwn(){ return loadYourOwn; };
    void SetLoadYourOwn( bool value ){ loadYourOwn = value; };
    char *GetSoundDir(){ return soundDirectory; };
    void SetSoundDir( char *text ){ lstrcpy( soundDirectory, text ); };
    bool GetRandomMusic(){ return randomMusic; };
    void SetRandomMusic( bool value ){ randomMusic = value; };
	bool GetIntroHelpWnd(){ return introHelpWnd; };
    void SetIntroHelpWnd( bool value ){ introHelpWnd = value; };

    CSettings();
    ~CSettings(){};
};

typedef CSettings *PCSettings;


#endif  // __SSSETTINGS__