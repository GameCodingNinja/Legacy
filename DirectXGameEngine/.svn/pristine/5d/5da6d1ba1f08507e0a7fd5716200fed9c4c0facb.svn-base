/************************************************************************
*    FILE NAME:       statcounter.h
*
*    DESCRIPTION:     3D stat counter class singleton
************************************************************************/  

#ifndef __stat_counter_h__
#define __stat_counter_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/pointint.h>

class CStatCounter
{
public:

    // Get the instance of the singleton class
    static CStatCounter & Instance()
    {
        static CStatCounter statCounter;
        return statCounter;
    }

    // Inc the display counter
    void IncDisplayCounter( size_t i = 1 );

    // Inc the collision counter
    void IncCollisionCounter( size_t i = 1 );

    // Inc the shadow counter
    void IncShadowCounter();

    // Inc the shadow build counter
    void IncShadowBuildCounter();

    // Set the player position
    void SetPlayerPos( const CPointInt & pos );

    // Have we run through one cycle.
    void IncCycle();

    // Set the flag to ignore counters
    void IgnoreCounters( bool value );

    // Get the stat string
    std::string & GetStatString();

protected:

    // Reset the counter
    void ResetCounters();

private:

    CStatCounter();
    ~CStatCounter();

    // Counter for static visual objects
    size_t xObjCounter;

    // Counter for collision objects
    size_t cObjCounter;

    // Shadow counter
    size_t shadowCounter;

    // Shadow build counter
    size_t shadowBuildCounter;

    // The sector position of the player
    CPointInt playerPos;

    // Elapsed time counter
    long double elapsedFPSCounter;

    // cycle counter. This counter is never reset
    size_t cycleCounter;

    // Flag to ignor all counting
    bool ignorAllCounters;

    // Stat string
    std::string statStr;

};

#endif  // __stat_counter_h__