/************************************************************************
*    FILE NAME:       command.h
*
*    DESCRIPTION:     Series of script steps executed one after another
************************************************************************/  

#ifndef __command_h__
#define __command_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <scripting/scriptdefs.h>
#include <xmlParser/xmlParser.h>

// Forward Declaration(s)
class CScriptStep;
class iScriptObject;
class CScriptObject;

class CCommand : public boost::noncopyable
{
public:

    CCommand();
    CCommand( CCommand * pCopy );
    ~CCommand();

    // Load the script from node
    void LoadFromNode( XMLNode & node );

    // Update the command
    //bool Update( iScriptObject * pScriptObj );
    bool Update( CScriptObject * pScriptObj );

    // Increment the step
    //void IncStep( iScriptObject * pScriptObj );
    void IncStep( CScriptObject * pScriptObj );

    // Reset all the scripts in the step
    void Reset();

    // Get the number of steps in a command
    size_t GetStepCount();

    // Get the Name of this command
    std::string & GetName();

    // Get the script end mode
    ECommandEndMode GetEndMode();

private:

    // Vector of script steps
    boost::ptr_vector<CScriptStep> spScriptStepVec;

    // What to do when the command is finished
    ECommandEndMode commandEndMode;

};

#endif	// __command_h__