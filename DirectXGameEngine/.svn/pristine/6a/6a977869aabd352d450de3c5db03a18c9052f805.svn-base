
/************************************************************************
*    FILE NAME:       objectshadowdata3d.cpp
*
*    DESCRIPTION:     Class containing the 3D object's shadow data
************************************************************************/

// Physical component dependency
#include <3d/objectshadowdata3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <common/color.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CObjectShadowData3D::CObjectShadowData3D()
                   : projectionType( CSettings::EPT_PERSPECTIVE ),
                     minZDist(0),
                     maxZDist(0),
                     bias(0),
                     orthoScreenPercent(0),
                     castShadow(false)
{
}   // constructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CObjectShadowData3D::LoadFromNode( const XMLNode & objectNode )
{
    XMLNode shadowNode = objectNode.getChildNode( "shadow" );

    if( !shadowNode.isEmpty() )
    {
        // Get the file info
        if( shadowNode.isAttributeSet("file") )
            file = shadowNode.getAttribute( "file" );

        if( shadowNode.isAttributeSet("castShadow") )
            castShadow = std::string(shadowNode.getAttribute( "castShadow" )) == "true";

        // Get the shader info that must be defined
        XMLNode shaderNode = shadowNode.getChildNode( "shader" );

        if( !shaderNode.isEmpty() )
        {
            effect = shaderNode.getAttribute( "effect" );
            technique = shaderNode.getAttribute( "technique" );
        }

        // Raise an exception if the shader effect or technique are not set
        if( effect.empty() || technique.empty() )
            throw NExcept::CCriticalException("Shader effect or techique not set!",
                boost::str( boost::format("Shader object data missing.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        // Get the projection info
        XMLNode projectionNode = shadowNode.getChildNode( "projection" );

        if( !projectionNode.isEmpty() )
        {
            if( std::string(projectionNode.getAttribute( "type" )) == "perspective" )
                projectionType = CSettings::EPT_PERSPECTIVE;

            else if( std::string(projectionNode.getAttribute( "type" )) == "orthographic" )
                projectionType = CSettings::EPT_ORTHOGRAPHIC;

            if( projectionNode.isAttributeSet("minZDist") )
                minZDist = atof(projectionNode.getAttribute( "minZDist" ));

            if( projectionNode.isAttributeSet("maxZDist") )
                maxZDist = atof(projectionNode.getAttribute( "maxZDist" ));

            if( projectionNode.isAttributeSet("bias") )
                bias = atof(projectionNode.getAttribute( "bias" ));

            if( projectionNode.isAttributeSet("orthoScreenPercent") )
                orthoScreenPercent = atof(projectionNode.getAttribute( "orthoScreenPercent" ));
        }
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Get the file path
************************************************************************/
const std::string & CObjectShadowData3D::GetFile() const 
{
    return file;
}


/************************************************************************
*    desc:  Get the projection type
************************************************************************/
CSettings::EProjectionType CObjectShadowData3D::GetProjectionType() const
{
    return projectionType;
}


/************************************************************************
*    desc:  Get the name of the shading effect
************************************************************************/
const std::string & CObjectShadowData3D::GetEffect() const
{
    return effect;
}


/************************************************************************
*    desc:  Get the name of the shading technique
************************************************************************/
const std::string & CObjectShadowData3D::GetTechnique() const
{
    return technique;
}


/************************************************************************
*    desc:  Get the minimum z
************************************************************************/
float CObjectShadowData3D::GetMinZDist() const
{
    return minZDist;
}


/************************************************************************
*    desc:  Get the maximum z
************************************************************************/
float CObjectShadowData3D::GetMaxZDist() const 
{
    return maxZDist;
}


/************************************************************************
*    desc:  Get the bias
************************************************************************/
float CObjectShadowData3D::GetBias() const 
{
    return bias;
}


/************************************************************************
*    desc:  Get the orthographic screen percentage
************************************************************************/
float CObjectShadowData3D::GetOrthoScrnPercent() const 
{
    return orthoScreenPercent;
}


/************************************************************************
*    desc:  Get the shadow flag
************************************************************************/
bool CObjectShadowData3D::GetCastShadow() const 
{
    return castShadow;
}
