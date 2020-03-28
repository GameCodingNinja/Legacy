/************************************************************************
*    FILE NAME:       spritegroup2d.h
*
*    DESCRIPTION:     2D sprite group class that holds a visual and
*					  collision component
************************************************************************/

#ifndef __sprite_group_2d_h__
#define __sprite_group_2d_h__

// Physical component dependency
#include <scripting/scriptobject.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <common/point.h>
#include <common/worldpoint.h>
#include <2d/objectdatalist2d.h>
//#include <2d/objectdata2d.h>
#include <3d/worldcamera.h>
#include <utilities/genfunc.h>
#include <misc/settings.h>

// Forward declaration(s)
class CVisualSprite2D;
class CCollisionSprite2D;
class iSmartSpriteGroup;
class CInstanceMesh2D;
class CSpriteData;
class CObjectData2D;
class CObjectVisualData2D;
class CObjectCollisionData2D;
class CObjectScriptData2D;
namespace NText { class CTextureFor2D; }

class CSpriteGroup2D : public CScriptObject 
{
public:

    // Constructor
    CSpriteGroup2D();
    CSpriteGroup2D( const CSpriteData & spriteData );
    CSpriteGroup2D( CObjectData2D * pObjData, bool camera = false, std::string & meshTag = std::string("") );

    // Destructor
    ~CSpriteGroup2D();

    // Initialize the group
    void Init( bool camera, std::string & meshTag );
    void Init( const CSpriteData & spriteData );
    void Init( const std::string & group,
               const std::string & type,
               bool camera = false,
               std::string & meshTag = std::string("") );

    // Transform the sprite group
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, 
                            const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );
    
    // Update the sprite
    void Update();

    // Render the sprite
    void Render();

    // See if the sprite is in view
    bool InView();

    // Is the visual or collision sprite active
    bool IsVisualActive() const;
    bool IsCollisionActive() const;

    // Get the size of the sprite's visual mesh
    CSize<float> GetSize( bool scaled = true ) const;

    // Get the visual sprite
    const CVisualSprite2D * GetVisualSprite() const;
    CVisualSprite2D * GetVisualSprite();

    // Get the collision sprite
    const CCollisionSprite2D * GetCollisionSprite() const;
    CCollisionSprite2D * GetCollisionSprite();

    // Get the object data components
    const CObjectVisualData2D & GetVisualData() const;
    const CObjectCollisionData2D & GetCollisionData() const;
    const CObjectScriptData2D & GetScriptData() const;

    // Check if the passed in point is within the sprite
    bool IsPointInSprite( const CPoint & point );
    bool IsPointInSprite( const CPointInt & point );
    bool IsPointInSprite( const CWorldPoint & point );

    // Set and get the current frame of animation
    void SetCurrentFrame( int frame );
    int GetCurrentFrame() const;

    // Get the frame count
    int GetFrameCount() const;

    // Set, get, and increment the frame timer
    void SetFrameTimer( float value );
    void IncFrameTimer( float value );
    float GetFrameTimer() const;

    // Get the active texture
    NText::CTextureFor2D * GetActiveTexture();

    // Get the projection type
    CSettings::EProjectionType GetProjectionType();

    // Get the sprite group's color multiplied by its visual sprite's color
    CColor GetResultColor() const;

    // Set the AI pointer
    void SetSmartSpriteGrp( iSmartSpriteGroup * pSmartSpriteGrp );

    // Set the instance mesh pointer
    void SetInstanceMesh( CInstanceMesh2D * pInstance );

private:

    // Object's data. 
    // NOTE: This pointer belongs to the CObjectDataList2D singleton
    CObjectData2D * pObjectData;

    // iSmart Sprite Group Interface
    boost::scoped_ptr<iSmartSpriteGroup> spSmartSpriteGrp;

    // The visual sprite of the group
    boost::scoped_ptr<CVisualSprite2D> spVisual;

    // The collision sprite of the group
    boost::scoped_ptr<CCollisionSprite2D> spCollision;

    // Instance mesh that the sprite uses
    // NOTE: This pointer belongs to the CInstanceMeshManager singleton
    CInstanceMesh2D * pInstanceMesh;
};

#endif  // __group_h__
