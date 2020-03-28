/************************************************************************
*    FILE NAME:       animatedspritegroup3d.cpp
*
*    DESCRIPTION:     Animated sprite group class
************************************************************************/

// Physical component dependency
#include <3d/animatedspritegroup3d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CAnimatedSpriteGroup3D::CAnimatedSpriteGroup3D()
{
}	// Constructer */

/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CAnimatedSpriteGroup3D::~CAnimatedSpriteGroup3D()
{
}	// Destructor */


/************************************************************************
*    desc:  Init the sprite
************************************************************************/
void CAnimatedSpriteGroup3D::Init( std::string & group, std::string & type )
{
	pObjectData = CObjectDataList3D::Instance().GetData( group, type );

	// Initialize the visual sprite
	if( !GetVisualData().GetFile().empty() )
	{
		spVisual.reset( new CVisualSprite3D );
		spVisual->Init( *pObjectData );
	}

	// Intialize the collision sprite
	if( !GetCollisionData().GetFile().empty() )
	{
		spCollision.reset( new CCollisionSprite3D );
		spCollision->Init( *pObjectData );
	}

}	// InitVisual


/************************************************************************
*    desc:  Set the sprite position
*
*    param: Point & position - pos to set
************************************************************************/
void CAnimatedSpriteGroup3D::SetPos( CPoint & position )
{
    if( visual.IsActive() )
        visual.SetPos( position );

    if( collision.IsActive() )
        collision.SetPos( position );

}   //SetPos */

void CAnimatedSpriteGroup3D::SetPos( float x, float y, float z )
{
    SetPos( CPoint(x, y, z) );

}	//SetPos */

/************************************************************************
*    desc:  Set the pre-translation matrix
*
*    param: Point & rotation - rotation amount
************************************************************************/
void CAnimatedSpriteGroup3D::SetRot( CPoint & rotation )
{
    if( visual.IsActive() )
        visual.SetRot( rotation );

    if( collision.IsActive() )
        collision.SetRot( rotation );

}	// SetRot */

void CAnimatedSpriteGroup3D::SetRot( float x, float y, float z )
{
	SetRot( CPoint(x, y, z) );

}	// SetRot */

/************************************************************************
*    desc:  Inc the sprite position
*
*    param: Point & position - pos to inc
************************************************************************/
void CAnimatedSpriteGroup3D::IncPos( CPoint & position )
{
    if( visual.IsActive() )
        visual.IncPos( position );

    if( collision.IsActive() )
        collision.IncPos( position );

}	// IncPos */

void CAnimatedSpriteGroup3D::IncPos( float x, float y, float z )
{
	IncPos( CPoint(x, y, z) );

}	// IncPos */

/************************************************************************
*    desc:  Inc the pre-translation matrix
*
*    param: Point & rotation - rotation inc
************************************************************************/
void CAnimatedSpriteGroup3D::IncRot( CPoint & rotation )
{
    if( visual.IsActive() )
        visual.IncRot( rotation );

    if( collision.IsActive() )
        collision.IncRot( rotation );

}	// IncRot */

void CAnimatedSpriteGroup3D::IncRot( float x, float y, float z )
{
	IncRot( CPoint(x, y, z) );

}	// IncRot */


/************************************************************************
*    desc:  Transform the sprite's center point
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CAnimatedSpriteGroup3D::Transform( CMatrix & matrix )
{
    // Transform the center of the object
    if( visual.IsActive() )
        visual.Transform( matrix );

    if( collision.IsActive() )
        collision.Transform();

}	// Transform */

/************************************************************************
*    desc:  Scale the sprite's size
*
*    param: float scale - scale percentage
************************************************************************/
void CAnimatedSpriteGroup3D::SetScale( float scale )
{
    if( visual.IsActive() )
        visual.SetScale( scale );

    if( collision.IsActive() )
        collision.SetScale( scale );

}	// SetScale */

void CAnimatedSpriteGroup3D::SetScale( CPoint & scale )
{
    if( visual.IsActive() )
        visual.SetScale( scale );

    if( collision.IsActive() )
        collision.SetScale( scale );

}	// SetScale */


/************************************************************************
*    desc:  Scale the sprite's default size
*
*    param: float scale - scale percentage
************************************************************************/
void CAnimatedSpriteGroup3D::SetDefaultScale( float scale )
{
    if( visual.IsActive() )
        visual.SetDefaultScale( scale );

    if( collision.IsActive() )
        collision.SetDefaultScale( scale );

}	// SetDefaultScale */

void CAnimatedSpriteGroup3D::SetDefaultScale( CPoint & scale )
{
    if( visual.IsActive() )
        visual.SetDefaultScale( scale );

    if( collision.IsActive() )
        collision.SetDefaultScale( scale );

}	// SetDefaultScale */


/************************************************************************
*    desc:  Sets the current animation
*
*    param: string & name - name of animation
************************************************************************/
void CAnimatedSpriteGroup3D::SetAnimation( const string & name )
{
    if( visual.IsActive() )
		visual.SetAnimation( name );

}	// SetAnimation */


/************************************************************************
*    desc:  Inc the animation timer
************************************************************************/
void CAnimatedSpriteGroup3D::IncAnimationTime()
{
    if( visual.IsActive() )
		visual.IncAnimationTime();

}	// UpdateAnimation */


/************************************************************************
*    desc:  Check for collision and react to it. World view collision
*
*    param: CPoint center - Center point of the object
*           float radius - radius of the object
*           float floorPadding - padding for collision
*           float wallPadding - padding for collision
*
*    ret: CCollisionCheck & - All collision information
************************************************************************/
CCollisionCheck & CAnimatedSpriteGroup3D::ReactToCollision( CPoint & center, float radius,
														    float floorPadding, float wallPadding )
{
	if( collision.IsActive() )
		return collision.ReactToCollision( center, radius, floorPadding, wallPadding );

	return dummyCollision;

}	// ReactToCollision */


/************************************************************************
*    desc:  Check for collision and react to it. Camera view collision
*
*    param: CMatric & matrix - passed in matrix class
*			CPoint center - Center point of the object
*           float radius - radius of the object
*           float floorPadding - padding for collision
*           float wallPadding - padding for collision
*
*    ret: CCollisionCheck & - All collision information
************************************************************************/
CCollisionCheck & CAnimatedSpriteGroup3D::ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
														    float floorPadding, float wallPadding )
{
	if( collision.IsActive() )
		return collision.ReactToCollision( matrix, center, radius, floorPadding, wallPadding );

	return dummyCollision;

}	// ReactToCollision */


/************************************************************************
*    desc:  Get the visual data
*
*    ret:	const CObjectVisualData2D & - visual data
************************************************************************/
const CObjectVisualData3D & CAnimatedSpriteGroup3D::GetVisualData() const
{
    return pObjectData->GetVisualData();

}	// GetVisualData */


/************************************************************************
*    desc:  Get the collision data
*
*    ret:	const CObjectCollisionData2D & - collision data
************************************************************************/
const CObjectCollisionData3D & CAnimatedSpriteGroup3D::GetCollisionData() const
{
    return pObjectData->GetCollisionData();

}	// GetCollisionData */


/************************************************************************
*    desc:  Get the desired Sprite
*
*    ret:	visual sprite, visual cell sprite, collision sprite, or 
*			shadow sprite
************************************************************************/
CJointAnimSprite3D & CAnimatedSpriteGroup3D::GetVisualSprite()
{
    return visual;

}	// GetVisualSprite */

CCollisionSprite3D & CAnimatedSpriteGroup3D::GetCollisionSprite()
{
    return collision;

}	// GetCollisionSprite */