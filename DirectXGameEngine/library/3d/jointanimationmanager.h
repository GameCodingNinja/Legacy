
/************************************************************************
*    FILE NAME:       jointanimationmanager.h
*
*    DESCRIPTION:     3D joint animation loading class singleton
************************************************************************/  

#ifndef __joint_animation_manager_h__
#define __joint_animation_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Forward declaration(s)
class CMeshAnim;

class CJointAnimMgr
{
public:

    // Get the instance of the singleton class
    static CJointAnimMgr & Instance()
    {
        static CJointAnimMgr jointAnimMgr;
        return jointAnimMgr;
    }

    // Load the material from file path
    CMeshAnim * LoadFromFile( std::string & jointAnimPath );

    // Load the joint animation from RSA file
    CMeshAnim * LoadAnimFromRSA( std::string & filePath );

protected:

    // map list loaded animations
    boost::ptr_map<std::string, CMeshAnim> spAnimMap;
    boost::ptr_map<std::string, CMeshAnim>::iterator animMapIter;

private:

    CJointAnimMgr();
    ~CJointAnimMgr();

};

#endif  // __joint_animation_manager_h__
