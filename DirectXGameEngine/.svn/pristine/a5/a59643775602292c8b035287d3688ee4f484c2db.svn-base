
/************************************************************************
*    FILE NAME:       criticalsection.h
*
*    DESCRIPTION:     Class template
************************************************************************/

#ifndef __critical_section_h__
#define __critical_section_h__

class CCriticalSection
{
public:

    // Get the instance of the singleton class
    static CCriticalSection & Instance()
    {
        static CCriticalSection criticalSection;
        return criticalSection;
    }

    // Lock other threads from access
    void Lock();

    // Unlock so other threads can have access
    void Unlock();

private:

    // Constructor
    CCriticalSection();

    // Destructor
    ~CCriticalSection();

private:

    // critical class
    CRITICAL_SECTION cs;

};

#endif  // __critical_section_h__


