
/************************************************************************
*    FILE NAME:       criticalsection.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

#define STRICT 
#include <windows.h> 

// Physical component dependency
#include <utilities/criticalsection.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CCriticalSection::CCriticalSection()
{
    InitializeCriticalSection(&cs);

}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CCriticalSection::~CCriticalSection()
{
    DeleteCriticalSection(&cs);

}	// destructer


/************************************************************************
*    desc:  Lock other threads from access                                                             
************************************************************************/
void CCriticalSection::Lock()
{
    EnterCriticalSection(&cs);

}	// Lock


/************************************************************************
*    desc:  Unlock so other threads can have access                                                             
************************************************************************/
void CCriticalSection::Unlock()
{
    LeaveCriticalSection(&cs);

}	// Unlock





