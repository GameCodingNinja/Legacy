
/************************************************************************
*    FILE NAME:       hudmanager.cpp
*
*    DESCRIPTION:     Hud manager class
************************************************************************/

// Physical component dependency
#include <managers/hudmanager.h>

// Standard lib dependencies

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <gui/uiprogressbar.h>
#include <gui/uicontrolfactory.h>
#include <gui/uicontrol.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CHudManager::CHudManager()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CHudManager::~CHudManager()
{
}	// destructer


/************************************************************************
*    desc:  Unload the hud
************************************************************************/
void CHudManager::Clear()
{
    spHudCtrlMap.clear();

}	// UnloadHud


/************************************************************************
*    desc:  Load the hud info from file
*  
*    param: std::string filePath - path to file
************************************************************************/
void CHudManager::LoadListTable( const string & filePath )
{
    if( hudLstTable.empty() )
    {
        // this open and parse the XML file:
        XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "hudListTable" );

        // Load the name and file
        for( int i = 0; i < node.nChildNode("hud"); ++i )
        {
            const XMLNode hudNode = node.getChildNode( "hud", i );

            hudLstTable.insert( make_pair(hudNode.getAttribute( "name" ), hudNode.getAttribute( "file" )) );
        }
    }

}	// LoadListTable


/************************************************************************
*    desc:  Load the hud 
*  
*    param: std::string filePath - path to file
************************************************************************/
void CHudManager::LoadHud( const string & hudName )
{
    lstTblIter = hudLstTable.find( hudName );

    if( lstTblIter != hudLstTable.end() )
        LoadHudFromFile( hudName, lstTblIter->second );

}	// LoadHud


/************************************************************************
*    desc:  Load the hud from file
*  
*    param: node - XML node
************************************************************************/
void CHudManager::LoadHudFromFile( const string & hudName, const string & filePath )
{
    Clear();

    // this open and parse the XML file:
    XMLNode objLstNode = XMLNode::openFileHelper( filePath.c_str(), "hudControls" );

    for( int i = 0; i < objLstNode.nChildNode("control"); ++i )
    {
        CUIControl * pCtrl = NUIControlFactory::Create( objLstNode.getChildNode( "control", i ) );

        spHudCtrlMap.insert( pCtrl->GetExecutionAction(), pCtrl );
    }

}	// LoadHudFromFile


/************************************************************************
*    desc:  Get the hud control
*  
*    return: CUIControl
************************************************************************/
CUIControl * CHudManager::GetHudControl( const string & ctrlAction )
{
    // See if this control is in the hud map
    hudMapIter = spHudCtrlMap.find( ctrlAction );

    if( hudMapIter != spHudCtrlMap.end() )
        return hudMapIter->second;

    throw NExcept::CCriticalException("Get Hud Control Error!",
                boost::str( boost::format("Hud Control action can't be found (%s).\n\n%s\nLine: %s") % ctrlAction.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetHudControl


/************************************************************************
*    desc:  Reset the dynamic positions of menus
************************************************************************/
void CHudManager::ResetDynamicOffset()
{
    for( hudMapIter = spHudCtrlMap.begin(); 
         hudMapIter != spHudCtrlMap.end();
         ++hudMapIter )
    {
        hudMapIter->second->SetDynamicPos();
    }

}	// ResetDynamicOffset


/************************************************************************
*    desc:  Transform the hud
************************************************************************/
void CHudManager::Transform()
{
    for( hudMapIter = spHudCtrlMap.begin(); 
         hudMapIter != spHudCtrlMap.end();
         ++hudMapIter )
    {
        if( hudMapIter->second->IsVisible() )
            hudMapIter->second->Transform();
    }

}	// Transform


/************************************************************************
*    desc:  Update the hud
************************************************************************/
void CHudManager::Update()
{
    for( hudMapIter = spHudCtrlMap.begin(); 
         hudMapIter != spHudCtrlMap.end();
         ++hudMapIter )
    {
        if( hudMapIter->second->IsVisible() )
            hudMapIter->second->Update();
    }

}	// Update


/************************************************************************
*    desc:  Render the hud
************************************************************************/
void CHudManager::Render()
{
    // Render hud conrols
    for( hudMapIter = spHudCtrlMap.begin(); 
         hudMapIter != spHudCtrlMap.end();
         ++hudMapIter )
    {
        if( hudMapIter->second->IsVisible() )
            hudMapIter->second->Render();
    }

}	// Render





