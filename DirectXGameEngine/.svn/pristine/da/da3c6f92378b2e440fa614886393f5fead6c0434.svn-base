/************************************************************************
*    FILE NAME:       highresolutiontimer.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

#ifndef __highresolutiontimer_h__
#define __highresolutiontimer_h__

// Game OS dependencies
#include <windows.h>

class CHighResTimer
{
public:

    // Get the instance of the singleton class
    static CHighResTimer & Instance()
    {
        static CHighResTimer highResTimer;
        return highResTimer;
    }

    // Setup the performance timer if we have one
    bool InitPerformanceTimer();

    // Calc the elapsed time
    void CalcElapsedTime();

    // Get the elapsed time
    float GetElapsedTime();

    // Get the FPS
    float GetFPS();

    // Get the time
    double GetTime();

private:

    // Constructor
    CHighResTimer();

    // Destructor
    ~CHighResTimer();

    // the inverse of the timer frenquency - multipling is faster
    double inverseTimerFrequency;

    // Last recorded time
    INT64 lastTime;

    // The amount of time that has elapsed between frames
    float elapsedTime;

    // The frames per second
    float fps;

};

#endif  // __highresolutiontimer_h__