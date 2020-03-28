/************************************************************************
*
*    PRODUCT:         Bot class
*
*    FILE NAME:       CBot.h
*
*    DESCRIPTION:     AI base class for game agents/bots 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CBOT_H_
#define _CBOT_H_

#include "CAnimMapObj.h"     // Animation specific mesh class
#include "CGraphList.h"      // Graph template class
#include "CPathNode.h"       // Node class
#include "CPlayer.h"		 // player class
#include "CMapMgr.h"         // map manager


/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CBot : public CAnimMapObj<mesh_type>
{
protected:

	enum EGraphDirection
    {
        EGD_FORWARD = 0,
        EGD_BACKWARDS
    };

	// The current node
	CPathNode * currentNode;
	// Next node
	CPathNode * nextNode;
	// X movement
	float moveX;
	// Z movement
	float moveZ; 
	// Move vector;
	float moveVector;
	float tmpRotation;
	// Graph direction
    EGraphDirection graphNodeDir;
    // Bot move direction
    EGraphDirection botNodeDir;
	// Player encounter
    bool playerEncounter;

    // Rotate to face the player
    void RotateToFacePlayer( CPlayer &player );
	// Determin the movement direction
	void SetMoveDir();
	// Select the next node to move to
    void ChooseNextRandomNode();
	// Switch the direction of the bot based on the player pos
	void ChangeBotDir( CPlayer &player );
	// Select the next to make your way to the players position
    void ChooseNextNodeInPlayerDir( CPlayer &player );
	// Move the bot to randomly walk around the map
    void RandomWalk( CGraphList <CPathNode> &graphList, CPlayer &player );
	// Chase the player
    void PursuePlayer( CGraphList <CPathNode> &graphList, CPlayer &player );
	// Choose the next node in the players direction
	void SameNodePlayerDirestion( CPlayer &player );
	// Select the next subnode in the players direction
	void ChooseNextSubnodeInPlayerDir( CPlayer &player );

	// Is the bot close to player
	virtual bool IsBotCloseToPlayer( CPlayer &player ) = 0;
	// abstact function for if random nodes should be picked
	virtual bool PickFromOtherNodes( float nodeKey ) = 0;
	// Render the animation frame
    virtual int Render( CMapMgr<CMeshMap> &map, CMapType &playerMapTyper, CMatrix &m );

public:

	CBot();
    virtual ~CBot();

	// Find the node the bot is standing at
	void FindPathNode( CGraphList <CPathNode> &graphList );
};


/************************************************************************
*    FUNCTION NAME:  Constructer                                                             
************************************************************************/
template <class mesh_type>
CBot<mesh_type>::CBot()
                : currentNode(NULL), nextNode(NULL), moveX(0.0), moveZ(0.0),
				  moveVector(0.0), graphNodeDir(EGD_FORWARD), botNodeDir(EGD_FORWARD),
				  playerEncounter(false)
{
	// Set the starting direction
	dir.z = 150.0f;
}


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
template <class mesh_type>
CBot<mesh_type>::~CBot()
{
}


