
/************************************************************************
*    FILE NAME:       objectshaderdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's shader data
************************************************************************/

#ifndef __object_shader_data_2d_h__
#define __object_shader_data_2d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/color.h>
#include <scripting/iobjectshaderdata.h>

class CObjectShaderData2D// : public iObjectShaderData
{
public:
    
    CObjectShaderData2D(){}
        
    // Set and get the name of the shading technique
    void SetTechnique(const std::string & tech){technique = tech;}
    const std::string & GetTechnique() const {return technique;}

    // Set and get the name of the shading effect
    void SetEffect(const std::string & eff){effect = eff;}
    const std::string & GetEffect() const {return effect;}

    // Set, get, and increment the color
    void SetColor( const CColor & _color ){color = _color;}
    void IncColor( const CColor & _color ){color += _color;}
    const CColor & GetColor() const {return color;}

    // Set and get the default color
    void SetDefaultColor( const CColor & _color ){defaultColor = _color;}
    const CColor & GetDefaultColor() const {return defaultColor;}

    // Reset the color back to its default color
    void ResetColor(){color = defaultColor;}

private:

    // Name of the shader technique and effect
    std::string technique, effect;		

    // Material color adjustment and its default
    CColor color, defaultColor;			

};

#endif  // __object_shader_data_2d_h__