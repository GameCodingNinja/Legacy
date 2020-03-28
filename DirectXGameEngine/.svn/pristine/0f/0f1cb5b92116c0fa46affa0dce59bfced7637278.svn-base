
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_2d_h__
#define __object_visual_data_2d_h__

// Standard lib dependencies
#include <string>
#include <utility>

// Game lib dependencies
#include <common/size.h>
#include <common/color.h>
#include <misc/settings.h>

// Forward Declarations
struct XMLNode;

class CObjectVisualData2D
{
public:
    
    // Constructor
    CObjectVisualData2D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );

    // Get the file path
    const std::string & GetFile() const;

    // Get the projection type
    CSettings::EProjectionType GetProjectionType() const;

    // Set and get the size of the mesh
    void SetSize( const CSize<float> & _size );
    const CSize<float> & GetSize() const;

    // Get the number of rows
    int GetRows() const;

    // Get the number of columns
    int GetColumns() const;

    // Get the name of the shading effect
    const std::string & GetEffect() const;

    // Get the name of the shading technique
    const std::string & GetTechnique() const;

    // Get the color
    const CColor & GetColor() const;
        
private:
    
    // Mesh file path
    std::string file;

    // Size of the mesh
    CSize<float> size;

    // Number of rows and columns to divide the visual mesh
    // first = rows, second = columns
    std::pair<int,int> rowsColumns;

    // The type of projection to render the visual object
    CSettings::EProjectionType projectionType;

    // Name of the shader technique and effect
    std::string effect, technique;		

    // Initial color of the object
    CColor color;

};

#endif  // __object_visual_data_2d_h__