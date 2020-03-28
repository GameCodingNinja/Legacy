/************************************************************************
*    FILE NAME:       stagemanager.cpp
*
*    DESCRIPTION:     Class that manages stage objects
************************************************************************/

// Physical component dependency
#include "stagemanager.h"

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xwindow.h"
#include "misc\\genfunc.h"
#include "misc\\deletefuncs.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************
CStageManager::CStageManager()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************
CStageManager::~CStageManager()
{
}   // Destructer


/************************************************************************
*    desc:  Load the stage object list
*
*    param: String & filePath - path to xml file
************************************************************************
void CStageManager::LoadStage( string & filePath )
{
    stage.LoadFromXML( filePath );

}	// LoadObjectList */


