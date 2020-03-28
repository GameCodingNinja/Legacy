
/************************************************************************
*    FILE NAME:       scriptobject.h
*
*    DESCRIPTION:     Object to handle script actions
************************************************************************/

#ifndef __script_object_h__
#define __script_object_h__

// Physical component dependency
#include <common/object.h>

// Boost lib dependencies
#include <boost/scoped_ptr.hpp>

// Game lib dependencies
#include <common/color.h>
#include <scripting/scriptdefs.h>

// Forward declaration(s)
class CCommandScript;
class CCommand;

class CScriptObject : public CObject
{
public:

    // Constructor
    CScriptObject();

    // Destructor
    virtual ~CScriptObject();

    // Update the script object
    virtual void Update();

    // Set and get the command script
    void SetCommandScript( CCommandScript * pScript );
    CCommandScript * GetCommandScript();

    // Set and get the active command and command name
    virtual void SetActiveCommand( const std::string & commandName );
    CCommand * GetActiveCommand();
    const std::string & GetActiveCommandName() const;

    // Get the end mode of the active command
    ECommandEndMode GetCommandEndMode() const;

    // Is the end mode of the active command to loop
    bool IsEndModeLoop() const;

    // Set, get, increment, and reset the color
    void SetColor( const CColor & _color );
    void IncColor( const CColor & _color );
    const CColor & GetColor() const;
    virtual void ResetColor();

    // Set and get the default color
    virtual void SetDefaultColor( const CColor & _color );
    virtual const CColor & GetDefaultColor() const;

    // Set, get, and increment the timer
    void SetTimer( float value );
    void IncTimer( float value );
    float GetTimer() const;

    // Set and get the animating flag
    void SetAnimating( bool value );
    bool IsAnimating() const;

    // Set, get, and increment the current frame of animation
    virtual void SetCurrentFrame( int frame );
    virtual void IncCurrentFrame( int frame );
    virtual int GetCurrentFrame() const;

    // Set and get the current frame of animation
    virtual int GetFrameCount() const;

    // Set, get, and increment the frame timer
    virtual void SetFrameTimer( float value );
    virtual void IncFrameTimer( float value );
    virtual float GetFrameTimer() const;

    // Set, get, and increment the step counter
    void SetStepCounter( int value );
    void IncStepCounter( int value );
    int GetStepCounter() const;

protected:

    // Command script for animation
    boost::scoped_ptr<CCommandScript> spCommandScript;	

    // Active command and name
    CCommand * pActiveCommand;	
    std::string activeCommandName;

    // Color of the script object
    CColor color;

    // NOTE: This is a temporary class object. This will be
    // relocated to the object data later on
    CColor defaultColor;

    // Script step counter
    int stepCounter;

    // Timer
    float timer;

    // Animating flag
    bool animating;
};

#endif  // __script_object_h__


