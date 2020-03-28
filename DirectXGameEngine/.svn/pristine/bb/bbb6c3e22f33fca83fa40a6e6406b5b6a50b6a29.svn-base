/************************************************************************
*    FILE NAME:       animatedspritelist.h
*
*    DESCRIPTION:     Stage class
************************************************************************/

#ifndef __animatedspritelist_h__
#define __animatedspritelist_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// Game lib dependencies
#include "point.h"
#include "spritegroup.h"
#include "animatedspritegroup.h"
//#include "stage.h"
#include "collisioncheck.h"

class CActor
{
public:

    CActor();
    virtual ~CActor();

    virtual bool LoadFromXML(std::string filePath);
    
    // Inc the animation timer
    void IncAnimationTime();

    // Sets a vector of actors to the current actor vector, overwriting the previous vector
    virtual void SetActorVector( std::vector<CGroup *> & actorVector );

    // Adds a vector of actors to the current actor vector
    virtual void AddActorVector( std::vector<CGroup *> & actorVector );

    // Send all pointers to their rightful actor vectors
    virtual void SortActorVectors();

    // Clear all actor vectors
    virtual void ClearActorVectors();

    // Renders the actors
    virtual void Render( CMatrix & matrix );

    // Renders the actor's shadow sprites
    virtual void RenderShadow( CMatrix & matrix );

    // Renders the sprites that have the possibility of shadows being cast on them
    virtual void RenderShadowable( CMatrix & matrix );

    // Reacts to collision on an actor basis
    virtual void ReactToStageCollision( std::vector<CGroup *> & colObj, CCollisionCheck & cCheck );

    // Reacts to collision against the player on an actor basis
    virtual void ReactToPlayerCollision( CMatrix & matrix, CCollisionCheck & cCheck, float radius, float fpad, float wpad );

    // Allow objects suseptible to gravity to fall
    virtual void ReactToGravity( float speed, float elapsedTime );

    // Map of all object groups
    std::map<std::string, CGroup *> allGroupsMap;

    // Map of all object groups
    //std::map<std::string, CObjectInfo *> allObjectInfo;

private:

    // Vector containing all objects
    std::vector<CGroup *> allGroupsVector;

    // Vector containing all objects will an outline
    std::vector<CGroup *> otherVector;
    // Vector containing objects with collision against the player
    std::vector<CGroup *> colPlayerVector;
    // Vector containing objects with collision against other objects
    std::vector<CGroup *> colObjVector;
    // Vector containing all objects that colide with something
    std::vector<CGroup *> allColVector;
    // Vector containing objects that will have shadows
    std::vector<CGroup *> shadowVector;

};
#endif  // __animatedspritelist_h__