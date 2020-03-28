/************************************************************************
*    FILE NAME:       characterloadinfo.cpp
*
*    DESCRIPTION:     Class which holds the names of a character and its
*					  position and rotation whenever it becomes active
************************************************************************/

// Physical component dependency
#include "characterloadinfo.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCharacterLoadInfo::CCharacterLoadInfo()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCharacterLoadInfo::~CCharacterLoadInfo()
{
}   // Destructer


/************************************************************************
*    desc:  Adds a name of a specific character       
*
*	 param: string & name - specific character name
************************************************************************/
void CCharacterLoadInfo::SetName( string & _name )
{
    name = _name;

}	// SetName


/************************************************************************
*    desc:  Gets a name to the specific character
*
*    ret:	string & name - specific character name
************************************************************************/
string & CCharacterLoadInfo::GetName()
{
    return name;

}   // GetPos


/************************************************************************
*    desc:  Set the position of the character when activated
*
*    param: Point & position - pos to set
************************************************************************/
void CCharacterLoadInfo::SetPos( CPoint & position )
{
    pos = position;

}   // SetPos

void CCharacterLoadInfo::SetPos( float x, float y, float z )
{
    pos = CPoint(x, y, z);

}	// SetPos


/************************************************************************
*    desc:  Get the position of the character when activated
*
*    ret:	Point & position - pos to set
************************************************************************/
CPoint & CCharacterLoadInfo::GetPos()
{
    return pos;

}   // GetPos


/************************************************************************
*    desc:  Set the pre-transition rotation of the character when activated
*
*    param: Point & rotation - rotation amount
************************************************************************/
void CCharacterLoadInfo::SetPreTransRot( CPoint & rotation )
{
    preTransRot = rotation;

}	// SetPreTransRot

void CCharacterLoadInfo::SetPreTransRot( float x, float y, float z )
{
    preTransRot =  CPoint(x, y, z);

}	// SetPreTransRot


/************************************************************************
*    desc:  Get the pre-transition rotation of the character when activated
*
*    ret:	Point & rotation - rotation amount
************************************************************************/
CPoint & CCharacterLoadInfo::GetPreTransRot()
{
    return preTransRot;

}   // GetPos