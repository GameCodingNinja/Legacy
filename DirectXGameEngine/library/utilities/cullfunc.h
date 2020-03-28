/************************************************************************
*    FILE NAME:       cullfunc.h
*
*    DESCRIPTION:     Cull checking functions
************************************************************************/  

#ifndef __cullfunc_h__
#define __cullfunc_h__

// Game lib dependencies
#include <common/point.h>
#include <common/size.h>
#include <common/defs.h>
#include <misc/settings.h>
#include <system/xdevice.h>

/************************************************************************
*    desc:  Check if an object is within the perspective view frustum              
*
*	 param: CPoint & center	- center of the object
*			float radius    - radius of the object
*
*	 ret:	bool - whether or not the object is in the view frustum
************************************************************************/
inline bool InPerspectiveView( CPoint & center, float radius )
{
    float viewAngleRad = M_PI * CSettings::Instance().GetViewAngle() / 180.f;
    float farWidth = tan( viewAngleRad ) * center.z;
    float farHeight = tan( viewAngleRad * CXDevice::Instance().GetSquarePercentage() ) * center.z;

    // First check to see if the object is even within view
    if( center.z - radius > CXDevice::Instance().GetMaxZDist() )
        return false;

    // Check against the minimum z distance
    if( center.z + radius < CSettings::Instance().GetMinZdist() )
        return false;

    // Check the right and left sides of the screen
    if(abs(center.x) - radius > farWidth)
        return false;

    // Check the top and bottom sides of the screen
    if(abs(center.y) - radius > farHeight)
        return false;
        
    // if we made it this far, the object was not culled
    return true;

}	// InPerspectiveView


/************************************************************************
*    desc:  Check if an object is within the orthographic view frustum              
*
*	 param: CPoint & center	- center of the object
*			float radius    - radius of the object
*
*	 ret:	bool - whether or not the object is in the view frustum
************************************************************************/
inline bool InOrthographicView( CPoint & center, float radius )
{
    CSize<int> tmpSize;
    tmpSize.h = int( CSettings::Instance().GetDefaultSize().h ) >> 1;
    tmpSize.w = int( CSettings::Instance().GetDefaultSize().w ) >> 1;

    // Check against the right side of the screen
    if( fabs(center.x) - radius > tmpSize.w )
        return false;

    // Check against the top of the screen
    if( fabs(center.y) - radius > tmpSize.h )
        return false;

    // Check against the maximum z distance
    if( center.z - radius > CSettings::Instance().GetMaxZdist() )
        return false;

    // Check against the minimum z distance
    if( center.z + radius < CSettings::Instance().GetMinZdist() )
        return false;

    // If we made it this far, the object is in view
    return true;

}	// InOrthographicView


#endif  // __cullfunc_h__

