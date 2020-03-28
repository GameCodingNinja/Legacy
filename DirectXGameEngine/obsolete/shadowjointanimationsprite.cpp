/************************************************************************
*    FILE NAME:       shadow jointanimationsprite.cpp
*
*    DESCRIPTION:     3D shadow joint animation sprite class
************************************************************************/

// Physical component dependency
#include "shadowjointanimationsprite.h"

// Game OS dependencies
#include <assert.h>

// Game lib dependencies
#include "meshmanager.h"
#include "xwindow.h"
#include "mesh.h"
#include "statcounter.h"
#include "face.h"
#include "genfunc.h"
#include "joint.h"
#include "shadowjointanimationmesh.h"
#include "xmlParser\\xmlParser.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CShadowJointAnimSprite::CShadowJointAnimSprite()
                      : CShadowSprite(),
                        pCurrentAnim(NULL),
                        time(1.0f),
                        pTransToAnim(NULL),
                        transTime(0.f),
                        transFrameCount(0.f),
                        pFace(NULL)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CShadowJointAnimSprite::~CShadowJointAnimSprite()
{
    
}   // Destructer


/************************************************************************
*    desc:  Init the sprite with the path to the mesh file
*  
*    param: std::string & meshPath - path to mesh xml
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowJointAnimSprite::Init( string & path )
{
    // Load the mesh
    pMesh = CMeshMgr::Instance().GetShadowJointMesh( path );

    if( NULL == pMesh )
    {
        return false;
    }

    // Have the mesh class create our DirectX vertex buffer. It returns a pointer
    // that we are responcible for Deleting
    if( (pFace = (dynamic_cast<CShadowJointAnimMesh *>(pMesh))->GetFacePtr()) == NULL )
    {
        return false;
    }

    AllocateArrays();

    return true;

}   // Init


/************************************************************************
*    desc:  Set the active animation
*  
*    param: string & name - name of animation
*           float frameCount - number of frames to transition the animation
************************************************************************/
void CShadowJointAnimSprite::SetAnimation( std::string & name, float frameCount )
{
    // Set the current animation if not set
    if( pCurrentAnim == NULL || frameCount < 1.f )
    {
        pCurrentAnim = pMesh->GetAnimation( name );

        time = 1.f;
    }
    else if( (animNameStr != name) )
    {
        // Only reset the time if we are not transitioning
        if( pTransToAnim == NULL )
        {
            transTime = 0.f;
        }

        transFrameCount = frameCount;

        pTransToAnim = pMesh->GetAnimation( name );
    }

    animNameStr = name;

}	// SetAnimation


/************************************************************************
*    desc:  Update the animation
*
*    param: CMatric & matrix - passed in matrix class
*    param: CLightLst & lightLst - light list class
************************************************************************/
void CShadowJointAnimSprite::UpdateAnimation( CMatrix & matrix, CLightLst & lightLst )
{
    /*if( pCurrentAnim != NULL && lightLst.IsShadowCast() )
    {
        bool cullMesh = true;

        // Transform the center point - need this to be in world view
        Transform();

        // Don't increment the time during a transition
        if( pTransToAnim == NULL )
        {
            // increment the time
            time += CHighResTimer::Instance().GetElapsedTime() * pCurrentAnim->GetSpeed();

            // Reset the time if we go over
            if( time > pCurrentAnim->GetFrameCount() )
            {
                time = 1.0f + CHighResTimer::Instance().GetElapsedTime() * pCurrentAnim->GetSpeed();
            }
        }
        // Inc the trans time during a transition
        else
        {
            transTime += CHighResTimer::Instance().GetElapsedTime() * pTransToAnim->GetSpeed();
        }

        // All lights that cast shadow must all be culled to cull the animation
        for( unsigned int i = 0; i < lightLst.GetCount() && cullMesh; i++ )
        {
            if( lightLst.GetLight(i)->castShadow && lightLst.GetLight(i)->enable )
            {
                // See if we should cull this animation and shadow
                cullMesh = PreBuildCullCheck( matrix, lightLst.GetLight(i)->GetPos() );
            }
        }

        // Do we need to cull this animation
        if( !cullMesh )
        {
            if( pTransToAnim != NULL )
            {
                if( transTime < transFrameCount )
                {
                    // Since this data is owned by the mesh, it might have been reset by another sprite
                    pTransToAnim->CalcTweenPosRot( 1.0001f );
                    pCurrentAnim->CalcTweenPosRot( time );
                    pCurrentAnim->TransitionTweensPosRot( pFace, pTransToAnim, transTime, transFrameCount );
                }
                else
                {
                    // Set the transition animation to the current animation
                    pCurrentAnim = pTransToAnim;
                    time = 1.f + CHighResTimer::Instance().GetElapsedTime() * pCurrentAnim->GetSpeed();

                    pCurrentAnim->TransformJoints( pFace, time );
                    pMesh->CalcSurfaceNormal();

                    pTransToAnim = NULL;
                    transTime = 0.f;
                    transFrameCount = 0.f;
                }
            }
            else
            {
                pCurrentAnim->TransformJoints( pFace, time );
                pMesh->CalcSurfaceNormal();
            }

            // This call only builds the shadow if it's not culled
            BuildMesh( matrix, lightLst );
        }
    }*/

}	// UpdateAnimation


