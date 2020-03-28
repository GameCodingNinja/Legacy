/************************************************************************
*    FILE NAME:       stagedatalist.cpp
*
*    DESCRIPTION:    Class that holds a map of stage data used for
*					 later loading
************************************************************************/

// Physical component dependency
#include <managers/stagedatalist.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <common/stagedata.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStageDataList::CStageDataList()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStageDataList::~CStageDataList()
{
}   // Destructer


/************************************************************************
*    desc:  load the meshs from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
void CStageDataList::LoadFromXML( const string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "stageList" );

    for( int i = 0; i < mainNode.nChildNode(); ++i )
    {
        XMLNode stageNode = mainNode.getChildNode( "stage", i );

        // Get the stage's name and file path
        string name = stageNode.getAttribute( "name" );
        string file = stageNode.getAttribute( "file" );

        // Make a temporary stage object
        CStageData * pTmpData = new CStageData();

        // Insert the stage data into the map
        spStageMap.insert( name, pTmpData );

        pTmpData->LoadFromXML( file );
    }

}	// LoadFromXML


/************************************************************************
*    desc:  Get the data to a stage
*
*    param: String name - name of the stage
*
*	 ret:	CStageData * - stage data
************************************************************************/
CStageData * CStageDataList::GetData( const string & name )
{
    stageMapIter = spStageMap.find( name );

    if( stageMapIter != spStageMap.end() )
        return stageMapIter->second;

    throw NExcept::CCriticalException("Stage Data List Get Data Error!",
                boost::str( boost::format("Stage data list name can't be found (%s).\n\n%s\nLine: %s") % name.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetData