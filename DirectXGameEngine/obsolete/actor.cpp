/************************************************************************
*    FILE NAME:       animatedspritelist.cpp
*
*    DESCRIPTION:     Class to load joint animated sprites
************************************************************************/

// Physical component dependency
#include "actor.h"

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
#include "xwindow.h"
#include "misc\\genfunc.h"
#include "misc\\shader.h"
#include "misc\\deletefuncs.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CActor::CActor()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CActor::~CActor()
{
    DeleteMapPointers( allObjectInfo );
    DeleteVectorPointers( allGroupsVector );

}   // Destructer


/************************************************************************
*    desc:  load the meshs from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
bool CActor::LoadFromXML( string filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "actors" );
    XMLNode objectListNode = mainNode.getChildNode("objectList");

    for( int i = 0; i < objectListNode.nChildNode(); i++ )
    {
        XMLNode objectNode = objectListNode.getChildNode( "object", i);
        
        //New up the actor's information
        CObjectInfo * pTmpInfo = new CObjectInfo();

        CGroup * pTmpGroup;

        string objName = objectNode.getAttribute( "name" );
        string objAnimated = objectNode.getAttribute( "animated" );
        string objShading = objectNode.getAttribute( "shadingStyle" );

        bool animated = false;

        if( objAnimated == "true" )
        {
            animated = true;
        }

        // News up a temporary group
        if( animated == true )
        {
            pTmpGroup = new CAnimatedSpriteGroup();
        }
        else
        {
            pTmpGroup = new CSpriteGroup();
        }

        //pTmpInfo->shaderDisplay = objShading;

        /***********************************************************
        *	fileNode
        *	Gets the file paths for the sprites in the group
        ***********************************************************/
        XMLNode fileNode = objectNode.getChildNode( "file" );

        string objVisual = fileNode.getAttribute( "visual" );
        string objCollision = fileNode.getAttribute( "collision" );
        string objShadow = fileNode.getAttribute( "shadow" );
        string objAlpha = fileNode.getAttribute( "alpha" );
        string objOther = fileNode.getAttribute( "other" );
        
        pTmpInfo->visual_filePath = objVisual;
        pTmpInfo->other_filePath = objOther;
        pTmpInfo->collision_filePath = objCollision;
        pTmpInfo->alpha_filePath = objAlpha;


        /***********************************************************
        *	collideNode & colideWithNode
        *	colideWithNode is the childNode of collideNode
        *   contains the information used for the CCollisionStats object
        ***********************************************************/
        if( objCollision != "" )
        {
            XMLNode collideNode = objectNode.getChildNode( "collideWith" );
            string colPlayer = collideNode.getAttribute( "player" );
            string colObj = collideNode.getAttribute( "object" );
            float fpad = atof( collideNode.getAttribute( "fpad" ) );
            float wpad = atof( collideNode.getAttribute( "wpad" ) );

            if( colPlayer == "true")
            {
                pTmpInfo->collidePlayer = true;
            }
            if( colObj == "true")
            {
                pTmpInfo->collideObj = true;
            }

            pTmpInfo->collisionStats.SetFloorPad( fpad );
            pTmpInfo->collisionStats.SetWallPad( wpad );
        }


        /***********************************************************
        *	otherNode
        *	Decides whether or not an object will be culled or fall
        *	with gravity
        ***********************************************************/            
        XMLNode otherNode = objectNode.getChildNode( "otherProperties" );
        string objGravity = otherNode.getAttribute( "gravity" );
        string objShadowable = otherNode.getAttribute( "shadowable" );
        float objShadowCastDist = atof( otherNode.getAttribute( "shadowCastDist" ) );
        string objDefaultAnim = otherNode.getAttribute( "defaultAnimation" );

        if( objGravity == "true" )
        {
            pTmpInfo->gravity = true;
        }
        if( objDefaultAnim != "" )
        {
            pTmpInfo->defaultAnimation = objDefaultAnim;
            pTmpGroup->SetAnimation( objDefaultAnim );
        }


        /***********************************************************
        *	positionNode
        *	Has the position of the sprite group
        ***********************************************************/
        XMLNode positionNode = objectNode.getChildNode( "position" );
        CPoint pos;
        pos.x = atof( positionNode.getAttribute( "x" ) );
        pos.y = atof( positionNode.getAttribute( "y" ) );
        pos.z = atof( positionNode.getAttribute( "z" ) );


        /***********************************************************
        *	preTransRotNode
        *	Has the pre-translation rotation of the sprite group
        ***********************************************************/
        XMLNode preTransRotNode = objectNode.getChildNode( "preTransRot" );
        CPoint preTransRot;
        preTransRot.x = atof( preTransRotNode.getAttribute( "x" ) );
        preTransRot.y = atof( preTransRotNode.getAttribute( "y" ) );
        preTransRot.z = atof( preTransRotNode.getAttribute( "z" ) );


        /**********************************************************
        *	scaleNode
        *	Has the object scale information. A scale of 1 = no change
        ***********************************************************/
        XMLNode scaleNode = objectNode.getChildNode( "scale" );
        float scale = atof( scaleNode.getAttribute( "scale" ) );
        
        pTmpInfo->scale = scale;

        if( !pTmpGroup->CreateGroupCopy( pTmpInfo ) )
            return false;

        // Apply the position and rotations to the pTmpGroup
        pTmpGroup->SetPos( pos );
        pTmpGroup->SetPreTransRot( preTransRot );

        // Insert all the animated sprite groups into a map
        allGroupsVector.push_back( pTmpGroup );
        allGroupsMap.insert( make_pair( objName, pTmpGroup ) );
        allObjectInfo.insert( make_pair( objName, pTmpInfo ) );
        
    }

    SortActorVectors();

    return true;

}	// LoadFromXML


