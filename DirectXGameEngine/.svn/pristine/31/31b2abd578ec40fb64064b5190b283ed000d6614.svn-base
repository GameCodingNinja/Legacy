/************************************************************************
*    FILE NAME:       sectordata.h
*
*    DESCRIPTION:     Class that holds sector's file path and position
*					  for later loading
************************************************************************/

#ifndef __sector_data_h__
#define __sector_data_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "common/pointint.h"

// Forward Declarations
struct XMLNode;

class CSectorData
{
public:

    // Load thes sector data from node
    void LoadFromNode( const XMLNode & sectorNode );

    // Get the file path
    const std::string & GetFile() const;

    // World position of the sector
    const CPointInt & GetPos() const;

private:

    // File path of the sector
    std::string file;

    // World position of the sector
    CPointInt position;

};

#endif	// __sector_data_h__