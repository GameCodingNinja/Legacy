
/************************************************************************
*
*    PRODUCT:         Example OpenGL 3D Windows API Program
*
*    FILE NAME:       Example.cpp
*
*    DESCRIPTION:     Show's how simple dealing with Windows really is 
*
*    Copyright(c) by Howard C Rosenorn    www.howiesfunware.com
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "gl/gl.h"           // standard OpenGL include
#include "gl/glu.h"          // OpenGL utilities

#include "3D/CBaseCamera.h"	 // Camera class
#include "CPlayer.h"         // Player class

#include "Example.h"         // Header file for this *.cpp file.
#include "minTools.h"        // Misc tool functions

#include "assert.h"

#include "tagfileparser/datadefnode.h"
#include "tagfileparser/tokenizer.h"
#include "tagfileparser/tagfileparser.h"

#include "DoNothingDlg.h"    // The do nothing dialog box for example sake

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// Globals
CExampleWnd *pExampleWnd;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************                                                             
*    DESCRIPTION:           The entry point for all good windows programs
*
*    Input:    HINSTANCE hInst - This progams instance
*              HINSTANCE hPrevInstance - Of no value to 32 bit windows programs
*              LPSTR lpszCmdParam - The command line is sent by Windows. The command
*                                   originally has the path to this program but
*                                   Windows parses it out before it hands it over.
*                                   To get the command line with the path in it you
*                                   need to call GetCommandLine() API call.
*              int nCmdShow - Specifies how the window is to be shown.
*                             Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED   
*
*    Output:   int - Usually it's the last message wParam.
************************************************************************/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdParam, int nCmdShow )
{
    MSG msg;

    // Set wParam to 0 in case need to return right away.
    msg.wParam = 0;

    // Create the parent window object and set the pointer
    CExampleWnd ExampleWnd;
    pExampleWnd = &ExampleWnd;

    // Create the parent window
    if( ExampleWnd.CreateParentWnd( hInst, nCmdShow ) )
    {        
        // A game requires an infinite loop
        while( true )
        {
            // Check for waiting messages
            if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
                // Is it time to quit
                if( msg.message == WM_QUIT )
                    break;

                // Change the format of certain messages
                TranslateMessage( &msg );

                // Pass the message to WndProc() for processing
                DispatchMessage( &msg );
            }
            else
            {
                // Call the game loop
                ExampleWnd.GameLoop();
            }
        }
    }

    return msg.wParam;

}   // WinMain


/************************************************************************                                                             
*    DESCRIPTION:           Standard Windows message handler.
*                           We are using this to call our object functions
*
*    Input:    HWND hwnd      - Handle of the window receiving the message
*              UINT Message   - Message being sent 
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   returntype - LRESULT
************************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_DESTROY,   pExampleWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND,   pExampleWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,    pExampleWnd->OnCreate );
        default:
            return DefWindowProc( hwnd, Message, wParam, lParam );
    }

} // WndProc


/************************************************************************
*
*                      Window Object Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
************************************************************************/
CExampleWnd::CExampleWnd()
{
    // Init class members
    gameRunning = false;
    gameLoopTimer = 0;
    debugExtra[0] = 0;
	yOrientation = -1.0;
	debugDisplayCounter = 0;
	gameFrameRate = 0;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer
************************************************************************/
CExampleWnd::~CExampleWnd()
{
	DeleteVectorPointers( mapLightLst );
	DeleteMapPointers( lightLst );

}   // Destructer


/************************************************************************
*    DESCRIPTION:           Create the parent Window
*
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype bool - True or False depending on if successful
************************************************************************/
bool CExampleWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Register the parent window with Windows
    if( !RegisterWnd( hInstance ) )
        return false;

    // Create the parent window
    if( !CreateWnd( hInstance, nCmdShow ) )
        return false;

    return true;

}   // CreateParentWnd


