
/************************************************************************
*
*    PRODUCT:         Blink Object list
*
*    FILE NAME:       TBlinkList.h
*
*    DESCRIPTION:     Object for holding lists of bitmap offsets.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __BLINKOBJ_LIST_H__
#define __BLINKOBJ_LIST_H__

#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size 

#include "TObjList.h"       // Parent object class


class TBlink
{
public:
    RECT Dest;
    RECT ArtBlinkOn;
	RECT ArtBlinkOff;
    DWORD BlinkTimer;
    DWORD TimerIntervalOn;
	DWORD TimerIntervalOff;
	BOOL ChangeState;
	int BlinkState;

    // Init the data members on allocation
    TBlink()
    {
		SetRect( &Dest, 0,0,0,0 );
		SetRect( &ArtBlinkOn, 0,0,0,0 );
		SetRect( &ArtBlinkOff, 0,0,0,0 );
		BlinkTimer = 0;
		TimerIntervalOn = 0;
		TimerIntervalOff = 0;
		ChangeState = FALSE;
		BlinkState = 0;  
    }
};
typedef TBlink * PTBlink;



class TBlinkList : public TObjList
{

public:
    // Common Functions

    // Constructor
    TBlinkList();
    // Destructor - Put all your cleanup code here
    ~TBlinkList();
    // Add a string
    virtual BOOL Add();
    // Frees the pointer from the object class
    void Free();
    // Get the size of the data needed for the Obj class
    // Save everything but the TStoryList
    virtual int GetObjSize( int Index ){ return 0; };

    // Get pointer to the destination rect
    RECT *GetDestRect( int Index ){ return &(PTBlink(GetObj( Index ))->Dest); };
    // Get pointer to the ArtBlinkOn rect
    RECT *GetArtBlinkOn( int Index ){ return &(PTBlink(GetObj( Index ))->ArtBlinkOn); };
    // Get pointer to the ArtBlinkOff rect
    RECT *GetArtBlinkOff( int Index ){ return &(PTBlink(GetObj( Index ))->ArtBlinkOff); };

    // Set the blink timer
    void SetBlinkTimer( int Index, DWORD Value ){ PTBlink(GetObj( Index ))->BlinkTimer = Value; };
    // Get the blink timer
    DWORD GetBlinkTimer( int Index ){ return PTBlink(GetObj( Index ))->BlinkTimer; };

    // Set the Timer Interval On
    void SetTimerIntervalOn( int Index, DWORD Value ){ PTBlink(GetObj( Index ))->TimerIntervalOn = Value; };
    // Get the Timer Interval On
    DWORD GetTimerIntervalOn( int Index ){ return PTBlink(GetObj( Index ))->TimerIntervalOn; };

    // Set the Timer Interval Off
    void SetTimerIntervalOff( int Index, DWORD Value ){ PTBlink(GetObj( Index ))->TimerIntervalOff = Value; };
    // Get the Timer Interval Off
    DWORD GetTimerIntervalOff( int Index ){ return PTBlink(GetObj( Index ))->TimerIntervalOff; };

    // Set the Changed Made
    void SetChangeState( int Index, BOOL Value ){ PTBlink(GetObj( Index ))->ChangeState = Value; };
    // Get the Changed Made
    BOOL GetChangeState( int Index ){ return PTBlink(GetObj( Index ))->ChangeState; };

    // Set the Blink On Off
    void SetBlinkState( int Index, int Value ){ PTBlink(GetObj( Index ))->BlinkState = Value; };
    // Get the Blink On Off
    int GetBlinkState( int Index ){ return PTBlink(GetObj( Index ))->BlinkState; };
};

typedef TBlinkList *PTBlinkList;


#endif  /* __BLINKOBJ_LIST_H__ */