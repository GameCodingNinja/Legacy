
/************************************************************************
*
*    PRODUCT:         Starter
*
*    FILE NAME:       Starter.cpp
*
*    DESCRIPTION:     The begining of all great API windows programs
*
*    IMPLEMENTATION: How it's done 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "Starter.h"         // Header file for this *.cpp file.
#include "gTools.h"          // A misc group of useful functions

// Globals
PCStarterWnd pStarterWnd;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         WinMain                                                             
*
*    DESCRIPTION:           The entry point for all good windows programs
*
*    FUNCTION PARAMETERS:
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
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdParam, int nCmdShow )
{
    MSG msg;

    // Set wParam to 0 in case need to return right away.
    msg.wParam = 0;

    // Create the parent window object
    CStarterWnd StarterWnd;
    pStarterWnd = &StarterWnd;

    // Create the parent window
    if( StarterWnd.CreateParentWnd( hInst, nCmdShow ) )
    {
        // Do some startup Inits
        if( StarterWnd.GameInit() )
        {
            // A game requires an infinite loop
            while( TRUE )
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
                    StarterWnd.GameLoop();
                }
            }
        }
    }

    return msg.wParam;

}   // WinMain


/************************************************************************
*    FUNCTION NAME:         WndProc                                                             
*
*    DESCRIPTION:           Standard Windows message handler.
*                           We are using this to call our object functions
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*              UINT Message   - Message being sent 
*              WPARAM wParam  - Additional information included with message
*              LPARAM lParam  - Additional information included with message
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hwnd, WM_DESTROY,   pStarterWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_COMMAND,   pStarterWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_CREATE,    pStarterWnd->OnCreate );
        HANDLE_MSG( hwnd, WM_MOUSEMOVE, pStarterWnd->OnMouseMove );
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
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CStarterWnd::CStarterWnd()
{
    // Init class members
    hScreenDC = NULL;
    gameRunning = FALSE;
    shadedTbl = NULL;
    ZBuffer = NULL;
    gameLoopCounter = 0;
    gameLoopTimer = 0;
	bufferRenderTime = 0;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CStarterWnd::~CStarterWnd()
{
}   // Destructer



/************************************************************************
*    FUNCTION NAME:         CreateParentWnd()                                                             
*
*    DESCRIPTION:           Create the parent Window
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CStarterWnd::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Save the program instance
    hInst = hInstance;

    // Register the parent window with Windows
    if( !RegisterWnd( hInst ) )
        return FALSE;

    // Create the parent window
    if( !CreateWnd( hInst, nCmdShow ) )
        return FALSE;

    return TRUE;

}   // CreateParentWnd


/************************************************************************
*    FUNCTION NAME:         RegisterWnd                                                             
*
*    DESCRIPTION:           All windows created with CreateWindow
*                           must be registered first. This does not include
*                           windows controls like buttons, listboxex and such
*                           because they register themselves.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*    Output:   BOOL            - Returns TRUE or FALSE to indicate if
*                                the task was secessful.
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CStarterWnd::RegisterWnd( HINSTANCE hInstance )
{
    WNDCLASSEX w;

    w.cbSize = sizeof( WNDCLASSEX );
    w.style = CS_HREDRAW|CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;
    w.hIcon = LoadIcon( hInst, "AppIcon" );
    w.hIconSm = NULL;
    w.hCursor = LoadCursor( NULL, IDC_ARROW );
    w.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    w.lpszMenuName = "Menu_1";
    w.lpszClassName = WINDOW_NAME;

    return ( RegisterClassEx(&w) != 0 );

}   // Starter_RegisterWnd 


/************************************************************************
*    FUNCTION NAME:         CreateWnd                                                             
*
*    DESCRIPTION:           Create the parent window
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progams instance
*
*              int nCmdShow    - Specifies how the window is to be shown.
*                                Example: SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
*
*    Output:   HWND - A handle to the newly created window
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CStarterWnd::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window & save the window handle
    hParentWnd = CreateWindow( WINDOW_NAME, WINDOW_TITLE, WS_OVERLAPPEDWINDOW|WS_VISIBLE|CS_OWNDC,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

    // Make sure we have a handle to the window
    if( hParentWnd == NULL )
        return FALSE;

    return TRUE;

}   // CreateWnd


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::OnDestroy( HWND hwnd )
{
    // Unload the game
    GameShutDown( hwnd );
    
    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


/************************************************************************
*    FUNCTION NAME:         OnCreate()                                                             
*
*    DESCRIPTION:           Standard Windows OnCreate message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message
*      LPCREATESTRUCT lpCreateStruct  - Structure used in window creation 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CStarterWnd::OnCreate( HWND hwnd, LPCREATESTRUCT lpCreateStruct )
{
    // Center this window on the screen
    CenterWindow( hwnd, 820, 650 );

    return TRUE;

}   // Starter_OnCreate


/************************************************************************
*    FUNCTION NAME:         OnCommand                                                             
*
*    DESCRIPTION:           Standard Windows OnCommand message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              int id          - Menu ID of the of the control.
*              HWND hwndCtl    - Handle of the control receiving the message
*              UINT codeNotify - The action taking place
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    switch( id )
    {
        case MNU_START:
        	StartGame();            
        break;

        case MNU_EXIT:
            // Destroy the parent window
            DestroyWindow( hwnd );
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         StartGame                                                             
*
*    DESCRIPTION:           Start the game
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::StartGame()
{
	RECT rect;

	gameLoopTimer = 0;
    gameRunning = TRUE;

	// get the window rect
	GetWindowRect( hParentWnd, &rect );

	// confine the mouse to the window rect because
	// it will only remain hidden if it is inside our window
	//ClipCursor( &rect );
	
	// Set the center point for the mouse for movement comparison
	mouseCenterPt.x = rect.left + ((rect.right - rect.left) / 2);
	mouseCenterPt.y = rect.top + ((rect.bottom - rect.top) / 2);

	// Hide the mouse
	ShowCursor( false );

	// Set the mouse position to the middle of the window
	SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

}	// StartGame


/************************************************************************
*    FUNCTION NAME:         StopGame                                                             
*
*    DESCRIPTION:           Stop the game
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::StopGame()
{
	gameRunning = FALSE;
    //ClipCursor( NULL );
	ShowCursor( true );

}	// StopGame


/************************************************************************
*    FUNCTION NAME:         GameInit                                                             
*
*    DESCRIPTION:           Do some game startup inits
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CStarterWnd::GameInit()
{
    BOOL result = FALSE;

    // temp
    renderWidth = 800;
    renderHeight = 600;
    cameraSize = 320;
    // 320, 400

    // Get the device context to the main screen
    hScreenDC = GetDC( hParentWnd );

    // One time initilization of the cos and sin tables
    // Makes no difference who calls it because the tables are static
    mapMatrixYZ.InitCosSinTables();

    // Make sure we have a screen DC
    if( hScreenDC == NULL )
    {
        PostMsg( "Init Error", "Can't get a DC from the window handle. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Allocate the Z Buffer
    ZBuffer = new int[ renderWidth * renderHeight ];
    if( ZBuffer == NULL )
    {
        PostMsg( "Init Error", "Can't allocate Z buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Zero out the ZBuffer
    memsetQuad( ZBuffer, 0, renderWidth * renderHeight );

    // Init the classes using the screens DC. That way it doesn't have to make one
    buffer.InitActiveDC( hScreenDC );

    // Create the video buffer
    if( !buffer.CreateDIBSectionBuffer( renderWidth, -renderHeight, BIT_COUNT_16 ) )
    {
        PostMsg( "Init Error", "Can't allocate background buffer. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

	//////////////////////////////////////////////////////
    //            Load Polygon Objects
    //////////////////////////////////////////////////////

    // Load the map polygon object 
    if( !mapPly.LoadPolygonObjFile( "box.ply" ) )
    {
        PostMsg( "Init Error", "Unable to load map PLY file" );
        goto HANDLE_ERROR;
    }
	/*
	// Load the first person polygon object 
    if( !fp.LoadPolygonObjFile( "C:\\Funware Projects\\3Dgame\\firstPerson.ply" ) )
    {
        PostMsg( "Init Error", "Unable to load fs PLY file" );
        goto HANDLE_ERROR;
    }
	*/
	//////////////////////////////////////////////////////
    //            Load Texture Library
    //////////////////////////////////////////////////////

    // Load the attack ship texture library 
    if( !mapTxt.ReadTexture( "box.bt" ) )
    {
        PostMsg( "Init Error", "Unable to load BT file" );
        goto HANDLE_ERROR;
    }

	//////////////////////////////////////////////////////
    //           Generate Shaded Table
    //////////////////////////////////////////////////////

    // Allocate the shaded lookup table
    shadedTbl = new unsigned short int[ SHADE_COUNT * COLOR_COUNT_16_BIT ];
    if( shadedTbl == NULL )
    {
        PostMsg( "Init Error", "Can't allocate shaded lookup table. Your system resources may be low so try restarting your computer." );
        goto HANDLE_ERROR;
    }

    // Generate the 16 bit shaded table
    Generate16BitShadedTable( shadedTbl );

    // Set the shaded table
    mapPly.SetShadeLookUpTable( shadedTbl );

    // Set the ZBuffer
    mapPly.SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    mapPly.SetRenderSize( renderWidth, renderHeight );

    // Set the maxium Z and Light distance
    mapPly.SetMaxZDistance( 400000 );
    mapPly.SetMaxLightDistance( 900000 );

    // Set the camera scale
    mapPly.SetCameraScale( cameraSize );

    // Set the video buffer
    mapPly.SetVideoBuffer( buffer.GetPtrToDIBsectionBits() );

    // Set the texture libraries
    mapPly.SetTextures( &mapTxt );

    // Load the coordinate file 
    if( !mapPly.ReadTextCoord( "box.crd" ) )
    {
        PostMsg( "Init Error", "Unable to load CRD file" );
        goto HANDLE_ERROR;
    }
	/*
	// First Person center points and radius for collision testing
	fpCenterX =	fp.GetPoly(0)->CalcCenterX();
	fpCenterY =	fp.GetPoly(0)->CalcCenterY();
	fpCenterZ =	fp.GetPoly(0)->CalcCenterZ();
	fpRadius = fp.GetPoly(0)->Radius;
	*/
	// First time startup render
	//RenderBuffer();

    result = TRUE;

