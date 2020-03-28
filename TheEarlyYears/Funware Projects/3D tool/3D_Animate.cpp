
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
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include "3D_Animate.h"      // Header file for this *.cpp file.
#include "gTools.h"          // A misc group of useful functions
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "PictLoad.h"        // Dialog for loading images that shows a preview window
#include "TMoveItem.h"       // Dialog for moving items in a list box


// Globals
PC3DAnimator p3DAnimatorWnd;


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
    MSG Msg;
    BOOL Result = FALSE;
    HACCEL hAccel;

    // Create the parent window object
    p3DAnimatorWnd = new C3DAnimator();

    // Create the parent window
    if( p3DAnimatorWnd )
        Result = p3DAnimatorWnd->CreateParentWnd( hInst, nCmdShow );

    // Kick out of the program if there has been an error
    if( !Result )
        return Result;

    // Load the accelerators
    hAccel = LoadAccelerators( hInst, "EditMapAccl" );
    if( !hAccel )
    {
        PostMsg( "Error", "Error loading accelerator table" );
        return FALSE;
    }

    // Standard windows message loop
    while( GetMessage( &Msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( p3DAnimatorWnd->GetParentWnd(), hAccel, &Msg) )
        {
            // We are goint to have to handle our own mouse wheel
            // messages because we don't have a text box with text in it.
            if( Msg.message == WM_MOUSEWHEEL )
                p3DAnimatorWnd->HandleSubClassProc( Msg.hwnd, Msg.message, Msg.wParam, Msg.lParam );

            TranslateMessage( &Msg );
            DispatchMessage( &Msg );
        }
    }

    return Msg.wParam;

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
        HANDLE_MSG( hwnd, WM_INITDIALOG, p3DAnimatorWnd->OnInitDialog );
        HANDLE_MSG( hwnd, WM_COMMAND,    p3DAnimatorWnd->OnCommand );
        HANDLE_MSG( hwnd, WM_DESTROY,    p3DAnimatorWnd->OnDestroy );
        HANDLE_MSG( hwnd, WM_NCDESTROY,  p3DAnimatorWnd->OnNCDestroy );
        default:
            return FALSE;
    }

} // WndProc


