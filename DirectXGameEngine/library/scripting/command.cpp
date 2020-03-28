/************************************************************************
*    FILE NAME:       command.cpp
*
*    DESCRIPTION:     Series of script steps executed one after another
************************************************************************/

// Physical component dependency
#include <scripting/command.h>

// Standard lib dependencies
#include <string>

// Game lib dependency
#include <scripting/scriptstep.h>
//#include <scripting/iscriptobject.h>
#include <scripting/scriptobject.h>

// Required namespace(s)
using namespace std;

// Disables the signed/unsigned mismatch warning
#pragma warning(disable : 4018)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCommand::CCommand()
        : commandEndMode(EANM_STOP)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCommand::~CCommand()
{
}   // Destructer


/************************************************************************
*    desc:  Load the script from node
*  
*    param: node
************************************************************************/
void CCommand::LoadFromNode( XMLNode & node )
{
    string endMode = node.getAttribute("endMode");

    commandEndMode = EANM_STOP;

    // Set end mode
    if( endMode == "loop" )
        commandEndMode = EANM_LOOP;

    else if( endMode == "hide" )
        commandEndMode = EANM_HIDE;

    else if( endMode == "ping pong" )
        commandEndMode = EANM_PING_PONG;

    // Get the number of steps in this commands 
    int stepCount = node.nChildNode("scriptStep");

    spScriptStepVec.reserve( stepCount );

    for( int i = 0; i < stepCount; ++i )
    {
        XMLNode stepNode = node.getChildNode("scriptStep", i);

        spScriptStepVec.push_back( new CScriptStep() );

        spScriptStepVec.back().LoadFromNode( stepNode );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Copy Constructer                                                             
************************************************************************/
CCommand::CCommand( CCommand * pCopy )
{
    commandEndMode = pCopy->commandEndMode;

    spScriptStepVec.reserve( pCopy->spScriptStepVec.size() );

    for( size_t i = 0; i < pCopy->spScriptStepVec.size(); ++i )
    {
        CScriptStep * pTmpStep = new CScriptStep( &pCopy->spScriptStepVec[i] );
        spScriptStepVec.push_back( pTmpStep );
    }

}   // Copy Constructer


/************************************************************************
*    desc:  Update the command
*
*    param: pScriptObj - script object to update
*
*	 ret:	bool - command finished or about to begin another loop
************************************************************************/
bool CCommand::Update( CScriptObject * pScriptObj )
{
    // Counter to keep track of what step is currently being executed
    int stepCounter = pScriptObj->GetStepCounter();

    // Update if the step counter is less than the number of steps
    if( stepCounter < spScriptStepVec.size() )
    {
        // If the script step's update function returns true, then it's
        // time to move on to the next step of the command
        if( spScriptStepVec[stepCounter].Update( pScriptObj ) )
        {
            IncStep( pScriptObj );
        }
    }
    // If the step counter is greater than or equal to the number of steps,
    // perform the specified end mode for the command
    else if( stepCounter >= spScriptStepVec.size() )
    {
        if( commandEndMode == EANM_LOOP )
        {
            pScriptObj->SetStepCounter( 0 );
            Reset();

            return true;
        }
        else if( commandEndMode == EANM_STOP )
        {
            pScriptObj->SetAnimating( false );

            return true;
        }
        else if( commandEndMode == EANM_HIDE )
        {
            pScriptObj->SetVisible( false );
            pScriptObj->SetAnimating( false );

            return true;
        }
    }

    return false;

}	// Update


/************************************************************************
*    desc:  Values to reset after a step has been finished
*
*    param: CScriptObject * pScriptObj - script object to reset
************************************************************************/
void CCommand::IncStep( CScriptObject * pScriptObj )
{
    pScriptObj->IncStepCounter( 1 );
    pScriptObj->SetTimer( 0 );
    pScriptObj->SetFrameTimer( 0 );

}	// Reset


/************************************************************************
*    desc:  Reset the script steps
************************************************************************/
void CCommand::Reset()
{
    for( size_t i = 0; i < spScriptStepVec.size(); ++i )
        spScriptStepVec[i].Reset();

}	// Reset

 
/************************************************************************
*    desc:  Get the number of steps in a command
*
*    ret:	int - number of script steps
************************************************************************/
size_t CCommand::GetStepCount()
{
    return spScriptStepVec.size();

}	// GetStepCount


/************************************************************************
*    desc:  Get the script end mode
*
*    ret:	ECommandEndMode - 
************************************************************************/
ECommandEndMode CCommand::GetEndMode()
{
    return commandEndMode;

}	// GetEndMode
