/************************************************************************
*    FILE NAME:       eventapi.h
*
*    DESCRIPTION:     Event related classes
************************************************************************/

#ifndef __eventapi_h__
#define __eventapi_h__

// Standard lib dependencies
#include <vector>


/************************************************************************
*    DESCRIPTION:     Publisher event class
************************************************************************/
template <class event_type, class callback_type>
class CPublisher
{
public:

    // Get the instance
    static CPublisher<event_type, callback_type> & Instance()
    {
        static CPublisher<event_type, callback_type> publisher;
        return publisher;
    }

    // Get the call bavk vector
    const std::vector<callback_type> & GetCallbackVec() const
    {
        return callbackVec;
    }

    // Subscribe to this publisher
    void Subscribe(void * pVoid, callback_type callback)
    {
        if( find(voidPointerVec.begin(), voidPointerVec.end(), pVoid) ==
            voidPointerVec.end() )
        {
            callbackVec.push_back(callback);
            voidPointerVec.push_back(pVoid);
        }
    }

    // Unsubscribe to this publisher
    void Unsubscribe(void * pVoid)
    {
        for( size_t i = 0; i < voidPointerVec.size(); ++i )
        {
            // Can't compair call backs so we are compairing void pointers
            if( pVoid == voidPointerVec[i] )
            {
                callbackVec.erase( callbackVec.begin() + i );
                voidPointerVec.erase( voidPointerVec.begin() + i );
                break;
            }
        }
    }

private:

    CPublisher(){};
    ~CPublisher(){};

    // Call back vector list
    std::vector<callback_type> callbackVec;

    // Void pointer for comparison list
    std::vector<void *> voidPointerVec;
};


/************************************************************************
*    DESCRIPTION:     Subscriber event class
************************************************************************/
template <class event_type, class callback_type>
class CSubscriber
{
public:

    // Constructor
    CSubscriber(callback_type callbackType)
    {
        callback = callbackType;
    }

    // Subscribe to this publisher
    void Subscribe()
    {
        CPublisher<event_type, callback_type>::Instance().Subscribe(this, callback);
    }

    // Unsubscribe to this publisher
    void Unsubscribe()
    {
        CPublisher<event_type, callback_type>::Instance().Unsubscribe(this);
    }

    // Store the call back
    callback_type callback;
};

#endif  // __eventapi_h__
