
/************************************************************************
*    FILE NAME:       physicsworldmanager.cpp
*
*    DESCRIPTION:     Box2D b2World wrapper class
************************************************************************/

// Physical component dependency
#include <managers/physicsworldmanager.h>

// Standard lib dependencies

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <xmlParser/xmlParser.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer
************************************************************************/
CPhysicsWorldManager::CPhysicsWorldManager()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CPhysicsWorldManager::~CPhysicsWorldManager()
{
}	// destructer


/************************************************************************
*    desc:  Load the physics worlds from an XML
*
*	 param:	const string & filePath - file to load
************************************************************************/
void CPhysicsWorldManager::LoadFromXML( const string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "physicsWorldList" );

    for( int i = 0; i < mainNode.nChildNode(); ++i )
    {
        XMLNode worldNode = mainNode.getChildNode( "world", i );

        // Get the world's name
        string name = worldNode.getAttribute( "name" );

        // Create the world and add it to the map
        CPhysicsWorld * pTmpWorld = new CPhysicsWorld();
        spWorldMap.insert( name, new CPhysicsWorld() );

        // Have the physics world load the rest of its data
        pTmpWorld->LoadFromNode( worldNode );
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Get the physics world
*
*	 param: const string & name - name of the world
*
*	 ret:	CPhysicsWorld * - physics world
************************************************************************/
CPhysicsWorld * CPhysicsWorldManager::GetWorld( const string & name )
{
    // Find the world
    spWorldMapIter = spWorldMap.find( name );

    // If we found it, return it. If not, we have a problem
    if( spWorldMapIter != spWorldMap.end() )
        return spWorldMapIter->second;

    else
        throw NExcept::CCriticalException("Physics World Manager Error!",
                boost::str( boost::format("Physics world doesn't exist (%s).\n\n%s\nLine: %s") 
                            % name.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetWorld


/************************************************************************
*    desc:  Delete a world
*
*	 param: const string & name - name of the world
************************************************************************/
void CPhysicsWorldManager::DeleteWorld( const string & name )
{
    // Find the world
    spWorldMapIter = spWorldMap.find( name );

    // If we find it, delete it. If not, we have a problem
    if( spWorldMapIter != spWorldMap.end() )
        spWorldMap.erase( spWorldMapIter );
    
    else
        throw NExcept::CCriticalException("Physics World Manager Error!",
                boost::str( boost::format("Physics world doesn't exist (%s).\n\n%s\nLine: %s") 
                            % name.c_str() % __FUNCTION__ % __LINE__ ));

}	// DeleteWorld


/************************************************************************
*    desc:  Clear the entire physics world manager
************************************************************************/
void CPhysicsWorldManager::Clear()
{
    spWorldMapIter = spWorldMap.begin();

    while( spWorldMapIter != spWorldMap.end() )
        spWorldMapIter = spWorldMap.erase( spWorldMapIter );

}	// Clear