HANDLE_ERROR:;

    return result;

}   // GameInit


/************************************************************************
*    FUNCTION NAME:         GameShutDown                                                             
*
*    DESCRIPTION:           Shutdown the game
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::GameShutDown( HWND hwnd )
{
    // Stop the game
    gameRunning = FALSE;

    // Turn off mouse clipping
    ClipCursor( NULL );
    
    // Free the buffer
    buffer.Free();

    // Free the z buffer
    if( ZBuffer )
    {
        delete [] ZBuffer;
        ZBuffer = NULL;
    }

    // Free the shaded table
    if( shadedTbl )
    {
        delete [] shadedTbl;
        shadedTbl = NULL;
    }

    // Release the screen DC. Make sure this is the last item to free
    if( hScreenDC != NULL )
        ReleaseDC( hwnd, hScreenDC );

}   // GameLoop


/************************************************************************
*    FUNCTION NAME:         GameLoop                                                             
*
*    DESCRIPTION:           Call to excute the game loop
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::GameLoop()
{
    if( gameRunning )
    {
        // Is it time to render again
        if( gameLoopTimer < timeGetTime() )
        {
            // Get user imput
            GetUserImput();

			// Check for collisions
			CheckForCollision();

            // Render buffer and show it
            RenderBuffer();

            // Inc the game loop counter
            ++gameLoopCounter;

            // Reset the timer
            gameLoopTimer = timeGetTime() + 30;
        }
    }
}   // GameLoop


/************************************************************************
*    FUNCTION NAME:         CheckForCollision                                                             
*
*    DESCRIPTION:           Check for collision
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::CheckForCollision()
{	/*
	int index = 0;
	PPanel3D pPannel;

	do
	{
		pPannel = mapPly.Collide( index, fpCenterX, fpCenterY, fpCenterZ, fpRadius );
		++index;

	}
	while( pPannel != NULL );
	*/
}	// CheckForCollision


