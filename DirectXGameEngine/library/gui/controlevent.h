/************************************************************************
*    FILE NAME:       controlevent.h
*
*    DESCRIPTION:     Event related classes
************************************************************************/

#ifndef __control_event_h__
#define __control_event_h__

// Boost lib dependencies
#include <boost/function.hpp>

// Game lib dependencies
#include <misc/eventapi.h>
#include <gui/uicontrol.h>
#include <gui/uimenu.h>

/************************************************************************
*    DESCRIPTION:     Class for ui control action events
************************************************************************/
class CControlActionEvent
{
public:

    // Define the callback for this event
    typedef boost::function<void (CUIControl *)> CallbackType;

    // Dispatch the message
    void Dispatch( CUIControl * pControl )
    {
        // Get the vector of callbacks
        const std::vector<CallbackType> & callbackVec = 
            CPublisher<CControlActionEvent, CControlActionEvent::CallbackType>::Instance().GetCallbackVec();

        // Call all the callbacks
        for( size_t i = 0; i < callbackVec.size(); ++i )
            callbackVec[i](pControl);
    }
};


/************************************************************************
*    DESCRIPTION:     Class for smart gui init events
************************************************************************/
class CSmartGuiCreateEvent
{
public:

    // Define the callback for this event
    typedef boost::function<void (CUIControl *, CUIMenu *)> CallbackType;

    // Dispatch the message
    void Dispatch( CUIControl * pControl )
    {
        // Get the vector of callbacks
        const std::vector<CallbackType> & callbackVec = 
            CPublisher<CSmartGuiCreateEvent, CSmartGuiCreateEvent::CallbackType>::Instance().GetCallbackVec();

        // Call all the callbacks
        for( size_t i = 0; i < callbackVec.size(); ++i )
            callbackVec[i](pControl, NULL);
    }

    // Dispatch the message
    void Dispatch( CUIMenu * pMenu )
    {
        // Get the vector of callbacks
        const std::vector<CallbackType> & callbackVec = 
            CPublisher<CSmartGuiCreateEvent, CSmartGuiCreateEvent::CallbackType>::Instance().GetCallbackVec();

        // Call all the callbacks
        for( size_t i = 0; i < callbackVec.size(); ++i )
            callbackVec[i](NULL, pMenu);
    }
};


#endif  // __control_event_h__
