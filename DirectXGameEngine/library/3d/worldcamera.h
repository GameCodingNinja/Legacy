/************************************************************************
*    FILE NAME:       worldcamera.h
*
*    DESCRIPTION:     Singleton class that holds the world camera
*					  position and rotation
************************************************************************/

#ifndef __world_camera_h__
#define __world_camera_h__

// Game lib dependencies
#include <common/worldpoint.h>
#include <common/point.h>
#include <common/matrix.h>

class CWorldCamera
{
public:

    // Get the instance of the singleton class
    static CWorldCamera & Instance()
    {
        static CWorldCamera camera;
        return camera;
    }

    // Set-Get the camera's world position
    void SetPos( const CWorldPoint & pos );
    const CWorldPoint & GetPos() const;

    // Increment the camera's world position
    void IncPos( const CPoint & pos );
    void IncPos( const CPointInt & pos );
    void IncPos( const CWorldPoint & pos );

    // Set-Get the camera's rotation
    void SetRotation( const CPoint & rot );
    const CPoint & GetRotation() const;
    const CMatrix & GetRotationMatrix() const;

    // Increment the camera's rotation
    void IncRotation( const CPoint & rot );

    // Transform the passed in point
    void Transform( CPoint & dest, const CWorldPoint & source ) const;

protected:

    CWorldCamera();
    ~CWorldCamera();

    // The world position of the camera
    CWorldPoint worldPos;

    // The rotation of the camera
    CPoint rotation;

    // Camera rotation matrix matrix. Only the rotation portion
    // of the camera's transformations are stored here
    CMatrix rotMatrix;

};

#endif  // __world_camera_h__