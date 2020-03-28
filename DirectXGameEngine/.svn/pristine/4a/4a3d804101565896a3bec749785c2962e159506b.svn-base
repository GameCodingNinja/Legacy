
/************************************************************************
*    FILE NAME:       jointanimation.h
*
*    DESCRIPTION:     3D joint animation class
************************************************************************/  

#ifndef __joint_animation_h__
#define __joint_animation_h__

// Physical component dependency
#include <3d/keyframe.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/scoped_array.hpp>

class CJointAnim
{
public:

    CJointAnim() : pKeyFrame(NULL), keyFrameCount(0)
    {}

    // joint animation class
    boost::scoped_array<CKeyFrame> pKeyFrame;

    // total number of key frames in this animation
    int keyFrameCount;

    // joint name
    std::string name;

    // joint parent
    std::string parent;

};

#endif  // __joint_animation_h__

