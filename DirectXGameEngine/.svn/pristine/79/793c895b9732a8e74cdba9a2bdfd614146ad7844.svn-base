
/************************************************************************
*    FILE NAME:       actorsprite2d.cpp
*
*    DESCRIPTION:     Class that holds sprite groups and governs them
*					  with a game specific AI class
************************************************************************/

// Physical component dependency
#include <2d/actorsprite2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/iaibase.h>
#include <2d/objectdatalist2d.h>
#include <2d/spritegroup2d.h>
#include <2d/collisionsprite2d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/cullfunc.h>
#include <managers/actordatalist.h>
#include <managers/instancemeshmanager.h>
#include <common/actordata.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CActorSprite2D::CActorSprite2D()
              : projectionType(CSettings::EPT_NULL)
{
}   // Constructor


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CActorSprite2D::~CActorSprite2D()
{
}	// Destructer


/************************************************************************
*    desc:  Set the AI pointer. This class owns the pointer
************************************************************************/
void CActorSprite2D::SetAI( iAIBase * pAIBase )
{
    spAI.reset(pAIBase);

}	// SetAI


/************************************************************************
*    desc:  Get the AI pointer
*  
*    ret: iAIBase - 
************************************************************************/
iAIBase * CActorSprite2D::GetAIPtr()
{
    return spAI.get();

}	// GetAIPtr


