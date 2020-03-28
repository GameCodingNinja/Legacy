
/************************************************************************
*
*    PRODUCT:         Screen Saver Settings
*
*    FILE NAME:       SSSettings.cpp
*
*    DESCRIPTION:     Saves screen saver settings to the ini file 
*
*    IMPLEMENTATION:  I use ini files for all the programs I write and
*                     stay away from using the Registry. The Registry can
*                     get hosed and I would rather not be one of the
*                     suspects. Using ini files is fast and safe and if
*                     something were to go wrong because the users computer
*                     has low system resources, the worst that can happen
*                     is that the ini files get hit and my program
*                     doesn't work where as the whole compter is hosed
*                     nothing works. 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "Settings.h"        // Header file for this *.cpp file.
#include MAIN_APP_TOOLS      // A misc group of useful functions

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf



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

TSettings::TSettings()
{
    // Get the windows directory
    GetTheWindowsDirectory( WinDir );

    // Set the default zoom index value
    // ShareTheMemories doesn't save the zoom index
    ZoomIndex = ZOOM_100_PERCENT_INDEX;
}


/************************************************************************
*    FUNCTION NAME:         GetPrivateString                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    none 
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

void TSettings::GetPrivateString()
{
    char IniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    // Is the sound on or off
    FWindowState = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window State", SW_SHOWNORMAL );//SW_MAXIMIZE );
    FWindowX = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window X", "0" );
    FWindowY = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window Y", "0" );
    FWindowW = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window W", "0" );
    FWindowH = GetSettingsInt( IniFilePath, PRODUCT_NAME, "Window H", "0" );
	showInstallInfo = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Show Installer Info", "1" );

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
	char StrTemp[20];

    // Get Orderform settings
    lstrcpy( Name, GetSettingsChar( IniFilePath, ORDER_FORM, "Name", "" ) );
    lstrcpy( Address1, GetSettingsChar( IniFilePath, ORDER_FORM, "Address1", "" ) );
    lstrcpy( Address2, GetSettingsChar( IniFilePath, ORDER_FORM, "Address2", "" ) );
    lstrcpy( City, GetSettingsChar( IniFilePath, ORDER_FORM, "City", "" ) );
    StateIndex = GetSettingsInt( IniFilePath, ORDER_FORM, "State Index", "0" );
    lstrcpy( Zip, GetSettingsChar( IniFilePath, ORDER_FORM, "Zip", "" ) );
    lstrcpy( Email, GetSettingsChar( IniFilePath, ORDER_FORM, "Email", "" ) );
    IncludeFloppy = GetSettingsBool( IniFilePath, ORDER_FORM, "Floppy included", "0" );

    // Get custom Colors
    for( int i = 0; i < MAX_CUSTOM_COLORS; ++i )
    {
        sprintf( StrTemp, "%s%d", "Custom Color ", i );
        CustomColors[i] = GetSettingsInt( IniFilePath, PRODUCT_NAME, StrTemp, "16777215" );
    }
    #endif

    // Get the paths to the different projects
    #ifdef EBOOK_TOOL
    lstrcpy( ProjectPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Path", "C:\\Program Files\\Share the Memories\\Sample ePhoto Book" ) );
    lstrcpy( ProjectArtPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Art Path", "C:\\Program Files\\Share the Memories\\Sample ePhoto Book" ) );
    lstrcpy( ProjectSoundPath, GetSettingsChar( IniFilePath, PRODUCT_NAME, "Project Sound Path", "" ) );
    #else
		// Notice we don't get the ZoomIndex. We don't save the ZoomIndex for Share The Memories 
        #ifdef SHARE_THE_MEMORIES
        SizeWindowToFit = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Size Wnd To Fit", "0" );
        PlaySoundPageTurn = GetSettingsBool( IniFilePath, PRODUCT_NAME, "Play Sound Page Turn", "1" );
        #endif
    #endif

}   // GetPrivateString


/************************************************************************
*    FUNCTION NAME:         WriteSecretIniFile                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - Pointer to file name that gets added to
*                               the Windows directory
*              char *Section  - The name of the section in the ini file
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

void TSettings::WriteSecretIniFile( char *FileName, char *Section )
{
    char IniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, FileName );

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
    // Save to the secret Ini file
    SaveIniSetting( IniFilePath, Section, "Reg No", RegNum );
    // Save if they want to see the orderform help window again
    SaveIniSetting( IniFilePath, Section, "Order Form Help", FOrderFrmHelpWnd );
    // Save if they want to see the intro window again
    SaveIniSetting( IniFilePath, Section, "Intro Help", FIntroHelpWnd );
    #endif

    // Write in View program font info
    #if !defined( EBOOK_TOOL ) && !defined( SHARE_THE_MEMORIES )

        SaveIniSetting( IniFilePath, Section, "Font Height", lf.lfHeight );
        SaveIniSetting( IniFilePath, Section, "Font Width", lf.lfWidth );
        SaveIniSetting( IniFilePath, Section, "Font Escapement", lf.lfEscapement );
        SaveIniSetting( IniFilePath, Section, "Font Orientation", lf.lfOrientation );
        SaveIniSetting( IniFilePath, Section, "Font Weight", lf.lfWeight );
        SaveIniSetting( IniFilePath, Section, "Font Italic", lf.lfItalic );
        SaveIniSetting( IniFilePath, Section, "Font Underline", lf.lfUnderline );
        SaveIniSetting( IniFilePath, Section, "Font StrikeOut", lf.lfStrikeOut );
        SaveIniSetting( IniFilePath, Section, "Font CharSet", lf.lfCharSet );
        SaveIniSetting( IniFilePath, Section, "Font OutPrecision", lf.lfOutPrecision );
        SaveIniSetting( IniFilePath, Section, "Font ClipPrecision", lf.lfClipPrecision );
        SaveIniSetting( IniFilePath, Section, "Font Quality", lf.lfQuality );
        SaveIniSetting( IniFilePath, Section, "Font PitchAndFamily", lf.lfPitchAndFamily );
        SaveIniSetting( IniFilePath, Section, "Font FaceName", lf.lfFaceName );
        SaveIniSetting( IniFilePath, Section, "Font Color", FontColor );

        // Save last page viewed
        SaveIniSetting( IniFilePath, Section, "Last Page", LastPage );

        // Save background color used
        SaveIniSetting( IniFilePath, Section, "Backgrnd Text Color", BackgrndTextColor );

        // Save the zoom index
        SaveIniSetting( IniFilePath, Section, "Zoom Index", ZoomIndex );

        // Save the PlaySoundPageTurn setting
        SaveIniSetting( IniFilePath, Section, "Play Sound Page Turn", PlaySoundPageTurn );

        // Save the SizeWindowToFit setting
        SaveIniSetting( IniFilePath, Section, "Size Wnd To Fit", SizeWindowToFit );

    #endif

}   // WriteSecretIniFile


/************************************************************************
*    FUNCTION NAME:         GetSecretIniFile                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName - Pointer to file name that gets added to
*                               the Windows directory
*              char *Section  - The name of the section in the ini file 
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

void TSettings::GetSecretIniFile( char *FileName, char *Section )
{
    char IniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, FileName );

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
    // read from the secret ini file
    lstrcpy( RegNum, GetSettingsChar( IniFilePath, Section, "Reg No", FAKE_VERSION_NUMBER ) );
    FOrderFrmHelpWnd = GetSettingsBool( IniFilePath, Section, "Order Form Help", "1" );
    FIntroHelpWnd = GetSettingsBool( IniFilePath, Section, "Intro Help", "1" );
    #endif

    // Read View program font info
    #if !defined( EBOOK_TOOL ) && !defined( SHARE_THE_MEMORIES )

        lf.lfHeight = GetSettingsInt( IniFilePath, Section, "Font Height", "0" ); 
        lf.lfWidth = GetSettingsInt( IniFilePath, Section, "Font Width", "0" ); 
        lf.lfEscapement = GetSettingsInt( IniFilePath, Section, "Font Escapement", "0" ); 
        lf.lfOrientation = GetSettingsInt( IniFilePath, Section, "Font Orientation", "0" ); 
        lf.lfWeight = GetSettingsInt( IniFilePath, Section, "Font Weight", "0" ); 
        lf.lfItalic = GetSettingsInt( IniFilePath, Section, "Font Italic", "0" ); 
        lf.lfUnderline = GetSettingsInt( IniFilePath, Section, "Font Underline", "0" ); 
        lf.lfStrikeOut = GetSettingsInt( IniFilePath, Section, "Font StrikeOut", "0" ); 
        lf.lfCharSet = GetSettingsInt( IniFilePath, Section, "Font CharSet", "0" ); 
        lf.lfOutPrecision = GetSettingsInt( IniFilePath, Section, "Font OutPrecision", "0" ); 
        lf.lfClipPrecision = GetSettingsInt( IniFilePath, Section, "Font ClipPrecision", "0" ); 
        lf.lfQuality = GetSettingsInt( IniFilePath, Section, "Font Quality", "0" ); 
        lf.lfPitchAndFamily = GetSettingsInt( IniFilePath, Section, "Font PitchAndFamily", "0" );
        lstrcpy( lf.lfFaceName, GetSettingsChar( IniFilePath, Section, "Font FaceName", "" ) );
        FontColor = GetSettingsInt( IniFilePath, Section, "Font Color", "0" );

        // Get last page viewed
        LastPage = GetSettingsInt( IniFilePath, Section, "Last Page", "0" );

        // Get the background text color
        char StrTmp[20];
        sprintf( StrTmp, "%u", NO_COLOR_SET );
        BackgrndTextColor = GetSettingsInt( IniFilePath, Section, "Backgrnd Text Color", StrTmp );

        // Get the Zoom Index
        ZoomIndex  = GetSettingsInt( IniFilePath, Section, "Zoom Index", ZOOM_100_PERCENT_INDEX_STR );

        // Get the Size window to fit option
        SizeWindowToFit = GetSettingsBool( IniFilePath, Section, "Size Wnd To Fit", "0" );

        // Get the Play Sound On Page Turn option
        PlaySoundPageTurn = GetSettingsBool( IniFilePath, Section, "Play Sound Page Turn", "1" );

    #endif

}   // GetSecretIniFile


/************************************************************************
*    FUNCTION NAME:         WritePrivateString                                                             
*
*    DESCRIPTION:           This class is a screen saver specific class.
*                           Enter in all the values you need to get for
*                           the screen saver
*
*    FUNCTION PARAMETERS:
*    Input:    none 
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

void TSettings::WritePrivateString()
{
    char IniFilePath[MAX_STR_PATH];

    // Setup the path to the ini file
    strcpy( IniFilePath, WinDir );
    strcat( IniFilePath, INI_FILE_NAME );

    // Save the sound on or off
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window State", FWindowState );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window X", FWindowX );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window Y", FWindowY );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window W", FWindowW );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Window H", FWindowH );
	SaveIniSetting( IniFilePath, PRODUCT_NAME, "Show Installer Info", showInstallInfo );

    #if defined( EBOOK_TOOL ) || !defined( SHARE_THE_MEMORIES )
	char StrTemp[20];

    // Save Orderform settings
    SaveIniSetting( IniFilePath, ORDER_FORM, "Name", Name );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Address1", Address1 );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Address2", Address2 );
    SaveIniSetting( IniFilePath, ORDER_FORM, "City", City );
    SaveIniSetting( IniFilePath, ORDER_FORM, "State Index", StateIndex );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Zip", Zip );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Email", Email );
    SaveIniSetting( IniFilePath, ORDER_FORM, "Floppy included", IncludeFloppy );

    // Save custom Colors
    for( int i = 0; i < MAX_CUSTOM_COLORS; ++i )
    {
        sprintf( StrTemp, "%s%d", "Custom Color ", i );
        SaveIniSetting( IniFilePath, PRODUCT_NAME, StrTemp, CustomColors[i] );
    }
    #endif

    // Save the paths to the different projects
    #ifdef EBOOK_TOOL
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Path", ProjectPath );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Art Path", ProjectArtPath );
    SaveIniSetting( IniFilePath, PRODUCT_NAME, "Project Sound Path", ProjectSoundPath );
    #else
		// Notice we don't get the ZoomIndex. We don't save the ZoomIndex for Share The Memories
        #ifdef SHARE_THE_MEMORIES
        SaveIniSetting( IniFilePath, PRODUCT_NAME, "Size Wnd To Fit", SizeWindowToFit );
        SaveIniSetting( IniFilePath, PRODUCT_NAME, "Play Sound Page Turn", PlaySoundPageTurn );
        #endif
    #endif

}   // WritePrivateString
