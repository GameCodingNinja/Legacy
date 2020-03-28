
/************************************************************************
*   FILE NAME:   scriptsetcolortodefault.h
*
*	SCRIPT CALL: SetColorToDefault
*
*	DESCRIPTION: Sets the color of the sprite to the object's default color
*
*	EXAMPLE:	 <SetColorToDefault r="" g="" b="" a=""/>
************************************************************************/

// Physical component dependency
#include <scripting/scriptsetcolortodefault.h>

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
CScriptSetColorToDefault::CScriptSetColorToDefault()
{
    type = EST_SET_COLOR_TO_DEFAULT;

}   // constructor

CScriptSetColorToDefault::CScriptSetColorToDefault( CScript * pCopy ) 
{
    bColor = *pCopy->GetBoolColor();
    type = EST_SET_COLOR_TO_DEFAULT;

}	// constructor

CScriptSetColorToDefault::CScriptSetColorToDefault( CScript & copy ) 
{
    bColor = *copy.GetBoolColor();
    type = EST_SET_COLOR_TO_DEFAULT;

}	// constructor


/************************************************************************
*    desc:  Update the script animation 
*  
*    param: CScriptObject * pScriptObj - object to animate
*
*	 ret:	bool - true if the script is finished, false if its not
************************************************************************/
bool CScriptSetColorToDefault::Update( CScriptObject * pScriptObj )
{
    // Set the color of the object
    if( !finished )
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

    return finished;

}	// Update


/************************************************************************
*    desc:  Set the individual components of the color to true
************************************************************************/
void CScriptSetColorToDefault::SetFlagR()
{ 
    bColor.r = true; 

}	// SetR

void CScriptSetColorToDefault::SetFlagG()
{ 
    bColor.g = true; 

}	// SetG

void CScriptSetColorToDefault::SetFlagB()
{ 
    bColor.b = true; 

}	// SetB

void CScriptSetColorToDefault::SetFlagA()
{  
    bColor.a = true; 

}	// SetA


/************************************************************************
*    desc:  Get a pointer to the boolean color 
*
*	 ret:	CBoolColor * - boolean color
************************************************************************/
CBoolColor * CScriptSetColorToDefault::GetBoolColor()
{
    return &bColor;

}	// GetBoolColor


