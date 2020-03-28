/************************************************************************
*    FILE NAME:       charactergroup.cpp
*
*    DESCRIPTION:     Class which holds a group of CCharacterLoadInfo
*					  objects.
************************************************************************/

// Physical component dependency
#include "charactergroup.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCharacterGroup::CCharacterGroup()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCharacterGroup::~CCharacterGroup()
{
}   // Destructer


/************************************************************************
*    desc:  Set function for the character group  
*
*	 param: vector<CCharacterLoadInfo> & grp - new group
************************************************************************/
void CCharacterGroup::SetGroup( vector<CCharacterLoadInfo *> & grp )
{
    group = grp;

}	// SetGroup


/************************************************************************
*    desc:  Get function for the character group  
*
*	 ret:   vector<CCharacterLoadInfo> & group - current group
************************************************************************/
vector<CCharacterLoadInfo *> & CCharacterGroup::GetGroup()
{
    return group;

}	// GetGroup


/************************************************************************
*    desc:  Adds a single character to the group
*
*	 param: CCharacterLoadInfo & character - added inactive character
************************************************************************/
void CCharacterGroup::AddCharacter( CCharacterLoadInfo * pCharacter )
{
    group.push_back( pCharacter );

}	// AddCharacter