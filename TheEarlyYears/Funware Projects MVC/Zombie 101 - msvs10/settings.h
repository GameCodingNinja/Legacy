
/************************************************************************
*
*    FILE NAME:       settings.h
*
*    DESCRIPTION:     Saver settings to the ini file  
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSSETTINGS_H__
#define __SSSETTINGS_H__

#include "baseset.h"    // Base class header file.
#include "3D/types.h"

class CSettings : public CBaseSettings
{
private:

    bool invertMouse;
	int screenWidth;
	int screenHeight;
	ERM_RENDER_DEVICE renderDevice;
	EDM_DISPLAY_MODE displayMode;
	bool disableTextures;

public:

    // Retrieve program settings
    void GetPrivateString( char *iniFileStr, char *sectionStr );
    // Save program settings
    void WritePrivateString( char *iniFileStr, char *sectionStr );

	// data specific members
    bool GetInvertMouse(){ return invertMouse; };
	void SetInvertMouse( bool value ){ invertMouse = value; };

    int GetScreenWidth(){ return screenWidth; };
	void SetScreenWidth( int value ){ screenWidth = value; };

    int GetScreenHeight(){ return screenHeight; };
	void SetScreenHeight( int value ){ screenHeight = value; };

    ERM_RENDER_DEVICE GetRenderDevice(){ return renderDevice; };
	void SetRenderDevice( ERM_RENDER_DEVICE value ){ renderDevice = value; };

    EDM_DISPLAY_MODE GetDisplayMode(){ return displayMode; };
	void SetDisplayMode( EDM_DISPLAY_MODE value ){ displayMode = value; };

	bool GetDisableTextures(){ return disableTextures; };
	void SetDisableTextures( bool value ){ disableTextures = value; };

    CSettings(){};
    ~CSettings(){};
};

typedef CSettings *PCSettings;


#endif  // __SSSETTINGS__