/************************************************************************
*    FUNCTION NAME:         GetUserImput                                                             
*
*    DESCRIPTION:           Act upon what the user is doing
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::GetUserImput()
{
    short int keyState;
	POINT mousePt;
	float viewX=0, viewY=0, viewZ=0;

	// See if we want to escape out
	keyState = GetAsyncKeyState( VK_ESCAPE );
    if( HIBYTE( keyState ) )
    {
    	StopGame();
    	return;    
    }

    keyState = GetAsyncKeyState( VK_UP );
    if( HIBYTE( keyState ) )
    {
        //mapMatrixYZ.Translate( 0.0, 0.0, -MOVEMENT_SPEED );
		viewZ = -MOVEMENT_SPEED;
    }

    keyState = GetAsyncKeyState( VK_DOWN );
    if( HIBYTE( keyState ) )
    {
        //mapMatrixYZ.Translate( 0.0, 0.0, MOVEMENT_SPEED );
		viewZ = MOVEMENT_SPEED;
    }

    keyState = GetAsyncKeyState( VK_LEFT );
    if( HIBYTE( keyState ) )
    {
        //mapMatrixYZ.Translate( MOVEMENT_SPEED, 0.0, 0.0 );
		viewX = MOVEMENT_SPEED;
    }

    keyState = GetAsyncKeyState( VK_RIGHT );
    if( HIBYTE( keyState ) )
    {
        //mapMatrixYZ.Translate( -MOVEMENT_SPEED, 0.0, 0.0 );
		viewX = -MOVEMENT_SPEED;
    }

	//mapPly.Collide( 0, viewX, viewY, viewZ, 30 );
	mapMatrixYZ.Translate( viewX, viewY, viewZ );

	// Get the mouse X & Y position
	GetCursorPos( &mousePt );

	// Do the Y rotation. We are using the difference between the center abd the new offsets
	mapMatrixYZ.Rotate( 0, mouseCenterPt.x - mousePt.x, 0 );

	// The X rotation has to be done seperately and added into final matrix
	//mapMatrixX.Rotate( mouseCenterPt.y - mousePt.y, 0, 0 );

	// Reset the cursor positions
	SetCursorPos( mouseCenterPt.x, mouseCenterPt.y );

}   // GetUserImput


/************************************************************************
*    FUNCTION NAME:         RenderBuffer                                                             
*
*    DESCRIPTION:           Render to the buffer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   BOOL - True or False depending on if successful
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  12/29/03       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::RenderBuffer()
{
    unsigned int start, stop, count;
	Matrix3D tmpMatrix;

    // Get the starting time
    start = timeGetTime();

	// Use a temporary matrix to merge the two matrixes into one
	tmpMatrix.MergeMatrix( mapMatrixYZ.Matrix );
	tmpMatrix.MergeMatrix( mapMatrixX.Matrix );

    // Render the buffer. Flip, flop for the negitive to the positive
    if( gameLoopCounter & 1 )
    {
        count = mapPly.PaintVBuffer16To16( tmpMatrix, NEG_ZBUFFER );
    }
    else
    {
        count = mapPly.PaintVBuffer16To16( tmpMatrix, POS_ZBUFFER );
    }

    // Ho long did it take to render?
    stop = timeGetTime() - start;

    // Lets see how long this is taking
    wsprintf( debugStr, "render buffer: %u;  Buffer to Screen: %u;  Poly Count: %u", stop, bufferRenderTime, count );
    TextOut( buffer.Handle(), 0, 0, debugStr, strlen(debugStr) );

	// Get the starting time
    start = timeGetTime();

    // Disply the buffer to the screen
    buffer.Draw( hScreenDC );

	bufferRenderTime = timeGetTime() - start;

}   // RenderBuffer


/************************************************************************
*    FUNCTION NAME:         OnMouseMove()                                                             
*
*    DESCRIPTION:           Standard Windows OnMouseMove message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd      - Handle of the window receiving the message 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    3/30/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CStarterWnd::OnMouseMove( HWND hwnd, int X, int Y, UINT keyFlags )
{
    if( gameRunning )
    {
		
		//MessageBeep(0);
    }
}   // OnMouseMove