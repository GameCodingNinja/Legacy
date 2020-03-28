/************************************************************************
*    FILE NAME:       jointnode.cpp
*
*    DESCRIPTION:     3D joint node class
************************************************************************/  

// Physical component dependency
#include <3d/jointnode.h>

// Game lib dependencies
#include <common/face.h>
#include <3d/jointanimation.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CJointNode::CJointNode() 
          : pPrevNode(NULL), pJointAnim(NULL)
{
}


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CJointNode::~CJointNode()
{
}


/************************************************************************                                                             
*    desc:  Reset for searching
************************************************************************/
void CJointNode::Reset()
{
    it = nextNode.begin();
}


/************************************************************************                                                             
*    desc:  Print the name of the node
************************************************************************/
void CJointNode::PrintId()
{
    //NGenFunc::PostDebugMsg( "%s", id.c_str() );
}


/************************************************************************                                                             
*    desc:  Get the next node
************************************************************************/
CJointNode * CJointNode::GetNextNode()
{
    CJointNode * pResult = NULL;

    if( it != nextNode.end() )
    {
        pResult = *it;
        ++it;
    }

    return pResult;

}	// GetNextNode


/************************************************************************
*    desc:  Transform the joint
************************************************************************/
void CJointNode::Transform()
{
    // init the matrix
    matrix.InitilizeMatrix();

    matrix.Rotate( tweenRot );
    matrix.Translate( tweenPos );

    matrix *= orientationMatrix;

    // Merge in the previous matrix so that the joint gets
    // the movement of it's parent joint
    if( pPrevNode != NULL )
        matrix *= pPrevNode->matrix;

}	// Transform


/************************************************************************
*    desc:  Calc the tween rotation and translation
*
*    param: float time - Hi-res milisecond time that has passed
************************************************************************/
void CJointNode::CalcTweenPosRot( float time )
{
    // find the time slot this belongs in
    if( pJointAnim != NULL )
    {
        for( int i = 0; i < pJointAnim->keyFrameCount - 1; ++i )
        {
            if( time > pJointAnim->pKeyFrame[i].time &&
                time < pJointAnim->pKeyFrame[i+1].time )
            {
                // Calculate the amount (0.0 - 1.0) to average between the two key frames
                float range = pJointAnim->pKeyFrame[i+1].time - pJointAnim->pKeyFrame[i].time;
                float amount = 1.0f - ((pJointAnim->pKeyFrame[i+1].time - time) / range);

                // Convert the position
                tweenPos.x = ((1.f-amount) * pJointAnim->pKeyFrame[i].pos.x) +
                                 (amount * pJointAnim->pKeyFrame[i+1].pos.x);
                tweenPos.y = ((1.f-amount) * pJointAnim->pKeyFrame[i].pos.y) +
                                 (amount * pJointAnim->pKeyFrame[i+1].pos.y);
                tweenPos.z = ((1.f-amount) * pJointAnim->pKeyFrame[i].pos.z) +
                                 (amount * pJointAnim->pKeyFrame[i+1].pos.z);

                // Convert the rotation
                tweenRot.x = ((1.f-amount) * pJointAnim->pKeyFrame[i].rot.x) +
                                 (amount * pJointAnim->pKeyFrame[i+1].rot.x);
                tweenRot.y = ((1.f-amount) * pJointAnim->pKeyFrame[i].rot.y) +
                                 (amount * pJointAnim->pKeyFrame[i+1].rot.y);
                tweenRot.z = ((1.f-amount) * pJointAnim->pKeyFrame[i].rot.z) +
                                 (amount * pJointAnim->pKeyFrame[i+1].rot.z);

                break;
            }
        }
    }

}	// TweenPosRot


/************************************************************************
*    desc:  Transition the tween rotation and translation
*
*	 parm:  CJointNode * pJointNode - node to trans with
*           float time - time kept by the animation to be transitioned
*           float frameCount - frame count of the transition
************************************************************************/
void CJointNode::TransTweenPosRot( CJointNode * pJointNode,
                                   float time,
                                   float frameCount )
{
    // Calculate the amount (0.0 - 1.0) to average between the tweens
    float amount = time / frameCount;

    // Convert the position
    tweenPos.x = ((1.f-amount) * tweenPos.x) + (amount * pJointNode->tweenPos.x);
    tweenPos.y = ((1.f-amount) * tweenPos.y) + (amount * pJointNode->tweenPos.y);
    tweenPos.z = ((1.f-amount) * tweenPos.z) + (amount * pJointNode->tweenPos.z);

    // Convert the rotation
    tweenRot.x = ((1.f-amount) * tweenRot.x) + (amount * pJointNode->tweenRot.x);
    tweenRot.y = ((1.f-amount) * tweenRot.y) + (amount * pJointNode->tweenRot.y);
    tweenRot.z = ((1.f-amount) * tweenRot.z) + (amount * pJointNode->tweenRot.z);

}	// TransTweenPosRot