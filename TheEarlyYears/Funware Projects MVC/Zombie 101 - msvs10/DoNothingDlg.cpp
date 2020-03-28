/************************************************************************
*
*    PRODUCT:         Template Dialog box
*
*    FILE NAME:       DoNothingDlg.cpp
*
*    DESCRIPTION:     Template Dialog
*
*    IMPLEMENTATION:  Template Dialog
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

#include "DoNothingDlg.h"    // Header for this *.cpp file
#include "3D/types.h"        // Misc type specifiers

// Globals
PCDoNothingDlg pDoNothingDlg;


/************************************************************************
*
*                       Non Object Related Functions
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:        Display_DoNothingDlg                                                             
*
*    DESCRIPTION:           Create dialog box
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst - This progam instance
*              HWND hwnd - Handle to the parent window
*
*    Output:   returntype - none
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    HCR           9/28/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL Display_DoNothingDlg( HINSTANCE hInst, HWND hwnd )
{
    // Create the object
	CDoNothingDlg doNothingDlg;
    pDoNothingDlg = &doNothingDlg;

    // because we create a dialog box, we don't return until the window is closed
    return doNothingDlg.CreateDialogWnd( hInst, hwnd );

}   // Display_DoNothingDlg


/************************************************************************
*    FUNCTION NAME:         DoNothingDlg_Proc
*
*    DESCRIPTION:           Standard Windows dialog message handler.
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDlg      - Handle of the window receiving the message
*              UINT uMsg      - Message being sent
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

LRESULT CALLBACK DoNothingDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        HANDLE_MSG( hDlg, WM_INITDIALOG, pDoNothingDlg->OnInitDialog );
        HANDLE_MSG( hDlg, WM_COMMAND,    pDoNothingDlg->OnCommand );
        HANDLE_MSG( hDlg, WM_DESTROY,    pDoNothingDlg->OnDestroy );
        default:
            return FALSE;
    }
}   // DoNothingDlg_Proc


/************************************************************************
*
*                        Dialog Object Functions
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

CDoNothingDlg::CDoNothingDlg()
{
    // Init class members

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

CDoNothingDlg::~CDoNothingDlg()
{
}   // Destructer



/************************************************************************
*    FUNCTION NAME:         CreateDialogWnd()                                                             
*
*    DESCRIPTION:           Create the dialog Window
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

BOOL CDoNothingDlg::CreateDialogWnd( HINSTANCE hInstance, HWND hwnd )
{
    // Init class variables
    hInst = hInstance;
    hParentWnd = hwnd;

    return DialogBox( hInst, MAKEINTRESOURCE( DLG_DO_NOTHING ), hParentWnd, (DLGPROC)DoNothingDlg_Proc );

}   // CreateDialogWnd


/************************************************************************
*    FUNCTION NAME:         OnInitDialog()                                                             
*
*    DESCRIPTION:           Standard Windows OnInitDialog message handler.
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

BOOL CDoNothingDlg::OnInitDialog( HWND hwnd, HWND hwndFocus, LPARAM lParam )
{	
    // read in the ini file
    settings.GetPrivateString( INI_FILE_NAME, "config" );
	
	switch( settings.GetRenderDevice() )
	{
		case ERD_SOFTWARE:
		{
			Button_SetCheck( GetDlgItem( hwnd, ID_RDO_SOFTWARE ), true );
		}
		break;

		case ERD_DIRECTX:
		{
			Button_SetCheck( GetDlgItem( hwnd, ID_RDO_DIRECTX ), true );
		}
		break;

		case ERD_OPENGL:
		{
			Button_SetCheck( GetDlgItem( hwnd, ID_RDO_OPENGL ), true );
		}
		break;
	};

	if( settings.GetScreenWidth() == 640 && settings.GetScreenHeight() == 480 )
	{
		Button_SetCheck( GetDlgItem( hwnd, ID_RDO_640_480 ), true );
	}
	else if( settings.GetScreenWidth() == 800 && settings.GetScreenHeight() == 600 )
	{
		Button_SetCheck( GetDlgItem( hwnd, ID_RDO_800_600 ), true );
	}
	else if( settings.GetScreenWidth() == 1024 && settings.GetScreenHeight() == 768 )
	{
		Button_SetCheck( GetDlgItem( hwnd, ID_RDO_1024_768 ), true );
	}
	else if( settings.GetScreenWidth() == 1280 && settings.GetScreenHeight() == 1024 )
	{
		Button_SetCheck( GetDlgItem( hwnd, ID_RDO_1280_1024 ), true );
	}

	if( settings.GetDisplayMode() == EDM_FULL_SCREEN )
	{
		Button_SetCheck( GetDlgItem( hwnd, ID_CHK_FULL_SCREEN ), true );
	}
	
	Button_SetCheck( GetDlgItem( hwnd, CHK_INVERT_MOUSE ), settings.GetInvertMouse() );
	Button_SetCheck( GetDlgItem( hwnd, CHK_NO_TEXTURES ), settings.GetDisableTextures() );

    return true;

}   // OnInitDialog


/************************************************************************
*    FUNCTION NAME:         OnCommand()                                                             
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CDoNothingDlg::OnCommand( HWND hwnd, int id, HWND hwndCtl, UINT codeNotify )
{
    CSettings settings;

    switch( id )
    {
        case BTN_CANCEL:

			if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_DIRECTX ) ) )
			{
				settings.SetRenderDevice( ERD_DIRECTX );
			}
			else if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_SOFTWARE ) ) )
			{
				settings.SetRenderDevice( ERD_SOFTWARE );
			}
			else if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_OPENGL ) ) )
			{
				settings.SetRenderDevice( ERD_OPENGL );
			}

            if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_640_480 ) ) )
			{
				settings.SetScreenWidth( 640 );
				settings.SetScreenHeight( 480 );
			}
			else if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_800_600 ) ) )
			{
				settings.SetScreenWidth( 800 );
				settings.SetScreenHeight( 600 );
			}
			else if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_1024_768 ) ) )
			{
				settings.SetScreenWidth( 1024 );
				settings.SetScreenHeight( 768 );
			}
			else if( Button_GetCheck( GetDlgItem( hwnd, ID_RDO_1280_1024 ) ) )
			{
				settings.SetScreenWidth( 1280 );
				settings.SetScreenHeight( 1024 );
			}

            if( Button_GetCheck( GetDlgItem( hwnd, ID_CHK_FULL_SCREEN ) ) )
			{
				settings.SetDisplayMode( EDM_FULL_SCREEN );
			}
			else
			{
				settings.SetDisplayMode( EDM_WINDOWED );
			}

			if( Button_GetCheck( GetDlgItem( hwnd, CHK_INVERT_MOUSE ) ) )
			{
				settings.SetInvertMouse( true );
			}
			else
			{
				settings.SetInvertMouse( false );
			}

			if( Button_GetCheck( GetDlgItem( hwnd, CHK_NO_TEXTURES ) ) )
			{
				settings.SetDisableTextures( true );
			}
			else
			{
				settings.SetDisableTextures( false );
			}
			
			settings.WritePrivateString( INI_FILE_NAME, "config" );

            // The TRUE is the return parameter. This can be an int.
            EndDialog( hwnd, TRUE );        
        break;
    }

}   // OnCommand


/************************************************************************
*    FUNCTION NAME:         OnDestroy                                                             
*
*    DESCRIPTION:           Standard Windows OnDestroy message handler.
*                           You could free memory and do other clean up.
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

void CDoNothingDlg::OnDestroy( HWND hwnd ) 
{

} // OnDestroy