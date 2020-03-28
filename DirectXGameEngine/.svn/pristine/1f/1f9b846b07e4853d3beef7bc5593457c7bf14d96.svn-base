/************************************************************************
*    FILE NAME:       animatedspritegroup3d.h
*
*    DESCRIPTION:     SpriteGroup class
************************************************************************/

#ifndef __animatedspritegroup_3d_h__
#define __animatedspritegroup_3d_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <3d/collisionsprite3d.h>
#include <3d/jointanimationsprite3D.h>
#include <3d/group.h>

class CAnimatedSpriteGroup3D : public CGroup
{
public:

	CAnimatedSpriteGroup3D();
	virtual ~CAnimatedSpriteGroup3D();

	// Init the sprite
	void Init( std::string & group, std::string & type );

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
	void SetDefaultScale(float scale);
	void SetDefaultScale(CPoint & scale);

	// Set the animation
	void SetAnimation( const std::string & name );

	// Inc the animation timer
	void IncAnimationTime();

	// Check for collision and react to it
	virtual CCollisionCheck & ReactToCollision( CPoint & center, float radius,
												float floorPadding, float wallPadding );

	// Check for collision and react to it
	virtual CCollisionCheck & ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
												float floorPadding, float wallPadding );

	// Get each individual sprite
	CJointAnimSprite3D & GetVisualSprite();
	CCollisionSprite3D & GetCollisionSprite();

	// Get the object data components
	const CObjectVisualData3D & GetVisualData() const;
	const CObjectCollisionData3D & GetCollisionData() const;

	//void Render();
	//void RenderShadowMap();

private:

	// The visual sprite of the group
	boost::scoped_ptr<CJointAnimSprite3D> spVisual;

	// The collision sprite of the group
	boost::scoped_ptr<CCollisionSprite3D> spCollision;

	// Joint animated sprites for the visual mesh and the cell mesh.
	CJointAnimSprite3D visual;

	// Collision sprite of the group.
    CCollisionSprite3D collision;

	CCollisionCheck dummyCollision;
};
#endif  // __animatedspritegroup_h__