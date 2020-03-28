
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

// Physical component dependency
#include <scripting/scriptcolor.h>

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
CScriptColor::CScriptColor() 
            : speed(0),
              time(0),
              colorIncSet(false)
{ 
    type = EST_COLOR; 

}	// constructor

CScriptColor::CScriptColor( CScript * pCopy ) 
            : colorIncSet(false)
{ 
    color = *pCopy->GetColor();
    speed = pCopy->GetSpeed();
    time = pCopy->GetTime();
    type = EST_COLOR; 

}	// constructor

CScriptColor::CScriptColor( CScript & copy ) 
            : colorIncSet(false)
{ 
    color = *copy.GetColor();
    speed = copy.GetSpeed();
    time = copy.GetTime();
    type = EST_COLOR; 

}	// constructor


/************************************************************************
*    desc:  Function to incrementally color to a specific color 
*  
*    param: CScriptObject * pScriptObj - object to color
************************************************************************/
void CScriptColor::ColorToViaTime( CScriptObject * pScriptObj )
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
            pScriptObj->SetColor( finalColor );
            finished = true;
        }
    }
    // Set the incremental color value. This is how much the color will
    // increment per millisecond
    else
    {
        finalColor = color + pScriptObj->GetColor();
        colorInc = color / time;

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
bool CScriptColor::Update( CScriptObject * pScriptObj )
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
void CScriptColor::Reset()
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
void CScriptColor::SetTime( float _time )
{
    time = _time;

}	// SetTime


/************************************************************************
*    desc:  Get the amount of time to transition from one color to another
*
*	 ret:	float - time
************************************************************************/
float CScriptColor::GetTime()
{
    return time;

}	// GetTime


/************************************************************************
*    desc:  Set the speed in which the color should change
*
*	 param:	float _time - value to set
************************************************************************/
void CScriptColor::SetSpeed( float _speed )
{
    speed = _speed;

}	// SetSpeed


/************************************************************************
*    desc:  Get the speed in which the color should change
*
*	 param:	float - speed
************************************************************************/
float CScriptColor::GetSpeed()
{
    return speed;

}	// GetSpeed


/************************************************************************
*    desc:  Set the individual components of the color 
*  
*    param: float r/g/b/a - value to set
************************************************************************/
void CScriptColor::SetR( float r )
{ 
    color.r = r;  

}	// SetR

void CScriptColor::SetG( float g )
{ 
    color.g = g;  

}	// SetG

void CScriptColor::SetB( float b )
{ 
    color.b = b;  

}	// SetB

void CScriptColor::SetA( float a )
{ 
    color.a = a; 

}	// SetA


/************************************************************************
*    desc:  Get a pointer to the color 
*
*	 ret:	CColor * - color
************************************************************************/
CColor * CScriptColor::GetColor()
{
    return &color;

}	// GetColor