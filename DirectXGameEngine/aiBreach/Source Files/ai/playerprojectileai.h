
/************************************************************************
*    FILE NAME:       playerprojectileai.h
*
*    DESCRIPTION:     Class player projectile AI
************************************************************************/

#ifndef __player_projectile_ai_h__
#define __player_projectile_ai_h__

// Physical component dependency
#include <2d/iaibase.h>

// Standard lib dependencies

// Game lib dependencies
#include <common/size.h>
#include <common/point.h>
#include <common/worldpoint.h>

// Forward declaration(s)
class CSpriteGroup2D;

class CPlayerProjectileAI : public iAIBase
{
public:

    // Constructor
    explicit CPlayerProjectileAI( CActorSprite2D * _pActor );

    // Destructor
    virtual ~CPlayerProjectileAI();

    // Initialize the projectile
    void Init( CPoint & shooterVel );

    // Check for collision and react to it
    virtual void ReactToCollision(){};

    // Update animations, Move sprites
    virtual void Update();

private:

    // The velocity and ship momentum of the projectile
    CPoint velocity;
    
    // A boolean for when it's our first update of this projectile
    bool firstUpdate;

    // A pointer to the ship that fired the projectile
    CActorSprite2D * pShipActor;

    // The initial position of the projectile
    CWorldPoint initialPos;

};

#endif  // __player_projectile_ai_h__


