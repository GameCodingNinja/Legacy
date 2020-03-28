/************************************************************************
*
*    PRODUCT:         Screen saver Window Class
*
*    FILE NAME:       SSWnd.h
*
*    DESCRIPTION:     Screen saver window
*
*    IMPLEMENTATION:  Designed to be used as a screen saver window. New
*                     projects would use this like a library and not
*                     alter the code unless adding a new feature 
*                     ( that is NOT screen saver specific ) that
*                     all the screen savers would use or making fixes.
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __SSWND_H__
#define __SSWND_H__

// defines
#define WINDOW_CLASS        "SaverClass"
#define WINDOW_TITLE        "Funware Screen Saver"
#define TICKS_TO_IGNORE     5
#define MINIMUM_MOUSE_MOVE  5
#define PREVIEW_WND         0
#define SAVER_WND           1
#define MOUSE_RESET_TIMER   3
#define ANIMATION_TIMER     5
#define ONE_SECOND          1000L
#define TWO_SECONDS         2000L
#define FIVE_SECONDS        5000L
#define TEN_SECONDS         10000L
#define TWENTY_SECONDS      20000L
#define DEBUG_WIDTH         320//213
#define DEBUG_HEIGHT        240//160
#define DEBUG_OFFSET        20
#define WM_STARUP_SS        (WM_USER + 10)

// Type define the function in the library we need to use
typedef VOID (WINAPI *PWDCHANGEPASSWORD)( LPCSTR lpcRegkeyname, HWND hwnd, UINT uiReserved1, UINT uiReserved2 );
// Create typedef of pointer to VerifyScreenSavePwd function
typedef BOOL (WINAPI *VERIFYSCREENSAVEPWD)(HWND hWnd);


// Enums
enum {
  ssNone=0,
  ssConfig,
  ssPassword,
  ssPreview,
  ssSaver,
};

struct SSaverModeData
{
    int windowWidth;        // Window width - Reflects the value depending on the mode of the saver 
    int windowHeight;       // Window hieght - Reflects the value depending on the mode of the saver
    int desktopWidth;       // Width of desktop that could include duel monitors
    int desktopHeight;      // Height of desktop that could include duel monitors
    int screenWidth;        // Width of the screen NOT including any duel monitors
    int screenHeight;       // Height of the screen NOT including any duel monitors
    int saverMode;          // Screen saver mode
    HINSTANCE hInst;        // This programs instance
    HWND hwnd;              // The handle the screen saver window
    bool active;            // Flag indicating that the screen saver is running
    bool loading;           // Flag indicating that the screen saver is doing it's startup loading
    HDC hScreenDC;          // Handle the the screen savers window DC
    RECT passRect;          // Rect of the password window used for erasing the password window
    bool duelMonitor;       // Flag that indicates if the computer is using more then one monitor
    bool monitorIsAsleep;   // Flag that indicates that the monitor has gown asleep
    bool repaintScreen;     // Used to tell the running screen saver that the whole screen needs to be repainted.
                            // Currently used for when the monitor comes back on when it was asleep.
    bool pauseSound;        // Pause the sound when the monitor has gown asleep
    bool screenResResized;  // Flag that indicates the screen saver has resized the users screen resolution
    HANDLE hSSThread;       // Handle to thread
    DWORD ssThreadID;       // not currently used for anything.
};
typedef SSaverModeData *PSSaverModeData;


// The main windows message window
LRESULT CALLBACK SaverWndProc( HWND, UINT, WPARAM, LPARAM );
// The preview windows message window. I seperated them to help keep things simple
LRESULT CALLBACK PreviewWndProc( HWND, UINT, WPARAM, LPARAM );
// Timer callback function for resting the mouseticks counter
void CALLBACK MouseResetTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime );
// Thread function the will run and animate the screen saver
unsigned long _stdcall ScreenSaverThread( void *pSSaver );

class CSSaverWnd
{
protected:
    // Structure holding all important data that needs to be passed on to the screen saver
    SSaverModeData ssData;
    // Mouse previous X
    int previousX;
    // Mouse previous X          
    int previousY;
    // Mouse nudge counts          
    int mouseTicks;
    // Return code for the windows timer to reset the mouse clicks         
    int timerReturnCode;
    // Flag to keep out additional windows messages while we are checking if we are to close the screen saver
    bool checkingForClose;

    // Register the window
    bool RegisterWnd( HINSTANCE hInst, int aSaverMode );
    // Create the windows
    HWND CreateWnd( HINSTANCE hInst, int aSaverMode, HWND aPreview );
    // Screen saver specific functions
    void RunPasswordMode( HWND hwnd );
    // Function checks to see if we're ready to quit the saver
    void MouseEvent( HWND hwnd );
    // Parse the command line to see what it is we are doing
    int ParseCommandLine( char *CmdLine, HWND *hwnd );
    // Did the user enter in a password that need to be checked?
    bool VerifyPassword( HWND hwnd );
    // Set the monitor is asleep flag
    void SetMonitorIsAsleep( bool Value ){ ssData.monitorIsAsleep = Value; };
    // See if the monitor is asleep
    bool GetMonitorIsAsleep(){ return ssData.monitorIsAsleep; };
    // Set the flag that tells the screen saver a full screen repaint is needed
    void SetRepaintScreen(){ ssData.repaintScreen = true; };

public:
    // Constructor
    CSSaverWnd();
    // Destructor
    ~CSSaverWnd();

    // Standard Windows OnCreate message handler
    bool OnCreate(HWND hwnd, CREATESTRUCT* lpCreateStruct);
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnButtonClick message handler
    void OnButton( HWND hwnd );
    // Standard Windows OnMouseMove message handler
    void OnMouseMove( HWND hwnd, int x, int y, UINT keyFlags );
    // Standard Windows OnClose message handler
    void OnClose( HWND hwnd );
    // Use onshow to hide the mouse
    void OnShowWindow( HWND hwnd, bool fShow, UINT status );
    // Standard Windows OnNCPaint message handler
    void OnNCPaint( HWND hwnd, HRGN hrgn );
    // Reset the nimber of mouse ticks to ignore
    void SetMouseTicks( int value ){ mouseTicks = value; };
    // Create the dialog box
    bool CreateParentWnd( HINSTANCE hInst, LPSTR lpszCmdParam );
    // Access function to this instance. Other parts of the program may need the instance
    HINSTANCE GetInstance(){ return ssData.hInst; };
    // Get pointer to screen saver data
    PSSaverModeData GetSSDataPtr(){ return &ssData; };
    // Wake up the sleeping screen saver
    void WakeSleepingScreenSaver();
    // Put screen saver to sleep
    void PutScreenSaverToSleep();
    // Check for password before closing
    void CloseIfOk( HWND hwnd );
    // Create the thread to run the screen saver
    bool CreateScreenSaverThread( void *pScreenSaver );
};

typedef CSSaverWnd *PCSSaverWnd;


#endif  /* __SSWND__ */

