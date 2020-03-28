
/************************************************************************
*	FILE NAME:   scriptcolorto.h
*
*	SCRIPT CALL: ColorTo
*
*	DESCRIPTION: Using time - Gradually transitions the current 
*				 color of a sprite to another color within the specified
*				 amount of time.
*
*				 Using speed - Gradually transitions the current color of
*				 a sprite to another color at a specified speed.
*
*	EXAMPLE:	 <ColorTo method="time" time="2000" r="1" g="1" b="1" a="0"/>
*				 <ColorTo method="speed" speed="5" r="1" g="1" b="1" a="0"/>
*
*	NOTE:		 1000 time = 1 second
*				 
*				 1 speed = 1 color magnitude per second. The color magnitude
*				 is sqrt( (colorDifference.r * colorDifference.r) +
*						  (colorDifference.g * colorDifference.g) +
*						  (colorDifference.b * colorDifference.b) +
*						  (colorDifference.a * colorDifference.a) )
************************************************************************/

#ifndef __script_color_to_h__
#define __script_color_to_h__

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptobject.h>
#include <scripting/script.h>

class CScriptColorTo : public CScript
{
public:

    // Constructors
    CScriptColorTo();
    CScriptColorTo( CScript * pCopy );
    CScriptColorTo( CScript & copy );

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
    CColor colorInc;
    float speed;
    float time;
    bool colorIncSet;

};

#endif  // __script_color_to_h__