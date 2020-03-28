
/************************************************************************
*    FILE NAME:       scriptobject.cpp
*
*    DESCRIPTION:     Object to handle script actions
************************************************************************/

// Physical component dependency
#include <scripting/scriptobject.h>

// Game lib dependencies
#include <scripting/commandscript.h>
#include <scripting/command.h>
#include <utilities/highresolutiontimer.h>

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
CScriptObject::CScriptObject()
             : pActiveCommand(NULL),
               stepCounter(0),
               timer(0),
               animating(false)
{
}   // constructor


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CScriptObject::~CScriptObject()
{
}   // Destructer


/****************************************************************************
*    desc:	Update the script object
****************************************************************************/
void CScriptObject::Update()
{
    // If there is an active command and the sprite is animating, update the sprite
    if( animating && pActiveCommand != NULL )
    {
        // Increment timers
        IncTimer( CHighResTimer::Instance().GetElapsedTime() );
        IncFrameTimer( CHighResTimer::Instance().GetElapsedTime() );

        // Update the active command
        pActiveCommand->Update( this );
    }

}	// Update


/************************************************************************
*    desc:  Set the command script  
*
*	 param:	CCommandScript * pScript - script pointer to copy
************************************************************************/
void CScriptObject::SetCommandScript( CCommandScript * pScript )
{
    spCommandScript.reset( new CCommandScript(pScript) );

}	// SetCommandScript


/************************************************************************
*    desc:  Get the command script  
*
*	 ret:	CCommandScript * - command script
************************************************************************/
CCommandScript * CScriptObject::GetCommandScript()
{
    return spCommandScript.get();

}	// GetCommandScript


/************************************************************************
*    desc:  Set the active command
*  
*    param: string & commandName - new active command
************************************************************************/
void CScriptObject::SetActiveCommand( const string & commandName )
{
    // Providing the sprite has a script, enter the if
    if( spCommandScript != NULL )
    {
        CCommand * pCmd = spCommandScript->GetCommand( commandName );

        if( pCmd != NULL )
        {
            // Set as the active command
            pActiveCommand = pCmd;
            activeCommandName = commandName;
            
            // Reset some values
            SetTimer( 0 );
            SetFrameTimer( 0 );
            SetStepCounter( 0 );
            SetAnimating( true );
            pActiveCommand->Reset();
            ResetColor();
        }
    }

}	// SetActiveCommand


/************************************************************************
*    desc:  Get the active command  
*
*	 ret:	CCommand * - active command
************************************************************************/
CCommand * CScriptObject::GetActiveCommand()
{
    return pActiveCommand;

}	// GetActiveCommand


/************************************************************************
*    desc:  Get the name of the active command  
*
*	 ret:	const string & - active command name
************************************************************************/
const string & CScriptObject::GetActiveCommandName() const
{
    return activeCommandName;

}	// GetActiveCommandName


/************************************************************************
*    desc:  Get the animation end mode
*
*	 ret:	ECommandEndMode
************************************************************************/
ECommandEndMode CScriptObject::GetCommandEndMode() const
{
    if( pActiveCommand != NULL )
        return pActiveCommand->GetEndMode();

    return EANM_NULL;

}	// GetCommandEndMode


/************************************************************************
*    desc:  Is the end mode of the active command to loop
*
*	 ret:	bool - true or false
************************************************************************/
bool CScriptObject::IsEndModeLoop() const
{
    return ( GetCommandEndMode() == EANM_LOOP );

}	// IsEndModeLoop


/************************************************************************
*    desc:  Set the color
*  
*    param: const CColor & _color - color to set to
************************************************************************/
void CScriptObject::SetColor( const CColor & _color )
{
    color = _color;

}	// SetColor


/************************************************************************
*    desc:  Increment the color
*  
*    param: const CColor & _color - color to increment by
************************************************************************/
void CScriptObject::IncColor( const CColor & _color )
{
    color += _color;

}	// IncColor


/************************************************************************
*    desc:  Get the color  
*
*	 ret:	const CColor & - color of the object
************************************************************************/
const CColor & CScriptObject::GetColor() const
{
    return color;

}	// GetColor