/************************************************************************
*    FUNCTION NAME:         SubClassProc                                                             
*
*    DESCRIPTION:           Subclassing windows controls
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

LRESULT CALLBACK SubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    return p3DAnimatorWnd->HandleSubClassProc( hCtrl, Message, wParam, lParam );

}   // SubClassProc


/************************************************************************
*    FUNCTION NAME:         PlayAnimTimer                                                             
*
*    DESCRIPTION:           Callback timer function that plays the animation(s).
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

void CALLBACK PlayAnimTimer( HWND hwnd, UINT uTimerMsg, UINT uTimerID, DWORD dwTime )
{
    // Play the animations
    p3DAnimatorWnd->TimerPlayMovements( hwnd );

}   // PlayAnimTimer


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

C3DAnimator::C3DAnimator()
{
    // Init class members
    ZBuffer = NULL;
    FlipFlopZBuffer = FALSE;
    ZBufferFlipFlopCounter = FALSE;
    MouseButtonDown = FALSE;
    ActiveIndex = -1;
    MouseMoveState = 0;
    CameraListSave = FALSE;
    CameraListFileName[0] = 0;
    hViewWndDC = NULL;
    TimerReturnCode = 0;
    CameraFrameCounter = 0;

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

C3DAnimator::~C3DAnimator()
{
    if( ZBuffer )
        delete [] ZBuffer;

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

BOOL C3DAnimator::CreateParentWnd( HINSTANCE hInstance, int nCmdShow )
{
    BOOL Result = FALSE;

    // Save the program instance
    hInst = hInstance;

    // Create the parent window
    if( CreateWnd( hInst, nCmdShow ) )
        Result = TRUE;
    else
        // Let them know there was a problem
        MessageBox( 0, "Error creating parent window.", "Error", MB_OK );

    return Result;

}   // CreateParentWnd 


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

BOOL C3DAnimator::CreateWnd( HINSTANCE hInstance, int nCmdShow )
{
    // Create the parent window & save the window handle
    if( !CreateDialog( hInstance, "DLG_3D_ANIMATE", NULL, (DLGPROC)WndProc ) )
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

void C3DAnimator::OnDestroy( HWND hwnd )
{
    int i;

    if( TimerReturnCode )
        // Kill the timer if it is left running
        KillTimer( hwnd, ANIM_TIMER_HANDLE );

    // release the view window DC
    if( hViewWndDC )
        ReleaseDC( hSubCtrl[ STC_VIEWER ], hViewWndDC );
    
    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS; ++i )
    {
        // Give all the subclassed controls their procedure back 
        // before destroying the dialog box
        if( OldSubCtrlProc[ i ] )
            SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, LONG( OldSubCtrlProc[ i ] ) );
    }

    // This causes the WinMain to fall out of it's while loop
    PostQuitMessage( 0 );

} // OnDestroy


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

void C3DAnimator::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    int Index;

    switch( id )
    {
        case BTN_CANCEL:
        case MNU_EXIT:
            QuitProgram( hwnd );
        break;

        case MNU_OPEN_POLYGON:
            MnuClk_OpenPolygonObj( hwnd );
        break;
          
        case MNU_SAVE_TXT_COORD:
            if( PlyList.GetAllowTextCtrls( ActiveIndex ) )
                SaveCRDfile( hwnd, ActiveIndex );
        break;

        case MNU_SAVE_TXT_LIBRARY:
            SaveTextureLibrary( hwnd, ActiveIndex );
        break;

        case MNU_SAVE_CAM_MOVE:
            SaveCameraMoveFile( hwnd );
        break;

        case MNU_OPEN_CAM_MOVE:
            OpenCameraMoveFile( hwnd );
        break;

        case MNU_SAVE_OBJ_MOVE:
            SaveObjectMoveFile( hwnd, ActiveIndex );
        break;

        case MNU_OPEN_OBJ_MOVE:
            OpenObjMoveFile( hwnd );
        break;
          
        case MNU_SAVEALL:
            MnuClk_SaveAll( hwnd );
        break;

        case LST_POLYGON_OBJS:
            LstClk_PolyObjListBox( hwnd, hwndCtl, codeNotify );
        break;

        case MNU_CLOSE_POLYGON:
            MnuClk_CloseObject( hwnd );
        break;

        case CHK_MOVE_ACCUMULATIVE:
            RenderScene();
        break;

        case BTN_OFFSET_MOVE:
            BtnClk_RecordObjectOffset( hwnd );
        break;

        case LST_OFFSET_MOVE:
            LstClk_ObjectMove( hwnd, hwndCtl, codeNotify );
        break;

        case BTN_CHANGE_OFFSET_MOVE:
            BtnClk_ChangeObjectOffset( hwnd );
        break;

        case BTN_CAM_OFF_MOVE:
            BtnClk_RecordCameraOffset( hwnd );
        break;

        case LST_CAM_OFFSET_MOVE:
            LstClk_CameraMove( hwnd, hwndCtl, codeNotify );
        break;

        case BTN_CAM_CHANGE:
            BtnClk_ChangeCameraOffset( hwnd );
        break;

        case LST_TEXTURES:
            LstClk_LoadSelectedTexture( hwnd, hwndCtl, codeNotify );    
        break;

        case BTN_APPLY_TEXT:
            if( PlyList.GetAllowTextCtrls( ActiveIndex ) )
            {
                // Get the index of the selected polygon
                Index = ListBox_GetCurSel( GetDlgItem( hwnd, LST_TEXTURES ) );

                // Apply the texture properties
                BtnClk_ApplyTexture( hwnd, Index );
            }    
        break;

        case BTN_ROTATE:
            BtnClk_RotateTexture();        
        break;

        case CHK_TEXTURE_MODE:
            BtnClk_EnableDisableTextureControls( hwnd );
        break;

        case MNU_ADD_TEXT_TO_LIB:
            MnuClk_AddTesture( hwnd );
        break;

        case BTN_ADD_MOVE:
            BtnClk_AddMoveToAnimate( hwnd );
        break;

        case BTN_PLAY_MOVE:
            BtnClk_PlayMovements( hwnd );
        break;

        case MNU_MOVE_TEXTURE:
            MnuClk_MoveTexture( hwnd );
        break;

        case MNU_DELETE_TEXTURE:
            MnuClk_DeleteTexture( hwnd );
        break;

        case MNU_EXPORT_BYTE_LIST:
        case MNU_EXPORT_WORD_LIST:
        case MNU_EXPORT_INT_LIST:
            MnuClk_ExportList( hwnd, id );
        break;

        case MNU_REBUILD_LIBRARY:
            MnuClk_RebuildTextureLibrary( hwnd );   
        break;

        case MNU_SELECT_ALL:
            MnuClk_SelectAll( hwnd );
        break;

        case CHK_TRAILS:
            BtnClk_EnableDisableTrails( hwnd, hwndCtl );
        break;

        case CHK_ROTATE_FROM_PT:
            BtnClk_EnableDisableRotatePt( hwnd, hwndCtl );
        break;

        case CHK_AS_LIGHT_SOURCE:
            BtnClk_EnableDisableAsLightSource( hwnd, hwndCtl );
        break;

        case BTN_MIN_X: 
        case BTN_PLUS_X:
        case BTN_MIN_Z: 
        case BTN_PLUS_Z:
        case BTN_PLUS_Y:
        case BTN_MIN_Y: 
            BtnClk_DirectBtns( hwnd, id );
        break;

        case MNU_RESET_CAMERA:
            // Reinit the matrix
            CameraMatrix.InitilizeMatrix();

            // Reset the other camera
            CameraView.XPos = CameraView.YPos = CameraView.ZPos = 0;
            CameraView.XRot = CameraView.YRot = CameraView.ZRot = 0;

            // Render the scene
            RenderScene();

            // Show it
            VBuffer.Draw( hViewWndDC );

            // Show the camera coordinates
            Edit_DisplayValue( hSubCtrl[ X_TRAN_CAM ], 0 );
            Edit_DisplayValue( hSubCtrl[ Y_TRAN_CAM ], 0 );
            Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], 0 );
            Edit_DisplayValue( hSubCtrl[ X_ROT_CAM ], 0 );
            Edit_DisplayValue( hSubCtrl[ Y_ROT_CAM ], 0 );
            Edit_DisplayValue( hSubCtrl[ Z_ROT_CAM ], 0 );
        break;

        case CHK_WIRE_FRAME:
            BtnClk_EnableDisableWireFrame( hwnd, hwndCtl );
        break;

        case MNU_DEL_DEAD_TXT:
            MnuClk_DeleteDeadTextures( hwnd );
        break;

        case MNU_EXPORT_COMPRESSED:
            // See if the menu is checked
            if( IsMenuChecked( hwnd, MNU_EXPORT_COMPRESSED ) )
            {
                // We need to uncheck this menu
                Settings.SetCompressedList( FALSE );
                CheckMenuItem( GetMenu( hwnd ), MNU_EXPORT_COMPRESSED, MF_UNCHECKED );
            }
            else
            {
                // We need to uncheck this menu
                Settings.SetCompressedList( TRUE );
                CheckMenuItem( GetMenu( hwnd ), MNU_EXPORT_COMPRESSED, MF_CHECKED );
            }
        break;

        case MNU_SORT_Z_ORDER:
            MnuClk_SortOnZOrder( hwnd );
        break;

        case MNU_ALL_FORWARD_POLYS:
            MnuClk_SelectAllFacingPolys( hwnd );
        break;

		case MNU_1_TO_1_RATIO:
			MnuClk_1_To_1_TextureToPolyRatio( hwnd );
		break;

		case MNU_CALC_LIGHTING:
			MnuClk_RecalcVertexLighting( hwnd );
		break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         MnuClk_SortOnZOrder                                                             
*
*    DESCRIPTION:           Sort all the polygons on Z order
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_SortOnZOrder( HWND hwnd )
{
    PlyList.GetPolyObjClass( ActiveIndex )->Sort();

    // Set the flag for the Polygon Object file
    PlyList.SetPlyObjLive( ActiveIndex, TRUE );

    // Set the flag for the Texture Coordinate file
    // This will need to be resaved because the data has been sorted
    PlyList.SetCDRLive( ActiveIndex, TRUE );

    RenderScene();

    // Show it
    VBuffer.Draw( hViewWndDC );

}   // MnuClk_SortOnZOrder


/************************************************************************
*    FUNCTION NAME:         MnuClk_RecalcVertexLighting                                                             
*
*    DESCRIPTION:           Recalculate the lighting
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_RecalcVertexLighting( HWND hwnd )
{
    PlyList.GetPolyObjClass( ActiveIndex )->CalculateShading();

    // Set the flag for the Texture Coordinate file
    // This will need to be resaved because the data has been sorted
    PlyList.SetCDRLive( ActiveIndex, TRUE );

    RenderScene();

    // Show it
    VBuffer.Draw( hViewWndDC );

}   // MnuClk_SortOnZOrder


/************************************************************************
*    FUNCTION NAME:         MnuClk_SelectAll                                                             
*
*    DESCRIPTION:           Select all textures
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_SelectAll( HWND hwnd )
{
    int i;

    // clear out the list
    SelectedPlyLst.Free();

    if( PlyList.Count() )
    {
        for( i = 0; i < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++i )
            // Add the selected index
            SelectedPlyLst.Add( i );

        // Clear out all the controls
        Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_COUNT ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_RATE ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_INTENSITY ), "" );

        Edit_SetText( GetDlgItem( hwnd, EDT_U ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_V ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_U_WIDTH ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_V_HEIGHT ), "" );

        Button_SetCheck( GetDlgItem( hwnd, CHK_TRANS_POLY ), FALSE );
        Button_SetCheck( GetDlgItem( hwnd, CHK_NO_CHECK_EXTEN ), FALSE );
        Button_SetCheck( GetDlgItem( hwnd, CHK_SOLID ), FALSE );

        // Unselect the texture in the texture window
        DisplayTextureLstFile();

        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
}   // MnuClk_SelectAll


/************************************************************************
*    FUNCTION NAME:         MnuClk_SelectAllFacingPolys                                                             
*
*    DESCRIPTION:           Select all facing polygons
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_SelectAllFacingPolys( HWND hwnd )
{
    int i;

    // clear out the list
    SelectedPlyLst.Free();

    if( PlyList.Count() )
    {
        for( i = 0; i < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++i )
        {
            // See if the polygon is facing us
            if( PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->CalcVisible3D() )
                // Add the selected index
                SelectedPlyLst.Add( i );
        }

        // Clear out all the controls
        Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_COUNT ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_RATE ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_INTENSITY ), "" );

        Edit_SetText( GetDlgItem( hwnd, EDT_U ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_V ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_U_WIDTH ), "" );
        Edit_SetText( GetDlgItem( hwnd, EDT_V_HEIGHT ), "" );

        Button_SetCheck( GetDlgItem( hwnd, CHK_TRANS_POLY ), FALSE );
        Button_SetCheck( GetDlgItem( hwnd, CHK_NO_CHECK_EXTEN ), FALSE );
        Button_SetCheck( GetDlgItem( hwnd, CHK_SOLID ), FALSE );

        // Unselect the texture in the texture window
        DisplayTextureLstFile();

        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
}   // MnuClk_SelectAllFacingPolys


/************************************************************************
*    FUNCTION NAME:         BtnClk_DirectBtns                                                             
*
*    DESCRIPTION:           Moves the directions by clicking on the buttons
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

void C3DAnimator::BtnClk_DirectBtns( HWND hwnd, int MoveType )
{
    TCamera Camera;
    BOOL Rotate = Button_GetCheck( GetDlgItem( hwnd, CHK_ROT ) );

    // get the move amount
    int MoveAmount = Edit_GetValue( GetDlgItem( hwnd, EDT_XYTRANS_MOVE_AMT ) );

    // Translate and rotate the new positions.
    // Are we moving the camera or the object?
    // The objects rotations are oposite that of the camera
    if( Button_GetCheck( GetDlgItem( hwnd, RDO_MOVE_CAMERA ) ) )
    {
        // Record the movement
        if( MoveType == BTN_MIN_X )
            if( Rotate )
                Camera.XRot = MoveAmount;
            else
                Camera.XPos = MoveAmount;
        else if( MoveType == BTN_PLUS_X )
            if( Rotate )
                Camera.XRot = -MoveAmount;
            else
                Camera.XPos = -MoveAmount;

        else if( MoveType == BTN_MIN_Y )
            if( Rotate )
                Camera.YRot = MoveAmount;
            else
                Camera.YPos = MoveAmount;
        else if( MoveType == BTN_PLUS_Y )
            if( Rotate )
                Camera.YRot = -MoveAmount;
            else
                Camera.YPos = -MoveAmount;

        else if( MoveType == BTN_MIN_Z )
            if( Rotate )
                Camera.ZRot = MoveAmount;
            else
                Camera.ZPos = MoveAmount;
        else if( MoveType == BTN_PLUS_Z )
            if( Rotate )
                Camera.ZRot = -MoveAmount;
            else
                Camera.ZPos = -MoveAmount;

        // Inc the camers matrix in case needed
        CameraMatrix.Translate( Camera.XPos, Camera.YPos, Camera.ZPos );
        CameraMatrix.Rotate( Camera.XRot, Camera.YRot, Camera.ZRot );

        // Inc the camera movements
        CameraView.XPos += Camera.XPos;
        CameraView.YPos += Camera.YPos;
        CameraView.ZPos += Camera.ZPos;
        CameraView.XRot += Camera.XRot;
        CameraView.YRot += Camera.YRot;
        CameraView.ZRot += Camera.ZRot;

        // Inc the step camera for saving camer zero point movements
        cameraStep.XPos += Camera.XPos;
        cameraStep.YPos += Camera.YPos;
        cameraStep.ZPos += Camera.ZPos;
        cameraStep.XRot += Camera.XRot;
        cameraStep.YRot += Camera.YRot;
        cameraStep.ZRot += Camera.ZRot;

        // Show the camera coordinates
        Edit_DisplayValue( hSubCtrl[ X_TRAN_CAM ], CameraView.XPos );
        Edit_DisplayValue( hSubCtrl[ Y_TRAN_CAM ], CameraView.YPos );
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], CameraView.ZPos );
        Edit_DisplayValue( hSubCtrl[ X_ROT_CAM ], CameraView.XRot );
        Edit_DisplayValue( hSubCtrl[ Y_ROT_CAM ], CameraView.YRot );
        Edit_DisplayValue( hSubCtrl[ Z_ROT_CAM ], CameraView.ZRot );
    }
    else
    {
        // Record the movement
        if( MoveType == BTN_MIN_X )
            if( Rotate )
                Camera.XRot = -MoveAmount;
            else
                Camera.XPos = -MoveAmount;
        else if( MoveType == BTN_PLUS_X )
            if( Rotate )
                Camera.XRot = MoveAmount;
            else
                Camera.XPos = MoveAmount;

        else if( MoveType == BTN_MIN_Y )
            if( Rotate )
                Camera.YRot = MoveAmount;
            else
                Camera.YPos = MoveAmount;
        else if( MoveType == BTN_PLUS_Y )
            if( Rotate )
                Camera.YRot = -MoveAmount;
            else
                Camera.YPos = -MoveAmount;

        else if( MoveType == BTN_MIN_Z )
            if( Rotate )
                Camera.ZRot = MoveAmount;
            else
                Camera.ZPos = MoveAmount;
        else if( MoveType == BTN_PLUS_Z )
            if( Rotate )
                Camera.ZRot = -MoveAmount;
            else
                Camera.ZPos = -MoveAmount;

        // Inc the camera movements
        PlyList.GetOffset( ActiveIndex )->XPos += Camera.XPos;
        PlyList.GetOffset( ActiveIndex )->YPos += Camera.YPos;
        PlyList.GetOffset( ActiveIndex )->ZPos += Camera.ZPos;
        PlyList.GetOffset( ActiveIndex )->XRot += Camera.XRot;
        PlyList.GetOffset( ActiveIndex )->YRot += Camera.YRot;
        PlyList.GetOffset( ActiveIndex )->ZRot += Camera.ZRot;
        
        // Show the object coordinates
        //Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], PlyList.GetRotatePtSave( ActiveIndex )->XPos );
        //Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], PlyList.GetRotatePtSave( ActiveIndex )->YPos );
        //Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], PlyList.GetRotatePtSave( ActiveIndex )->ZPos );
        Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->XPos );
        Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->YPos );
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->ZPos );
        Edit_DisplayValue( hSubCtrl[ X_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->XRot );
        Edit_DisplayValue( hSubCtrl[ Y_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->YRot );
        Edit_DisplayValue( hSubCtrl[ Z_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->ZRot );          
    }

    // Render the scene
    RenderScene();

    // Show it
    VBuffer.Draw( hViewWndDC );

}   // ViewWnd_OnMouseMove


/************************************************************************
*    FUNCTION NAME:         BtnClk_EnableDisableTrails                                                             
*
*    DESCRIPTION:           Turn on or off trails for plotting path
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_EnableDisableTrails( HWND hwnd, HWND hwndCtl )
{
    // Enable or disable the texture controls
    if( Button_GetCheck( hwndCtl ) )
        PlyList.SetTrails( ActiveIndex, TRUE );
    else
        PlyList.SetTrails( ActiveIndex, FALSE );

    RenderScene();

    // Show it
    VBuffer.Draw( hViewWndDC );

}   // BtnClk_EnableDisableTrails


/************************************************************************
*    FUNCTION NAME:         BtnClk_EnableDisableRotatePt                                                             
*
*    DESCRIPTION:           Enable rotating from a given point
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_EnableDisableRotatePt( HWND hwnd, HWND hwndCtl )
{
    // Enable or disable the texture controls
    if( Button_GetCheck( hwndCtl ) )
    {
        PlyList.SetRotatePoint( ActiveIndex, TRUE );

        // Save the point we want to rotate from
        PlyList.GetRotatePt( ActiveIndex )->XPos = PlyList.GetOffset( ActiveIndex )->XPos;
        PlyList.GetRotatePt( ActiveIndex )->YPos = PlyList.GetOffset( ActiveIndex )->YPos;
        PlyList.GetRotatePt( ActiveIndex )->ZPos = PlyList.GetOffset( ActiveIndex )->ZPos;
    }
    else
    {
        PlyList.SetRotatePoint( ActiveIndex, FALSE );
        PlyList.GetRotatePt( ActiveIndex )->ZRot = -1;
    }

}   // BtnClk_EnableDisableRotatePt


/************************************************************************
*    FUNCTION NAME:         BtnClk_EnableDisableAsLightSource                                                             
*
*    DESCRIPTION:           Use object for light source information
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_EnableDisableAsLightSource( HWND hwnd, HWND hwndCtl )
{
    // Enable or disable the texture controls
    if( Button_GetCheck( hwndCtl ) )
    {
        PlyList.SetUseAsLightSource( ActiveIndex, TRUE );

        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
    else
    {
        PlyList.SetUseAsLightSource( ActiveIndex, FALSE );
    }

}   // BtnClk_EnableDisableAsLightSource


/************************************************************************
*    FUNCTION NAME:         BtnClk_EnableDisableWireFrame                                                             
*
*    DESCRIPTION:           Enable / disable wire frame rendering
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_EnableDisableWireFrame( HWND hwnd, HWND hwndCtl )
{
    // Enable or disable the texture controls
    if( Button_GetCheck( hwndCtl ) )
        PlyList.SetWireFrame( ActiveIndex, TRUE );
    else
        PlyList.SetWireFrame( ActiveIndex, FALSE );

    RenderScene();

    // Show it
    VBuffer.Draw( hViewWndDC );

}   // BtnClk_EnableDisableWireFrame


/************************************************************************
*    FUNCTION NAME:         MnuClk_RebuildTextureLibrary                                                             
*
*    DESCRIPTION:           rebuild the texture library
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_RebuildTextureLibrary( HWND hwnd )
{
    if( BuildTextureLibrary( hwnd, ActiveIndex ) )
    {
        // Set the shaded look-up table
        if( SystemBitDepth <= 8 )
            PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to8BitShadedTable() );
        else
        {
            if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
                PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to16BitShadedTable() );
            else
                PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get16to16BitShadedTable() );
        }

        // Reset all the panels
        PlyList.GetPolyObjClass( ActiveIndex )->ReInitPanels();

        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }

}   // MnuClk_RebuildTextureLibrary


/************************************************************************
*    FUNCTION NAME:         MnuClk_ExportList                                                             
*
*    DESCRIPTION:           Export a trimmed down camera or movement list
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_ExportList( HWND hwnd, int ExportType )
{
    PTCameraList pTempList;
    char ExportTitle[50], FileName[MAX_PATH], FilePath[MAX_PATH];

    // Check that we have something to export
    if( Button_GetCheck( GetDlgItem( hwnd, RDO_MOVE_CAMERA ) ) && !CameraList.Count() )
        return;
    else if( Button_GetCheck( GetDlgItem( hwnd, RDO_MOVE_OBJECT ) ) && !PlyList.GetPolyOffList( ActiveIndex )->Count() )
        return;

    // Get the list
    if( Button_GetCheck( GetDlgItem( hwnd, RDO_MOVE_CAMERA ) ) )
    {
        pTempList = &CameraList;

        // Setup the export dialog box caption
        strcpy( ExportTitle, "Export Camera " );
    }
    else if( Button_GetCheck( GetDlgItem( hwnd, RDO_MOVE_OBJECT ) ) )
    {
        pTempList = PlyList.GetPolyOffList( ActiveIndex );

        // Setup the export dialog box caption
        strcpy( ExportTitle, "Export Object " );
    }

    // Finish the dialog caption
    if( ExportType == MNU_EXPORT_BYTE_LIST )
        strcat( ExportTitle, "Byte List" );
    if( ExportType == MNU_EXPORT_WORD_LIST )
        strcat( ExportTitle, "Word List" );
    if( ExportType == MNU_EXPORT_INT_LIST )
        strcat( ExportTitle, "Int List" );

    // Null the file name
    FileName[0] = 0;

    // Make a path to the projects poly file
    CropOutPath( FilePath, PlyList.GetPolyObjFileName( ActiveIndex ) );

    // Show the windows get file box
    if( GetFileName( hwnd, hInst, FileName, ExportTitle, FilePath, 
                     BIN_FILE_TYPES, BIN_EXT, SAVE_FILENAME_DIALOG_BOX ) )
    {
        // Export as a simple list
        pTempList->ExportSimpleList( FileName, ExportType - MNU_EXPORT_BYTE_LIST, IsMenuChecked( hwnd, MNU_EXPORT_COMPRESSED ) );
    }

}   // MnuClk_ExportList


/************************************************************************
*    FUNCTION NAME:         MnuClk_MoveTexture                                                             
*
*    DESCRIPTION:           Move texture in texture list
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_MoveTexture( HWND hwnd )
{
    int ListIndex, MoveIndex, i, MinIndex, MaxIndex, ImageIndex, Offset;
    HWND hTextLst;

    // Get the handle to the texture list control
    hTextLst = GetDlgItem( hwnd, LST_TEXTURES );

    // Make sure we have a selection to the animation
    ListIndex = ListBox_GetCurSel( hTextLst );
    if( ListIndex == LB_ERR )
        return;

    // Do they want to move to another index
    MoveIndex = DisplayMoveItemDlg( hInst, hwnd, hTextLst, ListIndex, PlyList.GetTextStrList( ActiveIndex )->Count() );

    if( MoveIndex > -1 )
    {
        // Move the selected item to the new index
        PlyList.GetTextStrList( ActiveIndex )->MoveToNewPos( ListIndex, MoveIndex );

        // Clear out the frame list box
        ListBox_ResetContent( hTextLst );

        // Load the list box with the updated texture list
        for( i = 0; i < PlyList.GetTextStrList( ActiveIndex )->Count(); ++i )
        {
            // Add animation name to create animation list box
            ListBox_AddString( hTextLst, PlyList.GetTextStrList( ActiveIndex )->GetString( i ) );
        }

        // Select the list box item
        ListBox_SetSel( hTextLst, TRUE, MoveIndex );

        // Init our range
        if( ListIndex < MoveIndex )
        {
            MinIndex = ListIndex;
            MaxIndex = MoveIndex;
            Offset = -1;
        }
        else
        {
            MaxIndex = ListIndex;
            MinIndex = MoveIndex;
            Offset = 1;
        }

        // Go through and shift all the indexes around to their new correct order
        for( i = 0; i < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++i )
        {
            // Get the image index
            ImageIndex = PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->GetTextureHandle();

            // We only need to move indexes that fall in the specific range
            if( ImageIndex >= MinIndex && ImageIndex <= MaxIndex )
            {
                // Specificly move the selected index
                if( ImageIndex == ListIndex )
                    PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->SetTextureHandle( MoveIndex );

                // Move around all the other indexes in between
                else
                    PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->SetTextureHandle( ImageIndex + Offset );
            }
        }

        // Set the flag for the Texture Coordinate file
        PlyList.SetCDRLive( ActiveIndex, TRUE );

        // Rebuild the texture library
        if( BuildTextureLibrary( hwnd, ActiveIndex ) )
        {
            // Set the shaded look-up table
            if( SystemBitDepth <= 8 )
                PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to8BitShadedTable() );
            else
            {
                if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
                    PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to16BitShadedTable() );
                else
                    PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get16to16BitShadedTable() );
            }

            // Reset all the panels
            PlyList.GetPolyObjClass( ActiveIndex )->ReInitPanels();

            // Render the polygon object
            RenderScene();

            // Show it
            VBuffer.Draw( hViewWndDC );
        }
    }

}   // MnuClk_MoveTexture


/************************************************************************
*    FUNCTION NAME:         MnuClk_DeleteDeadTextures                                                             
*
*    DESCRIPTION:           Delete all the unused and dead textures
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_DeleteDeadTextures( HWND hwnd )
{
    int i, j, ImageIndex, textCount, textIndex, deleteCount=0, frameCount;
    BOOL textureUsed;

    // Ask if they want to delete the selected texture
    if( ActiveIndex > -1 && IDYES == AskQuestion( MB_YESNO, "Question?", "Do you want to delete all unused textures?" ) )
    {
        // Get the total number of textures
        textCount = PlyList.GetTextStrList( ActiveIndex )->Count();

        // Go through and check if we are using all the textures
        for( i = 0; i < textCount; ++i )
        {
            // Set the flag to false
            textureUsed = FALSE;
            
            // Go through and see if this texture is being used
            for( j = 0; j < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++j )
            {
                // Get the frame count because this could be an animated polygon
                frameCount = PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( j )->GetFrameCount();

                // Get the texture index
                textIndex = PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( j )->GetTextureHandle();
                
                // Get the image index and see if it matches the texture index we are checking
                if( i - deleteCount >= textIndex && i - deleteCount <= textIndex + frameCount )
                {
                    textureUsed = TRUE;
                    break;  
                }
            }

            // Delete the texture if it is not being used
            if( !textureUsed )
            {
                // Move the selected item to the new index
                PlyList.GetTextStrList( ActiveIndex )->Delete( i - deleteCount );

                // Delete the list string
                ListBox_DeleteString( GetDlgItem( hwnd, LST_TEXTURES ), i - deleteCount );

                // Update the delete counter
                ++deleteCount;

                // Go through and shift all the indexes around to their new correct order
                for( j = 0; j < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++j )
                {
                    // Get the image index
                    ImageIndex = PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( j )->GetTextureHandle();

                    // Move around all the other indexes in between
                    if( ImageIndex > i - deleteCount )
                        PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( j )->SetTextureHandle( ImageIndex - 1 );
                }
            }
        }

        // Indicate the number of deleted textures
        PostMsg( "Dead Texture Delete", "The number of deleted dead textures was %d.", deleteCount );

        // Rebuild the texture library if we deleted some textures
        if( deleteCount )
        {
            // Rebuild the texture library
            if( BuildTextureLibrary( hwnd, ActiveIndex ) )
            {
                // Set the shaded look-up table
                if( SystemBitDepth <= 8 )
                    PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to8BitShadedTable() );
                else
                {
                    if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
                        PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to16BitShadedTable() );
                    else
                        PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get16to16BitShadedTable() );
                }

                // Reset all the panels
                PlyList.GetPolyObjClass( ActiveIndex )->ReInitPanels();
            }

            // Set the flag for the Texture Coordinate file
            PlyList.SetCDRLive( ActiveIndex, TRUE );

            // Render the polygon object
            RenderScene();

            // Show it
            VBuffer.Draw( hViewWndDC );
        }
    }
}   // MnuClk_DeleteDeadTextures


/************************************************************************
*    FUNCTION NAME:         MnuClk_DeleteTexture                                                             
*
*    DESCRIPTION:           Delete texture in texture list
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_DeleteTexture( HWND hwnd )
{
    int ListIndex, i, ImageIndex;

    // Make sure we have a selection to the animation
    ListIndex = ListBox_GetCurSel( GetDlgItem( hwnd, LST_TEXTURES ) );
    if( ListIndex == LB_ERR )
        return;

    // Ask if they want to delete the selected texture
    if( IDYES == AskQuestion( MB_YESNO, "Question?", "Do you want to delete the selected texture (%s)?", 
                              PlyList.GetTextStrList( ActiveIndex )->GetString( ListIndex ) ) )
    {
        // Delete the list string
        ListBox_DeleteString( GetDlgItem( hwnd, LST_TEXTURES ), ListIndex );

        // Move the selected item to the new index
        PlyList.GetTextStrList( ActiveIndex )->Delete( ListIndex );

        // Rebuild the texture library
        if( BuildTextureLibrary( hwnd, ActiveIndex ) )
        {
            // Set the shaded look-up table
            if( SystemBitDepth <= 8 )
                PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to8BitShadedTable() );
            else
            {
                if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
                    PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to16BitShadedTable() );
                else
                    PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get16to16BitShadedTable() );
            }

            // Go through and shift all the indexes around to their new correct order
            for( i = 0; i < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++i )
            {
                // Get the image index
                ImageIndex = PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->GetTextureHandle();

                // Specificly reset the selected index to the first index
                if( ImageIndex == ListIndex )
                    PlyList.GetPolyObjClass( ActiveIndex )->SetTexture( i, 0 );

                // Move around all the other indexes in between
                else if( ImageIndex > ListIndex )
                    PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->SetTextureHandle( ImageIndex - 1 );
            }

            // Reset all the panels
            PlyList.GetPolyObjClass( ActiveIndex )->ReInitPanels();

            // Set the flag for the Texture Coordinate file
            PlyList.SetCDRLive( ActiveIndex, TRUE );

            // Render the polygon object
            RenderScene();

            // Show it
            VBuffer.Draw( hViewWndDC );
        }
    }

}   // MnuClk_DeleteTexture


/************************************************************************
*    FUNCTION NAME:         BtnClk_EnableDisableTextureControls                                                             
*
*    DESCRIPTION:           Enable or disable the texture controls
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_EnableDisableTextureControls( HWND hwnd )
{
    int i;
    HMENU hMenu;

    // Get the menu
    hMenu = GetMenu(hwnd);

    // Free the selection list
    SelectedPlyLst.Free();

    // Enable or disable the texture controls
    if( Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) )
    {
        if( PlyList.GetAllowTextCtrls( ActiveIndex ) )
        {
            LstClk_PolyObjListBox( hwnd, hSubCtrl[ LST_POLYOBJS ], CBN_SELCHANGE );
        }

        for( i = 0; i < MAX_TEXT_MENU_CONTROLS; ++i )
            EnableMenuItem( hMenu, MNU_DELETE_TEXTURE+i, MF_ENABLED );
    }
    else
    {
        ListBox_ResetContent( GetDlgItem( hwnd, LST_TEXTURES ) );
        EnableWindow( hSubCtrl[ STC_PREVIEW ], FALSE );

        for( i = 0; i < MAX_TEXTURE_EDIT; ++i )
            EnableWindow( GetDlgItem( hwnd, LST_TEXTURES + i ), FALSE );

        for( i = 0; i < MAX_TEXT_MENU_CONTROLS; ++i )
            EnableMenuItem( hMenu, MNU_DELETE_TEXTURE+i, MF_GRAYED|MF_DISABLED );
            
        // render the scene in case a selection was made
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }

}   // BtnClk_EnableDisableTextureControls


/************************************************************************
*    FUNCTION NAME:         LstClk_PolyObjListBox                                                             
*
*    DESCRIPTION:           Handle all the polygon object list tasks
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::LstClk_PolyObjListBox( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int i;
    char StrTmp[100];
    HMENU hMenu = GetMenu(hwnd);
    
    if( codeNotify == CBN_SELCHANGE )
    {            
        // Make sure we have a selection to the animation
        ActiveIndex = ListBox_GetCurSel( hwndCtl );
        if( ActiveIndex == LB_ERR )
            return;

        // Reset the list box
        ListBox_ResetContent( hSubCtrl[ LST_OFF_MOVE ] );

        // Free the selection list
        SelectedPlyLst.Free();

        // Load the list into the list box
        for( i = 0; i < PlyList.GetPolyOffList( ActiveIndex )->Count(); ++i )
        {
            // Create the string to add to the list box
            sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)PlyList.GetPolyOffList( ActiveIndex )->GetXPos( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetYPos( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetZPos( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetXRot( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetYRot( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetZRot( i ) );
            // Add the string to the list box
            ListBox_AddString( hSubCtrl[ LST_OFF_MOVE ], StrTmp );
        }

        // Display the objects offsets
        Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->XPos );
        Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->YPos );
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->ZPos );
        Edit_DisplayValue( hSubCtrl[ X_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->XRot );
        Edit_DisplayValue( hSubCtrl[ Y_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->YRot );
        Edit_DisplayValue( hSubCtrl[ Z_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->ZRot );

        // Set the check box that indicates the object will trail
        Button_SetCheck( GetDlgItem( hwnd, CHK_TRAILS ), PlyList.GetTrails( ActiveIndex ) );

        // Set the check box that indicates the object will rotate from a given point
        Button_SetCheck( GetDlgItem( hwnd, CHK_ROTATE_FROM_PT ), PlyList.GetRotatePoint( ActiveIndex ) );

        // Set the check box that indicates the object will be rendered in wire frame
        Button_SetCheck( GetDlgItem( hwnd, CHK_WIRE_FRAME ), PlyList.GetWireFrame( ActiveIndex ) );

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );

        // Setup the texture window of we are in texture mode
        if( PlyList.GetAllowTextCtrls( ActiveIndex ) && 
            Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) )
        {
            // Init the texture data window
            DisplayTextureLstFile();

            for( i = 0; i < MAX_TEXTURE_EDIT; ++i )
                EnableWindow( GetDlgItem( hwnd, LST_TEXTURES + i ), TRUE );

            // Enable texture menu items
            for( i = 0; i < MAX_TEXT_MENU_CONTROLS; ++i )
                EnableMenuItem( hMenu, MNU_DELETE_TEXTURE+i, MF_ENABLED );

            EnableWindow( hSubCtrl[ STC_PREVIEW ], TRUE );
        }
        else
        {
            Button_SetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ), FALSE );
            ListBox_ResetContent( GetDlgItem( hwnd, LST_TEXTURES ) );
            EnableWindow( hSubCtrl[ STC_PREVIEW ], FALSE );

            for( i = 0; i < MAX_TEXTURE_EDIT; ++i )
                EnableWindow( GetDlgItem( hwnd, LST_TEXTURES + i ), FALSE );

            for( i = 0; i < MAX_TEXT_MENU_CONTROLS; ++i )
                EnableMenuItem( hMenu, MNU_DELETE_TEXTURE+i, MF_GRAYED|MF_DISABLED );
        }
    }

}   // LstClk_PolyObjListBox


/************************************************************************
*    FUNCTION NAME:         LstClk_ObjectMove                                                             
*
*    DESCRIPTION:           Reset the offsets to the selected index
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::LstClk_ObjectMove( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int Index;
    //int i, rotX=0, rotY=0, rotZ=0;

    if( codeNotify == CBN_SELCHANGE && PlyList.Count() )
    {            
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hwndCtl );
        if( Index == LB_ERR )
            return;

        // Hold the index in the rotate Z value
        if( PlyList.GetRotatePoint( ActiveIndex ) )
            PlyList.GetRotatePt( ActiveIndex )->ZRot = Index;

        // Transfer the polygon list offset list to the polygon offset
        // The render function only uses the one structure so copy the indexed data into that
        PlyList.GetOffset( ActiveIndex )->XPos = PlyList.GetPolyOffList( ActiveIndex )->GetXPos( Index );
        PlyList.GetOffset( ActiveIndex )->YPos = PlyList.GetPolyOffList( ActiveIndex )->GetYPos( Index );
        PlyList.GetOffset( ActiveIndex )->ZPos = PlyList.GetPolyOffList( ActiveIndex )->GetZPos( Index );
        PlyList.GetOffset( ActiveIndex )->XRot = PlyList.GetPolyOffList( ActiveIndex )->GetXRot( Index );
        PlyList.GetOffset( ActiveIndex )->YRot = PlyList.GetPolyOffList( ActiveIndex )->GetYRot( Index );
        PlyList.GetOffset( ActiveIndex )->ZRot = PlyList.GetPolyOffList( ActiveIndex )->GetZRot( Index );

        /*
        PlyList.GetMatrix( ActiveIndex )->InitilizeMatrix();

        for( i = 0; i < Index+1; ++i )
        {
            PlyList.GetMatrix( ActiveIndex )->Translate( FlipFlopSign( PlyList.GetPolyOffList( ActiveIndex )->GetXPos( i )), 
                                                         FlipFlopSign( PlyList.GetPolyOffList( ActiveIndex )->GetYPos( i )), 
                                                         FlipFlopSign( PlyList.GetPolyOffList( ActiveIndex )->GetZPos( i )) );

            PlyList.GetMatrix( ActiveIndex )->Rotate( FlipFlopSign( PlyList.GetPolyOffList( ActiveIndex )->GetXRot( i )), 
                                                      FlipFlopSign( PlyList.GetPolyOffList( ActiveIndex )->GetYRot( i )), 
                                                      FlipFlopSign( PlyList.GetPolyOffList( ActiveIndex )->GetZRot( i )) );
        }
        

        PlyList.GetOffset( ActiveIndex )->XPos = PlyList.GetMatrix( ActiveIndex )->Matrix[ 0 ][ 0 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 1 ][ 0 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 2 ][ 0 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 3 ][ 0 ];
        PlyList.GetOffset( ActiveIndex )->YPos = PlyList.GetMatrix( ActiveIndex )->Matrix[ 0 ][ 1 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 1 ][ 1 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 2 ][ 1 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 3 ][ 1 ];
        PlyList.GetOffset( ActiveIndex )->ZPos = PlyList.GetMatrix( ActiveIndex )->Matrix[ 0 ][ 2 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 1 ][ 2 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 2 ][ 2 ] + PlyList.GetMatrix( ActiveIndex )->Matrix[ 3 ][ 2 ];
        */

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );

        // Show the camera coordinates
        Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->XPos );
        Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->YPos );
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->ZPos );
        Edit_DisplayValue( hSubCtrl[ X_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->XRot );
        Edit_DisplayValue( hSubCtrl[ Y_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->YRot );
        Edit_DisplayValue( hSubCtrl[ Z_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->ZRot );
    }

}   // LstClk_ObjectMove


/************************************************************************
*    FUNCTION NAME:         BtnClk_RecordObjectOffset                                                             
*
*    DESCRIPTION:           Record the current object offset
*                           Save the offset information to the selected
*                           polygon object
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_RecordObjectOffset( HWND hwnd )
{
    char StrTmp[100];
    int Index;

    if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_OBJECT ) ) &&
        PlyList.Count() )
    {
        // Get the current number of entries in the list
        Index = PlyList.GetPolyOffList( ActiveIndex )->Count();

        // Add a blank entry to the list
        PlyList.GetPolyOffList( ActiveIndex )->Add();

        // Set the flag that indicates a file change has been made
        PlyList.SetLiveObjMove( ActiveIndex, TRUE );

        // Save the offset information
        PlyList.GetPolyOffList( ActiveIndex )->SetXPos( Index, PlyList.GetRotatePtSave( ActiveIndex )->XPos );
        PlyList.GetPolyOffList( ActiveIndex )->SetYPos( Index, PlyList.GetRotatePtSave( ActiveIndex )->YPos );
        PlyList.GetPolyOffList( ActiveIndex )->SetZPos( Index, PlyList.GetRotatePtSave( ActiveIndex )->ZPos );
        PlyList.GetPolyOffList( ActiveIndex )->SetXRot( Index, PlyList.GetOffset( ActiveIndex )->XRot );
        PlyList.GetPolyOffList( ActiveIndex )->SetYRot( Index, PlyList.GetOffset( ActiveIndex )->YRot );
        PlyList.GetPolyOffList( ActiveIndex )->SetZRot( Index, PlyList.GetOffset( ActiveIndex )->ZRot );

        // Create the string to add to the list box
        sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)PlyList.GetRotatePtSave( ActiveIndex )->XPos,
                                              (int)PlyList.GetRotatePtSave( ActiveIndex )->YPos,
                                              (int)PlyList.GetRotatePtSave( ActiveIndex )->ZPos,
                                              (int)PlyList.GetOffset( ActiveIndex )->XRot,
                                              (int)PlyList.GetOffset( ActiveIndex )->YRot,
                                              (int)PlyList.GetOffset( ActiveIndex )->ZRot );
        // Add the string to the list box
        ListBox_AddString( hSubCtrl[ LST_OFF_MOVE ], StrTmp );

        // Display the total number of object move entries
        Edit_DisplayValue( GetDlgItem( hwnd, STC_OBJECT_MV_COUNT ), PlyList.GetPolyOffList( ActiveIndex )->Count() );
    }

}   // BtnClk_RecordObjectOffset


/************************************************************************
*    FUNCTION NAME:         BtnClk_ChangeObjectOffset                                                             
*
*    DESCRIPTION:           Change the selected object offset
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_ChangeObjectOffset( HWND hwnd )
{
    char StrTmp[100];
    int Index;

    if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_OBJECT ) ) &&
        PlyList.Count() )
    {
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hSubCtrl[ LST_OFF_MOVE ] );
        if( Index == LB_ERR )
            return;

        // Set the flag that indicates a file change has been made
        PlyList.SetLiveObjMove( ActiveIndex, TRUE );

        // Resave the offset information
        PlyList.GetPolyOffList( ActiveIndex )->SetXPos( Index, PlyList.GetOffset( ActiveIndex )->XPos );
        PlyList.GetPolyOffList( ActiveIndex )->SetYPos( Index, PlyList.GetOffset( ActiveIndex )->YPos );
        PlyList.GetPolyOffList( ActiveIndex )->SetZPos( Index, PlyList.GetOffset( ActiveIndex )->ZPos );
        PlyList.GetPolyOffList( ActiveIndex )->SetXRot( Index, PlyList.GetOffset( ActiveIndex )->XRot );
        PlyList.GetPolyOffList( ActiveIndex )->SetYRot( Index, PlyList.GetOffset( ActiveIndex )->YRot );
        PlyList.GetPolyOffList( ActiveIndex )->SetZRot( Index, PlyList.GetOffset( ActiveIndex )->ZRot );

        // Create the string to add to the list box
        sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", PlyList.GetOffset( ActiveIndex )->XPos,
                                              PlyList.GetOffset( ActiveIndex )->YPos,
                                              PlyList.GetOffset( ActiveIndex )->ZPos,
                                              PlyList.GetOffset( ActiveIndex )->XRot,
                                              PlyList.GetOffset( ActiveIndex )->YRot,
                                              PlyList.GetOffset( ActiveIndex )->ZRot );

        // Delete the string we want to change
        ListBox_DeleteString( hSubCtrl[ LST_OFF_MOVE ], Index );

        // Add the string to the list box
        ListBox_InsertString( hSubCtrl[ LST_OFF_MOVE ], Index, StrTmp );
    }

}   // BtnClk_ChangeObjectOffset


/************************************************************************
*    FUNCTION NAME:         BtnClk_RecordCameraOffset                                                             
*
*    DESCRIPTION:           Record the current camera offset information.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_RecordCameraOffset( HWND hwnd )
{
    char StrTmp[100];
    int Index;

    if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_CAMERA ) ) )
    {
        // Get the current number of entries in the list
        Index = CameraList.Count();

        // Add a blank entry to the list
        CameraList.Add();

        if( Index && Button_GetCheck( GetDlgItem( hParentWnd, CHK_MOVE_ACCUMULATIVE ) ) )
        {
            // Save the offset information
            CameraList.SetXPos( Index, cameraStep.XPos );
            CameraList.SetYPos( Index, cameraStep.YPos );
            CameraList.SetZPos( Index, cameraStep.ZPos );
            CameraList.SetXRot( Index, cameraStep.XRot );
            CameraList.SetYRot( Index, cameraStep.YRot );
            CameraList.SetZRot( Index, cameraStep.ZRot );

            // Create the string to add to the list box
            sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)cameraStep.XPos,
                                                  (int)cameraStep.YPos,
                                                  (int)cameraStep.ZPos,
                                                  (int)cameraStep.XRot,
                                                  (int)cameraStep.YRot,
                                                  (int)cameraStep.ZRot );

            // Reset the camera
            cameraStep.Clear();
        }
        else
        {
            // Save the offset information
            CameraList.SetXPos( Index, CameraView.XPos );
            CameraList.SetYPos( Index, CameraView.YPos );
            CameraList.SetZPos( Index, CameraView.ZPos );
            CameraList.SetXRot( Index, CameraView.XRot );
            CameraList.SetYRot( Index, CameraView.YRot );
            CameraList.SetZRot( Index, CameraView.ZRot );

            // Create the string to add to the list box
            sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)CameraView.XPos,
                                                  (int)CameraView.YPos,
                                                  (int)CameraView.ZPos,
                                                  (int)CameraView.XRot,
                                                  (int)CameraView.YRot,
                                                  (int)CameraView.ZRot );
        }

        // Add the string to the list box
        ListBox_AddString( hSubCtrl[ LST_CAM_MOVE ], StrTmp );

        // Display the total number of camera entries
        Edit_DisplayValue( GetDlgItem( hwnd, STC_CAMERA_COUNT ), CameraList.Count() );

        // Set the flag that indicates we have made a change
        CameraListSave = TRUE;
    }

}   // BtnClk_RecordCameraOffset


/************************************************************************
*    FUNCTION NAME:         BtnClk_ChangeCameraOffset                                                             
*
*    DESCRIPTION:           Change the selected camera offset
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::BtnClk_ChangeCameraOffset( HWND hwnd )
{
    char StrTmp[100];
    int Index;

    if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_CAMERA ) ) )
    {
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hSubCtrl[ LST_CAM_MOVE ] );
        if( Index == LB_ERR )
            return;

        // Reset the camera
        cameraStep.Clear();

        // Resave the offset information
        CameraList.SetXPos( Index, CameraView.XPos );
        CameraList.SetYPos( Index, CameraView.YPos );
        CameraList.SetZPos( Index, CameraView.ZPos );
        CameraList.SetXRot( Index, CameraView.XRot );
        CameraList.SetYRot( Index, CameraView.YRot );
        CameraList.SetZRot( Index, CameraView.ZRot );

        // Create the string to add to the list box
        sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)CameraView.XPos,
                                              (int)CameraView.YPos,
                                              (int)CameraView.ZPos,
                                              (int)CameraView.XRot,
                                              (int)CameraView.YRot,
                                              (int)CameraView.ZRot );

        // Delete the string we want to change
        ListBox_DeleteString( hSubCtrl[ LST_CAM_MOVE ], Index );

        // Add the string to the list box
        ListBox_InsertString( hSubCtrl[ LST_CAM_MOVE ], Index, StrTmp );

        // Set the new cursor selection
        ListBox_SetCurSel( hSubCtrl[ LST_CAM_MOVE ], Index + 1 );

        // Animate the selection
        LstClk_CameraMove( hwnd, hSubCtrl[ LST_CAM_MOVE ], CBN_SELCHANGE );

        // Set the flag that indicates we have made a change
        CameraListSave = TRUE;
    }

}   // BtnClk_ChangeCameraOffset


/************************************************************************
*    FUNCTION NAME:         LstClk_CameraMove                                                             
*
*    DESCRIPTION:           Reset the offsets to the selected index
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::LstClk_CameraMove( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int Index, i;

    if( codeNotify == CBN_SELCHANGE && CameraList.Count() )
    {            
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hwndCtl );
        if( Index == LB_ERR )
            return;

        // Reset the camera
        cameraStep.Clear();

        // Are we doing an accumulitive camera move?
        if( Button_GetCheck( GetDlgItem( hParentWnd, CHK_MOVE_ACCUMULATIVE ) ) )
        {
            CameraMatrix.InitilizeMatrix();

            for( i = 0; i < Index+1; ++i )
            {
                CameraMatrix.Translate( CameraList.GetXPos( i ), CameraList.GetYPos( i ), CameraList.GetZPos( i ) );
                CameraMatrix.Rotate( CameraList.GetXRot( i ), CameraList.GetYRot( i ), CameraList.GetZRot( i ) );
            }
        }
        else
        {
            // Get the saved animation offset
            CameraView.XPos = CameraList.GetXPos( Index );
            CameraView.YPos = CameraList.GetYPos( Index );
            CameraView.ZPos = CameraList.GetZPos( Index );
            CameraView.XRot = CameraList.GetXRot( Index );
            CameraView.YRot = CameraList.GetYRot( Index );
            CameraView.ZRot = CameraList.GetZRot( Index );
        }

        // Render the scene
        if( PlyList.Count() )
        {
            RenderScene();

            // Show it
            VBuffer.Draw( hViewWndDC );
        }

        // Show the camera coordinates
        Edit_DisplayValue( hSubCtrl[ X_TRAN_CAM ], CameraView.XPos );
        Edit_DisplayValue( hSubCtrl[ Y_TRAN_CAM ], CameraView.YPos );
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], CameraView.ZPos );
        Edit_DisplayValue( hSubCtrl[ X_ROT_CAM ], CameraView.XRot );
        Edit_DisplayValue( hSubCtrl[ Y_ROT_CAM ], CameraView.YRot );
        Edit_DisplayValue( hSubCtrl[ Z_ROT_CAM ], CameraView.ZRot );
    }

}   // LstClk_CameraMove


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

BOOL C3DAnimator::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{
    BOOL Result = FALSE;
    int i;
    RECT rect;
    char TmpPath[MAX_PATH+1];

    // Set the dialog windows caption icon to the app icon
    SetClassLong( hwnd, GCL_HICON, (LONG) LoadIcon( hInst, "AppIcon" ) );

    // Save the handle to the parent window
    hParentWnd = hwnd;

    // Update the window caption
    SetWindowText( hwnd, WINDOW_TITLE );

    // Get the bit depth of the system, be it 8, 16, 24, 32
    SystemBitDepth = GetSystemBitDepth( hwnd );

    // get the path to the exe file
    ParsePathFromCmdLine( GetCommandLine(), TmpPath );
    CropOutPath( TmpPath, AppPath );

    // Get the config settings
    Settings.GetPrivateString( INI_FILE_NAME, INI_SECTION_NAME );

    // Get the rect of the midia window
    GetWindowRect( hwnd, &rect );
    rect.right -= rect.left;
    rect.bottom -= rect.top;

    // Center the window if it is way off the screen
    if( Settings.GetWindowX() < -rect.right || Settings.GetWindowY() < -rect.bottom ||
        Settings.GetWindowX() > GetSystemMetrics( SM_CXSCREEN ) || Settings.GetWindowY() > GetSystemMetrics( SM_CYSCREEN ) )
    {
        CenterWindow( hwnd, rect.right, rect.bottom );
    }
    else
        // Put a window at a given position and size
        MoveWindow( hwnd, Settings.GetWindowX(), Settings.GetWindowY(), rect.right, rect.bottom, TRUE );

    // Subclass all the needed windows controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS; ++i )
    {
        // Save the control handle for later use
        hSubCtrl[ i ] = GetDlgItem( hwnd, EDT_X_TRAN_CAM + i );

        // Subclass the windows control
        OldSubCtrlProc[ i ] = (WNDPROC)SetWindowLong( hSubCtrl[ i ], GWL_WNDPROC, 
                                                      LONG( SubClassProc ) );
    }

    // Get the view window DC
    hViewWndDC = GetDC( hSubCtrl[ STC_VIEWER ] );

    // Init the class using the screens DC. That way it doesn't have to make one
    VBuffer.InitActiveDC( hViewWndDC );

    // Get the rect of the view window
    GetClientRect( hSubCtrl[ STC_VIEWER ], &ViewRect );

    // Allocate the Z Buffer
    ZBuffer = new int [ ViewRect.right * ViewRect.bottom ];
    if( !ZBuffer )
    {
        PostMsg( "Error", "Error Allocating Z Buffer." );
        goto HANDLE_ERROR;
    }

    // Zero out the ZBuffer
    memset( ZBuffer, 0, sizeof(int)*(ViewRect.right * ViewRect.bottom) );

    // One time initilization of the cos and sin tables
    CameraMatrix.InitCosSinTables();

    // Get the rect of the static window
    GetClientRect( GetDlgItem( hwnd, STC_TEXTURE_PREVIEW ), &rect );

    // Make a buffer the size of the static window
    textPrevBuf.CreateStandardBuffer( rect.right, rect.bottom );

    // Reset the buffer with the window color. 
    textPrevBuf.ColorRect( textPrevBuf.GetRect(), (COLORREF)GetSysColor( COLOR_BTNFACE ) );

    /**********************************
    *     Setup the controls
    ***********************************/

    // Display the camera coordinates
    Edit_DisplayValue( hSubCtrl[ X_TRAN_CAM ], 0 );
    Edit_DisplayValue( hSubCtrl[ Y_TRAN_CAM ], 0 );
    if( Settings.GetCameraZeroPoint() )
    {
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], 0 );
    }
    else
    {
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], CAMERA_Z_OFFSET );
        // Set the camera back a little bit
        CameraMatrix.Translate( 0, 0, CAMERA_Z_OFFSET );   
        CameraView.ZPos = CAMERA_Z_OFFSET;
    }
    Edit_DisplayValue( hSubCtrl[ X_ROT_CAM ], 0 );
    Edit_DisplayValue( hSubCtrl[ Y_ROT_CAM ], 0 );
    Edit_DisplayValue( hSubCtrl[ Z_ROT_CAM ], 0 );

    // Show the frames per second
    Edit_DisplayValue( GetDlgItem( hwnd, EDT_FPS ), Settings.GetFramesPerSec() );

    // Set the camers radio buttons
    if( Settings.GetMoveCamera() )
        Button_SetCheck( GetDlgItem( hwnd, RDO_MOVE_CAMERA ), TRUE );
    else
        Button_SetCheck( GetDlgItem( hwnd, RDO_MOVE_OBJECT ), TRUE );

    // Set the animation mode check box
    Button_SetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ), Settings.GetTextureMode() );

    // Set the camera zero point check box
    Button_SetCheck( GetDlgItem( hwnd, CHK_MOVE_ACCUMULATIVE ), Settings.GetCameraZeroPoint() );

    Edit_DisplayValue( hSubCtrl[ EDT_CAM_SIZE ], Settings.GetCameraSize() );
    Edit_DisplayValue( hSubCtrl[ EDT_Z_DIS ], Settings.GetMaxZDistance() );
    Edit_DisplayValue( hSubCtrl[ EDT_LIGHT_DIST ], Settings.GetMaxLightDistance() );
    Edit_DisplayValue( GetDlgItem( hwnd, EDT_XYTRANS_MOVE_AMT ), Settings.GetXYTransAmount() );
    Edit_DisplayValue( GetDlgItem( hwnd, EDT_ZTRANS_MOVE_AMT ), Settings.GetZTransAmount() );

    // Check the menu if it is checked
    if( Settings.GetCompressedList() )
        CheckMenuItem( GetMenu( hwnd ), MNU_EXPORT_COMPRESSED, MF_CHECKED );

    // Disable the controls
    EnableControls( hwnd, FALSE );

    Result = TRUE;

