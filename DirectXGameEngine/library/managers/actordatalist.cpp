
/************************************************************************
*    FILE NAME:       actordatalist.cpp
*
*    DESCRIPTION:     CActorDataList Class
************************************************************************/

// Physical component dependency
#include <managers/actordatalist.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <common/actordata.h>
#include <xmlParser/xmlParser.h>
#include <utilities/exceptionhandling.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CActorDataList::CActorDataList()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CActorDataList::~CActorDataList()
{
}	// destructer


/************************************************************************
*    desc:  Clear out the actor data
************************************************************************/
void CActorDataList::Clear()
{
    spActorDataMap.clear();
}


/************************************************************************
*    desc:  load the meshs from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CActorDataList::LoadFromXML( const std::string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "actorDataList" );

    for( int i = 0; i < mainNode.nChildNode(); ++i )
    {
        XMLNode actorDataNode = mainNode.getChildNode( "actorData", i );

        // Get the reference name of this actor
        std::string name = actorDataNode.getAttribute( "name" );

        // Allocate the actor data
        CActorData * pActorData = new CActorData();

        // Insert the stage data into the ptr_map
        spActorDataMap.insert( name, pActorData );

        // Load the actor from node
        pActorData->LoadFromNode( actorDataNode );
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Get the data to a stage
*
*    param: String name - name of the stage
*
*	 ret:	CStageData * - stage data
************************************************************************/
CActorData * CActorDataList::GetData( const std::string & name )
{
    actorDataMapIter = spActorDataMap.find( name );

    if( actorDataMapIter != spActorDataMap.end() )
        return actorDataMapIter->second;

    throw NExcept::CCriticalException("Stage Data List Get Data Error!",
                boost::str( boost::format("Stage data list name can't be found (%s).\n\n%s\nLine: %s") % name.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetData

