
/************************************************************************
*    FILE NAME:       playershipai.cpp
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

// Physical component dependency
#include "playershipai.h"

// Standard lib dependencies
#include <string>
#include <math.h>

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <2d/spritegroup2d.h>
#include <2d/collisionsprite2d.h>
#include <2d/physicsworld.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <controller/gamecontroller.h>
#include <managers/actormanager.h>
#include <xact/xact.h>

// Game dependencies
#include "playerprojectileai.h"

// The min/max speed of the ship
const float MAX_VELOCITY = 60.f;

// The acceleration of the ship
const float ACCELERATION = 300.f;

// The min/max speed of the ship's rotation
const float ANG_SPEED = 2.f;
const float MAX_ANGULAR_VELOCITY = 0.7f * ANG_SPEED;
const float MIN_ANGULAR_VELOCITY = 0.1f;

// The angular acceleration of the ship
const float ANGULAR_ACCELERATION = 0.005f * ANG_SPEED;

// How far you need to move a gamepad stick to move at max velocity
const int MAX_VELOCITY_THRESHOLD = 8000;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CPlayerShipAI::CPlayerShipAI( CActorSprite2D * _pActor )
             : iAIBase(_pActor),
               playerShootTimer(200, true),
               elapsedTime(0),
               angularVelocity(0),
               angularAcceleration(0),
               velRatio(1),
               pGun( pActor->GetSpriteGroup( "gun" ) )
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CPlayerShipAI::~CPlayerShipAI()
{
}	// destructer


/************************************************************************
*    desc:  Update                                                             
************************************************************************/
void CPlayerShipAI::HandleGameInput()
{
    // get the elapsed time
    elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    // Reset the booleans
    accelerating = false;
    rotating = false;
    shooting = false;

    // If we're using mouse and keyboard controls, we go into this function to handle the input
    if( CGameController::Instance().GetLastDevicedUsed() == NDevice::MOUSE || 
        CGameController::Instance().GetLastDevicedUsed() == NDevice::KEYBOARD )
        HandleMouseKeyboardControls();
    
    // If we're using joypad controls, we go into this function to handle the input
    else if( CGameController::Instance().GetLastDevicedUsed() == NDevice::JOYPAD )
        HandleJoypadControls();

    // Handle the ship movement
    HandleAcceleration();

    // Handle the ship rotation
    HandleRotation();

    // Handle the ship shooting
    HandleShooting();

}	// HandleGameInput


/************************************************************************
*    desc:  Handle the input if we're using mouse controls                                                             
************************************************************************/
void CPlayerShipAI::HandleMouseKeyboardControls()
{
    // Determine if we're moving
    if( CGameController::Instance().WasAction("Up")   == NDevice::EAP_HOLD &&
        CGameController::Instance().WasAction("Left") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint( -M_SQRT1_2, M_SQRT1_2);
        accelerating = rotating = true;
        rotDestination = 135.f;
    }
    else if( CGameController::Instance().WasAction("Up")    == NDevice::EAP_HOLD &&
             CGameController::Instance().WasAction("Right") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint( M_SQRT1_2, M_SQRT1_2);
        accelerating = rotating = true;
        rotDestination = 45.f;
    }
    else if( CGameController::Instance().WasAction("Down") == NDevice::EAP_HOLD &&
             CGameController::Instance().WasAction("Left") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint( -M_SQRT1_2, -M_SQRT1_2);
        accelerating = rotating = true;
        rotDestination = 225.f;
    }
    else if( CGameController::Instance().WasAction("Down")  == NDevice::EAP_HOLD &&
             CGameController::Instance().WasAction("Right") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint( M_SQRT1_2, -M_SQRT1_2);
        accelerating = rotating = true;
        rotDestination = 315.f;
    }
    else if( CGameController::Instance().WasAction("Up") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint(0,1);
        accelerating = rotating = true;
        rotDestination = 90.f;
    }
    else if( CGameController::Instance().WasAction("Down") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint(0,-1);
        accelerating = rotating = true;
        rotDestination = 270.f;
    }
    else if( CGameController::Instance().WasAction("Left") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint(-1,0);
        accelerating = rotating = true;
        rotDestination = 180.f;
    }
    else if( CGameController::Instance().WasAction("Right") == NDevice::EAP_HOLD )
    {
        accelDir = CPoint(1,0);
        accelerating = rotating = true;
        rotDestination = 0.f;
    }

    // Set the velocity ratio to 1, because keyboard movement isn't analog
    velRatio = 1.f;

    // Determine if a player wants to shoot a projectile
    shooting = ( CGameController::Instance().WasAction("Shoot") == NDevice::EAP_DOWN || 
                 CGameController::Instance().WasAction("Shoot") == NDevice::EAP_HOLD );

    // Calculate the rotation of the ship's gun
    CPoint tmpMousePos = CGameController::Instance().GetAbsolutePosScaled();
    tmpMousePos -= pActor->GetPos();
    gunRotation = atan2( tmpMousePos.y, tmpMousePos.x ) * 180.f / M_PI;

}	// HandleMouseKeyboardControls


