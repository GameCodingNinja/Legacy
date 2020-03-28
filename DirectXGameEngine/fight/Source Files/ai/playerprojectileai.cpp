
/************************************************************************
*    FILE NAME:       playerprojectileai.cpp
*
*    DESCRIPTION:     Class player projectile AI
************************************************************************/

// Physical component dependency
#include "playerprojectileai.h"

// Standard lib dependencies
#include <utilities/highresolutiontimer.h>

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <2d/spritegroup2d.h>
#include <gui/uimenutree.h>
#include <xact/xact.h>
#include <common/matrix.h>

// Speed of the projectile
const float PROJECTILE_SPEED = 2.6f;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CPlayerProjectileAI::CPlayerProjectileAI( CActorSprite2D * _pActor )
                   : iAIBase(_pActor),
                     firstUpdate(true)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CPlayerProjectileAI::~CPlayerProjectileAI()
{
}	// destructer


/************************************************************************
*    desc:  Initialize the projectile
*
*	 param:	CPoint & shooterVel - the velocity of whatever fired the projectile
*							      is added to the projectile's velocity
************************************************************************/
void CPlayerProjectileAI::Init( CPoint & shooterVel )
{
    // Get the sprite group
    initialPos = pActor->GetPos();

    // Set the velocity of the projectile
    float rotation = pActor->GetRot().z * DEG_TO_RAD;
    velocity.x = cos( rotation );
    velocity.y = sin( rotation );
    velocity *= PROJECTILE_SPEED;
    velocity += shooterVel;

    // Play the laser01 sound effect
    CXAct::Instance().Play( "laser01" );

}	// Init


/************************************************************************
*    desc:  Update the projectile                                                           
************************************************************************/
void CPlayerProjectileAI::Update()
{
    // If it's the projectile's first update, we don't want to increment 
    // it's position by the velocity 
    if( !firstUpdate )
        pActor->IncPos( velocity * CHighResTimer::Instance().GetElapsedTime() );
    else
        firstUpdate = false;

    // Get the difference between the projectile's initial position and where it currently
    // is. If it's more than 2 sectors away from its starting point, the projectile dies
    CWorldPoint difference = pActor->GetPos() - initialPos;

    if( abs(difference.x.i) > 3 ||
        abs(difference.y.i) > 3 )
        Invalidate();

}	// Update
