/************************************************************************
*    FILE NAME:       hotspot.h
*
*    DESCRIPTION:     Hot spot class
************************************************************************/

#ifndef __hotspot_h__
#define __hotspot_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/point.h>
#include <common/matrix.h>
#include <3d/spritegroup3d.h>

class CHotSpot
{
public:

    CHotSpot();
    ~CHotSpot();

    void SetRadius( float _radius );
    float GetRadius();

    void SetPos( CPoint & position );
    CPoint & GetPos();

    void SetRot( CPoint & position );
    CPoint & GetRot();

    void SetActive( bool _active );
    bool GetActive();

    void RenderHotSpot( CMatrix & matrix );

    bool SetSpriteGroup( std::string & visual, std::string & collision );
    CSpriteGroup3D & GetHotSpot();

protected:

    CSpriteGroup3D spotMarker;

    float radius;
    CPoint pos;
    CPoint rot;
    bool active;

};
#endif  // __hotspot_h__
