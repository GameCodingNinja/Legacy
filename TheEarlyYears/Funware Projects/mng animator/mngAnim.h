
/************************************************************************
*
*    PRODUCT:         MNG Animator
*
*    FILE NAME:       mngAnim.h
*
*    DESCRIPTION:     Program for outputing animation scripts for MNG files
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __MNG_ANIM_WND_H__
#define __MNG_ANIM_WND_H__

#include "mng.h"             // MNG header file
#include "TStoryBoardList.h" // List object for handling the list of animations
#include "Settings.h"        // Ini project settings file
#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "zOrderList.h"      // List object for storing indexes and Z Orders
#include "TStrList.h"        // List object for storing lists of strings

// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
// Callback timer function that plays the animation(s)
void CALLBACK PlayAnimTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime );


class CMNGanimWnd
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Handle to parent dialog window
    HWND hParentWnd;
    // MNG Class
    CMNG mng;
    // Ini file settings object
    TSettings Settings;
    // Subclassed window control handles
    HWND hSubCtrl[ MAX_SUBCLASS_CTRLS ];
    // Matching windows procedure for subclassed controls
    WNDPROC OldSubCtrlProc[ MAX_SUBCLASS_CTRLS ];
    // Preview buffer
    TBitmap ViewBuffer, PreviewBuffer, BackBuffer;
    // Preview window rect
    RECT PreviewRect, ViewRect;
    // Storyboard array
    TStoryListArry StoryLstAry;
    // The current active script
    int activeScript;
    // Mouse cursor offsets
    int MouseMoveX;
    int MouseMoveY;
    // Mouse button down flag
    BOOL MouseButtonDown;
    // Background has been loaded
    BOOL BackgroundLoaded;
    // Z Order list
    CzOrderList zOrderList;
    // Return code for the windows timer
    int TimerReturnCode;
    // List of mng paths
    TStringList mngPathList;
    // List of script paths
    TStringList scriptPathList;
    // List of scripta to animate
    TStringList scriptAnimList;
    // Flag that indicates we are to loop animations
    BOOL fLoopAnimations;
    // Holds the animation index with the largest frame count
    int animBiggestIndex;

    // Create the parent window
    BOOL CreateWnd( HINSTANCE hInstance, int nCmdShow );
    // Standard Windows OnClose message handler
    void QuitProgram( HWND hwnd );
    // Set the window caption
    void SetWindowCaption( HWND hwnd );
    // Gets all the file names via a suplied extension, in a give directory
    void GetAllFileNamesInDir( char *Dir, char *Ext, HWND hListCtrl );
    // Update the list boxes that show the file names
    void UpdateFileListBoxes( HWND hwnd );
    // Display the image resource
    void BlitImageToBuffer( PTDIB pDib, PTBitmap pBuffer, LPRECT pRect, HWND hStc_Ctrl=NULL, int count = 0 );
    // Update the view buffer
    void UpdateViewBuffer( HWND hwnd );
    // Blit the sprite to the view buffer
    void BlitSprite( PTDIB pDib, PTStoryBoard pStoryBoard, int baseX, int baseY, BOOL PaintOnly = FALSE );
    // Shows the selected image file
    void SelectAnimFrame( HWND hwnd, LPRECT rect );

    // Set the sound properties of the frame(s)
    void MnuClk_SetSoundProperties( HWND hwnd );
    // Save all the active scripts
    void MnuClk_SaveAllAciveScripts( HWND hwnd );
    // Open path to project
    void MnuClk_OpenProjPath( HWND hwnd );
    // Load a file to use as the background
    void MnuClk_LoadBackground( HWND hwnd );
    // Clear the background
    void MnuClk_ClearBackground( HWND hwnd );
    // Clear the sound properties of the frame(s)
    void MnuClk_ClearSoundProperties( HWND hwnd );
    // Clear the check mark for playing sounds
    void MnuClk_PlaySound( HWND hwnd );

    // Message handler for the preview window
    LRESULT OnPreviewWnd( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation frame X control
    LRESULT OnEdit_FrameXCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation frame Y control
    LRESULT OnEdit_FrameYCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation base Z control
    LRESULT OnEdit_BaseZCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation base X control
    LRESULT OnEdit_BaseXCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation base Y control
    LRESULT OnEdit_BaseYCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation frame count control
    LRESULT OnEdit_FrameCountCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the Z Order control
    LRESULT OnEdit_FrameZOrderCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the view window
    LRESULT OnViewWnd( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the animation frames list control
    LRESULT OnList_AnimFramesCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the script to animate list control
    LRESULT OnList_ScrptToAnimCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the script files list control
    LRESULT OnList_ScriptFilesCtrl( HWND hwnd, int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Click handler for the MNG file list
    void ListClk_MNGfileList( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Click handler for the MNG file frames list
    void ListClk_MNGfileFrames( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Click handler for the script file list
    void ListClk_ScriptLst( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Click handler for the frame list
    void ListClk_AnimFrameLst( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Click handler for the available MNG file frames list
    void ListClk_AvailableMNGFrames( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Click handler for the scripts to animate control
    void ListClk_ScrptToAnimLst( HWND hwnd, HWND hwndCtl, UINT codeNotify );
	// Click handler for the animation type control
	void ListClk_AnimTypeLst( HWND hwnd, HWND hwndCtl, UINT codeNotify );

    // Add MNG frame as animation frame
    void BtnClk_AddFrame( HWND hwnd );
    // Add an script to the list to animate
    void BtnClk_AddScriptToAnimate( HWND hwnd );
    // Create a script file
    void BtnClk_CreateScript( HWND hwnd );
    // Play the selected animations
    void BtnClk_PlayScripts( HWND hwnd );

public:
    // Constructor
    CMNGanimWnd();
    // Destructor
    ~CMNGanimWnd();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnSetFocus message handler
    void OnActivate( HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized );
    // Standard Windows OnNotify message handler
    LRESULT OnNotify( HWND hwnd, int idFrom, NMHDR FAR * pnmhdr );
    // Register and create the parent window
    BOOL CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Get a handle to the parent window
    HWND GetParentWnd(){ return hParentWnd; };
    // Called from the subclassed control
    LRESULT HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Play the selected animations
    void PlayAnimations( HWND hwnd );

    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef CMNGanimWnd *PCMNGanimWnd;


#endif  // __MNG_ANIM_WND_H__