
/************************************************************************
*   FILE NAME:   scriptsetcolortodefault.h
*
*	SCRIPT CALL: SetColorToDefault
*
*	DESCRIPTION: Sets the color of the sprite to the object's default color
*
*	EXAMPLE:	 <SetColorToDefault r="" g="" b="" a=""/>
************************************************************************/

#ifndef __script_set_color_to_default_h__
#define __script_set_color_to_default_h__

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptobject.h>
#include <scripting/script.h>

class CScriptSetColorToDefault : public CScript
{
public:

    // Constructors
    CScriptSetColorToDefault();
    CScriptSetColorToDefault( CScript * pCopy );
    CScriptSetColorToDefault( CScript & copy );

    // Update the script animation
    bool Update( CScriptObject * pScriptObj );

    // Set the individual components of the color boolean
    void SetFlagR();
    void SetFlagG();
    void SetFlagB();
    void SetFlagA();

    // Get a pointer to the boolean color
    CBoolColor * GetBoolColor();

private:

    CBoolColor bColor;

};

#endif  // __script_set_color_to_default_h__