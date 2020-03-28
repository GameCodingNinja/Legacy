
/************************************************************************
*    FILE NAME:       enemyshipai.h
*
*    DESCRIPTION:     Class enemy ship AI
************************************************************************/

#ifndef __enemy_ship_ai_h__
#define __enemy_ship_ai_h__

// Physical component dependency
#include <2d/iaibase.h>

// Standard lib dependencies

// Game lib dependencies

// Boost lib dependencies

// Forward declaration(s)

class CEnemyShipAI : public iAIBase
{
public:

    // Constructor
    explicit CEnemyShipAI( CActorSprite2D * _pActor );

    // Destructor
    virtual ~CEnemyShipAI();

    // Check for collision and react to it
    virtual void ReactToCollision(){};

    // Update animations, Move sprites
    virtual void Update(){}

private:


};

#endif  // __enemy_ship_ai_h__


