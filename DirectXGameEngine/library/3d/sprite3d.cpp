/************************************************************************
*    FILE NAME:       sprite3d.cpp
*
*    DESCRIPTION:     3D sprite class
************************************************************************/

// Physical component dependency
#include <3d/sprite3d.h>

// Game lib dependencies
#include <system/xdevice.h>
#include <misc/settings.h>
#include <3d/mesh3d.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSprite3D::CSprite3D()
         : pMesh(NULL)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CSprite3D::~CSprite3D()
{
}   // Destructer


/************************************************************************
*    desc:  Get the radius
*  
*    return: float - sprite radius
************************************************************************/
float CSprite3D::GetRadius()
{
    CPoint tmpScale = GetScale();
    float tmpScaleValue = tmpScale.x;

    if( tmpScaleValue < tmpScale.y )
        tmpScaleValue = tmpScale.y;

    else if( tmpScaleValue < tmpScale.z )
        tmpScaleValue = tmpScale.z;

    return pMesh->GetRadius() * tmpScaleValue * tmpScaleValue;

}   // GetRadius


/************************************************************************
*    desc:  Get the squar root radius
*  
*    return: float - sprite radius
************************************************************************/
float CSprite3D::GetRadiusSqrt()
{
    CPoint tmpScale = GetScale();
    float tmpScaleValue = tmpScale.x;

    if( tmpScaleValue < tmpScale.y )
        tmpScaleValue = tmpScale.y;

    else if( tmpScaleValue < tmpScale.z )
        tmpScaleValue = tmpScale.z;

    return pMesh->GetRadiusSqrt() * tmpScaleValue;

}   // GetRadiusSqrt


/************************************************************************
*    desc:  Is this sprite active
*  
*    return: bool - sprite scale
************************************************************************/
bool CSprite3D::IsActive()
{
    return (pMesh != NULL);

}   // IsActive


/************************************************************************
*    desc:  Test position of object in frustum to see if it can
*           be culled. This assumes a 45 degree view area.
*
*    param: CPoint & centerPt - translated center point
*    param: float radius - radius of mesh
*
*    ret:   bool - True indicates object is outside frustum
************************************************************************/
int CSprite3D::CullMesh_BoundSphere( CPoint & centerPt, float radius )
{
    //////////////////////////////////////////
    // Test against the far & near plains
    //////////////////////////////////////////

    float offset_width = radius / sin( M_PI * CSettings::Instance().GetViewAngle() / 180.f );
    float offset_height = radius / sin( ( M_PI * CSettings::Instance().GetViewAngle() * CXDevice::Instance().GetFrustrumYRatio() ) / 180.f );

    // First check to see if the object is even within view
    if( centerPt.z - radius >= CXDevice::Instance().GetMaxZDist() )
    {
        return 1;
    }

    if( !IsCollision_BoundingSphere( CPoint(0.f,0.f,0.f), 100.f ) )
    {
        float farPoint = centerPt.z * CXDevice::Instance().GetSquarePercentage();

        // Check the right and left sides of the screen
        // This check will also cull the object if it is behind us
        if(farPoint <= fabs(centerPt.x) - offset_width)
        {
            return 2;
        }
    
        farPoint = centerPt.z * CXDevice::Instance().GetFrustrumYRatio();

        // Check the top and bottom sides of the screen
        if(farPoint <= fabs(centerPt.y) - offset_height)
        {
            return 3;
        }
    }
        
    // if we made it this far, the object was not culled
    return 0;

}   // CullMesh_BoundSphere


/************************************************************************
*    desc:  Do bounding sphere collision check on object center point
*           and radius. This is a squared check so no sqrt is used for speed.
*
*    param: CPoint center - Center point of the object
*           float radius - squared radius of the object
************************************************************************/
bool CSprite3D::IsCollision_BoundingSphere( CPoint & center, float radius )
{
    bool result = false;

    // Get the length from the object center to the passed in center
    //float distance = trans_center.GetLengthSquared( center );
    float distance = GetTransPos().GetLengthSquared( center );

    // If the distance is less then the two radiuses, we might be colliding
    if( distance <= GetRadius() + radius )
    {
        result = true;
    }

    return result;

}	// IsCollision_BoundingSphereMesh


