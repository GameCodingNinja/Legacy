/************************************************************************
*    FILE NAME:       hotspot.cpp
*
*    DESCRIPTION:     Hot spot class
************************************************************************/

// Physical component dependency
#include <3d/hotspot.h>

// Game lib dependencies
#include <3d/visualsprite3D.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CHotSpot::CHotSpot()
         :radius(0),
          pos(0,0,0),
          active(false)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CHotSpot::~CHotSpot()
{
}   // Destructer


/************************************************************************
*    desc:  Set the radius of the hotspot
*
*    param: float _radius - new radius value
************************************************************************/
void CHotSpot::SetRadius( float _radius )
{
    radius = _radius;

}	// SetRadius


/************************************************************************
*    desc:  Returns the radius of the hotspot
*
*    ret:	float radius - radius value
************************************************************************/
float CHotSpot::GetRadius()
{
    return radius;

}	// GetRadius


/************************************************************************
*    desc:  Set the position of the hotspot
*
*    param: CPoint position - new pos value
************************************************************************/
void CHotSpot::SetPos( CPoint & position )
{
    pos = position;

    spotMarker.SetPos( pos );

}	// SetPos


/************************************************************************
*    desc:  Returns the position of the hotspot
*
*    ret:	CPoint pos - position value
************************************************************************/
CPoint & CHotSpot::GetPos()
{
    return pos;

}	// GetPos


/************************************************************************
*    desc:  Set the pre transition rotation of the hotspot
*
*    param: CPoint rotation - new rot value
************************************************************************/
void CHotSpot::SetRot( CPoint & rotation )
{
    rot = rotation;

    spotMarker.SetRot( rot );

}	// SetPos


/************************************************************************
*    desc:  Returns the position of the hotspot
*
*    ret:	CPoint rot - pre transition rotation value
************************************************************************/
CPoint & CHotSpot::GetRot()
{
    return rot;

}	// GetPos


/************************************************************************
*    desc:  Set the activity of the hotspot
*
*    param: bool _active - should the hotspot do anything when touched
************************************************************************/
void CHotSpot::SetActive( bool _active )
{
    active = _active;

}	// SetActive


/************************************************************************
*    desc:  Returns the activity of the hotspot
*
*    ret:	bool active - should the hotspot do anything when touched
************************************************************************/
bool CHotSpot::GetActive()
{
    return active;

}	// GetActive


/************************************************************************
*    desc:  Temporarily Renders the hotspot for visual reference
*
*	 param:	CMatrix & matrix - view matrix
************************************************************************/
void CHotSpot::RenderHotSpot( CMatrix & matrix )
{
    spotMarker.GetVisualSprite().Render( matrix );

}	// RenderHotSpot


/************************************************************************
*    desc:  Returns the marker sprite group
*
*	 ret:	CSpriteGroup spotMarker
************************************************************************/
CSpriteGroup3D & CHotSpot::GetHotSpot()
{
    return spotMarker;

}	// GetHotSpot

/************************************************************************
*    desc:  Initialize the sprite group
*
*	 param:	string dest - filepath of the destination stage
************************************************************************/
bool CHotSpot::SetSpriteGroup( std::string & visual, std::string & collision )
{
    bool result = true;

    // NOTE: Need to rework this.
    //spotMarker.InitVisual( visual );
    //spotMarker.InitCollision( collision );

    return result;

}	// SetSpriteGroup