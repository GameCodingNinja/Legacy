
/************************************************************************4r
*    FILE NAME:       ojectvisualdata3d.cpp
*
*    DESCRIPTION:     Class containing the 3D object's visual data
************************************************************************/

// Physical component dependency
#include <3d/objectvisualdata3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CObjectVisualData3D::CObjectVisualData3D()
                   : projectionType( CSettings::EPT_PERSPECTIVE ),
                     specularShine(0),
                     specularIntensity(0),
                     alpha(false)
{
}   // constructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CObjectVisualData3D::LoadFromNode( const XMLNode & objectNode )
{
    XMLNode visualNode = objectNode.getChildNode( "visual" );

    if( !visualNode.isEmpty() )
    {
        // Get the file and alpha info
        if( visualNode.isAttributeSet("file") )
            file = visualNode.getAttribute( "file" );

        if( visualNode.isAttributeSet("alpha") )
            alpha = std::string( visualNode.getAttribute( "alpha" )) == "true";

        // Get the projection info
        XMLNode projectionNode = visualNode.getChildNode( "projection" );

        if( !projectionNode.isEmpty() )
        {
            if( std::string(projectionNode.getAttribute( "type" )) == "perspective" )
                projectionType = CSettings::EPT_PERSPECTIVE;

            else if( std::string(projectionNode.getAttribute( "type" )) == "orthographic" )
                projectionType = CSettings::EPT_ORTHOGRAPHIC;
        }

        // Get the shader info that must be defined
        XMLNode shaderNode = visualNode.getChildNode( "shader" );

        if( !shaderNode.isEmpty() )
        {
            effect = shaderNode.getAttribute( "effect" );
            technique = shaderNode.getAttribute( "technique" );
        }

        // Raise an exception if the shader effect or technique are not set
        if( effect.empty() || technique.empty() )
            throw NExcept::CCriticalException("Shader effect or techique not set!",
                boost::str( boost::format("Shader object data missing.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        // Get the specular info
        XMLNode specularNode = visualNode.getChildNode( "specular" );

        if( !specularNode.isEmpty() )
        {
            specularShine = atof(specularNode.getAttribute( "shine" ));
            specularIntensity = atof(specularNode.getAttribute( "intensity" ));
        }

        // Get the color info
        color = NParseHelper::LoadColor( visualNode );

        // Get the animation info
        XMLNode animationNode = visualNode.getChildNode( "animation" );

        if( !animationNode.isEmpty() )
        {
            animFile = animationNode.getAttribute( "file" );
            defaultAnimation = animationNode.getAttribute( "default" );
        }
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Get the file path
************************************************************************/
const std::string & CObjectVisualData3D::GetFile() const 
{
    return file;
}


/************************************************************************
*    desc:  Get the projection type
************************************************************************/
CSettings::EProjectionType CObjectVisualData3D::GetProjectionType() const
{
    return projectionType;
}


/************************************************************************
*    desc:  Get the name of the shading effect
************************************************************************/
const std::string & CObjectVisualData3D::GetEffect() const
{
    return effect;
}


/************************************************************************
*    desc:  Get the name of the shading technique
************************************************************************/
const std::string & CObjectVisualData3D::GetTechnique() const
{
    return technique;
}


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CObjectVisualData3D::GetColor() const 
{
    return color;
}


/************************************************************************
*    desc:  Get the specular shine
************************************************************************/
float CObjectVisualData3D::GetSpecularShine() const
{
    return specularShine;
}


/************************************************************************
*    desc:  Get the specular intensity
************************************************************************/
float CObjectVisualData3D::GetSpecularIntensity() const
{
    return specularIntensity;
}


/************************************************************************
*    desc:  Get the flag if uses alpha textures and require sorting
************************************************************************/
bool CObjectVisualData3D::IsAlpha() const
{
    return alpha;
}


/************************************************************************
*    desc:  Get the animated flag
************************************************************************/
bool CObjectVisualData3D::IsAnimated() const
{
    return !animFile.empty();
}


/************************************************************************
*    desc:  Get the path to the animation file
************************************************************************/
const std::string & CObjectVisualData3D::GetAnimFile() const
{
    return animFile;
}


/************************************************************************
*    desc:  Get the name of the default animation
************************************************************************/
const std::string & CObjectVisualData3D::GetDefaultAnim() const
{
    return defaultAnimation;
}