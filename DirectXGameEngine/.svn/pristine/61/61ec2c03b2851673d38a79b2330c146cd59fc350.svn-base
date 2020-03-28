
/************************************************************************
*    FILE NAME:       statcounter.cpp
*
*    DESCRIPTION:     3D stat counter class singleton
************************************************************************/

// Physical component dependency
#include <utilities/statcounter.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <system/xdevice.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStatCounter::CStatCounter()
            : cycleCounter(0),
              ignorAllCounters(false)
{
    ResetCounters();

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStatCounter::~CStatCounter()
{

}   // Destructer


/************************************************************************
*    desc:  Inc the display counter                                                             
************************************************************************/
void CStatCounter::IncDisplayCounter( size_t i )
{
    if( !ignorAllCounters )
    {
        xObjCounter += i;
    }

}	// IncDisplayCounter


/************************************************************************
*    desc:  Inc the collision counter                                                             
************************************************************************/
void CStatCounter::IncCollisionCounter( size_t i )
{
    if( !ignorAllCounters )
    {
        cObjCounter += i;
    }

}	// IncCollisionCounter


/************************************************************************
*    desc:  Inc the shadow counter                                                             
************************************************************************/
void CStatCounter::IncShadowCounter()
{
    if( !ignorAllCounters )
    {
        ++shadowCounter;
    }

}	// IncShadowCounter


/************************************************************************
*    desc:  Inc the shadow build counter                                                             
************************************************************************/
void CStatCounter::IncShadowBuildCounter()
{
    if( !ignorAllCounters )
    {
        ++shadowBuildCounter;
    }

}	// IncShadowBuildCounter


/************************************************************************
*    desc:  Set the player position
*
*	 param: CPointInt & pos - position to set to
************************************************************************/
void CStatCounter::SetPlayerPos( const CPointInt & pos )
{
    playerPos = pos;

}	// SetPlayerPos


/************************************************************************
*    desc:  Have we run through one cycle.
*
*	 ret: bool - true if one cycle has been completed
************************************************************************/
void CStatCounter::IncCycle()
{
    // These counters are incremeented each game loop cycle so they can
    // be placed here in this function because this function is also called
    // each game loop cycle
    elapsedFPSCounter += CHighResTimer::Instance().GetFPS();

    ++cycleCounter;

}	// HasCycleCompleted


/************************************************************************
*    desc:  Reset the counter                                                             
************************************************************************/
void CStatCounter::ResetCounters()
{
    cObjCounter = 0;
    xObjCounter = 0;
    shadowCounter = 0;
    shadowBuildCounter = 0;
    elapsedFPSCounter = 0.0;
    cycleCounter = 0;

}	// ResetCounters


/************************************************************************
*    desc:  Set the flag to ignore counters
*  
*    param: bool value - 
************************************************************************/
void CStatCounter::IgnoreCounters( bool value )
{
    ignorAllCounters = value;

}	// IgnoreCounters


/************************************************************************
*    desc:  Get the stat string                                                             
************************************************************************/
string & CStatCounter::GetStatString()
{
    statStr = boost::str( boost::format("fps: %1% - xObjs: %2% - cObjs: %3% - Shadows: %4% - PlayerPos: %5%x %6%y - Vid Mem Used: %7% megs") 
                    % ((int)(elapsedFPSCounter / (double)cycleCounter))
                    % (xObjCounter / cycleCounter)
                    % (cObjCounter / cycleCounter)
                    % (shadowCounter / cycleCounter)
                    % (playerPos.x)
                    % (playerPos.y)
                    % CXDevice::Instance().GetVideoMemUsed() );

    // The purpose of our counters is to display the stats. Now that the
    // stats are displayed, we can reset out counters.
    ResetCounters();

    return statStr;

}	// GetStatString




