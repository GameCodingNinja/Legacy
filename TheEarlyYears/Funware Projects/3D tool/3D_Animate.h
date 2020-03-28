
/************************************************************************
*
*    PRODUCT:         Starter
*
*    FILE NAME:       Starter.h
*
*    DESCRIPTION:     The begining
*
*    IMPLEMENTATION: How it's done 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __STARTER_H__
#define __STARTER_H__

#include "Settings.h"        // Save the user settings
#include "TStrList.h"        // Object file for holding lists of strings
#include "TPolyLst.h"        // Header file for polygon list object
#include "TBitmap.h"         // Class for loading bitmaps
#include "TCanvas.h"         // Canvas object for easy printing of text, lines boxes to the screen.
#include "TIntList.h"        // List object for storing lists of ints

/*******************************
*    3D Engine include files 
*******************************/

#include "TextType.h"
#include "PalShade.h"
#include "PolyObj.h"


// Make a structure that is 23 bits in size and cut
// it up into 2/3 5 bit chunks for 16 bit RGB color.
struct SDWordRGB
{
    BYTE B1 : 5;
    BYTE G1 : 5;
    BYTE R1 : 5;
    BYTE NotUsed1 :1;
    BYTE B2 : 5;
    BYTE G2 : 5;
    BYTE R2 : 5;
    BYTE NotUsed2 :1;
};
typedef SDWordRGB *PSDWordRGB;


// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
// Call back for subclassing any of the controls procedure
LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
// Callback timer function that plays the animation(s)
void CALLBACK PlayAnimTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime );


class C3DAnimator
{
protected:
    // Program instance
    HINSTANCE hInst;
    // Parent window
    HWND hParentWnd;
    // Program settings
    TSettings Settings;
    // Camera 
    TCamera CameraView;
    // Inc camera steps
    TCamera cameraStep;
    // Matrix object
    Matrix3D CameraMatrix;
    // Z Buffer
    int *ZBuffer;
    // Video buffer
    TBitmap VBuffer;    
    // Handle to thread animating map
    HANDLE hThread;
    // Thread running flag
    BOOL ThreadRunning;
    // Index of the selected polygon
    TIntList SelectedPlyLst;
    // Thread fall through
    BOOL ThreadActive;
    // View window Rect
    RECT ViewRect;
    // Flip flop counter
    BOOL ZBufferFlipFlopCounter;
    // Flip flop flag
    BOOL FlipFlopZBuffer;
    // Subclassed window control handles
    HWND hSubCtrl[ MAX_SUBCLASS_CTRLS ];
    // Matching windows procedure for subclassed controls
    WNDPROC OldSubCtrlProc[ MAX_SUBCLASS_CTRLS ];
    // Object list for the allocated objects
    TPolygonList PlyList;
    // Camera list of offsets
    TCameraList CameraList;
    // Flag to indicate if a camera list needs to saved
    BOOL CameraListSave;
    // Camera list file name
    char CameraListFileName[MAX_PATH+1];
    // Mouse button down flag
    BOOL MouseButtonDown;
    // The state of the mouse move. Translation or rotation
    int MouseMoveState;
    // Indicates the active index
    int ActiveIndex;
    // Mouse move variables for tracking movement
    int MouseMoveX, MouseMoveY;
    // Texture view window buffer
    TBitmap textPrevBuf, loadedBitmap;
    // Path to app exe
    char AppPath[MAX_PATH+1];
    // the current system bit depth
    int SystemBitDepth;
    // Display DC
    HDC hViewWndDC;
    // Easy blitting of lines, fills and so on
    TCanvas canvas;
    // Light source polygon
    TPolyObj LightPoly;
    // Texture for the light polygon box
    ImageData LightTxt;
    // Return code for the windows timer
    int TimerReturnCode;
    // integer list classes
    TIntList intList;
    // The Camera frame counter
    int CameraFrameCounter;
    // Falg that indicates if we are doing an acumlative camera move
    BOOL DoIncCameraMove;
    // Timer play averages
    DWORD SlowestTime, FastestTime, TimerCounter, TotalRenderTime;
    // Render Flag
    BOOL Rendering;      

