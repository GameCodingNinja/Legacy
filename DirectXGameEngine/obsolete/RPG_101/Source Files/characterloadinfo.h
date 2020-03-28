/************************************************************************
*    FILE NAME:       characterloadinfo.h
*
*    DESCRIPTION:     Class which holds the names of a character and its
*					  position and rotation whenever it becomes active
************************************************************************/

#ifndef __characterloadinfo_h__
#define __characterloadinfo_h__

// Standard lib dependencies
#include <vector>
#include <string>

// Game lib dependencies
#include "3d\\point.h"

class CCharacterLoadInfo
{
public:

    CCharacterLoadInfo();
    ~CCharacterLoadInfo();

    // Adds a name of a specific character
    void SetName( std::string & _name );

    // Gets a name to the specific character
    std::string & GetName();

    // Set the position of the character when activated
    void SetPos( float x, float y, float z );
    void SetPos( CPoint & position );

    // Get the position of the character when activated
    CPoint & GetPos();

    // Set the pre-transition rotation of the character when activated
    void SetPreTransRot( float x, float y, float z );
    void SetPreTransRot( CPoint & rotation );

    // Get the pre-transition rotation of the character when activated
    CPoint & GetPreTransRot();

private:

    // A vector of animated sprite names
    std::string name;

    // Starting position of the character when active
    CPoint pos;

    // Starting pre-transition rotation of the character when active
    CPoint preTransRot;

};
#endif  // __characterloadinfo_h__