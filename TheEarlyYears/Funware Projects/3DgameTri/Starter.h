
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

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps


/*******************************
*    3D Engine include files 
*******************************/

#include "CMeshTri.h"
#include "CMatrix.h"
#include "CTextLib.h"
#include "CMeshMgr.h"        // Mesh manager


// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

class CStarterWnd
{
protected:
    HINSTANCE hInst;
    HWND hParentWnd;

    // Image buffers
    TBitmap buffer;
    // Matrix class
    CMatrix matrixYZ, matrixX;
    // Bitmap texture library
    CTextureLib mapTxt;
    // Polygon object
    CMeshTri mapMsh[9];
    // Light list
    CLightList lightList;
    // Screen device contex
    HDC hScreenDC;
    // flag to indicate the game needs to stop
    BOOL gameRunning;
    // render buffer size
    int renderWidth, renderHeight;
    // Camera size
    int cameraSize;
    // Debug string
    char debugStr[200];
    // cycle counter
    unsigned int gameLoopCounter;
    // Game Loop Timer
    unsigned int gameLoopTimer;
    // Center point to judge position to
    POINT mouseCenterPt;
    // Buffer render time
    unsigned int bufferRenderTime;
    // flag that indicates a mouse button is down
    bool buttonDown;
    CMeshMgr mshMgr;  

    // All windows created with CreateWindow must be registered first.
    // This does not include windows controls like buttons, listboxex and such
    // because they register themselves
    BOOL RegisterWnd( HINSTANCE hInstance );
    // Create the parent window
    BOOL CreateWnd( HINSTANCE hInstance, int nCmdShow );
    // Act upon what the user is doing
    void GetUserImput();
    // Render to the buffer
    void RenderBuffer();
    // Start the game
    void StartGame();
    // Stop the game
    void StopGame();
	// React to the collision
    void ReactToCollision();

public:
    // Constructor
    CStarterWnd();
    // Destructor
    ~CStarterWnd();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    BOOL OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Standard Windows OnMouseMove message handler
    void OnMouseMove( HWND hwnd, int X, int Y, UINT keyFlags );
    // Register and create the parent window
    BOOL CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Game loop
    void GameLoop();
    // Startup Init
    BOOL GameInit();
    // Shut down of game
    void GameShutDown( HWND hwnd );
};

typedef CStarterWnd *PCStarterWnd;


#endif  // __STARTER_H__