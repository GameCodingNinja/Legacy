
/************************************************************************
*
*    FILE NAME:       jointmultilinklist.h
*
*    DESCRIPTION:     Dynamic joint linked tree list template class
*
************************************************************************/

#ifndef __JOINT_LINK_LIST_H__
#define __JOINT_LINK_LIST_H__

// Physical component dependency
#include <utilities/multilinklist.h>

template <class node_type>
class CJointLinkLst : public CMultiLinkLst<node_type>
{
public:

    // Constructer
    CJointLinkLst();

    // Destructer
    virtual ~CJointLinkLst();

    // Transform the joint verts 
    void TransformJoints( float time );

    // Transition to the tween Rot & pos to the passed in animation
    void TransitionTweensPosRot( CJointLinkLst<node_type> * pTrans,
                                 float time,
                                 float frameCount );

    // Transition the tween pos & rot
    void TransTweenPosRot( CJointLinkLst<node_type> * pTrans,
                           float time,
                           float frameCount );

    // Calc the tween rotation and translation
    void CalcTweenPosRot( float time );

    // Get/Set Frame Count
    float GetFrameCount();
    void SetFrameCount( float value );

    // Get/Set Speed
    float GetSpeed();
    void SetSpeed( float value );

protected:

    // Transform the joint verts
    void Transform( node_type node );

    // Max time frames
    float frameCount;

    // Default speed of animation
    float speed;

};


/************************************************************************
*    desc:   Constructer                                                             
************************************************************************/
template <class node_type>
CJointLinkLst<node_type>::CJointLinkLst() : frameCount(0), speed(0)
{

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
template <class node_type>
CJointLinkLst<node_type>::~CJointLinkLst()
{

}   // Destructer


/************************************************************************
*    desc:  Transform the joint verts
*
*	 parm:  float time - time kept by the animation
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::TransformJoints( float time )
{
    // reset the node iterators for searching
    ResetSearchIterators();

    // Calc the tween rotation and translation
    CalcTweenPosRot( time );

    // Transform through out the node tree
    Transform( head );

}   // TransformJoints


/************************************************************************
*    desc:  Transition to the tween Rot & pos to the passed in animation
*
*	 parm:  CJointLinkLst<node_type> * pTrans - link list to trans with
*           float time - time kept by the animation to be transitioned
*           float frameCount - frame count of the transition
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::TransitionTweensPosRot( CJointLinkLst<node_type> * pTrans,
                                                       float time,
                                                       float frameCount )
{
    // reset the node iterators for searching
    ResetSearchIterators();

    // Transition to the tween Rot & pos
    TransTweenPosRot( pTrans, time, frameCount );

    // Transform through out the node tree
    Transform( head );

}   // TransitionTweensPosRot


/************************************************************************
*    desc:  Transition the tween pos & rot and transform joints
*
*	 parm:  CJointLinkLst<node_type> * pTrans - link list to trans with
*           float time - time kept by the animation to be transitioned
*           float frameCount - frame count of the transition
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::TransTweenPosRot( CJointLinkLst<node_type> * pTrans, 
                                                 float time,
                                                 float frameCount )
{
    for( size_t i = 0; i < nodeList.size(); ++i )
    {
        nodeList[i]->TransTweenPosRot( pTrans->GetNode(i), time, frameCount );
    }
}	// TransTweenPosRot


/************************************************************************                                                             
*    desc:  Transform the joint verts
*           NOTE: This is a recursive function
*
*    param: node_type * node - new node
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::Transform( node_type node )
{
    node_type nextNode = NULL;

    // Transform the joints
    node->Transform();

    do
    {
        // get the next node 
        nextNode = node->GetNextNode();

        if( nextNode != NULL )
        {
            // Call a recursive function to the next node
            Transform( nextNode );
        }
    }
    while( nextNode != NULL );

}	// Transform


/************************************************************************                                                             
*    desc:  Calc the tween rotation and translation
*
*    param: float time - animation time
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::CalcTweenPosRot( float time )
{
    for( std::vector<node_type>::iterator it = nodeList.begin(); 
             it != nodeList.end(); ++it )
    {
        (*it)->CalcTweenPosRot( time );
    }
}	// ResetSearchIterators


/************************************************************************                                                             
*    desc:  Get frame count
*
*    ret: float
************************************************************************/
template <class node_type>
float CJointLinkLst<node_type>::GetFrameCount()
{
    return frameCount;
}


/************************************************************************                                                             
*    desc:  Set frame count
*
*    param: float - frame count
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::SetFrameCount( float value )
{
    frameCount = value;
}


/************************************************************************                                                             
*    desc:  Get speed
*
*    ret: float
************************************************************************/
template <class node_type>
float CJointLinkLst<node_type>::GetSpeed()
{
    return speed;
}


/************************************************************************                                                             
*    desc:  Set speed
*
*    param: float - speed
************************************************************************/
template <class node_type>
void CJointLinkLst<node_type>::SetSpeed( float value )
{
    speed = value;
}


#endif	// __JOINT_LINK_LIST_H__
