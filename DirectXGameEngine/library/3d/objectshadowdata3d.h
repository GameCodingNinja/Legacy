
/************************************************************************
*    FILE NAME:       objectshadowdata3d.h
*
*    DESCRIPTION:     Class containing the 3D object's shadow data
************************************************************************/

#ifndef __object_shadow_data_3d_h__
#define __object_shadow_data_3d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <misc/settings.h>

// Forward Declarations
struct XMLNode;

class CObjectShadowData3D
{
public:
    
    // Constructer
    CObjectShadowData3D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );

    // Get the file path
    const std::string & GetFile() const;

    // Get the name of the shading effect
    const std::string & GetEffect() const;

    // Get the name of the shading technique
    const std::string & GetTechnique() const;

    // Get the projection type
    CSettings::EProjectionType GetProjectionType() const;

    // Get the minimum Z distance
    float GetMinZDist() const;

    // Get the maximum Z distance
    float GetMaxZDist() const;

    // Get the bias
    float GetBias() const;

    // Get the orthographic screen percentage
    float GetOrthoScrnPercent() const;

    // Get the flag if uses alpha textures and require sorting
    bool GetCastShadow() const;

private:

    // Mesh file path - In the even a simpler mesh is needed for shadows
    std::string file;

    // Name of the shader technique and effect
    std::string technique, effect;

    // Type of shadow projection
    CSettings::EProjectionType projectionType;

    // Shadow values
    float minZDist, maxZDist, bias, orthoScreenPercent;

    // Does this cast a shadow?
    bool castShadow;

};

#endif  // __object_shadow_data_3d_h__