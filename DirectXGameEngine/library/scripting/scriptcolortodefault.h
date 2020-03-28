
/************************************************************************
*	FILE NAME:   scriptcolortodefault.h
*
*	SCRIPT CALL: ColorToDefault
*
*	DESCRIPTION: Using time - Gradually transitions the current color of a
*				 sprite to the default color within the specified amount 
*				 of time.
*
*				 Using speed - Gradually transitions the current color of
*				 a sprite to the default color at a specified speed.
*
*	EXAMPLE:	 <ColorToDefault method="time" time="2000" r="1" g="1" b="1" a="0"/>
*				 <ColorToDefault method="speed" speed="5" r="1" g="1" b="1" a="0"/>
*
*	NOTE:		 1000 time = 1 second
*				 
*				 1 speed = 1 color magnitude per second. The color magnitude
*				 is sqrt( (colorDifference.r * colorDifference.r) +
*						  (colorDifference.g * colorDifference.g) +
*						  (colorDifference.b * colorDifference.b) +
*						  (colorDifference.a * colorDifference.a) )
************************************************************************/

#ifndef __script_color_to_default_h__
#define __script_color_to_default_h__

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptobject.h>
#include <scripting/script.h>

class CScriptColorToDefault : public CScript
{
public:

    // Constructors
    CScriptColorToDefault();
    CScriptColorToDefault( CScript * pCopy );
    CScriptColorToDefault( CScript & copy );

    // Function to incrementally color to a specific color 
    void ColorToViaTime( CScriptObject * pScriptObj );

    // Update the script animation
    bool Update( CScriptObject * pScriptObj );

    // Reset the variables of the script
    void Reset();

    // Set and get the amount of time to transition from one
    // color to another
    void SetTime( float _time );
    float GetTime();

    // Set and get speed in which the color should change
    void SetSpeed( float _speed );
    float GetSpeed();

    // Set the individual components of the color boolean
    void SetFlagR();
    void SetFlagG();
    void SetFlagB();
    void SetFlagA();

    // Get a pointer to the boolean color
    CBoolColor * GetBoolColor();

private:

    CBoolColor bColor;
    CColor colorInc;
    float speed;
    float time;
    bool colorIncSet;

};

#endif  // __script_color_to_default_h__