/************************************************************************
*    DESCRIPTION:           All windows created with CreateWindow
*                           must be registered first. This does not include
*                           windows controls like buttons, listboxex and such
*                           because they register themselves.
*
*    Input:    HINSTANCE hInst - This progams instance
*
*    Output:   returntype bool - True or False depending on if successful
************************************************************************/
bool CExampleWnd::RegisterWnd( HINSTANCE hInstance )
{
    WNDCLASSEX w;

    w.cbSize = sizeof( WNDCLASSEX );
    w.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC|CS_BYTEALIGNCLIENT;
    w.lpfnWndProc = WndProc;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;
    w.hIcon = LoadIcon( hInstance, "AppIcon" );
    w.hIconSm = NULL;
    w.hCursor = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = NULL;
    w.lpszMenuName = NULL;
    w.lpszClassName = WINDOW_NAME;

    return ( RegisterClassEx(&w) != 0 );

}   // RegisterWnd 


/************************************************************************
*    DESCRIPTION:           Create the parent window
*
*    Input:    HINSTANCE hInst - This progams instance
*
*              int nCmdShow    - Specifies how the window is to be shown.
*                                Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*
*    Output:   returntype bool - True or False depending on if successful
************************************************************************/
bool CExampleWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{    
    Display_DoNothingDlg( hInstance, 0 );

	// read in the ini file
    settings.GetPrivateString( INI_FILE_NAME, "config" );

	// Are we to invert the mouse?
	if( settings.GetInvertMouse() )
	{
		yOrientation = 1.0;
	}

    // Create the 3D window and make sure we have a handle
    if( window3D.Create3DWind( settings.GetScreenWidth(),
    						   settings.GetScreenHeight(),
    						   hInstance,
                               settings.GetDisplayMode(), 
                               WINDOW_NAME,
                               WINDOW_TITLE ) == NULL )
    {
        return false;
    }

    // Create the 3D device
    if( window3D.Create3DDevice( settings.GetRenderDevice(),
    							 45.0f, 
                                 20.0f,
                                 4000 ) == false )
    {
        return false;
    }

    // Show The Window
    ShowWindow( window3D.GetWndHandle(), SW_SHOW );
    
    // Slightly Higher Priority                                     
    SetForegroundWindow( window3D.GetWndHandle() );
                                            
    // Make it the focus
    SetFocus( window3D.GetWndHandle() );

    // Load game related objects
    if( LoadGameObjects() == false )
        return false;

	// Update the camera view
    camera.UpdateView();

    return true;

}   // CreateWnd


/************************************************************************
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*
*    Input:    HWND hwnd      - Handle of the window receiving the message 
************************************************************************/
void CExampleWnd::OnDestroy( HWND hwnd )
{    
    GameShutDown( hwnd );
    
    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


/************************************************************************
*    DESCRIPTION:           Standard Windows OnCreate message handler.
*
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype bool - True or False depending on if successful
************************************************************************/
bool CExampleWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{    

    return true;

}   // Starter_OnCreate


/************************************************************************
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
************************************************************************/
void CExampleWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case MNU_EXIT:  
            DestroyWindow( hwnd );    
        break;
    }

}   // OnCommand


/************************************************************************
*    DESCRIPTION:           Start the game
************************************************************************/
void CExampleWnd::StartGame()
{
    if( !gameRunning )
    {
        gameLoopTimer = 0;
        gameRunning = true;
    }

}   // StartGame


/************************************************************************
*    DESCRIPTION:           Stop the game
************************************************************************/
void CExampleWnd::StopGame()
{
    gameRunning = false;
    ShowCursor( true );

}   // StopGame


