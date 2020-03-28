/************************************************************************
*    FILE NAME:       script.h
*
*    DESCRIPTION:     Script interface class
************************************************************************/  

#ifndef __script_h__
#define __script_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/point.h>
#include <common/worldpoint.h>
#include <common/color.h>
#include <scripting/scriptdefs.h>
#include <scripting/scriptobject.h>

// The class used as an interface for the other script classes
class CScript : public boost::noncopyable
{
public:

    CScript():type(EST_NULL),finished(false){}
    CScript( CScript * pCopy ):type(EST_NULL),finished(false){}

    EScriptType type;

    // The visibility to set an object to
    virtual void SetVisible(bool visible){}
    virtual bool IsVisible(){return false;}

    // An amount of time used to determine when something
    // should happen after a certain amount of time
    virtual void SetTime(float time){}
    virtual float GetTime(){return 0;}

    // The speed in which something should change
    virtual void SetSpeed(float speed){}
    virtual float GetSpeed(){return 0;}

    // A frame to set the current frame to
    virtual void SetFrame(int frame){}
    virtual int GetFrame(){return 0;}

    // The starting frame of an animation
    virtual void SetStartFrame(int frame){}
    virtual int GetStartFrame(){return 0;}

    // The ending frame of an animation
    virtual void SetEndFrame(int frame){}
    virtual int GetEndFrame(){return 0;}

    // The frames per second of an animation
    virtual void SetFPS(float fps){}
    virtual float GetFPS(){return 0;}

    // The color to set an object to
    virtual void SetR(float r){}
    virtual void SetG(float g){}
    virtual void SetB(float b){}
    virtual void SetA(float a){}
    virtual void SetFlagR(){}
    virtual void SetFlagG(){}
    virtual void SetFlagB(){}
    virtual void SetFlagA(){}
    virtual void ResetDefaultColor(){}
    virtual CColor * GetColor(){return NULL;}
    virtual CBoolColor * GetBoolColor(){return NULL;}
    virtual bool GetResetDefaultColor(){return false;}

    // The point to change an object's position, rotation, or scale
    virtual void SetX(float x){}
    virtual void SetY(float y){}
    virtual void SetZ(float z){}
    virtual void SetX(int x){}
    virtual void SetY(int y){}
    virtual void SetZ(int z){}
    virtual CWorldPoint * GetWorldPoint(){return NULL;}
    virtual CBoolPoint * GetBoolPointFloat(){return NULL;}
    virtual CBoolPoint * GetBoolPointInt(){return NULL;}

    virtual void SetSound( std::string & sound ){}
    virtual std::string * GetSound(){return NULL;}

    // The update function where the adjustments to the object take place
    virtual bool Update( CScriptObject * pScriptObj ){return false;}

    // Reset the script variables
    virtual void Reset(){finished = false;}

protected:

    bool finished;

};

#endif	// __script_h__