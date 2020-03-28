
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

    int WindowX, WindowY;
    char ProjectPath[ MAX_PATH ];
    int CameraSize, MaxZDistance, MaxLightDistance;
    int XYTransAmount, ZTransAmount;
	BOOL MoveCamera, TextureMode, CameraZeroPoint;
	int FramesPerSec;
	BOOL CompressedList;
public:

    void GetPrivateString( char *IniFile, char *Section );
    void WritePrivateString( char *IniFile, char *Section );

    // Add your programr specific access functions here
    int GetWindowX(){ return WindowX; };
    void SetWindowX( int Value ){ WindowX = Value; };
    int GetWindowY(){ return WindowY; };
    void SetWindowY( int Value ){ WindowY = Value; };
    int GetCameraSize(){ return CameraSize; };
    void SetCameraSize( int Value ){ CameraSize = Value; };
    int GetMaxZDistance(){ return MaxZDistance; };
    void SetMaxZDistance( int Value ){ MaxZDistance = Value; };
    int GetMaxLightDistance(){ return MaxLightDistance; };
    void SetMaxLightDistance( int Value ){ MaxLightDistance = Value; };

    int GetXYTransAmount(){ return XYTransAmount; };
    void SetXYTransAmount( int Value ){ XYTransAmount = Value; };
    int GetZTransAmount(){ return ZTransAmount; };
    void SetZTransAmount( int Value ){ ZTransAmount = Value; };

    int GetFramesPerSec(){ return FramesPerSec; };
    void SetFramesPerSec( int Value ){ FramesPerSec = Value; };

    BOOL GetMoveCamera(){ return MoveCamera; };
    void SetMoveCamera( BOOL Value ){ MoveCamera = Value; };
    BOOL GetTextureMode(){ return TextureMode; };
    void SetTextureMode( BOOL Value ){ TextureMode = Value; };
    BOOL GetCameraZeroPoint(){ return CameraZeroPoint; };
    void SetCameraZeroPoint( BOOL Value ){ CameraZeroPoint = Value; };

    BOOL GetCompressedList(){ return CompressedList; };
    void SetCompressedList( BOOL Value ){ CompressedList = Value; };
    
    // Path to project file
    char *GetProjectPath(){ return ProjectPath; };
    void SetProjectPath( char *Value ){ lstrcpy( ProjectPath, Value ); };  

    TSettings();
    ~TSettings(){};
};

typedef TSettings *PTSettings;


#endif  // __SETTINGS__