
/************************************************************************
*	FILE NAME:   scriptcolorto.cpp
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

// Physical component dependency
#include <scripting/scriptcolorto.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructors
************************************************************************/
CScriptColorTo::CScriptColorTo() 
              : speed(0),
                time(0),
                colorIncSet(false)
{ 
    type = EST_COLOR_TO; 

}	// constructor

CScriptColorTo::CScriptColorTo( CScript * pCopy ) 
              : colorIncSet(false)
{ 
    color = *pCopy->GetColor();
    bColor = *pCopy->GetBoolColor();
    speed = pCopy->GetSpeed();
    time = pCopy->GetTime();
    type = EST_COLOR_TO; 

}	// constructor

CScriptColorTo::CScriptColorTo( CScript & copy ) 
               : colorIncSet(false)
{ 
    color = *copy.GetColor();
    bColor = *copy.GetBoolColor();
    speed = copy.GetSpeed();
    time = copy.GetTime();
    type = EST_COLOR_TO; 

}	// constructor


/************************************************************************
*    desc:  Function to incrementally color to a specific color 
*  
*    param: CScriptObject * pScriptObj - object to color
************************************************************************/
void CScriptColorTo::ColorToViaTime( CScriptObject * pScriptObj )
{
    // We only want to set the incremental color once, so if it's already set,
    // we enter here
    if( colorIncSet )
    {
        // If we're before the allotted time, increment the color
        if( pScriptObj->GetTimer() < time )
        {
            CColor tmpColor = colorInc * CHighResTimer::Instance().GetElapsedTime();
            pScriptObj->IncColor( tmpColor );
        }
        // If we're equal to or past the allotted time, set the color
        else
        {
            CColor newColor = pScriptObj->GetColor();

            if( bColor.r )
                newColor.r = color.r;

            if( bColor.g )
                newColor.g = color.g;

            if( bColor.b )
                newColor.b = color.b;

            if( bColor.a )
                newColor.a = color.a;

            pScriptObj->SetColor( newColor );
            finished = true;
        }
    }
    // Set the incremental color value. This is how much the color will
    // increment per millisecond
    else
    {
        colorInc = (color - pScriptObj->GetColor()) / time;

        if( !bColor.r )
            colorInc.r = 0;

        if( !bColor.g )
            colorInc.g = 0;

        if( !bColor.b )
            colorInc.b = 0;

        if( !bColor.a )
            colorInc.a = 0;

        colorIncSet = true;
    }

}	// ColorToViaTime


/************************************************************************
*    desc:  Update the script animation 
*  
*    param: CScriptObject * pScriptObj - object to animate
*
*	 ret:	bool - true if the script is finished, false if its not
************************************************************************/
bool CScriptColorTo::Update( CScriptObject * pScriptObj )
{
    // Increment the color of the object
    if( !finished )
    {
        if( time != 0 )
            ColorToViaTime( pScriptObj );
    }

    return finished;

}	// Update


/************************************************************************
*    desc:  Reset the variables of the script 
************************************************************************/
void CScriptColorTo::Reset()
{
    finished = false;
    colorIncSet = false;
    colorInc.Clear();

}	// Reset


/************************************************************************
*    desc:  Set the amount of time to transition from one color to another
*
*	 param:	float _time - value to set
************************************************************************/
void CScriptColorTo::SetTime( float _time )
{
    time = _time;

}	// SetTime


/************************************************************************
*    desc:  Get the amount of time to transition from one color to another
*
*	 ret:	float - time
************************************************************************/
float CScriptColorTo::GetTime()
{
    return time;

}	// GetTime


/************************************************************************
*    desc:  Set the speed in which the color should change
*
*	 param:	float _time - value to set
************************************************************************/
void CScriptColorTo::SetSpeed( float _speed )
{
    speed = _speed;

}	// SetSpeed


/************************************************************************
*    desc:  Get the speed in which the color should change
*
*	 param:	float - speed
************************************************************************/
float CScriptColorTo::GetSpeed()
{
    return speed;

}	// GetSpeed


/************************************************************************
*    desc:  Set the individual components of the color 
*  
*    param: float r/g/b/a - value to set
************************************************************************/
void CScriptColorTo::SetR( float r )
{ 
    color.r = r; 
    bColor.r = true; 

}	// SetR

void CScriptColorTo::SetG( float g )
{ 
    color.g = g; 
    bColor.g = true; 

}	// SetG

void CScriptColorTo::SetB( float b )
{ 
    color.b = b; 
    bColor.b = true; 

}	// SetB

void CScriptColorTo::SetA( float a )
{ 
    color.a = a; 
    bColor.a = true; 

}	// SetA


/************************************************************************
*    desc:  Get a pointer to the color 
*
*	 ret:	CColor * - color
************************************************************************/
CColor * CScriptColorTo::GetColor()
{
    return &color;

}	// GetColor


/************************************************************************
*    desc:  Get a pointer to the boolean color 
*
*	 ret:	CBoolColor * - boolean color
************************************************************************/
CBoolColor * CScriptColorTo::GetBoolColor()
{
    return &bColor;

}	// GetBoolColor