/************************************************************************
*    desc:  Create the actor
*
*    param: const string & name - name of actor sprite
*			bool camera         - whether or not the sprite is transformed
*						          by the camera
*
*    return: const string & - ai name
************************************************************************/
const std::string & CActorSprite2D::Create( const std::string & name, bool camera )
{
    // Get the actor data by name. NOTE: We don't own this pointer!
    CActorData * pActorData = CActorDataList::Instance().GetData( name );

    // Get the instance mesh pointer if this actor is to be instanced
    CInstanceMesh2D * pInstMesh(NULL);
    if( !pActorData->GetInstance().empty() )
        pInstMesh = CInstanceMeshManager::Instance().GetInstanceMeshPtr( pActorData->GetInstance() );

    // Get the position of this actor
    SetPos( pActorData->GetPos() );

    float radius = 0;

    // Get all the sprite group data
    for( size_t i = 0; i < pActorData->GetSpriteDataSize(); ++i )
    {
        CSpriteData & spriteData = pActorData->GetSpriteData(i);

        // Get a copy of the unique mesh tag name extension
        std::string uniqueMeshTag = spriteData.GetUniqueMeshTag();

        // The pointer is placed within the safe map
        spGroupVector.push_back( new CSpriteGroup2D() );

        // Get the object data and init the group
        spGroupVector.back().SetParent( this );

        // Initialize the sprite group
        spGroupVector.back().Init( spriteData );

        // Set the instance mesh pointer
        spGroupVector.back().SetInstanceMesh( pInstMesh );

        // Set the projection type
        if( projectionType == CSettings::EPT_NULL )
            projectionType = spGroupVector.back().GetProjectionType();

        else if( projectionType != spGroupVector.back().GetProjectionType() )
            throw NExcept::CCriticalException( "Actor Sprite Error!", 
                                               boost::str( boost::format("The pieces of the actor sprite are not the same projection type.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ) );

        // Figure out the actor's radius
        float tmpRadius = spGroupVector.back().GetPos().GetLengthSquared() + spGroupVector.back().GetRadius();

        if( tmpRadius > radius )
            radius = tmpRadius;

        // Does this sprite group have a name for east access?
        if( !spriteData.GetName().empty() )
            pGroupMap.insert( make_pair( spriteData.GetName(), &spGroupVector.back() ) );
    }

    // Set the radius
    SetRadius( radius );

    // Intialize the collision sprite
    if( !pActorData->GetCollisionData().GetFile().empty() )
    {
        spCollision.reset( new CCollisionSprite2D( pActorData->GetCollisionData(), this ) );
        //spCollision->SetParent( this );
        //spCollision->Init( pActorData->GetCollisionData().GetWorld(), 
                           //pActorData->GetCollisionData().GetSize(),
                           //pActorData->GetCollisionData().GetType(),
                           //pActorData->GetCollisionData().GetDensity(), 
                           //pActorData->GetCollisionData().GetRestitution() );
    }

    return pActorData->GetAIName();

}	// Create


/************************************************************************
*    desc:  Get the sprite group
*
*    param: uint i - index of the sprite group
*
*    return: CSpriteGroup2D * - pointer to the sprite group
************************************************************************/
CSpriteGroup2D * CActorSprite2D::GetSpriteGroup( uint i )
{
    return &spGroupVector[i];

}	// GetSpriteGroup


/************************************************************************
*    desc:  Get the sprite group
*
*    param: const std::string & name - name of the sprite group
*
*    return: CSpriteGroup2D * - pointer to the sprite group
************************************************************************/
CSpriteGroup2D * CActorSprite2D::GetSpriteGroup( const std::string & name )
{
    groupMapIter = pGroupMap.find( name );
    if( groupMapIter == pGroupMap.end() )
        throw NExcept::CCriticalException( "Actor Sprite Group Access Error!", 
                                           boost::str( boost::format("Sprite group name does not exist (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ) );

    return groupMapIter->second;

}	// GetSpriteGroup


/************************************************************************
*    desc:  Get the collision sprite
*
*    ret:	CCollisionSprite2D & - collision sprite
************************************************************************/
const CCollisionSprite2D * CActorSprite2D::GetCollisionSprite() const
{
    if( !spCollision )
        throw NExcept::CCriticalException("Actor Sprite 2D Error!",
                boost::str( boost::format("Collision sprite is null.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return spCollision.get();

}	// GetCollisionSprite 

CCollisionSprite2D * CActorSprite2D::GetCollisionSprite()
{
    if( !spCollision )
        throw NExcept::CCriticalException("Actor Sprite 2D Error!",
                boost::str( boost::format("Collision sprite is null.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return spCollision.get();

}	// GetCollisionSprite


/************************************************************************
*    desc:  Get the number of sprite groups in the actor
*
*	 param:	size_t - sprite group count
************************************************************************/
size_t CActorSprite2D::GetSpriteGroupCount()
{
    return spGroupVector.size();

}	// GetSpriteGroupCount


/************************************************************************
*    desc:  Test if this actor is valid
************************************************************************/
bool CActorSprite2D::IsValid()
{
    if( spAI.get() != NULL )
        return spAI->IsValid();

    return true;

}	// IsValid


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CActorSprite2D::HandleGameInput()
{
    if( spAI.get() != NULL )
        spAI->HandleGameInput();

}	// HandleGameInput


/************************************************************************
*    desc:  Check for collision and react to it.
************************************************************************/
void CActorSprite2D::ReactToCollision()
{
    if( spAI.get() != NULL )
        spAI->ReactToCollision();

}	// ReactToCollision


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CActorSprite2D::Transform()
{
    if( spCollision )
    {
        // Apply any changes in position, rotation, and scale to the collision sprite
        spCollision->Transform();

        // If the collision sprite isn't static and isn't sleeping, see if we want
        // it to change either the position or rotation of the sprite group
        if( spCollision->GetType() != b2_staticBody && spCollision->IsAwake() )
        {
            if( !GetParameters().IsSet( CObject::TRANSLATE ) )
                SetPos( spCollision->GetInterpPos() );

            if( !GetParameters().IsSet( CObject::ROTATE ) && !spCollision->IsRotationFixed() )
                SetRot( CPoint(0,0,spCollision->GetInterpRot() ) );
        }
    }

    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CObject::Transform();

        // Tell the children of this actor that they need to transform
        for( size_t i = 0; i < spGroupVector.size(); ++i )
            spGroupVector[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the actor's visual components
    for( size_t i = 0; i < spGroupVector.size(); ++i )
        spGroupVector[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the actor
*
*	 param: const CMatrix & _scaledMatrix   - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point       - passed in relative position
************************************************************************/
void CActorSprite2D::Transform( const CMatrix & _scaledMatrix,
                                const CMatrix & _unscaledMatrix, 
                                const CWorldPoint & point )
{
    if( spCollision )
    {
        // Apply any changes in position, rotation, and scale to the collision sprite
        spCollision->Transform();

        // If the collision sprite isn't static and isn't sleeping, see if we want
        // it to change either the position or rotation of the sprite group
        if( spCollision->GetType() != b2_staticBody && spCollision->IsAwake() )
        {
            if( !GetParameters().IsSet( CObject::TRANSLATE ) )
                SetPos( spCollision->GetInterpPos() );

            if( !GetParameters().IsSet( CObject::ROTATE ) && !spCollision->IsRotationFixed() )
                SetRot( CPoint(0,0,spCollision->GetInterpRot() ) );
        }
    }

    if( GetParameters().IsSet( CObject::TRANSFORM ) )
    {
        CBitmask tmpTransParam = GetParameters();
        CObject::Transform( _scaledMatrix, _unscaledMatrix, point );

        // Tell the children of this actor that they need to transform
        for( size_t i = 0; i < spGroupVector.size(); ++i )
            spGroupVector[i].SetTransformParameters( tmpTransParam, true );
    }

    // Transform the actor's visual components
    for( size_t i = 0; i < spGroupVector.size(); ++i )
        spGroupVector[i].Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/************************************************************************
*    desc:  Update animations, Move sprites
************************************************************************/
void CActorSprite2D::Update()
{
    if( spAI.get() != NULL )
        spAI->Update();
    
    for( size_t i = 0; i < spGroupVector.size(); ++i )
        spGroupVector[i].Update();
    
}	// Update


/************************************************************************
*    desc:  Render the actor
************************************************************************/
void CActorSprite2D::Render()
{
    if( IsVisible() && InView() )
    {
        for( size_t i = 0; i < spGroupVector.size(); ++i )
            spGroupVector[i].Render();
    }

}	// Render


/************************************************************************
*    desc:  Is the sprite in the view frustum
*  
*    return: bool - if the sprite is in the view frustum
************************************************************************/
bool CActorSprite2D::InView()
{
    // Test if this object is within the view frustum
    bool inView = false;

    CPoint tmp = CWorldCamera::Instance().GetPos() + GetTransPos();

    if( projectionType == CSettings::EPT_ORTHOGRAPHIC )
        inView = InOrthographicView( tmp, GetRadius() );
    else if( projectionType == CSettings::EPT_PERSPECTIVE )
        inView = InPerspectiveView( tmp, GetRadius() );

    return inView;

}	// InView



