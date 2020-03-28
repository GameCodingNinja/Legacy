
/************************************************************************
*    FILE NAME:       matuvlist.cpp
*
*    DESCRIPTION:     Class that holds the UVs to a material
************************************************************************/

// Physical component dependency
#include "matuvlist.h"

// Standard lib dependencies
#include <assert.h>

// Game lib dependencies
#include "utilities\\genfunc.h"

// Boost lib dependencies

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
CMatUVList::CMatUVList()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CMatUVList::~CMatUVList()
{
}	// destructer


/************************************************************************
*    desc:  Load the UV list from an XML node
*
*	 param:	XMLNode & node - node used to load in the UV list
*
*	 ret:	bool - whether or not the node was successfully loaded
************************************************************************/
bool CMatUVList::LoadFromNode( XMLNode & node )
{
    // Get the number of rows and columns as well as the total number of materials
    XMLNode divNode = node.getChildNode("divisions");
    int total = atoi( divNode.getAttribute("total") );
    int rows = atoi( divNode.getAttribute("rows") );
    int columns = atoi( divNode.getAttribute("columns") );

    // Calculate the increment size
    incrementSize.w = 1.f / float(columns);
    incrementSize.h = 1.f / float(rows);

    // If any of these values are less than 1, something got messed up
    if( total < 1 || rows < 1 || columns < 1 || incrementSize.w <= 0 || incrementSize.h <= 0 )
        return false;

    uvVector.reserve( total );

    // Counter to make sure we don't go over the total number of materials
    int matCounter = 0;

    CUV tmpUV;

    // Calculate the UVs for each material in the file
    for( int i = 0; i < rows; i++ )
    {
        tmpUV.v = float(i) / float(rows);

        for( int j = 0; j < columns; j++ )
        {
            if( matCounter >= total )
                break;

            tmpUV.u = float(j) / float(columns);
            uvVector.push_back( tmpUV );

            matCounter++;
        }
    }

    return true;

}   // LoadFromXML */


/************************************************************************
*    desc:  Get the increment size of the material
*
*	 ret:	CSize<int> & - size of the increments
************************************************************************/
CSize<float> & CMatUVList::GetIncSize()
{
    return incrementSize;

}	// GetIncSize */


/************************************************************************
*    desc:  Get a pair of UV in the UV vector
*
*	 param:	unsigned int index - index into the UV vector
*
*	 ret:	CUV & - desired UV
************************************************************************/
CUV & CMatUVList::GetUV( unsigned int index )
{
    // At no point should we be asking for a UV not in the vector
    assert( index < uvVector.size() );

    return uvVector[index];

}	// GetUV */