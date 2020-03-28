
/************************************************************************
*
*    PRODUCT:         Example OpenGL 3D Windows API Program
*
*    FILE NAME:       Example.h
*
*    DESCRIPTION:     Show's how simple dealing with Windows really is 
*
*    Copyright(c) by Howard C Rosenorn    www.howiesfunware.com
*    All Rights Reserved
*
************************************************************************/


#ifndef __EXAMPLE_WND_H__
#define __EXAMPLE_WND_H__

#include "3D/CMapMgr.h"
#include "CZombie.h"
#include "3D/C3DWindow.h"		 // 3D window interface
#include "3D/CGraphList.h"      // Graph template class
#include "3D/CPathNode.h"       // Node class
#include "CPlayer.h"		 // player class
#include "settings.h"
#include "3D/CMapLightLst.h"

// Standard Windows message handler
LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );


class CExampleWnd
{
protected:

    // flag to indicate the game needs to stop
    bool gameRunning;
    // Debug string
    char debugStr[200];
	char debugExtra[200];
	// Debug disply counter
	int debugDisplayCounter;
    // Game Loop Timer
    INT64 gameLoopTimer;
	INT64 gameFrameRate;
    // Center point to judge position to
    POINT mouseCenterPt;
    // class for loading native map files
    CMapMgr<CMeshMap> map;
    // Texture library for map file
    CTextureLib mapTxtLib;
	// Texture library for objects file
	CTextureLib objectTxtLib;
    // light list class
    CLightList globalLightLst;
    // flag that indicates a move button is down
    int buttonUpDown;
	int buttonLeftRight;
	// camera
	CBaseCamera camera;
    // 3d window interface
	C3DWindow window3D;
	// Player class
	CPlayer player;
	// Graph class
	CGraphList <CPathNode>graph;
	// animated bot
	CZombie<CAnimMesh> zombieAnim;
	// animated map object
	CAnimMeshLst<CAnimMesh> mapObjects;
	// Y orientation
	float yOrientation;
	// current settings
	CSettings settings;
	// Map light list
	std::map<std::string, CLight *> lightLst;
	// Map light list class array
	std::vector<CMapLightList *> mapLightLst;

    // All windows created with CreateWindow must be registered first.
    // This does not include windows controls like buttons, listboxex and such
    // because they register themselves
    bool RegisterWnd( HINSTANCE hInstance );
    // Create the parent window
    bool CreateWnd( HINSTANCE hInstance, int nCmdShow );
    // Act upon what the user is doing
    void GetUserImput();
    // Render to the buffer
    void RenderOpenGL();
	void RenderDirectX();
	void RenderSoftware();
    // Start the game
    void StartGame();
    // Stop the game
    void StopGame();
    // React to the collision
    void ReactToCollision();
    
	// DisplayDebugInfo
	void DisplayDebugInfo();
	// Update animations, bot positions and so on
	void Update();
	// Render object like bots and 3D aniations
	void RenderObjects();

	// Load game related objects
    bool LoadGameObjects();
	// Game window init
	void GameWindowInit();
	// Load the software lights
	bool LoadSoftwareLights();
	// Load the map
	bool LoadMap();
	// Load the graph the bots will follow
	bool LoadBotGraph();
	// Load the map objects
	bool LoadMapObjects();
	// Load the bots
	bool LoadBots();
	// Setup the player
	bool SetupPlayer();
	// Setup the light data
	void SetLightData( CDataDefNode * pChild, TFloatVector &mapScale );

public:
    // Constructor
    CExampleWnd();
    // Destructor
    ~CExampleWnd();

    // Standard Windows OnCommand message handler
    void OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify );
    // Initilization function
    bool OnCreate( HWND hwnd, CREATESTRUCT* lpCreateStruct );
    // Standard Windows OnDestroy message handler
    void OnDestroy( HWND hwnd );
    // Register and create the parent window
    bool CreateParentWnd( HINSTANCE hInstance, int nCmdShow );
    // Game loop
    void GameLoop();
    // Shut down of game
    void GameShutDown( HWND hwnd );
	// Get the handle to the window
	HWND GetWndHandle(){ return window3D.GetWndHandle(); };
	// Get the time
	INT64 GetPerformanceTime();
};


#endif  // __EXAMPLE_WND_H__