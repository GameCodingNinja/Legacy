
/************************************************************************
*    FILE NAME:       joint.h
*
*    DESCRIPTION:     3D joint class
************************************************************************/  

#ifndef __joint_h__
#define __joint_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "common\\point.h"

class CJoint
{
public:

    // Constructor
    CJoint() : vertCount(0)
    {}

    // Joint position
    CPoint headPos;
    CPoint tailPos;

    // Number of verts associated with this joint
    unsigned int vertCount;

    // joint name
    std::string name;

    // joint parent
    std::string parent;

    // Joint orientation matrix
    CMatrix matrix;
};

#endif  // __joint_h__

