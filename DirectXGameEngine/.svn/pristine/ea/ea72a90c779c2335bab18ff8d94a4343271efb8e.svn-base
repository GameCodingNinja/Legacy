/************************************************************************
*    FILE NAME:       iscriptobject.h
*
*    DESCRIPTION:     Script object interface class
************************************************************************/  

#ifndef __i_script_object_h__
#define __i_script_object_h__

// Game lib dependencies
#include <scripting/iobjectanimationdata.h>
#include <scripting/iobjectshaderdata.h>
#include <common/color.h>
#include <common/object.h>
#include <2d/visualsprite2d.h>
#include <2d/collisionsprite2d.h>

// Disable the warning about return local variables
#pragma warning (disable : 4172 )

// The class used as an interface for the other script classes
class iScriptObject
{
public:

    iScriptObject(){};
    virtual ~iScriptObject(){};

    // Get the animation data
    //virtual iObjectAnimationData & GetAnimationData() = 0;

    // Get the lighting data
    //virtual iObjectShaderData & GetShaderData() = 0;

    // Set the visible property
    virtual void SetVisible( bool visible ) = 0;

    // Set the color property
    virtual void SetMaterialColor( const CColor & color ) = 0;

    // Get the visible property
    virtual int GetFrameCount() const = 0;

    // Set-Get the object position
    virtual void SetPos( CPoint & position ){}
    virtual void SetPos( CPointInt & position ){}
    virtual void SetPos( CWorldPoint & position ){}
    virtual const CWorldPoint & GetPos() const {return CWorldPoint();}
    virtual CWorldPoint & GetPos() {return CWorldPoint();}

    // Increment the object position
    virtual void IncPos( CPoint & position ){}
    virtual void IncPos( CPointInt & position ){}
    virtual void IncPos( CWorldPoint & position ){}

};

#endif	// __i_script_object_h__