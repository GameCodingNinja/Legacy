
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

    bool musicOff;

public:

    void GetPrivateString();
    void WritePrivateString();

    /******************************************
    *  Standard screen saver settings
    *****************************************/
    bool GetMusicOff(){ return musicOff; };

    CSettings();
    ~CSettings(){};
};

typedef CSettings *PCSettings;


#endif  // __SSSETTINGS__