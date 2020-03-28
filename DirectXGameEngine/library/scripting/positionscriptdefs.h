/************************************************************************
*    FILE NAME:       positionscriptdefs.h
*
*    DESCRIPTION:     Position script related classes
************************************************************************/  

#ifndef __position_script_defs_h__
#define __position_script_defs_h__

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <scripting/scriptdefs.h>
#include <common/point.h>
#include <common/worldpoint.h>

/************************************************************************
*	Script Call: SetPosition
*
*	Description: Sets the position of the sprite to the position designated in
*				 the script call
*
*	Example:	 <SetPosition f_x="50" f_y="100" f_z="1"/>
*				 <SetPosition i_x="90" i_y="10" i_z="10"/>
*				 <SetPosition i_x="90" i_y="10" i_z="10" f_x="50" f_y="100" f_z="1"/>
************************************************************************/
class CScriptSetPosition : public CScript
{
public:

    CScriptSetPosition()
    { type = EST_SET_POSITION; }

    CScriptSetPosition( CScript * pCopy )
    {
        pos = *pCopy->GetWorldPoint();
        bFloatPos = *pCopy->GetBoolPointFloat();
        bIntPos = *pCopy->GetBoolPointInt();

        type = EST_SET_POSITION; 
    }

    CScriptSetPosition( CScript & copy )
    {
        pos = *copy.GetWorldPoint();
        bFloatPos = *copy.GetBoolPointFloat();
        bIntPos = *copy.GetBoolPointInt();

        type = EST_SET_POSITION; 
    }

    bool Update( CScriptObject * pScriptObj )
    {
        // Set the color of the object
        if( !finished )
        {
            CWorldPoint newPos = pScriptObj->GetPos();

            if( bFloatPos.x )
                newPos.x.f = pos.x.f;

            if( bFloatPos.y )
                newPos.y.f = pos.y.f;

            if( bFloatPos.z )
                newPos.z.f = pos.z.f;

            if( bIntPos.x )
                newPos.x.i = pos.x.i;

            if( bIntPos.y )
                newPos.y.i = pos.y.i;

            if( bIntPos.z )
                newPos.z.i = pos.z.i;

            pScriptObj->SetPos( newPos );

            finished = true;
        }

        return finished;
    }

    // The position to set an object to
    void SetX(float x){ pos.x.f = x; bFloatPos.x = true; }
    void SetY(float y){ pos.y.f = y; bFloatPos.y = true; }
    void SetZ(float z){ pos.z.f = z; bFloatPos.z = true; }
    void SetX(int x){ pos.x.i = x; bIntPos.x = true; }
    void SetY(int y){ pos.y.i = y; bIntPos.y = true; }
    void SetZ(int z){ pos.z.i = z; bIntPos.z = true; }

    // Get functions
    CWorldPoint * GetWorldPoint(){return &pos;}
    CBoolPoint * GetBoolPointFloat(){return &bFloatPos;}
    CBoolPoint * GetBoolPointInt(){return &bIntPos;}

private:

    CWorldPoint pos;
    CBoolPoint bFloatPos;
    CBoolPoint bIntPos;
};


/************************************************************************
*	Script Call: IncPosition
*
*	Description: Increments the position of the sprite by the designated 
*				 amount in the script call
*
*	Example:	 <IncPosition f_x="50" f_y="100" f_z="1"/>
*				 <IncPosition i_x="90" i_y="10" i_z="10"/>
*				 <IncPosition i_x="90" i_y="10" i_z="10" f_x="50" f_y="100" f_z="1"/>
************************************************************************/
class CScriptIncPosition : public CScript
{
public:

    CScriptIncPosition(){ type = EST_INC_POSITION; }

    CScriptIncPosition( CScript * pCopy )
    { 
        pos = *pCopy->GetWorldPoint();

        type = EST_INC_POSITION; 
    }

    CScriptIncPosition( CScript & copy )
    { 
        pos = *copy.GetWorldPoint();

        type = EST_INC_POSITION; 
    }

    bool Update( CScriptObject * pScriptObj )
    {
        // Increment the color of the object
        if( !finished )
        {
            pScriptObj->IncPos( pos );

            finished = true;
        }

        return finished;
    }

    // The position to set an object to
    void SetX(float x){ pos.x.f = x; }
    void SetY(float y){ pos.y.f = y; }
    void SetZ(float z){ pos.z.f = z; }
    void SetX(int x){ pos.x.i = x; }
    void SetY(int y){ pos.y.i = y; }
    void SetZ(int z){ pos.z.i = z; }

