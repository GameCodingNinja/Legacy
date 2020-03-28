/************************************************************************
*    FILE NAME:       spritegroup3d.h
*
*    DESCRIPTION:     SpriteGroup class
************************************************************************/

#ifndef __spritegroup_3d_h__
#define __spritegroup_3d_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/matrix.h>

// Forward declaration(s)
class CVisualSprite3D;
class CCollisionSprite3D;
class CObjectVisualData3D;
class CObjectData3D;
class CObjectCollisionData3D;
class CCollisionCheck;
class CPoint;
class CMatrix;

class CSpriteGroup3D : boost::noncopyable
{
public:

    CSpriteGroup3D();
    CSpriteGroup3D( CObjectData3D * pObjData );
    virtual ~CSpriteGroup3D();

    // Init the sprite group
    void Init( const std::string & group, const std::string & type );

    // Is the visual or collision sprite active
    bool IsVisualActive() const;
    bool IsCollisionActive() const;

    // Set the position of the group
    void SetPos( CPoint & position );
    void SetPos( float x, float y, float z );

    // Set the pre-translation rotation of the group
    void SetRot( CPoint & rotation );
    void SetRot( float x, float y, float z );

    // Increment the position of the group
    void IncPos( CPoint & position );
    void IncPos( float x, float y, float z );

    // Increment the pre-translation rotation of the group
    void IncRot( CPoint & rotation );
    void IncRot( float x, float y, float z );

    // Transform the sprites' center points
    void Transform( CMatrix & matrix );
    
    // Set the scale of the group
    void SetScale(float scale);
    void SetScale(CPoint & scale);

    // Check for collision and react to it
    CCollisionCheck & ReactToCollision( CPoint & center, float radius,
                                        float floorPadding, float wallPadding );

    // Check for collision and react to it
    CCollisionCheck & ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
                                        float floorPadding, float wallPadding );

    //void Render();
    //void RenderShadowMap();

    // Get the object data components
    const CObjectVisualData3D & GetVisualData() const;
    const CObjectCollisionData3D & GetCollisionData() const;

    // Get the desired Sprite
    CVisualSprite3D & GetVisualSprite();
    CCollisionSprite3D & GetCollisionSprite();

    // Set the animation
    void SetAnimation( const std::string & name );

    // Inc the animation timer
    void IncAnimationTime();

private:

    // Init the sprite group
    void Init();

private:

    // The visual sprite of the group
    boost::scoped_ptr<CVisualSprite3D> spVisual;

    // The collision sprite of the group
    boost::scoped_ptr<CCollisionSprite3D> spCollision;

    // Object's data. 
    // NOTE: This pointer belongs to the CObjectDataList2D singleton
    CObjectData3D * pObjectData;

};
#endif  // __spritegroup_h__