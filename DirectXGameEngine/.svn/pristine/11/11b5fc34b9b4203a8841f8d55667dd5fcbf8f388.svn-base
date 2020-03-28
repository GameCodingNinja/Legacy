
/************************************************************************
*    FILE NAME:       ojectvisualdata3d.h
*
*    DESCRIPTION:     Class containing the 3D object's visual data
************************************************************************/

#ifndef __object_visual_data_3d_h__
#define __object_visual_data_3d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <misc/settings.h>
#include <common/color.h>

// Forward Declarations
struct XMLNode;

class CObjectVisualData3D
{
public:
    
    // Constructor
    CObjectVisualData3D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );

    // Get the projection type
    CSettings::EProjectionType GetProjectionType() const;

    // Get the file path
    const std::string & GetFile() const;

    // Get the name of the shading effect
    const std::string & GetEffect() const;

    // Get the name of the shading technique
    const std::string & GetTechnique() const;

    // Get the color
    const CColor & GetColor() const;

    // Get the specular shine
    float GetSpecularShine() const;

    // Get the specular intensity
    float GetSpecularIntensity() const;

    // Get the flag if uses alpha textures and require sorting
    bool IsAlpha() const;

    // Get the animated flag
    bool IsAnimated() const;

    // Get the path to the animation file
    const std::string & GetAnimFile() const;

    // Get the name of the default animation
    const std::string & GetDefaultAnim() const;
        
private:
    
    // Mesh file path
    std::string file;		

    // The type of projection to render the visual object
    CSettings::EProjectionType projectionType;

    // Name of the shader technique and effect
    std::string effect, technique;		

    // Initial color of the object
    CColor color;

    // Value of specular shine and intensity
    float specularShine, specularIntensity;

    // Does it use alpha textures and require sorting?
    bool alpha;

    // xml with all the joint animation files
    std::string animFile;

    // The default animation if the object is animated
    std::string defaultAnimation;
};

#endif  // __object_visual_data_3d_h__