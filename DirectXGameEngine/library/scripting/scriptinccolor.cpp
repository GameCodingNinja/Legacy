
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

// Physical component dependency
#include <scripting/scriptinccolor.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer
************************************************************************/
CScriptIncColor::CScriptIncColor()
{ 
    type = EST_INC_COLOR; 

}	// constructor

CScriptIncColor::CScriptIncColor( CScript * pCopy )
{ 
    color = *pCopy->GetColor();
    bColor = *pCopy->GetBoolColor();

    type = EST_INC_COLOR; 

}	// constructor

CScriptIncColor::CScriptIncColor( CScript & copy )
{ 
    color = *copy.GetColor();
    bColor = *copy.GetBoolColor();

    type = EST_INC_COLOR; 

}	// constructor


/************************************************************************
*    desc:  Update the script animation 
*  
*    param: CScriptObject * pScriptObj - object to animate
*
*	 ret:	bool - true if the script is finished, false if its not
************************************************************************/
bool CScriptIncColor::Update( CScriptObject * pScriptObj )
{
    // Increment the color of the object
    if( !finished )
    {
        CColor newColor(0,0,0,0);

        if( bColor.r )
            newColor.r = color.r;

        if( bColor.g )
            newColor.g = color.g;

        if( bColor.b )
            newColor.b = color.b;

        if( bColor.a )
            newColor.a = color.a;

        pScriptObj->IncColor( newColor );
        finished = true;
    }

    return finished;

}	// Update


/************************************************************************
*    desc:  Set the individual components of the color 
*  
*    param: float r/g/b/a - value to set
************************************************************************/
void CScriptIncColor::SetR( float r )
{ 
    color.r = r; 
    bColor.r = true; 

}	// SetR

void CScriptIncColor::SetG( float g )
{ 
    color.g = g; 
    bColor.g = true; 

}	// SetG

void CScriptIncColor::SetB( float b )
{ 
    color.b = b; 
    bColor.b = true; 

}	// SetB

void CScriptIncColor::SetA( float a )
{ 
    color.a = a; 
    bColor.a = true; 

}	// SetA


/************************************************************************
*    desc:  Get a pointer to the color 
*
*	 ret:	CColor * - color
************************************************************************/
CColor * CScriptIncColor::GetColor()
{
    return &color;

}	// GetColor


/************************************************************************
*    desc:  Get a pointer to the boolean color 
*
*	 ret:	CBoolColor * - boolean color
************************************************************************/
CBoolColor * CScriptIncColor::GetBoolColor()
{
    return &bColor;

}	// GetBoolColor