/************************************************************************                                                            
*    DESCRIPTION:  Rotate to face the player
*
*    Input:  CPlayer &playerp - player class
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::RotateToFacePlayer( CPlayer &player )
{
	// Set the two 2D points
	CPoint2D bot(dir.x, dir.z);
	CPoint2D ply(player.GetPos().tx - pos.tx, player.GetPos().tz - pos.tz);

	bot.Normalize();
	ply.Normalize();

	float length = CPoint2D::GetLength( bot, ply );

    // clear the rotation matrix
	rotMatrix.InitilizeMatrix();

	if( length > 0.0 )
	{
		rotMatrix.Rotate( 0, -CPoint2D::GetAngle( bot, ply ), 0 );
	}
	else if( length < 0.0 )
	{
		rotMatrix.Rotate( 0, CPoint2D::GetAngle( bot, ply ), 0 );
	}

}	// RotateToFacePlayer


/************************************************************************                                                            
*    DESCRIPTION:  Find the node the bot is standing at
*
*    Input:  CGraphList <CPathNode> &graphList - graph class
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::FindPathNode( CGraphList <CPathNode> &graphList )
{
	// Transform the position and direction
	matrix.Transform( pos );
	rotMatrix.Transform( dir );

	currentNode = graphList.FindNodeByCollision( pos, radius );

    // position the bot right on the node
    if( currentNode != NULL )
	{
		matrix.SetX( currentNode->GetX() );
		matrix.SetZ( currentNode->GetZ() );
		matrix.Transform( pos );	
	}

}	// FindPathNode


/************************************************************************                                                            
*    DESCRIPTION:  Determin the movement direction
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::SetMoveDir()
{
	moveX = moveZ = 0.0;

	if( nextNode != NULL && currentNode != NULL )
	{
		rotMatrix.InitilizeMatrix();

		if( fabs(nextNode->GetX() - currentNode->GetX()) > 
		    fabs(nextNode->GetZ() - currentNode->GetZ()) )
		{
			if( (nextNode->GetX() - currentNode->GetX()) < 0 )
			{
				moveX = -moveVector;
				rotMatrix.Rotate( 0, -90, 0 );
			}
			else
			{
				moveX = moveVector;
				rotMatrix.Rotate( 0, 90, 0 );
			}
		}
		else
		{
			if( (nextNode->GetZ() - currentNode->GetZ()) < 0 )
			{
				moveZ = -moveVector;
				rotMatrix.Rotate( 0, 180, 0 );
			}
			else
			{
				moveZ = moveVector;
			}
		}
	}
}	// SetMoveDir


/************************************************************************                                                            
*    DESCRIPTION:  Move the bot to randomly walk around the map
*
*    Input:  CGraphList <CPathNode> &graphList - graph class
*            CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::RandomWalk( CGraphList <CPathNode> &graphList,
                                     CPlayer &player )
{
    if( nextNode != NULL &&
        CPoint::GetLengthXZDy( nextNode->GetPos(), pos ) <= (moveVector * moveVector) )
    {
        ChooseNextRandomNode();

        SetMoveDir();
    }

}   // RandomWalk


/************************************************************************                                                            
*    DESCRIPTION:  Select the next node to move to
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::ChooseNextRandomNode()
{
    if( nextNode != NULL )
    {
        // Reset the position of the bot
    	matrix.SetX( nextNode->GetX() );
    	matrix.SetZ( nextNode->GetZ() );

        // The current node is now the next node
        currentNode = nextNode;
    }

    if( currentNode != NULL )
    {
        if( botNodeDir == EGD_FORWARD )
        {
            if( graphNodeDir == EGD_FORWARD )
            {
                if( (currentNode->GetNextNodeCount() > 1) && 
                     PickFromOtherNodes( currentNode->GetKey() ) )
                    nextNode = currentNode->GetNextNode( (rand() % (currentNode->GetNextNodeCount()-1)) + 1 );
                else
                    nextNode = currentNode->GetNextNode();

                if( nextNode == NULL )
                {
                    nextNode = currentNode->GetPrevNode();
                    graphNodeDir = EGD_BACKWARDS;

                    if( currentNode->GetMainFlag() )
                        botNodeDir = EGD_BACKWARDS;
                }
            }
            else if( graphNodeDir == EGD_BACKWARDS )
            {
                if( currentNode->GetMainFlag() )
                {
                    nextNode = currentNode->GetNextNode();
                    graphNodeDir = EGD_FORWARD;
                }
                else
                {
                    nextNode = currentNode->GetPrevNode();
                }

                if( nextNode == NULL )
                {
                    nextNode = currentNode->GetNextNode();
                    graphNodeDir = EGD_FORWARD;
                }
            }
        }
        else if( botNodeDir == EGD_BACKWARDS )
        {
            if( graphNodeDir == EGD_BACKWARDS )
            {
                if( currentNode->GetMainFlag() &&
                    (currentNode->GetNextNodeCount() > 1) &&
                    PickFromOtherNodes( currentNode->GetKey() ) )
                {
                    nextNode = currentNode->GetNextNode( (rand() % (currentNode->GetNextNodeCount()-1)) + 1 );
                    graphNodeDir = EGD_FORWARD;
                }
                else
                {
                    nextNode = currentNode->GetPrevNode();
                }

                if( nextNode == NULL )
                {
                    nextNode = currentNode->GetNextNode();
                    graphNodeDir = EGD_FORWARD;

                    if( currentNode->GetMainFlag() )
                        botNodeDir = EGD_FORWARD;
                }
            }
            else if( graphNodeDir == EGD_FORWARD )
            {
                if( currentNode->GetNextNodeCount() > 1 )
                    nextNode = currentNode->GetNextNode( rand() % currentNode->GetNextNodeCount() );
                else
                    nextNode = currentNode->GetNextNode();

                if( nextNode == NULL )
                {
                    nextNode = currentNode->GetPrevNode();
                    graphNodeDir = EGD_BACKWARDS;
                }
            }
        }
    }

}   // ChooseNextRandomNode


/************************************************************************                                                            
*    DESCRIPTION:  Move the bot to randomly walk around the map
*
*    Input:  CGraphList <CPathNode> &graphList - graph class
*            CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::PursuePlayer( CGraphList <CPathNode> &graphList,
                                       CPlayer &player )
{
    if( IsBotCloseToPlayer( player ) )
    {
		// Need to stop the bot in it's tracks
		moveX = moveZ = 0.0;
		
		// Have the bot face the player
		RotateToFacePlayer( player );
	
		if( IsAnimFinish() )
		{	        			
			playerEncounter = true;
			SetAnimation( "attack", 10 );
		}
    }
	else if( nextNode == NULL )
    {
        ChooseNextNodeInPlayerDir( player );
        SetMoveDir();
    }
    else if( playerEncounter )
    {
        if( IsAnimFinish() )
		{
			if( CPoint::GetLengthXZDy( nextNode->GetPos(), pos ) <= (moveVector * moveVector) )
			{
				ChooseNextNodeInPlayerDir( player );
			}
			else
			{
				ChangeBotDir( player );
			}

	        SetMoveDir();
	        playerEncounter = false;
			SetAnimation( "walk", 10 );
		}
    }
	// same as sqrt( GetLengthXZDy ) <= moveVector
	else if( CPoint::GetLengthXZDy( nextNode->GetPos(), pos ) <= (moveVector * moveVector) )
    {
        ChooseNextNodeInPlayerDir( player );
        SetMoveDir();   
    }
	// safety net if the other conditions don't catch
	else if( moveX == 0.0 && moveZ == 0.0 )
	{
		SetMoveDir();
	}

}   // PursuePlayer


/************************************************************************                                                            
*    DESCRIPTION:  Switch the direction of the bot based on the player pos
*
*    Input:  CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::ChangeBotDir( CPlayer &player )
{
	CPoint2D bot(currentNode->GetPos().tx - pos.tx,
                  currentNode->GetPos().tz - pos.tz);
	CPoint2D ply(player.GetPos().tx - pos.tx,
                 player.GetPos().tz - pos.tz);

    if( CPoint2D::GetDotProduct( bot, ply ) > 0.0 )
    {                       
        CPathNode * tmpNode = currentNode;
		currentNode =  nextNode;
		nextNode = tmpNode;

		// Switch the directin which we are
		// working through the nodes
		if( graphNodeDir == EGD_FORWARD )
		{
			graphNodeDir = EGD_BACKWARDS;
		}
		else
		{
			graphNodeDir = EGD_FORWARD;
		}                     
    }

}	// ChangeBotDir


/************************************************************************                                                            
*    DESCRIPTION:  Select the next to make your way to the players position
*
*    Input:  CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::ChooseNextNodeInPlayerDir( CPlayer &player )
{   
    if( nextNode != NULL )
    {
        // Reset the position of the bot
    	matrix.SetX( nextNode->GetX() );
    	matrix.SetZ( nextNode->GetZ() );

        // The current node is now the next node
        currentNode = nextNode;
        nextNode = NULL;
    }

    // The fractional part is the sub nodes. All we care about here is the main nodes
    if( currentNode != NULL )
    {
		if( currentNode->GetMainFlag() && (int)currentNode->GetKey() < (int)player.GetNode()->GetKey() )
	    {
	        nextNode = currentNode->GetNextNode();
			graphNodeDir = EGD_FORWARD;  
	    }
	    else if( currentNode->GetMainFlag() && (int)currentNode->GetKey() > (int)player.GetNode()->GetKey() )
	    {
	        nextNode = currentNode->GetPrevNode();
			graphNodeDir = EGD_BACKWARDS;
	    }
	    else if( (int)currentNode->GetKey() == (int)player.GetNode()->GetKey() )
	    {
	        // If we are at the same node then we need to determine which
	        // next node is in the player's path
	        if( *currentNode == *player.GetNode() )
	        {
				SameNodePlayerDirestion( player );
	        }
			else  // Player has gown off on one of the side nodes
			{
				ChooseNextSubnodeInPlayerDir( player );
			}
		}
		else if( !currentNode->GetMainFlag() )
		{
			ChooseNextSubnodeInPlayerDir( player );
		}
    }

}   // ChooseNextNodeInPlayerDir


/************************************************************************                                                            
*    DESCRIPTION:  Select the next subnode in the players direction
*
*    Input:  CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::ChooseNextSubnodeInPlayerDir( CPlayer &player )
{
	// Make sure we are in the same sub node
	if( (int)currentNode->GetKey() == (int)player.GetNode()->GetKey() )
	{
		// choose which subnode we need to travel down
		if( currentNode->GetNextNodeCount() > 1 )
		{
			if( currentNode->GetKey() < player.GetNode()->GetKey() )
		    {
				for( unsigned int i = 0; i < currentNode->GetNextNodeCount(); ++i )
				{
					// Is the player node in-between the sub nodes
					if( player.GetNode()->GetKey() >= currentNode->GetNextNode(i)->GetKey() )
					{
						nextNode = currentNode->GetNextNode(i);
					}
				}

				graphNodeDir = EGD_FORWARD;  
		    }
		    else if( currentNode->GetKey() > player.GetNode()->GetKey() )
		    {
		        nextNode = currentNode->GetPrevNode();
				graphNodeDir = EGD_BACKWARDS;
		    }
		}
		else
		{
			if( currentNode->GetKey() < player.GetNode()->GetKey() )
		    {
		        nextNode = currentNode->GetNextNode();
				graphNodeDir = EGD_FORWARD;  
		    }
		    else if( currentNode->GetKey() > player.GetNode()->GetKey() )
		    {
		        nextNode = currentNode->GetPrevNode();
				graphNodeDir = EGD_BACKWARDS;
		    }
		}
	}
	// if we are not in the same sub node, just work our way out
	else
	{
		nextNode = currentNode->GetPrevNode();
		graphNodeDir = EGD_BACKWARDS;
	}

}	// ChooseNextSubnodeInPlayerDir


/************************************************************************                                                            
*    DESCRIPTION:  Choose the next node in the players direction
*
*    Input:  CPlayer &player - player object
************************************************************************/
template <class mesh_type>
void CBot<mesh_type>::SameNodePlayerDirestion( CPlayer &player )
{
	CPathNode * tmpNextNode;
    CPathNode * tmpPrevNode;

	tmpNextNode = currentNode->GetNextNode();
	tmpPrevNode = currentNode->GetPrevNode();

    if( tmpNextNode != NULL && tmpPrevNode != NULL )
    {
		CPoint2D node(currentNode->GetPos().tx - player.GetPos().tx,
                  	  currentNode->GetPos().tz - player.GetPos().tz);
		CPoint2D ply(player.GetPos().tx - tmpNextNode->GetPos().tx,
                 	 player.GetPos().tz - tmpNextNode->GetPos().tz);
        
        if( CPoint2D::GetDotProduct( node, ply ) > 0.0 )
        {                       
            nextNode = tmpNextNode;                     
        }
		else
		{
			nextNode = tmpPrevNode;
		}
    }
}	// SameNodePlayerDirestion