/************************************************************************
*    desc:  Sets a vector of actors to the current actor vector, 
*			overwriting the previous vector
*
*	 param: vector<CGroup *> & actorVector - vector to set
************************************************************************/
void CActor::SetActorVector( vector<CGroup *> & actorVector )
{
    ClearActorVectors();

    allGroupsVector = actorVector;

    SortActorVectors();

}	// SetActorVector


/************************************************************************
*    desc:  Adds a vector of actors to the current actor vector
*
*	 param: vector<CGroup *> & actorVector - vector to add
************************************************************************/
void CActor::AddActorVector( vector<CGroup *> & actorVector )
{
    for( unsigned int i = 0; i < actorVector.size(); i++ )
    {
        allGroupsVector.push_back( actorVector[i] );
    }

    SortActorVectors();

}	// AddActorVector


/************************************************************************
*    desc:  Clear all animated sprite group vectors
************************************************************************/
void CActor::ClearActorVectors()
{
    DeleteVectorPointers(allGroupsVector);

    allGroupsVector.clear();
    otherVector.clear();
    colPlayerVector.clear();
    colObjVector.clear();
    allColVector.clear();
    shadowVector.clear();

}	// ClearActorVectors


/************************************************************************
*    desc:  Send all pointers to their rightful animated sprite group 
*			vectors
************************************************************************/
void CActor::SortActorVectors()
{
    for( unsigned int i = 0; i < allGroupsVector.size(); i++ )
    {
        if( allGroupsVector[i]->GetCollisionSprite().IsActive() )
        {
            allColVector.push_back( allGroupsVector[i] );

            if( allGroupsVector[i]->GetObjectInfo().collideObj )
            {
                colObjVector.push_back( allGroupsVector[i] );
            }
            if( allGroupsVector[i]->GetObjectInfo().collidePlayer )
            {
                colPlayerVector.push_back( allGroupsVector[i] );
            }
        }

        if( allGroupsVector[i]->GetOtherSprite().IsActive() )
        {
            otherVector.push_back( allGroupsVector[i] );
        }
    }

}	// SortActorVectors


/************************************************************************
*    desc:  Inc the animation timer
************************************************************************/
void CActor::IncAnimationTime()
{
    for(unsigned int i = 0; i < allGroupsVector.size(); i++)
    {
        allGroupsVector[i]->IncAnimationTime();
    }

}	// UpdateAnimation