/************************************************************************
*    DESCRIPTION:           Do some gl inits 
*
*    Output:   returntype bool - True or False depending on if successful
************************************************************************/
bool CExampleWnd::LoadGameObjects()
{
    bool result = false;
    
	// Misc game wind inits
	GameWindowInit();

    // Load the software lights
    if( !LoadSoftwareLights() )
		goto HANDLE_ERROR;

	if( !LoadBotGraph() )
		goto HANDLE_ERROR;

	if( !LoadMap() )
		goto HANDLE_ERROR;

    if( !LoadMapObjects() )
		goto HANDLE_ERROR;

	if( !LoadBots() )
		goto HANDLE_ERROR;

	if( !SetupPlayer() )
		goto HANDLE_ERROR;

    // Let it run
    gameRunning = true;

    result = true;

HANDLE_ERROR:;

    return result;

}   // LoadGameObjects


/************************************************************************
*    DESCRIPTION:   Game window init 
*
*    Output:   returntype bool - True or False depending on if successful
*
************************************************************************/
void CExampleWnd::GameWindowInit()
{
	RECT rect;
	
    // Seed the random number generator
    srand ( GetPerformanceTime() );

    // get the window rect
    GetWindowRect( window3D.GetWndHandle(), &rect );
    
    // Set the center point for the mouse for movement comparison
    mouseCenterPt.x = rect.left + ((rect.right - rect.left) / 2);
    mouseCenterPt.y = rect.top + ((rect.bottom - rect.top) / 2);

    // Hide the mouse
    ShowCursor( false );

    // Set the mouse position to the middle of the window
    SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

}	// GameWindowInit


/************************************************************************
*    DESCRIPTION:   Load the software lights 
*
*    Output:   returntype bool - True or False depending on if successful
*
************************************************************************/
bool CExampleWnd::LoadSoftwareLights()
{
	TFloatVector mapScale;
	TDataDefNodeVector globalLstData;
	TDataDefNodeVector areaLstData;
	TDataDefNodeVector lightLstData;
	std::string globalDynType;
	CDataDefNode * pChild;
	SDynamicType dynamicType;
	SObjectType objStrType;

	CDataDefNode * pTree = CTagFileParser::GetInstance().ParseFile( "config\\mapLight.cfg" );
    if( pTree == NULL )
		return false;

    pChild = pTree->GetChild("maplight");
    if( pChild == NULL )
		return false;

	// Get the map scale
	pChild->GetDataByName("mapScale", mapScale);

	// Retrieve the light list data.    
    if( !pTree->GetDataByName("lightList", lightLstData) )
		return false;

    if( lightLstData.size() == 0 )
		return false;

	// Load the light data from the light list
	for( TDataDefNodeVectorIter it = lightLstData.begin();
	     it != lightLstData.end();
	     ++it )
	{
		SetLightData( (*it), mapScale );	
	}	

	/////////////////////////
	//  Global Light List
	/////////////////////////

	pChild = pTree->GetChild("globalList");
    if( pChild == NULL )
		return false;

	// Get the dynamic type
	pChild->GetDataByName("dynamicType", globalDynType);

	// Retrieve all of the global light data.    
    if( !pChild->GetDataByName("lightIdList", globalLstData) )
		return false;

    if( globalLstData.size() == 0 )
		return false;

    // Set the light type
    globalLightLst.SetDynamicLightType( dynamicType.GetType(globalDynType) );

	// Load the light data for the global lights
	for( TDataDefNodeVectorIter it = globalLstData.begin();
	     it != globalLstData.end();
	     ++it )
	{
		std::string tmpStrId;

		// Get the string id of the light we are looking for
		(*it)->GetDataByName( "Id", tmpStrId );

		// Insert the light
		globalLightLst.InsertLight( lightLst[tmpStrId] );
	}
	
	// Retrieve all of the area light data.    
    if( !pTree->GetDataByName("areaList", areaLstData) )
		return false;

    if( areaLstData.size() == 0 )
		return false;
		
	// Load the light data for the area lights
	for( TDataDefNodeVectorIter itod = areaLstData.begin();
	     itod != areaLstData.end();
	     ++itod )
	{
		TDataDefNodeVector lightIdLstData;
		std::string objectTypeStr;
		std::string objectDynTypeStr;

		// Retrieve all of the area light data.    
	    if( !(*itod)->GetDataByName("lightIdList", lightIdLstData) )
			return false;

	    if( lightIdLstData.size() == 0 )
			return false;

		// New up a light list area
		mapLightLst.push_back( new CMapLightList() );

		(*itod)->GetDataByName("floorId", mapLightLst.back()->floorID);
		(*itod)->GetDataByName("mapId", mapLightLst.back()->mapID);
		(*itod)->GetDataByName("objectType", objectTypeStr);
		(*itod)->GetDataByName("dynamicType", objectDynTypeStr);
		mapLightLst.back()->objectType = objStrType.GetType( objectTypeStr );

		// Set the light type
    	mapLightLst.back()->SetDynamicLightType( dynamicType.GetType(objectDynTypeStr) );

		// Load the light data for the area lights
		for( TDataDefNodeVectorIter itld = lightIdLstData.begin();
		     itld != lightIdLstData.end();
		     ++itld )
		{
			std::string tmpStrId;

			// Get the string id of the light we are looking for
			(*itld)->GetDataByName( "Id", tmpStrId );

			// Insert the light
			mapLightLst.back()->InsertLight( lightLst[tmpStrId] );
		}
	}    

	return true;

}	// LoadSoftwareLights


