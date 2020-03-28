
/************************************************************************
*
*    PRODUCT:         Screen Saver Animation Base Class
*
*    FILE NAME:       SSThread.h
*
*    DESCRIPTION:     Thread that runs the screen saver
*
*    IMPLEMENTATION:  This thread is screen saver specific. Here is where
*                     you initilize your screen saver, run it and then
*                     free all allocated memory and resources. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __SSBASE_H__
#define __SSBASE_H__

#include "TCanvas.h"         // Canvas object for easy printing of text, lines boxes to the screen.
#include "SSWnd.h"           // The screen saver window header file.

#define DEFAULT_SCREEN_W              1024
#define DEFAULT_SCREEN_H              768

#define GET_NUM_MESSAGES              2
#define NEED_HIGHER_COLOR_MODE        50
#define HOW_TO_REG_MSG                100
#define HOW_TO_CHANGE_COLOR_SET       101
#define HOW_TO_CHANGE_HIGHER_COLOR    102
#define SHAREWARE_MESSAGE_SLEEP       5000
#define SHAREWARE_MESSAGE_DELAY       30000
#define SAVER_QUIT                    FALSE


// Thread function the will run and animate the screen saver
//unsigned long _stdcall ScreenSaverThread( void *ssData );


class CScreenSaver
{
protected:
    // Common Data Types
    PTSaverModeData pssData;
    // The color bit depth the computer is currently in
    int SystemBitDepth;
    // The rect of the window which could include more then one monitor
    RECT WindowRect;
    // The rect of the first monitor
    RECT ScreenRect;
    // The rect of the desktop which could include more then one monitor
    RECT DesktopRect;
    // Stock object black brush
    HBRUSH BlackBrush;
    // Error number counter
    int Error_Number;
    // Canvas object for text, line and fill functions
    TCanvas canvas;
    // Get the Width & Height of the screen of the first monitor only.
    // Use these values if your screen saver or parts of needs to be confined 
    // to the first monitor if more then one monitor is available.
    int ScreenWidth, ScreenHeight;
    // Get the Width & Height of the desktop which could include more then one monitor
    // Use these values if your screen saver can work across a desktop size than may be
    // more then one monitor in size. If more then one monitor is available, the desktop
    // size is a combination of the monitors
    int DesktopWidth, DesktopHeight;
    // The error message test
    char errorStr[300];

    // Width of the screen saver window
    int WindowWidth(){ return pssData->windowWidth; };
    // Height of the screen saver window
    int WindowHeight(){ return pssData->windowHeight; };
    // Mode: ssPreview or ssSaver
    int Mode(){ return pssData->SaverMode; };
    // Are we running more then one monitor?
    BOOL IsDuelMonitor(){ return pssData->duelMonitor; };
    // Instance of this screen saver
    HINSTANCE Instance(){ return pssData->hInst; };
    // Handle to the window
    HWND Handle(){ return pssData->hwnd; };
    // Did some one close the password window? Repaint the screen
    // because the password dialog box doesn't erase itself. If your
    // screen saver doesn't paint the whole screen when it is running,
    // use the rect this function passes and update that rect
    LPRECT GetPasswordWindowRect(){ return &pssData->PassRect; };
    // Sets the Active variable to false and kicks the screen saver out of the while loop
    void StopScreenSaver(){ pssData->Active = FALSE; };
    // Kill the screen saver if there was an error
    void ScreenSaverErrorAbortSaver();
    // Set the handle to the screen's DC
    void SetScreenDC( HDC ScreenDC ){ pssData->hScreenDC = ScreenDC; };
    // Get the handle to the screen DC
    HDC GetScreenDC(){ return pssData->hScreenDC; };
    // Rest the password rect to all zeros because we will be testing it later
    void ResetPasswordRect(){ SetRect( &pssData->PassRect, 0,0,0,0 ); };
    // General Start Up initilization
    BOOL LoadGeneralStartUpData();
    // Load project specific init code. Calls virtual load functions
    BOOL LoadSpecificStartUpData();
    // If the bottom value has been set, then we have a password dialog box to paint over
    BOOL PaintPasswordDialogRect(){ return pssData->PassRect.bottom; };
    // Draws the shareware screen
    BOOL DisplayShareWareScreen( HDC outputDC = NULL, int width = 0, int height = 0, int mainMsg = 0, int bottomInfoMsg = 0, BOOL timeOutScreen = TRUE );
    // Checks to see if they are using one of the temporary registration numbers
    BOOL ValidateTempRegNum( char *FileName, unsigned long CountMax );
    // Tells us if we need to repaint the whole screen
    BOOL RepaintScreen();
    // Init buffers, load graphics etc. This is just s virtual hook and should
    // only be called from it's inherited class.
    virtual BOOL LoadGraphics(){ return TRUE; };
    // Init buffers, load sounds etc. This is just s virtual hook and should
    // only be called from it's inherited class.
    virtual BOOL LoadSound(){ return TRUE; };
    // Get the user control panel settings. This is just s virtual hook and should
    // only be called from it's inherited class.
    virtual BOOL LoadControlPanelPrefs(){ return TRUE; };
    // Init screen saver data. This is just s virtual hook and should
    // only be called from it's inherited class.
    virtual BOOL InitSaverData(){ return TRUE; };
    // Init music player. This is just s virtual hook and should
    // only be called from it's inherited class.
    virtual void InitMusicPlayer(){};
    // Set Sound pause
    void SetSoundPause( BOOL Value ){ pssData->PauseSound = Value; };
    // Get Sound pause
    BOOL GetSoundPause(){ return pssData->PauseSound; };
    // Set the state of the screen resize flag
    void SetScreenResize( BOOL Value ){ pssData->ScreenResResized = Value; };
    // Get the state of the screen resize flag
    BOOL GetScreenResize(){ return pssData->ScreenResResized; };

public:
    // Common Functions

    // Constructor
    CScreenSaver( PTSaverModeData ssData );
    // Destructor - Put all the general cleanup code here
    ~CScreenSaver();
    // Container for screen saver init code
    BOOL InitilizeScreenSaver();
    // Check to see if the screen saver has ended
    BOOL SaverActive(){ return pssData->Active; };
    // Screen saver loading flag is set here
    void SetScreenSaverLoading( BOOL loading ){ pssData->Loading = loading; };
    // See if the monitor is asleep
    BOOL GetMonitorIsAsleep(){ return pssData->MonitorIsAsleep; };
    // First time display of the buffer to start the screen saver
    virtual void DisplayBufferOnStartUp(){};
	// Load the screen saver
	BOOL Load();
};

typedef CScreenSaver *PCScreenSaver;


#endif  // __SSBASE_H__