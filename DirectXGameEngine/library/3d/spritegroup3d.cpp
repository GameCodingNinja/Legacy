/************************************************************************
*    FILE NAME:       spritegroup3d.cpp
*
*    DESCRIPTION:     SpriteGroup class
************************************************************************/

// Physical component dependency
#include <3d/spritegroup3d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <3d/objectdatalist3d.h>
#include <3d/visualsprite3D.h>
#include <3d/jointanimationsprite3d.h>
#include <3d/collisionsprite3d.h>
#include <3d/objectdata3d.h>
#include <3d/collisioncheck.h>
#include <utilities/genfunc.h>
#include <common/matrix.h>
#include <common/point.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSpriteGroup3D::CSpriteGroup3D()
              : pObjectData(NULL)
{
}	// Constructer

CSpriteGroup3D::CSpriteGroup3D( CObjectData3D * pObjData )
              : pObjectData(pObjData)
{
    Init();

}	// Constructer


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CSpriteGroup3D::~CSpriteGroup3D()
{
}	// Destructor


/************************************************************************
*    desc:  Init the sprite group
************************************************************************/
void CSpriteGroup3D::Init( const std::string & group, const std::string & type )
{
    pObjectData = CObjectDataList3D::Instance().GetData( group, type );

    Init();

}	// Init


/************************************************************************
*    desc:  Init the sprite group
************************************************************************/
void CSpriteGroup3D::Init()
{
    // Initialize the visual sprite
    if( !GetVisualData().GetFile().empty() )
    {
        if( GetVisualData().IsAnimated() )
            spVisual.reset( new CJointAnimSprite3D( pObjectData ) );
        else
            spVisual.reset( new CVisualSprite3D( pObjectData ) );
    }

    // Intialize the collision sprite
    if( !GetCollisionData().GetFile().empty() )
        spCollision.reset( new CCollisionSprite3D( pObjectData ) );

}	// Init


/************************************************************************
*    desc:  Set the sprite position
*
*    param: Point position - pos to set
************************************************************************/
void CSpriteGroup3D::SetPos( CPoint & position )
{
    if( spVisual )
        spVisual->SetPos( position );

    if( spCollision )
        spCollision->SetPos( position );

}	// SetPos

void CSpriteGroup3D::SetPos( float x, float y, float z )
{
    SetPos( CPoint(x, y, z) );

}	// SetPos


/************************************************************************
*    desc:  Set the pre-translation matrix
*
*    param: Point rotation - rotation amount
************************************************************************/
void CSpriteGroup3D::SetRot( CPoint & rotation )
{
    if( spVisual )
        spVisual->SetRot( rotation );

    if( spCollision )
        spCollision->SetRot( rotation );

}	// SetRot

void CSpriteGroup3D::SetRot( float x, float y, float z )
{
    SetRot( CPoint(x, y, z) );

}	// SetRot


/************************************************************************
*    desc:  Inc the sprite position
*
*    param: Point position - pos to inc
************************************************************************/
void CSpriteGroup3D::IncPos( CPoint & position )
{
    if( spVisual )
        spVisual->IncPos( position );

    if( spCollision )
        spCollision->IncPos( position );

}	// IncPos

void CSpriteGroup3D::IncPos( float x, float y, float z )
{
    IncPos( CPoint(x, y, z) );

}	// IncPos


/************************************************************************
*    desc:  Inc the pre-translation matrix
*
*    param: Point rotation - rotation inc
************************************************************************/
void CSpriteGroup3D::IncRot( CPoint & rotation )
{
    if( spVisual )
        spVisual->IncRot( rotation );

    if( spCollision )
        spCollision->IncRot( rotation );

}	// IncRot

void CSpriteGroup3D::IncRot( float x, float y, float z )
{
    IncRot( CPoint(x, y, z) );

}	// IncRot


/************************************************************************
*    desc:  Transform the sprite's center point
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CSpriteGroup3D::Transform( CMatrix & matrix )
{
    // Transform the center of the object
    if( spVisual )
        spVisual->Transform( matrix );

    if( spCollision )
        spCollision->Transform();

}	// Transform


/************************************************************************
*    desc:  Scale the sprite's size
*
*    param: float scale - scale percentage
************************************************************************/
void CSpriteGroup3D::SetScale( float scale )
{
    if( spVisual )
        spVisual->SetScale( scale );

    if( spCollision )
        spCollision->SetScale( scale );

}	// SetScale

void CSpriteGroup3D::SetScale( CPoint & scale )
{
    if( spVisual )
        spVisual->SetScale( scale );

    if( spCollision )
        spCollision->SetScale( scale );

}	// SetScale


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
CCollisionCheck & CSpriteGroup3D::ReactToCollision( CPoint & center, float radius,
                                                          float floorPadding, float wallPadding )
{
    return spCollision->ReactToCollision( center, radius, floorPadding, wallPadding );

}	// ReactToCollision


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
CCollisionCheck & CSpriteGroup3D::ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
                                                          float floorPadding, float wallPadding )
{
    return spCollision->ReactToCollision( matrix, center, radius, floorPadding, wallPadding );

}	// ReactToCollision


/************************************************************************
*    desc:  Get the desired Sprite
*
*    return: GLSprite, CollisionSprite, or Shdadow Sprite
************************************************************************/
CVisualSprite3D & CSpriteGroup3D::GetVisualSprite()
{
    return *spVisual;

}	// GetVisualSprite

CCollisionSprite3D & CSpriteGroup3D::GetCollisionSprite()
{
    return *spCollision;

}	// GetCollisionSprite


/************************************************************************
*    desc:  Get the visual data
*
*    ret:	const CObjectVisualData2D & - visual data
************************************************************************/
const CObjectVisualData3D & CSpriteGroup3D::GetVisualData() const
{
    return pObjectData->GetVisualData();

}	// GetVisualData


/************************************************************************
*    desc:  Get the collision data
*
*    ret:	const CObjectCollisionData2D & - collision data
************************************************************************/
const CObjectCollisionData3D & CSpriteGroup3D::GetCollisionData() const
{
    return pObjectData->GetCollisionData();

}	// GetCollisionData


/************************************************************************
*    desc:  Is the visual sprite active
*  
*    return: bool - if the visual sprite is active
************************************************************************/
bool CSpriteGroup3D::IsVisualActive() const
{
    return (spVisual != nullptr);

}	// IsVisualActive


/************************************************************************
*    desc:  Is the collision sprite active
*  
*    return: bool - if the collision sprite is active
************************************************************************/
bool CSpriteGroup3D::IsCollisionActive() const
{
    return (spCollision != nullptr);

}	// IsCollisionActive


/************************************************************************
*    desc:  Sets the current animation
*
*    param: string & name - name of animation
************************************************************************/
void CSpriteGroup3D::SetAnimation( const std::string & name )
{
    if( spVisual && GetVisualData().IsAnimated() )
        NGenFunc::DynCast<CJointAnimSprite3D *>(spVisual.get())->SetAnimation( name );

}	// SetAnimation


/************************************************************************
*    desc:  Inc the animation timer
************************************************************************/
void CSpriteGroup3D::IncAnimationTime()
{
    if( spVisual && GetVisualData().IsAnimated() )
        NGenFunc::DynCast<CJointAnimSprite3D *>(spVisual.get())->IncAnimationTime();

}	// UpdateAnimation