/************************************************************************
*    DESCRIPTION:   Setup the light data
************************************************************************/
void CExampleWnd::SetLightData( CDataDefNode * pChild, TFloatVector &mapScale )
{
	SLightDirType lightTypeStr;
	SShadeType shadeTypeStr;
	std::string typeStr;
	std::string idStr;
	ELT_lightType lightType;
	EShade_Type shadeType;
	TFloatVector color;
	TFloatVector trans;
	float range(255), offset(0), radius(200);

	pChild->GetDataByName("Id", idStr);

	pChild->GetDataByName("lightType", typeStr);
	lightType = lightTypeStr.GetType(typeStr);

	pChild->GetDataByName("shadeType", typeStr);
	shadeType = shadeTypeStr.GetType(typeStr);

	pChild->GetDataByName("color", color);
	pChild->GetDataByName("trans", trans);
	pChild->GetDataByName("range", range);
	pChild->GetDataByName("offset", offset);
	pChild->GetDataByName("radius", radius);

	// Allocate the light
	CLight * pLight = new CLight;

	// Scale the radius
	if( radius != 0 )
		radius *= ((mapScale[0] + mapScale[1] + mapScale[2]) / 3) * radius;

	if( color.size() > 0 )
		pLight->SetColor( color[0], color[1], color[2] );

	if( trans.size() > 0 )
	{
		CMatrix tmpMatrix;
		CPoint mapScalePoint, transPoint;
		mapScalePoint.Init( mapScale );
		transPoint.Init( trans );
    	tmpMatrix.Scale( mapScalePoint );
		tmpMatrix.Transform( transPoint );
		transPoint.MakePermanent();

		pLight->SetLightPt( transPoint.x, transPoint.y, transPoint.z );
	}

	pLight->SetIntenRangeStart( range, offset );
	pLight->SetLightType( lightType, shadeType );
	pLight->SetSpotRadius( radius );

	lightLst.insert( make_pair(idStr, pLight) );

}	// SetLightData


/************************************************************************
*    DESCRIPTION:   Load the graph the bots will follow 
*
*    Output:   returntype bool - True or False depending on if successful
*
************************************************************************/
bool CExampleWnd::LoadBotGraph()
{
    /*if( !graph.LoadFromFile( "txt\\graph4.txt" ) )
	{
		PostMsg( "Error", "Unable to load graph file." );
        return false;
	}

	graph.Scale( 20, 20, 20 );*/

	return true;

}	// LoadBotGraph


