
/************************************************************************
*	FILE NAME:   scriptcolortodefault.cpp
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

// Physical component dependency
#include <scripting/scriptcolortodefault.h>

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
CScriptColorToDefault::CScriptColorToDefault() 
                     : speed(0),
                       time(0),
                       colorIncSet(false)
{ 
    type = EST_COLOR_TO_DEFAULT; 

}	// constructor

CScriptColorToDefault::CScriptColorToDefault( CScript * pCopy ) 
                     : colorIncSet(false)
{ 
    bColor = *pCopy->GetBoolColor();
    speed = pCopy->GetSpeed();
    time = pCopy->GetTime();
    type = EST_COLOR_TO_DEFAULT;

}	// constructor

CScriptColorToDefault::CScriptColorToDefault( CScript & copy ) 
                     : colorIncSet(false)
{ 
    bColor = *copy.GetBoolColor();
    speed = copy.GetSpeed();
    time = copy.GetTime();
    type = EST_COLOR_TO_DEFAULT;

}	// constructor


/************************************************************************
*    desc:  Function to incrementally color to a specific color 
*  
*    param: CScriptObject * pScriptObj - object to color
************************************************************************/
void CScriptColorToDefault::ColorToViaTime( CScriptObject * pScriptObj )
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
                newColor.r = pScriptObj->GetDefaultColor().r;

            if( bColor.g )
                newColor.g = pScriptObj->GetDefaultColor().g;

            if( bColor.b )
                newColor.b = pScriptObj->GetDefaultColor().b;

            if( bColor.a )
                newColor.a = pScriptObj->GetDefaultColor().a;

            pScriptObj->SetColor( newColor );
            finished = true;
        }
    }
    // Set the incremental color value. This is how much the color will
    // increment per millisecond
    else
    {
        colorInc = (pScriptObj->GetDefaultColor() - pScriptObj->GetColor()) / time;

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
bool CScriptColorToDefault::Update( CScriptObject * pScriptObj )
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
void CScriptColorToDefault::Reset()
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
void CScriptColorToDefault::SetTime( float _time )
{
    time = _time;

}	// SetTime


/************************************************************************
*    desc:  Get the amount of time to transition from one color to another
*
*	 ret:	float - time
************************************************************************/
float CScriptColorToDefault::GetTime()
{
    return time;

}	// GetTime


/************************************************************************
*    desc:  Set the speed in which the color should change
*
*	 param:	float _time - value to set
************************************************************************/
void CScriptColorToDefault::SetSpeed( float _speed )
{
    speed = _speed;

}	// SetSpeed


/************************************************************************
*    desc:  Get the speed in which the color should change
*
*	 param:	float - speed
************************************************************************/
float CScriptColorToDefault::GetSpeed()
{
    return speed;

}	// GetSpeed


/************************************************************************
*    desc:  Set the individual components of the color to true
************************************************************************/
void CScriptColorToDefault::SetFlagR()
{ 
    bColor.r = true; 

}	// SetFlagR

void CScriptColorToDefault::SetFlagG()
{ 
    bColor.g = true; 

}	// SetFlagG

void CScriptColorToDefault::SetFlagB()
{ 
    bColor.b = true; 

}	// SetFlagB

void CScriptColorToDefault::SetFlagA()
{  
    bColor.a = true; 

}	// SetFlagA


/************************************************************************
*    desc:  Get a pointer to the boolean color 
*
*	 ret:	CBoolColor * - boolean color
************************************************************************/
CBoolColor * CScriptColorToDefault::GetBoolColor()
{
    return &bColor;

}	// GetBoolColor