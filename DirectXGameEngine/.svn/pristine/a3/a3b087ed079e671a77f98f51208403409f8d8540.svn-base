/************************************************************************
*    FILE NAME:       nonplayercharacter.h
*
*    DESCRIPTION:     Base class for characters not controlled by the
*					  player
************************************************************************/  

#ifndef __nonplayercharacter_h__
#define __nonplayercharacter_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "3d\\point.h"
#include "character.h"

class CAnimatedSpriteGroup;

class CNonPlayerCharacter : public CCharacter
{
public:

    CNonPlayerCharacter();
    virtual ~CNonPlayerCharacter();

    // Update the character's actions
    virtual void Update() = 0;

    // Calculate destinations and actions the character will perform
    virtual void Calculate() = 0;

    // Move the character in the direction of a target
    virtual void Move();
    //virtual void MoveToDestination();

    virtual void Wander(float boxRadius);

    virtual void DetermineDirection();

    // Set/Get the destination point
    void SetDestination(CPoint & dest);
    CPoint & GetDestination();

    // Set/Get the direction vector
    void SetDirection(CPoint & dir);
    CPoint & GetDirection();

    // Set/Get the movementSpeed
    void SetMovementSpeed(float speed);
    float GetMovementSpeed();

    // Set/Get the rotationSpeed
    void SetRotationSpeed(float speed);
    float GetRotationSpeed();

    // Set/Get the original position
    void SetOriginalPos(CPoint & pos);
    CPoint & GetOriginalPos();

protected:

    CPoint direction;
    CPoint destination;
    CPoint originalPos;

    float angleFacing;
    float destinationAngle;

    bool atDestination;
    bool wait;

    float movementSpeed;
    float rotationSpeed;
    float rotationMultiplier;
    float rotationCounter;

    float wait_timer;

};

#endif  // __nonplayercharacter_h__