/************************************************************************
*    DESCRIPTION:   Load the map 
*
*    Output:   returntype bool - True or False depending on if successful
*
************************************************************************/
bool CExampleWnd::LoadMap()
{
	vector<string> mapPolyStrLst;
	vector<string> mapTxtStrLst;

	// Get the file names and choos the first one. Needed durring development
	GetAllFileNamesInDir( mapPolyStrLst, "map", "*.map" );
	GetAllFileNamesInDir( mapTxtStrLst, "map", "*.bt" );
    
    // Load the map
	if( mapPolyStrLst.size() > 0 && !map.LoadFromFile( const_cast<char *>(mapPolyStrLst.back().c_str()) ) )
    {
        PostMsg( "Error", "Unable to load MAP file." );
        return false;
    }

	// Apply the texture to the map
	if( !settings.GetDisableTextures() )
	{
		// Set the mip mapping flag before loading
		mapTxtLib.SetMipMapping(true);
		
		// Load the texture library for this object into video memory
		if( mapTxtStrLst.size() > 0 && !mapTxtLib.LoadFromFile( const_cast<char *>(mapTxtStrLst.back().c_str()), 
			(ERM_RENDER_DEVICE)window3D.GetRenderDevice() ) )
		{
		    PostMsg( "Error", "Unable to load textures." );
		    return false;
		}

	    // Apply the texture library to the mesh class
	    map.SetTextureLibrary( mapTxtLib );
	}

	// Set the mesh specific light list
	map.SetLightsToMesh( mapLightLst );

	// Set the global light list
    map.SetLightList( &globalLightLst );

	// The map will now attach doors to rooms and link it all together.
	// This also copies the light list pointer to the area pointer.
	map.SetupMap();

	// Since all the lighting has all been done, clear it all.
	map.ClearMeshLights();

	// Attach nodes to rooms
    /*if( !map.AddGraphNodesToRooms( graph ) )
	{
        PostMsg( "Error", "Not all graph nodes found." );
        goto HANDLE_ERROR;
    }*/

	return true;

}	// LoadMap


/************************************************************************
*    DESCRIPTION:   Load the map objects 
*
*    Output:   returntype bool - True or False depending on if successful
*
************************************************************************/
bool CExampleWnd::LoadMapObjects()
{
    // Load the map objects
    if( !mapObjects.LoadFromFile( "objects\\objects.meshAnim" ) )
    {
        PostMsg( "Error", "Unable to load map objects file." );
        return false;
    }

	// Apply the texture to the map
	if( !settings.GetDisableTextures() )
	{
		// Set the mip mapping flag before loading
		objectTxtLib.SetMipMapping(true);
		
		// Load the texture library for this object into video memory
		if( !objectTxtLib.LoadFromFile( "objects\\objects.bt", 
			(ERM_RENDER_DEVICE)window3D.GetRenderDevice() ) )
		{
		    PostMsg( "Error", "Unable to load textures." );
		    return false;
		}

	    // Apply the texture library to the mesh class
	    mapObjects.SetTextureLibrary( objectTxtLib );
	}

	// Setup the tweans and shadow meshes
    mapObjects.SetUpTweens();
	mapObjects.SetUpShadows();

	// Make all the lights dynamic because the objects are all reused
	globalLightLst.SetDynamicLightType( EDL_WORLD_DYNAMIC_LIGHTNING );

	for( std::vector<CMapLightList *>::iterator it = mapLightLst.begin();
		     it != mapLightLst.end();
		     ++it )
	{
		(*it)->SetDynamicLightType( EDL_WORLD_DYNAMIC_LIGHTNING );
	}

    return map.SetObjsToRooms( "config\\mapObj.cfg",
						       mapObjects );

}	// LoadMapObjects


