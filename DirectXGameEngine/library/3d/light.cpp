/************************************************************************
*    FILE NAME:       light.cpp
*
*    DESCRIPTION:     3D light class
************************************************************************/

// Physical component dependency
#include <3d/light.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <xmlParser/xmlParser.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CLight::CLight( ELightType lightType )
      : type(lightType),
        enable(false),
        castShadow(false),
        radius(0.f),
        diffuse(1.f, 1.f, 1.f)
{
}   // Constructer


/************************************************************************
*    desc:  Load the light info from XML file
*  
*    param: std::string filePath - path to file
************************************************************************/
void CLight::LoadFromXML( XMLNode & lightNode )
{
    XMLNode node;

    // Is the light enabled
    if( string(lightNode.getAttribute("enable")) == string("yes") )
        SetEnable( true );

    // Does this light cast a shadow?
    if( lightNode.isAttributeSet("castShadow") &&
        (string(lightNode.getAttribute("castShadow")) == string("yes")) )
        SetCastShadow( true );

    // get a node to the position
    node = lightNode.getChildNode("position");

    // if available, set the position
    if( !node.isEmpty() )
        SetPos( CPoint( atof(node.getAttribute("x")),
                        atof(node.getAttribute("y")),
                        atof(node.getAttribute("z")) ) );

    // get a node to the defuse color
    node = lightNode.getChildNode("diffuse");

    // if available, set the defuse color
    if( !node.isEmpty() )
        SetDiffuse( CPoint( atof(node.getAttribute("r")),
                            atof(node.getAttribute("g")),
                            atof(node.getAttribute("b")) ) );

    // get a node to the direction
    node = lightNode.getChildNode("direction");

    // if available, set the direction
    if( !node.isEmpty() )
    {
        SetDirection( CPoint( atof(node.getAttribute("x")),
                              atof(node.getAttribute("y")),
                              atof(node.getAttribute("z")) ) );

        GetDirection().Normalize();
        GetDirection().FlipSigns();
    }

    // get a node to the rotation
    node = lightNode.getChildNode("rotation");

    // if available, set the properties
    if( !node.isEmpty() )
        SetRotation( CPoint( atof(node.getAttribute("x")),
                             atof(node.getAttribute("y")),
                             atof(node.getAttribute("z")) ) );

    // get a node to the properties
    node = lightNode.getChildNode("properties");

    // if available, set the properties
    if( !node.isEmpty() )
        SetRadius( atof(node.getAttribute("radius")) );

}	// LoadFromXML


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CLight::~CLight()
{
}   // Destructor


/************************************************************************
*    desc:  Set enable
************************************************************************/
void CLight::SetEnable( bool value )
{
    enable = value;
}


/************************************************************************
*    desc:  Get enable
************************************************************************/
bool CLight::GetEnable()
{
    return enable;
}


/************************************************************************
*    desc:  Set the light type
************************************************************************/
void CLight::SetType( ELightType value )
{
    type = value;
}


/************************************************************************
*    desc:  Get the light type
************************************************************************/
ELightType CLight::GetType()
{
    return type;
}


/************************************************************************
*    desc:  Set the cast shadow
************************************************************************/
void CLight::SetCastShadow( bool value )
{
    castShadow = value;
}


/************************************************************************
*    desc:  Get the cast shadow
************************************************************************/
bool CLight::GetCastShadow()
{
    return castShadow;
}


/************************************************************************
*    desc:  Set the diffuse
************************************************************************/
void CLight::SetDiffuse( CPoint & value )
{
    diffuse = value;
}


/************************************************************************
*    desc:  Get the diffuse
************************************************************************/
CPoint & CLight::GetDiffuse()
{
    return diffuse;
}


/************************************************************************
*    desc:  Set the direction
************************************************************************/
void CLight::SetDirection( CPoint & value )
{
    direction = value;
}


/************************************************************************
*    desc:  Get the direction
************************************************************************/
CPoint & CLight::GetDirection()
{
    return direction;
}


/************************************************************************
*    desc:  Set the radius
************************************************************************/
void CLight::SetRadius( float value )
{
    radius = value;
}


/************************************************************************
*    desc:  Get the radius
************************************************************************/
float CLight::GetRadius()
{
    return radius;
}


/************************************************************************
*    desc:  Set the rotation
************************************************************************/
void CLight::SetRotation( CPoint & value )
{
    rotation = value;
}


/************************************************************************
*    desc:  Get the rotation
************************************************************************/
CPoint & CLight::GetRotation()
{
    return rotation;
}