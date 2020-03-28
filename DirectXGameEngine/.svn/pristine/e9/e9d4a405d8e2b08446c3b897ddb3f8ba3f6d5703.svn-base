
/************************************************************************
*    FILE NAME:       collisionsprite3d.h
*
*    DESCRIPTION:     3D collision sprite class
************************************************************************/

#ifndef __collision_sprite_3d_h__
#define __collision_sprite_3d_h__

// Physical component dependency
#include <3d/sprite3d.h>

// Boost lib dependencies
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/point.h>
#include <3d/collisioncheck.h>

// Forward declaration(s)
class CFace;
class CObjectData3D;
class CObjectCollisionData3D;

class CCollisionSprite3D : public CSprite3D
{
public:

    CCollisionSprite3D();
    CCollisionSprite3D( CObjectData3D * pObjData );
    virtual ~CCollisionSprite3D();

    // Init the sprite
    void Init( const std::string & group, const std::string & type );

    // Check for collision and react to it
    CCollisionCheck & ReactToCollision( CPoint & center, float radius,
                                        float floorPadding, float wallPadding );

    // Check for collision and react to it
    CCollisionCheck & ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
                                        float floorPadding, float wallPadding );

    // Do bounding sphere face collision check
    virtual bool IsCollision_BoundingSphere( CPoint & center, float radius );

    // Last check for collision.
    bool IsCollision_Intersection( CPoint & center, float floorPadding,
                                   float wallPadding );

    // Get the object data components
    const CObjectCollisionData3D & GetCollisionData() const;

    // Camera collision functions
    //bool IsCameraCollision_Intersection( CPoint & center, float padding );
    //bool IsCameraCollision_BoundingSphere( CPoint & center, float radius );
    //CCollisionCheck & ReactToCameraCollision( CMatrix & matrix, CPoint & center, float radius,
                                              //float padding );

private:

    // Init the sprite
    void Init();

private:

    // Object's data. 
    // NOTE: This pointer belongs to the CObjectDataList2D singleton
    CObjectData3D * pObjectData;

    // Check for collision and react to it
    CCollisionCheck & Collision( CPoint & center, float radius,
                                 float floorPadding, float wallPadding );

    // Collision check class
    CCollisionCheck colCheck;

    // Translated mesh data
    boost::scoped_array<CFace> spTransFace;

};

#endif  // __collision_sprite_h__
