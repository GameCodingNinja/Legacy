
/************************************************************************
*    FILE NAME:       thread.h
*
*    DESCRIPTION:     CThread Class
************************************************************************/

#ifndef __thread_h__
#define __thread_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <utilities/exceptionhandling.h>

namespace thread
{
    // Use an odd number for the exit code so that it can
    // be spotted in the debug window when the thread exits
    enum{ THREAD_EXIT_CODE = 12345 };

    template<class T>
    class CThread : public boost::noncopyable
    {
        // new type Method: pointer to a object's method (this call)
        typedef DWORD (T::* Method)();

    public:

        // Creates a new Thread object. object: the one which method should be
        // executed. method: pointer to the object's method.
        CThread() : hThread(NULL), threadID(0)
        { }

        // Destructor
        ~CThread(void)
        { Reset(); }

        // Reset to run again
        void Reset()
        {
            threadID = 0;
            NDelFunc::CloseWinHandle(hThread);
        }

        // Starts executing the objects method in a concurrent thread.
        void Start(T* _object, DWORD ( T::* _method)())
        {
            if( Running() )
                throw NExcept::CCriticalException("Thread Create Error!",
                    "Can't start because thread al ready running!");

            object = _object;
            method = _method;

            Reset();

            // Create the thread
            hThread = CreateThread( NULL,
                                    0,
                                    (LPTHREAD_START_ROUTINE) CThread<T>::run,
                                    this,
                                    0,
                                    &threadID );

            if( hThread == NULL )
                throw NExcept::CCriticalException("Thread Create Error!",
                    "Failed to create thread!");
        }

        // Blocks the calling thread until this thread has stopped.
        void Join()
        {
            WaitForSingleObject(hThread, INFINITE);
        }

        // Has the thread started
        bool Started()
        {
            if( threadID > 0 )
                return true;
            
            return false;
        }

        // Is the thread running
        bool Running()
        {
            if( hThread != NULL )
            {
                DWORD exitCode = 0;
                GetExitCodeThread(hThread, &exitCode);

                if( exitCode == STILL_ACTIVE )
                    return true;

                else if( exitCode == THREAD_EXIT_CODE )
                    return false;
            }
            
            return false;
        }

        // Has thread finished
        bool Finished()
        {
            return (Started() && !Running());
        }

        // Get the thread ID
        DWORD GetId()
        {
            return threadID;
        }

        // Get the thread ID
        void Terminate()
        {
            if( threadID != NULL )
                TerminateThread( threadID, THREAD_EXIT_CODE );
        }

    private:

        // unique handle to the thread
        HANDLE hThread;

        // thread id - 0 until started
        DWORD threadID;

        // the object which owns the method
        T * object;   

        // the method of the object
        Method method;

        // flag to indicate the thread was stated
        bool started;

    private:

        // This function gets executed by a concurrent thread.
        static DWORD run(LPVOID thread_obj)
        {
            CThread<T>* thread = (CThread<T>*)thread_obj;
            return (thread->object->*thread->method)();
        }

    };

}	// namespace thread

#endif  // __thread_h__