/************************************************************************
*    desc:  Renders the actors
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CActor::Render( CMatrix & matrix )
{
    /*for(unsigned int i = 0; i < allGroupsVector.size(); i++)
    {
        CShader::Instance().GetActiveShader()->SetTechnique( allGroupsVector[i]->GetShaderDisplay().c_str() );
        allGroupsVector[i]->GetVisualSprite().Render( matrix );
    }

    for(unsigned int i = 0; i < otherVector.size(); i++)
    {
        otherVector[i]->GetOtherSprite().Render( matrix );
    }*/

}	// Render


/************************************************************************
*    desc:  Renders the actor's shadow sprites
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CActor::RenderShadow( CMatrix & matrix )
{
    /*for(unsigned int i = 0; i < shadowVector.size(); i++)
    {
        shadowVector[i]->GetShadowSprite().Render( matrix );
    }*/

}	// RenderShadow


/************************************************************************
*    desc:  Renders the sprites that have the possibility of shadows 
*			being cast on them
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CActor::RenderShadowable( CMatrix & matrix )
{
    /*for( unsigned int i = 0; i < allGroupsVector.size(); i++ )
    {
        if(allGroupsVector[i]->GetObjectInfo().shadowable)
        {
            allGroupsVector[i]->GetVisualSprite().Render( matrix );
        }
    }

    for( unsigned int i = 0; i < otherVector.size(); i++ )
    {
        if(otherVector[i]->GetObjectInfo().shadowable)
        {
            otherVector[i]->GetOtherSprite().Render( matrix );
        }
    }*/

}	// RenderShadowable


/************************************************************************
*    desc:  Reacts to collision on an actor basis
*
*	 param:	vector<CGroup *> & colObj
*			- the vector of objects in which the actors can
*			  collide with
*
*			CCollisionCheck & cCheck -
************************************************************************/
void CActor::ReactToStageCollision( vector<CGroup *> & colObj, CCollisionCheck & cCheck )
{
    for( unsigned int i = 0; i < colObjVector.size(); i++ )
    {
        if( colObjVector[i]->GetObjectInfo().colideActive )
        {
            cCheck.Reset();

            colObjVector[i]->GetCollisionSprite().Transform();

            for(unsigned int j = 0; j < colObj.size(); j++)
            {
                cCheck.Sort( colObj[j]->ReactToCollision
                           ( colObjVector[i]->GetCollisionSprite().GetCenter(), 
                             colObjVector[i]->GetCollisionSprite().GetRadius(), 
                             colObjVector[i]->GetObjectInfo().collisionStats.GetFloorPad(), 
                             colObjVector[i]->GetObjectInfo().collisionStats.GetWallPad() ) );
            }

            colObjVector[i]->IncPos( cCheck.GetFinalObjectOffset() );
        }
    }  

}	// ReactToStageCollision


/************************************************************************
*    desc:  Reacts to collision against player on an actor basis
*
*	 param:	CMatrix & matrix - camera move matrix
*			CCollisionCheck & cCheck -
*			float radius - player radius
*			float fpad - floor pad
*			float wpad - wall pad
************************************************************************/
void CActor::ReactToPlayerCollision( CMatrix & matrix, CCollisionCheck & cCheck, float radius, float fpad, float wpad )
{
    // Apply all animated sprite collisions against the player
    for( unsigned int i = 0; i < colPlayerVector.size(); i++ )
    {
        if( colPlayerVector[i]->GetObjectInfo().colideActive )
        {
            cCheck.Sort( colPlayerVector[i]->ReactToCollision( matrix, CPoint(0,0,0), radius, fpad, wpad ) );
        }
    }

}	// ReactToPlayerCollision


/************************************************************************
*    desc:  Allow objects suseptible to gravity to fall
*
*	 param:	float speed - speed of gravity
************************************************************************/
void CActor::ReactToGravity( float speed, float elapsedTime )
{
    for(unsigned int i = 0; i < allGroupsVector.size(); i++)
    {
        if( allGroupsVector[i]->GetObjectInfo().gravity )
        {
            allGroupsVector[i]->IncPos( 0, -0.05f * elapsedTime, 0 );
        }
    }

}	// ReactToGravity