
/************************************************************************
*    FILE NAME:       normal.h
*
*    DESCRIPTION:     3D Normal class
************************************************************************/  

#ifndef __normal_h__
#define __normal_h__

// Physical component dependency
#include <common/point.h>

class CNormal : public CPoint
{
public:

    // constructor
    CNormal()
    {
    }

    // Copy constructor
    CNormal( const CNormal &obj )
    {
        *this = obj;
    }

    // Copy constructor
    CNormal( float _x, float _y, float _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }

    // = operator
    void operator = ( const CPoint & p )
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }

};

#endif  // __normal_h__

