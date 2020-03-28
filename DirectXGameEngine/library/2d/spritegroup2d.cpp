/************************************************************************
*    FILE NAME:       spritegroup2d.cpp
*
*    DESCRIPTION:     2D sprite group class that holds a visual and
*					  collision component
************************************************************************/

// Physical component dependency
#include <2d/spritegroup2d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <scripting/scriptmanager.h>
#include <managers/shader.h>
#include <misc/settings.h>
#include <utilities/exceptionhandling.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/cullfunc.h>
#include <scripting/scriptmanager.h>
#include <2d/ismartspritegroup.h>
#include <2d/instancemesh2d.h>
#include <2d/visualsprite2d.h>
#include <2d/collisionsprite2d.h>
#include <2d/objectdatalist2d.h>
#include <2d/objectdata2d.h>
#include <common/spritedata.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSpriteGroup2D::CSpriteGroup2D()
              : pObjectData(NULL),
                pInstanceMesh(NULL)
{
}	// Constructor

CSpriteGroup2D::CSpriteGroup2D( const CSpriteData & spriteData )
              : pObjectData(NULL),
                pInstanceMesh(NULL)
{
    Init( spriteData );

}	// Constructor

CSpriteGroup2D::CSpriteGroup2D( CObjectData2D * pObjData, bool camera, std::string & meshTag )
              : pObjectData(pObjData),
                pInstanceMesh(NULL)
{
    Init( camera, meshTag );

}	// Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CSpriteGroup2D::~CSpriteGroup2D()
{
}	// Destructor


/************************************************************************
*    desc:  Initilizes the sprite group
************************************************************************/
void CSpriteGroup2D::Init( const std::string & group,
                           const std::string & type,
                           bool camera,
                           std::string & meshTag )
{
    // Init our pointer to this objects data. NOTE: We don't own this pointer
    pObjectData = CObjectDataList2D::Instance().GetData( group, type );

    Init( camera, meshTag );

}	// Init


/************************************************************************
*    desc:  Initilizes the sprites
*
*	 param:	const CSpriteData & spriteData - data to initialize the sprite
*											 with
************************************************************************/
void CSpriteGroup2D::Init( const CSpriteData & spriteData )
{
    // Init our pointer to this objects data. NOTE: We don't own this pointer
    pObjectData = CObjectDataList2D::Instance().GetData( spriteData.GetGroup(), spriteData.GetType() );

    // A negative width or height indicates full screen size
    if( ( GetVisualData().GetSize().w < 0 ) || ( GetVisualData().GetSize().h < 0 ) )
    {
        CSize<float> size;
        size.w = CSettings::Instance().GetDefaultSize().w;
        size.h = CSettings::Instance().GetDefaultSize().h;

        pObjectData->GetVisualData().SetSize( size );
    }

    // Set the transformations of the sprite
    SetTransData( spriteData.GetTransformData() );

    // Initialize the visual sprite
    if( !GetVisualData().GetFile().empty() )
    {
        spVisual.reset( new CVisualSprite2D( pObjectData, spriteData.UseCamera(), spriteData.GetUniqueMeshTag() ) );
        spVisual->SetParent( this );
        SetRadius( spVisual->GetRadius() );
    }

    // Intialize the collision sprite
    if( !GetCollisionData().GetFile().empty() )
        spCollision.reset( new CCollisionSprite2D( pObjectData, this ) );

    // Determine which radius to use for the sprite group
    if( spVisual )
        SetRadius( spVisual->GetRadius() );

    // Set up the script
    if( !GetScriptData().GetName().empty() )
        SetCommandScript( CScriptMgr::Instance().GetCommandScript( 
                          pObjectData->GetGroup(), GetScriptData().GetName() ) );

    // Start the default command
    if( !GetScriptData().GetCommand().empty() )
        SetActiveCommand( GetScriptData().GetCommand() );

}	// Init


