
/************************************************************************
*    FILE NAME:       matuvlist.h
*
*    DESCRIPTION:     Class that holds the UVs to a material
************************************************************************/

#ifndef __mat_uv_list_h__
#define __mat_uv_list_h__

// Standard lib dependencies
#include <vector>
#include <string>

// Game lib dependencies
#include "common\\uv.h"
#include "common\\size.h"
#include "utilities\\xmlparsehelper.h"

// Boost lib dependencies

class CMatUVList
{
public:

    // Constructor
    CMatUVList();

    // Destructor
    virtual ~CMatUVList();

    // Load the UV list from an XML node
    bool LoadFromNode( XMLNode & node );

    // Get the increment size of the material
    CSize<float> & GetIncSize();

    // Get a pair of UV in the UV vector
    CUV & GetUV( unsigned int index );

protected:
    
    // Vector with all the UVs
    std::vector<CUV> uvVector;

    // The size of each material in the file
    CSize<float> incrementSize;

};

#endif  // __mat_uv_list_h__


