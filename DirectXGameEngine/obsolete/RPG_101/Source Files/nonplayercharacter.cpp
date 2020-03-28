/************************************************************************
*    FILE NAME:       nonplayercharacter.cpp
*
*    DESCRIPTION:     Base class for characters not controlled by the
*					  player
************************************************************************/  

// Standard Library dependencies
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

// Game lib dependencies
#include "3d\\highresolutiontimer.h"
#include "3d\\animatedspritegroup.h"
#include "genfunc.h"

// Physical component dependency
#include "nonplayercharacter.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CNonPlayerCharacter::CNonPlayerCharacter()
     : direction(0.0f,0.0f,0.0f),
       destination(0.0f,0.0f,0.0f),
       movementSpeed(0.057f),
       rotationSpeed(0.0033f),
       rotationMultiplier(1.0f),
       rotationCounter(0.0f),
       wait_timer(0.0f),
       atDestination(true),
       wait(false)
{

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CNonPlayerCharacter::~CNonPlayerCharacter()
{
}   // Destructer


/************************************************************************
*    desc:  Set the destination of where the character will travel to
*
*	 param: CPoint dest - new destination
************************************************************************/
void CNonPlayerCharacter::SetDestination( CPoint & dest )
{
    destination = dest;

}	// SetDestination


/************************************************************************
*    desc:  Get the destination of where the character is traveling to
*
*	 ret:   CPoint destination - current destination
************************************************************************/
CPoint & CNonPlayerCharacter::GetDestination()
{
    return destination;

}	// GetDestination


/************************************************************************
*    desc:  Set the direction of the character travel
*
*	 param: CPoint dir - new direction
************************************************************************/
void CNonPlayerCharacter::SetDirection( CPoint & dir )
{
    direction = dir;

}	// SetDirection


/************************************************************************
*    desc:  Get the direction of the character travel
*
*	 ret:   CPoint direction - current direction
************************************************************************/
CPoint & CNonPlayerCharacter::GetDirection()
{
    return direction;

}	// GetDirection


/************************************************************************
*    desc:  Set the speed of the character travel
*
*	 param: float speed - new speed value
************************************************************************/
void CNonPlayerCharacter::SetMovementSpeed( float speed )
{
    movementSpeed = speed;

}	// SetMovementSpeed


/************************************************************************
*    desc:  Get the speed of the character travel
*
*	 ret:   float speed - current speed value
************************************************************************/
float CNonPlayerCharacter::GetMovementSpeed()
{
    return movementSpeed;

}	// GetMovementSpeed


/************************************************************************
*    desc:  Set the speed of the character rotation in degrees
*
*	 param: float speed - new speed value
************************************************************************/
void CNonPlayerCharacter::SetRotationSpeed( float speed )
{
    rotationSpeed = speed;

}	// SetRotationSpeed


/************************************************************************
*    desc:  Get the speed of the character rotation in degrees
*
*	 ret:   float speed - current speed value
************************************************************************/
float CNonPlayerCharacter::GetRotationSpeed()
{
    return rotationSpeed;

}	// GetRotationSpeed


/************************************************************************
*    desc:  Set the original position
*
*	 param: CPoint pos - new original position
************************************************************************/
void CNonPlayerCharacter::SetOriginalPos( CPoint & pos )
{
    originalPos = pos;

}	// SetOriginalPos


/************************************************************************
*    desc:  Get the original position
*
*	 ret:   CPoint originalPos - original position
************************************************************************/
CPoint & CNonPlayerCharacter::GetOriginalPos()
{
    return originalPos;

}	// GetOriginalPos


/************************************************************************
*    desc:  Causes NPC to wander around within a bounding box
*
*	 param: float boxRadius - bounding box radius for the wander
************************************************************************/
void CNonPlayerCharacter::Wander(float boxRadius)
{
    // Setup temporary variables
    float x, z;
    int xDir, zDir;

    // If the NPC is at its destination, then it needs to determine another destination
    if( atDestination )
    {
        rotationMultiplier = 1.0f;
        rotationCounter = 0.f;

        // Determine a percentage between 0 and 1 to apply to the bounding box radius
        x = (float)rand() / RAND_MAX;
        z = (float)rand() / RAND_MAX;
    
        // Determine the direction (or sign) of where the NPC is going relative to its
        // original position
        xDir = rand() % 100;
        zDir = rand() % 100;
    
        // Determine the NPC's destination relative to it's original position
        if( xDir < 50 )
        {
            x = x * -boxRadius;
        }
        else
        {
            x = x * boxRadius;
        }

        if( zDir < 50 )
        {
            z = z * -boxRadius;
        }
        else
        {
            z = z * boxRadius;
        }

        // Determine the NPC's destination relative to the world
        destination.x = originalPos.x + x;
        destination.z = -(originalPos.z + z);

        // Now that the NPC has a new destination, it is not currently "atDestination"
        atDestination = false;
    }

    if( !wait )
    {
        // Determine the direction the NPC will go
        DetermineDirection();

        // Move the character in the determined direction
        Move();

        // A temporary point is made to avoid calling pCharacter->GetVisualSprite().GetPos() a million times
        CPoint charPos; 
        charPos = pCharacter->GetVisualSprite().GetPos();

        // Check and see if the character has come within 1.f of it's destination. Once it hits this point
        // then it has been considered to "reach it's destination"
    
        if( charPos.x > (destination.x - 1.f) && charPos.x < (destination.x + 1.f) &&
            charPos.z > (destination.z - 1.f) && charPos.z < (destination.z + 1.f) )
        {
            wait = true;
            pCharacter->SetAnimation( string("idle") );
        }
        else if( abs(rotationCounter) > (2 * PI) )
        {
            wait = true;
            pCharacter->SetAnimation( string("idle") );
        }
    }
    else
    {
        wait_timer += CHighResTimer::Instance().GetElapsedTime();

        if( wait_timer > 3000 )
        {
            wait = false;
            atDestination = true;
            wait_timer = 0.0f;
            pCharacter->SetAnimation( string("run") );
        }
    }
    // To prevent the character just moving in one direction forever if it accidentally lags past its destination,
    // a check is made to see if the character has left the bounding wander box. If so, then it is considered to
    /*/ "reach it's destination"
    else if(charPos.x > (originalPos.x + boxRadius) || charPos.x < (originalPos.x - boxRadius) ||
            charPos.z > (originalPos.z + boxRadius) || charPos.z < (originalPos.z - boxRadius) )
    {
        atDestination = true;
    }*/


}	// Wander


/************************************************************************
*    desc:  Moves the NPC in the direction of the
************************************************************************/
void CNonPlayerCharacter::Move()
{
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    pCharacter->IncPos( direction.x * elapsedTime, 
                        direction.y * elapsedTime, 
                        direction.z * elapsedTime );
    
}	// Move


/************************************************************************
*    desc:  Determines the direction of the NPC from the destination
************************************************************************/
void CNonPlayerCharacter::DetermineDirection()
{
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    // Setup temporary variables
    float x(0), z(0), theta(0), angleFacing(0), angleDifference(0), tmp(0);

    // Find the large triangle x and z
    x = destination.x - pCharacter->GetVisualSprite().GetPos().x;
    z = destination.z - pCharacter->GetVisualSprite().GetPos().z;
    
    // Find the angle from the character to the destination
    theta = atan( z / x );

    // Due to the nature of arctan() we must check the sign for x and add PI to
    // theta if x is negative
    if( x < 0 )
    {
        theta += PI;
    }
    if( theta < 0 )
    {
        theta = (2 * PI) + theta;
    }

    // Convert to the standard unit circle value
    angleFacing = -( pCharacter->GetVisualSprite().GetPreTransRot().y + 90 );

    // Use the angle's positive equivalent
    if( angleFacing < 0 )
    {
        angleFacing = 360.f + angleFacing;
    }

    // Convert to radians
    angleFacing = (angleFacing * PI) / 180.f;

    // Find the difference. This is so we can figure out which direction the NPC will turn
    // and make the least effort to get to the destination.
    angleDifference = theta - angleFacing;

    // Since this multiplication is used several times, we do it once and use it elsewhere
    tmp = rotationSpeed * elapsedTime * rotationMultiplier;

    // Rotate in the direction of the destination
    if( angleDifference < (tmp) && angleDifference > -(tmp) )
    {
        angleFacing = theta;
        rotationMultiplier = 1.f;
    }
    else if( abs(angleDifference) < PI )
    {
        if( angleDifference > 0 )
        {
            angleFacing = angleFacing + tmp;
            rotationCounter += tmp;
        }
        else
        {
            angleFacing = angleFacing - tmp;
            rotationCounter -= tmp;
        }
    }
    else
    {
        if( angleDifference < 0 )
        {
            angleFacing = angleFacing + tmp;
            rotationCounter += tmp;
        }
        else
        {
            angleFacing = angleFacing - tmp;
            rotationCounter -= tmp;
        }
    }
    
    // Increase the rotation multiplier so that the character
    //rotationMultiplier += 0.005f;

    // Convert to degrees
    theta = (angleFacing * 180.f) / PI;

    theta = -theta - 90;

    if( theta < 0 )
    {
        theta = 360 + theta;
    }

    // Rotate the character to the direction of movement
    pCharacter->SetPreTransRot( 0, theta, 0 );

    // Set the direction of movement
    direction.x = movementSpeed * cos( angleFacing );
    direction.z = movementSpeed * sin( angleFacing );

}	// DetermineDirection