/************************************************************************
*    desc:  Initilizes the sprites
*
*	 param:	bool camera	     - whether or not the sprite is transformed
*							   by the camera
*			string & meshTag - the unique tag for the mesh
************************************************************************/
void CSpriteGroup2D::Init( bool camera, std::string & meshTag )
{
    // A negative width or height indicates full screen size
    if( ( GetVisualData().GetSize().w < 0 ) || ( GetVisualData().GetSize().h < 0 ) )
    {
        CSize<float> size;
        size.w = CSettings::Instance().GetDefaultSize().w;
        size.h = CSettings::Instance().GetDefaultSize().h;

        pObjectData->GetVisualData().SetSize( size );
    }

    // Initialize the visual sprite
    if( !GetVisualData().GetFile().empty() )
    {
        spVisual.reset( new CVisualSprite2D( pObjectData, camera, meshTag ) );
        spVisual->SetParent( this );
        SetRadius( spVisual->GetRadius() );
    }

    // Intialize the collision sprite
    if( !GetCollisionData().GetFile().empty() )
        spCollision.reset( new CCollisionSprite2D( pObjectData, this ) );

    // Determine which radius to use for the sprite group
    if( spVisual )
        SetRadius( spVisual->GetRadius() );

    // Set up the script
    if( !GetScriptData().GetName().empty() )
        SetCommandScript( CScriptMgr::Instance().GetCommandScript( 
                          pObjectData->GetGroup(), GetScriptData().GetName() ) );

    // Start the default command
    if( !GetScriptData().GetCommand().empty() )
        SetActiveCommand( GetScriptData().GetCommand() );

}	// Init


/************************************************************************
*    desc:  Transform the sprite group
************************************************************************/
void CSpriteGroup2D::Transform()
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

        // Tell the visual and collision sprites that they need to transform
        if( spVisual )
            spVisual->SetTransformParameters( tmpTransParam, true );
    }

    // Transform the visual sprite
    if( spVisual )
        spVisual->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );

}	// Transform


/************************************************************************
*    desc:  Transform the sprite group
*
*	 param: const CMatrix & _scaledMatrix   - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point       - passed in relative position
************************************************************************/
void CSpriteGroup2D::Transform( const CMatrix & _scaledMatrix, 
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

        // Tell the visual and collision sprites that they need to transform
        if( spVisual )
            spVisual->SetTransformParameters( tmpTransParam, true );
    }

    // Transform the visual sprite
    if( spVisual )
        spVisual->Transform( GetScaledMatrix(), GetUnscaledMatrix(), GetTransPos() );
    
}	// Transform


/****************************************************************************
*    desc:	Update the sprite group
****************************************************************************/
void CSpriteGroup2D::Update()
{
    // Update the scripting 
    CScriptObject::Update();

    // Update the smart sprite group
    if( spSmartSpriteGrp )
        spSmartSpriteGrp->Update();

}	// Update


/****************************************************************************
*    desc:	Render the visual sprite
****************************************************************************/
void CSpriteGroup2D::Render()
{
    // Render the visual sprite
    if( IsVisualActive() && IsVisible() )
    {
        // If it's in view, we render it
        if( InView() )
        {
            if( pInstanceMesh != NULL )
                pInstanceMesh->AddSprite( this );
            else
                spVisual->Render( GetColor(), true );
        }
    }

}	// Render


/************************************************************************
*    desc:  Is the sprite in the view frustum
*  
*    return: bool - if the sprite is in the view frustum
************************************************************************/
bool CSpriteGroup2D::InView()
{
    // Test if this object is within the view frustum
    bool inView = false;

    CPoint tmp;

    if( spVisual->IsCameraUsed() )
        tmp = CWorldCamera::Instance().GetPos() + GetTransPos();
    else
        tmp = GetTransPos();

    if( spVisual->GetProjectionType() == CSettings::EPT_ORTHOGRAPHIC )
        inView = InOrthographicView( tmp, GetRadius() );
    else if( spVisual->GetProjectionType() == CSettings::EPT_PERSPECTIVE )
        inView = InPerspectiveView( tmp, GetRadius() );

    return inView;

}	// InView