/************************************************************************
*    DESCRIPTION:   Load the bots 
*
*    Output:   returntype bool - True or False depending on if successful
************************************************************************/
bool CExampleWnd::LoadBots()
{

	// Load the bot
    /*if( !zombieAnim.LoadFromFile( "tmpZombie\\tmpZombie.meshAnim" ) )
    {
        PostMsg( "Error", "Unable to load PLY animation file." );
        goto HANDLE_ERROR;
    }

    zombieAnim.SetLightList( botLightList );
    zombieAnim.Scale( 0.5, 0.5, 0.5 );

	// This need to be the last call after everything has been setup
    zombieAnim.SetUpTweens();*/

    //zombieAnim.SetAnimation( "idle", 30 );
	//zombieAnim.SetAnimation( "walk", 10 );
	//zombieAnim.SetAnimation( "attack", 10 );

	// set object's offset and radius
	/*zombieAnim.Init( 30, 82 );

    if( !map.SetSpawnPoint( zombieAnim.GetMapType(),
							zombieAnim.GetWorldSpaceMatrix(), 
	                        EOT_MAP_BOT_SPAWN_POINT, 0, 1 ) )
	{
        PostMsg( "Error", "Bot spawn point not found." );
        goto HANDLE_ERROR;
    }*/

	// Find the node at the bots position
	//zombieAnim.FindPathNode( graph );

	// Set the state of the bot
	//zombieAnim.SetAIState( CZombie<CAnimMesh>::EZS_PERSUE_PLAYER, player );

	return true;

}	// LoadBots


/************************************************************************
*    DESCRIPTION:   Setup the player 
*
*    Output:   returntype bool - True or False depending on if successful
*
************************************************************************/
bool CExampleWnd::SetupPlayer()
{
	if( !player.LoadSpawnPoint( "config\\player.cfg",
								camera.GetWorldSpaceMatrix(),
								map ) )
	{
        PostMsg( "Error", "Player spawn point not found." );
        return false;
    }

	// Update the pos and direction based on the spawn point
	player.UpdatePosDir( camera.GetWorldSpaceMatrix() );

	// Find the closest path node of the player
	//player.FindClosestPathNode( map );

	return true;

}	// SetupPlayer


/************************************************************************
*    DESCRIPTION:           Shutdown the game
************************************************************************/
void CExampleWnd::GameShutDown( HWND hwnd )
{
    // Stop the game
    gameRunning = false;

    if( window3D.GetRenderDevice() == ERD_OPENGL )
    {
        // wait for any rendering to finish before we
        // start cleaning up.
        glFinish();
    }

    window3D.Free();

}   // GameShutDown


/************************************************************************
*    DESCRIPTION:           Call to excute the game loop
************************************************************************/
void CExampleWnd::GameLoop()
{
    if( gameRunning )
    {
         // Keep the speed consistant
         if( gameLoopTimer < GetPerformanceTime() )
         {
			 // allow it through every 10 miliseconds, 1000 / 25 fps = 40 mil
			 gameLoopTimer = GetPerformanceTime() + 10;

             // Get user imput
             GetUserImput();

             // React to collision
             ReactToCollision();

			 // Update animations, bot positions and so on
			 Update();

             // Render and show it
             if( window3D.GetRenderDevice() == ERD_SOFTWARE )
             {
                RenderSoftware();
             }
             else if( window3D.GetRenderDevice() == ERD_DIRECTX )
             {
                RenderDirectX();
             }
             else if( window3D.GetRenderDevice() == ERD_OPENGL )
             {
                 RenderOpenGL();
             }

			 DisplayDebugInfo();
         }
    }
    else
    {
        // See if we want to start again
        if( HIBYTE( GetAsyncKeyState( VK_HOME ) ) )
        {
            StartGame();    
        }
    }
}   // GameLoop


/************************************************************************                                                            
*    DESCRIPTION:    Update animations, bot positions and so on
************************************************************************/
void CExampleWnd::Update()
{
	// Update the camera view
    camera.UpdateView();

	player.Update( camera.GetWorldSpaceMatrix(), map );

}	// Update


