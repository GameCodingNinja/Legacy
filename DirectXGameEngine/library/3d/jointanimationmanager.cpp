
/************************************************************************
*    FILE NAME:       jointanimationmanager.cpp
*
*    DESCRIPTION:     3D joint animation loading class singleton
************************************************************************/

// Physical component dependency
#include <3d/jointanimationmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>

// Game lib dependencies
#include <system/xdevice.h>
#include <utilities/exceptionhandling.h>
#include <utilities/smartpointers.h>
#include <managers/shader.h>
#include <misc/spritebinaryfileheader.h>
#include <3d/meshanimation.h>
#include <3d/jointanimation.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CJointAnimMgr::CJointAnimMgr()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CJointAnimMgr::~CJointAnimMgr()
{
}   // Destructer


/************************************************************************
*    desc:  Load the joint animation from file path
*  
*    param: string & filePath - path to joint animation file
* 
*    return: CMeshAnim * - NULL on fail
************************************************************************/
CMeshAnim * CJointAnimMgr::LoadFromFile( string & jointAnimPath )
{
    // See if this joint animation has already been loaded
    animMapIter = spAnimMap.find( jointAnimPath );

    // If it's not found, load the joint animation and add it to the list
    if( animMapIter == spAnimMap.end() )
        return LoadAnimFromRSA( jointAnimPath );

    else
        return animMapIter->second;

}	// LoadFromFile


/************************************************************************
*    desc:  Load the joint animation from RSA file
*  
*    param: string & _filePath - path to file
*    
*    return: CMeshAnim * - NULL on fail
************************************************************************/
CMeshAnim * CJointAnimMgr::LoadAnimFromRSA( string & filePath )
{
    bool result = false;

    CSpriteAnimationBinaryFileHeader fileHeader;

    // Open the file
    NSmart::scoped_filehandle_ptr<FILE> pFile( fopen( filePath.c_str(), "rb" ) );
    if( pFile.isNull() )
        throw NExcept::CCriticalException("Joint Animation Manager Load Error!",
            boost::str( boost::format("Error Opening file (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

    // Read in the file header
    fread( &fileHeader, 1, sizeof(fileHeader), pFile.get() );

    // Allocate our mesh animation class
    CMeshAnim * pMeshAnim = new CMeshAnim();

    // Add the joint animation to our list
    spAnimMap.insert( filePath, pMeshAnim );

    // Set the counts
    pMeshAnim->jCount = fileHeader.joint_count;
    pMeshAnim->fCount = fileHeader.frame_count;

    // Load in some info
    float frameCount = fileHeader.frame_count;

    // Allocate all the joints in this animation
    pMeshAnim->pJointAnim.reset( new CJointAnim[ fileHeader.joint_count ] );

    // Set up a variable to test where we are in the file
    int tag_check;

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != JOINT )
        throw NExcept::CCriticalException("Joint Animation Manager Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

    for( int i = 0; i < fileHeader.joint_count; ++i )
    {
        // Load in the joint information
        CBinaryJointAnimation joint;

        fread( &joint, 1, sizeof(joint), pFile.get() );

        pMeshAnim->pJointAnim[i].name = joint.name;
        pMeshAnim->pJointAnim[i].parent = joint.parentName;

        // Read in new tag
        fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

        // Check and make sure we are where we should be
        if( tag_check != KEY_FRAME )
            throw NExcept::CCriticalException("Joint Animation Manager Load Error!",
                boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

        // Not all joints will have information
        if( joint.keyframe_count > 0 )
        {
            int keyFrameCount = joint.keyframe_count;
            pMeshAnim->pJointAnim[i].keyFrameCount = keyFrameCount;
            pMeshAnim->pJointAnim[i].pKeyFrame.reset( new CKeyFrame[keyFrameCount] );

            // Allocate an array to load in this group of keyframes
            boost::scoped_array<CBinaryKeyFrame> pBinaryKeyFrame( new CBinaryKeyFrame[keyFrameCount] );

            // Read in this group of key farmes
            fread( pBinaryKeyFrame.get(), keyFrameCount, sizeof(CBinaryKeyFrame), pFile.get() );

            for( int j = 0; j < keyFrameCount; ++j )
            {
                pMeshAnim->pJointAnim[i].pKeyFrame[j].time = pBinaryKeyFrame[j].time;

                pMeshAnim->pJointAnim[i].pKeyFrame[j].pos.x = pBinaryKeyFrame[j].x;
                pMeshAnim->pJointAnim[i].pKeyFrame[j].pos.y = pBinaryKeyFrame[j].y;
                pMeshAnim->pJointAnim[i].pKeyFrame[j].pos.z = pBinaryKeyFrame[j].z;

                pMeshAnim->pJointAnim[i].pKeyFrame[j].rot.x = pBinaryKeyFrame[j].rx;
                pMeshAnim->pJointAnim[i].pKeyFrame[j].rot.y = pBinaryKeyFrame[j].ry;
                pMeshAnim->pJointAnim[i].pKeyFrame[j].rot.z = pBinaryKeyFrame[j].rz;
            }
        }
    }

    return pMeshAnim;

}	// LoadAnimFromRSA