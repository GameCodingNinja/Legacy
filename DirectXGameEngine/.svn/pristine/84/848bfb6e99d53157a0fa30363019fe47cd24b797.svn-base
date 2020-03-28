
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.cpp
*
*    DESCRIPTION:     Class containing the 3D object's visual data
************************************************************************/

// Physical component dependency
#include <2d/objectvisualdata2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CObjectVisualData2D::CObjectVisualData2D()
                   : projectionType( CSettings::EPT_ORTHOGRAPHIC ),
                     rowsColumns(1,1)
{
}   // constructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CObjectVisualData2D::LoadFromNode( const XMLNode & objectNode )
{
    const XMLNode visualNode = objectNode.getChildNode( "visual" );

    if( !visualNode.isEmpty() )
    {
        if( visualNode.isAttributeSet("file") )
            file = visualNode.getAttribute( "file" );

        // Get the projection node
        const XMLNode projectionNode = visualNode.getChildNode( "projection" );
            
        if( projectionNode.isAttributeSet("type") )
        {
            if( std::string(projectionNode.getAttribute( "type" )) == "perspective" )
                projectionType = CSettings::EPT_PERSPECTIVE;

            else if( std::string(projectionNode.getAttribute( "type" )) == "orthographic" )
                projectionType = CSettings::EPT_ORTHOGRAPHIC;
        }

        // The size is the desired size of the mesh, which overrites the size of the image
        const XMLNode sizeNode = visualNode.getChildNode( "size" );

        if( !sizeNode.isEmpty() )
            size = NParseHelper::LoadSize( sizeNode );

        // The divisions are the number of quads used to make up the mesh, and are specified
        // with rows and columns
        const XMLNode divNode = visualNode.getChildNode( "divisions" );

        if( !divNode.isEmpty() )
            // first = rows, second = columns
            rowsColumns = NParseHelper::LoadDivisions( divNode );

        // Check for material color
        color = NParseHelper::LoadColor( visualNode );

        // The shader node determines which effect and technique a visual sprite will use
        const XMLNode shaderNode = visualNode.getChildNode( "shader" );

        if( !shaderNode.isEmpty() )
        {
            // Set the strings for shader
            if( shaderNode.isAttributeSet("effect") )
                effect = shaderNode.getAttribute( "effect" );

            if( shaderNode.isAttributeSet("technique") )
                technique = shaderNode.getAttribute( "technique" );
        }

        // Raise an exception if the shader effect or technique are not set
        if( effect.empty() || technique.empty() )
            throw NExcept::CCriticalException("Shader effect or techique not set!",
                boost::str( boost::format("Shader object data missing.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Get the file path
************************************************************************/
const std::string & CObjectVisualData2D::GetFile() const 
{
    return file;
}


/************************************************************************
*    desc:  Get the projection type
************************************************************************/
CSettings::EProjectionType CObjectVisualData2D::GetProjectionType() const
{
    return projectionType;
}


/************************************************************************
*    desc:  Set and get the size
************************************************************************/
void CObjectVisualData2D::SetSize( const CSize<float> & _size )
{
    size = _size;
}

const CSize<float> & CObjectVisualData2D::GetSize() const
{
    return size;
}


/************************************************************************
*    desc:  Get the number of rows
************************************************************************/
int CObjectVisualData2D::GetRows() const
{
    return rowsColumns.first;
}


/************************************************************************
*    desc:  Get the number of columns
************************************************************************/
int CObjectVisualData2D::GetColumns() const
{
    return rowsColumns.second;
}


/************************************************************************
*    desc:  Get the name of the shading effect
************************************************************************/
const std::string & CObjectVisualData2D::GetEffect() const
{
    return effect;
}


/************************************************************************
*    desc:  Get the name of the shading technique
************************************************************************/
const std::string & CObjectVisualData2D::GetTechnique() const
{
    return technique;
}


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CObjectVisualData2D::GetColor() const 
{
    return color;
}
