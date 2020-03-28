/************************************************************************
*    FILE NAME:       hotspotwarp.h
*
*    DESCRIPTION:     Hot spot warp class
************************************************************************/

#ifndef __hotspotwarp_h__
#define __hotspotwarp_h__

// Physical component dependency
#include <3d/hotspot.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/point.h>

class CHotSpotWarp : public CHotSpot
{
public:

    CHotSpotWarp();
    ~CHotSpotWarp();

    void SetCameraPos( CPoint pos );
    CPoint GetCameraPos();

    void SetCameraRot( CPoint rot );
    CPoint GetCameraRot();

    void SetDestination( std::string dest );
    std::string GetDestination();
    
private:
    
    std::string destination;

    CPoint camera_pos;
    CPoint camera_rot;

};
#endif  // __hotspotwarp_h__