/************************************************************************
*    desc:  Sets the color back to its starting color
*  
*    param: float value - time to set to
************************************************************************/
void CScriptObject::ResetColor()
{
    color = defaultColor;

}	// ResetColor


/************************************************************************
*    desc:  Set the default color
*  
*    param: const CColor & _color - color to set to
************************************************************************/
void CScriptObject::SetDefaultColor( const CColor & _color )
{
    defaultColor = _color;

}	// SetDefaultColor


/************************************************************************
*    desc:  Get the default color  
*
*	 ret:	const CColor & - default color of the object
************************************************************************/
const CColor & CScriptObject::GetDefaultColor() const
{
    return defaultColor;

}	// GetDefaultColor


/************************************************************************
*    desc:  Set the timer
*  
*    param: float value - time to set to
************************************************************************/
void CScriptObject::SetTimer( float value )
{
    timer = value;

}	// SetTimer


/************************************************************************
*    desc:  Increment the timer
*  
*    param: float value - time to increment by
************************************************************************/
void CScriptObject::IncTimer( float value )
{
    timer += value;

}	// IncTimer


/************************************************************************
*    desc:  Get the timer  
*
*	 ret:	float - time elapsed of the command
************************************************************************/
float CScriptObject::GetTimer() const
{
    return timer;

}	// GetTimer


// Set and get the animating flag
/************************************************************************
*    desc:  Set the animating flag
*  
*    param: bool value - value to set to
************************************************************************/
void CScriptObject::SetAnimating( bool value )
{
    animating = value;

}	// SetAnimating


/************************************************************************
*    desc:  Is the object animating
*
*	 ret:	bool - true or false
************************************************************************/
bool CScriptObject::IsAnimating() const
{
    return animating;

}	// IsAnimating


/************************************************************************
*    desc:  Set the current frame
*  
*    param: int frame - frame to set to
************************************************************************/
void CScriptObject::SetCurrentFrame( int frame )
{
    // Base does nothing

}	// SetCurrentFrame


/************************************************************************
*    desc:  Increment the current frame
*  
*    param: int frame - ammount to increment by
************************************************************************/
void CScriptObject::IncCurrentFrame( int frame )
{
    // Base does nothing

}	// SetCurrentFrame


/************************************************************************
*    desc:  Get the current frame  
*
*	 ret:	int - the current frame
************************************************************************/
int CScriptObject::GetCurrentFrame() const
{
    // Base returns zero
    return 0;

}	// GetCurrentFrame


/************************************************************************
*    desc:  Get frame count  
*
*	 ret:	int - number of frames in the object
************************************************************************/
int CScriptObject::GetFrameCount() const
{
    // Base returns zero
    return 0;

}	// GetFrameCount


/************************************************************************
*    desc:  Set the frame timer
*  
*    param: float value - time to set to
************************************************************************/
void CScriptObject::SetFrameTimer( float value )
{
    // Base does nothing

}	// SetFrameTimer


/************************************************************************
*    desc:  Increment the frame timer
*  
*    param: float value - time to increment by
************************************************************************/
void CScriptObject::IncFrameTimer( float value )
{
    // Base does nothing

}	// IncFrameTimer


/************************************************************************
*    desc:  Get the frame timer  
*
*	 ret:	float - time elapsed in the frame
************************************************************************/
float CScriptObject::GetFrameTimer() const
{
    // Base returns zero
    return 0;

}	// GetFrameTimer


/************************************************************************
*    desc:  Set the timer
*  
*    param: int value - value to set to
************************************************************************/
void CScriptObject::SetStepCounter( int value )
{
    stepCounter = value;

}	// SetTimer


/************************************************************************
*    desc:  Increment the timer
*  
*    param: int value - value to increment by
************************************************************************/
void CScriptObject::IncStepCounter( int value )
{
    stepCounter += value;

}	// IncTimer


/************************************************************************
*    desc:  Get the timer  
*
*	 ret:	int - current script step of the command
************************************************************************/
int CScriptObject::GetStepCounter() const
{
    return stepCounter;

}	// GetTimer