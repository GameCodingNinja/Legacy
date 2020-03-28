
/************************************************************************
*    FILE NAME:       actorsprite2d.h
*
*    DESCRIPTION:     Class that holds sprite groups and governs them
*					  with a game specific AI class
************************************************************************/

#ifndef __actor_sprite_2d_h__
#define __actor_sprite_2d_h__

// Physical component dependency
#include <scripting/scriptobject.h>

// Standard lib dependencies
#include <string>
#include <map>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <common/worldpoint.h>
#include <misc/settings.h>

// Forward declaration(s)
class CCollisionSprite2D;
class iAIBase;
class CSpriteGroup2D;
class CActorData;

class CActorSprite2D : public CScriptObject
{
public:

    // Constructor
    CActorSprite2D();

    // Destructor
    virtual ~CActorSprite2D();

    // Create the actor
    const std::string & Create( const std::string & name, bool camera = false );

    // Set the AI pointer
    void SetAI( iAIBase * pAIBase );

    // Get the AI pointer
    iAIBase * GetAIPtr();

    // Test if this actor is valid
    bool IsValid();

    // React to what the player is doing
    void HandleGameInput();

    // Check for collision and react to it
    void ReactToCollision();

    // Update the actor
    void Update();

    // Render the actor
    void Render();

    // Is the sprite in the view frustum
    bool InView();

    // Get the sprite group
    CSpriteGroup2D * GetSpriteGroup( uint i );
    CSpriteGroup2D * GetSpriteGroup( const std::string & name );

    // Get the collision sprite
    const CCollisionSprite2D * GetCollisionSprite() const;
    CCollisionSprite2D * GetCollisionSprite();

    // Get the number of sprite groups in the actor
    size_t GetSpriteGroupCount();

    // Transform the actor
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

private:

    // Base AI class pointer
    boost::scoped_ptr<iAIBase> spAI;

    // Vector of sprite groups
    boost::ptr_vector<CSpriteGroup2D> spGroupVector;
    boost::ptr_vector<CSpriteGroup2D>::iterator groupIter;

    // Map of sprite groups for easy access
    std::map<std::string, CSpriteGroup2D *> pGroupMap;
    std::map<std::string, CSpriteGroup2D *>::iterator groupMapIter;

    // Actor's collision sprite
    boost::scoped_ptr<CCollisionSprite2D> spCollision;

    // The projection type of the sprite
    CSettings::EProjectionType projectionType;

};

#endif  // __actor_sprite_2d_h__


