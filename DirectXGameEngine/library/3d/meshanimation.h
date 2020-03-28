
/************************************************************************
*    FILE NAME:       meshanimation.h
*
*    DESCRIPTION:     3D mesh animation class
************************************************************************/  

#ifndef __mesh_animation_h__
#define __mesh_animation_h__

// Boost lib dependencies
#include <boost/scoped_array.hpp>

// Forward declaration(s)
class CJointAnim;

class CMeshAnim
{
public:

    CMeshAnim() : pJointAnim(NULL), jCount(0), fCount(0)
    {}

    // joint animation class
    boost::scoped_array<CJointAnim> pJointAnim;

    // Total number of joints in this animation
    int jCount;

    // Total number of frames in this animation
    int fCount;

};

#endif  // __mesh_animation_h__

