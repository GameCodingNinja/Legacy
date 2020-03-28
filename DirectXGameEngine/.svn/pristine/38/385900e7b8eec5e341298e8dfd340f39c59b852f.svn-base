
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

// Physical component dependency
#include <scripting/scriptsetcolor.h>

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
CScriptSetColor::CScriptSetColor()
{
    resetDefaultColor = false;
    type = EST_SET_COLOR;

}   // constructor

CScriptSetColor::CScriptSetColor( CScript * pCopy ) 
{
    color = *pCopy->GetColor();
    bColor = *pCopy->GetBoolColor();
    resetDefaultColor = pCopy->GetResetDefaultColor();

    type = EST_SET_COLOR;

}	// constructor

CScriptSetColor::CScriptSetColor( CScript & copy ) 
{
    color = *copy.GetColor();
    bColor = *copy.GetBoolColor();
    resetDefaultColor = copy.GetResetDefaultColor();

    type = EST_SET_COLOR; 

}	// constructor


/************************************************************************
*    desc:  Update the script animation 
*  
*    param: CScriptObject * pScriptObj - object to animate
*
*	 ret:	bool - true if the script is finished, false if its not
************************************************************************/
bool CScriptSetColor::Update( CScriptObject * pScriptObj )
{
    // Set the color of the object
    if( !finished )
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

        if( resetDefaultColor )
            pScriptObj->SetDefaultColor( newColor );

        finished = true;
    }

    return finished;

}	// Update


/************************************************************************
*    desc:  Set the individual components of the color 
*  
*    param: float r/g/b/a - value to set
************************************************************************/
void CScriptSetColor::SetR( float r )
{ 
    color.r = r; 
    bColor.r = true; 

}	// SetR

void CScriptSetColor::SetG( float g )
{ 
    color.g = g; 
    bColor.g = true; 

}	// SetG

void CScriptSetColor::SetB( float b )
{ 
    color.b = b; 
    bColor.b = true; 

}	// SetB

void CScriptSetColor::SetA( float a )
{ 
    color.a = a; 
    bColor.a = true; 

}	// SetA


/************************************************************************
*    desc:  Set the reset default color flag to true 
************************************************************************/
void CScriptSetColor::ResetDefaultColor()
{ 
    resetDefaultColor = true; 

}	// ResetDefaultColor


/************************************************************************
*    desc:  Get the reset default color flag
*
*	 ret:	bool - reset default color flag
************************************************************************/
bool CScriptSetColor::GetResetDefaultColor()
{
    return resetDefaultColor;

}	// GetResetDefaultColor


/************************************************************************
*    desc:  Get a pointer to the color 
*
*	 ret:	CColor * - color
************************************************************************/
CColor * CScriptSetColor::GetColor()
{
    return &color;

}	// GetColor


/************************************************************************
*    desc:  Get a pointer to the boolean color 
*
*	 ret:	CBoolColor * - boolean color
************************************************************************/
CBoolColor * CScriptSetColor::GetBoolColor()
{
    return &bColor;

}	// GetBoolColor


