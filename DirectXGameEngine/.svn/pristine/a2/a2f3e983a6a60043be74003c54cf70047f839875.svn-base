/************************************************************************
*    FILE NAME:       framescriptdefs.h
*
*    DESCRIPTION:     Frame script related classes
************************************************************************/  

#ifndef __frame_script_defs_h__
#define __frame_script_defs_h__

// Physical component dependency
#include <scripting/script.h>

// Game lib dependencies
#include <scripting/scriptdefs.h>
#include <scripting/scriptobject.h>

// Script class used to set what frame of animation is visible
class CScriptSetFrame : public CScript
{
public:

    CScriptSetFrame(){ type = EST_SET_FRAME; }

    CScriptSetFrame( CScript * pCopy )
    { 
        frame = pCopy->GetFrame();
        type = EST_SET_FRAME; 
    }

    CScriptSetFrame( CScript & copy )
    { 
        frame = copy.GetFrame();
        type = EST_SET_FRAME; 
    }

    // Frame variable used to set the current visible frame
    int frame;

    // Set and get functions for frame
    virtual void SetFrame(int _frame){ frame = _frame; };
    virtual int GetFrame(){return frame;};

    // Update function
    virtual bool Update( CScriptObject * pScriptObj )
    {
        if( !finished )
        {
            pScriptObj->SetCurrentFrame( frame );
            finished = true;
        }

        return finished;
    }

};


// Script class to animate through all frames of an animation
class CScriptAnimate : public CScript
{
public:

    CScriptAnimate():time(0.f){ type = EST_ANIMATE; }

    CScriptAnimate( CScript * pCopy )
    { 
        time = pCopy->GetTime();
        type = EST_ANIMATE; 
    }

    CScriptAnimate( CScript & copy )
    { 
        time = copy.GetTime();
        type = EST_ANIMATE; 
    }

    // The amount of time it takes to change frames
    float time;

    // Set and get functions for time
    virtual void SetTime(float _time){ time = _time; };
    virtual float GetTime(){return time;};

    // Update function
    virtual bool Update( CScriptObject * pScriptObj )
    {
        if( !finished )
        {
            // If the frame timer is greater than the time it takes to change
            // frames, it is time to switch to the next frame of animation
            if( pScriptObj->GetFrameTimer() > GetTime() )
            {
                // Compensate for time differences
                float timeReset = pScriptObj->GetFrameTimer();
                do
                {
                    timeReset -= GetTime();
                }
                while( timeReset > GetTime() );

                // Set initial variables
                pScriptObj->SetFrameTimer( timeReset );
                finished = true;
                int frameStart = 0;
                int frameEnd = pScriptObj->GetFrameCount() - 1;
                int newFrame = pScriptObj->GetCurrentFrame() + 1;

                // Keep animating until the current frame exceeds the end frame
                if( newFrame <= frameEnd )
                {
                    pScriptObj->SetCurrentFrame( newFrame );
                    finished = false;
                }
                // If the end frame is exceeded, we reset the current frame to the end frame
                else if( pScriptObj->GetCurrentFrame() > frameEnd )
                    pScriptObj->SetCurrentFrame( frameEnd );
            }
        }

        return finished;
    }

};


// Script class to animate a specific segment of frames in an animation
class CScriptAnimateFrame : public CScript
{
public:

    CScriptAnimateFrame():startFrame(0),endFrame(0),time(0),frameInc(0){ type = EST_ANIMATE_FRAME;}

    CScriptAnimateFrame( CScript * pCopy ):frameInc(0)
    { 
        startFrame = pCopy->GetStartFrame();
        endFrame = pCopy->GetEndFrame();
        time = pCopy->GetTime();
        type = EST_ANIMATE_FRAME; 
    }

    CScriptAnimateFrame( CScript & copy ):frameInc(0)
    { 
        startFrame = copy.GetStartFrame();
        endFrame = copy.GetEndFrame();
        time = copy.GetTime();
        type = EST_ANIMATE_FRAME; 
    }

    // Set and get functions for time
    void SetTime(float _time){ time = _time; }
    float GetTime(){return time;}

    // Set and get functions for the starting frame
    void SetStartFrame(int frame){ startFrame = frame; }
    int GetStartFrame(){ return startFrame; }

    // Set and get functions for the ending frame
    void SetEndFrame(int frame){ endFrame = frame; }
    int GetEndFrame(){ return endFrame; }

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        if( !finished )
        {
            // If we haven't set the frame increment ammount yet, we set it and set the
            // current frame to the start frame
            if( frameInc == 0 )
            {
                finished = false;

                // If the end frame is less than the start frame, the animation increments backwards
                if( startFrame < endFrame )
                    frameInc = 1;
                else
                    frameInc = -1;

                pScriptObj->SetCurrentFrame( startFrame );
            }
            // If the frame timer is greater than the time it takes to change
            // frames, it is time to switch to the next frame of animation
            else if( pScriptObj->GetFrameTimer() > time )
            {
                // Compensate for time differences
                float timeReset = pScriptObj->GetFrameTimer();

                // If multiple frames have passed since last update, just
                // count it as one frame
                while( timeReset > time )
                    timeReset -= time;

                // Set initial variables
                pScriptObj->SetFrameTimer( timeReset );
                finished = true;

                // Calculate the new current frame
                int newFrame = pScriptObj->GetCurrentFrame() + frameInc;

                // Keep animating until the current frame exceeds the end frame
                if( ((frameInc ==  1) && (newFrame < endFrame)) || 
                    ((frameInc == -1) && (newFrame > endFrame)) )
                {
                    pScriptObj->SetCurrentFrame( newFrame );
                    finished = false;
                }
                // If the end frame is exceeded, we reset the current frame to the end frame
                else if( ((frameInc ==  1) && (newFrame > endFrame)) || 
                         ((frameInc == -1) && (newFrame < endFrame)) )
                {
                    pScriptObj->SetCurrentFrame( endFrame );
                }
            }
        }

        return finished;
    }

    void Reset()
    {
        finished = false;
        frameInc = 0;
    }

private:

    // The start and end frames of the animation
    int startFrame, endFrame;

    // The amount of time it takes to change frames
    float time;

    // Direction of frame incrementation
    int frameInc;
};

#endif  // __script_defs_h__