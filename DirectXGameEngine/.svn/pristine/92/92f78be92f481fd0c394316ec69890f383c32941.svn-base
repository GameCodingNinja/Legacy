
/************************************************************************
*    FILE NAME:       radian.h
*
*    DESCRIPTION:     3D Radian class
************************************************************************/  

#ifndef __radian_h__
#define __radian_h__

// Physical component dependency
#include <common/point.h>

// Game lib dependencies
#include <common/defs.h>

class CRadian : public CPoint
{
public:

    // constructor
    CRadian()
    {
    }

    // Copy constructor
    CRadian( const CRadian &obj )
    {
        *this = obj;
    }

    // Copy constructor
    CRadian( const CPoint &obj )
    {
        x = obj.x * DEG_TO_RAD;
        y = obj.y * DEG_TO_RAD;
        z = obj.z * DEG_TO_RAD;
    }

    // Copy constructor
    CRadian( float _x, float _y, float _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }

};

#endif  // __radian_h__

