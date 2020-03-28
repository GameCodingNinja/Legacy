/************************************************************************
*    FILE NAME:       scriptstep.h
*
*    DESCRIPTION:     Class used to precess certain script actions at 
*					  the same time
************************************************************************/  

#ifndef __script_step_h__
#define __script_step_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>

// Forward Declaration(s)
class CScript;
//class iScriptObject;
class CScriptObject;

class CScriptStep : public boost::noncopyable
{
public:

    CScriptStep();
    CScriptStep( CScriptStep * pCopy );
    ~CScriptStep();

    // Load the script from node
    void LoadFromNode( XMLNode & node );

    // Update the script step
    //bool Update( iScriptObject * pScriptObj );
    bool Update( CScriptObject * pScriptObj );

    // Reset the scripts in the step
    void Reset();

private:

    // Vector of scripts within the step
    boost::ptr_vector<CScript> spScriptVec;

};

#endif	// __script_step_h__