/************************************************************************
*    FILE NAME:       xmlparsehelper.h
*
*    DESCRIPTION:     General xml parse helper functions
************************************************************************/           

#ifndef __xml_parse_helper_h__
#define __xml_parse_helper_h__

// Standard lib dependencies
#include <assert.h>
#include <utility>
#include <string>

// Game lib dependencies
#include <common/point.h>
#include <common/pointInt.h>
#include <common/worldpoint.h>
#include <common/transformdata.h>
#include <common/color.h>
#include <common/size.h>
#include <common/defs.h>
#include <common/vertex2d.h>
#include <common/dynamicoffset.h>
#include <xmlParser/xmlParser.h>

namespace NParseHelper
{
    // Load the 2d vertex
    CVertex2D LoadVertex2d( const XMLNode & node );

    // load position data
    CWorldPoint LoadPosition( const XMLNode & node );
    CPointInt LoadPositionInt( const XMLNode & node );

    // Load the rotation
    CPoint LoadRotation( const XMLNode & node );

    // Load the scale
    CPoint LoadScale( const XMLNode & node );

    // Load the transform data
    CTransformData LoadTransformData( const XMLNode & node );

    // Load the color
    CColor LoadColor(const  XMLNode & node );

    // Load the size
    CSize<float> LoadSize( const XMLNode & node );

    // Load the divisions of a mesh
    std::pair<int,int> LoadDivisions( const XMLNode & node );

    // Load the horizontal alignment
    EHorzAlignment LoadHorzAlignment( const XMLNode & node, EHorzAlignment horzAlignment );

    // Load the vertical alignment
    EVertAlignment LoadVertAlignment( const XMLNode & node, EVertAlignment vertAlignment );

    // Load the dynamic offset
    CDynamicOffset LoadDynamicOffset( const XMLNode & node );
}

#endif  // __xml_parse_helper_h__
