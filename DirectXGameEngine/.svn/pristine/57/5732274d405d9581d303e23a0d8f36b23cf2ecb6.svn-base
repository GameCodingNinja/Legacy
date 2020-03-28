/************************************************************************
*    FILE NAME:       iobjectanimationdata.h
*
*    DESCRIPTION:     Script object animation data interface class
************************************************************************/  

#ifndef __i_object_animation_data_h__
#define __i_object_animation_data_h__

// The class used as an interface for the other script classes
class iObjectAnimationData
{
public:

    iObjectAnimationData(){};
    virtual ~iObjectAnimationData(){};

    virtual void SetCurrentFrame(int value) = 0;
    virtual void IncCurrentFrame(int value) = 0;
    virtual int GetCurrentFrame() const = 0;

    virtual void SetHoldCounter(int value) = 0;
    virtual void IncHoldCounter(int value) = 0;
    virtual int GetHoldCounter() const = 0;

    virtual void SetFrameCounter(int value) = 0;
    virtual void IncFrameCounter(int value) = 0;
    virtual int GetFrameCounter() const = 0;

    virtual void SetStepCounter(int value) = 0;
    virtual void IncStepCounter(int value) = 0;
    virtual int GetStepCounter() const = 0;

    virtual void SetTimer(float value) = 0;
    virtual void IncTimer(float value) = 0;
    virtual float GetTimer() const = 0;

    virtual void SetFrameTimer(float value) = 0;
    virtual float GetFrameTimer() const = 0;

    virtual void SetAnimating(bool value) = 0;
    virtual bool IsAnimating() const = 0;

};

#endif	// __i_object_animation_data_h__