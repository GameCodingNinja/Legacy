
/************************************************************************
*   FILE NAME:   scriptsetcolor.h
*
*	SCRIPT CALL: SetColor
*
*	DESCRIPTION: Sets the color of the sprite to the color designated in
*				 the script call
*
*	EXAMPLE:	 <SetColor r="1" g="1" b="1" a="1"/>
************************************************************************/

#ifndef __script_set_color_h__
#define __script_set_color_h__

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptobject.h>
#include <scripting/script.h>

class CScriptSetColor : public CScript
{
public:

    // Constructors
    CScriptSetColor();
    CScriptSetColor( CScript * pCopy );
    CScriptSetColor( CScript & copy );

    // Update the script animation
    bool Update( CScriptObject * pScriptObj );

    // Set the individual components of the color
    void SetR( float r );
    void SetG( float g );
    void SetB( float b );
    void SetA( float a );

    // Set and get the reset default color flag
    void ResetDefaultColor();
    bool GetResetDefaultColor();

    // Get a pointer to the color and boolean color
    CColor * GetColor();
    CBoolColor * GetBoolColor();

private:

    CColor color;
    CBoolColor bColor;
    bool resetDefaultColor;

};

#endif  // __script_set_color_h__