/************************************************************************
*    desc:  Handle the input if we're using joypad controls                                                             
************************************************************************/
void CPlayerShipAI::HandleJoypadControls()
{
    // Determine if we are rotating
    const CDeviceMovement &devMovement = CGameController::Instance().GetMovement();

    // If we're accelerating, we want to calculate the velocity ratio. This is so that if
    // the player slightly tilts the stick, he'll move slower
    if( accelerating = ( abs(devMovement.gamepad1Y) > 0 || abs(devMovement.gamepad1X) > 0 ) )
    {
        // Get the magnitude of the analog stick vector
        float analogMag = sqrt( (float)( devMovement.gamepad1Y * devMovement.gamepad1Y + 
                                         devMovement.gamepad1X * devMovement.gamepad1X ) );

        if( analogMag >= MAX_VELOCITY_THRESHOLD )
            velRatio = 1.f;
        else
            velRatio = analogMag / (float)MAX_VELOCITY_THRESHOLD;

        // Set the acceleration direction
        accelDir.x =  (float)devMovement.gamepad1X / analogMag;
        accelDir.y = -(float)devMovement.gamepad1Y / analogMag;

        // Calculate the rotation value we're rotating towards
        rotating = true;
        rotDestination = atan2( (float)devMovement.gamepad1Y, (float)devMovement.gamepad1X );
        rotDestination = (rotDestination * -180.f) / M_PI;

        // Restrict the rotation to a positive number between 0 and 360
        if( rotDestination < 0 )
            rotDestination += 360.f;
        else if( rotDestination > 360 )
            rotDestination -= 360.f;
    }

    // Calculate the rotation of the gun
    if( abs(devMovement.gamepad2Y) > 0 || abs(devMovement.gamepad2X) > 0 )
        gunRotation = atan2( (float)devMovement.gamepad2Y, (float)devMovement.gamepad2X ) * -180.f / M_PI;

    // Determine if a player wants to shoot a projectile
    shooting = ( CGameController::Instance().WasAction("Shoot") == NDevice::EAP_DOWN || 
                 CGameController::Instance().WasAction("Shoot") == NDevice::EAP_HOLD );

}	// HandleJoypadControls


/************************************************************************
*    desc:  Handle the ship's acceleration
************************************************************************/
void CPlayerShipAI::HandleAcceleration()
{
    // If the player isn't accelerating or has passed the max velocity, we leave
    if( !accelerating || 
         pActor->GetCollisionSprite()->GetVelocityMag() >= (MAX_VELOCITY * velRatio) ||
        !pActor->GetCollisionSprite()->GetWorld()->BeginStep() )
        return;

    // Incorporate the acceleration into the velocity
    CPoint accelVector = accelDir * ACCELERATION;

    // Apply the acceleration
    pActor->GetCollisionSprite()->ApplyAcceleration( accelVector );

}	// HandleMovement


/************************************************************************
*    desc:  Handle the ship's rotation 
************************************************************************/
void CPlayerShipAI::HandleRotation()
{
    // If the ship is turning, we handle it here
    if( rotating )
    {
        float angAccelDir, rotDiff, rotDiffA, rotDiffB;

        // Get the difference between the ship's current rotation and where it wants to be
        rotDiffA = rotDestination - pActor->GetRot().z;

        // Set the rotDiffB to have the opposite sign of rotDiffA, but have the same rotation position
        if( rotDiffA > 0 )
            rotDiffB = rotDiffA - 360.f;
        else
            rotDiffB = rotDiffA + 360.f;

        // Find the shortest direction to rotate in
        if( abs(rotDiffA) < abs(rotDiffB) )
            rotDiff = rotDiffA;
        else
            rotDiff = rotDiffB;
        
        if( rotDiff > 0 )
            angAccelDir = 1;
        else
            angAccelDir = -1;
        
        // Calculate the angularVelocity
        angularVelocity = angularVelocity + (angAccelDir * ANGULAR_ACCELERATION * elapsedTime);

        // We calculate the peak velocity. The peak velocity will deccelerate us if we're nearing the angle we want
        float peakVelocity = sqrt( 2 * ANGULAR_ACCELERATION * abs(rotDiff) ) * 0.5f;

        if( (angularVelocity > 0 && angularVelocity > peakVelocity) ||
            (angularVelocity < 0 && angularVelocity < -peakVelocity) )
            angularVelocity = peakVelocity * angAccelDir;

        // We don't want to accelerate past our maximum velocity
        if( abs(angularVelocity) > MAX_ANGULAR_VELOCITY )
            angularVelocity = MAX_ANGULAR_VELOCITY * angAccelDir;

        if( abs( rotDiff ) < abs(angularVelocity * elapsedTime) )
        {
            pActor->SetRot( CPoint(0,0,rotDestination) );
            angularVelocity = 0;
        }
        else
            pActor->IncRot( CPoint(0,0,angularVelocity * velRatio * elapsedTime) );
    }

}	// HandleRotation


/************************************************************************
*    desc:  Handle the ship's shooting 
************************************************************************/
void CPlayerShipAI::HandleShooting()
{
    // Rotate the gun
    pGun->SetRot( CPoint(0,0,gunRotation - pActor->GetRot().z) );

    // If the player hits the shoot button, and our time has expired, create a projectile actor
    if( playerShootTimer.Expired() && shooting )
    {
        // Set the projectile's offset
        CPoint projectileOffset;
        float tmpRot = gunRotation * DEG_TO_RAD;
        projectileOffset.x = 30 * cos( tmpRot );
        projectileOffset.y = 30 * sin( tmpRot );
        projectileOffset.z = 0;
        
        // Create the projectile actor
        CActorSprite2D * pProjectile = CActorManager::Instance().CreateActorPtr2D(
            "player_projectile", pActor->GetPos() + projectileOffset, CPoint(0,0,gunRotation) );

        // Add the projectile actor to the actor manager
        CActorManager::Instance().AddActorToVec2D( "player_projectiles", pProjectile );

        // Get the projectile's AI and initialize it with the ship's velocity
        CPlayerProjectileAI * pBulletAI = NGenFunc::DynCast<CPlayerProjectileAI*>( pProjectile->GetAIPtr() );
        pBulletAI->Init( CPoint() );

        playerShootTimer.Reset();
    }

}	// HandleShooting