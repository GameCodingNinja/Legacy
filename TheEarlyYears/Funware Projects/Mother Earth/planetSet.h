/************************************************************************
*
*    PRODUCT:         Planet Settings
*
*    FILE NAME:       planetSet.h
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


#ifndef __PLANET_SET_H__
#define __PLANET_SET_H__

#include "BaseSet.h"    // Base class header file.

class CPlanetSet : public TBaseSettings
{
protected:

    // Path to the ini file to load
    char iniFilePath[MAX_PATH];

	// Planet Settings
	int RotationVector;
	float AxisAngle;
	int ZoomMax;

public:

    CPlanetSet();
    ~CPlanetSet(){};

    // Get settings
    void GetPrivateString();
    // Save settings
    void WritePrivateString();
	// Copies the file name to the class varaible
    void SetIniFilePath( char *filePath );

    int GetRotationVector(){ return RotationVector; };
	float GetAxisAngle(){ return AxisAngle; };
	int GetZoomMax(){ return ZoomMax; };
};


#endif  // __SSSETTINGS__