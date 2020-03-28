/************************************************************************
*    FILE NAME:       highresolutiontimer.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

// Physical component dependency
#include <utilities/highresolutiontimer.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CHighResTimer::CHighResTimer()
             : inverseTimerFrequency(0.0),
               lastTime(INT64(0.0)),
               elapsedTime(0.0f),
               fps(0.0f)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CHighResTimer::~CHighResTimer()
{
}   // Destructer


/***************************************************************************
*    desc:  Setup the performance timer if we have one
*
*    ret:  bool - do we have a performance timer
****************************************************************************/
bool CHighResTimer::InitPerformanceTimer()
{
    INT64 frenquency = 0;

    // See if a performance timer is available
    BOOL hiRezTimerSupported = QueryPerformanceFrequency( (LARGE_INTEGER*)&frenquency );

    if( !hiRezTimerSupported || frenquency == 0 || frenquency == 1000 )
        throw NExcept::CCriticalException("High Resolution Timer Error",
            "System appears to not have a performance timer. Game requires a perforance timer.");

    else
    {
        // inverse it so that we can do a simple multiplication instead of division
        inverseTimerFrequency = 1000.0 / (double)frenquency;

        // Init the lastTime variable for the first runthrough
        QueryPerformanceCounter( (LARGE_INTEGER*)&lastTime );

        return true;
    }

}  // InitPerformanceTimer


/***************************************************************************
*    desc:  Calc the elapsed time
****************************************************************************/
void CHighResTimer::CalcElapsedTime()
{
    INT64 time;

    // Get the current performance time
    QueryPerformanceCounter( (LARGE_INTEGER*)&time );

    // Set the elapsed time
    elapsedTime = (float)((time - lastTime) * inverseTimerFrequency);

    // Set the fps
    fps = 1000.0f / elapsedTime;

    // Catch any hickups - cap to about 10 fps
    if( elapsedTime > 100.0f )
    {
        elapsedTime = 100.0f;
    }

    // Reset the last time
    lastTime = time;
        
}  // GetPerformanceTime


/***************************************************************************
*    desc:  Get the elapsed time
****************************************************************************/
float CHighResTimer::GetElapsedTime()
{
    return elapsedTime;
}


/***************************************************************************
*    desc:  get the elapsed time
****************************************************************************/
float CHighResTimer::GetFPS()
{
    return fps;
}


/***************************************************************************
*    desc:  get the raw time
****************************************************************************/
double CHighResTimer::GetTime()
{
    INT64 time;

    // Get the current performance time
    QueryPerformanceCounter( (LARGE_INTEGER*)&time );

    return (double)(time * inverseTimerFrequency);

}	// GetTime