/************************************************************************
*    DESCRIPTION:           Display the debug info
************************************************************************/
void CExampleWnd::DisplayDebugInfo()
{
	// How long did it take to render?
	++debugDisplayCounter;

	if( debugDisplayCounter == 5 )
	{
		sprintf( debugStr, 
				 " fps: %2.1f;    %s", 
				 ((float)debugDisplayCounter / ((float)GetPerformanceTime() - (float)gameFrameRate)) / 0.001f,
				 debugExtra );

        gameFrameRate = GetPerformanceTime();

        //sprintf( debugStr, "%s", debugExtra );

		//sprintf( debugStr, "%f,    %f,    %f", 
		//                   player.GetPos().tx, player.GetPos().ty, player.GetPos().tz );//,
		//                   zombieAnim.GetPos().tx, zombieAnim.GetPos().ty, zombieAnim.GetPos().tz );
		//sprintf( debugStr, "%f,    %f,    %f", zombieAnim.GetPos().tx, zombieAnim.GetPos().ty, zombieAnim.GetPos().tz );
		//sprintf( debugStr, "%f", zombieAnim.tmpdir );

        /*sprintf( debugStr, "posx %f,  posz %f,  dirx %f,  dirz %f,  plyx %f,  plyz %f,  rot %f",
		         zombieAnim.GetPos().tx, zombieAnim.GetPos().tz,
				 zombieAnim.GetDir().tx, zombieAnim.GetDir().tz,
				 player.GetPos().tx, player.GetPos().tz,
				 zombieAnim.GetPos().tz - player.GetPos().tz );*/
				 //zombieAnim.GetTmpRot() );

		window3D.SetWindowCaption( debugStr );

		debugDisplayCounter = 0;
	}

}	// DisplayDebugInfo


/************************************************************************
*    DESCRIPTION:           Act upon what the user is doing
************************************************************************/
void CExampleWnd::GetUserImput()
{
    short int keyState;
    POINT mousePt;
    float transX = 0;
    float transZ = 0;
    buttonUpDown = 0;
    buttonLeftRight = 0;

    // See if we want to escape out
    keyState = GetAsyncKeyState( VK_ESCAPE );
    if( HIBYTE( keyState ) )
    {
        if( gameRunning )
            StopGame();

        PostMessage( window3D.GetWndHandle(), WM_DESTROY, 0, 0 );

        return;    
    }

    keyState = GetAsyncKeyState( VK_UP );
    if( HIBYTE( keyState ) )
    {
        transZ = player.GetMoveSpeed();
        buttonUpDown = -1;
    }

    keyState = GetAsyncKeyState( VK_DOWN );
    if( HIBYTE( keyState ) )
    {
        transZ = -player.GetMoveSpeed();
        buttonUpDown = 1;
    }

    keyState = GetAsyncKeyState( VK_LEFT );
    if( HIBYTE( keyState ) )
    {
        transX = player.GetStrafeSpeed();
        buttonLeftRight = -1;
    }

    keyState = GetAsyncKeyState( VK_RIGHT );
    if( HIBYTE( keyState ) )
    {
        transX = -player.GetStrafeSpeed();
        buttonLeftRight = 1;
    }

    // Get the mouse X & Y position
    GetCursorPos( &mousePt );

    // Reset the cursor positions
    SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );
    
    // Add in the translation
	camera.Translate( transX, player.GetGravity(), transZ );

    // Do the Y rotation. We are using the difference between the center and the new offsets
	camera.Rotate( (((float)mouseCenterPt.y - (float)mousePt.y)/5.5) * yOrientation, 
				   -(((float)mouseCenterPt.x - (float)mousePt.x)/5.5), 0 );

	// We need the opposite rotation to get a model view point
	player.SetOppositeRotate( 0, ((float)mouseCenterPt.x - (float)mousePt.x)/5.5, 0 );    

}   // GetUserImput


/************************************************************************
*    DESCRIPTION:           Check for collision and react to it. I feel
*                           that collision reaction is best when it is
*                           handled outside the engine because every
*                           situation is different. Let the engine indicate
*                           it has hit something, but that is it.
************************************************************************/
void CExampleWnd::ReactToCollision()
{
    map.ReactToCollision( player.GetMapType(),
    					  camera.GetWorldSpaceMatrix(),
						  player.GetRadius(),
						  player.GetDistFromFloor(),
						  player.GetDistFromWall(),
    					  buttonUpDown,
    					  buttonLeftRight );

}   // ReactToCollision


