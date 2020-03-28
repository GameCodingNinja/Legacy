
/************************************************************************
*	FILE NAME:   scriptcolor.h
*
*	SCRIPT CALL: Color
*
*	DESCRIPTION: Using time - Gradually transitions the current 
*				 color of a sprite to another color, relative to the starting
*				 color, within the specified amount of time.
*
*				 Using speed - Gradually transitions the current color of
*				 a sprite to another color, relative to the starting color,
*				 at a specified speed.
*
*	EXAMPLE:	 <Color method="time" time="2000" r="0" g="0.2" b="-0.2" a="0"/>
*				 <Color method="speed" speed="5" r="0" g="0.2" b="-0.2" a="0"/>
*
*	NOTE:		 1000 time = 1 second
*				 
*				 1 speed = 1 color magnitude per second. The color magnitude
*				 is sqrt( (colorDifference.r * colorDifference.r) +
*						  (colorDifference.g * colorDifference.g) +
*						  (colorDifference.b * colorDifference.b) +
*						  (colorDifference.a * colorDifference.a) )
************************************************************************/

#ifndef __script_color_h__
#define __script_color_h__

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptobject.h>
#include <scripting/script.h>

class CScriptColor : public CScript
{
public:

    // Constructors
    CScriptColor();
    CScriptColor( CScript * pCopy );
    CScriptColor( CScript & copy );

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

private:

    CColor color;
    CColor colorInc;
    CColor finalColor;
    float speed;
    float time;
    bool colorIncSet;

};


#endif  // __script_color_h__