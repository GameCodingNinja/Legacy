/************************************************************************
*    FILE NAME:       stagedata.h
*
*    DESCRIPTION:     Class that contains all objects for a single stage
************************************************************************/

#ifndef __stage_data_h__
#define __stage_data_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/sectordata.h>

// Forward declaration(s)
class CSectorData;

class CStageData : public boost::noncopyable
{
public:

    CStageData(){}
    ~CStageData(){}

    // Load the stage data from an xml
    void LoadFromXML( std::string & filePath );

    // Get sector data
    const CSectorData & GetSectorData( uint index ) const;

    // Get the camera position
    const CPoint & CameraPos() const;

    // Get the camera rotation
    const CPoint & CameraRot() const;

    // Get the shader effect
    const std::string & GetEffect() const;

    // Get Sector count
    size_t GetSectorCount() const;

private:

    // Vector of sectors for later loading
    boost::ptr_vector<CSectorData> spSectorVec;
    
    // Filepath of the sky mesh
    std::string skyFilePath;

    // Position and rotation of the sky mesh
    CPoint skyPos;
    CPoint skyRot;

    // Position and rotation of the default camera
    CPoint cameraPos;
    CPoint cameraRot;

    // Shader effect
    std::string effect;

};

#endif  // __stage_data_h__