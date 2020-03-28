
/************************************************************************
*    FILE NAME:       playershipai.h
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

#ifndef __player_ship_ai_h__
#define __player_ship_ai_h__

// Physical component dependency
#include <2d/iaibase.h>

// Forward declaration(s)
class CActorSprite;

class CPlayerShipAI : public iAIBase
{
public:

    // Constructor
    explicit CPlayerShipAI( CActorSprite2D * _pActor );

    // Destructor
    virtual ~CPlayerShipAI();

    // React to what the player is doing
    virtual void HandleGameInput();

    // Check for collision and react to it
    virtual void ReactToCollision(){};

    // Update animations, Move sprites
    virtual void Update(){}

private:

};

#endif  // __player_ship_ai_h__


