
/************************************************************************
*    FILE NAME:       jointnode.h
*
*    DESCRIPTION:     3D joint node class
************************************************************************/  

#ifndef __joint_node_h__
#define __joint_node_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/matrix.h>

// Forward declaration(s)
class CJointAnim;

class CJointNode : public boost::noncopyable
{
public:

    // Constructor
    CJointNode();

    // destructor
    ~CJointNode();

    // previous node
    CJointNode * pPrevNode;

    // next node
    std::vector<CJointNode *> nextNode;

    // next node iterator
    std::vector<CJointNode *>::iterator it;

    // Joint position
    CPoint headPos;
    CPoint tailPos;

    // Joint orientation matrix
    CMatrix orientationMatrix;

    // tween pos
    CPoint tweenPos;

    // Tween rot
    CRadian tweenRot;

    // Matrix class to do the transforming
    // of the verts and normals
    CMatrix matrix;

    // id of node
    std::string id;

    // parent id of node
    std::string parentId;

    // Pointer to animation data
    CJointAnim * pJointAnim;
                                                             
    // Reset for searching
    void Reset();

    // Print the name of the node
    void PrintId();

    // Get the next node
    CJointNode * GetNextNode();

    // Transform the joint
    void Transform();

    // Get the tween rotation and translation
    void CalcTweenPosRot( float time );

    // transition the tween rotation and translation
    void TransTweenPosRot( CJointNode * pJointNode,
                           float time,
                           float frameCount );
};

#endif  // __joint_node_h__