/************************************************************************
*    desc:  Is the visual sprite active
*  
*    ret:	bool - if the visual sprite is active
************************************************************************/
bool CSpriteGroup2D::IsVisualActive() const
{
    return (spVisual != nullptr);

}	// IsVisualActive


/************************************************************************
*    desc:  Is the collision sprite active
*  
*    ret:	bool - if the collision sprite is active
************************************************************************/
bool CSpriteGroup2D::IsCollisionActive() const
{
    return (spCollision != nullptr);

}	// IsCollisionActive


/************************************************************************
*    desc:  Get the size of the sprite's visual mesh
*
*	 param:	
*  
*    ret:	CSize<float> - the size
************************************************************************/
CSize<float> CSpriteGroup2D::GetSize( bool scaled ) const
{
    return spVisual->GetSize( scaled );

}	// GetSize


/************************************************************************
*    desc:  Get the visual sprite
*
*    ret:	CVisualSprite2D * - visual sprite
************************************************************************/
const CVisualSprite2D * CSpriteGroup2D::GetVisualSprite() const
{
    if( !spVisual )
        throw NExcept::CCriticalException("Sprite Group 2D Error!",
                boost::str( boost::format("Visual sprite is null.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return spVisual.get();

}	// GetVisualSprite 

CVisualSprite2D * CSpriteGroup2D::GetVisualSprite()
{
    if( !spVisual )
        throw NExcept::CCriticalException("Sprite Group 2D Error!",
                boost::str( boost::format("Visual sprite is null.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return spVisual.get();

}	// GetVisualSprite


/************************************************************************
*    desc:  Get the collision sprite
*
*    ret:	CCollisionSprite2D & - collision sprite
************************************************************************/
const CCollisionSprite2D * CSpriteGroup2D::GetCollisionSprite() const
{
    if( !spCollision )
        throw NExcept::CCriticalException("Sprite Group 2D Error!",
                boost::str( boost::format("Collision sprite is null.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return spCollision.get();

}	// GetCollisionSprite 

CCollisionSprite2D * CSpriteGroup2D::GetCollisionSprite()
{
    if( !spCollision )
        throw NExcept::CCriticalException("Sprite Group 2D Error!",
                boost::str( boost::format("Collision sprite is null.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

    return spCollision.get();

}	// GetCollisionSprite


/************************************************************************
*    desc:  Get the visual data
*
*    ret:	const CObjectVisualData2D & - visual data
************************************************************************/
const CObjectVisualData2D & CSpriteGroup2D::GetVisualData() const
{
    return pObjectData->GetVisualData();

}	// GetVisualData


/************************************************************************
*    desc:  Get the collision data
*
*    ret:	const CObjectCollisionData2D & - collision data
************************************************************************/
const CObjectCollisionData2D & CSpriteGroup2D::GetCollisionData() const
{
    return pObjectData->GetCollisionData();

}	// GetCollisionData


/************************************************************************
*    desc:  Get the script data
*
*    ret:	const CObjectScriptData2D & - script data
************************************************************************/
const CObjectScriptData2D & CSpriteGroup2D::GetScriptData() const
{
    return pObjectData->GetScriptData();

}	// GetCollisionData


/************************************************************************
*    desc:  Check if the passed in point is within the sprite
*  
*    param:	const CPoint/CPointInt/CWorldPoint & - point to check
*
*	 ret:	bool - result
************************************************************************/
bool CSpriteGroup2D::IsPointInSprite( const CPoint & point )
{
    if( spCollision )
        return spCollision->IsPointInSprite( point );

    return false;

}	// IsPointInSprite

bool CSpriteGroup2D::IsPointInSprite( const CPointInt & point )
{
    if( spCollision )
        return spCollision->IsPointInSprite( point );

    return false;

}	// IsPointInSprite

bool CSpriteGroup2D::IsPointInSprite( const CWorldPoint & point )
{
    if( spCollision )
        return spCollision->IsPointInSprite( point );

    return false;

}	// IsPointInSprite


/************************************************************************
*    desc:  Set the current frame
*  
*    param: int frame - frame to set to
************************************************************************/
void CSpriteGroup2D::SetCurrentFrame( int frame )
{
    if( spVisual )
        spVisual->SetCurrentFrame( frame );

}	// SetCurrentFrame


/************************************************************************
*    desc:  Get the current frame  
*
*	 ret:	int - the current frame
************************************************************************/
int CSpriteGroup2D::GetCurrentFrame() const
{
    if( spVisual )
        return spVisual->GetCurrentFrame();

    return 0;

}	// GetCurrentFrame


/************************************************************************
*    desc:  Get the frame count
*
*    ret:	int -
************************************************************************/
int CSpriteGroup2D::GetFrameCount() const
{
    if( spVisual )
        return spVisual->GetFrameCount();

    return 0;

}	// GetFrameCount


/************************************************************************
*    desc:  Set the frame timer
*  
*    param: float value - time to set to
************************************************************************/
void CSpriteGroup2D::SetFrameTimer( float value )
{
    if( spVisual )
        spVisual->SetFrameTimer( value );

}	// SetFrameTimer


/************************************************************************
*    desc:  Increment the frame timer
*  
*    param: float value - time to increment by
************************************************************************/
void CSpriteGroup2D::IncFrameTimer( float value )
{
    if( spVisual )
        spVisual->IncFrameTimer( value );

}	// IncFrameTimer


/************************************************************************
*    desc:  Get the frame timer  
*
*	 ret:	float - time elapsed in the frame
************************************************************************/
float CSpriteGroup2D::GetFrameTimer() const
{
    if( spVisual )
        return spVisual->GetFrameTimer();

    return 0;

}	// GetFrameTimer


/************************************************************************
*    desc:  Get the active texture
*  
*    return: NText::CTextureFor2D * - projection type
************************************************************************/
NText::CTextureFor2D * CSpriteGroup2D::GetActiveTexture()
{
    if( spVisual )
        return spVisual->GetActiveTexture();

    return NULL;

}	// GetActiveTexture


/************************************************************************
*    desc:  Get the projection type
*  
*    return: CSettings::EProjectionType - projection type
************************************************************************/
CSettings::EProjectionType CSpriteGroup2D::GetProjectionType()
{
    if( spVisual )
        return spVisual->GetProjectionType();

    return CSettings::EPT_ORTHOGRAPHIC;

}	// GetProjectionType


/************************************************************************
*    desc:  Get the sprite group's color multiplied by its visual sprite's  
*			color
*
*	 ret:	CColor - multiplied color
************************************************************************/
CColor CSpriteGroup2D::GetResultColor() const
{
    if( spVisual )
        return GetColor() * spVisual->GetSpriteColor();

    return GetColor();

}	// GetResultColor


/************************************************************************
*    desc:  Set the Smart Sprite Group pointer. This class owns the pointer
************************************************************************/
void CSpriteGroup2D::SetSmartSpriteGrp( iSmartSpriteGroup * pSmartSpriteGrp )
{
    spSmartSpriteGrp.reset(pSmartSpriteGrp);

}	// SetSmartSpriteGrp


/************************************************************************
*    desc:  Set the instance mesh pointer
*  
*    param: CInstanceMesh2D * pInstance - instance mesh the sprite now
*										  belongs to
************************************************************************/
void CSpriteGroup2D::SetInstanceMesh( CInstanceMesh2D * pInstance )
{
    pInstanceMesh = pInstance;

}	// SetInstanceMesh