
/************************************************************************
*   FILE NAME:   scriptinccolor.h
*
*	SCRIPT CALL: IncColor
*
*	DESCRIPTION: Increments the color of the sprite by the designated 
*				 amount in the script call
*
*	EXAMPLE:	 <IncColor r="0.1" g="0.1" b="-0.3" a="-0.15"/>
************************************************************************/

#ifndef __script_inc_color_h__
#define __script_inc_color_h__

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptobject.h>
#include <scripting/script.h>

class CScriptIncColor : public CScript
{
public:

    // Constructors
    CScriptIncColor();
    CScriptIncColor( CScript * pCopy );
    CScriptIncColor( CScript & copy );

    // Update the script animation
    bool Update( CScriptObject * pScriptObj );

    // Set the individual components of the color
    void SetR( float r );
    void SetG( float g );
    void SetB( float b );
    void SetA( float a );

    // Get a pointer to the color and boolean color
    CColor * GetColor();
    CBoolColor * GetBoolColor();

private:

    CColor color;
    CBoolColor bColor;
};

#endif  // __script_inc_color_h__