    // Create the parent window
    BOOL CreateWnd( HINSTANCE hInstance, int nCmdShow );
    // One quick and easy place to quit the program.
    void QuitProgram( HWND hwnd );
    // Open camera movement file
    BOOL OpenCameraMoveFile( HWND hwnd );
    // Open object movement file
    BOOL OpenObjMoveFile( HWND hwnd );
    // See if we need to save any of the polygon object data
    BOOL CheckToSaveObjectData( HWND hwnd, int Index );
    // See if we need to save the polygon object texture data
    BOOL CheckToSaveObjCDR( HWND hwnd, int Index );
    // See if we need to save the polygon object move list
    BOOL CheckToSaveObjMove( HWND hwnd, int Index );
    // See if we need to save the camera movements
    BOOL CheckToSaveCameraMove( HWND hwnd );
	// See if we need to save the polygon object data
	BOOL CheckToSavePlyObj( HWND hwnd, int Index );
    // Save texture coordinate fil
    BOOL SaveCRDfile( HWND hwnd, int Index );
    // See if we need to save the polygon texture library
    BOOL CheckToSaveObjTxtLibrary( HWND hwnd, int Index );
    // Save coordinate animation file
    BOOL SaveObjectMoveFile( HWND hwnd, int Index );
    // Save camera coordinate animation file
    BOOL SaveCameraMoveFile( HWND hwnd );
	// Save the polygon object file
	BOOL SavePlyObjfile( HWND hwnd, int Index );
    // Setup the polyObject
    BOOL LoadPolygonObject( HWND hwnd, char *ProjectName );
    // Render the scene
    void RenderScene();
    // Render the polygon objects
    int Render( int Index, PTCameraOffset pObjCamera );
    // Force a repaint onto the view window
    void RepaintViewWnd();
    // Load the texture INI file to display the different file names in the list
    void DisplayTextureLstFile();
    // Update texture controls with selected polygon info.
    void UpdateSelectedPolyInfo( HWND hwnd, int ListIndex );
    // Enable or disable controls
    void EnableControls( HWND hwnd, BOOL Enable );
    // Load the texture library for this polygon object
    BOOL LoadTextureLibrary( HWND hwnd, int PolyObjIndex );
    // Build the texture library for this polygon object
    BOOL BuildTextureLibrary( HWND hwnd, int PolyObjIndex );
    // Save the texture library for this polygon object
    BOOL SaveTextureLibrary( HWND hwnd, int PolyObjIndex );
    // Init the video buffer
    BOOL InitVideoBuffer();
    // Load the texture coordinates file. This file holds information about each polygon
    void LoadTextureCoordinates( char *ProjectPath );
    // Load the polygon object file or load the DXF file and convert it to our file format
    BOOL LoadPlyObjFile( char *ProjectPath );
    // Check the texture to see that it is valid
    BOOL CheckTexture( char *FileName );
    // Clear the video buffer
    void ClearVideoBuffer();
    // The light source polygon object
    void SetupLightSourcePoly();

