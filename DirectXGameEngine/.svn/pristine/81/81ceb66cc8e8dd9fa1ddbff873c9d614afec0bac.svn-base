/************************************************************************
*    FILE NAME:       shaderevent.h
*
*    DESCRIPTION:     Event related classes
************************************************************************/

#ifndef __shader_event_h__
#define __shader_event_h__

// DirectX lib dependencies
#include <d3dx9effect.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <boost/function.hpp>
#include <misc/eventapi.h>

// Forward declaration(s)
class CEffectData;

/************************************************************************
*    DESCRIPTION:     Class for shader init events
************************************************************************/
class CShaderInitEvent
{
public:

    // Define the callback for this event
    typedef boost::function<void (CEffectData *)> CallbackType;

    // Dispatch the message
    void Dispatch( CEffectData * pEffectData )
    {
        // Get the vector of callbacks
        const std::vector<CallbackType> & callbackVec = 
            CPublisher<CShaderInitEvent, CShaderInitEvent::CallbackType>::Instance().GetCallbackVec();

        // Call all the callbacks
        for( size_t i = 0; i < callbackVec.size(); ++i )
            callbackVec[i](pEffectData);
    }
};

#endif  // __shader_event_h__