    // Get functions
    CWorldPoint * GetWorldPoint(){return &pos;}

private:

    CWorldPoint pos;
};


/************************************************************************
*	Script Call: MoveTo
*
*	Description: Using time - Gradually transitions the current position
*				 of a sprite to another position within the specified
*				 amount of time.
*
*				 Using speed - Gradually transitions the current position
*				 of a sprite to another position at a specified speed.
*
*	Example:	 <MoveTo method="time" time="2000" i_x="1" i_y="2" i_z="3" f_x="100" f_y="10" f_z="1"/>
*				 <MoveTo method="speed" speed="500" i_x="1" i_y="2" i_z="3" f_x="100" f_y="10" f_z="1"/>
*
*	NOTE:		 1 time = 1 millisecond
*				 1000 time = 1 second
*				 
*				 1 speed = 1 pixel per second
************************************************************************/
class CScriptMoveTo : public CScript
{
public:

    CScriptMoveTo() : speed(0),time(0),posIncSet(false)
    { type = EST_MOVE_TO; }

    CScriptMoveTo( CScript * pCopy ) : posIncSet(false)
    { 
        pos = *pCopy->GetWorldPoint();
        bFloatPos = *pCopy->GetBoolPointFloat();
        bIntPos = *pCopy->GetBoolPointInt();
        speed = pCopy->GetSpeed();
        time = pCopy->GetTime();

        type = EST_MOVE_TO; 
    }

    CScriptMoveTo( CScript & copy ) : posIncSet(false)
    { 
        pos = *copy.GetWorldPoint();
        bFloatPos = *copy.GetBoolPointFloat();
        bIntPos = *copy.GetBoolPointInt();
        speed = copy.GetSpeed();
        time = copy.GetTime();

        type = EST_MOVE_TO; 
    }

    // An amount of time used to determine when something
    // should happen after a certain amount of time
    void SetTime(float _time){time = _time;}
    float GetTime(){return time;}

    // The speed in which something should change
    void SetSpeed(float _speed){speed = _speed;}
    float GetSpeed(){return speed;}

    void MoveToViaTime( CScriptObject * pScriptObj )
    {
        if( posIncSet )
        {
            if( pScriptObj->GetTimer() < time )
                pScriptObj->IncPos( posInc * CHighResTimer::Instance().GetElapsedTime() );
            
            else
            {
                CWorldPoint newPos = pScriptObj->GetPos();

                if( bFloatPos.x )
                    newPos.x.f = pos.x.f;

                if( bFloatPos.y )
                    newPos.y.f = pos.y.f;

                if( bFloatPos.z )
                    newPos.z.f = pos.z.f;

                if( bIntPos.x )
                    newPos.x.i = pos.x.i;

                if( bIntPos.y )
                    newPos.y.i = pos.y.i;

                if( bIntPos.z )
                    newPos.z.i = pos.z.i;

                pScriptObj->SetPos( newPos );

                finished = true;
            }
        }
        else
        {
            posInc = ( pos - pScriptObj->GetPos() ) / time;

            if( !bFloatPos.x && !bIntPos.x )
                posInc.x = 0;

            if( !bFloatPos.y && !bIntPos.y )
                posInc.y = 0;

            if( !bFloatPos.z && !bIntPos.z )
                posInc.z = 0;

            posIncSet = true;
        }
    }

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        // Increment the color of the object
        if( !finished )
        {
            if( time != 0 )
                MoveToViaTime( pScriptObj );
        }

        return finished;
    }


    void Reset()
    {
        finished = false;
        posIncSet = false;
        posInc.Clear();
    }


    // The position to set an object to
    void SetX(float x){ pos.x.f = x; bFloatPos.x = true; }
    void SetY(float y){ pos.y.f = y; bFloatPos.y = true; }
    void SetZ(float z){ pos.z.f = z; bFloatPos.z = true; }
    void SetX(int x){ pos.x.i = x; bIntPos.x = true; }
    void SetY(int y){ pos.y.i = y; bIntPos.y = true; }
    void SetZ(int z){ pos.z.i = z; bIntPos.z = true; }

    // Get functions
    CWorldPoint * GetWorldPoint(){return &pos;}
    CBoolPoint * GetBoolPointFloat(){return &bFloatPos;}
    CBoolPoint * GetBoolPointInt(){return &bIntPos;}

