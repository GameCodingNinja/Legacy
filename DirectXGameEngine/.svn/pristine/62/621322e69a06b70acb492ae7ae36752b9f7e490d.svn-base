
/************************************************************************
*    FILE NAME:       objectanimationdata2d.h
*
*    DESCRIPTION:     Class containing the object's animation data
************************************************************************/

#ifndef __object_animation_data_2d_h__
#define __object_animation_data_2d_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <scripting/command.h>
#include <scripting/commandscript.h>
#include <scripting/iobjectanimationdata.h>
#include <utilities/deletefuncs.h>

class CObjectAnimationData2D// : public iObjectAnimationData
{
public:
    CObjectAnimationData2D()//:pCommandScript(NULL),pActiveCommand(NULL),currentFrame(0),
                             //holdCounter(0),frameCounter(0),stepCounter(0),
                             //timer(0),frameTimer(0),animating(false)
    {}

    //~CObjectAnimationData2D(){ NDelFunc::Delete(pCommandScript); }

    // Set and get the name of the script
    void SetScriptName(const std::string & name){scriptName = name;}
    const std::string & GetScriptName() const {return scriptName;}

    // Set and get the name of the default command
    void SetDefaultCommand(const std::string & command){defaultCommand = command;}
    const std::string & GetDefaultCommand() const {return defaultCommand;}

    // Set and get the command script
    //void NewCommandScript(CCommandScript * pScript){NDelFunc::Delete(pCommandScript); pCommandScript = new CCommandScript(pScript);}
    //CCommandScript * GetCommandScript(){return pCommandScript;}

    // Set and get the active command and command name
    //void SetActiveCommand(CCommand * pCmd, const std::string & name){pActiveCommand = pCmd; activeCommandName = name;}
    //CCommand * GetActiveCommand(){return pActiveCommand;}
    //const std::string & GetActiveCommandName() const {return activeCommandName;}

    // Set, get, and increment the current frame
    //void SetCurrentFrame(int value){currentFrame = value;}
    //void IncCurrentFrame(int value){currentFrame += value;}
    //int GetCurrentFrame() const {return currentFrame;}

    // Set, get, and increment the hold counter
    //void SetHoldCounter(int value){holdCounter = value;}
    //void IncHoldCounter(int value){holdCounter += value;}
    //int GetHoldCounter() const {return holdCounter;}

    // Set, get, and increment the frame counter
    //void SetFrameCounter(int value){frameCounter = value;}
    //void IncFrameCounter(int value){frameCounter += value;}
    //int GetFrameCounter()const {return frameCounter;}

    // Set, get, and increment the step counter
    //void SetStepCounter(int value){stepCounter = value;}
    //void IncStepCounter(int value){stepCounter += value;}
    //int GetStepCounter() const {return stepCounter;}

    // Set, get, and increment the timer
    //void SetTimer(float value){timer = value;}
    //void IncTimer(float value){timer += value;}
    //float GetTimer() const {return timer;}

    // Set, get, and increment the frame timer
    //void SetFrameTimer(float value){frameTimer = value;}
    //float GetFrameTimer() const {return frameTimer;}

    // Set and get the animating flag
    //void SetAnimating(bool value){animating = value;}
    //bool IsAnimating() const {return animating;}

private:

    // Name of the script and name of the default command
    std::string scriptName, defaultCommand;						

    // Active command name
    //std::string activeCommandName;		

    // Command script for animation
    //CCommandScript * pCommandScript;	

    // Active command
    //CCommand * pActiveCommand;							

    // The current frame as well as counters used in the scripts
    //int currentFrame, holdCounter, frameCounter, stepCounter;	

    // Timers
    //float timer, frameTimer;

    // Animating flag
    //bool animating;
};

#endif  // __object_animation_data_2d_h__