    // Subclassed static control
    LRESULT OnViewWnd( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Subclassed static control
    LRESULT OnPreviewWnd( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // View window paint handler
    void ViewWnd_OnPaint( HWND hwnd );
    // View window MouseMove handler
    void ViewWnd_OnMouseMove( HWND hwnd, int X, int Y, UINT keyFlags );
    // View window LButtonDown handler
    void ViewWnd_OnLButtonDown( HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags );
    // View window RButtonDown handler
    void ViewWnd_OnRButtonDown( HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags );
    // View window LButtonUp handler
    void ViewWnd_OnLButtonUp( HWND hwnd, int X, int Y, UINT keyFlags );
    // Standard Windows OnMouseWheel message handler
    int ViewWnd_OnMouseWheel( HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys );

    // Message handler for the polygon list
    LRESULT OnList_PolyObjs( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the camera offset animation list
    LRESULT OnList_CameraAnim( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the object offset animation list
    LRESULT OnList_ObjOffsetAnim( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for the object offset animation list
    LRESULT OnList_MOVEMENT( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Message handler for camera edit controls
    LRESULT OnEdit_Offsets( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler for camera size edit controls
    LRESULT OnEdit_CameraSize( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Message handler max Z distance edit controls
    LRESULT OnEdit_MaxDistance( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam );

    // Handle all the polygon object list tasks
    void LstClk_PolyObjListBox( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Reset the offsets to the selected index
    void LstClk_ObjectMove( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Reset the offsets to the selected index
    void LstClk_CameraMove( HWND hwnd, HWND hwndCtl, UINT codeNotify );
    // Load the texture
    void LstClk_LoadSelectedTexture( HWND hwnd, HWND hwndCtl, UINT codeNotify );

    // Record the current object offset
    void BtnClk_RecordObjectOffset( HWND hwnd );
    // Change the current object offset
    void BtnClk_ChangeObjectOffset( HWND hwnd );
    // Record the current object offset information
    void BtnClk_RecordCameraOffset( HWND hwnd );
    // Change the selected camera offset
    void BtnClk_ChangeCameraOffset( HWND hwnd );
    // Enable or disable the texture controls
    void BtnClk_EnableDisableTextureControls( HWND hwnd );
    // Play the selected object movements
    void BtnClk_PlayMovements( HWND hwnd );
    // Turn on or off trails for plotting path
    void BtnClk_EnableDisableTrails( HWND hwnd, HWND hwndCtl );
    // Enable rotating from a given point
    void BtnClk_EnableDisableRotatePt( HWND hwnd, HWND hwndCtl );
    // Moves the directions by clicking on the buttons
    void BtnClk_DirectBtns( HWND hwnd, int MoveType );
    // Rotate the texture to the selected polygon
    void BtnClk_RotateTexture();
    // Apply the texture to the selected polygon
    void BtnClk_ApplyTexture( HWND hwnd, int TextIndex );
    // Enable / disable wire frame rendering
    void BtnClk_EnableDisableWireFrame( HWND hwnd, HWND hwndCtl );
    // Add the polygon object movement to the list
    void BtnClk_AddMoveToAnimate( HWND hwnd );
    // Use object for light source information
    void BtnClk_EnableDisableAsLightSource( HWND hwnd, HWND hwndCtl );

    // Move texture in texture list
    void MnuClk_MoveTexture( HWND hwnd );
    // Delete texture in texture list
    void MnuClk_DeleteTexture( HWND hwnd );
    // Save Everything
    void MnuClk_SaveAll( HWND hwnd );
    // Export a trimmed down camera or movement list
    void MnuClk_ExportList( HWND hwnd, int ExportType );
    // rebuild the texture library
    void MnuClk_RebuildTextureLibrary( HWND hwnd );
    // Open polygon objects to edit
    BOOL MnuClk_OpenPolygonObj( HWND hwnd );
    // Close out the selected object
    void MnuClk_CloseObject( HWND hwnd );
    // Select all textures
    void MnuClk_SelectAll( HWND hwnd );
	// Select all facing polygons
	void MnuClk_SelectAllFacingPolys( HWND hwnd );
	// Delete all the unused and dead textures
	void MnuClk_DeleteDeadTextures( HWND hwnd );
	// Sort all the polygons on Z order
	void MnuClk_SortOnZOrder( HWND hwnd );
    // Add a texture to the polygon library
    BOOL MnuClk_AddTesture( HWND hwnd );
	// Apply each texture to the corrisponding polygon index
	void MnuClk_1_To_1_TextureToPolyRatio( HWND hwnd );
	// Recalculate the lighting
	void MnuClk_RecalcVertexLighting( HWND hwnd );

public:
    // Constructor
    C3DAnimator();
    // Destructor
    ~C3DAnimator();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Register and create the parent window
    BOOL CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Get a handle to the parent window
    HWND GetParentWnd(){ return hParentWnd; };
    // Handles the subclassing of controls. Check the window handle 
    // to see which control is being trapped.
    LRESULT HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam );
    // Play the selected movements.
    void TimerPlayMovements( HWND hwnd );
    // Delete ourselves
    // This is the last message sent which is why we use it for deleting the object 
    void OnNCDestroy( HWND hwnd ){ delete this; };
};

typedef C3DAnimator *PC3DAnimator;


#endif  // __STARTER_H__