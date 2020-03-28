
/************************************************************************
*    FILE NAME:       megatexturemanager.cpp
*
*    DESCRIPTION:     Class that combines textures into larger ones
************************************************************************/

// Physical component dependency
#include <managers/megatexturemanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <common/megatexturecomponent.h>
#include <common/megatexture.h>

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CMegaTextureManager::CMegaTextureManager()
{
}   // constructor


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CMegaTextureManager::~CMegaTextureManager()
{
}	// destructer


/************************************************************************
*    desc:  Get a mega texture
*  
*    param: const string & group - texture group
*
*	 ret:	NText::CTextureFor2D * - mega texture to return
************************************************************************/
CMegaTexture * CMegaTextureManager::GetTexture( const std::string & group )
{
    spMegaTextureMapIter = spMegaTextureMap.find( group );

    if( spMegaTextureMapIter == spMegaTextureMap.end() )
        throw NExcept::CCriticalException( "Mega Texture Manager Error!", 
                                           boost::str( boost::format("Mega texture (%s) doesn't exist.\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));

    return spMegaTextureMapIter->second;

}	// GetTexture


/************************************************************************
*    desc:  Render a specific group's mega texture
*  
*    param: string & group - mega texture to render
************************************************************************/
void CMegaTextureManager::Render( const std::string & group )
{
    spMegaTextureMapIter = spMegaTextureMap.find( group );

    if( spMegaTextureMapIter == spMegaTextureMap.end() )
        throw NExcept::CCriticalException( "Mega Texture Manager Error!", 
                                           boost::str( boost::format("Mega texture (%s) doesn't exist.\n\n%s\nLine: %s") % group % __FUNCTION__ % __LINE__ ));

    spMegaTextureMapIter->second->Render();

}	// Render


/************************************************************************
*    desc:  Delete the texture and component data associated with a 
*			specific group
*  
*    param: string & group - group to delete
************************************************************************/
void CMegaTextureManager::DeleteMegaTexture( const std::string & group )
{
    spMegaTextureMapIter = spMegaTextureMap.find( group );

    if( spMegaTextureMapIter != spMegaTextureMap.end() )
        spMegaTextureMap.erase( spMegaTextureMapIter );

}	// DeleteMegaTexture


/************************************************************************
*    desc:  Clear all the Mega Textures
************************************************************************/
void CMegaTextureManager::Clear()
{
    spMegaTextureMap.clear();

}	// Clear


/************************************************************************
*    desc:  Create a mega texture using the group name passed in
*  
*    param: string & group - group of textures to combine
************************************************************************/
void CMegaTextureManager::CreateMegaTexture( const string & group, int wLimit )
{
    spMegaTextureMapIter = spMegaTextureMap.find( group );

    if( spMegaTextureMapIter == spMegaTextureMap.end() )
    {
        CMegaTexture * pTmpMegaTexture = new CMegaTexture();
        spMegaTextureMap.insert( group, pTmpMegaTexture );

        pTmpMegaTexture->CreateMegaTexture( group, wLimit );
    }

}	// CreateMegaTexture
