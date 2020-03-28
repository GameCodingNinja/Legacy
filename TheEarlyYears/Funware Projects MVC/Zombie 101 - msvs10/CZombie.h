/************************************************************************
*
*    PRODUCT:         Zombie class
*
*    FILE NAME:       CZombie.h
*
*    DESCRIPTION:     AI class for zombies 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CZOMBIE_H_
#define _CZOMBIE_H_

#include "3D/CBot.h"       // Animation specific mesh class


/************************************************************************
*
*
************************************************************************/

template <class mesh_type>
class CZombie : public CBot<mesh_type>
{
public:

    enum EZombieState
    {
        EZS_IDLE,
        EZS_RANDOM_WALK,
        EZS_PERSUE_PLAYER,
    };

protected:

    // AI state
    EZombieState aiState;
	// last randomized node key
	float lastRandNodeKey;

    // Update the AI
    void UpdateAI( CGraphList <CPathNode> &graphList, CPlayer &player );
	// Is the bot close to player
	virtual bool IsBotCloseToPlayer( CPlayer &player );
	// abstact function for if random nodes should be picked
	virtual bool PickFromOtherNodes( float nodeKey  );

public:

    CZombie();
    virtual ~CZombie();

    // Update the bot
    void Update( CMapMgr<CMeshMap> &map, CGraphList <CPathNode> &graphList, CPlayer &player );
    // Set the ai state
    void SetAIState( EZombieState state, CPlayer &player );
};


/************************************************************************
*    FUNCTION NAME:  Constructer                                                             
************************************************************************/
template <class mesh_type>
CZombie<mesh_type>::CZombie() :
                    aiState(EZS_IDLE), lastRandNodeKey(0.0)
{

}


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
template <class mesh_type>
CZombie<mesh_type>::~CZombie()
{
}


/************************************************************************                                                            
*    DESCRIPTION:  Update the bot
*
*    Input:  CMapMgr<CMeshMap> &map - map class
*            CGraphList <CPathNode> &graphList - graph class
*            CPlayer &player - player class
************************************************************************/
template <class mesh_type>
void CZombie<mesh_type>::Update( CMapMgr<CMeshMap> &map,
                                 CGraphList <CPathNode> &graphList,
                                 CPlayer &player )
{
    // no point in doing the collision detection if the bot 
    // is not seen and not moving
    if( moveVector > 0.0 || map.WasRendered( mapType ) )
    {
        // Move the bot
        matrix.Translate( moveX, 0, moveZ);

        // Transform the position and direction
        matrix.Transform( pos );
        rotMatrix.Transform( dir );

        // React to the floor which resets the y height of the bot.
        map.BotFloorCollision( matrix, mapType, pos, radius );

        // Combines the matrix and rotation matrix
        DoWorldTransformation();

        // Push the bot up from the floor
        matrixMrg.Translate( 0, floorOffset, 0 );
    }
    
    // Update the bots Ai
    UpdateAI( graphList, player );    

}   // Update


/************************************************************************                                                            
*    DESCRIPTION:  Update the AI
*
*    Input:  CGraphList <CPathNode> &graphList - graph class
*            CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CZombie<mesh_type>::UpdateAI( CGraphList <CPathNode> &graphList,
                                   CPlayer &player )
{
    switch( aiState )
    {
        case EZS_IDLE:
        {
            break;
        }

        case EZS_RANDOM_WALK:
        {
            RandomWalk( graphList, player );
            break;
        }

        case EZS_PERSUE_PLAYER:
        {
            PursuePlayer( graphList, player );
            break;
        }
    }

}   // UpdateAI


/************************************************************************                                                            
*    DESCRIPTION:  Set the AI state
*
*    Input:  EZombieState state - The AI state
************************************************************************/
template <class mesh_type>
void CZombie<mesh_type>::SetAIState( EZombieState state, CPlayer &player )
{
    aiState = state;

    switch( state )
    {
        case EZS_IDLE:
        {
            moveVector = 0;
			SetAnimation( "idle", 30 );

            break;
        }

        case EZS_RANDOM_WALK:
        {
            moveVector = 0.5;

            SetAnimation( "walk", 20 );

            if( nextNode == NULL )
            {
                // Choose the node to walk to
                ChooseNextRandomNode();

                // Depending on the node, set the move dir, speed
                // and have the bot face the direction it is moving
                SetMoveDir();
            }

            break;
        }

        case EZS_PERSUE_PLAYER:
        {
            moveVector = 1;

            SetAnimation( "walk", 10 );

            if( nextNode == NULL )
            {
                // Choose the next node in the player's direction
                ChooseNextNodeInPlayerDir( player );

                // Depending on the node, set the move dir, speed
                // and have the bot face the direction it is moving
                SetMoveDir();
            }

            break;
        }
    }
}   // SetAIState


/************************************************************************                                                            
*    DESCRIPTION:  is the bot close to the player
*
*    Input:  CPlayer &player - player object
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CZombie<mesh_type>::IsBotCloseToPlayer( CPlayer &player )
{
	// same as sqrt( GetLengthXZDy() ) < 100.0
	return (player.GetPos().GetLengthXZDy( pos ) < 10000.0);
}


/************************************************************************                                                            
*    DESCRIPTION:  function for if random nodes should be picked
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CZombie<mesh_type>::PickFromOtherNodes( float nodeKey )
{
	bool result = false;

	if( lastRandNodeKey != nodeKey )
	{
		result = ((rand() % 5) == 0);
		lastRandNodeKey = nodeKey;
	}

	return result;
}

#endif  // _CZOMBIE_H_