/************************************************************************                                                            
*    DESCRIPTION:  Render the animated frame
*
*    Input:  CMapMgr<CMeshMap> &map - map manager class
*            CMapType &playerMapType - use the player map type to determine
*                                      if the object should be rendered.   
*            CMatrix &m - move matrix
*
*    Output:   int - polygon count of the mesh rendered
************************************************************************/
template <class mesh_type>
int CBot<mesh_type>::Render( CMapMgr<CMeshMap> &map, CMapType &playerMapType, CMatrix &m )
{
    int result = 0;

    // If we have a map loaded, use it to determin if we are to render the animation.
    // If the area of this animation was NOT rendered, then it can't be seen
    // and we can jump out now. Very quick, simple check
    if( !map.WasRendered( mapType ) )
        return result;

	// Do the final view transformation
	DoViewTransformation( m );

	// Render only if it can be seen through the doorway. If the animation and the
    // view are in the same room, this function returns true.
    // Only check if animation is in a room. If it was in a doorway then the WasRendered
    // is the only check we need because if the doorway was rendered then the animation
    // should automatically be rendered too.
    if( map.IsVisableThruRenderedDoorways( mapType, playerMapType,
                                           pMesh[frameIndex].GetRadius(),
                                           pMesh[frameIndex].GetCenterPt() ) )
	{
		// HAve the parent class handle the render
    	result = CAnimMeshLst<mesh_type>::Render();
	}

    // inc the frame even if it wasn't rendered
    IncFrame();

    return result;

}   // Render


#endif  // _CBOT_H_