HANDLE_ERROR:;

    if( !Result )
        DestroyWindow( hwnd );

    return Result;

}   // Starter_OnCreate


/************************************************************************
*    FUNCTION NAME:         QuitProgram                                                             
*
*    DESCRIPTION:           One quick and easy place to quit the program.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::QuitProgram( HWND hwnd )
{
    int i;
    RECT r;

    // Check if we need to save any data
    for( i = 0; i < PlyList.Count(); ++i )
        if( !CheckToSaveObjectData( hwnd, i ) )
            return;

    // See if we need to save any camera movements
    if( !CheckToSaveCameraMove( hwnd ) )
        return;

    // Get the XY of the window
    GetWindowRect( hwnd, &r );

    // Save the position of the window
    Settings.SetWindowX( r.left );
    Settings.SetWindowY( r.top );

    // Save the values of the edit controls
    Settings.SetCameraSize( Edit_GetValue( hSubCtrl[ EDT_CAM_SIZE ] ) );
    Settings.SetMaxZDistance( Edit_GetValue( hSubCtrl[ EDT_Z_DIS ] ) );
    Settings.SetMaxLightDistance( Edit_GetValue( hSubCtrl[ EDT_LIGHT_DIST ] ) );

    Settings.SetXYTransAmount( Edit_GetValue( GetDlgItem( hwnd, EDT_XYTRANS_MOVE_AMT ) ) );
    Settings.SetZTransAmount( Edit_GetValue( GetDlgItem( hwnd, EDT_ZTRANS_MOVE_AMT ) ) );

    // Save the radio button selection
    if( Button_GetCheck( GetDlgItem( hwnd, RDO_MOVE_CAMERA ) ) )
        Settings.SetMoveCamera( TRUE );
    else
        Settings.SetMoveCamera( FALSE );

    // Save the animation mode check box
    Settings.SetTextureMode( Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) );

    // Save the camera zero point check box
    Settings.SetCameraZeroPoint( Button_GetCheck( GetDlgItem( hwnd, CHK_MOVE_ACCUMULATIVE ) ) );

    // Save the frames per second
    Settings.SetFramesPerSec( Edit_GetValue( GetDlgItem( hwnd, EDT_FPS ) ) );

    // Save the program settings
    Settings.WritePrivateString( INI_FILE_NAME, INI_SECTION_NAME );

    DestroyWindow( hwnd );

}   // QuitProgram


/************************************************************************
*    FUNCTION NAME:         MnuClk_OpenPolygonObj                                                             
*
*    DESCRIPTION:           Open polygon objects to edit
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::MnuClk_OpenPolygonObj( HWND hwnd )
{
    char TmpProjFile[MAX_PATH];

    // Terminate the temp string
    TmpProjFile[0] = 0;

    // Show the windows get file box
    if( GetFileName( hwnd, hInst, TmpProjFile, OPEN_FILE_CAPTION,
                     Settings.GetProjectPath(), PROJECT_FILE_MASK, PROJECT_EXT ) )
    {
        // Save the file open path for next time
        CropOutPath( TmpProjFile, Settings.GetProjectPath() );

        // Load the polygon object
        if( !LoadPolygonObject( hwnd, TmpProjFile ) )
            return FALSE;

        // Enable the controls
        if( PlyList.Count() == 1 )
            EnableControls( hwnd, TRUE );
    }

    return TRUE;

}   // MnuClk_OpenPolygonObj


/************************************************************************
*    FUNCTION NAME:         OpenCameraMoveFile                                                             
*
*    DESCRIPTION:           Open camera movement file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::OpenCameraMoveFile( HWND hwnd )
{
    char TmpProjFile[MAX_PATH];
    char StrTmp[100];
    int i;

    // Terminate the temp string
    TmpProjFile[0] = 0;

    // Check to see if we need to save
    if( !CheckToSaveCameraMove( hwnd ) )
        return FALSE;

    // Show the windows get file box
    if( GetFileName( hwnd, hInst, TmpProjFile, OPEN_OBJ_MOVE_COOR,
                     Settings.GetProjectPath(), COORDINATE_FILE_MASK, COORDINATE_EXT ) )
    {
        // Save the file open path for next time
        Settings.SetProjectPath( TmpProjFile );

        // Copy over the file name
        strcpy( CameraListFileName, TmpProjFile );

        // Free the camera data
        CameraList.Free();

        // Reset the list box
        ListBox_ResetContent( hSubCtrl[ LST_CAM_MOVE ] );

        // Load the polygon object
        if( !CameraList.LoadFromFile( CameraListFileName, OPEN_EXISTING ) )
            return FALSE;

        // Load the list into the list box
        for( i = 0; i < CameraList.Count(); ++i )
        {
            // Save the offset information
            CameraView.XPos = CameraList.GetXPos( i );
            CameraView.YPos = CameraList.GetYPos( i );
            CameraView.ZPos = CameraList.GetZPos( i );
            CameraView.XRot = CameraList.GetXRot( i );
            CameraView.YRot = CameraList.GetYRot( i );
            CameraView.ZRot = CameraList.GetZRot( i );

            // Create the string to add to the list box
            sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)CameraView.XPos,
                                                  (int)CameraView.YPos,
                                                  (int)CameraView.ZPos,
                                                  (int)CameraView.XRot,
                                                  (int)CameraView.YRot,
                                                  (int)CameraView.ZRot );
            // Add the string to the list box
            ListBox_AddString( hSubCtrl[ LST_CAM_MOVE ], StrTmp );
        }

        // Display the total number of camera entries
        Edit_DisplayValue( GetDlgItem( hwnd, STC_CAMERA_COUNT ), i );        
    }

    return TRUE;

}   // OpenCameraMoveFile


/************************************************************************
*    FUNCTION NAME:         OpenObjMoveFile                                                             
*
*    DESCRIPTION:           Open object movement file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::OpenObjMoveFile( HWND hwnd )
{
    char TmpProjFile[MAX_PATH];
    char StrTmp[100];
    int i;

    // Terminate the temp string
    TmpProjFile[0] = 0;

    // Check to see if we need to save
    if( !CheckToSaveObjMove( hwnd, ActiveIndex ) )
        return FALSE;

    // Show the windows get file box
    if( GetFileName( hwnd, hInst, TmpProjFile, OPEN_OBJ_MOVE_COOR,
                     Settings.GetProjectPath(), COORDINATE_FILE_MASK, COORDINATE_EXT ) )
    {
        // Save the file open path for next time
        Settings.SetProjectPath( TmpProjFile );

        // Copy over the file name
        strcpy( PlyList.GetMoveFileName( ActiveIndex ), TmpProjFile );

        // Free the move list
        PlyList.GetPolyOffList( ActiveIndex )->Free();

        // Reset the list box
        ListBox_ResetContent( hSubCtrl[ LST_OFF_MOVE ] );

        // Load the polygon object
        if( !PlyList.GetPolyOffList( ActiveIndex )->LoadFromFile( PlyList.GetMoveFileName( ActiveIndex ), OPEN_EXISTING ) )
            return FALSE;

        // Load the list into the list box
        for( i = 0; i < PlyList.GetPolyOffList( ActiveIndex )->Count(); ++i )
        {
            // Create the string to add to the list box
            sprintf( StrTmp, "%d,%d,%d,%d,%d,%d", (int)PlyList.GetPolyOffList( ActiveIndex )->GetXPos( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetYPos( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetZPos( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetXRot( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetYRot( i ),
                                                  (int)PlyList.GetPolyOffList( ActiveIndex )->GetZRot( i ) );
            // Add the string to the list box
            ListBox_AddString( hSubCtrl[ LST_OFF_MOVE ], StrTmp );
        }
        
        // Display the total number of object move entries
        Edit_DisplayValue( GetDlgItem( hwnd, STC_OBJECT_MV_COUNT ), i );        
    }

    return TRUE;

}   // OpenObjMoveFile


/************************************************************************
*    FUNCTION NAME:         EnableControls                                                             
*
*    DESCRIPTION:           Enable or disable controls
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              BOOL Enable     - Enable or disable the menu 
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

void C3DAnimator::EnableControls( HWND hwnd, BOOL Enable )
{
    int i;
    HMENU hMenu;

    // Get the menu
    hMenu = GetMenu(hwnd);

    // Enable/disable menu items
    for( i = 0; i < MAX_MENU_CONTROLS; ++i )
        EnableMenuItem( hMenu, MNU_SAVE_TXT_LIBRARY+i, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

    // Enable/disable all the subclassed controls
    for( i = 0; i < MAX_SUBCLASS_CTRLS-2; ++i )
        EnableWindow( hSubCtrl[ i ], Enable );

    // Enable/disable the buttons
    for( i = 0; i < MAX_BUTTON_CONTROLS; ++i )
        EnableWindow( GetDlgItem( hwnd, RDO_MOVE_CAMERA + i ), Enable );

    // Enable/disable the statics
    for( i = 0; i < MAX_STATIC_CONTROLS; ++i )
        EnableWindow( GetDlgItem( hwnd, GRP_MOVEMENT_BOX + i ), Enable );

    // Enable/disable the texture list
    if( !Enable || Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) )
    {
        for( i = 0; i < MAX_TEXTURE_EDIT; ++i )
            EnableWindow( GetDlgItem( hwnd, LST_TEXTURES + i ), Enable );

        // Enable texture menu items
        for( i = 0; i < MAX_TEXT_MENU_CONTROLS; ++i )
            EnableMenuItem( hMenu, MNU_DELETE_TEXTURE+i, (Enable ? MF_ENABLED : MF_GRAYED|MF_DISABLED) );

        EnableWindow( hSubCtrl[ STC_PREVIEW ], Enable );
    }

    // Erase the contents the texture list box
    if( !Enable )
        ListBox_ResetContent( GetDlgItem( hwnd, LST_TEXTURES ) );

    // Enable/disable the buttons
    for( i = 0; i < MAX_EDIT; ++i )
        EnableWindow( GetDlgItem( hwnd, EDT_XYTRANS_MOVE_AMT + i ), Enable );

    if( Enable )
        SetFocus( hSubCtrl[ X_TRAN_CAM ] );

}   // EnableControls


/************************************************************************
*    FUNCTION NAME:         mnuClose                                                             
*
*    DESCRIPTION:           Close out the selected object
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_CloseObject( HWND hwnd )
{    
    int Index, ListBoxCount, i;
    char StrTmp[100], CheckStr[100];

    // Make sure we have a selection to the animation
    Index = ListBox_GetCurSel( hSubCtrl[ LST_POLYOBJS ] );
    if( Index == LB_ERR )
        return;

    // clear out the selection list
    SelectedPlyLst.Free();

    // Check if we need to save any data
    CheckToSaveObjectData( hwnd, Index );

    // Get the string listed in the listbox
    ListBox_GetText( hSubCtrl[ LST_POLYOBJS ], Index, StrTmp );

    // Need to know how many items in the list box
    ListBoxCount = ListBox_GetCount( hSubCtrl[ LST_MOVE ] );

    // Go through all the strings in the anim to run list box to 
    // be sure we are not adding duplicate animations
    for( i = 0; i < ListBoxCount; ++i )
    {
        ListBox_GetText( hSubCtrl[ LST_MOVE ], i, CheckStr );

        if( strcmpi( StrTmp, CheckStr ) == 0 )
        {
            // Delete the string from the list box
            ListBox_DeleteString( hSubCtrl[ LST_MOVE ], i );

            // If the list box has no items in it, disable all the controls
            if( ListBox_GetCount( hSubCtrl[ LST_MOVE ] ) == 0 )
            {
                Edit_Enable( GetDlgItem( hwnd, EDT_FPS ), FALSE );
                Button_Enable( GetDlgItem( hwnd, BTN_PLAY_MOVE ), FALSE );
                ListBox_Enable( hSubCtrl[ LST_MOVE ], FALSE );
            }

            break;
        }
    }

    // Delete the selected polygon object
    PlyList.Delete( Index );

    // Delete the string from the list box
    ListBox_DeleteString( hSubCtrl[ LST_POLYOBJS ], Index );

    // Reset the object movement list box if there was anything in it
    ListBox_ResetContent( hSubCtrl[ LST_OFF_MOVE ] );

    // Dec the active index 
    if( ActiveIndex >= PlyList.Count() )
        ActiveIndex = PlyList.Count() - 1;

    // Render the scene
    if( PlyList.Count() )
    {
        // Select the first polgon in the list
        ListBox_SetCurSel( hSubCtrl[ LST_POLYOBJS ], 0 );

        // Render the remaining polygons
        RenderScene();
        
        // Load the texture list box with the selected polygon's objects
        LstClk_PolyObjListBox( hwnd, hSubCtrl[ LST_POLYOBJS ], CBN_SELCHANGE );
    }
    else
    {
        // Erase the video buffer
        ClearVideoBuffer();

        EnableControls( hwnd, FALSE );
        ActiveIndex = -1;
        CameraView.Clear();
        cameraStep.Clear();
        CameraMatrix.InitilizeMatrix();

        // Display the object coordinates
        Edit_DisplayValue( hSubCtrl[ X_TRAN_CAM ], 0 );
        Edit_DisplayValue( hSubCtrl[ Y_TRAN_CAM ], 0 );
        Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], 0 );
        Edit_DisplayValue( hSubCtrl[ X_ROT_CAM ], 0 );
        Edit_DisplayValue( hSubCtrl[ Y_ROT_CAM ], 0 );
        Edit_DisplayValue( hSubCtrl[ Z_ROT_CAM ], 0 );
    }

    // Force a repait to the view window
    RepaintViewWnd();
    
    // Display the object coordinates
    Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ X_ROT_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Y_ROT_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Z_ROT_OBJ ], 0 );            

}   // MnuClk_CloseObject


/************************************************************************
*    FUNCTION NAME:         CheckToSaveObjectData                                                             
*
*    DESCRIPTION:           See if we need to save the polygon object data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::CheckToSaveObjectData( HWND hwnd, int Index )
{
    // Check to see if they need to save their changes
    return ( CheckToSaveObjCDR( hwnd, Index ) && CheckToSaveObjMove( hwnd, Index ) && 
             CheckToSaveObjTxtLibrary( hwnd, Index ) && CheckToSavePlyObj( hwnd, Index ) );

}   // CheckToSaveObjectData


/************************************************************************
*    FUNCTION NAME:         CheckToSaveObjCDR                                                             
*
*    DESCRIPTION:           See if we need to save the polygon object texture data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::CheckToSaveObjCDR( HWND hwnd, int Index )
{
    BOOL Result = TRUE;
    int MsgResult;

    // Check to see if they need to save their changes
    if( PlyList.GetCDRLive( Index ) )
    {
        MsgResult = MessageBox( hwnd, "Your texture coordinate settings has not been saved. Do you want to save?", 
                             "Save texture coordinate Changes?", MB_YESNOCANCEL );

        // Did they choose to save the project?
        if( MsgResult == IDYES )
        {
            if( !SaveCRDfile( hwnd, Index ) )
                Result = FALSE;
        }
        else if( MsgResult == IDCANCEL )
            Result = FALSE;
    }

    return Result;

}   // CheckToSaveObjCDR


/************************************************************************
*    FUNCTION NAME:         CheckToSavePlyObj                                                             
*
*    DESCRIPTION:           See if we need to save the polygon object data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::CheckToSavePlyObj( HWND hwnd, int Index )
{
    BOOL Result = TRUE;
    int MsgResult;

    // Check to see if they need to save their changes
    if( PlyList.GetPlyObjLive( Index ) )
    {
        MsgResult = MessageBox( hwnd, "Your polygon object file has not been saved. Do you want to save?", 
                             "Save polygon object Changes?", MB_YESNOCANCEL );

        // Did they choose to save the project?
        if( MsgResult == IDYES )
        {
            if( !SavePlyObjfile( hwnd, Index ) )
                Result = FALSE;
        }
        else if( MsgResult == IDCANCEL )
            Result = FALSE;
    }

    return Result;

}   // CheckToSavePlyObj


/************************************************************************
*    FUNCTION NAME:         CheckToSaveObjTxtLibrary                                                             
*
*    DESCRIPTION:           See if we need to save the polygon texture library
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::CheckToSaveObjTxtLibrary( HWND hwnd, int Index )
{
    BOOL Result = TRUE;
    int MsgResult;

    // Check to see if they need to save their changes
    if( PlyList.GetTxtLibaryLive( Index ) )
    {
        MsgResult = MessageBox( hwnd, "Your Texture Library has not been saved. Do you want to save?", 
                             "Save Texture Library Changes?", MB_YESNOCANCEL );

        // Did they choose to save the project?
        if( MsgResult == IDYES )
        {
            if( !SaveTextureLibrary( hwnd, Index ) )
                Result = FALSE;
        }
        else if( MsgResult == IDCANCEL )
            Result = FALSE;
    }

    return Result;

}   // CheckToSaveObjTxtLibrary


/************************************************************************
*    FUNCTION NAME:         CheckToSaveObjMove                                                             
*
*    DESCRIPTION:           See if we need to save the polygon object move list
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::CheckToSaveObjMove( HWND hwnd, int Index )
{
    BOOL Result = TRUE;
    int MsgResult;

    // Check to see if they need to save their changes
    if( PlyList.GetLiveObjMove( Index ) )
    {
        MsgResult = MessageBox( hwnd, "Your object move list has not been saved. Do you want to save?", 
                             "Save object move list Changes?", MB_YESNOCANCEL );

        // Did they choose to save the project?
        if( MsgResult == IDYES )
        {
            if( !SaveObjectMoveFile( hwnd, Index ) )
                Result = FALSE;
        }
        else if( MsgResult == IDCANCEL )
            Result = FALSE;
    }

    return Result;

}   // CheckToSaveObjMove


/************************************************************************
*    FUNCTION NAME:         CheckToSaveCameraMove                                                             
*
*    DESCRIPTION:           See if we need to save the camera movements
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::CheckToSaveCameraMove( HWND hwnd )
{
    BOOL Result = TRUE;
    int MsgResult;

    // Check to see if they need to save their changes
    if( CameraListSave )
    {
        MsgResult = MessageBox( hwnd, "Your camera movements have not been saved. Do you want to save?", 
                             "Save texture Changes?", MB_YESNOCANCEL );

        // Did they choose to save the project?
        if( MsgResult == IDYES )
        {
            if( !SaveCameraMoveFile( hwnd ) )
                Result = FALSE;
        }
        else if( MsgResult == IDCANCEL )
            Result = FALSE;
    }

    return Result;

}   // CheckToSaveCameraMove


/************************************************************************
*    FUNCTION NAME:         SaveCRDfile                                                             
*
*    DESCRIPTION:           Save texture coordinate file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::SaveCRDfile( HWND hwnd, int Index )
{
    char TmpFile[MAX_PATH];

    if( PlyList.Count() && PlyList.GetCDRLive( Index ) )
    {
        // Take the name of the DXF file and replace the extension with crd
        RelpaceFileExtension( PlyList.GetPolyObjFileName( Index ), TmpFile, ".crd" );

        // Write out the new text coordinates
        if( PlyList.GetPolyObjClass( Index )->WriteTextCoord( TmpFile ) )
            // Set the flag that indicates a file change has been made
            PlyList.SetCDRLive( Index, FALSE );
    }

    return TRUE;

}   // SaveCRDfile


/************************************************************************
*    FUNCTION NAME:         SavePlyObjfile                                                             
*
*    DESCRIPTION:           Save the polygon object file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::SavePlyObjfile( HWND hwnd, int Index )
{
    char TmpFile[MAX_PATH];

    if( PlyList.Count() && PlyList.GetPlyObjLive( Index ) )
    {
        // Take the name of the DXF file and replace the extension with crd
        RelpaceFileExtension( PlyList.GetPolyObjFileName( Index ), TmpFile, ".ply" );

        // Write out the new text coordinates
        if( PlyList.GetPolyObjClass( Index )->SavePolygonObjFile( TmpFile ) )
            // Set the flag that indicates a file change has been made
            PlyList.SetPlyObjLive( Index, FALSE );
    }

    return TRUE;

}   // SavePlyObjfile


/************************************************************************
*    FUNCTION NAME:         SaveObjectMoveFile                                                             
*
*    DESCRIPTION:           Save coordinate animation file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::SaveObjectMoveFile( HWND hwnd, int Index )
{
    char *TmpFile;
    char TmpAnimFile[MAX_PATH];

    if( PlyList.Count() && PlyList.GetLiveObjMove( Index ) )
    {
        // Get the pointer to the file name
        TmpFile = PlyList.GetMoveFileName( Index );

        // Check if we have a file name to save with
        if( !TmpFile[0] )
        {
            // Terminate
            TmpAnimFile[0] = 0;

            // Show the windows get file box
            if( GetFileName( hwnd, hInst, TmpAnimFile, SAVE_OBJ_MOVE_COOR, Settings.GetProjectPath(), 
                             COORDINATE_FILE_MASK, COORDINATE_EXT, SAVE_FILENAME_DIALOG_BOX ) )
            {
                // save the file name
                strcpy( PlyList.GetMoveFileName( Index ), TmpAnimFile );
            }
        }

        // Get the pointer to the file name
        TmpFile = PlyList.GetMoveFileName( Index );

        // Check if we have a file name to save with
        if( TmpFile[0] )
            // Write out the new text coordinates
            if( PlyList.GetPolyOffList( Index )->SaveToFile( PlyList.GetMoveFileName( Index ), CREATE_ALWAYS ) )
                // Set the flag that indicates a file change has been made
                PlyList.SetLiveObjMove( Index, FALSE );
    }

    return TRUE;

}   // SaveObjectMoveFile


/************************************************************************
*    FUNCTION NAME:         SaveCameraMoveFile                                                             
*
*    DESCRIPTION:           Save camera coordinate animation file
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd       - Handle of the window receiving the message
*              var2type Var2Name - Description 
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

BOOL C3DAnimator::SaveCameraMoveFile( HWND hwnd )
{
    char TmpAnimFile[MAX_PATH];
    BOOL Result = FALSE;

    if( CameraList.Count() )
    {
        // Check if we have a file name to save with
        if( !CameraListFileName[0] )
        {
            // Terminate
            TmpAnimFile[0] = 0;

            // Show the windows get file box
            if( GetFileName( hwnd, hInst, TmpAnimFile, SAVE_OBJ_MOVE_COOR, Settings.GetProjectPath(), 
                             COORDINATE_FILE_MASK, COORDINATE_EXT, SAVE_FILENAME_DIALOG_BOX ) )
            {
                // save the file name
                strcpy( CameraListFileName, TmpAnimFile );
            }
        }

        // Check if we have a file name to save with
        if( CameraListFileName[0] )
            // Write out the new text coordinates
            Result = CameraList.SaveToFile( CameraListFileName, CREATE_ALWAYS );
    }

    // Reset the save flag
    if( Result )
        CameraListSave = FALSE;

    return Result;

}   // SaveCameraMoveFile


/************************************************************************
*    FUNCTION NAME:         MnuClk_SaveAll                                                             
*
*    DESCRIPTION:           Save Everything
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
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

void C3DAnimator::MnuClk_SaveAll( HWND hwnd )
{
    int i;

    // Check if we need to save any data
    for( i = 0; i < PlyList.Count(); ++i )
    {
        // Save Texture Coordinate file
        SaveCRDfile( hwnd, i );

        // Save the texture library
        SaveTextureLibrary( hwnd, i );

        // Save any object movements
        SaveObjectMoveFile( hwnd, i );

        // Save any Polygon object data
        SavePlyObjfile( hwnd, i );
    }

    // Save the camera movement file
    SaveCameraMoveFile( hwnd );

}   // MnuClk_SaveAll


/************************************************************************
*    FUNCTION NAME:         LoadPolygonObject                                                             
*
*    DESCRIPTION:           Setup the polyObject
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

BOOL C3DAnimator::LoadPolygonObject( HWND hwnd, char *ProjectName )
{
    char TmpStr[MAX_PATH+1];
    BOOL Result = FALSE;

    // Save the active index
    ActiveIndex = PlyList.Count();

    // Add a polygon list object
    if( !PlyList.Add() )
    {
        PostMsg( "Error", "Error allocating polygon list." );
        goto HANDLE_ERROR;
    }

    // Store the file name
    strcpy( PlyList.GetPolyObjFileName( ActiveIndex ), ProjectName );

    // Load the polygon object PLY or DXF file
    if( !LoadPlyObjFile( ProjectName ) )
        goto HANDLE_ERROR;

    // Load the texture library
    if( !LoadTextureLibrary( hwnd, ActiveIndex ) )
        goto HANDLE_ERROR;

    // Create the buffer only once
    if( VBuffer.IsEmpty() )
    {
        if( !InitVideoBuffer() )
            goto HANDLE_ERROR;

        // Load the polygon object
        //SetupLightSourcePoly();
    }

    // Set the ZBuffer
    PlyList.GetPolyObjClass( ActiveIndex )->SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    PlyList.GetPolyObjClass( ActiveIndex )->SetRenderSize( ViewRect.right, ViewRect.bottom );

    // Set the maxium distance
    PlyList.GetPolyObjClass( ActiveIndex )->SetMaxZDistance( Edit_GetValue( hSubCtrl[ EDT_Z_DIS ] ) );

    // Set the maxium light distance
    PlyList.GetPolyObjClass( ActiveIndex )->SetMaxLightDistance( Edit_GetValue( hSubCtrl[ EDT_LIGHT_DIST ] ) );

    // Set the scale
    PlyList.GetPolyObjClass( ActiveIndex )->SetCameraScale( Edit_GetValue( hSubCtrl[ EDT_CAM_SIZE ] ) );

    // Set the video buffer
    PlyList.GetPolyObjClass( ActiveIndex )->SetVideoBuffer( VBuffer.GetPtrToDIBsectionBits() );

    // Set the texture data
    PlyList.GetPolyObjClass( ActiveIndex )->SetTextures( PlyList.GetTextClass( ActiveIndex ) );

    // Set the shaded look-up table
    if( SystemBitDepth <= 8 )
        PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to8BitShadedTable() );
    else
    {
        if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
            PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to16BitShadedTable() );
        else
            PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get16to16BitShadedTable() );
    }

    // Load the texture coordinates file or default everything to the first polygon in the library
    LoadTextureCoordinates( ProjectName );

    // Render the scene
    RenderScene();

    // Force a repait to the view window
    RepaintViewWnd();
    
    // Display the object coordinates
    Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ X_ROT_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Y_ROT_OBJ ], 0 );
    Edit_DisplayValue( hSubCtrl[ Z_ROT_OBJ ], 0 );

    // First pull the file name from the path
    CropFileNameFromPath( ProjectName, TmpStr );

    // Now remove the file extension
    RelpaceFileExtension( NULL, TmpStr, "" );

    // Add to the list box
    ListBox_AddString( hSubCtrl[ LST_POLYOBJS ], TmpStr );
    ListBox_SetCurSel( hSubCtrl[ LST_POLYOBJS ], ActiveIndex );

    // Init to -1 incase they select trails
    PlyList.GetRotatePt( ActiveIndex )->ZRot = -1;
    
    Result = TRUE;
    
HANDLE_ERROR:;

    // Delete the allocate polygon object because we had an error
    if( !Result && ActiveIndex < PlyList.Count() )
        PlyList.Delete( ActiveIndex );    

    return TRUE;

}   // LoadPolygonObject


/************************************************************************
*    FUNCTION NAME:         LoadPlyObjFile                                                             
*
*    DESCRIPTION:           Load the polygon object file or load the DXF
*                           file and convert it to our file format
*                           
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

BOOL C3DAnimator::LoadPlyObjFile( char *ProjectPath )
{
    char TmpFile[MAX_PATH+1];
    int volatile FileType;

    // Get the file type
    FileType = GetFileType( ProjectPath );

    // Did they select the PLY file type to load?
    if( FileType == FILE_TYPE_PLY )
    {
        if( !PlyList.GetPolyObjClass( ActiveIndex )->LoadPolygonObjFile( ProjectPath ) )
        {
            PostMsg( "Error", "Error loading Polygon object file." );
            return FALSE;           
        }
    }

    // Did they select the DXF file type to load?
    else if( FileType == FILE_TYPE_DXF )
    {
        // If not, load the DXF file
        if( !PlyList.GetPolyObjClass( ActiveIndex )->LoadDXF( ProjectPath ) )
        {
            PostMsg( "Error", "Error loading DXF file." );
            return FALSE;           
        }
        else
        {
            // Take the name of the file and replace the extension with bin
            RelpaceFileExtension( ProjectPath, TmpFile, ".ply" );

            // Write the new polygon object file
            PlyList.GetPolyObjClass( ActiveIndex )->SavePolygonObjFile( TmpFile );
        }
    }
    else
        return FALSE;

    return TRUE;

}   // LoadPlyObjFile


/************************************************************************
*    FUNCTION NAME:         LoadTextureCoordinates                                                             
*
*    DESCRIPTION:           Load the texture coordinates file. This
*                           file holds information about each polygon
*                           
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

void C3DAnimator::LoadTextureCoordinates( char *ProjectPath )
{
    char TmpFile[MAX_PATH+1];

    // Take the name of the DXF file and replace the extension with crd
    RelpaceFileExtension( ProjectPath, TmpFile, ".crd" );

    // Load the file that holds the texture coordinates and other panel information
    if( !PlyList.GetPolyObjClass( ActiveIndex )->ReadTextCoord( TmpFile ) )
    {
        // Initialize default texture coords:
        PlyList.GetPolyObjClass( ActiveIndex )->InitDefText();

		// indicate we need to save the coordinates file
		PlyList.SetCDRLive( ActiveIndex, TRUE );
    }

}   // LoadTextureCoordinates


/************************************************************************
*    FUNCTION NAME:         LoadTextureLibrary                                                             
*
*    DESCRIPTION:           Load the texture library for this polygon object
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

BOOL C3DAnimator::LoadTextureLibrary( HWND hwnd, int PolyObjIndex )
{
    char TmpFile[MAX_PATH+1];
    TLoadRes LoadRes;
    int i;

    // Take the name of the file and replace the extension with lst
    RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".lst" );
        
    // Load the texture list file
    PlyList.GetTextStrList( PolyObjIndex )->LoadFromFile( TmpFile, OPEN_EXISTING );

    // Warn them if there are any missing textures
    for( i = 0; i < PlyList.GetTextStrList( PolyObjIndex )->Count(); ++i )
    {
        // Check that we are not missing any files
        if( !SearchPath( Settings.GetProjectPath(), PlyList.GetTextStrList( PolyObjIndex )->GetString( i ), NULL, NULL, NULL, NULL ) )
        {
            PostMsg( "Error Loading Texture List", "Missing Texture File (%s).", PlyList.GetTextStrList( PolyObjIndex )->GetString( i ) );
            break;
        }
    }

    // Take the name of the file and replace the extension with bt
    RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".bt" );
    
    // Read the texture data if it is there.
    if( PlyList.GetTextClass( PolyObjIndex )->ReadTexture( TmpFile ) )
    {
        // Only load in the 8 bit related shaded tables
        if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
        {        
            // Take the name of the DXF file and replace the extension with st8
            RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".st8" );

            // Read the 8 bit shaded table
            if( !PlyList.GetShadedTableClass( PolyObjIndex )->LoadTable8to8bit( TmpFile ) )
            {
                // Generate the shaded table  150.0F, 150.0F, 150.0F
                PlyList.GetShadedTableClass( PolyObjIndex )->GenTable( 0.0F, 0.0F, 0.0F, 
                                             PlyList.GetTextClass( PolyObjIndex )->TMap[ 0 ].Palette );

                // Save the 8 bit shaded table
                PlyList.GetShadedTableClass( PolyObjIndex )->SaveTable8to8bit( TmpFile );
            }

            // Take the name of the DXF file and replace the extension with s16
            RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".s16" );

            // Read the 16 bit shaded table
            if( !PlyList.GetShadedTableClass( PolyObjIndex )->LoadTable8to16bit( TmpFile ) )
            {
                // Generate the shaded table  150.0F, 150.0F, 150.0F
                PlyList.GetShadedTableClass( PolyObjIndex )->GenTable( 0.0F, 0.0F, 0.0F, 
                                             PlyList.GetTextClass( PolyObjIndex )->TMap[ 0 ].Palette );

                // Save the 16 bit shaded table
                PlyList.GetShadedTableClass( PolyObjIndex )->SaveTable8to16bit( TmpFile );
            }
        }
        else if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 16 )
        {
            // Check to see if their computer is in 16 bit mode or higher
            if( SystemBitDepth <= 8 )
            {
                PostMsg( "System Color Bit Depth", "The polygon object you are trying to load uses 16 bit color textures. You need to set your display to 16 bit or higher to view this object correctly." );
                return FALSE;
            }
            
            // Take the name of the DXF file and replace the extension with s16
            RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".rgb" );

            // Read the 16 bit RGB shaded table
            if( !PlyList.GetShadedTableClass( PolyObjIndex )->LoadTable16to16bit( TmpFile ) )
            {
                // Generate the shaded table  150.0F, 150.0F, 150.0F
                PlyList.GetShadedTableClass( PolyObjIndex )->GenTable( 0.0F, 0.0F, 0.0F, 
                                             PlyList.GetTextClass( PolyObjIndex )->TMap[ 0 ].Palette );

                // Save the 16 bit shaded table
                PlyList.GetShadedTableClass( PolyObjIndex )->SaveTable16to16bit( TmpFile );
            }
        }

        // Make sure the file list and the texture list have the same number of items
        // If they don't, rebuild the texture library
        if( PlyList.GetTextStrList( PolyObjIndex )->Count() != PlyList.GetTextClass( PolyObjIndex )->Count() )
        {
            // Build a new texture library
            if( !BuildTextureLibrary( hwnd, PolyObjIndex ) )
                return FALSE;
        }

        // Enable the texture controls for this polygon
        PlyList.SetAllowTextCtrls( PolyObjIndex, TRUE );

        // Setup the texture window of we are in texture mode
        if( Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) )
        {
            // Init the texture data window
            DisplayTextureLstFile();
        }
    }
    // We don't have a texture file setup so use the default texture and shaded table from resource
    else
    {
        // Load the shaded table from resource
        if( SystemBitDepth <= 8 )
        {
            if( !LoadTheResource( hInst, "defShadedTable8", "BMPSHD", &LoadRes ) )
            {
                PostMsg( "Error", "Error loading default shaded table." );
                return FALSE;
            }

            // Load the 8 bit shaded table 
            PlyList.GetShadedTableClass( PolyObjIndex )->LoadFromPointer8To8( (BYTE *)LoadRes.pData );
        }
        else
        {
            if( !LoadTheResource( hInst, "defShadedTable16", "BMPSHD", &LoadRes ) )
            {
                PostMsg( "Error", "Error loading default shaded table." );
                return FALSE;
            }

            // Load the 16 bit shaded table 
            PlyList.GetShadedTableClass( PolyObjIndex )->LoadFromPointer8To16( (WORD *)LoadRes.pData );
        }

        // Load the texture library from resource
        if( !LoadTheResource( hInst, "defTexture", "BMPTXT", &LoadRes ) )
        {
            PostMsg( "Error", "Error loading default texture library." );
            return FALSE;
        }

        // Load the texture library 
        if( !PlyList.GetTextClass( PolyObjIndex )->LoadFromPointer( (BYTE *)LoadRes.pData ) )
        {
            PostMsg( "Error", "Error loading default texture library." );
            return FALSE;
        }
    }

    return TRUE;

}   // LoadTextureLibrary


/************************************************************************
*    FUNCTION NAME:         SaveTextureLibrary                                                             
*
*    DESCRIPTION:           Save the texture library for this polygon object
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

BOOL C3DAnimator::SaveTextureLibrary( HWND hwnd, int PolyObjIndex )
{
    char TmpFile[MAX_PATH+1];

    // Get the number of textures
    if( PlyList.GetTextStrList( PolyObjIndex )->Count() && PlyList.GetTxtLibaryLive( PolyObjIndex ) )
    {
        // Take the name of the file and replace the extension with lst
        RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".lst" );
            
        // Save the texture library string list file
        if( !PlyList.GetTextStrList( PolyObjIndex )->SaveToFile( TmpFile, CREATE_ALWAYS ) )
            return FALSE;

        // Take the name of the file and replace the extension with bt
        RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".bt" );
        
        // Save the texture library file
        if( !PlyList.GetTextClass( PolyObjIndex )->WriteTexture( TmpFile ) )
            return FALSE;

        // Take the name of the DXF file and replace the extension with stb
        RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".st8" );

        // Save the 8 bit shaded table
        if( !PlyList.GetShadedTableClass( PolyObjIndex )->SaveTable8to8bit( TmpFile ) )
            return FALSE;

        // Take the name of the DXF file and replace the extension with stb
        RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".s16" );

        // Save the 16 bit shaded table
        if( !PlyList.GetShadedTableClass( PolyObjIndex )->SaveTable8to16bit( TmpFile ) )
            return FALSE;

        // Take the name of the DXF file and replace the extension with stb
        RelpaceFileExtension( PlyList.GetPolyObjFileName( PolyObjIndex ), TmpFile, ".rgb" );

        // Save the 16 bit shaded table
        if( !PlyList.GetShadedTableClass( PolyObjIndex )->SaveTable16to16bit( TmpFile ) )
            return FALSE;

        // Enable the texture controls for this polygon
        PlyList.SetAllowTextCtrls( PolyObjIndex, TRUE );

        // Setup the texture window of we are in texture mode
        if( Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) )
        {
            // Init the texture data window
            DisplayTextureLstFile();
        }
    }

    // Set the flag that indicates a file change has been made
    PlyList.SetTxtLibaryLive( PolyObjIndex, FALSE );

    return TRUE;

}   // SaveTextureLibrary


/************************************************************************
*    FUNCTION NAME:         BuildTextureLibrary                                                             
*
*    DESCRIPTION:           Build the texture library for this polygon object
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

BOOL C3DAnimator::BuildTextureLibrary( HWND hwnd, int PolyObjIndex )
{
    char Path[ MAX_PATH ], BMPPath[ MAX_PATH ];
    int i;

    // Make sure we have something to build with
    if( PlyList.GetTextStrList( PolyObjIndex )->Count() )
    {
        // Strip the path from the file name
        CropOutPath( PlyList.GetPolyObjFileName( PolyObjIndex ), Path );

        // Warn them if there are any missing textures
        for( i = 0; i < PlyList.GetTextStrList( PolyObjIndex )->Count(); ++i )
        {
            // Check that we are not missing any files
            if( !SearchPath( Path, PlyList.GetTextStrList( PolyObjIndex )->GetString( i ), NULL, NULL, NULL, NULL ) )
            {
                PostMsg( "Error Loading Texture List", "Missing Texture File (%s). Can't rebuild texture library.", PlyList.GetTextStrList( PolyObjIndex )->GetString( i ) );
                return FALSE;
            }
        }

        // Allocate a texture library        
        if( !PlyList.GetTextClass( PolyObjIndex )->AllocateTextLibrary( PlyList.GetTextStrList( PolyObjIndex )->Count() ) )
            return FALSE;

        // Build the texture library
        for( i = 0; i < PlyList.GetTextStrList( PolyObjIndex )->Count(); ++i )
        {
            // Combine the path with the file name
            sprintf( BMPPath, "%s\\%s", Path, PlyList.GetTextStrList( PolyObjIndex )->GetString( i ) );

            // Load the texture and build the texture library
            if( !PlyList.GetTextClass( PolyObjIndex )->BuildTextLibrary( i, BMPPath ) )
                return FALSE;
        }

		// Reset the pointer to the polygon object
		PlyList.GetPolyObjClass( PolyObjIndex )->SetTextures( PlyList.GetTextClass( PolyObjIndex ) );

        // Generate the shaded tables 8 and 16 bit color
        PlyList.GetShadedTableClass( PolyObjIndex )->GenTable( 0.0F, 0.0F, 0.0F, 
                                     PlyList.GetTextClass( PolyObjIndex )->TMap[ 0 ].Palette );

        // Set the flag that indicates a file change has been made
        PlyList.SetTxtLibaryLive( PolyObjIndex, TRUE );
    }

    return TRUE;

}   // BuildTextureLibrary


/************************************************************************
*    FUNCTION NAME:         InitVideoBuffer                                                             
*
*    DESCRIPTION:           Init the video buffer
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

BOOL C3DAnimator::InitVideoBuffer()
{
    if( SystemBitDepth <= 8 )
    {
        if( !VBuffer.CreateDIBSectionBuffer( ViewRect.right, -ViewRect.bottom, BIT_COUNT_8, 
             PlyList.GetTextClass( ActiveIndex )->TMap[ 0 ].Palette ) )
        {
            PostMsg( "Error", "Error allocating video buffer." );
            return FALSE;
        }
    }
    else
    {
        if( !VBuffer.CreateDIBSectionBuffer( ViewRect.right, -ViewRect.bottom, BIT_COUNT_16, 
             PlyList.GetTextClass( ActiveIndex )->TMap[ 0 ].Palette ) )
        {
            PostMsg( "Error", "Error allocating video buffer." );
            return FALSE;
        }
    }

    // Init the class using the buffers DC.
    canvas.InitActiveDC( VBuffer.Handle() );

    // Use the Canvas class to allocate and manage a pen.
    // Other than this, it does nothing
    canvas.SetPen( PS_SOLID, 2, RGB( 120, 120, 120 ) );

    return TRUE;

}   // InitVideoBuffer


/************************************************************************
*    FUNCTION NAME:         DisplayTextureLstFile                                                             
*
*    DESCRIPTION:           Load the texture lst file to display the 
*                           different file names in the list
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - File name to load 
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

void C3DAnimator::DisplayTextureLstFile()
{
    int i;

    // Clear out the texture list list box
    ListBox_ResetContent( GetDlgItem( hParentWnd, LST_TEXTURES ) );

    if( PlyList.GetTextStrList( ActiveIndex )->Count() )
    {
        // Display the list of textures
        for( i = 0; i < PlyList.GetTextStrList( ActiveIndex )->Count(); ++i )
            ListBox_AddString( GetDlgItem( hParentWnd, LST_TEXTURES ), PlyList.GetTextStrList( ActiveIndex )->GetString( i ) );

        // Enable the texture controls for this object
        PlyList.SetAllowTextCtrls( ActiveIndex, TRUE );
    }
    else
        PlyList.SetAllowTextCtrls( ActiveIndex, FALSE );

}   // DisplayTextureLstFile


/************************************************************************
*    FUNCTION NAME:              RenderScene
*
*    DESCRIPTION:                Render the scene
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

void C3DAnimator::RenderScene()
{
    DWORD PaintBufferStartTime, PaintBufferEndTime;
    int PolyCount=0, i, j;
    char StrTmp[100];

    // Zero out the ZBuffer
    memset( ZBuffer, 0, sizeof(int)*(ViewRect.right * ViewRect.bottom) );

    // Erase the video buffer
    ClearVideoBuffer();

    // See if any of the objects are being used as a light source
    for( i = 0; i < PlyList.Count(); ++i )
    {
        // If so, reset the light source and break out
        if( PlyList.GetUseAsLightSource( i ) )
        {
            // reset all the light sources
            for( j = 0; j < PlyList.Count(); ++j )
            {
                if( i != j )
                    PlyList.GetPolyObjClass( j )->SetLightSource( -PlyList.GetOffset( i )->XPos, 
                                                                  -PlyList.GetOffset( i )->YPos, 
                                                                  -PlyList.GetOffset( i )->ZPos );
            }
            break;
        }
    }

    // Record the time before we paint the buffer
    PaintBufferStartTime = timeGetTime();

    for( i = 0; i < PlyList.Count(); ++i )
    {
        // Do they have trails turned on?
        if( PlyList.GetTrails( i ) )
        {
            for( j = 0; j < PlyList.GetPolyOffList( i )->Count(); ++j )
                if( j != PlyList.GetRotatePt( i )->ZRot )
                    PolyCount += Render( i, PlyList.GetPolyOffList( i )->GetCameraOffset( j ) );
        }

        // Render the current offset for the object
        PolyCount += Render( i, PlyList.GetOffset( i ) );
    }

    // Record the time after we paint the buffer
    PaintBufferEndTime = timeGetTime();

    // Display the total number of polygons in this object
    sprintf( StrTmp, "Poly/Vert: %u/%u", 
             PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(),
             PlyList.GetPolyObjClass( ActiveIndex )->GetTotalVertCount() );
    Static_SetText( GetDlgItem( hParentWnd, STC_POLYGON_COUNT ), StrTmp );

    // Display the number of polygons used to render this scene
    sprintf( StrTmp, "Render Count:  %d", PolyCount );
    Static_SetText( GetDlgItem( hParentWnd, STC_POLY_RENDER ), StrTmp );

    // Display the number of miliseconds taken to render this scene
    sprintf( StrTmp, "Render Time:  %u", PaintBufferEndTime - PaintBufferStartTime );
    Static_SetText( GetDlgItem( hParentWnd, STC_RENDER_TIME ), StrTmp );

    if( SystemBitDepth <= 8 )
    {
        // Outline any selected polygons
        for( i = 0; i < SelectedPlyLst.Count(); ++i )          
            PlyList.GetPolyObjClass( ActiveIndex )->OutlinePolygon8to8( SelectedPlyLst.GetInt( i ) );
    }
    else
    {
        // Outline any selected polygons
        for( i = 0; i < SelectedPlyLst.Count(); ++i )          
            PlyList.GetPolyObjClass( ActiveIndex )->OutlinePolygon8to16( SelectedPlyLst.GetInt( i ) );
    }

}   // RenderScene


/************************************************************************
*    FUNCTION NAME:              Render
*
*    DESCRIPTION:                Render the polygon objects
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

int C3DAnimator::Render( int Index, PTCameraOffset pObjCamera )
{
    TCamera camera;
    int PolyCount;
    Matrix3D WorldMatrix;

    WorldMatrix.Translate( pObjCamera->XPos, pObjCamera->YPos, pObjCamera->ZPos );

    // Add in the object coordinates
    if( Button_GetCheck( GetDlgItem( hParentWnd, CHK_ROTATE_FROM_PT )   ) )
    {
        WorldMatrix.RotateFromPoint( PlyList.GetRotatePt( Index )->XPos, 
                                     PlyList.GetRotatePt( Index )->YPos, 
                                     PlyList.GetRotatePt( Index )->ZPos,
                                     pObjCamera->XRot, pObjCamera->YRot, pObjCamera->ZRot );
    }
    else
    {
        WorldMatrix.RotateAtDest( pObjCamera->XRot, pObjCamera->YRot, pObjCamera->ZRot );
    }

    // Save the result of the movement that may have involved a rotation which would yeald new offsets
    WorldMatrix.GetPoint( PlyList.GetRotatePtSave( Index )->XPos, 
                          PlyList.GetRotatePtSave( Index )->YPos, 
                          PlyList.GetRotatePtSave( Index )->ZPos );

    // Use the world camera
    if( Button_GetCheck( GetDlgItem( hParentWnd, CHK_MOVE_ACCUMULATIVE ) ) )
    {
        WorldMatrix.MergeMatrix( CameraMatrix.Matrix );
    }
    else
    {
        WorldMatrix.Translate( CameraView.XPos + camera.XPos, CameraView.YPos + camera.YPos, CameraView.ZPos + camera.ZPos );
        WorldMatrix.Rotate( CameraView.XRot + camera.XRot, CameraView.YRot + camera.YRot, CameraView.ZRot + camera.ZRot );
    }

    if( !PlyList.GetWireFrame( Index ) )
    {
        if( SystemBitDepth <= 8 )
        {
            PolyCount = PlyList.GetPolyObjClass( Index )->PaintVBuffer8To8( WorldMatrix, POS_ZBUFFER );
        }
        else
        {
            if( PlyList.GetTextClass( Index )->GetBitCount() == 8 )
                PolyCount = PlyList.GetPolyObjClass( Index )->PaintVBuffer8To16( WorldMatrix, POS_ZBUFFER );
            else
                PolyCount = PlyList.GetPolyObjClass( Index )->PaintVBuffer16To16( WorldMatrix, POS_ZBUFFER );
        }
    }
    else  // Paint wire frame only
    {
        if( SystemBitDepth <= 8 )
            PolyCount = PlyList.GetPolyObjClass( Index )->PaintWireFrame8to8( WorldMatrix, TRUE );
        else
            PolyCount = PlyList.GetPolyObjClass( Index )->PaintWireFrame8to16( WorldMatrix, TRUE );
    }

    return PolyCount;

}   // Render


/************************************************************************
*    FUNCTION NAME:         OnList_PolyObjs                                                             
*
*    DESCRIPTION:           Subclassed list control
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

LRESULT C3DAnimator::OnList_PolyObjs( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Ask if they want to delete the selected animation
        if( IDYES == MessageBox( hParentWnd, "Do you want to close out this polygon object?", "Question", MB_YESNO ) )        
            MnuClk_CloseObject( hParentWnd );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_PolyObjs


/************************************************************************
*    FUNCTION NAME:         OnList_CameraAnim                                                             
*
*    DESCRIPTION:           Subclassed list control
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

LRESULT C3DAnimator::OnList_CameraAnim( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int Index;

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
        if( Index == LB_ERR )
            return Result;

        // Ask if they want to delete the selected animation
        if( IDYES == MessageBox( hParentWnd, "Do you want to delete the selected camera Offset?", "Question", MB_YESNO ) )
        {
            // Delete the index
            CameraList.Delete( Index );

            // Delete the item from the list box
            ListBox_DeleteString( hSubCtrl[ Ctrl ], Index );

            // Set the flag that indicates a file change has been made
            if( CameraList.Count() )
                CameraListSave = TRUE;
            else        
                CameraListSave = FALSE;
                
            // Display the total number of camera entries
            Edit_DisplayValue( GetDlgItem( hParentWnd, STC_CAMERA_COUNT ), CameraList.Count() );               
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_CameraAnim


/************************************************************************
*    FUNCTION NAME:         OnList_ObjOffsetAnim                                                             
*
*    DESCRIPTION:           Subclassed list control
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

LRESULT C3DAnimator::OnList_ObjOffsetAnim( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int Index;

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
        if( Index == LB_ERR )
            return Result;

        // Ask if they want to delete the selected animation
        if( IDYES == MessageBox( hParentWnd, "Do you want to delete the selected object Offset?", "Question", MB_YESNO ) )
        {
            // Delete the index
            PlyList.GetPolyOffList( ActiveIndex )->Delete( Index );

            // Delete the item from the list box
            ListBox_DeleteString( hSubCtrl[ Ctrl ], Index );
            
            // Set the flag that indicates a file change has been made
            if( PlyList.GetPolyOffList( ActiveIndex )->Count() )
                PlyList.SetLiveObjMove( ActiveIndex, TRUE );
            else        
                PlyList.SetLiveObjMove( ActiveIndex, FALSE );
                
            // Display the total number of object move entries
            Edit_DisplayValue( GetDlgItem( hParentWnd, STC_OBJECT_MV_COUNT ), PlyList.GetPolyOffList( ActiveIndex )->Count() );               
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_ObjOffsetAnim


/************************************************************************
*    FUNCTION NAME:         OnList_MOVEMENT                                                             
*
*    DESCRIPTION:           Subclassed list control
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

LRESULT C3DAnimator::OnList_MOVEMENT( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int Index;

    if( Message == WM_KEYUP && wParam == DELETE_KEY )
    {
        // Make sure we have a selection to the animation
        Index = ListBox_GetCurSel( hSubCtrl[ Ctrl ] );
        if( Index == LB_ERR )
            return Result;

        // Delete the item from the list box
        ListBox_DeleteString( hSubCtrl[ Ctrl ], Index );

        // If the list box has no items in it, disable all the controls
        if( ListBox_GetCount( hSubCtrl[ Ctrl ] ) == 0 )
        {
            Edit_Enable( GetDlgItem( hParentWnd, EDT_FPS ), FALSE );
            Button_Enable( GetDlgItem( hParentWnd, BTN_PLAY_MOVE ), FALSE );
            ListBox_Enable( hSubCtrl[ Ctrl ], FALSE );
        }
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnList_MOVEMENT


/************************************************************************
*    FUNCTION NAME:         OnEdit_CameraSize                                                             
*
*    DESCRIPTION:           Subclassed edit control
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

LRESULT C3DAnimator::OnEdit_CameraSize( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int i;

    if( Message == WM_KEYUP && PlyList.Count() )
    {
        for( i = 0; i < PlyList.Count(); ++i )
        {
            // Reset the camera scale
            PlyList.GetPolyObjClass( i )->SetCameraScale( Edit_GetValue( hSubCtrl[ Ctrl ] ) );

            // Reset all the panels
            PlyList.GetPolyObjClass( i )->ReInitPanels();
        }

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_CameraSize


/************************************************************************
*    FUNCTION NAME:         OnEdit_MaxDistance                                                             
*
*    DESCRIPTION:           Subclassed edit control
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

LRESULT C3DAnimator::OnEdit_MaxDistance( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int i;

    if( Message == WM_KEYUP && PlyList.Count() )
    {
        for( i = 0; i < PlyList.Count(); ++i )
        {
            if( Ctrl == EDT_Z_DIS )
            {
                // Reset the max z distance
                PlyList.GetPolyObjClass( i )->SetMaxZDistance( Edit_GetValue( hSubCtrl[ Ctrl ] ) );
            }
            else if( Ctrl == EDT_LIGHT_DIST )
            {
                // Reset the max z distance
                PlyList.GetPolyObjClass( i )->SetMaxLightDistance( Edit_GetValue( hSubCtrl[ Ctrl ] ) );
            }

            // Reset all the panels
            PlyList.GetPolyObjClass( i )->ReInitPanels();
        }

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_MaxDistance


/************************************************************************
*    FUNCTION NAME:         OnEdit_CameraOffsets                                                             
*
*    DESCRIPTION:           Subclassed edit control
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

LRESULT C3DAnimator::OnEdit_Offsets( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;

    if( Message == WM_KEYUP && PlyList.Count() )
    {
        switch( Ctrl )
        {
            // Camera Offsets
            case X_TRAN_CAM:
                CameraView.XPos = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Y_TRAN_CAM:
                CameraView.YPos = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Z_TRAN_CAM:
                CameraView.ZPos = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case X_ROT_CAM:
                CameraView.XRot = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Y_ROT_CAM:
                CameraView.YRot = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Z_ROT_CAM:
                CameraView.ZRot = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;

            // Object Offstes
            case X_TRAN_OBJ:
                PlyList.GetOffset( ActiveIndex )->XPos = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Y_TRAN_OBJ:
                PlyList.GetOffset( ActiveIndex )->YPos = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Z_TRAN_OBJ:
                PlyList.GetOffset( ActiveIndex )->ZPos = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case X_ROT_OBJ:
                PlyList.GetOffset( ActiveIndex )->XRot = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Y_ROT_OBJ:
                PlyList.GetOffset( ActiveIndex )->YRot = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
            case Z_ROT_OBJ:
                PlyList.GetOffset( ActiveIndex )->ZRot = Edit_GetValue( hSubCtrl[ Ctrl ] );
            break;
        }

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnEdit_CameraOffsets


/************************************************************************
*    FUNCTION NAME:         HandleSubClassProc                                                             
*
*    DESCRIPTION:           Handles the subclassing of controls. Check the
*                           window handle to see which control is being trapped.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hwnd    - Handle of the window receiving the message 
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

LRESULT C3DAnimator::HandleSubClassProc( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;
    int i;

    /////////////////////////////////////////////////////
    // Each subclassed control has it's own message handler.
    // That's to keep this function as general and simple
    // as possible. You dig?
    /////////////////////////////////////////////////////

    if( hCtrl == hSubCtrl[ STC_VIEWER ] )
        Result = OnViewWnd( hCtrl, Message, wParam, lParam );

    // We are forcing mouse wheel events into the the view window
    if( Message == WM_MOUSEWHEEL )
        Result = OnViewWnd( hSubCtrl[ STC_VIEWER ], Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ LST_POLYOBJS ] )
        Result = OnList_PolyObjs( LST_POLYOBJS, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ LST_CAM_MOVE ] )
        Result = OnList_CameraAnim( LST_CAM_MOVE, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ LST_OFF_MOVE ] )
        Result = OnList_ObjOffsetAnim( LST_OFF_MOVE, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ LST_MOVE ] )
        Result = OnList_MOVEMENT( LST_MOVE, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ EDT_CAM_SIZE ] )
        Result = OnEdit_CameraSize( EDT_CAM_SIZE, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ EDT_Z_DIS ] )
        Result = OnEdit_MaxDistance( EDT_Z_DIS, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ EDT_LIGHT_DIST ] )
        Result = OnEdit_MaxDistance( EDT_LIGHT_DIST, Message, wParam, lParam );

    else if( hCtrl == hSubCtrl[ STC_PREVIEW ] )
        Result = OnPreviewWnd( STC_PREVIEW, Message, wParam, lParam );

    // Handle the all the offsets
    else
    {       
        for( i = 0; i < MAX_OFFSET_EDT_CTRLS; ++i )
        {
            if( hCtrl == hSubCtrl[ X_TRAN_CAM + i ] )
            {
                Result = OnEdit_Offsets( X_TRAN_CAM + i, Message, wParam, lParam );
                break;
            }
        }
    }

    return Result;

}   // HandleSubClassProc


/************************************************************************
*    FUNCTION NAME:         OnPreviewWnd                                                             
*
*    DESCRIPTION:           Subclassed static control
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

LRESULT C3DAnimator::OnPreviewWnd( int Ctrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    LRESULT Result = FALSE;

    if( Message == WM_PAINT )
    {
        PAINTSTRUCT paintStruct;
        HDC PaintDC;

        // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
        // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
        // message handler. You CAN NOT paint any other way. Even if you have
        // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
        // and EndPaint API calls or bad things will happen,
        PaintDC = BeginPaint( hSubCtrl[ Ctrl ], &paintStruct );

        // Blit the buffer to the screen
        if( !textPrevBuf.IsEmpty() )
        {
            // Load the palette if we are in 8 bit or less mode
            // Init the screens palette
            if( SystemBitDepth <= 8 )
            {
                // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
                // palette will never get deleted
                SelectPalette( PaintDC, loadedBitmap.GetPalette(), FALSE );
                RealizePalette( PaintDC );
            }

            // Blit bitmap buffer to the screen
            textPrevBuf.Draw( PaintDC );
        }

        // Must be called
        EndPaint( hSubCtrl[ Ctrl ], &paintStruct );
    }
    else
        Result = CallWindowProc( OldSubCtrlProc[ Ctrl ], hSubCtrl[ Ctrl ], Message, wParam, lParam );

    return Result;

}   // OnPreviewWnd


/************************************************************************
*    FUNCTION NAME:         LstClk_LoadSelectedTexture                                                             
*
*    DESCRIPTION:           Load the texture
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - File name to load 
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

void C3DAnimator::LstClk_LoadSelectedTexture( HWND hwnd, HWND hwndCtl, UINT codeNotify )
{
    int ListIndex;
    char FileName[MAX_PATH], FilePath[MAX_PATH], TmpStr[MAX_PATH];
    RECT rect;

    // Get the selection index
    ListIndex = ListBox_GetCurSel( hwndCtl );

    if( codeNotify == CBN_SELCHANGE && ListIndex != LB_ERR )
    {
        // Get the text at the selected index
        ListBox_GetText( hwndCtl, ListIndex, FileName );

        // Crop out the path
        CropOutPath( PlyList.GetPolyObjFileName( ActiveIndex ), TmpStr );

        // Piece together the file name and the path
        sprintf( FilePath, "%s\\%s", TmpStr, FileName );

        // Load the bitmap file
        if( loadedBitmap.LoadFromFile( FilePath ) )
        {
            // Create the bitmap. This also frees the previous one
            loadedBitmap.CreateBitmap( STANDARD_DIB );

            // Reset the buffer with the window color. 
            textPrevBuf.ColorRect( textPrevBuf.GetRect(), (COLORREF)GetSysColor( COLOR_WINDOW ) );

            // Do we need to size the image to fit or is it small enough
            // to center in the middle
            if( loadedBitmap.width() <= textPrevBuf.width() && loadedBitmap.height() <= textPrevBuf.height() )
            {
                // Center one image inside the other
                CenterRectInRect( &rect, loadedBitmap.width(), loadedBitmap.height(), textPrevBuf.width(), textPrevBuf.height() );

                // Paint to the buffer
                loadedBitmap.Draw( &textPrevBuf, rect.left, rect.top );
            }
            else
            {
                // Size the image to fit into the buffer. If the image is smaller than the buffer
                // then the image is just centered
                SizeToFitRect( &rect, loadedBitmap.width(), loadedBitmap.height(), textPrevBuf.width(), textPrevBuf.height() );

                // Paint to the buffer
                loadedBitmap.StretchDraw( &textPrevBuf, &rect );
            }

            // Invalidate the whole window so it all gets painted
            InvalidateRect( hSubCtrl[ STC_PREVIEW ], textPrevBuf.GetRect(), TRUE );

            // Display the bitmap info
            sprintf( TmpStr, "%d x %d, %d Bit, %d of %d", 
                     loadedBitmap.width(), loadedBitmap.height(), loadedBitmap.GetBitCount(), ListIndex, ListBox_GetCount( hwndCtl ) );
            Static_SetText( GetDlgItem( hwnd, STC_TXT_INFO ), TmpStr );

            // Set the controls to the info of the selected polygon
            UpdateSelectedPolyInfo( hwnd, ListIndex );
        }
    }
}   // LstClk_LoadSelectedTexture


/************************************************************************
*    FUNCTION NAME:              UpdateSelectedPolyInfo                                                             
*
*    DESCRIPTION:                Update texture controls with selected
*                                polygon info.
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

void C3DAnimator::UpdateSelectedPolyInfo( HWND hwnd, int ListIndex )
{
    int Selection = -1;

    if( PlyList.Count() )
    {
        // To do this properly, only one plygon can be selected
        if( SelectedPlyLst.Count() == 1 )
            Selection = SelectedPlyLst.GetInt( 0 ); 
        
        if( Selection > -1 )
        {
            // Display the polygon's frame count
            Edit_DisplayValue( GetDlgItem( hwnd, EDT_FRAME_COUNT ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetFrameCount() );

            // Display the polygon's frame rate
            Edit_DisplayValue( GetDlgItem( hwnd, EDT_FRAME_RATE ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetFrameRate() );

            // Display the polygon's intensity
            Edit_DisplayValue( GetDlgItem( hwnd, EDT_INTENSITY ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetIntensity() );

            // Display the polygon's transparent attribute
            Button_SetCheck( GetDlgItem( hwnd, CHK_TRANS_POLY ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetTransValue() );

            // Display the polygon's check for backface attribute
            Button_SetCheck( GetDlgItem( hwnd, CHK_NO_CHECK_EXTEN ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetNoCheckExten() );

            // Display the polygon's solid color attribute
            Button_SetCheck( GetDlgItem( hwnd, CHK_SOLID ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetSolidColor() );

            if( ListIndex == PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetTextureHandle() )
            {
                // Display the polygon's U
                Edit_DisplayValue( GetDlgItem( hwnd, EDT_U ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetU() );

                // Display the polygon's V
                Edit_DisplayValue( GetDlgItem( hwnd, EDT_V ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetV() );

                // Display the polygon's U width
                Edit_DisplayValue( GetDlgItem( hwnd, EDT_U_WIDTH ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetUWidth() );

                // Display the polygon's V height
                Edit_DisplayValue( GetDlgItem( hwnd, EDT_V_HEIGHT ), PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetVHeight() );
            }
            else
            {
                Edit_SetText( GetDlgItem( hwnd, EDT_U ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_V ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_U_WIDTH ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_V_HEIGHT ), "0" );
            }
        }
        else
        {
            if( SelectedPlyLst.Count() > 1 )
            {
                // Clear out the edit controls
                Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_COUNT ), "" );
                Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_RATE ), "" );
                Edit_SetText( GetDlgItem( hwnd, EDT_INTENSITY ), "" );

                Edit_SetText( GetDlgItem( hwnd, EDT_U ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_V ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_U_WIDTH ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_V_HEIGHT ), "0" );
            }
            else
            {
                // Set the defaults
                Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_COUNT ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_FRAME_RATE ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_INTENSITY ), "0" );

                Edit_SetText( GetDlgItem( hwnd, EDT_U ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_V ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_U_WIDTH ), "0" );
                Edit_SetText( GetDlgItem( hwnd, EDT_V_HEIGHT ), "0" );
            }
        }
    }
}   // UpdateSelectedPolyInfo


/************************************************************************
*    FUNCTION NAME:              MnuClk_1_To_1_TextureToPolyRatio                                                             
*
*    DESCRIPTION:                Apply each texture to the corrisponding
*								 polygon index
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

void C3DAnimator::MnuClk_1_To_1_TextureToPolyRatio( HWND hwnd )
{
	int i;

	// Make sure we have more textures then polygons
	if( PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount() > PlyList.GetTextStrList( ActiveIndex )->Count() )
	{
		PostMsg( "Error", "Can't have more polygons then loaded textures" );
		return;
	}

	// Each polygon gets the same handle index
	for( i = 0; i < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++i )
		PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( i )->SetTextureHandle( i );

    // Set the flag that indicates a file change has been made
    PlyList.SetCDRLive( ActiveIndex, TRUE );

    // Render the polygon object
    RenderScene();

    // Show it
    VBuffer.Draw( hViewWndDC );

}	// MnuClk_1_To_1_TextureToPolyRatio


/************************************************************************
*    FUNCTION NAME:              BtnClk_ApplyTexture                                                             
*
*    DESCRIPTION:                Apply the texture to the selected polygon
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

void C3DAnimator::BtnClk_ApplyTexture( HWND hwnd, int TextIndex )
{
    WORD PolyInten;
    int i;

    if( PlyList.Count() && SelectedPlyLst.Count() )
    {
        for( i = 0; i < SelectedPlyLst.Count(); ++i )
        {
            // Only apply the texture index if one is selected
            // and all the edit fields have values
            if( TextIndex != LB_ERR && Edit_GetTextLength( GetDlgItem( hwnd, EDT_U ) ) &&
                Edit_GetTextLength( GetDlgItem( hwnd, EDT_V ) ) &&
                Edit_GetTextLength( GetDlgItem( hwnd, EDT_U_WIDTH ) ) &&
                Edit_GetTextLength( GetDlgItem( hwnd, EDT_V_HEIGHT ) ) )
            {
                // Set the selected texture to the selected polygon
                PlyList.GetPolyObjClass( ActiveIndex )->SetTexture( SelectedPlyLst.GetInt( i ), TextIndex, 
                    Edit_GetValue( GetDlgItem( hwnd, EDT_U ) ), 
                    Edit_GetValue( GetDlgItem( hwnd, EDT_V ) ), 
                    Edit_GetValue( GetDlgItem( hwnd, EDT_U_WIDTH ) ), 
                    Edit_GetValue( GetDlgItem( hwnd, EDT_V_HEIGHT ) ) );
            }

            // Set the flag that indicates a file change has been made
            PlyList.SetCDRLive( ActiveIndex, TRUE );

            // Have we set it up for animated textures

            // Save the frame count
            if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_FRAME_COUNT ) ) )
                PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( SelectedPlyLst.GetInt( i ) )->SetFrameCount( Edit_GetValue( GetDlgItem( hwnd, EDT_FRAME_COUNT ) ) );

            // Save the frame rate
            if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_FRAME_RATE ) ) )
                PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( SelectedPlyLst.GetInt( i ) )->SetFrameRate( Edit_GetValue( GetDlgItem( hwnd, EDT_FRAME_RATE ) ) );

            // Save the polygon's transparent attribute
            PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( SelectedPlyLst.GetInt( i ) )->SetTransValue( Button_GetCheck( GetDlgItem( hwnd, CHK_TRANS_POLY ) ) );

            // Save the polygon's two back side check attribute
            PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( SelectedPlyLst.GetInt( i ) )->SetNoCheckExten( Button_GetCheck( GetDlgItem( hwnd, CHK_NO_CHECK_EXTEN ) ) );

            // Save the polygon's solid color attribute
            PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( SelectedPlyLst.GetInt( i ) )->SetSolidColor( Button_GetCheck( GetDlgItem( hwnd, CHK_SOLID ) ) );

            if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_INTENSITY ) ) )
            {
                PolyInten = Edit_GetValue( GetDlgItem( hwnd, EDT_INTENSITY ) );

                // Make sure the intensity is in range
                if( PolyInten < SHADE_COUNT )
                {
                    // Save the polygon intensity               
                    PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( SelectedPlyLst.GetInt( i ) )->SetIntensity( PolyInten );
                }
            }
        }

        // Render the polygon object
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }

}   // BtnClk_ApplyTexture


/************************************************************************
*    FUNCTION NAME:              BtnClk_RotateTexture                                                             
*
*    DESCRIPTION:                Rotate the texture to the selected polygon
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

void C3DAnimator::BtnClk_RotateTexture()
{
    int i;

    if( PlyList.Count() )
    {
        for( i = 0; i < SelectedPlyLst.Count(); ++i )
            PlyList.GetPolyObjClass( ActiveIndex )->RotateTexture( SelectedPlyLst.GetInt( i ) );

        // Set the flag that indicates a file change has been made
        PlyList.SetCDRLive( ActiveIndex, TRUE );

        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
}   // BtnClk_RotateTexture



/************************************************************************
*
*                     View Window Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         OnViewWnd                                                             
*
*    DESCRIPTION:           Subclassed static control
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

LRESULT C3DAnimator::OnViewWnd( HWND hCtrl, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
        HANDLE_MSG( hCtrl, WM_PAINT,       ViewWnd_OnPaint );
        HANDLE_MSG( hCtrl, WM_MOUSEMOVE,   ViewWnd_OnMouseMove );
        HANDLE_MSG( hCtrl, WM_LBUTTONDOWN, ViewWnd_OnLButtonDown );
        HANDLE_MSG( hCtrl, WM_RBUTTONDOWN, ViewWnd_OnRButtonDown );
        HANDLE_MSG( hCtrl, WM_LBUTTONUP,   ViewWnd_OnLButtonUp );
        HANDLE_MSG( hCtrl, WM_RBUTTONUP,   ViewWnd_OnLButtonUp );
        HANDLE_MSG( hCtrl, WM_MOUSEWHEEL,  ViewWnd_OnMouseWheel );
        default:
            return CallWindowProc( OldSubCtrlProc[ STC_VIEWER ], hCtrl, Message, wParam, lParam );
    }

}   // OnViewWnd



/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnMouseMove()                                                             
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

void C3DAnimator::ViewWnd_OnMouseMove( HWND hwnd, int X, int Y, UINT keyFlags )
{
    int MoveAmount;
    TCamera Camera;

    if( MouseButtonDown && PlyList.Count() )
    {
        // get the move amount
        MoveAmount = Edit_GetValue( GetDlgItem( hParentWnd, EDT_XYTRANS_MOVE_AMT ) );

        // Translate and rotate the new positions.
        // Are we moving the camera or the object?
        // The objects rotations are oposite that of the camera
        if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_CAMERA ) ) )
        {
            // Record the movement
            if( MouseMoveX < X )
                if( MouseMoveState == ROTATE )
                    Camera.YRot = X - MouseMoveX;
                else
                    Camera.XPos = (X - MouseMoveX) * MoveAmount;
            else
                if( MouseMoveState == ROTATE )
                    Camera.YRot = -(MouseMoveX - X);
                else
                    Camera.XPos = -((MouseMoveX - X) * MoveAmount);

            if( MouseMoveY < Y )
                if( MouseMoveState == ROTATE )
                    Camera.XRot = Y - MouseMoveY;
                else
                    Camera.YPos = -((Y - MouseMoveY) * MoveAmount);
            else
                if( MouseMoveState == ROTATE )
                    Camera.XRot = -(MouseMoveY - Y);
                else
                    Camera.YPos = (MouseMoveY - Y) * MoveAmount;

            // Inc the camers matrix in case needed
            CameraMatrix.Translate( Camera.XPos, Camera.YPos, Camera.ZPos );
            CameraMatrix.Rotate( Camera.XRot, Camera.YRot, Camera.ZRot );

            // Inc the camera movements
            CameraView.XPos += Camera.XPos;
            CameraView.YPos += Camera.YPos;
            CameraView.XRot += Camera.XRot;
            CameraView.YRot += Camera.YRot;

            // Inc the step camera for saving camer zero point movements
            cameraStep.XPos += Camera.XPos;
            cameraStep.YPos += Camera.YPos;
            cameraStep.XRot += Camera.XRot;
            cameraStep.YRot += Camera.YRot;

            // Show the camera coordinates
            Edit_DisplayValue( hSubCtrl[ X_TRAN_CAM ], CameraView.XPos );
            Edit_DisplayValue( hSubCtrl[ Y_TRAN_CAM ], CameraView.YPos );
            Edit_DisplayValue( hSubCtrl[ X_ROT_CAM ], CameraView.XRot );
            Edit_DisplayValue( hSubCtrl[ Y_ROT_CAM ], CameraView.YRot );
        }
        else
        {
            // Record the movement
            if( MouseMoveX < X )
                if( MouseMoveState == ROTATE )
                    Camera.YRot = -(X - MouseMoveX);
                else
                    Camera.XPos = (X - MouseMoveX) + MoveAmount;
            else
                if( MouseMoveState == ROTATE )
                    Camera.YRot = MouseMoveX - X;
                else
                    Camera.XPos = -((MouseMoveX - X) + MoveAmount);

            if( MouseMoveY < Y )
                if( MouseMoveState == ROTATE )
                    Camera.XRot = -(Y - MouseMoveY);
                else
                    Camera.YPos = -((Y - MouseMoveY) + MoveAmount);
            else
                if( MouseMoveState == ROTATE )
                    Camera.XRot = MouseMoveY - Y;
                else
                    Camera.YPos = (MouseMoveY - Y) + MoveAmount;

            // Inc the camera movements
            PlyList.GetOffset( ActiveIndex )->XPos += Camera.XPos;
            PlyList.GetOffset( ActiveIndex )->YPos += Camera.YPos;
            PlyList.GetOffset( ActiveIndex )->XRot += Camera.XRot;
            PlyList.GetOffset( ActiveIndex )->YRot += Camera.YRot;
            
            // Show the object coordinates
            //Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], PlyList.GetRotatePtSave( ActiveIndex )->XPos );
            //Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], PlyList.GetRotatePtSave( ActiveIndex )->YPos );
            //Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], PlyList.GetRotatePtSave( ActiveIndex )->ZPos );
            Edit_DisplayValue( hSubCtrl[ X_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->XPos );
            Edit_DisplayValue( hSubCtrl[ Y_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->YPos );
            Edit_DisplayValue( hSubCtrl[ X_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->XRot );
            Edit_DisplayValue( hSubCtrl[ Y_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->YRot );          
        }

        // Save the new offsets
        MouseMoveX = X;
        MouseMoveY = Y;

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }
}   // ViewWnd_OnMouseMove



/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnMouseWheel()                                                             
*
*    DESCRIPTION:           Standard Windows OnMouseWheel message handler.
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

int C3DAnimator::ViewWnd_OnMouseWheel( HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys )
{
    int MoveAmount;
    TCamera Camera;

    if( PlyList.Count() )
    {
        // get the move amount
        MoveAmount = Edit_GetValue( GetDlgItem( hParentWnd, EDT_ZTRANS_MOVE_AMT ) );

        // See if they are holding down the Ctrl key
        // See if the high byte has been set
        if( HIBYTE( GetAsyncKeyState( VK_CONTROL ) ) )
        {
            if( zDelta > 0 )
                Camera.ZRot = 5;
            else
                Camera.ZRot = -5;
        }
        else
        {
            if( zDelta > 0 )
                Camera.ZPos = MoveAmount;
            else
                Camera.ZPos = -MoveAmount;
        }

        // Translate and rotate the new positions.
        // Are we moving the camera or the object?
        if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_CAMERA ) ) )
        {
            // Inc the camera movements
            CameraView.ZPos += Camera.ZPos;
            CameraView.ZRot += Camera.ZRot;

            // Inc the step camera for saving camer zero point movements
            cameraStep.ZPos += Camera.ZPos;
            cameraStep.ZRot += Camera.ZRot;

            // Inc the camers matrix in case needed
            CameraMatrix.Translate( Camera.XPos, Camera.YPos, Camera.ZPos );
            CameraMatrix.Rotate( Camera.XRot, Camera.YRot, Camera.ZRot );

            // Show the camera coordinates
            Edit_DisplayValue( hSubCtrl[ Z_TRAN_CAM ], CameraView.ZPos );
            Edit_DisplayValue( hSubCtrl[ Z_ROT_CAM ], CameraView.ZRot );
        }
        else
        {
            PlyList.GetOffset( ActiveIndex )->ZPos += Camera.ZPos;
            PlyList.GetOffset( ActiveIndex )->ZRot += Camera.ZRot;

            // Show the camera coordinates
            Edit_DisplayValue( hSubCtrl[ Z_TRAN_OBJ ], PlyList.GetOffset( ActiveIndex )->ZPos );
            Edit_DisplayValue( hSubCtrl[ Z_ROT_OBJ ], PlyList.GetOffset( ActiveIndex )->ZRot );
        }

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }

    return FALSE;

}   // ViewWnd_OnMouseWheel


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnLButtonDown()                                                             
*
*    DESCRIPTION:           Standard Windows OnLButtonDown message handler.
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

void C3DAnimator::ViewWnd_OnLButtonDown( HWND hwnd, BOOL fDoubleClick, int X, int Y, UINT keyFlags )
{
    MouseButtonDown = TRUE;
    MouseMoveState = TRANSLATE;

    // record the current move positions
    MouseMoveX = X;
    MouseMoveY = Y;

    // Clear out the int list if they click somewhere blank
    // and are not holding down the shift key
    if( !HIBYTE( GetAsyncKeyState( VK_SHIFT ) ) )
        SelectedPlyLst.Free();

}   // ViewWnd_OnLButtonDown


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnLButtonDown()                                                             
*
*    DESCRIPTION:           Standard Windows OnLButtonDown message handler.
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

void C3DAnimator::ViewWnd_OnRButtonDown( HWND hwnd, BOOL fDoubleClick, int X, int Y, UINT keyFlags )
{
    MouseButtonDown = TRUE;
    MouseMoveState = ROTATE;

    // record the current move positions
    MouseMoveX = X;
    MouseMoveY = Y;

    // Clear out the int list if they click somewhere blank
    // and are not holding down the shift key
    if( !HIBYTE( GetAsyncKeyState( VK_SHIFT ) ) )
        SelectedPlyLst.Free();

}   // ViewWnd_OnLButtonDown


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnLButtonUp()                                                             
*
*    DESCRIPTION:           Standard Windows OnLButtonUp message handler.
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

void C3DAnimator::ViewWnd_OnLButtonUp( HWND hwnd, int x, int y, UINT keyFlags )
{
    char StrTmp[50];
    int Selection, i;
    BOOL UseIndex = TRUE;

    MouseButtonDown = FALSE;

    if( PlyList.Count() && Button_GetCheck( GetDlgItem( hParentWnd, CHK_TEXTURE_MODE ) ) )
    {
        Selection = PlyList.GetPolyObjClass( ActiveIndex )->GetSelectedPolygon( x, y );

        if( Selection > -1 )
        {
            // If they are not holding down the shift key, rest the int list?
            if( !HIBYTE( GetAsyncKeyState( VK_SHIFT ) ) )
                SelectedPlyLst.Free();

            // Go through and see if that selecting has already been made. If so delete it from the list
            for( i = 0; i < SelectedPlyLst.Count(); ++i )
            {
                if( SelectedPlyLst.GetInt( i ) == Selection )
                {
                    SelectedPlyLst.Delete( i );
                    UseIndex = FALSE;
                }   
            }

            // Can we use the index?
            if( UseIndex )
            {
                // Add the selected index
                SelectedPlyLst.Add( Selection );

                // Select the listbox texture associated with the polygon
                ListBox_SetCurSel( GetDlgItem( hParentWnd, LST_TEXTURES ),
                                   PlyList.GetPolyObjClass( ActiveIndex )->GetPoly( Selection )->GetTextureHandle() );

                // Load the selected texture so that it can be seen in the list box
                if( PlyList.GetAllowTextCtrls( ActiveIndex ) )
                    LstClk_LoadSelectedTexture( hParentWnd, GetDlgItem( hParentWnd, LST_TEXTURES), CBN_SELCHANGE );
            }

            sprintf( StrTmp, "Polygon Index: %d", SelectedPlyLst.GetInt( SelectedPlyLst.Count()-1 ) );
            Static_SetText( GetDlgItem( hParentWnd, STC_POLYGON_INDEX ), StrTmp );
        }

        // Render the scene
        RenderScene();

        // Show it
        VBuffer.Draw( hViewWndDC );
    }

}   // ViewWnd_OnLButtonUp


/************************************************************************
*    FUNCTION NAME:         ViewWnd_OnPaint()                                                             
*
*    DESCRIPTION:           Standard Windows OnPaint message handler.
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

void C3DAnimator::ViewWnd_OnPaint( HWND hwnd )
{
    PAINTSTRUCT paintStruct;

    // Allow PAINTSTRUCT structure to get inited by the BeginPaint API call.
    // The BeginPaint and EndPaint API calls can only be use in the WM_PAINT
    // message handler. You CAN NOT paint any other way. Even if you have
    // nothing to paint, WM_PAINT message handler has to have the BeginPaint 
    // and EndPaint API calls or bad things will happen,
    BeginPaint( hwnd, &paintStruct );

    // Must be called
    EndPaint( hwnd, &paintStruct );

    // Blit the buffer to the screen
    if( !VBuffer.IsEmpty() && PlyList.Count() )
    {
        // Load the palette if we are in 8 bit or less mode
        // Init the screens palette
        if( SystemBitDepth <= 8 )
        {
            // Saver the old palette to select back into the dc. If this doesn't happen, the newly created
            // palette will never get deleted
            SelectPalette( hViewWndDC, VBuffer.GetPalette(), FALSE );
            RealizePalette( hViewWndDC );
        }

        // Blit the buffer to the screen 
        VBuffer.Draw( hViewWndDC );
    }

}   // ViewWnd_OnPaint


/************************************************************************
*    FUNCTION NAME:         RepaintPreviewWnd                                                             
*
*    DESCRIPTION:           Force a repaint onto the view window
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

void C3DAnimator::RepaintViewWnd()
{
    // Invalidate the whole window so it all gets painted
    InvalidateRect( hSubCtrl[ STC_VIEWER ], &ViewRect, TRUE );

}   // RepaintViewWnd


/************************************************************************
*    FUNCTION NAME:         AddImageResource                                                             
*
*    DESCRIPTION:           Add a new image resource to the project
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

BOOL C3DAnimator::MnuClk_AddTesture( HWND hwnd )
{
    TStringList StringList;
    BOOL Result = FALSE;
    char FileDir[MAX_PATH+1];
    char ProjectPath[MAX_PATH+1];
    char FileName[MAX_PATH*50];
    char StrTemp[MAX_PATH*2];
    char TextLoadCaption[MAX_PATH+1];
    int i, j, TxtLibCount;
    BOOL DuplicateResource;

    // Needs to be terminated for the common dialogs to work
    FileName[0] = 0;

    // Crop out the path to the loaded polygon object    
    CropOutPath( PlyList.GetPolyObjFileName( ActiveIndex ), ProjectPath );
    
    // First pull the file name from the path
    CropFileNameFromPath( PlyList.GetPolyObjFileName( ActiveIndex ), StrTemp );

    // Now remove the file extension
    RelpaceFileExtension( NULL, StrTemp, "" );
    
    // Build a custom menu for the texture load dialog box
    sprintf( TextLoadCaption, "Build Texture Library for %s", StrTemp ); 

    if( PictLoad_GetPicture( hInst, hwnd, TextLoadCaption,
        OFN_ALLOWMULTISELECT, FileName, sizeof(FileName), RES_FILE_TYPES, ProjectPath ) )
    {
        // Crop out the path
        CropOutPath( FileName, FileDir );

        // Check that these directories are the same.
        if( strcmpi( FileDir, ProjectPath ) != 0 )
        {
            PostMsg( "File Error", "All textures have to be in the same directory as the polygon object file." );
            goto LOAD_ERROR;
        }

        // Check for multiple selections
        // The way this works is that if this is a multiple selection, you have string after string
        // seperated with a NULL and the whole thing is terminated with two NULLs. The first string
        // is the path to the file and after that, all the file names until you hit the double NULL.
        // That is why the first string won't have a "." extension because it is just the path.
        i = strlen(FileName);
        if( FileName[i-4] != '.' )
        {
            // Save the pointer to the file path
            // This is for readability
            strcpy( FileDir, FileName );
            if( FileDir[i-1] != BACK_SLASH )
                strcat( FileDir, "\\" );

            // Jump to the next string
            ++i; 
            while( *(FileName + i) )
            {
                // Piece together the file paths
                strcpy( StrTemp, (FileName + i) );

                // Set the duplicate resource flag to false
                DuplicateResource = FALSE;
                
                // Make sure we are not adding a resource that is already part of the project
                for( j = 0; j < PlyList.GetTextStrList( ActiveIndex )->Count(); ++j )
                {
                    if( strcmpi( StrTemp, PlyList.GetTextStrList( ActiveIndex )->GetString( j ) ) == 0 ) // Non-case sensitive string compare
                    {
                        DuplicateResource = TRUE;
                        break;
                    }
                }

                // Add all the paths the the string list class
                // for easy sorting
                if( !DuplicateResource )
                    if( !StringList.Add( StrTemp ) )
                        goto LOAD_ERROR;


                i += strlen( (FileName + i) );
                // Jump to the next string
                ++i;    
            }
        }
        else // Single image load
        {
            // Crop the file name from the path
            CropFileNameFromPath( FileName, StrTemp );

            // Set the duplicate resource flag to false
            DuplicateResource = FALSE;

            // Make sure we are not adding a resource that is already part of the project
            for( j = 0; j < PlyList.GetTextStrList( ActiveIndex )->Count(); ++j )
            {
                if( strcmpi( StrTemp, PlyList.GetTextStrList( ActiveIndex )->GetString( j ) ) == 0 ) // Non-case sensitive string compare
                {
                    DuplicateResource = TRUE;
                    break;
                }
            }

            // Don't add duplicate resources.
            if( !DuplicateResource )
                if( !StringList.Add( StrTemp ) )
                    goto LOAD_ERROR;
        }

        // Sort the list
        StringList.Sort();

        // Check all the textures before adding them to the library
        for( i = 0; i < StringList.Count(); ++i )
        {
            sprintf( StrTemp, "%s\\%s", ProjectPath, StringList.GetString( i ) );
            if( !CheckTexture( StrTemp ) )
                goto LOAD_ERROR;
        }

        // Get the current texture library count before we add any new textures
        TxtLibCount = PlyList.GetTextStrList( ActiveIndex )->Count();

        // Add file name(s) to the texture file list
        for( i = 0; i < StringList.Count(); ++i )
        {
            if( !PlyList.GetTextStrList( ActiveIndex )->Add( StringList.GetString( i ) ) )
                goto LOAD_ERROR;    
        }

        // build the texture library
        if( !BuildTextureLibrary( hwnd, ActiveIndex ) )
            goto LOAD_ERROR;

        // Set the shaded look-up table
        if( SystemBitDepth <= 8 )
            PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to8BitShadedTable() );
        else
        {
            if( PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 )
                PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get8to16BitShadedTable() );
            else
                PlyList.GetPolyObjClass( ActiveIndex )->SetShadeLookUpTable( PlyList.GetShadedTableClass( ActiveIndex )->Get16to16BitShadedTable() );
        }

        // Reset all the panels
        PlyList.GetPolyObjClass( ActiveIndex )->ReInitPanels();

        // If this is the first time adding textures to the library,
        // we want to save the corridinate file.
        if( TxtLibCount == 0 )
        {
            // Set the flag that indicates a file change has been made
            PlyList.SetCDRLive( ActiveIndex, TRUE );

            // We also want to rest all polygons to the first texture
            for( i = 0; i < PlyList.GetPolyObjClass( ActiveIndex )->GetTotalPolyCount(); ++i )
                // Set the selected texture to the selected polygon
                PlyList.GetPolyObjClass( ActiveIndex )->SetTexture( i, 0 );
        }

        // Enable the texture controls for this polygon
        PlyList.SetAllowTextCtrls( ActiveIndex, TRUE );

        // Setup the texture window of we are in texture mode
        if( Button_GetCheck( GetDlgItem( hwnd, CHK_TEXTURE_MODE ) ) )
        {
            // Init the texture data window
            DisplayTextureLstFile();
        }

        if( !InitVideoBuffer() )
            goto LOAD_ERROR;

        // Render the scene
        RenderScene();

        // Force a repait to the view window
        RepaintViewWnd();
    }

    Result = TRUE;
    
LOAD_ERROR:;

    return Result;
}   // AddImageResource


/************************************************************************
*    FUNCTION NAME:         CheckTexture                                                             
*
*    DESCRIPTION:           Check the texture to see that it is valid
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

BOOL C3DAnimator::CheckTexture( char *FileName )
{
    TDIB dib;
    BOOL Result = FALSE;

    // load the bitmap
    if( dib.LoadFromFile( FileName ) )
    {
        // If this is our first texture, just mak sure it is 8 or 16 bit
        if( PlyList.GetTextStrList( ActiveIndex )->Count() == 0 )
        {
            // Make sure the bitmap is an 8, 16 or 24 bit image
            if( dib.GetBitCount() < 8 || dib.GetBitCount() > 24 )
            {
                PostMsg( "Error", "The bitmap file (%s) is not an 8, 16 or 24 bit graphic.", FileName );
                goto HANDLE_ERROR;
            }
        }
        else
        {
            // Make sure the bitmap is the same bit depth as all the other bitmaps in the library
            if( (PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 8 && dib.GetBitCount() != 8) ||
                (PlyList.GetTextClass( ActiveIndex )->GetBitCount() == 16 && (dib.GetBitCount() < 16 || dib.GetBitCount() > 24))  )
            {
                PostMsg( "Error", "The bitmap file (%s) is not an %d bit graphic. All images in a library need to be the same bit depth.", FileName, PlyList.GetTextClass( ActiveIndex )->GetBitCount() );
                goto HANDLE_ERROR;
            }
        }

        // Make sure the bitmap is not compressed
        if( dib.GetCompressionType() != BI_RGB )
        {
            PostMsg( "Error", RLE_ENCODED_BMP, FileName );
            goto HANDLE_ERROR;
        }

        // Make sure the bitmap is not too big
        if( dib.width() * dib.height() > 0x10000 )
        {
            PostMsg( "Error", "Texture file too big (%s). A texture file can not be larger than 64k.", FileName );
            goto HANDLE_ERROR;
        }

        // Make sure we are using the whole 256 color palette.
        // The engine assumes all 256 colors are being used
        if( dib.GetBitCount() == 8 && dib.getDIBcolors() < 256 )
        {
            PostMsg( "Error", "Texture file has a palette that is less then 256 colors (%s, %d colors). All 256 palette entries need to be used.", FileName, dib.getDIBcolors() );
            goto HANDLE_ERROR;
        }

        // Make sure the bitmap width is a supported width
        if( !(dib.width() == 2 || dib.width() == 4 || dib.width() == 8 || dib.width() == 16 ||
            dib.width() == 32 || dib.width() == 64 || dib.width() == 128 || dib.width() == 256) )
        {
            PostMsg( "Error", "The width of the texture is the wrong size (%s). Texture widths need to be 2, 4, 8, 16, 32, 64, 128 or 256 in size", FileName );
            goto HANDLE_ERROR;
        }
    }
    else
        goto HANDLE_ERROR;

    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // CheckResource


/************************************************************************
*    FUNCTION NAME:              ClearVideoBuffer
*
*    DESCRIPTION:                Clear the video buffer
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

void C3DAnimator::ClearVideoBuffer()
{
    // For 8 bit color, we can just use the zero index
    if( SystemBitDepth <= 8 )
        memset( VBuffer.GetPtrToDIBsectionBits(), 0, (VBuffer.width() * VBuffer.height()) * (VBuffer.GetDibSectionBitDepth() / 8) );

    else  // For 16 bit color, we need to do more work if we want a specfic color for the background
    {
        int register i, size;
        DWORD register *vidBuff;
        DWORD register color;
        PSDWordRGB pPSDWordRGB;

        // get the size in DWORD length
        size = (VBuffer.width() * VBuffer.height()) >> 1;

        // get the pointer to the buffer
        vidBuff = (DWORD *)VBuffer.GetPtrToDIBsectionBits();

        // set to pointer to the data
        pPSDWordRGB = PSDWordRGB(&color);

        // Set the 16 bit color data
        // This make a turquois like color
        pPSDWordRGB->R1 = 14;
        pPSDWordRGB->G1 = 18;
        pPSDWordRGB->B1 = 19;
        pPSDWordRGB->R2 = 14;
        pPSDWordRGB->G2 = 18;
        pPSDWordRGB->B2 = 19;

        for( i = 0; i < size; ++i )
            vidBuff[i] = color;
    }

}   // ClearVideoBuffer


/************************************************************************
*    FUNCTION NAME:              SetupLightSourcePoly
*
*    DESCRIPTION:                The light source polygon object
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

void C3DAnimator::SetupLightSourcePoly()
{
    TLoadRes LoadRes;

    // Set the light source which is the center of the box
    LightPoly.SetLightSource( 1.0F, 1.0F, 1.0F );

    // Load the polygon object from resource
    if( !LoadTheResource( hInst, "lightPoly", "POLY", &LoadRes ) )
        return;

    // Load the polygon object 
    if( !LightPoly.LoadPlyFromPointer( (BYTE *)LoadRes.pData ) )
        return;

    // Load the texture library from resource
    if( !LoadTheResource( hInst, "defTexture", "BMPTXT", &LoadRes ) )
        return;

    // Load the texture library 
    if( !LightTxt.LoadFromPointer( (BYTE *)LoadRes.pData ) )
        return;

    // Load the shaded table from resource
    if( SystemBitDepth <= 8 )
    {
        if( !LoadTheResource( hInst, "defShadedTable8", "BMPSHD", &LoadRes ) )
            return;

        // Load the 8 bit shaded table 
        LightPoly.SetShadeLookUpTable( (BYTE *)LoadRes.pData );
    }
    else
    {
        if( !LoadTheResource( hInst, "defShadedTable16", "BMPSHD", &LoadRes ) )
            return;

        // Load the 16 bit shaded table 
        LightPoly.SetShadeLookUpTable( (WORD *)LoadRes.pData );
    }

    // Set the ZBuffer
    LightPoly.SetZBuffer( ZBuffer );

    // Set the size of the rendering area
    LightPoly.SetRenderSize( ViewRect.right, ViewRect.bottom );

    // Set the maxium distance
    LightPoly.SetMaxZDistance( Edit_GetValue( hSubCtrl[ EDT_Z_DIS ] ) );

    // Set the scale
    LightPoly.SetCameraScale( Edit_GetValue( hSubCtrl[ EDT_CAM_SIZE ] ) );

    // Set the video buffer
    LightPoly.SetVideoBuffer( VBuffer.GetPtrToDIBsectionBits() );

    // Set the texture data
    LightPoly.SetTextures( &LightTxt );

    // Setup the default data
    LightPoly.InitDefText();

}   // SetupLightSourcePoly


/************************************************************************
*    FUNCTION NAME:         BtnClk_AddMoveToAnimate                                                             
*
*    DESCRIPTION:           Add the polygon object movement to the list
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

void C3DAnimator::BtnClk_AddMoveToAnimate( HWND hwnd )
{
    int ListBoxCount, i;
    char PromptStr[ MAX_ALIAS_SIZE ], CheckStr[ MAX_ALIAS_SIZE ];

    // Make sure we have a selection to the polygon object
    if( ActiveIndex < 0 )
        return;

    // Get the alias name
    if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_OBJECT ) ) )
    {
        // Make sure this polygon object has some move indexes in it
        if( !PlyList.GetPolyOffList( ActiveIndex )->Count() )
            return;

        ListBox_GetText( hSubCtrl[ LST_POLYOBJS ], ActiveIndex, PromptStr );
    }
    // If the camera is selected
    else if( Button_GetCheck( GetDlgItem( hParentWnd, RDO_MOVE_CAMERA ) ) )
    {
        // Make sure this camera has some move indexes in it
        if( !CameraList.Count() )
            return;

        // Add the word CAMERA to the list
        strcpy( PromptStr, CAMERA_STR );
    }

    // Need to know how many items in the list box
    ListBoxCount = ListBox_GetCount( hSubCtrl[ LST_MOVE ] );

    // Go through all the strings in the anim to run list box to 
    // be sure we are not adding duplicate animations
    for( i = 0; i < ListBoxCount; ++i )
    {
        ListBox_GetText( hSubCtrl[ LST_MOVE ], i, CheckStr );

        if( strcmpi( PromptStr, CheckStr ) == 0 )
            return;
    }

    // Add animation name to create animation list box
    ListBox_AddString( hSubCtrl[ LST_MOVE ], PromptStr );

    // If the list box has no items in it then we just added one
    // so enable all the controls
    if( ListBoxCount == 0 )
    {
        Edit_Enable( GetDlgItem( hwnd, EDT_FPS ), TRUE );
        Button_Enable( GetDlgItem( hwnd, BTN_PLAY_MOVE ), TRUE );
        ListBox_Enable( hSubCtrl[ LST_MOVE ], TRUE );
    }

}   // BtnClk_AddMoveToAnimate


/************************************************************************
*    FUNCTION NAME:         BtnClk_PlayMovements                                                             
*
*    DESCRIPTION:           Play the selected object movements
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

void C3DAnimator::BtnClk_PlayMovements( HWND hwnd )
{
    int AnimToRunCount, AnimLstCount, i, j;
    char AnimLstStr[ MAX_ALIAS_SIZE ], AnimToRunLstStr[ MAX_ALIAS_SIZE ];
    char AnimSpeed[20];

    // Make sure there is a number specifing the frames per second
    if( Edit_GetTextLength( GetDlgItem( hwnd, EDT_FPS ) ) )
    {
        if( !TimerReturnCode )
        {
            // Free all items from the int list
            intList.Free();

            // Need to know how many items in the list box
            AnimToRunCount = ListBox_GetCount( hSubCtrl[ LST_MOVE ] );

            // Make sure we have something to animate
            if( AnimToRunCount )
            {
                // Get the number of polygon objects available
                AnimLstCount = ListBox_GetCount( hSubCtrl[ LST_POLYOBJS ] );

                // Go through and find the indexes of these animations
                for( i = 0; i < AnimToRunCount; ++i )
                {
                    // Get the string from the animations to run list box
                    ListBox_GetText( hSubCtrl[ LST_MOVE ], i, AnimToRunLstStr );

                    for( j = 0; j < AnimLstCount; ++j )
                    {
                        // Get the string from the animations to run list box
                        ListBox_GetText( hSubCtrl[ LST_POLYOBJS ], j, AnimLstStr );

                        // If we have a match, save the animation index to use later
                        if( strcmp( AnimLstStr, AnimToRunLstStr ) == 0 )
                        {
                            // Get the index of the animation to run
                            intList.Add( j );

                            break;
                        }
                        // See if the camera has been added to the list
                        else if( strcmp( CAMERA_STR, AnimToRunLstStr ) == 0 )
                        {
                            // Get the index of the animation to run
                            intList.Add( CAMERA_MOVE_LST_ID );

                            // init the camera frame counter
                            CameraFrameCounter = 0;

                            break;
                        }
                    }
                }
            }

            // Are we doing an accumulitive camera move?
            if( Button_GetCheck( GetDlgItem( hwnd, CHK_MOVE_ACCUMULATIVE ) ) )
                DoIncCameraMove = TRUE;
            else
                DoIncCameraMove = FALSE;

            // Get the frames per second
            Edit_GetText( GetDlgItem( hwnd, EDT_FPS ), AnimSpeed, sizeof(AnimSpeed) );

            // init the averaging variables
            SlowestTime = 0;
            FastestTime = 0xFFFFFFFF;
            TimerCounter = 0;
            TotalRenderTime = 0;
            Rendering = FALSE;

            // Start the timer
            TimerReturnCode = (int)SetTimer( hwnd, ANIM_TIMER_HANDLE, 1000 / atoi( AnimSpeed ), (TIMERPROC)PlayAnimTimer );

            // Did we get a timer?
            if( !TimerReturnCode )
                PostMsg( "Error", "Error getting timer handle to run animations. Windows may be out of timers. Try restarting Windows to see if that helps." );
            else
                Button_SetText( GetDlgItem( hwnd, BTN_PLAY_MOVE ), "Stop" ); 
        }
        else
        {
            // Kill the timer
            KillTimer( hwnd, ANIM_TIMER_HANDLE );

            // Reset the button label
            Button_SetText( GetDlgItem( hwnd, BTN_PLAY_MOVE ), "Play" );

            // Clear the timer return code
            TimerReturnCode = 0;
        }
    }

}   // BtnClk_PlayMovements


/************************************************************************
*    FUNCTION NAME:         TimerPlayMovements                                                             
*
*    DESCRIPTION:           Play the selected movements. This function
*                           gets called from the windows timer.    
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

void C3DAnimator::TimerPlayMovements( HWND hwnd )
{
    int PolyCount=0, i, FrameCounter, IntLstIndex;
    DWORD PaintBufferStartTime, PaintBufferEndTime, time;
    char StrTmp[100];

    // Don't let it animate if we don't have a timer code
    // or if we are currently rendering
    if( TimerReturnCode == 0 || Rendering )
        return;

    // Set the render flag to indicate we are currently
    // in the render process
    Rendering = TRUE;

    // Zero out the ZBuffer
    memset( ZBuffer, 0, sizeof(int)*(ViewRect.right * ViewRect.bottom) );

    // Erase the video buffer
    ClearVideoBuffer();

    // Go through the list and see if we are animatingon the world camera do that first
    for( i = 0; i < intList.Count(); ++i )
    {
        if( intList.GetInt( i ) == CAMERA_MOVE_LST_ID )
        {
            // Are we doing an accumulitive camera move?
            if( DoIncCameraMove )
            {
                if( CameraFrameCounter == 0 )
                    // In case we doing an accumulitive camera move? Then clear out the matrix
                    CameraMatrix.InitilizeMatrix();

                // Do an accululative camera move
                CameraMatrix.Translate( CameraList.GetXPos( CameraFrameCounter ), CameraList.GetYPos( CameraFrameCounter ), 
                                        CameraList.GetZPos( CameraFrameCounter ) );
                CameraMatrix.Rotate( CameraList.GetXRot( CameraFrameCounter ), CameraList.GetYRot( CameraFrameCounter ), 
                                     CameraList.GetZRot( CameraFrameCounter ) );
            }
            else
            {
                // Get the saved animation offset
                CameraView.XPos = CameraList.GetXPos( CameraFrameCounter );
                CameraView.YPos = CameraList.GetYPos( CameraFrameCounter );
                CameraView.ZPos = CameraList.GetZPos( CameraFrameCounter );
                CameraView.XRot = CameraList.GetXRot( CameraFrameCounter );
                CameraView.YRot = CameraList.GetYRot( CameraFrameCounter );
                CameraView.ZRot = CameraList.GetZRot( CameraFrameCounter );
            }
            
            // Inc the counter
            CameraFrameCounter = (CameraFrameCounter + 1) % CameraList.Count();

            // Break out we are done with this part
            break;
        }
    }

    // Now animate the any polygon objects
    for( i = 0; i < intList.Count(); ++i )
    {
        // Get the index to what is to be animated
        IntLstIndex = intList.GetInt( i );

        if( IntLstIndex > CAMERA_MOVE_LST_ID )
        {           
            // Get the polygon object counter value
            FrameCounter = PlyList.GetFrameCounter( IntLstIndex );

            // Transfer the polygon list offset list to the polygon offset
            // The render function only uses the one structure so copy the indexed data into that
            PlyList.GetOffset( IntLstIndex )->XPos = PlyList.GetPolyOffList( IntLstIndex )->GetXPos( FrameCounter );
            PlyList.GetOffset( IntLstIndex )->YPos = PlyList.GetPolyOffList( IntLstIndex )->GetYPos( FrameCounter );
            PlyList.GetOffset( IntLstIndex )->ZPos = PlyList.GetPolyOffList( IntLstIndex )->GetZPos( FrameCounter );
            PlyList.GetOffset( IntLstIndex )->XRot = PlyList.GetPolyOffList( IntLstIndex )->GetXRot( FrameCounter );
            PlyList.GetOffset( IntLstIndex )->YRot = PlyList.GetPolyOffList( IntLstIndex )->GetYRot( FrameCounter );
            PlyList.GetOffset( IntLstIndex )->ZRot = PlyList.GetPolyOffList( IntLstIndex )->GetZRot( FrameCounter );

            // Inc the counter
            FrameCounter = (FrameCounter + 1) % PlyList.GetPolyOffList( IntLstIndex )->Count();

            // Inc the polygons object counter and saver it
            PlyList.SetFrameCounter( IntLstIndex, FrameCounter );
        }
    }

    // Record the time before we paint the buffer
    PaintBufferStartTime = timeGetTime();

    // Render all the polygon objects
    for( i = 0; i < PlyList.Count(); ++i )
        PolyCount += Render( i, PlyList.GetOffset( i ) );

    // Record the time after we paint the buffer
    PaintBufferEndTime = timeGetTime();

    // Get the render time
    time = PaintBufferEndTime - PaintBufferStartTime;

    // Show it
    VBuffer.Draw( hViewWndDC );

    // Find the slowest time
    if( time > SlowestTime )
        SlowestTime = time;

    // Find the fastest time
    if( time < FastestTime )
        FastestTime = time;

    ++TimerCounter;
    TotalRenderTime += time;

    // Display the number of polygons used to render this scene
    sprintf( StrTmp, "Render Count:  %d", PolyCount );
    Static_SetText( GetDlgItem( hParentWnd, STC_POLY_RENDER ), StrTmp );

    // Display the number of miliseconds taken to render this scene
    sprintf( StrTmp, "Render Time: %u,%u,%u,%u", PaintBufferEndTime - PaintBufferStartTime, FastestTime, SlowestTime, TotalRenderTime / TimerCounter );
    Static_SetText( GetDlgItem( hParentWnd, STC_RENDER_TIME ), StrTmp );

    // Set the render flag to indicate we are currently
    // done rendering
    Rendering = FALSE;

}   // TimerPlayMovements