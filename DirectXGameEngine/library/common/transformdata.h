
/************************************************************************
*    FILE NAME:       transformdata.h
*
*    DESCRIPTION:     Class to hold position, rotation, and scale data,
*					  as well as a bitmask to tell if either is set
************************************************************************/

#ifndef __transform_data_h__
#define __transform_data_h__

// Game lib dependencies
#include <common/point.h>
#include <common/worldpoint.h>
#include <common/bitmask.h>

class CTransformData
{
public:

    enum
    {
        // No parameters
        NONE = 0,

        // Transform parameters
        POSITION = 0x0001,
        ROTATION = 0x0002,
        SCALE	 = 0x0004
    };

    // Constructer
    CTransformData();

    // Set-Get the position
    void SetPos( CWorldPoint & _pos );
    const CWorldPoint & GetPos() const;

    // Set-Get the rotation
    void SetRot( CPoint & _rot );
    const CPoint & GetRot() const;

    // Set-Get the scale
    void SetScale( CPoint & _scale );
    const CPoint & GetScale() const;

    // Get the parameters
    const CBitmask & GetParameters() const;

private:

    // Transformations
    CWorldPoint pos;
    CPoint rot;
    CPoint scale;

    // Which transformations were set
    CBitmask parameters;

};

#endif  // __transform_data_h__


