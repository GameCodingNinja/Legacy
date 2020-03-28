/************************************************************************
*    FILE NAME:       command.h
*
*    DESCRIPTION:     Class containing all of the commands in an xml
************************************************************************/  

#ifndef __general_script_defs_h__
#define __general_script_defs_h__

// Game lib dependencies
#include <scripting/script.h>
#include <scripting/scriptdefs.h>
#include <scripting/scriptobject.h>

// Script class used to delay a script step by a certain amount of time
class CScriptSetHold : public CScript
{
public:

    CScriptSetHold():time(0){ type = EST_SET_HOLD; }
    CScriptSetHold( CScript * pCopy )
    {
        time = pCopy->GetTime();
        type = EST_SET_HOLD;
    }
    CScriptSetHold( CScript & copy )
    {
        time = copy.GetTime();
        type = EST_SET_HOLD;
    }

    // The amount of time it takes to complete the hold
    float time;

    // Set and get functions for time
    void SetTime(float _time){time = _time;}
    float GetTime(){return time;}

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        // If the timer is greater than the hold time, the hold is complete
        if( !finished && pScriptObj->GetTimer() > time )
            finished = true;

        return finished;
    }
    
};


// Script class used to set the visibility of the object
class CScriptSetVisible : public CScript
{
public:

    CScriptSetVisible():visible(false){ type = EST_SET_VISIBLE; }
    CScriptSetVisible( CScript * pCopy )
    {
        visible = pCopy->IsVisible();
        type = EST_SET_VISIBLE;
    }
    CScriptSetVisible( CScript & copy )
    {
        visible = copy.IsVisible();
        type = EST_SET_VISIBLE;
    }

    // The visibility of the object
    bool visible;

    // Set and get functions for the visibility
    virtual void SetVisible(bool _visible){visible = _visible;};
    virtual bool IsVisible(){return visible;};

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        // Set the visibility of the object
        if(!finished)
        {
            pScriptObj->SetVisible( visible );
            finished = true;
        }

        return finished;
    }

};


#endif  // __general_script_defs_h__