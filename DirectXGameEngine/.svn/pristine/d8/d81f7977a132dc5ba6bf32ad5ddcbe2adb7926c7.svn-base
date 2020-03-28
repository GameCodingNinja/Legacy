/************************************************************************
*    FILE NAME:       iobjectshaderdata.h
*
*    DESCRIPTION:     Script object shader data interface class
************************************************************************/  

#ifndef __i_object_shader_data_h__
#define __i_object_shader_data_h__

// Game lib dependencies
#include <common/color.h>

// The class used as an interface for the other script classes
class iObjectShaderData
{
public:

    iObjectShaderData(){};
    virtual ~iObjectShaderData(){};

    virtual void SetColor( const CColor & _color ) = 0;
    virtual void IncColor( const CColor & _color ) = 0;
    virtual void SetDefaultColor( const CColor & _color ) = 0;
    virtual const CColor & GetDefaultColor() const = 0;
    virtual const CColor & GetColor() const = 0;

};

#endif	// __i_object_shader_data_h__