private:

    CWorldPoint pos;
    CBoolPoint bFloatPos;
    CBoolPoint bIntPos;

    CPoint posInc;
    float speed;
    float time;
    bool posIncSet;

};


/************************************************************************
*	Script Call: Move
*
*	Description: Using time - Gradually transitions the current position
*				 of a sprite to another position, relative to the starting
*				 position, within the specified amount of time.
*
*				 Using speed - Gradually transitions the current position of
*				 a sprite to another positino, relative to the starting
*				 position, at a specified speed.
*
*	Example:	 <Move method="time" time="2000" i_x="1" i_y="-2" i_z="3" f_x="-100" f_y="10" f_z="1"/>
*				 <Move method="speed" speed="500" i_x="1" i_y="-2" i_z="3" f_x="-100" f_y="10" f_z="1"/>
*
*	NOTE:		 1 time = 1 millisecond
*				 1000 time = 1 second
*				 
*				 1 speed = 1 pixel per second
************************************************************************/
class CScriptMove : public CScript
{
public:

    CScriptMove() : speed(0),time(0),posIncSet(false)
    { type = EST_MOVE; }

    CScriptMove( CScript * pCopy ) : posIncSet(false)
    { 
        pos = *pCopy->GetWorldPoint();
        bFloatPos = *pCopy->GetBoolPointFloat();
        bIntPos = *pCopy->GetBoolPointInt();
        speed = pCopy->GetSpeed();
        time = pCopy->GetTime();

        type = EST_MOVE;
    }

    CScriptMove( CScript & copy ) : posIncSet(false)
    { 
        pos = *copy.GetWorldPoint();
        bFloatPos = *copy.GetBoolPointFloat();
        bIntPos = *copy.GetBoolPointInt();
        speed = copy.GetSpeed();
        time = copy.GetTime();

        type = EST_MOVE_TO; 
    }

    // An amount of time used to determine when something
    // should happen after a certain amount of time
    void SetTime(float _time){time = _time;}
    float GetTime(){return time;}

    // The speed in which something should change
    void SetSpeed(float _speed){speed = _speed;}
    float GetSpeed(){return speed;}

    void MoveViaTime( CScriptObject * pScriptObj )
    {
        if( posIncSet )
        {
            if( pScriptObj->GetTimer() < time )
                pScriptObj->IncPos( posInc * CHighResTimer::Instance().GetElapsedTime() );
            
            else
            {
                pScriptObj->SetPos( finalPos );
                finished = true;
            }
        }
        else
        {
            CWorldPoint tmpCurrentPos = pScriptObj->GetPos();
            finalPos = pos + pScriptObj->GetPos();
            posInc = pos / time;

            if( !bFloatPos.x && !bIntPos.x )
                finalPos.x = tmpCurrentPos.x;

            if( !bFloatPos.y && !bIntPos.y )
                finalPos.y = tmpCurrentPos.y;

            if( !bFloatPos.z && !bIntPos.z )
                finalPos.z = tmpCurrentPos.z;

            posIncSet = true;
        }
    }

    // Update function
    bool Update( CScriptObject * pScriptObj )
    {
        // Increment the color of the object
        if( !finished )
        {
            if( time != 0 )
                MoveViaTime( pScriptObj );
        }

        return finished;
    }


    void Reset()
    {
        finished = false;
        posIncSet = false;
        posInc.Clear();
    }

    // The position to set an object to
    void SetX(float x){ pos.x.f = x; bFloatPos.x = true; }
    void SetY(float y){ pos.y.f = y; bFloatPos.y = true; }
    void SetZ(float z){ pos.z.f = z; bFloatPos.z = true; }
    void SetX(int x){ pos.x.i = x; bIntPos.x = true; }
    void SetY(int y){ pos.y.i = y; bIntPos.y = true; }
    void SetZ(int z){ pos.z.i = z; bIntPos.z = true; }

    // Get functions
    CWorldPoint * GetWorldPoint(){return &pos;}
    CBoolPoint * GetBoolPointFloat(){return &bFloatPos;}
    CBoolPoint * GetBoolPointInt(){return &bIntPos;}

private:

    CWorldPoint pos;
    CBoolPoint bFloatPos;
    CBoolPoint bIntPos;
    CWorldPoint finalPos;

    CPoint posInc;
    float speed;
    float time;
    bool posIncSet;

};


#endif  // __position_script_defs_h__