/************************************************************************
*    DESCRIPTION:  Render to the OpenGL device
************************************************************************/
void CExampleWnd::RenderOpenGL()
{    
    // Clear the depth buffer
    glClear( GL_DEPTH_BUFFER_BIT );//| GL_COLOR_BUFFER_BIT );

    // update and render map objects
	RenderObjects();

    // show what we just rendered
    SwapBuffers( window3D.GetDCHandle() );

}   // RenderOpenGL


/************************************************************************
*    DESCRIPTION:  Render to the DirectX device
************************************************************************/
void CExampleWnd::RenderDirectX()
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)
    
    // Clear the depth buffer
    window3D.GetDirectXDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER,
                                        D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
    
    // May the rendering begin
    window3D.GetDirectXDevice()->BeginScene();

    // update and render map objects
	RenderObjects();
    
    // show what we just rendered
    window3D.GetDirectXDevice()->EndScene();
    window3D.GetDirectXDevice()->Present( NULL, NULL, NULL, NULL );

    #endif

}   // RenderDirectX


/************************************************************************
*    DESCRIPTION:  Render to the software device
************************************************************************/
void CExampleWnd::RenderSoftware()
{
    // clear the z buffer
    memsetQuad( window3D.GetZBufferBits(), 0, window3D.GetWidth() * window3D.GetHeight() );

    // Clear the screen if we are not rendering with textures
    if( mapTxtLib.Count() == 0 || settings.GetDisableTextures() )
	{	    
	    #ifdef _16_BIT_SOFTWARE_RENDERING_
	    memsetQuad( wi	ndow3D.GetBufferBits(), 0, (window3D.GetWidth() >> 1) * window3D.GetHeight() );
	    #else
	    memsetQuad( window3D.GetBufferBits(), 0, window3D.GetWidth() * window3D.GetHeight() );
	    #endif
	}

    // update and render map objects
	RenderObjects();

    window3D.DrawSoftDevice();

}   // RenderSoftware


/************************************************************************
*    DESCRIPTION:  Render object like bots and 3D aniations
************************************************************************/
void CExampleWnd::RenderObjects()
{
	// Render the map based on the position of the player
    map.Render( player.GetMapType(), camera.GetViewSpaceMatrix(), debugStr );

	// Update the object's position in the world based on what part of the map rendered
	//zombieAnim.Update( map, graph, player );

	// This will only render if the area the bot is in was rendered and if the
	// bot can be seen.
	int charCount = 0;//zombieAnim.Render( map, player.GetMapType(), camera.GetViewSpaceMatrix() );

	// Add in the character animation count
	sprintf( debugExtra, "%s,  anim char %d", debugStr, charCount );

}	// RenderObjects


/************************************************************************
*    DESCRIPTION:          Get the time 
*
*    Output:   INT64 - returns the millisecond time
************************************************************************/
INT64 CExampleWnd::GetPerformanceTime()
{
	static INT64 tmpFrenquency = 0;
	static BOOL hiRezTimerSupported = true;
	INT64 tmpTimer;
	INT64 result;
	
	// If the high res timer is not supported, use the windows timer.
	if( hiRezTimerSupported )
	{
		if( tmpFrenquency == 0 )
		{
			hiRezTimerSupported = QueryPerformanceFrequency( (LARGE_INTEGER*)&tmpFrenquency );

			if( !hiRezTimerSupported || tmpFrenquency == 0 )
			{
				hiRezTimerSupported = false;
			}

			// convert to milliseconds
			tmpFrenquency /= 1000; 
		}

		QueryPerformanceCounter( (LARGE_INTEGER*)&tmpTimer );

		result = tmpTimer / tmpFrenquency;
	}
	else
	{
	   result = timeGetTime();
	}

	return result;
		
}  // GetPerformanceTime