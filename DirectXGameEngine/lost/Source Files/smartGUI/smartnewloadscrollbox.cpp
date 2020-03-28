
/************************************************************************
*    FILE NAME:       smartnewsavescrollbox.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "smartnewloadscrollbox.h"

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <gui/uiscrollbox.h>
#include <utilities/genfunc.h>

// Game dependencies
#include "../game/savefile.h"

// Required namespace(s)
using namespace std;

// disable useless Microsoft deprecation warnings
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartNewLoadScrollBox::CSmartNewLoadScrollBox( CUIControl * _pUIControl )
                      : CSmartGuiControl( _pUIControl )
{
}   // constructor


/************************************************************************
*    desc:  Called when the gui is to be displayed
************************************************************************/
void CSmartNewLoadScrollBox::Display()
{
    const uint SAVE_NAME_FONT_INDEX(0);
    const uint DATE_PLAY_TIME_FONT_INDEX(1);

    // Get the file names
    CSaveFile::Instance().GetSaveFileNames();

    // Cast to the scroll box pointer
    CUIScrollBox * pScrollBoxCtrl = NGenFunc::DynCast<CUIScrollBox *>(pUIControl);

    // Clear out the scroll box to re-add all the controls because they might have changed
    pScrollBoxCtrl->RemoveAllScrollControls();

    // If this is for scroll box in the "new game" menu, add as the first 
    // control the one for creating a new game save file
    if( pScrollBoxCtrl->GetName() == "new_game_scroll_box" )
    {
        XMLNode mainNode = XMLNode::openFileHelper( "data/objects/2d/menus/controls/button_dynamic_create_game.ctrl", "control" );

        // Add a "Create New Save" control to the list box
        pScrollBoxCtrl->AddScrollControlFromNode(mainNode);
    }

    // Display any save file info
    if( CSaveFile::Instance().GetfileSaveCount() > 0 )
    {
        // Get the file name map
        const std::multimap<std::time_t, std::string> & fileMap = CSaveFile::Instance().GetFileNameMap();

        // Open and parse the XML file
        XMLNode mainNode;

        if( pScrollBoxCtrl->GetName() == "load_game_scroll_box" )
            mainNode = XMLNode::openFileHelper( "data/objects/2d/menus/controls/button_dynamic_load_game.ctrl", "control" );
        else
            mainNode = XMLNode::openFileHelper( "data/objects/2d/menus/controls/button_dynamic_overwrite_game.ctrl", "control" );

        // Add the control in reverse order so they display newest to oldest because the key is the file date
        for( std::multimap<std::time_t, std::string>::const_reverse_iterator it = fileMap.rbegin(); 
             it != fileMap.rend();
             ++it )
        {
            // Add a new button to the scroll box which returns a pointer to it
            CUIControl * pBtnCtrl = pScrollBoxCtrl->AddScrollControlFromNode(mainNode);

            // Set the file name to the string id for later access
            pBtnCtrl->SetName( it->second );

            // Find the file name deliminators
            size_t dashPos = it->second.find("_");
            size_t dotPos = it->second.find(".");

            // Get the save no. and the play time duration
            string saveNo = it->second.substr(0, dashPos);
            string playDuration = it->second.substr(dashPos+1, dotPos-dashPos-1);

            // Set the save No.
            pBtnCtrl->DisplayFontString("Save " + saveNo, SAVE_NAME_FONT_INDEX);

            // Format the file date/time to a character array
            char timeChar[50];
            std::strftime( timeChar, sizeof(timeChar), "%H:%M%p  %m/%d/%y", std::localtime(&it->first) );

            // Set to a string and convert to lower case
            std::string timeStr = timeChar;
            std::transform(timeStr.begin(), timeStr.end(), timeStr.begin(), ::tolower);

            // load the play duration via string format
            boost::posix_time::time_duration duration( boost::posix_time::duration_from_string(playDuration) );

            // Strip out the seconds
            string durStr = boost::posix_time::to_simple_string( duration );
            size_t colonPos = durStr.find_last_of(":");
            string trunDurStr = durStr.substr(0, colonPos);

            // Format the time stamp and play time string
            string datePlayTimeStr = boost::str( boost::format("%s|%s Play Time") % timeStr % trunDurStr );

            // Set the time stamp and play time string
            pBtnCtrl->DisplayFontString(datePlayTimeStr, DATE_PLAY_TIME_FONT_INDEX);
        }
    }

}	// Display









